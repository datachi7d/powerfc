/*
 * old.h
 *
 *  Created on: 13/11/2016
 *      Author: sean
 */

#ifndef OLD_H_
#define OLD_H_

typedef uint8_t byte_temperature;
typedef uint16_t short_float;
typedef uint8_t byte_float;
typedef uint8_t byte_degree;
typedef uint8_t byte_temperature;
typedef uint8_t byte_RPM;

typedef char sensor_string[4];
typedef char control_string[16];

typedef struct __attribute__((__packed__))
{
    uint8_t integer : 1;
    uint8_t decimal : 6;
} byte16;

typedef struct __attribute__((__packed__))
{
    uint8_t integer : 1;
    uint8_t decimal : 7;
} byte17;

typedef struct __attribute__((__packed__))
{
    uint8_t integer : 2;
    uint8_t decimal : 6;
} byte26;

typedef struct __attribute__((__packed__))
{
    uint8_t integer;
    uint8_t decimal;
} word88;


typedef struct __attribute__((__packed__))
{
    byte_temperature OilWater[2][3];
    byte16 Unkown;
} PFC_OilervsWaterTemperature;

typedef struct __attribute__((__packed__))
{
    uint8_t data[10];
} PFC_Protection;

typedef struct __attribute__((__packed__))
{
    uint16_t RevLimit;
    uint16_t FC_AE;
    uint16_t FC_EL;
    uint16_t FC_AC;
    uint16_t Idle_AE;
    uint16_t Idle_EL;
    uint16_t Idle_AC;
} PFC_RevIdle;

typedef struct __attribute__((__packed__))
{
    uint16_t RevLimit;
    uint16_t FC_AE;
    uint16_t FC_EL;
    uint16_t FC_AC;
    uint16_t Idle_AE;
    uint16_t Idle_EL;
} PFC_RevIdle1;

typedef struct __attribute__((__packed__))
{
    uint8_t unknown;
    word88 O2Feedback;
} PFC_O2Feedback;

typedef struct __attribute__((__packed__))
{
    uint8_t flag[5];
    control_string control[5];
} PFC_WarningControlFlagsStrings;

typedef struct __attribute__((__packed__))
{
    uint16_t boost[20];
    uint16_t rpm[20];
} PFC_MapReference;

typedef struct __attribute__((__packed__))
{
    uint8_t unkown[4];
} PFC_AirflowWarning;

typedef struct __attribute__((__packed__))
{
    uint8_t unkown[4];
} PFC_InjectorWarning;

typedef struct __attribute__((__packed__))
{
    uint8_t unkown[4];
} PFC_KnockWarning;

typedef struct __attribute__((__packed__))
{
    byte26 WaterTemp80C[2];
    byte26 WaterTemp50C[2];
    byte26 WaterTemp30C[2];
    byte26 WaterTemp10C[2];
    byte26 WaterTempN10C[2];
    byte26 WaterTempN30C[2];
    uint8_t unkown[2];
} PFC_WaterTemperatureCorrection;

typedef struct __attribute__((__packed__))
{
    byte_temperature Temperature[2];
    byte17 Correction[2];
    byte26 Boost[2];
} PFC_InjectorWaterBoostCorrection;

typedef struct __attribute__((__packed__))
{
    byte_temperature Temperature[3];
    byte17 Correction[3];
    byte26 Boost[2];
} PFC_InjectorAirBoostCorrection;

typedef struct __attribute__((__packed__))
{
    short_float Lag[6];
} PFC_InjPrimaryLag;



typedef struct __attribute__((__packed__))
{
    byte_RPM RPM[5];
    short_float Amount[5];
    short_float Decay[5];
} PFC_AccelerationInjectorRPMCorrection;


typedef struct __attribute__((__packed__))
{
    uint8_t Input;
    word88 Setting;
} TPSSetting;

typedef struct __attribute__((__packed__))
{
    TPSSetting Values[3]
} PFC_AccelerationInjectorTPSCorrection;

typedef struct __attribute__((__packed__))
{
    short_float Cranking[6];
} PFC_CrankingInjectorCorrection;

