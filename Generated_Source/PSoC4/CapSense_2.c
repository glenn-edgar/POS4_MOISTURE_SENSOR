/*******************************************************************************
* File Name: CapSense_2.c
* Version 2.40
*
* Description:
*  This file provides the source code for scanning APIs for the CapSense CSD
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_2.h"
#include "CapSense_2_PVT.h"
#include "cypins.h"
#include "cyapicallbacks.h"

#if(0u != CapSense_2_CSHL_API_GENERATE)
    #include "CapSense_2_CSHL.h"
#endif /* (0u != CapSense_2_CSHL_API_GENERATE) */

/* This definition is intended to prevent unexpected linker error.
   For more details please see the IAR Technical Note 49981 */
#if defined(__ICCARM__)
    extern void CapSense_2_EnableSensor(uint32 sensor);
    extern void CapSense_2_DisableSensor(uint32 sensor);
#endif /* (__ICCARM__) */

/* SmartSense functions */
#if (CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO)
    uint8 CapSense_2_lowLevelTuningDone = 0u;
    uint8 CapSense_2_scanSpeedTbl[((CapSense_2_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
#endif /* (CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) */

#if(CapSense_2_PRS_OPTIONS != CapSense_2__PRS_NONE)
    uint8 CapSense_2_prescalersTuningDone = 0u;
#endif /* (CapSense_2_PRS_OPTIONS == CapSense_2__PRS_NONE) */

/* Global software variables */
volatile uint8 CapSense_2_csdStatusVar = 0u;   /* CapSense CSD status, variable */
volatile uint8 CapSense_2_sensorIndex = 0u;    /* Index of scannig sensor */

/* Global array of Raw Counts */
uint16 CapSense_2_sensorRaw[CapSense_2_TOTAL_SENSOR_COUNT] = {0u};

/* Backup variables for trim registers*/
#if (CapSense_2_IDAC1_POLARITY == CapSense_2__IDAC_SINK)
    uint8 CapSense_2_csdTrim2;
#else
    uint8 CapSense_2_csdTrim1;
#endif /* (CapSense_2_IDAC1_POLARITY == CapSense_2__IDAC_SINK) */

/* Global array of un-scanned sensors state */
uint8 CapSense_2_unscannedSnsDriveMode[CapSense_2_PORT_PIN_CONFIG_TBL_ZISE];

/* Backup array for CapSense_2_sensorEnableMask */
uint8 CapSense_2_sensorEnableMaskBackup[(((CapSense_2_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];

/* Configured constants and arrays by Customizer */
uint8 CapSense_2_sensorEnableMask[(((CapSense_2_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)] = {
0x1u, };

reg32 * CapSense_2_pcTable[] = {
    (reg32 *)CapSense_2_Sns__Generic0_0__GEN__PC, 
};

const uint8 CapSense_2_portTable[] = {
    CapSense_2_Sns__Generic0_0__GEN__PORT, 
};

const uint32 CapSense_2_maskTable[] = {
    CapSense_2_Sns__Generic0_0__GEN__MASK, 
};

const uint8 CapSense_2_pinShiftTbl[] = {
    (uint8) CapSense_2_Sns__Generic0_0__GEN__SHIFT, 
};

const uint8 CapSense_2_modulationIDAC[CapSense_2_TOTAL_SENSOR_COUNT] = {
    80u, 
};
const uint8 CapSense_2_compensationIDAC[CapSense_2_TOTAL_SENSOR_COUNT] = {
    80u, 
};

const uint32 CapSense_2_widgetResolution[CapSense_2_RESOLUTIONS_TBL_SIZE] = {
    CapSense_2_RESOLUTION_16_BITS,
};

const uint8 CapSense_2_senseClkDividerVal[CapSense_2_TOTAL_SCANSLOT_COUNT] = {
    2u, 
};
const uint8 CapSense_2_sampleClkDividerVal[CapSense_2_TOTAL_SCANSLOT_COUNT] = {
    2u, 
};


const uint8 CapSense_2_widgetNumber[CapSense_2_TOTAL_SENSOR_COUNT] = {
    0u, /* Generic0__GEN */
};

reg32* const CapSense_2_prtSelTbl[CapSense_2_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_HSIOM_PORT_SEL0),
    ((reg32 *) CYREG_HSIOM_PORT_SEL1),
    ((reg32 *) CYREG_HSIOM_PORT_SEL2),
    ((reg32 *) CYREG_HSIOM_PORT_SEL3),
    ((reg32 *) CYREG_HSIOM_PORT_SEL4),
    ((reg32 *) CYREG_HSIOM_PORT_SEL5),
    ((reg32 *) CYREG_HSIOM_PORT_SEL6),
    ((reg32 *) CYREG_HSIOM_PORT_SEL7),
};

reg32* const CapSense_2_prtCfgTbl[CapSense_2_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_GPIO_PRT0_PC),
    ((reg32 *) CYREG_GPIO_PRT1_PC),
    ((reg32 *) CYREG_GPIO_PRT2_PC),
    ((reg32 *) CYREG_GPIO_PRT3_PC),
    ((reg32 *) CYREG_GPIO_PRT4_PC),
    ((reg32 *) CYREG_GPIO_PRT5_PC),
    ((reg32 *) CYREG_GPIO_PRT6_PC),
    ((reg32 *) CYREG_GPIO_PRT7_PC),
};

reg32* const CapSense_2_prtDRTbl[CapSense_2_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_GPIO_PRT0_DR),
    ((reg32 *) CYREG_GPIO_PRT1_DR),
    ((reg32 *) CYREG_GPIO_PRT2_DR),
    ((reg32 *) CYREG_GPIO_PRT3_DR),
    ((reg32 *) CYREG_GPIO_PRT4_DR),
    ((reg32 *) CYREG_GPIO_PRT5_DR),
    ((reg32 *) CYREG_GPIO_PRT6_DR),
    ((reg32 *) CYREG_GPIO_PRT7_DR),
};



/*******************************************************************************
* Function Name: CapSense_2_Init
********************************************************************************
*
* Summary:
*  API initializes default CapSense configuration provided by the customizer that defines
*  the mode of component operations and resets all the sensors to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_immunityIndex - defines immunity level.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_Init(void)
{
    uint32 curSensor;

    #if(CapSense_2_IS_SHIELD_ENABLE)

        #if((CapSense_2_CSH_TANK_PREGARGE_OPTION == CapSense_2__CAPPRIOBUF) || (0u != CapSense_2_CSH_TANK_ENABLE))
            uint32 newRegValue;

            newRegValue = CapSense_2_CTANK_CONNECTION_REG;
            newRegValue &= ~(CapSense_2_CSD_CTANK_CONNECTION_MASK);
            newRegValue |= CapSense_2_CSD_CTANK_TO_AMUXBUS_B;
            CapSense_2_CTANK_CONNECTION_REG = newRegValue;

            #if(CapSense_2_CSH_TANK_PREGARGE_OPTION == CapSense_2__CAPPRIOBUF)
                newRegValue = CapSense_2_CTANK_PORT_PC_REG;
                newRegValue &= ~(CapSense_2_CSD_CTANK_PC_MASK);
                newRegValue |= CapSense_2_CTANK_PC_STRONG_MODE;
                CapSense_2_CTANK_PORT_PC_REG = newRegValue;

                newRegValue = CapSense_2_CTANK_PORT_DR_REG;
                newRegValue &= ~(CapSense_2_CTANK_DR_MASK);
                newRegValue |= CapSense_2_CTANK_DR_CONFIG;
                CapSense_2_CTANK_PORT_DR_REG = newRegValue;
            #endif /* (CapSense_2_CSH_TANK_PREGARGE_OPTION == CapSense_2__CAPPRIOBUF) */

        #endif /* ((CapSense_2_CSH_TANK_PREGARGE_OPTION == CapSense_2__CAPPRIOBUF) || (CapSense_2_CSH_TANK_ENABLE)) */

        CapSense_2_EnableShieldElectrode((uint32)CapSense_2_SHIELD_PIN_NUMBER, (uint32)CapSense_2_SHIELD_PORT_NUMBER);

    #endif /* (CapSense_2_IS_SHIELD_ENABLE) */

    for(curSensor = 0u; curSensor < CapSense_2_TOTAL_SENSOR_COUNT; curSensor++)
    {
        CapSense_2_SetUnscannedSensorState(curSensor, CapSense_2_CONNECT_INACTIVE_SNS);
    }

    CapSense_2_CsdHwBlockInit();
    CapSense_2_SetShieldDelay(CapSense_2_SHIELD_DELAY );

    /* Clear all sensors */
    CapSense_2_ClearSensors();
}

/*******************************************************************************
* Function Name: CapSense_2_CsdHwBlockInit
********************************************************************************
*
* Summary:
*  Initialises the hardware parameters of the CSD block
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_csdTrim1 - Contains IDAC trim register value for Sourcing Mode.
*  CapSense_2_csdTrim2 - Contains IDAC trim register value for Sinking Mode.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_CsdHwBlockInit(void)
{
    uint32 newRegValue;

        /* Set trim registers for CSD Mode */
    #if (CapSense_2_IDAC1_POLARITY == CapSense_2__IDAC_SINK)
        /* iDAC1 Sinking Mode */
        CapSense_2_csdTrim2 = (uint8)CapSense_2_CSD_TRIM2_REG;
        newRegValue = CapSense_2_csdTrim2;
        newRegValue &= CapSense_2_CSD_IDAC1_TRIM_MASK;
        newRegValue |= (uint32)((uint32)CapSense_2_SFLASH_CSD_TRIM2_REG & (uint32)CapSense_2_CSFLASH_TRIM_IDAC1_MASK);

        #if (CapSense_2_IDAC_CNT == 2u)
            /* iDAC2 Sinking Mode */
            newRegValue &= CapSense_2_CSD_IDAC2_TRIM_MASK;
            newRegValue |= (uint32)((uint32)CapSense_2_SFLASH_CSD_TRIM2_REG & (uint32)CapSense_2_CSFLASH_TRIM_IDAC2_MASK);
        #endif /* (CapSense_2_IDAC_CNT == 2u) */
        CapSense_2_CSD_TRIM2_REG = newRegValue;
    #else
        /* iDAC1 Sourcing Mode */
        CapSense_2_csdTrim1 = (uint8)CapSense_2_CSD_TRIM1_REG;
        newRegValue = CapSense_2_csdTrim1;
        newRegValue &= CapSense_2_CSD_IDAC1_TRIM_MASK;
        newRegValue |= (uint32)((uint32)CapSense_2_SFLASH_CSD_TRIM1_REG & (uint32)CapSense_2_CSFLASH_TRIM_IDAC1_MASK);
        #if (CapSense_2_IDAC_CNT == 2u)
             /* iDAC2 Sourcing Mode */
            newRegValue &= CapSense_2_CSD_IDAC2_TRIM_MASK;
            newRegValue |= (uint32)((uint32)CapSense_2_SFLASH_CSD_TRIM1_REG & (uint32)CapSense_2_CSFLASH_TRIM_IDAC2_MASK);
        #endif
        CapSense_2_CSD_TRIM1_REG = newRegValue;
    #endif /* (CapSense_2_IDAC1_POLARITY == CapSense_2__IDAC_SINK) */

    /* Configure CSD and IDAC */
    #if (CapSense_2_IDAC_CNT > 1u)
        CapSense_2_CSD_IDAC_REG = CapSense_2_DEFAULT_CSD_IDAC_CONFIG;
        CapSense_2_CSD_CFG_REG = CapSense_2_DEFAULT_CSD_CONFIG;
    #else
        CapSense_2_CSD_IDAC_REG &= ~(CapSense_2_CSD_IDAC1_MODE_MASK  | CapSense_2_CSD_IDAC1_DATA_MASK);
        CapSense_2_CSD_IDAC_REG |= CapSense_2_DEFAULT_CSD_IDAC_CONFIG;

        CapSense_2_CSD_CFG_REG &= ~(CapSense_2_CSD_CONFIG_MASK);
        CapSense_2_CSD_CFG_REG |= (CapSense_2_DEFAULT_CSD_CONFIG);
    #endif /* (CapSense_2_IDAC_CNT > 1u) */


    /* Connect Cmod to AMUX bus */
    newRegValue = CapSense_2_CMOD_CONNECTION_REG;
    newRegValue &= ~(CapSense_2_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= CapSense_2_CSD_CMOD_TO_AMUXBUS_A;
    CapSense_2_CMOD_CONNECTION_REG = newRegValue;

    /* Configure Dead Band PWM if it is enabled */
    #if(CapSense_2_CSD_4B_PWM_MODE != CapSense_2__PWM_OFF)
        CapSense_2_CSD_4B_PWM_REG = CapSense_2_DEFAULT_CSD_4B_PWM_CONFIG;
    #endif /* (CapSense_2_CSD_4B_PWM_MODE != CapSense_2__PWM_OFF) */

    /* Setup ISR */
    CyIntDisable(CapSense_2_ISR_NUMBER);
    #if !defined(CY_EXTERNAL_INTERRUPT_CONFIG)
        (void)CyIntSetVector(CapSense_2_ISR_NUMBER, &CapSense_2_ISR);
        CyIntSetPriority(CapSense_2_ISR_NUMBER, CapSense_2_ISR_PRIORITY);
    #endif /* (CY_EXTERNAL_INTERRUPT_CONFIG) */
}


/*******************************************************************************
* Function Name: CapSense_2_Enable
********************************************************************************
*
* Summary:
*  Enables the CSD block and related resources to an active mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_Enable(void)
{
    uint32 newRegValue;

    /* Enable Clocks */
    CapSense_2_SenseClk_Stop();
    CapSense_2_SampleClk_Stop();

    CapSense_2_SampleClk_SetDividerValue((uint16)CapSense_2_INITIAL_CLK_DIVIDER);
    CapSense_2_SenseClk_SetDividerValue((uint16)CapSense_2_INITIAL_CLK_DIVIDER);

    #if (0u == CapSense_2_IS_M0S8PERI_BLOCK)
        CapSense_2_SenseClk_Start();
        CapSense_2_SampleClk_Start();
    #else
        CapSense_2_SampleClk_Start();
        CapSense_2_SenseClk_StartEx(CapSense_2_SampleClk__DIV_ID);
    #endif /* (0u == CapSense_2_IS_M0S8PERI_BLOCK) */

    /* Enable the CSD block */
    newRegValue = CapSense_2_CSD_CFG_REG;
    newRegValue |= (CapSense_2_CSD_CFG_ENABLE | CapSense_2_CSD_CFG_SENSE_COMP_EN
                                                   | CapSense_2_CSD_CFG_SENSE_EN);
    CapSense_2_CSD_CFG_REG = newRegValue;

    /* Enable interrupt */
    CyIntEnable(CapSense_2_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: CapSense_2_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin the component operation. CapSense_Start()
*  calls the CapSense_Init() function, and then calls the CapSense_Enable()
*  function. Initializes the registers and starts the CSD method of the CapSense
*  component. Resets all the sensors to an inactive state. Enables interrupts for
*  sensors scanning. When the SmartSense tuning mode is selected, the tuning procedure
*  is applied for all the sensors. The CapSense_Start() routine must be called before
*  any other API routines.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*   CapSense_2_initVar - used to check the initial configuration,
*   modified on the first function call.
*  CapSense_2_lowLevelTuningDone - Used to notify the Tuner GUI that
*   tuning of the scanning parameters is done.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_Start(void)
{

    static uint8 CapSense_2_initVar = 0u;

    #if((0u != CapSense_2_AUTOCALIBRATION_ENABLE) && (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_AUTO))
        uint32 curSensor;
        uint32 rawLevel;
        uint32 widget;
    #endif /* ((0u != CapSense_2_AUTOCALIBRATION_ENABLE) && (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_AUTO)) */

    if (CapSense_2_initVar == 0u)
    {
        CapSense_2_Init();
        CapSense_2_initVar = 1u;
    }
    CapSense_2_Enable();

    /* AutoTunning start */
    #if(CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO)
        #if(0u != CapSense_2_CSHL_API_GENERATE)
            CapSense_2_AutoTune();
            CapSense_2_lowLevelTuningDone = 1u;
        #endif /* (0u != CapSense_2_CSHL_API_GENERATE) */
    #else
        #if(0u != CapSense_2_AUTOCALIBRATION_ENABLE)

            #if(CapSense_2_IDAC_CNT > 1u)
                CapSense_2_CSD_IDAC_REG &= ~(CapSense_2_CSD_IDAC2_MODE_MASK);
            #endif /* (CapSense_2_IDAC_CNT > 1u) */

            for(curSensor = 0u; curSensor < CapSense_2_TOTAL_SCANSLOT_COUNT; curSensor++)
            {
                widget = CapSense_2_widgetNumber[curSensor];
                rawLevel = CapSense_2_widgetResolution[widget] >> CapSense_2_RESOLUTION_OFFSET;

                /* Calibration level should be equal to 85% from scanning resolution */
                rawLevel = (rawLevel * 85u) / 100u;

                CapSense_2_CalibrateSensor(curSensor, rawLevel, CapSense_2_modulationIDAC);
            }

            #if(0u != CapSense_2_TOTAL_CENTROIDS_COUNT)
                CapSense_2_NormalizeWidgets(CapSense_2_END_OF_WIDGETS_INDEX, CapSense_2_modulationIDAC);
            #endif /* (0u != CapSense_2_TOTAL_CENTROIDS_COUNT)  */

            #if(CapSense_2_IDAC_CNT > 1u)
                for(curSensor = 0u; curSensor < CapSense_2_TOTAL_SCANSLOT_COUNT; curSensor++)
                {
                    CapSense_2_compensationIDAC[curSensor] = CapSense_2_modulationIDAC[curSensor] / 2u;
                    CapSense_2_modulationIDAC[curSensor] = (CapSense_2_modulationIDAC[curSensor] + 1u) / 2u;
                }
                CapSense_2_CSD_IDAC_REG |= CapSense_2_CSD_IDAC2_MODE_FIXED;
            #endif /* (CapSense_2_IDAC_CNT > 1u) */

        #endif /* (0u != CapSense_2_AUTOCALIBRATION_ENABLE) */

    #endif /* ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) */

    /* Initialize Advanced Centroid */
    #if(CapSense_2_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
        CapSense_2_AdvancedCentroidInit();
    #endif /* (CapSense_2_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */

    /* Connect sense comparator input to AMUXA */
    CapSense_2_CSD_CFG_REG |= CapSense_2_CSD_CFG_SENSE_INSEL;
}


/*******************************************************************************
* Function Name: CapSense_2_Stop
********************************************************************************
*
* Summary:
*  Stops the sensor scanning, disables component interrupts, and resets all the
*  sensors to an inactive state. Disables the Active mode power template bits for
*  the subcomponents used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_csdTrim1 - Contains the IDAC trim register value for the Sourcing Mode.
*  CapSense_2_csdTrim2 - Contains the IDAC trim register value for vSinking Mode.
*
* Side Effects:
*  This function should be called after scans are completed.
*
*******************************************************************************/
void CapSense_2_Stop(void)
{
    /* Disable interrupt */
    CyIntDisable(CapSense_2_ISR_NUMBER);

    CapSense_2_CSD_CFG_REG &= ~(CapSense_2_CSD_CFG_SENSE_COMP_EN | CapSense_2_CSD_CFG_SENSE_EN);

    #if(CapSense_2_IDAC_CNT == 2u)
        CapSense_2_CSD_CFG_REG &= ~(CapSense_2_CSD_CFG_ENABLE);
    #endif /* (CapSense_2_IDAC_CNT == 2u) */

    /* Disable the Clocks */
    CapSense_2_SenseClk_Stop();
    CapSense_2_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: CapSense_2_FindNextSensor
********************************************************************************
*
* Summary:
*  Finds the next sensor to scan.
*
* Parameters:
*  snsIndex:  Current index of sensor.
*
* Return:
*  Returns the next sensor index to scan.
*
* Global Variables:
*  CapSense_2_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*
* Side Effects:
*  This function affects the  current scanning and should not
*  be used outside the component.
*
*******************************************************************************/
uint8 CapSense_2_FindNextSensor(uint8 snsIndex)
{
    uint32 enableFlag;

    /* Check if sensor enabled */
    do
    {
        /* Proceed with next sensor */
        snsIndex++;
        if(snsIndex == CapSense_2_TOTAL_SENSOR_COUNT)
        {
            break;
        }
        enableFlag = CapSense_2_GetBitValue(CapSense_2_sensorEnableMask, (uint32)snsIndex);
    }
    while(enableFlag == 0u);

    return ((uint8)snsIndex);
}


/*******************************************************************************
* Function Name: CapSense_2_ScanSensor
********************************************************************************
*
* Summary:
*  Sets the scan settings and starts scanning a sensor. After scanning is complete,
*  the ISR copies the measured sensor raw data to the global raw sensor array.
*  Use of the ISR ensures this function is non-blocking.
*  Each sensor has a unique number within the sensor array.
*  This number is assigned by the CapSense customizer in sequence.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and mode of scan as single scan.
*  CapSense_2_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to the provided sensor argument.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_ScanSensor(uint32 sensor)
{
    /* Clears status/control variable and set sensorIndex */
    CapSense_2_csdStatusVar = 0u;
    CapSense_2_sensorIndex = (uint8)sensor;

    /* Start of sensor scan */
    CapSense_2_csdStatusVar = (CapSense_2_SW_STS_BUSY | CapSense_2_SW_CTRL_SINGLE_SCAN);
    CapSense_2_PreScan(sensor);
}


#if(0u != CapSense_2_CSHL_API_GENERATE)
/*******************************************************************************
* Function Name: CapSense_2_ScanWidget
********************************************************************************
*
* Summary:
*  Sets the scan settings and starts scanning a widget.
*
* Parameters:
*  uint32 widget: Widget number.
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  CapSense_2_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function CapSense_2_FindNextSensor or
*  CapSense_2_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*  CapSense_2_sensorEnableMaskBackup[] - used to backup bit masks of the enabled
*   sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_ScanWidget(uint32 widget)
{
    uint32 sensorsPerWidget;
    uint32 lastSensor;
    uint32 snsIndex;

    /* Get first sensor in widget */
    CapSense_2_sensorIndex = CapSense_2_rawDataIndex[widget];

    /* Get number of sensors in widget */
    sensorsPerWidget = CapSense_2_numberOfSensors[widget];

    /* Check if generic Sensor */
    if(sensorsPerWidget == 0u)
    {
        sensorsPerWidget = 1u;
    }

    /* Get last sensor in widget */
    lastSensor = (CapSense_2_rawDataIndex[widget] + sensorsPerWidget) - 1u;

    /* Backup sensorEnableMask array */
    for(snsIndex = 0u; snsIndex < CapSense_2_TOTAL_SENSOR_MASK; snsIndex++)
    {
        /* Backup sensorEnableMask array */
        CapSense_2_sensorEnableMaskBackup[snsIndex] = CapSense_2_sensorEnableMask[snsIndex];
    }

    /* Update sensorEnableMask to scan the sensors that belong to widget */
    for(snsIndex = 0u; snsIndex < CapSense_2_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        /* Update sensorEnableMask array bits to scan the widget only */
        if((snsIndex >= CapSense_2_sensorIndex) && (snsIndex <= lastSensor))
        {
            /* Set sensor bit to scan */
            CapSense_2_SetBitValue(CapSense_2_sensorEnableMask, snsIndex, 1u);
        }
        else
        {
            /* Reset sensor bit to do not scan */
            CapSense_2_SetBitValue(CapSense_2_sensorEnableMask, snsIndex, 0u);
        }
    }

    /* Check end of scan condition */
    if(CapSense_2_sensorIndex < CapSense_2_TOTAL_SENSOR_COUNT)
    {
        /* Set widget busy bit in status/control variable*/
        CapSense_2_csdStatusVar = (CapSense_2_SW_STS_BUSY | CapSense_2_SW_CTRL_WIDGET_SCAN);
        /* Scan first sensor of widget*/
        CapSense_2_PreScan((uint32)CapSense_2_sensorIndex);
    }
}
#endif /* (0u != CapSense_2_CSHL_API_GENERATE) */


/*******************************************************************************
* Function Name: CapSense_2_ScanEnableWidgets
********************************************************************************
*
* Summary:
*  This is the preferred method to scan all of the enabled widgets.
*  The API starts scanning a sensor within the enabled widgets.
*  The ISR continues scanning the sensors until all the enabled widgets are scanned.
*  Use of the ISR ensures this function is non-blocking.
*  All the widgets are enabled by default except proximity widgets.
*  The proximity widgets must be manually enabled as their long scan time
*  is incompatible with a fast response required of other widget types.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  CapSense_2_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function CapSense_2_FindNextSensor or
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_ScanEnabledWidgets(void)
{
    /* Clears status/control variable and set sensorIndex */
    CapSense_2_csdStatusVar = 0u;
    CapSense_2_sensorIndex = 0xFFu;

    /* Find next sensor */
    CapSense_2_sensorIndex = (uint8)CapSense_2_FindNextSensor(CapSense_2_sensorIndex);

    /* Check end of scan condition */
    if(CapSense_2_sensorIndex < CapSense_2_TOTAL_SENSOR_COUNT)
    {
        CapSense_2_csdStatusVar |= CapSense_2_SW_STS_BUSY;
        CapSense_2_PreScan((uint32)CapSense_2_sensorIndex);
    }
}


/*******************************************************************************
* Function Name: CapSense_2_IsBusy
********************************************************************************
*
* Summary:
*  Returns the state of the CapSense component. 1 means that scanning in
*  progress and 0 means that scanning is complete.
*
* Parameters:
*  None
*
* Return:
*  Returns the state of scanning. 1 - scanning in progress, 0 - scanning
*  completed.
*
* Global Variables:
*  CapSense_2_csdStatusVar - used to provide the status and mode of the scanning process.
*  Checks the busy status.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 CapSense_2_IsBusy(void)
{
    return ((uint32)((0u != (CapSense_2_csdStatusVar & CapSense_2_SW_STS_BUSY)) ? 1u : 0u));
}


/*******************************************************************************
* Function Name: CapSense_2_ReadSensorRaw
********************************************************************************
*
* Summary:
*  Returns sensor raw data from the global CapSense_sensorRaw[ ] array.
*  Each scan sensor has a unique number within the sensor array.
*  This number is assigned by the CapSense customizer in sequence.
*  Raw data can be used to perform calculations outside of the CapSense
*  provided framework.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  Returns the current raw data value for a defined sensor number.
*
* Global Variables:
*  CapSense_2_sensorRaw[] - used to store sensors raw data.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 CapSense_2_ReadSensorRaw(uint32 sensor)
{
    return CapSense_2_sensorRaw[sensor];
}


/*******************************************************************************
* Function Name: CapSense_2_WriteSensorRaw
********************************************************************************
*
* Summary:
*  This API writes the raw count value passed as an argument to the sensor Raw count array.
*
* Parameters:
*  sensor:  Sensor number.
*  value: Raw count value.
*
* Global Variables:
*  CapSense_2_sensorRaw[] - used to store sensors raw data.
*
* Return:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_WriteSensorRaw(uint32 sensor, uint16 value)
{
    CapSense_2_sensorRaw[sensor] = value;
}


#if (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE)
    /*******************************************************************************
    * Function Name: CapSense_2_SetScanResolution
    ********************************************************************************
    *
    * Summary:
    *  Sets a value of the sensor scan resolution for a widget.
    *  The MIN resolution can be set 8-bit. The MAX Resolution can be set 16 bit.
    *  This function is not available for the tuning mode "None".
    *
    * Parameters:
    *  widget:     Widget index.
    *  resolution: Represents the resolution value. The following defines which are available in the
    *              CapSense_2.h file should be used:
    *              CapSense_2_RESOLUTION_6_BITS
    *              CapSense_2_RESOLUTION_7_BITS
    *              CapSense_2_RESOLUTION_8_BITS
    *              CapSense_2_RESOLUTION_9_BITS
    *              CapSense_2_RESOLUTION_10_BITS
    *              CapSense_2_RESOLUTION_11_BITS
    *              CapSense_2_RESOLUTION_12_BITS
    *              CapSense_2_RESOLUTION_13_BITS
    *              CapSense_2_RESOLUTION_14_BITS
    *              CapSense_2_RESOLUTION_15_BITS
    *              CapSense_2_RESOLUTION_16_BITS
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_2_widgetResolution[] - used to store scan resolution of each widget.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_SetScanResolution(uint32 widget, uint32 resolution)
    {
        CapSense_2_widgetResolution[widget] = resolution;
    }
#endif /* (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE) */


/*******************************************************************************
* Function Name: CapSense_2_GetScanResolution
********************************************************************************
*
* Summary:
*  Returns the resolution value for the appropriate sensor.
*  This function is not available for tuning mode "None".
*
* Parameters:
*  widget:     Widget index.
*
* Return:
*  resolution: Returns the resolution value for the appropriate sensor.
*              The resolution values are defined in the CapSense_2.h file
*              The defines are encountered below:
*              CapSense_2_RESOLUTION_6_BITS
*              CapSense_2_RESOLUTION_7_BITS
*              CapSense_2_RESOLUTION_8_BITS
*              CapSense_2_RESOLUTION_9_BITS
*              CapSense_2_RESOLUTION_10_BITS
*              CapSense_2_RESOLUTION_11_BITS
*              CapSense_2_RESOLUTION_12_BITS
*              CapSense_2_RESOLUTION_13_BITS
*              CapSense_2_RESOLUTION_14_BITS
*              CapSense_2_RESOLUTION_15_BITS
*              CapSense_2_RESOLUTION_16_BITS
*
* Global Variables:
*  CapSense_2_widgetResolution[] - used to store scan resolution of every widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 CapSense_2_GetScanResolution(uint32 widget)
{
    return(CapSense_2_widgetResolution[widget]);
}


/*******************************************************************************
* Function Name: CapSense_2_ClearSensors
********************************************************************************
*
* Summary:
*  Resets all the sensors to the non-sampling state by sequentially disconnecting
*  all the sensors from Analog MUX Bus and putting them to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_ClearSensors(void)
{
    uint32 snsIndex;

    for (snsIndex = 0u; snsIndex < CapSense_2_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        CapSense_2_DisableScanSlot(snsIndex);
    }
}


#if (CapSense_2_IS_COMPLEX_SCANSLOTS)
    /*******************************************************************************
    * Function Name: CapSense_2_EnableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Configures the selected slot to measure during the next measurement
    *  cycle. The corresponding pin/pins are set to Analog High-Z mode and
    *  connected to the Analog Mux Bus. This also enables the comparator function.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Constants:
    *  CapSense_2_portTable[]  - used to store the port number that pin
    *  belongs to for each sensor.
    *  CapSense_2_maskTable[]  - used to store the pin within the port for
    *  each sensor.
    *  CapSense_2_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for
    *  complex sensors.
    *  Bit 7 (msb) is used to define the sensor type: single or complex.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_EnableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;
        /* Read sensor type: single or complex */
        uint8 snsType = CapSense_2_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & CapSense_2_COMPLEX_SS_FLAG) == 0u)
        {
            /* Enable sensor (single) */
            CapSense_2_EnableSensor(slot);
        }
        else
        {
            /* Enable complex sensor */
            snsType &= ~CapSense_2_COMPLEX_SS_FLAG;
            index = &CapSense_2_indexTable[snsType];
            snsNumber = CapSense_2_maskTable[slot];

            for (j = 0u; j < snsNumber; j++)
            {
                CapSense_2_EnableSensor(index[j]);
            }
        }
    }


    /*******************************************************************************
    * Function Name: CapSense_2_DisableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Disables the selected slot. The corresponding pin/pins is/are disconnected
    *  from the Analog Mux Bus and connected to GND, High_Z or Shield electrode.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_2_portTable[]  - used to store the port number that pin
    *  belongs to for each sensor.
    *  CapSense_2_maskTable[]  - used to store the pin within the port for
    *  each sensor.
    *  CapSense_2_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for
    *  complex sensors.
    *  7bit(msb) is used to define the sensor type: single or complex.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_DisableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;

        /* Read sensor type: single or complex */
        uint8 snsType = CapSense_2_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & CapSense_2_COMPLEX_SS_FLAG) == 0u)
        {
            /* Disable sensor (single) */
            CapSense_2_DisableSensor(slot);
        }
        else
        {
            /* Disable complex sensor */
            snsType &= ~CapSense_2_COMPLEX_SS_FLAG;
            index = &CapSense_2_indexTable[snsType];
            snsNumber = CapSense_2_maskTable[slot];

            for (j=0; j < snsNumber; j++)
            {
                CapSense_2_DisableSensor(index[j]);
            }
        }
    }
