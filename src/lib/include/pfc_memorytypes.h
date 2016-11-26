/*
 * pfc_memorytypes.h
 *
 *  Created on: 26/11/2016
 *      Author: sean
 */

#ifndef SRC_LIB_INCLUDE_PFC_MEMORYTYPES_H_
#define SRC_LIB_INCLUDE_PFC_MEMORYTYPES_H_


#ifdef __cplusplus
extern "C" {
#endif

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

typedef int (*PFC_ConversionFunction)(pcf_conversion conversion, const void * value, void * output, int outputLength);

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
    PFC_MEMORYTYPE_SHORTBOOST,
    PFC_MEMORYTYPE_LAST,
} pfc_memorytype;


typedef enum
{
    PFC_BASICTYPE_INT,
    PFC_BASICTYPE_FLOAT,
    PFC_BASICTYPE_STRING
} pfc_basictype;

typedef struct
{
    pfc_memorytype MemoryType;
    pfc_basictype BasicType;
    PFC_ConversionFunction ConversionFunction;
} pfc_memorytype_conversioninfo;


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


pfc_conversion_error PFC_Convert_PFCValueToFloat(pfc_memorytype MemoryType, const void * Value, float * ConvertedValue);


#ifdef __cplusplus
}
#endif

#endif /* SRC_LIB_INCLUDE_PFC_MEMORYTYPES_H_ */
