   /*** FILEHEADER ****************************************************************
 *
 *    FILENAME:    ds1820.h
 *    DATE:        25.02.2005
 *    AUTHOR:      Christian Stadler
 *
 *    crc and search code ported from  PaulStoffregen/OneWire/OneWire.cpp (audrino code )
 *    DESCRIPTION: Driver for DS1820 1-Wire Temperature sensor (Dallas)
 *    Modified by Glenn Edgar for CYPRESS Products
 *
 ******************************************************************************/



#include <project.h>
#include "DS18B20.h"


#define TEMP_RES              0x100 /* temperature resolution => 1/256°C = 0.0039°C */


/* -------------------------------------------------------------------------- */
/*                         DS1820 Timing Parameters                           */
/* -------------------------------------------------------------------------- */

#define DS1820_RST_PULSE       480   /* master reset pulse time in [us] */
#define DS1820_MSTR_BITSTART   2     /* delay time for bit start by master */
#define DS1820_PRESENCE_WAIT   40    /* delay after master reset pulse in [us] */
#define DS1820_PRESENCE_FIN    480   /* dealy after reading of presence pulse [us] */
#define DS1820_BITREAD_DLY     5     /* bit read delay */
#define DS1820_BITWRITE_DLY    100   /* bit write delay */


/* -------------------------------------------------------------------------- */
/*                            DS1820 Registers                                */
/* -------------------------------------------------------------------------- */

#define DS1820_REG_TEMPLSB    0
#define DS1820_REG_TEMPMSB    1
#define DS1820_REG_CNTREMAIN  6
#define DS1820_REG_CNTPERSEC  7
#define DS1820_SCRPADMEM_LEN  9     /* length of scratchpad memory */




/* -------------------------------------------------------------------------- */
/*                            DS1820 Commands                                 */
/* -------------------------------------------------------------------------- */

#define DS1820_CMD_SEARCHROM     0xF0
#define DS1820_CMD_READROM       0x33
#define DS1820_CMD_MATCHROM      0x55
#define DS1820_CMD_SKIPROM       0xCC
#define DS1820_CMD_ALARMSEARCH   0xEC
#define DS1820_CMD_CONVERTTEMP   0x44
#define DS1820_CMD_WRITESCRPAD   0x4E
#define DS1820_CMD_READSCRPAD    0xBE
#define DS1820_CMD_COPYSCRPAD    0x48
#define DS1820_CMD_RECALLEE      0xB8


#define DS1820_FAMILY_CODE_DS18B20      0x28
#define DS1820_FAMILY_CODE_DS18S20      0x10


/* -------------------------------------------------------------------------- */
/*                            static variables                                */
/* -------------------------------------------------------------------------- */

static uint8 bDoneFlag;
static uint8 nLastDiscrepancy_u8;










static uint8 compute_crc8(const uint8 *addr, uint8 len);







/*******************************************************************************
 * FUNCTION:   DS1820_Reset
 * PURPOSE:    Initializes the DS1820 device.
 *
 * INPUT:      -
 * OUTPUT:     -
 * RETURN:     FALSE if at least one device is on the 1-wire bus, TRUE otherwise
 ******************************************************************************/
uint8 DS1820_Reset(ONE_WIRE_IO *io)
{
   uint8 bPresPulse;
   uint8  criticalState;
   criticalState = CyEnterCriticalSection();
   
   /* reset pulse */
   io->pin_write(0);
   CyDelayUs(DS1820_RST_PULSE);
   io->pin_write(1);

   /* wait until pullup pull 1-wire bus to high */
   CyDelayUs(DS1820_PRESENCE_WAIT);

   /* get presence pulse */
   bPresPulse = io->pin_read();

    CyDelayUs(424);
   
   CyExitCriticalSection(criticalState);

   return bPresPulse;
}


/*******************************************************************************
 * FUNCTION:   DS1820_ReadBit
 * PURPOSE:    Reads a single bit from the DS1820 device.
 *
 * INPUT:      -
 * OUTPUT:     -
 * RETURN:     bool        value of the bit which as been read form the DS1820
 ******************************************************************************/
uint8 DS1820_ReadBit(ONE_WIRE_IO *io)
{
   uint8 bBit;

    uint8  criticalState;
    criticalState = CyEnterCriticalSection();

   
   io->pin_write(0);
   CyDelayUs(DS1820_MSTR_BITSTART);
   io->pin_write(1);
   CyDelayUs(DS1820_BITREAD_DLY);

   bBit = io->pin_read();
   
   CyExitCriticalSection(criticalState);

   return (bBit);
}