#endif  /* CapSense_2_IS_COMPLEX_SCANSLOTS */


/*******************************************************************************
* Function Name: CapSense_2_EnableSensor
********************************************************************************
*
* Summary:
*  Configures the selected sensor to measure during the next measurement cycle.
*  The corresponding pins are set to Analog High-Z mode and connected to the
*  Analog Mux Bus. This also enables the comparator function.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  CapSense_2_pinShiftTbl[] - used to store position of pin that
*  configured as sensor in port.
*  CapSense_2_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  CapSense_2_PrtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_EnableSensor(uint32 sensor)
{
    uint8  pinModeShift;
    uint8  pinHSIOMShift;
    uint8 interruptState;
    uint32 newRegisterValue;
    uint32 port;

    port = (uint32) CapSense_2_portTable[sensor];
    pinModeShift = CapSense_2_pinShiftTbl[sensor]  * CapSense_2_PC_PIN_CFG_SIZE;
    pinHSIOMShift = CapSense_2_pinShiftTbl[sensor] * CapSense_2_HSIOM_PIN_CFG_SIZE;

    interruptState = CyEnterCriticalSection();

    newRegisterValue = *CapSense_2_prtSelTbl[port];
    newRegisterValue &= ~(CapSense_2_CSD_HSIOM_MASK << pinHSIOMShift);
    newRegisterValue |= (uint32)((uint32)CapSense_2_CSD_SENSE_PORT_MODE << pinHSIOMShift);

    *CapSense_2_prtCfgTbl[port] &= (uint32)~((uint32)CapSense_2_CSD_PIN_MODE_MASK << pinModeShift);
    *CapSense_2_prtSelTbl[port] = newRegisterValue;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CapSense_2_DisableSensor
********************************************************************************
*
* Summary:
*  Disables the selected sensor. The corresponding pin is disconnected from the
*  Analog Mux Bus and connected to GND, High_Z or Shield electrode.
*
* Parameters:
*  sensor:  Sensor number
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  CapSense_2_pinShiftTbl[] - used to store position of pin that
*  configured as a sensor in the port.
*  CapSense_2_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  CapSense_2_PrtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_DisableSensor(uint32 sensor)
{
    uint8 interruptState;
    uint32 newRegisterValue;

    uint32 port = (uint32) CapSense_2_portTable[sensor];
    uint8  pinHSIOMShift = CapSense_2_pinShiftTbl[sensor] * CapSense_2_HSIOM_PIN_CFG_SIZE;
    uint8  pinModeShift = CapSense_2_pinShiftTbl[sensor]  * CapSense_2_PC_PIN_CFG_SIZE;

    uint32 inactiveConnect = CapSense_2_SNS_HIZANALOG_CONNECT;
    uint32 sensorState = CapSense_2_unscannedSnsDriveMode[sensor];

    *CapSense_2_prtSelTbl[port] &= ~(CapSense_2_CSD_HSIOM_MASK << pinHSIOMShift);

    #if(CapSense_2_IS_SHIELD_ENABLE != 0)
    if(sensorState != (uint32)CapSense_2__SHIELD)
    {
    #else
        /* Connected to Ground if shield is disabled */
        if(sensorState == (uint32)CapSense_2__SHIELD)
        {
            sensorState = (uint32)CapSense_2__GROUND;
        }
    #endif /* (CapSense_2_IS_SHIELD_ENABLE) */
        if(sensorState != (uint32)CapSense_2__HIZ_ANALOG)
        {
            /* Connected to Ground */
            inactiveConnect = (uint32)CapSense_2_SNS_GROUND_CONNECT;
        }

        interruptState = CyEnterCriticalSection();

        /* Set drive mode */
        newRegisterValue = *CapSense_2_prtCfgTbl[port];
        newRegisterValue &= ~(CapSense_2_CSD_PIN_MODE_MASK << pinModeShift);
        newRegisterValue |=  (uint32)(inactiveConnect << pinModeShift);
        *CapSense_2_prtCfgTbl[port] =  newRegisterValue;

        CyExitCriticalSection(interruptState);

        *CapSense_2_prtDRTbl[port]  &=  (uint32)~(uint32)((uint32)1u << CapSense_2_pinShiftTbl[sensor]);
    #if(CapSense_2_IS_SHIELD_ENABLE != 0)
    }
    else
    {
        /* Connect to Shield */
        *CapSense_2_prtSelTbl[port] |= (CapSense_2_CSD_SHIELD_PORT_MODE << pinHSIOMShift);
    }
    #endif /* (CapSense_2_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: CapSense_2_SetDriveModeAllPins
