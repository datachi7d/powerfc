/*
 * PFC_Datastructures.h
 *
 *  Created on: 5/11/2016
 *      Author: sean
 */

#ifndef PFCDATA_H_
#define PFCDATA_H_

#include <stdint.h>
#include <stddef.h>

#define PFC_INVALID_ID (sizeof(PFC_Data)+1)

typedef uint8_t byte;
typedef uint8_t byte_temperature;
typedef uint8_t byte_voltage;
typedef uint8_t byte_float;
typedef uint8_t byte_percentage;
typedef uint8_t byte_degree;
typedef uint8_t byte_temperature;
typedef uint8_t byte_RPM;
typedef uint8_t byte_boost;

typedef uint16_t short_float;
typedef uint16_t short_speed;
typedef uint16_t short_percentage;
typedef uint16_t short_millisecond;
typedef uint16_t short_word;
typedef uint16_t short_boost;
typedef uint16_t short_voltage;
typedef uint16_t short_RPM;

typedef uint8_t control_flag;
typedef char sensor_string[4];
typedef char control_string[16];
typedef char type_string[8];
typedef char version_string[5];


typedef struct __attribute__((__packed__))
{
    uint8_t ID;
    uint8_t Length;
} PFC_Header;


typedef struct __attribute__((__packed__))
{
    uint8_t integer : 1;
    uint8_t decimal : 6;
} byte16;

//typedef struct __attribute__((__packed__))
//{
//    uint8_t integer : 1;
//    uint8_t decimal : 7;
//} byte17;

//typedef struct __attribute__((__packed__))
//{
//    uint8_t integer : 2;
//    uint8_t decimal : 6;
//} byte26;

//typedef struct __attribute__((__packed__))
//{
//    uint8_t integer;
//    uint8_t decimal;
//} word88;


typedef uint8_t byte17;
typedef uint8_t byte26;
typedef uint16_t word88;


typedef struct __attribute__((__packed__))
{
    uint8_t Input;
    word88 Setting;
} TPSSetting;

typedef struct __attribute__((__packed__))
{
    byte_temperature Temperature;
    byte_degree Retard;
} IgnitionTemperatureSetting;


static inline uint16_t convertShort(uint16_t value)
{
//    uint8_t * data = (uint8_t *) &value;
//
//    return (data[0] << 8) + data[1];


    return value;
}



static inline char * TPSSetting_toString(TPSSetting * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%d, %6f", value->Input, ((float)convertShort(value->Setting))/256.0f);
    return buffer;
}



static inline char * word88_toString(word88 * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%6f", ((float)convertShort(*value))/256.0f);
    return buffer;
}

static inline char * byte_toString(byte * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%d", *value);
    return buffer;
}

static inline char * byte_RPM_toString(byte_RPM * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%d", ((int)*value)*40);
    return buffer;
}


static inline char * byte_float_toString(byte_float * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%3.1f", ((float)((*value)*0.3898f)+49.697f));

    return buffer;
}

static inline char * byte_percentage_toString(byte_percentage * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%3.1f %%", ((float)((*value)/255.0f)*100.0f));

    return buffer;
}

static inline char * byte_voltage_toString(byte_voltage * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%1.2f V", ((float)((*value)*2.0f)/100.0f));

    return buffer;
}


static inline char * byte_boost_toString(byte_boost * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%1.5f kg/cm2", ((float)*value)/64.0f  );

    return buffer;
}


static inline char * byte17_toString(byte17 * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%1.5f", ((float)*value)/64.0f  );

    return buffer;
}


static inline char * byte26_toString(byte26 * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%1.5f", ((float)*value)/128.0f  );

    return buffer;
}

static inline char * short_boost_toString(short_boost * value)
{
    static char buffer[20] = { 0 };
    uint16_t short_value = convertShort(*value);

    if(short_value & 0x8000)
    {
        snprintf(buffer, sizeof(buffer), "%2.2f psi", (((float)(short_value&0xff))/100.0f)*14.2233f);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "%3d mmHg", ((int)(short_value&0x3ff))-760);
    }



    return buffer;
}

static inline char * short_voltage_toString(short_voltage * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%2.3f V", ((float)(convertShort(*value)))/(1000.0f));

    return buffer;
}

static inline char * short_speed_toString(short_speed * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%d km/h", (convertShort(*value)));

    return buffer;
}

static inline char * short_percentage_toString(short_percentage * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%3.3f %%", ((float)(convertShort(*value))/200.0f));

    return buffer;
}