typedef struct __attribute__((__packed__))
{
    short_float Percentage[6];
    short_float Lag[6];
} PFC_InjectorSizeCorrection;

typedef struct __attribute__((__packed__))
{
    byte_temperature Temperature;
    uint8_t Retard;
} IgnitionTemperatureSetting;

typedef struct __attribute__((__packed__))
{
    IgnitionTemperatureSetting Values[2];
} PFC_IgnitionAirTemperatureColdCorrection;

typedef struct __attribute__((__packed__))
{
    IgnitionTemperatureSetting Values[3];
} PFC_IgnitionAirTemperatureWarmCorrection;

typedef struct __attribute__((__packed__))
{
    IgnitionTemperatureSetting Values[2];
} PFC_IgnitionWaterTemperatureCorrection;


typedef struct __attribute__((__packed__))
{
    uint8_t boost[2];
} PFC_IgnitionBoostSF01Correction;

typedef struct __attribute__((__packed__))
{
    uint8_t boost[2];
} PFC_IgnitionBoostSF12Correction;


typedef struct __attribute__((__packed__))
{
    uint8_t dwell[6];
} PFC_IgnitionDwell;

typedef struct __attribute__((__packed__))
{
    byte26 values[6];
} PFC_IgnitionBatteryCorrection;

typedef struct __attribute__((__packed__))
{
    sensor_string sensors[20];
} PFC_SensorStrings;

typedef struct __attribute__((__packed__))
{
    uint16_t RPM;
    uint16_t PIM;
    uint16_t PIMVoltage;
    uint16_t TPSVoltage;
    uint16_t InjectorFP;
    uint16_t Injector;
    uint8_t Ignition;
    uint8_t Dwell;
    uint8_t Boost;
    uint8_t Duty;
    uint16_t Unkown1;
    uint8_t WaterTemperature;
    uint8_t AirTemperature;
    uint8_t Knock;
    uint8_t BatteryVoltage;
    uint16_t Speed;
    uint16_t Unkown2;
    uint8_t O2Sensor1;
    uint8_t O2Sensor2;
    uint16_t Unknown3;
} PFC_Advanced;

typedef struct __attribute__((__packed__))
{
    uint8_t ID;
    uint8_t Length;
} PFC_Header;