********************************************************************************
*
* Summary:
* This API sets the drive mode of port pins used by the CapSense
* component (sensors, guard, shield, shield tank and Cmod) to drive the
* mode specified by the argument.
*
* Parameters:
*  driveMode:  Drive mode definition.
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  CapSense_2_pinShiftTbl[] - used to store position of pin that
*  configured as a sensor in the port.
*
* Side Effects:
*  This API shall be called only after CapSense component is stopped.
*
*******************************************************************************/
void CapSense_2_SetDriveModeAllPins(uint32 driveMode)
{
    uint8 curSensor;
    #if (CapSense_2_IS_COMPLEX_SCANSLOTS)
        uint8 snsNumber;
        uint8 snsType;
    #endif  /* CapSense_2_IS_COMPLEX_SCANSLOTS */
    uint32 prtNumberTmp;
    uint32 pinNumberTmp;

    for(curSensor = 0u; curSensor < CapSense_2_TOTAL_SENSOR_COUNT; curSensor++)
    {
        #if (CapSense_2_IS_COMPLEX_SCANSLOTS)
            /* Read sensor type: single or complex */
            snsType = CapSense_2_portTable[curSensor];

            /* Check if sensor is complex */
            if ((snsType & CapSense_2_COMPLEX_SS_FLAG) == 0u)
            {
                /* The sensor is not complex */
                snsNumber = curSensor;
            }
            else
            {
                /* Get dedicated sensor ID of the complex sensor */
                snsType &= ~CapSense_2_COMPLEX_SS_FLAG;
                snsNumber = CapSense_2_indexTable[snsType];
            }

            prtNumberTmp = CapSense_2_portTable[snsNumber];
            pinNumberTmp = CapSense_2_pinShiftTbl[snsNumber];
        #else
            prtNumberTmp = CapSense_2_portTable[curSensor];
            pinNumberTmp = CapSense_2_pinShiftTbl[curSensor];
        #endif  /* CapSense_2_IS_COMPLEX_SCANSLOTS */

        CapSense_2_SetPinDriveMode(driveMode, pinNumberTmp, prtNumberTmp);
    }

    CapSense_2_SetPinDriveMode(driveMode, (uint32)CapSense_2_CMOD_PIN_NUMBER, (uint32)CapSense_2_CMOD_PRT_NUMBER);

    #if(0u != CapSense_2_CSH_TANK_ENABLE)
        CapSense_2_SetPinDriveMode(driveMode, (uint32)CapSense_2_CTANK_PIN_NUMBER, (uint32)CapSense_2_CTANK_PRT_NUMBER);
    #endif /* (0u != CapSense_2_CSH_TANK_ENABLE) */

    #if(0u != CapSense_2_IS_SHIELD_ENABLE)
        CapSense_2_SetPinDriveMode(driveMode, (uint32)CapSense_2_SHIELD_PIN_NUMBER, (uint32)CapSense_2_SHIELD_PORT_NUMBER);
    #endif /* (0u != CapSense_2_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: CapSense_2_RestoreDriveModeAllPins
********************************************************************************
*
* Summary:
*  This API restores the drive for all the CapSense port pins to the original
*  state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*
* Side Effects:
*  This API is called only after the CapSense component is stopped.
*
*******************************************************************************/
void CapSense_2_RestoreDriveModeAllPins(void)
{
    uint32 newRegisterValue;

    CapSense_2_SetDriveModeAllPins(CY_SYS_PINS_DM_ALG_HIZ);

    newRegisterValue = CapSense_2_CMOD_CONNECTION_REG;
    newRegisterValue &= ~(CapSense_2_CSD_CMOD_CONNECTION_MASK);
    newRegisterValue |= CapSense_2_CSD_CMOD_TO_AMUXBUS_A;
    CapSense_2_CMOD_CONNECTION_REG = newRegisterValue;

    #if(0u != CapSense_2_CSH_TANK_ENABLE)
        newRegisterValue = CapSense_2_CTANK_CONNECTION_REG;
        newRegisterValue &= ~(CapSense_2_CSD_CTANK_CONNECTION_MASK);
        newRegisterValue |= CapSense_2_CSD_CTANK_TO_AMUXBUS_B;
        CapSense_2_CTANK_CONNECTION_REG = newRegisterValue;
    #endif/* (0u != CapSense_2_CSH_TANK_ENABLE) */

    #if(0u != CapSense_2_IS_SHIELD_ENABLE)
        newRegisterValue = *CapSense_2_prtSelTbl[CapSense_2_SHIELD_PORT_NUMBER];
        newRegisterValue &= ~(CapSense_2_CSD_HSIOM_MASK << ((uint32)CapSense_2_SHIELD_PIN_NUMBER * CapSense_2_HSIOM_PIN_CFG_SIZE));
        newRegisterValue |= (CapSense_2_CSD_SHIELD_PORT_MODE << ((uint32)CapSense_2_SHIELD_PIN_NUMBER * CapSense_2_HSIOM_PIN_CFG_SIZE));
        *CapSense_2_prtSelTbl[CapSense_2_SHIELD_PORT_NUMBER] = newRegisterValue;
    #endif /* (0u != CapSense_2_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: CapSense_2_SetPinDriveMode
********************************************************************************
*
* Summary:
* This API sets the drive mode for the appropriate port/pin.
*
* Parameters:
*  driveMode:  Drive mode definition.
*  portNumber: contains port number (0, 1, 2).
*  pinNumber: contains pin number (0, 1, 2, ... , 7).
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  CapSense_2_prtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  This API is called only after CapSense component is stopped.
*
*******************************************************************************/
void CapSense_2_SetPinDriveMode(uint32 driveMode, uint32 pinNumber, uint32 portNumber)
{
    uint32  pinModeShift;
    uint32 newRegisterValue;

    pinModeShift  = pinNumber * CapSense_2_PC_PIN_CFG_SIZE;

    newRegisterValue = *CapSense_2_prtCfgTbl[portNumber];
    newRegisterValue &= ~(CapSense_2_CSD_PIN_MODE_MASK << pinModeShift);
    newRegisterValue |=  (uint32)((uint32)driveMode << pinModeShift);
    *CapSense_2_prtCfgTbl[portNumber] =  newRegisterValue;
}


/*******************************************************************************
* Function Name: CapSense_2_PreScan
********************************************************************************
*
* Summary:
*  Sets required settings, enables a sensor, removes Vref from AMUX and starts the
*  scanning process of the sensor.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
* CapSense_2_widgetNumber[] - This array contains numbers of widgets for each sensor.
* CapSense_2_widgetResolution[] - Contains the widget resolution.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_PreScan(uint32 sensor)
{
    uint8 widget;
    uint8 interruptState;
    uint32 newRegValue;
    uint32 counterResolution;

    #if(CapSense_2_PRS_OPTIONS == CapSense_2__PRS_AUTO)
        uint8 senseClkDivMath;
        uint8 sampleClkDivMath;
    #endif /* (CapSense_2_PRS_OPTIONS == CapSense_2__PRS_AUTO) */

    #if ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) &&\
         (0 != CapSense_2_IS_OVERSAMPLING_EN))
        uint32 oversamplingFactor;
    #endif /* ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) &&\
               (0 != CapSense_2_IS_OVERSAMPLING_EN)) */

    /* Define widget sensor belongs to */
    widget = CapSense_2_widgetNumber[sensor];

    /* Recalculate Counter Resolution to MSB 16 bits */
    counterResolution = CapSense_2_widgetResolution[widget];

    #if ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) &&\
         (0 != CapSense_2_IS_OVERSAMPLING_EN))
        oversamplingFactor = CapSense_2_GetBitValue(CapSense_2_scanSpeedTbl, sensor);

        if(counterResolution < CapSense_2_RESOLUTION_16_BITS)
        {
            counterResolution <<= oversamplingFactor;
            counterResolution |= (uint32)(CapSense_2_RESOLUTION_8_BITS);
        }
    #endif /* ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) &&\
               (0 != CapSense_2_IS_OVERSAMPLING_EN)) */

    #if (0u != CapSense_2_IS_M0S8PERI_BLOCK)
        CapSense_2_SenseClk_Stop();
        CapSense_2_SampleClk_Stop();
    #endif /* (0u != CapSense_2_IS_M0S8PERI_BLOCK) */

    #if (0u != CapSense_2_MULTIPLE_FREQUENCY_SET)
        CapSense_2_SampleClk_SetDividerValue((uint16)CapSense_2_sampleClkDividerVal[sensor]);
        CapSense_2_SenseClk_SetDividerValue((uint16)CapSense_2_senseClkDividerVal[sensor]);
    #else
        CapSense_2_SampleClk_SetDividerValue((uint16)CapSense_2_sampleClkDividerVal);
        CapSense_2_SenseClk_SetDividerValue((uint16)CapSense_2_senseClkDividerVal);
    #endif /* (0u != CapSense_2_MULTIPLE_FREQUENCY_SET) */

    #if (0u != CapSense_2_IS_M0S8PERI_BLOCK)
        CapSense_2_SampleClk_Start();
        CapSense_2_SenseClk_StartEx(CapSense_2_SampleClk__DIV_ID);

        #if(CapSense_2_PRS_OPTIONS == CapSense_2__PRS_NONE)
            CyIntDisable(CapSense_2_ISR_NUMBER);
            CapSense_2_CSD_CNT_REG = CapSense_2_ONE_CYCLE;
            while(0u != (CapSense_2_CSD_CNT_REG & CapSense_2_RESOLUTION_16_BITS))
            {
            /* Wait until scanning is complete */
            }
            CapSense_2_CSD_INTR_REG = 1u;
            CyIntClearPending(CapSense_2_ISR_NUMBER);
            CyIntEnable(CapSense_2_ISR_NUMBER);
        #endif /* CapSense_2_PRS_OPTIONS == CapSense_2__PRS_NONE */
    #endif /* (0u != CapSense_2_IS_M0S8PERI_BLOCK) */

