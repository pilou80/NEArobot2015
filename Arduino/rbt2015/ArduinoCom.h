#ifdef QT_CORE_LIB
    #define uint8 __UINT8_TYPE__
    #define uint16 __UINT8_TYPE__
    #define int16 __INT16_TYPE__
#endif
typedef enum{
	dataSetLidarSpeed, //uint16 speed in RPM  [RPi=>ARD]
	dataReportLidarSpeed, //uint16 speed in RPM  [RPi=>ARD]
	dataConfigStepper, //struct stepperConfig  [RPi=>ARD]
	dataCommandStepper, //struct stepperCommand  [RPi=>ARD]
	dataReportStepper, //struct stepperStatus   [ARD=>RPi]
	dataReportPower, //struct powerStatus [ARD=>RPi]
	dataReportInfraRed, //struct infraRedSensorStatus [ARD=>RPi]
	dataReportUltrasound //struct UltrasoundSensorStatus [ARD=>RPi]
}enumDataType; // Max 256 Types

typedef enum{
	stepFull,
	stepHalf,
	stepQuarter,
	stepEighth
}enumStepControl;

typedef struct{
	uint8 stepControl; //cast enumStepControl
	uint8 reset; //b0 => left || b1 => right
        uint16 minimumReportDelay; //in ms
        uint16 maximumReportDelay; //in ms
} structStepperConfig;

typedef enum{
        stepperCommandGoTo,
        stepperCommandIncrement,
} enumStepperCommandMode;

typedef struct{
	uint8 modeToValue; //cast enumStepperCommandMode
	uint8 accelerationLeft;
	uint8 accelerationRight;
	uint8 maxSpeedLeft; //StepBySecond
	uint8 maxSpeedRight; //StepBySecond
	int16 valueLeft;
	int16 valueRight;
} structStepperCommand;

typedef enum{
    telemeterIR0 = 0,
    telemeterIR1,
    telemeterIR2,

    telemeterUS0 =128,
    telemeterUS1,
    telemeterUS2,
    telemeterUS3
} enumTelemeter;

typedef struct{
    uint8 number; // 0 to 127 Infrared || 128 to 255 Ultrasound
    uint8 minimumReportDelay;
    uint8 maximumReportDelay;
    uint16 thresoldFilter; //How much increment before sending data
} structTelemeterConfig;

/*
B0 dataType	
B1 dataLength MSB
B2 dataLength LSB
Bn data
Bnx CRC16 MSB
Bnx CRC16 LSB
*/