typedef enum
{
    PFCID_Alive                                 =0x01,
    PFCID_OilervsWaterTemperature               =0x7e,
    PFCID_IgnitionLeading0                      =0x76,
    PFCID_IgnitionLeading1                      =0x77,
    PFCID_IgnitionLeading2                      =0x78,
    PFCID_IgnitionLeading3                      =0x79,
    PFCID_IgnitionTrailing0                     =0x81,
    PFCID_IgnitionTrailing1                     =0x82,
    PFCID_IgnitionTrailing2                     =0x83,
    PFCID_IgnitionTrailing3                     =0x84,
    PFCID_InjectorCorrection0                   =0x86,
    PFCID_InjectorCorrection1                   =0x87,
    PFCID_InjectorCorrection2                   =0x88,
    PFCID_InjectorCorrection3                   =0x89,
    PFCID_MapReference                          =0x8a,
    PFCID_RevIdle1                              =0x8b,
    PFCID_InjectorSizeCorrection                =0x8d,
    PFCID_CrankingInjectorCorrection            =0x8e,
    PFCID_WaterTemperatureCorrection            =0x8f,
    PFCID_InjectorWaterBoostCorrection          =0x90,
    PFCID_InjectorAirBoostCorrection            =0x91,
    PFCID_InjPrimaryLag                         =0x92,
    PFCID_AccelerationInjectorRPMCorrection     =0x93,
    PFCID_AccelerationInjectorTPSCorrection     =0x94,
    PFCID_IgnitionAirTemperatureColdCorrection  =0x96,
    PFCID_IgnitionBoostSF01Correction           =0x97,
    PFCID_IgnitionWaterTemperatureCorrection    =0x98,
    PFCID_IgnitionAirTemperatureWarmCorrection  =0x9a,
    PFCID_IgnitionDwell                         =0x9b,
    PFCID_IgnitionBatteryCorrection             =0x9c,
    PFCID_IgnitionBoostSF12Correction           =0x9d,
    PFCID_AirflowVolage                         =0xa0,
    PFCID_AirflowCurve0                         =0xa1,
    PFCID_AirflowCurve1                         =0xa2,
    PFCID_AirflowCurve2                         =0xa3,
    PFCID_AirflowCurve3                         =0xa4,
    PFCID_AirflowCurve4                         =0xa5,
    PFCID_AirflowCurve5                         =0xa6,
    PFCID_AirflowWarning                        =0xa7,
    PFCID_InjectorWarning                       =0xa8,
    PFCID_KnockWarning                          =0xa9,
    PFCID_O2Feedback                            =0xaa,
    PFCID_Protection                            =0xac,
    PFCID_Tuner                                 =0xad,
    PFCID_BaseFuel0                             =0xb0,
    PFCID_BaseFuel1                             =0xb1,
    PFCID_BaseFuel2                             =0xb2,
    PFCID_BaseFuel3                             =0xb3,
    PFCID_BaseFuel4                             =0xb4,
    PFCID_BaseFuel5                             =0xb5,
    PFCID_BaseFuel6                             =0xb6,
    PFCID_BaseFuel7                             =0xb7,
    PFCID_RevIdle                               =0xb8,
    PFCID_UnkownD0                              =0xd0,
    PFCID_WarningControlFlagsStrings            =0xd6,
    PFCID_UnkownD7                              =0xd7,
    PFCID_UnkownD8                              =0xd8,
    PFCID_UnkownD9                              =0xd9,
    PFCID_SensorStrings                         =0xdd,
    PFCID_UnkownCA                              =0xca,
    PFCID_AirflowDataStrings                    =0xce,
    PFCID_Advanced                              =0xf0,
    PFCID_Acknowledged                          =0xf2,
    PFCID_Initialiaztion                        =0xf3,
    PFCID_Version                               =0xf5,
    PFCID_FCEdit                                =0xf6,
} PFCID;


typedef struct
{
    PFCID ID;
    uint8_t * data;
    uint8_t Length;
} PFC_Data;

PFC_OilervsWaterTemperature OilervsWaterTemperature = { 0 };
PFC_Protection Protection = { 0 };
PFC_RevIdle RevIdle = { 0 };
PFC_RevIdle1 RevIdle1 = { 0 };
PFC_O2Feedback O2Feedback = { 0 };
PFC_WarningControlFlagsStrings WarningControlFlags = { .flag = { 0, 0, 0, 0, 0 }, .control = {
        "1.Boost cntl kit",
        "2.Air-Flow Warn.",
        "3.Injector Warn.",
        "4.Knock Warning ",
        "5.O2 F/B Control",
}};

//        "1.Seq.Turbo Cont",
//        "2.Injector Warn.",
//        "3.Knock Warning ",
//        "4.O2 F/B Control",
//        "5.Idle-IG Cntrl ",
//        }};


short_float BaseFuel[20][20] = { 0 };
#define PFC_BASE_FUEL_LINES 8
#define PFC_BASE_FUEL_LINE_SIZE (sizeof(BaseFuel) / PFC_BASE_FUEL_LINES)

byte_float InjectorCorrection[20][20] = { 0 };
#define PFC_INJ_COR_LINES 4
#define PFC_INJ_COR_LINE_SIZE (sizeof(InjectorCorrection) / PFC_INJ_COR_LINES)

byte_degree IgnitionLeading[20][20] = { 0 };
#define PFC_IGN_LEAD_LINES 4
#define PFC_IGN_LEAD_LINE_SIZE (sizeof(IgnitionLeading) / PFC_IGN_LEAD_LINES)

byte_degree IgnitionTrailing[20][20] = { 0 };
#define PFC_IGN_TRAIL_LINES 4
#define PFC_IGN_TRAIL_LINE_SIZE (sizeof(IgnitionTrailing) / PFC_IGN_TRAIL_LINES)