/*******************************************************************************
 * FUNCTION:   DS1820_WriteBit
 * PURPOSE:    Writes a single bit to the DS1820 device.
 *
 * INPUT:      bBit        value of bit to be written
 * OUTPUT:     -
 * RETURN:     -
 ******************************************************************************/
void DS1820_WriteBit(ONE_WIRE_IO *io,uint8 bBit)
{
    uint8  criticalState;
    criticalState = CyEnterCriticalSection();
    
   io-> pin_write(0);
   CyDelayUs(DS1820_MSTR_BITSTART);

   if (bBit != 0)  // False
   {
      io->pin_write(1);
   }

   CyDelayUs(DS1820_BITWRITE_DLY);
   io->pin_write(1);
   
   CyExitCriticalSection(criticalState);
}


/*******************************************************************************
 * FUNCTION:   DS1820_ReadByte
 * PURPOSE:    Reads a single byte from the DS1820 device.
 *
 * INPUT:      -
 * OUTPUT:     -
 * RETURN:     uint8          byte which has been read from the DS1820
 ******************************************************************************/
uint8 DS1820_ReadByte(ONE_WIRE_IO *io)
{
   uint8 i;
   uint8 value = 0;

   for (i=0 ; i < 8; i++)
   {
      if ( DS1820_ReadBit(io) )
      {
         value |= (1 << i);
      }
      CyDelayUs(120);
   }
   return(value);
}


/*******************************************************************************
 * FUNCTION:   DS1820_WriteByte
 * PURPOSE:    Writes a single byte to the DS1820 device.
 *
 * INPUT:      val_u8         byte to be written
 * OUTPUT:     -
 * RETURN:     -
 ******************************************************************************/
void DS1820_WriteByte(ONE_WIRE_IO *io,uint8 val_u8)
{
   uint8 i;
   uint8 temp;

   for (i=0; i < 8; i++)      /* writes byte, one bit at a time */
   {
      temp = val_u8 >> i;     /* shifts val right 'i' spaces */
      temp &= 0x01;           /* copy that bit to temp */
      DS1820_WriteBit(io,temp);  /* write bit in temp into */
   }

   CyDelayUs(105);
}



/* -------------------------------------------------------------------------- */
/*                             API Interface                                  */
/* -------------------------------------------------------------------------- */


/*******************************************************************************
 * FUNCTION:   DS1820_AddrDevice
 * PURPOSE:    Addresses a single or all devices on the 1-wire bus.
 *
 * INPUT:      nAddrMethod       use DS1820_CMD_MATCHROM to select a single
 *                               device or DS1820_CMD_SKIPROM to select all
 * OUTPUT:     -
 * RETURN:     -
 ******************************************************************************/
void DS1820_AddrDevice(ONE_WIRE_IO *io,ONE_WIRE_ADDRESS *addr)
{
   uint8 i;
   
      DS1820_WriteByte(io,DS1820_CMD_MATCHROM);     /* address single devices on bus */
      for (i = 0; i < DS1820_ADDR_LEN; i ++)
      {
         DS1820_WriteByte(io,addr->nRomAddr_au8[i]);
      }
 
}


/*******************************************************************************
 * FUNCTION:   DS1820_FindNextDevice
 * PURPOSE:    Finds next device connected to the 1-wire bus.
 *
 * INPUT:      -
 * OUTPUT:     nRomAddr_au8[]       ROM code of the next device
 * RETURN:     bool                 TRUE if there are more devices on the 1-wire
 *                                  bus, FALSE otherwise
 ******************************************************************************/