#if(CapSense_2_PRS_OPTIONS != CapSense_2__PRS_NONE)

    #if(CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO)
        if(CapSense_2_prescalersTuningDone != 0u)
        {
    #endif /* (CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) */

            CyIntDisable(CapSense_2_ISR_NUMBER);

            newRegValue = CapSense_2_CSD_CFG_REG;
            newRegValue |= CapSense_2_CSD_CFG_PRS_SELECT;

            #if(CapSense_2_PRS_OPTIONS == CapSense_2__PRS_AUTO)

                newRegValue &= ~(CapSense_2_PRS_MODE_MASK);

                #if (0u != CapSense_2_MULTIPLE_FREQUENCY_SET)
                    senseClkDivMath = CapSense_2_senseClkDividerVal[sensor];
                    sampleClkDivMath = CapSense_2_sampleClkDividerVal[sensor];
                #else
                    senseClkDivMath = CapSense_2_senseClkDividerVal;
                    sampleClkDivMath = CapSense_2_sampleClkDividerVal;
                #endif /* ( CapSense_2_MULTIPLE_FREQUENCY_SET) */

                #if(0u == CapSense_2_IS_M0S8PERI_BLOCK)
                    senseClkDivMath *= sampleClkDivMath;
                #endif /* (0u == CapSense_2_IS_M0S8PERI_BLOCK) */

                if((senseClkDivMath * CapSense_2_RESOLUTION_12_BITS) <
                   (sampleClkDivMath * CapSense_2_widgetResolution[widget]))
                {
                    newRegValue |= CapSense_2_CSD_PRS_12_BIT;
                }
            #endif /* (CapSense_2_PRS_OPTIONS == CapSense_2__PRS_AUTO) */

            CapSense_2_CSD_CFG_REG = newRegValue;

            CyIntEnable(CapSense_2_ISR_NUMBER);

    #if(CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO)
        }
    #endif /* (CapSense_2_PRS_OPTIONS != CapSense_2__PRS_NONE) */

