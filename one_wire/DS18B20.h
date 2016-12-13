/*
**
**
** File: DS18B20.h
**
**
**
**
*/

#ifndef DS18B20_H_
#define DS18B20_H_

#include <project.h>
 
typedef uint8 ( *PIN_READ )(void );
typedef void  ( *PIN_WRITE )( uint8 value );

#define DS1820_ADDR_LEN       8

typedef const struct 
{
    PIN_READ pin_read;
    PIN_WRITE pin_write;
}ONE_WIRE_IO;

typedef struct
{
    uint8 nRomAddr_au8[DS1820_ADDR_LEN];
}ONE_WIRE_ADDRESS;


uint8 DS1820_Reset(ONE_WIRE_IO *io );

/*******************************************************************************
 * FUNCTION:   DS1820_ReadBit
 * PURPOSE:    Reads a single bit from the DS1820 device.
 *
 * INPUT:      -
 * OUTPUT:     -
 * RETURN:     bool        value of the bit which as been read form the DS1820
 ******************************************************************************/
uint8 DS1820_ReadBit(ONE_WIRE_IO *io);



/*******************************************************************************
 * FUNCTION:   DS1820_WriteBit
 * PURPOSE:    Writes a single bit to the DS1820 device.
 *
 * INPUT:      bBit        value of bit to be written
 * OUTPUT:     -
 * RETURN:     -
 ******************************************************************************/
void DS1820_WriteBit(ONE_WIRE_IO *io, uint8 bBit);


/*******************************************************************************
 * FUNCTION:   DS1820_ReadByte
 * PURPOSE:    Reads a single byte from the DS1820 device.
 *
 * INPUT:      -
 * OUTPUT:     -
 * RETURN:     uint8          byte which has been read from the DS1820
 ******************************************************************************/
uint8 DS1820_ReadByte(ONE_WIRE_IO *io);

/*******************************************************************************
 * FUNCTION:   DS1820_WriteByte
 * PURPOSE:    Writes a single byte to the DS1820 device.
 *
 * INPUT:      val_u8         byte to be written
 * OUTPUT:     -
 * RETURN:     -
 ******************************************************************************/
void DS1820_WriteByte(ONE_WIRE_IO *io, uint8 val_u8);




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
void DS1820_AddrDevice(ONE_WIRE_IO *io,ONE_WIRE_ADDRESS *addr);





/*******************************************************************************
 * FUNCTION:   DS1820_FindFirstDevice
 * PURPOSE:    Starts the device search on the 1-wire bus.
 *
 * INPUT:      -
 * OUTPUT:     nRomAddr_au8[]       ROM code of the first device
 * RETURN:     bool                 TRUE if there are more devices on the 1-wire
 *                                  bus, FALSE otherwise
 ******************************************************************************/
uint8 DS1820_FindFirstDevice(ONE_WIRE_IO *io, ONE_WIRE_ADDRESS *addr);



void DS1820_StartTemp_Reading(ONE_WIRE_IO *io, ONE_WIRE_ADDRESS *addr);

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
int16 DS1820_GetTempRaw(ONE_WIRE_IO *io, ONE_WIRE_ADDRESS *addr);




#endif