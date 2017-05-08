#ifndef PFC_TYPES_H_
#define PFC_TYPES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
  PFC_ERROR_NONE = 0,
  PFC_ERROR_MEMORY,
  PFC_ERROR_NULL_PARAMETER,
  PFC_ERROR_NOT_FOUND,
  PFC_ERROR_LIST,
  PFC_ERROR_ID_CONFLICT,
  PFC_ERROR_FILEIO,
  PFC_ERROR_XML,
  PFC_ERROR_UNSET,
} pfc_error;


typedef uint8_t pfc_size;

typedef enum
{
    PFC_ID_MIN = 0,
    PFC_ID_IgnitionTrailing_0 = 0x81,
    PFC_ID_IgnitionTrailing_1 = 0x82,
    PFC_ID_IgnitionTrailing_2 = 0x83,
    PFC_ID_IgnitionTrailing_3 = 0x84,
    PFC_ID_InjectorCorrection_0 = 0x86,
    PFC_ID_InjectorCorrection_1 = 0x87,
    PFC_ID_InjectorCorrection_2 = 0x88,
    PFC_ID_InjectorCorrection_3 = 0x89,
    PFC_ID_MapReference = 0x8a,
    PFC_ID_RevIdle = 0x8b,
    PFC_ID_Unkown8C = 0x8c,
    PFC_ID_InjectorSizeCorrection = 0x8d,
    PFC_ID_InjectorCrankingCorrection = 0x8e,
    PFC_ID_InjectorWaterTemperatureCorrection = 0x8f,
    PFC_ID_InjectorWaterBoostCorrection = 0x90,
    PFC_ID_InjectorAirBoostCorrection = 0x91,
    PFC_ID_InjectorPrimaryLag = 0x92,
    PFC_ID_InjectorAccelerationRPMCorrection = 0x93,
    PFC_ID_InjectorAccelerationTPSCorrection = 0x94,
    PFC_ID_Unkown95 = 0x95,
    PFC_ID_IgnitionAirTemperatureColdCorrection = 0x96,
    PFC_ID_IgnitionBoostSF01Correction = 0x97,
    PFC_ID_IgnitionWaterTemperatureCorrection = 0x98,
    PFC_ID_Unkown99 = 0x99,
    PFC_ID_IgnitionAirTemperatureWarmCorrection = 0x9a,
    PFC_ID_IgnitionDwell = 0x9b,
    PFC_ID_IgnitionBatteryVoltageCorrection = 0x9c,
    PFC_ID_IgnitionBoostSF12Correction = 0x9d,
    PFC_ID_AirflowVoltage = 0xa0,
    PFC_ID_AirflowCurve0 = 0xa1,
    PFC_ID_AirflowCurve1 = 0xa2,
    PFC_ID_AirflowCurve2 = 0xa3,
    PFC_ID_AirflowCurve3 = 0xa4,
    PFC_ID_AirflowCurve4 = 0xa5,
    PFC_ID_UnkownA6 = 0xa6,
    PFC_ID_AirflowWarning = 0xa7,
    PFC_ID_InjectorWarning = 0xa8,
    PFC_ID_KnockWarning = 0xa9,
    PFC_ID_O2Feedback = 0xaa,
    PFC_ID_BoostControl = 0xab,
    PFC_ID_Protection = 0xac,
    PFC_ID_TunerString = 0xad,
    PFC_ID_WarningControlFlagsStrings = 0xd6,
    PFC_ID_SensorStrings = 0xdd,
    PFC_ID_Ignition_0 = 0xc1,
    PFC_ID_Ignition_1 = 0xc2,
    PFC_ID_Ignition_2 = 0xc3,
    PFC_ID_Ignition_3 = 0xc4,
    PFC_ID_UnkownC6_0 = 0xc6,
    PFC_ID_UnkownC6_1 = 0xc7,
    PFC_ID_UnkownC6_2 = 0xc8,
    PFC_ID_UnkownC6_3 = 0xc9,
    PFC_ID_Advanced = 0xf0,
    PFC_ID_TypeString = 0xf3,
    PFC_ID_UnkownF4 = 0xf4,
    PFC_ID_VersionString = 0xf5,
    PFC_ID_MAX = 0x100,
} PFC_ID;


#endif /* PFC_TYPES_H_ */