#endif /* (CapSense_2_PRS_OPTIONS == CapSense_2__PRS_NONE) */

    /* Set Idac Value */
    CyIntDisable(CapSense_2_ISR_NUMBER);
    newRegValue = CapSense_2_CSD_IDAC_REG;

#if (CapSense_2_IDAC_CNT == 1u)
    newRegValue &= ~(CapSense_2_CSD_IDAC1_DATA_MASK);
    newRegValue |= CapSense_2_modulationIDAC[sensor];
#else
    newRegValue &= ~(CapSense_2_CSD_IDAC1_DATA_MASK | CapSense_2_CSD_IDAC2_DATA_MASK);
    newRegValue |= (CapSense_2_modulationIDAC[sensor] |
                            (uint32)((uint32)CapSense_2_compensationIDAC[sensor] <<
                            CapSense_2_CSD_IDAC2_DATA_OFFSET));
#endif /* (CapSense_2_IDAC_CNT == 1u) */

    CapSense_2_CSD_IDAC_REG = newRegValue;

#if(CapSense_2_CMOD_PREGARGE_OPTION == CapSense_2__CAPPRIOBUF)
    newRegValue = CapSense_2_CMOD_CONNECTION_REG;
    newRegValue &= ~(CapSense_2_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= CapSense_2_CSD_CMOD_TO_AMUXBUS_A;
    CapSense_2_CMOD_CONNECTION_REG = newRegValue;

    newRegValue = CapSense_2_CMOD_PORT_PC_REG;
    newRegValue &= ~(CapSense_2_CSD_CMOD_PC_MASK);
    newRegValue |= CapSense_2_CMOD_PC_HIGH_Z_MODE;
    CapSense_2_CMOD_PORT_PC_REG = newRegValue;
#endif /* (CapSense_2_CMOD_PREGARGE_OPTION == CapSense_2__CAPPRIOBUF) */

    /* Disconnect Vref Buffer from AMUX */
    newRegValue = CapSense_2_CSD_CFG_REG;
    newRegValue &= ~(CapSense_2_PRECHARGE_CONFIG_MASK);
    newRegValue |= CapSense_2_CTANK_PRECHARGE_CONFIG;

    CyIntEnable(CapSense_2_ISR_NUMBER);

    /* Enable Sensor */
    CapSense_2_EnableScanSlot(sensor);

    interruptState = CyEnterCriticalSection();
    CapSense_2_CSD_CFG_REG = newRegValue;

    /* `#START CapSense_2_PreSettlingDelay_Debug` */

    /* `#END` */

#ifdef CapSense_2_PRE_SCAN_PRE_SETTLING_DELAY_DEBUG_CALLBACK
    CapSense_2_PreScan_Pre_SettlingDelay_Debug_Callback();
#endif /* CapSense_2_PRE_SCAN_PRE_SETTLING_DELAY_DEBUG_CALLBACK */

    CyDelayCycles(CapSense_2_GLITCH_ELIMINATION_CYCLES);

    /* `#START CapSense_2_PreScan_Debug` */

    /* `#END` */

#ifdef CapSense_2_PRE_SCAN_DEBUG_CALLBACK
    CapSense_2_PreScan_Debug_Callback();
#endif /* CapSense_2_PRE_SCAN_DEBUG_CALLBACK */

    CapSense_2_CSD_CNT_REG = counterResolution;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CapSense_2_PostScan
********************************************************************************
*
* Summary:
*  Stores the results of measurement in the CapSense_2_sensorRaw[] array,
*  sets the scanning sensor in the non-sampling state, turns off Idac(Current Source IDAC),
*  disconnects the IDAC(Sink mode), and applies Vref on AMUX.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_sensorRaw[] - used to store sensors raw data.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_PostScan(uint32 sensor)
{
    uint32 newRegValue;
#if ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) &&\
     (0 != CapSense_2_IS_OVERSAMPLING_EN))
    uint32 oversamplingFactor;
    uint32 widget;
#endif /* ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) &&\
           (0 != CapSense_2_IS_OVERSAMPLING_EN)) */

    /* `#START CapSense_2_PostScan_Debug` */

    /* `#END` */