uint8 DS1820_FindNextDevice(ONE_WIRE_IO *io,ONE_WIRE_ADDRESS *addr)
{
    uint8 state_u8;
    uint8 byteidx_u8;
    uint8 mask_u8 = 1;
    uint8 bitpos_u8 = 1;
    uint8 nDiscrepancyMarker_u8 = 0;
    uint8 bit_b;
    uint8 bStatus;
    uint8 next_b = 0; //FALSE;
    uint8 crc8;

    /* init ROM address */
    for (byteidx_u8=0; byteidx_u8 < 8; byteidx_u8 ++)
    {
        addr->nRomAddr_au8[byteidx_u8] = 0x00;
    }

    bStatus = DS1820_Reset(io);        /* reset the 1-wire */

    if (bStatus || bDoneFlag)        /* no device found */
    {
        nLastDiscrepancy_u8 = 0;     /* reset the search */
        return 0; //FALSE;
    }

    /* send search rom command */
    DS1820_WriteByte(io,DS1820_CMD_SEARCHROM);

    byteidx_u8 = 0;
    do
    {
        state_u8 = 0;

        /* read bit */
        if ( DS1820_ReadBit(io) != 0 )
        {
            state_u8 = 2;
        }
        CyDelayUs(120);

        /* read bit complement */
        if ( DS1820_ReadBit(io) != 0 )
        {
            state_u8 |= 1;
        }
        CyDelayUs(120);

        /* description for values of state_u8: */
        /* 00    There are devices connected to the bus which have conflicting */
        /*       bits in the current ROM code bit position. */
        /* 01    All devices connected to the bus have a 0 in this bit position. */
        /* 10    All devices connected to the bus have a 1 in this bit position. */
        /* 11    There are no devices connected to the 1-wire bus. */

        /* if there are no devices on the bus */
        if (state_u8 == 3)
        {
            break;
        }
        else
        {
            /* devices have the same logical value at this position */
            if (state_u8 > 0)
            {
                /* get bit value */
                bit_b = (uint8)(state_u8 >> 1);
            }
            /* devices have confilcting bits in the current ROM code */
            else
            {
                /* if there was a conflict on the last iteration */
                if (bitpos_u8 < nLastDiscrepancy_u8)
                {
                    /* take same bit as in last iteration */
                    bit_b = ( (addr->nRomAddr_au8[byteidx_u8] & mask_u8) > 0 );
                }
                else
                {
                    bit_b = (bitpos_u8 == nLastDiscrepancy_u8);
                }

                if (bit_b == 0)
                {
                    nDiscrepancyMarker_u8 = bitpos_u8;
                }
            }

            /* store bit in ROM address */
           if (bit_b != 0)
           {
               addr->nRomAddr_au8[byteidx_u8] |= mask_u8;
           }
           else
           {
               addr->nRomAddr_au8[byteidx_u8] &= ~mask_u8;
           }

           DS1820_WriteBit(io,bit_b);

           /* increment bit position */
           bitpos_u8 ++;

           /* calculate next mask value */
           mask_u8 = mask_u8 << 1;

           /* check if this byte has finished */
           if (mask_u8 == 0)
           {
               byteidx_u8 ++;  /* advance to next byte of ROM mask */
               mask_u8 = 1;    /* update mask */
           }
        }
    } while (byteidx_u8 < DS1820_ADDR_LEN);


    /* if search was unsuccessful then */
    if (bitpos_u8 < 65)
    {
        /* reset the last discrepancy to 0 */
        nLastDiscrepancy_u8 = 0;
    }
    else
    {
        /* search was successful */
        nLastDiscrepancy_u8 = nDiscrepancyMarker_u8;
        bDoneFlag = (nLastDiscrepancy_u8 == 0);

        /* indicates search is not complete yet, more parts remain */
        next_b = 1 ; //TRUE;
        // lets check crc on rom code
        crc8 = compute_crc8(&addr->nRomAddr_au8[0], 7);
        if( crc8 == addr->nRomAddr_au8[7] )
        {
            next_b = 1; // crc passed
        }
        else
        {
            next_b = 0;  // crc failed
        }
    }

    return next_b;
}


/*******************************************************************************
 * FUNCTION:   DS1820_FindFirstDevice
 * PURPOSE:    Starts the device search on the 1-wire bus.
 *
 * INPUT:      -
 * OUTPUT:     nRomAddr_au8[]       ROM code of the first device
 * RETURN:     bool                 TRUE if there are more devices on the 1-wire
 *                                  bus, FALSE otherwise
 ******************************************************************************/
uint8 DS1820_FindFirstDevice(ONE_WIRE_IO *io,ONE_WIRE_ADDRESS *addr)
{
    nLastDiscrepancy_u8 = 0;
    bDoneFlag = 0; //FALSE;

    return ( DS1820_FindNextDevice(io,addr) );
}