PFC_MapReference MapReference = { 0 };
PFC_AirflowWarning AirflowWarning = { 0 };
PFC_InjectorWarning InjectorWarning = { 0 };
PFC_KnockWarning KnockWarning = { 0 };

PFC_InjectorSizeCorrection InjectorSizeCorrection = { 0 };
PFC_CrankingInjectorCorrection CrankingInjectorCorrection = { 0 };
PFC_WaterTemperatureCorrection WaterTemperatureCorrection = { 0 };
PFC_InjectorWaterBoostCorrection         InjectorWaterBoostCorrection       = { 0 };
PFC_InjectorAirBoostCorrection           InjectorAirBoostCorrection         = { 0 };
PFC_InjPrimaryLag                        InjPrimaryLag                      = { 0 };
PFC_AccelerationInjectorRPMCorrection    AccelerationInjectorRPMCorrection  = { 0 };
PFC_AccelerationInjectorTPSCorrection    AccelerationInjectorTPSCorrection  = { 0 };

PFC_IgnitionAirTemperatureColdCorrection IgnitionAirTemperatureColdCorrection = { 0 };
PFC_IgnitionAirTemperatureWarmCorrection IgnitionAirTemperatureWarmCorrection = { 0 };
PFC_IgnitionWaterTemperatureCorrection IgnitionWaterTemperatureCorrection = { 0 };
PFC_IgnitionDwell IgnitionDwell = { 0 };
PFC_IgnitionBatteryCorrection IgnitionBatteryCorrection = { 0 };
PFC_IgnitionBoostSF01Correction IgnitionBoostSF01Correction = { 0 };
PFC_IgnitionBoostSF12Correction IgnitionBoostSF12Correction = { 0 };

PFC_SensorStrings SensorStrings = { 0 };

PFC_Advanced Advanced = { .BatteryVoltage = 125 };

static const uint8_t InitialiaztionResponse[] = {0x1d, 0x32, 0x4b};
static char Initialiaztion[] = "SR20DET1";
static const char Version[] = "5.03 ";
static char Tuner[8] = "        ";
static const uint8_t Alive = 1;


static uint8_t dummy[0xff] = {0};

static uint8_t UnkownD0[] = { 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80 };
static uint8_t UnkownD7[] = { 0x07, 0x07, 0x09, 0x01, 0x06, 0x07, 0x02, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x03, 0x91, 0x01 };
static uint8_t UnkownD8 = 0x28;
static uint8_t UnkownD9 = 0x14;
static uint8_t UnkownCA[] = { 0x00, 0x00, 0x8B, 0x05, 0xDE, 0x08, 0x17, 0x0B, 0x4D, 0x0D, 0x85, 0x0F, 0xBC, 0x11, 0x2C, 0x16, 0x9B, 0x1A, 0x0C, 0x1F, 0x79, 0x23, 0xE8, 0x27, 0x57, 0x2C, 0x36, 0x35, 0x14, 0x3E, 0xF2, 0x46, 0xD0, 0x4F, 0xAF, 0x58, 0x8D, 0x61, 0x6B, 0x6A, 0x20, 0x03, 0xB0, 0x04, 0x40, 0x06, 0xD0, 0x07, 0x60, 0x09, 0xF0, 0x0A, 0x80, 0x0C, 0x42, 0x0E, 0xA0, 0x0F, 0x30, 0x11, 0xC0, 0x12, 0x50, 0x14, 0xE0, 0x15, 0x70, 0x17, 0x00, 0x19, 0x90, 0x1A, 0x20, 0x1C, 0xB0, 0x1D, 0x40, 0x1F, 0xD0, 0x20 };