static inline char * short_millisecond_toString(short_millisecond * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%3.3f msec", (((float)(convertShort(*value)))*4.0f)/(1000.0f));

    return buffer;
}

static inline char * short_float_toString(short_float * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%6f", (((float)(convertShort(*value)))*4.0f)/(1000.0f));

    return buffer;
}

static inline char * short_RPM_toString(short_RPM * RPM)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%d RPM", convertShort(*RPM));
    return buffer;
}

static inline char * short_word_toString(short_word * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%d", convertShort(*value));
    return buffer;
}


static inline char * byte_temperature_toString(byte_temperature * value)
{
    static char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "%d °C", ((int)*value) - 80);
    return buffer;
}

static inline char * control_flag_toString(control_flag * flag)
{
    static char buffer[5] = { 0 };
    snprintf(buffer, sizeof(buffer), "%s", *flag == 0xFF ? "ON" : "OFF");
    return buffer;
}

static inline char * sensor_string_toString(sensor_string * string)
{
    static char buffer[sizeof(sensor_string) + 1] = { 0 };
    memcpy(buffer, string, sizeof(sensor_string));
    return buffer;
}

static inline char * control_string_toString(control_string * string)
{
    static char buffer[sizeof(control_string) + 1] = { 0 };
    memcpy(buffer, string, sizeof(control_string));
    return buffer;
}

static inline char * type_string_toString(type_string * string)
{
    static char buffer[sizeof(type_string) + 1] = { 0 };
    memcpy(buffer, string, sizeof(type_string));
    return buffer;
}

static inline char * version_string_toString(version_string * string)
{
    static char buffer[sizeof(version_string) + 1] = { 0 };
    memcpy(buffer, string, sizeof(version_string));
    return buffer;
}

static inline char * byte_degree_toString(byte_degree * degree)
{
    static char buffer[6] = { 0 };
    snprintf(buffer, sizeof(buffer), "%d", *degree);
    return buffer;
}


static inline char * IgnitionTemperatureSetting_toString(IgnitionTemperatureSetting * value)
{
    static char buffer[40] = { 0 };
    char * temp = byte_temperature_toString(&value->Temperature);
    char * retard = byte_degree_toString(&value->Retard);

    snprintf(buffer, sizeof(buffer), "%s, %s", temp, retard);
    return buffer;
}