/*******************************************************************************
 * FUNCTION:   DS1820_GetTempRaw
 * PURPOSE:    Get temperature raw value from single DS1820 device.
 *
 *             Scratchpad Memory Layout
 *             Byte  Register
 *             0     Temperature_LSB
 *             1     Temperature_MSB
 *             2     Temp Alarm High / User Byte 1
 *             3     Temp Alarm Low / User Byte 2
 *             4     Reserved
 *             5     Reserved
 *             6     Count_Remain
 *             7     Count_per_C
 *             8     CRC
 *
 *             Temperature calculation for DS18S20 (Family Code 0x10):
 *             =======================================================
 *                                             (Count_per_C - Count_Remain)
 *             Temperature = temp_raw - 0.25 + ----------------------------
 *                                                     Count_per_C
 *
 *             Where temp_raw is the value from the temp_MSB and temp_LSB with
 *             the least significant bit removed (the 0.5C bit).
 *
 *
 *             Temperature calculation for DS18B20 (Family Code 0x28):
 *             =======================================================
 *                      bit7   bit6   bit5   bit4   bit3   bit2   bit1   bit0
 *             LSB      2^3    2^2    2^1    2^0    2^-1   2^-2   2^-3   2^-4
 *                      bit15  bit14  bit13  bit12  bit3   bit2   bit1   bit0
 *             MSB      S      S      S      S      S      2^6    2^5    2^4
 *
 *             The temperature data is stored as a 16-bit sign-extended two’s
 *             complement number in the temperature register. The sign bits (S)
 *             indicate if the temperature is positive or negative: for
 *             positive numbers S = 0 and for negative numbers S = 1.
 *
 * RETURN:     sint16         raw temperature value with a resolution
 *                            of 1/256°C
 ******************************************************************************/
void DS1820_StartTemp_Reading(ONE_WIRE_IO *io,ONE_WIRE_ADDRESS *addr)
{

    /* --- start temperature conversion -------------------------------------- */
    DS1820_Reset(io);
    DS1820_AddrDevice(io,addr);     /* address the device */
    io->pin_write(1);
    DS1820_WriteByte(io,DS1820_CMD_CONVERTTEMP);   /* start conversion */
    //DS1820_DelayMs(DS1820_TEMPCONVERT_DLY);   /* wait for temperature conversion */
 }


// Delay 750 between Colum    CyDelay(750);

