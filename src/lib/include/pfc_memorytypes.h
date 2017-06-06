#ifndef SRC_LIB_INCLUDE_PFC_MEMORYTYPES_H_
#define SRC_LIB_INCLUDE_PFC_MEMORYTYPES_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "pfc_types.h"

#define PFC_SIZE_SHORT 2
#define PFC_SIZE_BYTE  1

typedef enum
{
    PFC_CONVERSION_ERROR_NONSET = 0,
    PFC_CONVERSION_ERROR_UNSUPPORTED,
    PFC_CONVERSION_ERROR_TYPELENGTH,
    PFC_CONVERSION_ERROR_NOERROR
} pfc_conversion_error;

#define PFC_CERROR_TO_INT(err) ((int)(-err));
#define PFC_INT_TO_CERROR(i) ((pfc_conversion_error)(-i))

typedef enum
{
    PFC_CONVERSION_TOSTRING,
    PFC_CONVERSION_TOSTRING_WITHUNIT,
    PFC_CONVERSION_TOBASIC,
    PFC_CONVERSION_TOPFC_FROMBASIC,
    PFC_CONVERSION_TOPFC_FROMSTRING,
} pcf_conversion;



/*
int PFC_Convert_X(pcf_conversion conversion, const void * value, void * output, int outputLength)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    switch(conversion)
    {
    case PFC_CONVERSION_TOSTRING:
        result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
        break;
    case PFC_CONVERSION_TOSTRING_WITHUNIT:
        result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
        break;
    case PFC_CONVERSION_TOBASIC:
        result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
        break;
    case PFC_CONVERSION_TOPFC_FROMBASIC:
        result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
        break;
    case PFC_CONVERSION_TOPFC_FROMSTRING:

    default:
        result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);

    }

    return result;
}
 */


typedef enum
{
    PFC_MEMORYTYPE_BYTE = 0,
    PFC_MEMORYTYPE_BYTERPM,
	PFC_MEMORYTYPE_BYTEMILLISECOND,
	PFC_MEMORYTYPE_BYTERPMCRANK,
	PFC_MEMORYTYPE_BYTELOAD,
    PFC_MEMORYTYPE_BYTETEMPERATURE,
    PFC_MEMORYTYPE_BYTEDEGREE,
    PFC_MEMORYTYPE_BYTEBATTERYVOLTAGE,
    PFC_MEMORYTYPE_BYTEVOLTAGE,
    PFC_MEMORYTYPE_BYTEBOOST,
    PFC_MEMORYTYPE_BYTEFLOAT,
    PFC_MEMORYTYPE_BYTEFLOAT17,
    PFC_MEMORYTYPE_BYTEFLOAT26,
	PFC_MEMORYTYPE_CONTROLFLAG,
    PFC_MEMORYTYPE_SHORT,
    PFC_MEMORYTYPE_SHORTFLOAT,
    PFC_MEMORYTYPE_SHORTFLOAT88,
    PFC_MEMORYTYPE_SHORTSPEED,
    PFC_MEMORYTYPE_SHORTRPM,
    PFC_MEMORYTYPE_SHORTVOLTAGE,
    PFC_MEMORYTYPE_SHORTBOOST,
    PFC_MEMORYTYPE_SHORTMILLISECOND,
	PFC_MEMORYTYPE_SHORTMILLIVOLTAGE,
    PFC_MEMORYTYPE_SHORTPERCENTAGE,
	PFC_MEMORYTYPE_SHORTLAG,
	PFC_MEMORYTYPE_SHORTCORRECTION,
	PFC_MEMORYTYPE_STRINGSENSOR,
	PFC_MEMORYTYPE_STRINGCONTROL,
	PFC_MEMORYTYPE_STRINGTYPE,
	PFC_MEMORYTYPE_STRINGVERSION,
	PFC_MEMORYTYPE_TPS_SETTING,
	PFC_MEMORYTYPE_IGNITIONTEMPERATURE_SETTING,
    PFC_MEMORYTYPE_LAST,
} pfc_memorytype;


typedef enum
{
    PFC_BASICTYPE_NONE,
    PFC_BASICTYPE_INT,
    PFC_BASICTYPE_FLOAT,
    PFC_BASICTYPE_STRING
} pfc_basictype;

//typedef uint8_t byte;
//typedef uint8_t byte_temperature;
//typedef uint8_t byte_voltage;
//typedef uint8_t byte_float;
//typedef uint8_t byte_percentage;
//typedef uint8_t byte_degree;
//typedef uint8_t byte_temperature;
//typedef uint8_t byte_RPM;
//typedef uint8_t byte_boost;
//
//typedef uint16_t short_float;
//typedef uint16_t short_speed;
//typedef uint16_t short_percentage;
//typedef uint16_t short_millisecond;
//typedef uint16_t short_word;
//typedef uint16_t short_boost;
//typedef uint16_t short_voltage;
//typedef uint16_t short_RPM;

pfc_size PFC_Convert_PFCValueSize(pfc_memorytype MemoryType);
pfc_conversion_error PFC_Convert_PFCValueToFloat(pfc_memorytype MemoryType, const void * Value, float * ConvertedValue);
pfc_conversion_error PFC_Convert_PFCValueToInt(pfc_memorytype MemoryType, const void * Value, int * ConvertedValue);
pfc_conversion_error PFC_Convert_PFCValueToString(pfc_memorytype MemoryType, bool Unit, const void * Value, char * ConvertedValue, int ConvertedValueLength);


const char * PFC_MemoryType_ToString(pfc_memorytype memory_type);
pfc_memorytype PFC_MemoryType_FromString(const char * name);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LIB_INCLUDE_PFC_MEMORYTYPES_H_ */