#define PFCStructs \
    _PFCID_Map(IgnitionTrailing, byte_degree, 20, 20, 4,        0x81) \
    \
    \
    _PFCID_Map(InjectorCorrection, byte_float, 20, 20, 4,       0x86) \
    \
    \
    _PFCID_Start_Struct(MapReference,                           0x8a) \
    _ValueArray(Boost, short_boost, 20) \
    _ValueArray(RPM, short_RPM, 20) \
    _PFCID_End_Struct(MapReference, 0x8a) \
    \
    \
    _PFCID_Start_Struct(RevIdle,                                0x8b) \
    _Value(RevLimit, short_RPM) \
    _Value(FuelCut, short_RPM) \
    _Value(FuelCutAC, short_RPM) \
    _Value(Idle, short_RPM) \
    _Value(IdleAC, short_RPM) \
    _Value(Unkown, short_RPM) \
    _PFCID_End_Struct(RevIdle, 0x8b) \
    \
    \
    _PFCID_Start_Struct(Unkown8C,                               0x8c) \
    _Value(Unkown, short_word) \
    _PFCID_End_Struct(Unkown8C, 0x8c) \
    \
    \
    _PFCID_Start_Struct(InjectorSizeCorrection,                0x8d) \
    _ValueArray(Percentage, short_float, 6) \
    _ValueArray(Lag, short_float, 6) \
    _PFCID_End_Struct(InjectorSizeCorrection, 0x8d) \
    \
    \
    _PFCID_Start_Struct(InjectorCrankingCorrection,            0x8e) \
    _ValueArray(Cranking, short_float, 6) \
    _PFCID_End_Struct(InjectorCrankingCorrection, 0x8e) \
    \
    \
    _PFCID_Start_Struct(InjectorWaterTemperatureCorrection,    0x8f) \
    _ValueArray(WaterTemp80C, byte26, 2) \
    _ValueArray(WaterTemp50C, byte26, 2) \
    _ValueArray(WaterTemp30C, byte26, 2) \
    _ValueArray(WaterTemp10C, byte26, 2) \
    _ValueArray(WaterTempN10C, byte26, 2) \
    _ValueArray(WaterTempN30C, byte26, 2) \
    _ValueArray(unkown, byte, 2) \
    _PFCID_End_Struct(InjectorWaterTemperatureCorrection, 0x8f) \
    \
    \
    _PFCID_Start_Struct(InjectorWaterBoostCorrection,           0x90) \
    _ValueArray(Temperature, byte_temperature, 2) \
    _ValueArray(Correction, byte17, 2) \
    _ValueArray(Boost, byte26, 2) \
    _PFCID_End_Struct(InjectorWaterBoostCorrection, 0x90) \
    \
    \
    _PFCID_Start_Struct(InjectorAirBoostCorrection,             0x91) \
    _ValueArray(Temperature, byte_temperature, 3) \
    _ValueArray(Correction, byte17, 3) \
    _ValueArray(Boost, byte26, 2) \
    _PFCID_End_Struct(InjectorAirBoostCorrection, 0x91) \
    \
    \
    _PFCID_Start_Struct(InjectorPrimaryLag,                     0x92) \
    _ValueArray(Lag, short_float, 6) \
    _PFCID_End_Struct(InjectorPrimaryLag, 0x92) \
    \
    \
    _PFCID_Start_Struct(InjectorAccelerationRPMCorrection,      0x93) \
    _ValueArray(RPM, byte_RPM, 5) \
    _ValueArray(Amount, short_float, 5) \
    _ValueArray(Decay, short_float, 5) \
    _PFCID_End_Struct(InjectorAccelerationRPMCorrection, 0x93) \
    \
    \
    _PFCID_Start_Struct(InjectorAccelerationTPSCorrection,      0x94) \
    _ValueArray(Values, TPSSetting, 3) \
    _PFCID_End_Struct(InjectorAccelerationTPSCorrection, 0x94) \
    \
    \
    _PFCID_Start_Struct(Unkown95,                               0x95) \
    _ValueArray(Unkown, short_word, 2) \
    _PFCID_End_Struct(Unkown95, 0x95) \
    \
    \
    _PFCID_Start_Struct(IgnitionAirTemperatureColdCorrection,      0x96) \
    _ValueArray(Values, IgnitionTemperatureSetting, 2) \
    _PFCID_End_Struct(IgnitionAirTemperatureColdCorrection, 0x96) \
    \
    \
    _PFCID_Start_Struct(IgnitionBoostSF01Correction,      0x97) \
    _ValueArray(boost, byte_boost, 2) \
    _PFCID_End_Struct(IgnitionBoostSF01Correction, 0x97) \
    \
    \
    _PFCID_Start_Struct(IgnitionWaterTemperatureCorrection,      0x98) \
    _ValueArray(Values, IgnitionTemperatureSetting, 2) \
    _PFCID_End_Struct(IgnitionWaterTemperatureCorrection, 0x98) \
    \
    \
    _PFCID_Start_Struct(Unkown99,                                   0x99) \
    _ValueArray(Unkown, short_word, 4) \
    _PFCID_End_Struct(Unkown99, 0x99) \
    \
    \
    _PFCID_Start_Struct(IgnitionAirTemperatureWarmCorrection,      0x9a) \
    _ValueArray(Values, IgnitionTemperatureSetting, 3) \
    _PFCID_End_Struct(IgnitionAirTemperatureWarmCorrection, 0x9a) \
    \
    \
    _PFCID_Start_Struct(IgnitionDwell,                               0x9b) \
    _ValueArray(Ignition, byte_degree, 6) \
    _PFCID_End_Struct(IgnitionDwell, 0x9b) \
    \
    \
    _PFCID_Start_Struct(IgnitionBatteryVoltageCorrection,          0x9c) \
    _ValueArray(Ignition, byte26, 6) \
    _PFCID_End_Struct(IgnitionBatteryVoltageCorrection, 0x9c) \
    \
    \
    _PFCID_Start_Struct(IgnitionBoostSF12Correction,      0x9d) \
    _ValueArray(boost, byte_boost, 2) \
    _PFCID_End_Struct(IgnitionBoostSF12Correction, 0x9d) \
    \
    \
    _PFCID_Start_Struct(AirflowVoltage,      0xa0) \
    _ValueArray(Voltage, short_float, 32) \
    _PFCID_End_Struct(AirflowVoltage, 0xa0) \
    \
    \
    _PFCID_Start_Struct(AirflowCurve0,      0xa1) \
    _ValueArray(Value, short_float, 32) \
    _PFCID_End_Struct(AirflowCurve0, 0xa1) \
    \
    \
    _PFCID_Start_Struct(AirflowCurve1,      0xa2) \
    _ValueArray(Value, short_float, 32) \
    _PFCID_End_Struct(AirflowCurve1, 0xa2) \
    \
    \
    _PFCID_Start_Struct(AirflowCurve2,      0xa3) \
    _ValueArray(Value, short_float, 32) \
    _PFCID_End_Struct(AirflowCurve2, 0xa3) \
    \
    \
    _PFCID_Start_Struct(AirflowCurve3,      0xa4) \
    _ValueArray(Value, short_float, 32) \
    _PFCID_End_Struct(AirflowCurve3, 0xa4) \
    \
    \
    _PFCID_Start_Struct(AirflowCurve4,      0xa5) \
    _ValueArray(Value, short_float, 32) \
    _PFCID_End_Struct(AirflowCurve4, 0xa5) \
    \
    \
    _PFCID_Start_Struct(UnkownA6,      0xa6) \
    _Value(Unkown, byte) \
    _PFCID_End_Struct(UnkownA6, 0xa6) \
    \
    \
    _PFCID_Start_Struct(AirflowWarning,      0xa7) \
    _ValueArray(Value, byte, 4) \
    _PFCID_End_Struct(AirflowWarning, 0xa7) \
    \
    \
    _PFCID_Start_Struct(InjectorWarning,      0xa8) \
    _ValueArray(Value, byte, 4) \
    _PFCID_End_Struct(InjectorWarning, 0xa8) \
    \
    \
    _PFCID_Start_Struct(KnockWarning,      0xa9) \
    _ValueArray(Value, byte, 4) \
    _PFCID_End_Struct(KnockWarning, 0xa9) \
    \
    \
    _PFCID_Start_Struct(O2Feedback,                            0xaa) \
    _Value(Unkown, byte) \
    _Value(Feedback, word88) \
    _PFCID_End_Struct(O2Feedback, 0xaa) \
    \
    \
    _PFCID_Start_Struct(BoostControl,      0xab) \
    _ValueArray(Unkown, byte, 3) \
    _ValueArray(Boost, byte_boost, 4) \
    _ValueArray(Duty, byte, 4) \
    _PFCID_End_Struct(BoostControl, 0xab) \
    \
    \
    _PFCID_Start_Struct(Protection,      0xac) \
    _ValueArray(Value, byte, 10) \
    _PFCID_End_Struct(Protection, 0xac) \
    \
    \
    _PFCID_Start_Struct(TunerString,                            0xad) \
    _Value(Tuner, type_string) \
    _PFCID_End_Struct(TunerString, 0xad) \
    \
    \
    _PFCID_Start_Struct(WarningControlFlagsStrings,             0xd6) \
    _ValueArray(Flag, control_flag, 5) \
    _ValueArray(Control, control_string, 5) \
    _PFCID_End_Struct(WarningControlFlagsStrings, 0xd6) \
    \
    \
    _PFCID_Start_Struct(SensorStrings,                          0xdd) \
    _ValueArray(Sensor, sensor_string, 20) \
    _PFCID_End_Struct(SensorStrings, 0xdd) \
    \
    \
    _PFCID_Map(Ignition, byte_degree, 20, 20, 4,                0xc1) \
    \
    \
    _PFCID_Map(UnkownC6, byte_degree, 20, 20, 4,                0xc6) \
    \
    \
    _PFCID_Start_Struct(Advanced,                               0xf0) \
    _Value(RPM, short_RPM) \
    _Value(EngineLoad, short_word) \
    _Value(AirFlow1, short_voltage) \
    _Value(AirFlow2, short_voltage) \
    _Value(Injector, short_millisecond) \
    _Value(FuelCorrection, short_float) \
    _Value(Ignition, byte_degree) \
    _Value(IgnitionDwell, byte_degree) \
    _Value(Boost, short_boost) \
    _Value(BoostDuty, short_percentage) \
    _Value(WaterTemperature, byte_temperature) \
    _Value(AirTemperature, byte_temperature) \
    _Value(KnockLevel, byte) \
    _Value(BatteryVoltage, byte) \
    _Value(Speed, short_speed) \
    _Value(AirflowUsage, short_word) \
    _Value(O2Sensor1, byte_voltage) \
    _Value(O2Sensor2, byte_voltage) \
    _Value(ThrottleVoltage, short_voltage) \
    _PFCID_End_Struct(Advanced, 0xf0) \
    \
    \
    _PFCID_Start_Struct(TypeString,                             0xf3) \
    _Value(Type, type_string) \
    _PFCID_End_Struct(TypeString, 0xf3) \
    \
    \
    _PFCID_Start_Struct(UnkownF4,                               0xf4) \
    _Value(Unkown, byte) \
    _PFCID_End_Struct(UnkownF4, 0xf4) \
    \
    \
    _PFCID_Start_Struct(VersionString,                          0xf5) \
    _Value(Version, version_string) \
    _PFCID_End_Struct(VersionString, 0xf5) \