#ifdef CapSense_2_POST_SCAN_DEBUG_CALLBACK
    CapSense_2_PostScan_Debug_Callback();
#endif /* CapSense_2_POST_SCAN_DEBUG_Callback */

    /* Read SlotResult from Raw Counter */
    CapSense_2_sensorRaw[sensor]  = (uint16)CapSense_2_CSD_CNT_REG;

#if ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) &&\
     (0 != CapSense_2_IS_OVERSAMPLING_EN))
    widget = CapSense_2_widgetNumber[sensor];
    if(CapSense_2_widgetResolution[widget] < CapSense_2_RESOLUTION_16_BITS)
    {
        oversamplingFactor = CapSense_2_GetBitValue(CapSense_2_scanSpeedTbl, sensor);
        CapSense_2_sensorRaw[sensor] >>= oversamplingFactor;
    }
#endif /* ((CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO) &&\
           (0 != CapSense_2_IS_OVERSAMPLING_EN)) */

    /* Disable Sensor */
    CapSense_2_DisableScanSlot(sensor);

    CyIntDisable(CapSense_2_ISR_NUMBER);

#if(CapSense_2_CMOD_PREGARGE_OPTION == CapSense_2__CAPPRIOBUF)
    newRegValue = CapSense_2_CMOD_CONNECTION_REG;
    newRegValue &= ~(CapSense_2_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= CapSense_2_CSD_CMOD_TO_AMUXBUS_B;
    CapSense_2_CMOD_CONNECTION_REG = newRegValue;

    newRegValue = CapSense_2_CMOD_PORT_PC_REG;
    newRegValue &= ~(CapSense_2_CSD_CMOD_PC_MASK);
    newRegValue |= CapSense_2_CMOD_PC_STRONG_MODE;
    CapSense_2_CMOD_PORT_PC_REG = newRegValue;

    newRegValue = CapSense_2_CMOD_PORT_DR_REG;
    newRegValue &= ~(CapSense_2_CMOD_DR_MASK);
    newRegValue |= CapSense_2_CMOD_DR_CONFIG;
    CapSense_2_CMOD_PORT_DR_REG = newRegValue;
#endif /* (CapSense_2_CMOD_PREGARGE_OPTION == CapSense_2__CAPPRIOBUF) */

    /* Connect Vref Buffer to AMUX bus  */
    newRegValue = CapSense_2_CSD_CFG_REG;
    newRegValue &= ~(CapSense_2_PRECHARGE_CONFIG_MASK);
    newRegValue |= CapSense_2_CMOD_PRECHARGE_CONFIG;
    CapSense_2_CSD_CFG_REG = newRegValue;

    /* Set Idac Value = 0 */
#if (CapSense_2_IDAC_CNT == 1u)
    CapSense_2_CSD_IDAC_REG &= ~(CapSense_2_CSD_IDAC1_DATA_MASK);
#else
    CapSense_2_CSD_IDAC_REG &= ~(CapSense_2_CSD_IDAC1_DATA_MASK | CapSense_2_CSD_IDAC2_DATA_MASK);
#endif /* (CapSense_2_IDAC_CNT == 1u) */

    CyIntEnable(CapSense_2_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: CapSense_2_EnableShieldElectrode
********************************************************************************
*
* Summary:
*  This API enables or disables the shield electrode on a specified port pin.
*
* Parameters:
*  portNumber: contains the shield electrode port number (0, 1, 2).
*  pinNumber: contains the shield electrode pin number (0, 1, 2, ... , 7).
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_prtCfgTbl[] - Contains pointers to the port config registers for each port.
*  CapSense_2_prtSelTbl[] - Contains pointers to the HSIOM registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_EnableShieldElectrode(uint32 pinNumber, uint32 portNumber)
{
    uint32 newRegValue;

    *CapSense_2_prtCfgTbl[portNumber] &= ~(CapSense_2_CSD_PIN_MODE_MASK << (pinNumber * CapSense_2_PC_PIN_CFG_SIZE));
    newRegValue = *CapSense_2_prtSelTbl[portNumber];
    newRegValue &= ~(CapSense_2_CSD_HSIOM_MASK << (pinNumber * CapSense_2_HSIOM_PIN_CFG_SIZE));
    newRegValue |= (CapSense_2_CSD_SHIELD_PORT_MODE << (pinNumber * CapSense_2_HSIOM_PIN_CFG_SIZE));
    *CapSense_2_prtSelTbl[portNumber] = newRegValue;
}


/*******************************************************************************
* Function Name: CapSense_2_SetShieldDelay
********************************************************************************
*
* Summary:
*  This API sets a shield delay.
*
* Parameters:
*  delay:  shield delay value:
*                               0 - no delay
*                               1 - 1 cycle delay
*                               2 - 2 cycles delay
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_SetShieldDelay(uint32 delay)
{
    uint32 newRegValue;

    delay &= 0x03u;

    newRegValue = CapSense_2_CSD_CFG_REG;
    newRegValue &= (uint32)(~(uint32)CapSense_2_SHIELD_DELAY);
    newRegValue |= (delay << CapSense_2_CSD_CFG_SHIELD_DELAY_POS);
    CapSense_2_CSD_CFG_REG = newRegValue;
}


/*******************************************************************************
* Function Name: CapSense_2_ReadCurrentScanningSensor
********************************************************************************
*
* Summary:
*  This API returns scanning sensor number when sensor scan is in progress.
*  When sensor scan is completed the API returns
*  $INSTANCE_NAME`_NOT_SENSOR (0xFFFFFFFF) (when no sensor is scanned).
*
* Parameters:
*   None.
*
* Return:
*  Returns Sensor number if sensor is being scanned and
*  $INSTANCE_NAME`_NOT_SENSOR (0xFFFFFFFF) if scanning is complete.
*
* Global Variables:
*  CapSense_2_sensorIndex - the sensor number is being scanned.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 CapSense_2_ReadCurrentScanningSensor(void)
{
    return ((uint32)(( 0u != (CapSense_2_csdStatusVar & CapSense_2_SW_STS_BUSY)) ?
                     CapSense_2_sensorIndex : CapSense_2_NOT_SENSOR));
}


/*******************************************************************************
* Function Name: CapSense_2_GetBitValue
********************************************************************************
*
* Summary:
*  The API returns a bit status of the bit in the table array which contains the masks.
*
* Parameters:
*  table[] - array with bit masks.
*  position - position of bit in the table[] array.
*
* Return:
*  0 - bit is not set; 1 - bit is set.
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 CapSense_2_GetBitValue(const uint8 table[], uint32 position)
{
    uint32 offset;
    uint8 enMask;

    /* position is divided by 8 to calculate the element of the
       table[] array that contains the enable bit
       for an appropriate sensor.
    */
    offset = (position >> 3u);

    /* The enMask calculation for the appropriate sensor. Operation (position & 0x07u)
       intends to calculate the enable bit offset for the 8-bit element of the
       table[] array.
    */
    enMask = 0x01u << (position & 0x07u);

    return ((0u !=(table[offset] & enMask)) ? 1Lu : 0Lu);
}


/*******************************************************************************
* Function Name: CapSense_2_SetBitValue
********************************************************************************
*
* Summary:
*  The API sets a bit status of the bit in the table array which contains masks.
*
* Parameters:
*  table[] - array with bit masks.
*  position - position of bit in the table[] array.
*  value: 0 - bit is not set; 1 - bit is set.
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_SetBitValue(uint8 table[], uint32 position, uint32 value)
{
    uint32 offset;
    uint8 enMask;

    /* position is divided by 8 to calculate the element of the
       table[] array that contains the enable bit
       for an appropriate sensor.
    */
    offset = (position >> 3u);

    /* The enMask calculation for the appropriate sensor. Operation (position & 0x07u)
       intends to calculate the enable bit offset for the 8-bit element of the
       table[] array.
    */
    enMask = 0x01u << (position & 0x07u);

    if(0u != value)
    {
        table[offset] |= enMask;
    }
    else
    {
        table[offset] &= (uint8)~(enMask);
    }
}