static PFC_Data data[] = {
                                    {.ID = PFCID_Alive,                      .data = (uint8_t*)&Alive,                                                  .Length = sizeof(Alive)},

                                    {.ID = PFCID_UnkownD0,                   .data = (uint8_t*)&UnkownD0[0],                                               .Length = sizeof(UnkownD0)},
                                    {.ID = PFCID_UnkownD7,                   .data = (uint8_t*)&UnkownD7[0],                                               .Length = sizeof(UnkownD7)},
                                    {.ID = PFCID_UnkownD8,                   .data = (uint8_t*)&UnkownD8,                                               .Length = sizeof(UnkownD8)},
                                    {.ID = PFCID_UnkownD9,                   .data = (uint8_t*)&UnkownD9,                                               .Length = sizeof(UnkownD9)},
                                    {.ID = PFCID_UnkownCA,                   .data = (uint8_t*)&UnkownCA[0],                                               .Length = sizeof(UnkownCA)},




                                    {.ID = PFCID_OilervsWaterTemperature,    .data = (uint8_t*)&OilervsWaterTemperature,                                .Length = sizeof(OilervsWaterTemperature)},

                                    {.ID = PFCID_IgnitionLeading0,           .data = ((uint8_t*)&IgnitionLeading[0]) + (0 * PFC_IGN_LEAD_LINE_SIZE),    .Length = PFC_IGN_LEAD_LINE_SIZE },
                                    {.ID = PFCID_IgnitionLeading1,           .data = ((uint8_t*)&IgnitionLeading[0]) + (1 * PFC_IGN_LEAD_LINE_SIZE),    .Length = PFC_IGN_LEAD_LINE_SIZE },
                                    {.ID = PFCID_IgnitionLeading2,           .data = ((uint8_t*)&IgnitionLeading[0]) + (2 * PFC_IGN_LEAD_LINE_SIZE),    .Length = PFC_IGN_LEAD_LINE_SIZE },
                                    {.ID = PFCID_IgnitionLeading3,           .data = ((uint8_t*)&IgnitionLeading[0]) + (3 * PFC_IGN_LEAD_LINE_SIZE),    .Length = PFC_IGN_LEAD_LINE_SIZE },

                                    {.ID = PFCID_IgnitionTrailing0,          .data = ((uint8_t*)&IgnitionTrailing[0]) + (0 * PFC_INJ_COR_LINE_SIZE),    .Length = PFC_INJ_COR_LINE_SIZE },
                                    {.ID = PFCID_IgnitionTrailing1,          .data = ((uint8_t*)&IgnitionTrailing[0]) + (1 * PFC_INJ_COR_LINE_SIZE),    .Length = PFC_INJ_COR_LINE_SIZE },
                                    {.ID = PFCID_IgnitionTrailing2,          .data = ((uint8_t*)&IgnitionTrailing[0]) + (2 * PFC_INJ_COR_LINE_SIZE),    .Length = PFC_INJ_COR_LINE_SIZE },
                                    {.ID = PFCID_IgnitionTrailing3,          .data = ((uint8_t*)&IgnitionTrailing[0]) + (3 * PFC_INJ_COR_LINE_SIZE),    .Length = PFC_INJ_COR_LINE_SIZE },

                                    {.ID = PFCID_InjectorCorrection0,        .data = ((uint8_t*)&InjectorCorrection[0]) + (0 * PFC_INJ_COR_LINE_SIZE),  .Length = PFC_INJ_COR_LINE_SIZE },
                                    {.ID = PFCID_InjectorCorrection1,        .data = ((uint8_t*)&InjectorCorrection[0]) + (1 * PFC_INJ_COR_LINE_SIZE),  .Length = PFC_INJ_COR_LINE_SIZE },
                                    {.ID = PFCID_InjectorCorrection2,        .data = ((uint8_t*)&InjectorCorrection[0]) + (2 * PFC_INJ_COR_LINE_SIZE),  .Length = PFC_INJ_COR_LINE_SIZE },
                                    {.ID = PFCID_InjectorCorrection3,        .data = ((uint8_t*)&InjectorCorrection[0]) + (3 * PFC_INJ_COR_LINE_SIZE),  .Length = PFC_INJ_COR_LINE_SIZE },

                                    {.ID = PFCID_MapReference,               .data = (uint8_t*)&MapReference,                                           .Length = sizeof(MapReference)},\
                                    {.ID = PFCID_RevIdle1,                   .data = (uint8_t*)&RevIdle1,                                               .Length = sizeof(RevIdle1)},

                                    {.ID = PFCID_InjectorSizeCorrection, .data = (uint8_t*)&InjectorSizeCorrection,                                     .Length = sizeof(InjectorSizeCorrection)},
                                    {.ID = PFCID_CrankingInjectorCorrection, .data = (uint8_t*)&CrankingInjectorCorrection,                             .Length = sizeof(CrankingInjectorCorrection)},
                                    {.ID = PFCID_WaterTemperatureCorrection, .data = (uint8_t*)&WaterTemperatureCorrection,                             .Length = sizeof(WaterTemperatureCorrection)},
                                    {.ID = PFCID_InjectorWaterBoostCorrection,       .data = (uint8_t*)&InjectorWaterBoostCorrection     ,                             .Length = sizeof(InjectorWaterBoostCorrection     )},
                                    {.ID = PFCID_InjectorAirBoostCorrection,         .data = (uint8_t*)&InjectorAirBoostCorrection       ,                             .Length = sizeof(InjectorAirBoostCorrection       )},
                                    {.ID = PFCID_InjPrimaryLag,                      .data = (uint8_t*)&InjPrimaryLag                    ,                             .Length = sizeof(InjPrimaryLag                    )},
                                    {.ID = PFCID_AccelerationInjectorRPMCorrection,  .data = (uint8_t*)&AccelerationInjectorRPMCorrection,                             .Length = sizeof(AccelerationInjectorRPMCorrection)},
                                    {.ID = PFCID_AccelerationInjectorTPSCorrection,  .data = (uint8_t*)&AccelerationInjectorTPSCorrection,                             .Length = sizeof(AccelerationInjectorTPSCorrection)},

                                    {.ID = PFCID_IgnitionAirTemperatureColdCorrection,  .data = (uint8_t*)&IgnitionAirTemperatureColdCorrection,                             .Length = sizeof(IgnitionAirTemperatureColdCorrection)},
                                    {.ID = PFCID_IgnitionAirTemperatureWarmCorrection,  .data = (uint8_t*)&IgnitionAirTemperatureWarmCorrection,                             .Length = sizeof(IgnitionAirTemperatureWarmCorrection)},
                                    {.ID = PFCID_IgnitionWaterTemperatureCorrection,  .data = (uint8_t*)&IgnitionWaterTemperatureCorrection,                             .Length = sizeof(IgnitionWaterTemperatureCorrection)},
                                    {.ID = PFCID_IgnitionDwell,  .data = (uint8_t*)&IgnitionDwell,                             .Length = sizeof(IgnitionDwell)},
                                    {.ID = PFCID_IgnitionBatteryCorrection,  .data = (uint8_t*)&IgnitionBatteryCorrection,                             .Length = sizeof(IgnitionBatteryCorrection)},
                                    {.ID = PFCID_IgnitionBoostSF01Correction,  .data = (uint8_t*)&IgnitionBoostSF01Correction,                             .Length = sizeof(IgnitionBoostSF01Correction)},
                                    {.ID = PFCID_IgnitionBoostSF12Correction,  .data = (uint8_t*)&IgnitionBoostSF12Correction,                             .Length = sizeof(IgnitionBoostSF12Correction)},



                                    {.ID = PFCID_AirflowVolage,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x40},
                                    {.ID = PFCID_AirflowCurve0,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x40},
                                    {.ID = PFCID_AirflowCurve1,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x40},
                                    {.ID = PFCID_AirflowCurve2,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x40},
                                    {.ID = PFCID_AirflowCurve3,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x40},
                                    {.ID = PFCID_AirflowCurve3,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x40},
                                    {.ID = PFCID_AirflowCurve4,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x40},
                                    {.ID = PFCID_AirflowCurve5,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x40},

                                    {.ID = PFCID_AirflowWarning,             .data = (uint8_t*)&AirflowWarning,                                         .Length = sizeof(AirflowWarning)},
                                    {.ID = PFCID_InjectorWarning,            .data = (uint8_t*)&InjectorWarning,                                        .Length = sizeof(InjectorWarning)},
                                    {.ID = PFCID_KnockWarning,               .data = (uint8_t*)&KnockWarning,                                           .Length = sizeof(KnockWarning)},
                                    {.ID = PFCID_O2Feedback,                 .data = (uint8_t*)&O2Feedback,                                             .Length = sizeof(O2Feedback)},
                                    {.ID = PFCID_Protection,                 .data = (uint8_t*)&Protection,                                             .Length = sizeof(Protection)},
                                    {.ID = PFCID_Tuner,                      .data = (uint8_t*)&Tuner,                                                  .Length = sizeof(Tuner)},

                                    {.ID = PFCID_BaseFuel0,                  .data = ((uint8_t*)&BaseFuel[0]) + (0 * PFC_BASE_FUEL_LINE_SIZE),          .Length = PFC_BASE_FUEL_LINE_SIZE },
                                    {.ID = PFCID_BaseFuel1,                  .data = ((uint8_t*)&BaseFuel[0]) + (1 * PFC_BASE_FUEL_LINE_SIZE),          .Length = PFC_BASE_FUEL_LINE_SIZE },
                                    {.ID = PFCID_BaseFuel2,                  .data = ((uint8_t*)&BaseFuel[0]) + (2 * PFC_BASE_FUEL_LINE_SIZE),          .Length = PFC_BASE_FUEL_LINE_SIZE },
                                    {.ID = PFCID_BaseFuel3,                  .data = ((uint8_t*)&BaseFuel[0]) + (3 * PFC_BASE_FUEL_LINE_SIZE),          .Length = PFC_BASE_FUEL_LINE_SIZE },
                                    {.ID = PFCID_BaseFuel4,                  .data = ((uint8_t*)&BaseFuel[0]) + (4 * PFC_BASE_FUEL_LINE_SIZE),          .Length = PFC_BASE_FUEL_LINE_SIZE },
                                    {.ID = PFCID_BaseFuel5,                  .data = ((uint8_t*)&BaseFuel[0]) + (5 * PFC_BASE_FUEL_LINE_SIZE),          .Length = PFC_BASE_FUEL_LINE_SIZE },
                                    {.ID = PFCID_BaseFuel6,                  .data = ((uint8_t*)&BaseFuel[0]) + (6 * PFC_BASE_FUEL_LINE_SIZE),          .Length = PFC_BASE_FUEL_LINE_SIZE },
                                    {.ID = PFCID_BaseFuel7,                  .data = ((uint8_t*)&BaseFuel[0]) + (7 * PFC_BASE_FUEL_LINE_SIZE),          .Length = PFC_BASE_FUEL_LINE_SIZE },

                                    {.ID = PFCID_RevIdle,                    .data = (uint8_t*)&RevIdle,                                                .Length = sizeof(RevIdle)},
                                    {.ID = PFCID_WarningControlFlagsStrings, .data = (uint8_t*)&WarningControlFlags,                                    .Length = sizeof(WarningControlFlags)},




                                    {.ID = PFCID_SensorStrings,                   .data = (uint8_t*)&SensorStrings,                                               .Length = sizeof(SensorStrings)},

                                    {.ID = PFCID_AirflowDataStrings,                   .data = (uint8_t*)&dummy[0],                                               .Length = 0x6b},

                                    {.ID = PFCID_Advanced,     .data = (uint8_t*)&Advanced,                                 .Length = sizeof(Advanced)},
                                    {.ID = PFCID_Initialiaztion,             .data = (uint8_t*)&Initialiaztion,                                         .Length = sizeof(Initialiaztion)-1},
                                    {.ID = PFCID_Version,                    .data = (uint8_t*)&Version,                                                .Length = sizeof(Version)-1},
                                    {.ID = PFCID_FCEdit,                    .data = (uint8_t*)&dummy[0],                                                .Length = 0x1},
};


#endif /* OLD_H_ */