#define _PFCID_Start_Struct(Name, ID) \
    typedef struct __attribute__((__packed__)) \
    { \

#define _PFCID_End_Struct(Name, ID) \
    } PFC_##Name; \

#define _PFCID_Map(Name, type, rows, columns, ID_count, ID) \
    typedef type PFC_ ## Name[rows][columns]; \


#define _ValueArray(Name, type, size) \
        type Name[size]; \

#define _Value(Name, type) \
        type Name; \

PFCStructs

#include "xstruct_undef.h"

#define _PFCID_Start_Struct(Name, ID) PFC_##Name Name;
#define _PFCID_Map(Name, type, rows, columns,  ID_count, ID) PFC_##Name Name;

#define _PFCID_End_Struct(Name, ID)
#define _ValueArray(Name, type, size)
#define _Value(Name, type)

typedef struct __attribute__((__packed__))
{
    PFCStructs
} PFC_Data;


#include "xstruct_undef.h"

#define _PFCID_Start_Struct(Name, ID) \
    if(PFC_ID == ID) return offsetof(PFC_Data, Name);
#define _PFCID_Map(Name, type, rows, columns,  ID_count, ID) \
        if(PFC_ID >= ID && PFC_ID < (ID+ID_count))\
            return offsetof(PFC_Data, Name) + ( (sizeof(PFC_##Name) / ID_count) * (PFC_ID - ID) );

#define _PFCID_End_Struct(Name, ID)
#define _ValueArray(Name, type, size)
#define _Value(Name, type)

size_t PFC_OffsetOfID(uint8_t PFC_ID)
{
    PFCStructs

    return PFC_INVALID_ID;
}

void * PFC_PointerOfID(PFC_Data * data, uint8_t PFC_ID)
{
    uint8_t * result = (uint8_t*)data;
    size_t offset = PFC_OffsetOfID(PFC_ID);

    if (offset != PFC_INVALID_ID)
    {
        result += offset;
    }
    else
    {
        result = NULL;
    }

    return result;
}



#include "xstruct_undef.h"

#define _PFCID_Start_Struct(Name, ID) \
    if(PFC_ID == ID) return sizeof(PFC_##Name);
#define _PFCID_Map(Name, type, rows, columns,  ID_count, ID) \
        if(PFC_ID >= ID && PFC_ID < (ID+ID_count))\
            return (sizeof(PFC_##Name) / ID_count);

#define _PFCID_End_Struct(Name, ID)
#define _ValueArray(Name, type, size)
#define _Value(Name, type)

size_t PFC_SizeOfID(uint8_t PFC_ID)
{
    PFCStructs

    return PFC_INVALID_ID;
}

#include "xstruct_undef.h"

#define _PFCID_Start_Struct(Name, ID) \
       if(PFC_ID == ID) \
       { \
           PFC_##Name * data_struct = &data->Name; \
            printf("%s (%02x)\n", #Name, ID); \


#define _ValueArray(Name, type, size) \
        for(int i = 0; i < size; i++) \
            printf("   %s[%d](%p) = %s\n", #Name, i, &data_struct->Name[i], type ## _toString(&data_struct->Name[i]));

#define _Value(Name, type) \
        printf("    %s (%p) = %s\n", #Name, &data_struct->Name, type ## _toString(&data_struct->Name));

#define _PFCID_End_Struct(Name, ID) \
       } \

#define _PFCID_Map(Name, type, rows, columns,  ID_count, ID) \
        if(PFC_ID == ID)\
        { \
            int column = 0; \
            int row = 0; \
            printf("%s (%02x) (%p) = \n            ", #Name, ID, &data->Name); \
            for(column = 0; column < columns; column++) \
                    printf("N%02d     ", column + 1); \
            printf("\n"); \
            for(row = 0; row < rows; row++) \
            { \
                printf("    L%02d  ", row+1); \
                for(column = 0; column < columns; column++) \
                        printf("%6s  ", type ## _toString(&data->Name[column][row])); \
                printf("\n"); \
            } \
         } \



void PFC_PrintID(PFC_Data * data, uint8_t PFC_ID)
{
    PFCStructs
}


#endif /* PFCDATA */