/*******************************************************************************
* Function Name: CapSense_2_GetSenseClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the sense clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be
*  from 0 to (CapSense_2_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the sense clock divider of the sensor.
*
* Global Variables:
*  CapSense_2_senseClkDividerVal[] - stores the sense clock divider values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 CapSense_2_GetSenseClkDivider(uint32 sensor)
{
    #if(0u != CapSense_2_MULTIPLE_FREQUENCY_SET)
        return((uint32)CapSense_2_senseClkDividerVal[sensor]);
    #else
        return((uint32)CapSense_2_senseClkDividerVal);
    #endif /* (0u != CapSense_2_MULTIPLE_FREQUENCY_SET) */
}

#if (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE)
    /*******************************************************************************
    * Function Name: CapSense_2_SetSenseClkDivider
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the sense clock divider for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  senseClk: represents the sense clock value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  CapSense_2_senseClkDividerVal[] - stores the sense clock divider values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_SetSenseClkDivider(uint32 sensor, uint32 senseClk)
    {
        #if(0u != CapSense_2_MULTIPLE_FREQUENCY_SET)
            CapSense_2_senseClkDividerVal[sensor] = (uint8)senseClk;
        #else
            CapSense_2_senseClkDividerVal = (uint8)senseClk;
        #endif /* (0u != CapSense_2_MULTIPLE_FREQUENCY_SET) */
    }
#endif /* (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE) */


/*******************************************************************************
* Function Name: CapSense_2_GetModulatorClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the modulator sample clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The value of index can be
*  from 0 to (CapSense_2_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulator sample clock divider for the  sensor.
*
* Global Variables:
*  CapSense_2_sampleClkDividerVal[] - stores the modulator sample divider values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 CapSense_2_GetModulatorClkDivider(uint32 sensor)
{
    #if(0u != CapSense_2_MULTIPLE_FREQUENCY_SET)
        return((uint32)CapSense_2_sampleClkDividerVal[sensor]);
    #else
        return((uint32)CapSense_2_sampleClkDividerVal);
    #endif /* (0u != CapSense_2_MULTIPLE_FREQUENCY_SET) */
}

#if (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE)
    /*******************************************************************************
    * Function Name: CapSense_2_SetModulatorClkDivider
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the modulator sample clock divider for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  modulatorClk: represents the modulator sample clock value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  CapSense_2_sampleClkDividerVal[] - stores the modulator sample divider values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk)
    {
        #if(0u != CapSense_2_MULTIPLE_FREQUENCY_SET)
            CapSense_2_sampleClkDividerVal[sensor] = (uint8)modulatorClk;
        #else
            CapSense_2_sampleClkDividerVal = (uint8)modulatorClk;
        #endif /* (0u != CapSense_2_MULTIPLE_FREQUENCY_SET) */
    }
#endif /* (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE) */

/*******************************************************************************
* Function Name: CapSense_2_GetModulationIDAC
********************************************************************************
*
* Summary:
*  This API returns a value of the modulation IDAC for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be
*  from 0 to (CapSense_2_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulation IDAC of the sensor.
*
* Global Variables:
*  CapSense_2_modulationIDAC[] - stores modulation IDAC values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 CapSense_2_GetModulationIDAC(uint32 sensor)
{
        return((uint32)CapSense_2_modulationIDAC[sensor]);
}

#if (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE)
    /*******************************************************************************
    * Function Name: CapSense_2_SetModulationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the modulation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  compIdacValue: represents the modulation IDAC data register value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  CapSense_2_modulationIDAC[] - array with modulation IDAC values
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_SetModulationIDAC(uint32 sensor, uint32 modIdacValue)
    {
        CapSense_2_modulationIDAC[sensor] = (uint8)modIdacValue;
    }
#endif /* (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE) */