int16 DS1820_GetTempRaw(ONE_WIRE_IO *io,ONE_WIRE_ADDRESS *addr)
{
    uint8 i;
    uint16 temp_u16;
    uint16 highres_u16;
    uint8 scrpad[DS1820_SCRPADMEM_LEN];

    /* --- read sratchpad ---------------------------------------------------- */
    DS1820_Reset(io);
    DS1820_AddrDevice(io,addr);   /* address the device */
    DS1820_WriteByte(io,DS1820_CMD_READSCRPAD);  /* read scratch pad */

    /* read scratch pad data */
    for (i=0; i < DS1820_SCRPADMEM_LEN; i++)
    {
        scrpad[i] = DS1820_ReadByte(io);
    }


    /* --- calculate temperature --------------------------------------------- */
    /* Formular for temperature calculation: */
    /* Temp = Temp_read - 0.25 + ((Count_per_C - Count_Remain)/Count_per_C) */

    /* get raw value of temperature (0.5°C resolution) */
    temp_u16 = 0;
    temp_u16 = (uint16)((uint16)scrpad[DS1820_REG_TEMPMSB] << 8);
    temp_u16 |= (uint16)(scrpad[DS1820_REG_TEMPLSB]);

    if (addr->nRomAddr_au8[0] == DS1820_FAMILY_CODE_DS18S20)
    {
        /* get temperature value in 1°C resolution */
        temp_u16 >>= 1;
    
        /* temperature resolution is TEMP_RES (0x100), so 1°C equals 0x100 */
        /* => convert to temperature to 1/256°C resolution */
        temp_u16 = ((uint16)temp_u16 << 8);
    
        /* now substract 0.25°C */
        temp_u16 -= ((uint16)TEMP_RES >> 2);
    
        /* now calculate high resolution */
        highres_u16 = scrpad[DS1820_REG_CNTPERSEC] - scrpad[DS1820_REG_CNTREMAIN];
        highres_u16 = ((uint16)highres_u16 << 8);
        if (scrpad[DS1820_REG_CNTPERSEC])
        {
            highres_u16 = highres_u16 / (uint16)scrpad[DS1820_REG_CNTPERSEC];
        }
    
        /* now calculate result */
        highres_u16 = highres_u16 + temp_u16;
    }
    else
    {
        /* 12 bit temperature value has 0.0625°C resolution */
        /* shift left by 4 to get 1/256°C resolution */
        highres_u16 = temp_u16;
        highres_u16 <<= 4;
    }

    return (highres_u16);
}
static const uint8 crc_table[] = {
      0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
     35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
    190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
     70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
    219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
     17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
     50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
     87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

//
// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (note: this might better be done without to
// table, it would probably be smaller and certainly fast enough
// compared to all those delayMicrosecond() calls.  But I got
// confused, so I use this table from the examples.)
//
static uint8 compute_crc8(const uint8 *addr, uint8 len)
{
	uint8 crc = 0;

	while (len--) {
		crc = crc_table[(crc ^ *addr++)];
	}
	return crc;
}
#if 0
    
    
// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
//
void OneWire::target_search(uint8_t family_code)
{
   // set the search state to find SearchFamily type devices
   ROM_NO[0] = family_code;
   for (uint8_t i = 1; i < 8; i++)
      ROM_NO[i] = 0;
   LastDiscrepancy = 64;
   LastFamilyDiscrepancy = 0;
   LastDeviceFlag = FALSE;
}

//
// Perform a search. If this function returns a '1' then it has
// enumerated the next device and you may retrieve the ROM from the
// OneWire::address variable. If there are no devices, no further
// devices, or something horrible happens in the middle of the
// enumeration then a 0 is returned.  If a new device is found then
// its address is copied to newAddr.  Use OneWire::reset_search() to
// start over.
//
// --- Replaced by the one from the Dallas Semiconductor web site ---
//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
uint8_t OneWire::search(uint8_t *newAddr, bool search_mode /* = true */)
{
   uint8_t id_bit_number;
   uint8_t last_zero, rom_byte_number, search_result;
   uint8_t id_bit, cmp_id_bit;

   unsigned char rom_byte_mask, search_direction;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;

   // if the last call was not the last one
   if (!LastDeviceFlag)
   {
      // 1-Wire reset
      if (!reset())
      {
         // reset the search
         LastDiscrepancy = 0;
         LastDeviceFlag = FALSE;
         LastFamilyDiscrepancy = 0;
         return FALSE;
      }

      // issue the search command
      if (search_mode == true) {
        write(0xF0);   // NORMAL SEARCH
      } else {
        write(0xEC);   // CONDITIONAL SEARCH
      }

      // loop to do the search
      do
      {
         // read a bit and its complement
         id_bit = read_bit();
         cmp_id_bit = read_bit();

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1))
            break;
         else
         {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit)
               search_direction = id_bit;  // bit write value for search
            else
            {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < LastDiscrepancy)
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               else
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == LastDiscrepancy);

               // if 0 was picked then record its position in LastZero
               if (search_direction == 0)
               {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            write_bit(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0)
            {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!(id_bit_number < 65))
      {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         LastDiscrepancy = last_zero;

         // check for last device
         if (LastDiscrepancy == 0)
            LastDeviceFlag = TRUE;

         search_result = TRUE;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ROM_NO[0])
   {
      LastDiscrepancy = 0;
      LastDeviceFlag = FALSE;
      LastFamilyDiscrepancy = 0;
      search_result = FALSE;
   } else {
      for (int i = 0; i < 8; i++) newAddr[i] = ROM_NO[i];
   }
   return search_result;
  }



// The 1-Wire CRC scheme is described in Maxim Application Note 27:
// "Understanding and Using Cyclic Redundancy Checks with Maxim iButton Products"
//


// This table comes from Dallas sample code where it is freely reusable,
// though Copyright (C) 2000 Dallas Semiconductor Corporation
static const uint8_t PROGMEM dscrc_table[] = {
      0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
     35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
    190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
     70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
    219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
     17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
     50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
     87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

//
// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (note: this might better be done without to
// table, it would probably be smaller and certainly fast enough
// compared to all those delayMicrosecond() calls.  But I got
// confused, so I use this table from the examples.)
//
uint8_t OneWire::crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		crc = pgm_read_byte(dscrc_table + (crc ^ *addr++));
	}
	return crc;
}
#endif