#if(CapSense_2_IDAC_CNT > 1u)
    /*******************************************************************************
    * Function Name: CapSense_2_GetCompensationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API returns a value of the compensation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor: sensor index. The index value can be
    *  from 0 to (CapSense_2_TOTAL_SENSOR_COUNT-1).
    *
    * Return:
    *  This API returns the compensation IDAC of the sensor.
    *
    * Global Variables:
    *  CapSense_2_compensationIDAC[] - stores the compensation IDAC values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    uint32 CapSense_2_GetCompensationIDAC(uint32 sensor)
    {
        return((uint32)CapSense_2_compensationIDAC[sensor]);
    }
#endif /* (CapSense_2_IDAC_CNT > 1u) */


#if((CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE) && (CapSense_2_IDAC_CNT > 1u))
    /*******************************************************************************
    * Function Name: CapSense_2_SetCompensationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of compensation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  compIdacValue: represents the compensation IDAC data register value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  CapSense_2_compensationIDAC[] - an array with compensation IDAC values
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue)
    {
        CapSense_2_compensationIDAC[sensor] = (uint8)compIdacValue;
    }
#endif /* ((CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE) && (CapSense_2_IDAC_CNT > 1u)) */

/*******************************************************************************
* Function Name: CapSense_2_GetIDACRange
********************************************************************************
*
* Summary:
*  This API returns a value that indicates the IDAC range used by the
*  component to scan sensors. The IDAC range is common for all the sensors.
*
* Parameters:
*  None
*
* Return:
*  This API Returns a value that indicates the IDAC range:
*      0 - IDAC range set to 4x
*      1 - IDAC range set to 8x
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 CapSense_2_GetIDACRange(void)
{
    return((0u != (CapSense_2_CSD_IDAC_REG & CapSense_2_CSD_IDAC1_RANGE_8X)) ? 1uL : 0uL);
}

#if (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE)
    /*******************************************************************************
    * Function Name: CapSense_2_SetIDACRange
    ********************************************************************************
    *
    * Summary:
    *  This API sets the IDAC range to the 4x (1.2uA/bit) or 8x (2.4uA/bit) mode.
    *  The IDAC range is common for all the sensors and common for the modulation and
    *  compensation IDACs.
    *
    * Parameters:
    *  iDacRange:  represents value for IDAC range
    *  0 -  IDAC range set to 4x (1.2uA/bit)
    *  1 or >1 - IDAC range set to 8x (2.4uA/bit)
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  None
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_SetIDACRange(uint32 iDacRange)
    {
        if(iDacRange != 0u)
        {
            /*  IDAC range = 8x (2.4uA/bit) */
            CapSense_2_CSD_IDAC_REG |= (CapSense_2_CSD_IDAC1_RANGE_8X |\
                                              CapSense_2_CSD_IDAC2_RANGE_8X);
        }
        else
        {
            /*  IDAC range = 4x (1.2uA/bit) */
            /*  IDAC range = 8x (2.4uA/bit) */
            CapSense_2_CSD_IDAC_REG &= ~(CapSense_2_CSD_IDAC1_RANGE_8X |\
                                               CapSense_2_CSD_IDAC2_RANGE_8X);
        }
    }
#endif /* (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_NONE) */


#if((0u != CapSense_2_AUTOCALIBRATION_ENABLE) || (CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO))
    /*******************************************************************************
    * Function Name: CapSense_2_GetSensorRaw
    ********************************************************************************
    *
    * Summary:
    *  The API updates and gets uprated raw data from the sensor.
    *
    * Parameters:
    *  sensor - Sensor number.
    *
    * Return:
    *  Returns the current raw data value for a defined sensor number.
    *
    * Global Variables:
    *  None
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    uint16  CapSense_2_GetSensorRaw(uint32 sensor)
    {
        uint32 curSample;
        uint32 avgVal = 0u;

        for(curSample = 0u; curSample < CapSense_2_AVG_SAMPLES_NUM; curSample++)
        {

            CapSense_2_ScanSensor((uint32)sensor);
            while(CapSense_2_IsBusy() == 1u)
            {
                /* Wait while sensor is busy */
            }
            avgVal += CapSense_2_ReadSensorRaw((uint32)sensor);
        }

        return((uint16)(avgVal / CapSense_2_AVG_SAMPLES_NUM));
    }
#endif /* ((0u != CapSense_2_AUTOCALIBRATION_ENABLE) && (CapSense_2_TUNING_METHOD == CapSense_2__TUNING_AUTO)) */


#if((0u != CapSense_2_AUTOCALIBRATION_ENABLE) && (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_AUTO))
    /*******************************************************************************
    * Function Name: CapSense_2_CalibrateSensor
    ********************************************************************************
    *
    * Summary:
    *  Computes the one sensor IDAC value, which provides the raw signal on
    *  a specified level, with a specified prescaler, speed, and resolution.
    *
    * Parameters:
    *  sensor - Sensor Number.
    *
    *  rawLevel -  Raw data level which should be reached during the calibration
    *              procedure.
    *
    *  idacLevelsTbl - Pointer to the modulatorIdac data register configuration table.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CapSense_2_CalibrateSensor(uint32 sensor, uint32 rawLevel, uint8 idacLevelsTbl[])
    {
        uint8 mask;
        uint16 rawData;

        rawData = 0u;

        mask = 0x80u;
        /* Init IDAC for null Channel */
        idacLevelsTbl[sensor] = 0x00u;

        do
        {
            /* Set bit for null Channel */
            idacLevelsTbl[sensor] |= mask;

            /* Scan null Channel and get Rawdata */
            rawData = CapSense_2_GetSensorRaw(sensor);

            /* Decrease IDAC until Rawdata reaches rawLevel */
            if(rawData < rawLevel)
            {
                /* Reset bit for null Channel  */
                idacLevelsTbl[sensor] &= (uint8)~mask;
            }

            mask >>= 1u;
        }
        while(mask > 0u);

    }
#endif /* ((0u != CapSense_2_AUTOCALIBRATION_ENABLE) && (CapSense_2_TUNING_METHOD != CapSense_2__TUNING_AUTO)) */


/*******************************************************************************
* Function Name: CapSense_2_SetUnscannedSensorState
********************************************************************************
*
* Summary:
*  This API sets a state for un-scanned sensors.
*  It is possible to set the state to Ground, High-Z, or the shield electrode.
*  The un-scanned sensor can be connected to a shield electrode only if the shield is
*  enabled. If the shield is disabled and this API is called with the parameter which
*  indicates the shield state, the un-scanned sensor will be connected to Ground.
*
* Parameters:
*  sensor - Sensor Number.
*  sensorState: This parameter indicates un-scanned sensor state:
*
*     CapSense_2__GROUND 0
*     CapSense_2__HIZ_ANALOG 1
*     CapSense_2__SHIELD 2
*
* Return:
*  None.
*
* Global Variables:
*  CapSense_2_unscannedSnsDriveMode[] - used to store the state for
*  un-scanned sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_SetUnscannedSensorState(uint32 sensor, uint32 sensorState)
{
    #if(CapSense_2_IS_COMPLEX_SCANSLOTS)
        uint8 snsType;
        uint8 curSensor;
        uint16 snsNumber;

        /* Read sensor type: single or complex */
        snsType = CapSense_2_portTable[sensor];

        /* Check if sensor is complex */
        if(0u != (snsType & CapSense_2_COMPLEX_SS_FLAG))
        {
            snsType &= (uint8)~CapSense_2_COMPLEX_SS_FLAG;
            snsNumber = (uint16)CapSense_2_maskTable[sensor];

            for (curSensor=0u; curSensor < snsNumber; curSensor++)
            {
                sensor = CapSense_2_indexTable[snsType + curSensor];
                CapSense_2_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
            }
        }
        else
        {
            CapSense_2_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
        }
    #else
        CapSense_2_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
    #endif /* (CapSense_2_IS_COMPLEX_SCANSLOTS) */
}

#if(0u != CapSense_2_TOTAL_CENTROIDS_COUNT)
/*******************************************************************************
* Function Name: CapSense_2_NormalizeWidgets
********************************************************************************
*
* Summary:
*  This API aligns all the parameters of the widget to the maximum parameter.
*
* Parameters:
*  uint32 widgetsNum: Number of widgets.
*  uint8 *dataPrt: pointer to array with widget parameters.
*
* Return:
*  None
*
* Global Variables:
*  CapSense_2_numberOfSensors[] - Number of sensors in the widget.
*  CapSense_2_rawDataIndex[currentWidget] - Contains the  1st sensor
*  position in the widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
void CapSense_2_NormalizeWidgets(uint32 widgetsNum, uint8 dataPtr[])
{
    uint32 currentWidget;
    uint32 currentSensor;
    uint32 lastSensor;
    uint32 sensorsPerWidget;
    uint32 maxPerWidget;

    for(currentWidget = 0u; currentWidget < widgetsNum; currentWidget++)
    {
        maxPerWidget = 0u;

        sensorsPerWidget = CapSense_2_numberOfSensors[currentWidget];
        currentSensor = CapSense_2_rawDataIndex[currentWidget];
        lastSensor = currentSensor + sensorsPerWidget;

        while(currentSensor < lastSensor)
        {
            if(maxPerWidget < dataPtr[currentSensor])
            {
                maxPerWidget = dataPtr[currentSensor];
            }
            currentSensor++;
        }

        currentSensor = CapSense_2_rawDataIndex[currentWidget];

        while(currentSensor < lastSensor)
        {
            dataPtr[currentSensor] = (uint8)maxPerWidget;
            currentSensor++;
        }
    }
}
#endif /* (0u != CapSense_2_TOTAL_CENTROIDS_COUNT) */

/* [] END OF FILE */
