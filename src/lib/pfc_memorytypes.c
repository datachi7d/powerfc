/*
 * pfc_memorytypes.c
 *
 *  Created on: 26/11/2016
 *      Author: sean
 */

#include <stdlib.h>
#include <string.h>

#include "pfc_types.h"
#include "pfc_memorytypes.h"

int Convert_Byte(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units);
int Convert_Short(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units);
int Convert_ShortVoltage(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units);
int Convert_ShortBoost(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units);

typedef int (*PFC_ConversionFunction)(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units);

typedef struct
{
    pfc_memorytype MemoryType;
    pfc_size Size;
    pfc_basictype BasicType;
    PFC_ConversionFunction ConversionFunction;
    const char * Units;
} pfc_memorytype_conversioninfo;

static const pfc_memorytype_conversioninfo conversionTable[] = {
        { .MemoryType = PFC_MEMORYTYPE_BYTE,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Byte,
          .Units = NULL,
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORT,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Short,
          .Units = NULL,
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTRPM,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Short,
          .Units = " RPM",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTVOLTAGE,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortVoltage,
          .Units = " V",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTBOOST,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortBoost,
          .Units = " mmHg, Kgcm2",
        }
};



static int Convert_Float(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units)
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
            if(outputLength == sizeof(float))
            {
                memcpy(output, value, sizeof(float));
                result = sizeof(float);
            }
            else
            {
                result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_TYPELENGTH);
            }
            break;
        case PFC_CONVERSION_TOPFC_FROMBASIC:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
            break;
        case PFC_CONVERSION_TOPFC_FROMSTRING:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
            break;

        default:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
    }

    return result;
}

static int Convert_Int(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units)
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
            if(outputLength == sizeof(int))
            {
                memcpy(output, value, sizeof(int));
                result = sizeof(int);
            }
            else
            {
                result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_TYPELENGTH);
            }
            break;

        case PFC_CONVERSION_TOPFC_FROMBASIC:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
            break;
        case PFC_CONVERSION_TOPFC_FROMSTRING:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
            break;

        default:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
    }

    return result;
}


int Convert_Byte(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = *((uint8_t *)value);
        result = Convert_Int(conversion, &intValue, output, outputLength, Units);
    }

    return result;
}

int Convert_Short(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = *((uint16_t *)value);
        result = Convert_Int(conversion, &intValue, output, outputLength, Units);
    }

    return result;
}

int Convert_ShortVoltage(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = ((float)(*((uint16_t*)value)))/10.0f;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units);
    }

    return result;
}

int Convert_ShortBoost(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = 0.0f;

        if( *((uint16_t*)value) & 0x8000 )
        {
            floatValue = (((float)(*((uint16_t*)value)&0xff))/100.0f);
        }
        else
        {
            floatValue = ((float)(*((uint16_t*)value)&0x3ff))-760.0f;
        }

        result = Convert_Float(conversion, &floatValue, output, outputLength, Units);
    }

    return result;
}

const pfc_memorytype_conversioninfo * getConverstionInfo(pfc_memorytype MemoryType)
{
    const pfc_memorytype_conversioninfo * memoryTypeConversion = NULL;
    int i = 0;

    for(i = 0; i < (sizeof(conversionTable) / sizeof(pfc_memorytype_conversioninfo)); i++)
    {
        if(conversionTable[i].MemoryType == MemoryType)
        {
            memoryTypeConversion = &conversionTable[i];
            break;
        }
    }

    return memoryTypeConversion;
}

pfc_size PFC_Convert_PFCValueSize(pfc_memorytype MemoryType)
{
    const pfc_memorytype_conversioninfo * memoryTypeConversion = getConverstionInfo(MemoryType);
    pfc_size Result = 0;

    if(memoryTypeConversion != NULL)
    {
        Result = memoryTypeConversion->Size;
    }

    return Result;
}

pfc_conversion_error PFC_Convert_PFCValueToBasic(pfc_basictype BasicType, pfc_memorytype MemoryType, const void * Value, void * ConvertedValue, int ConvertedValueSize)
{
    const pfc_memorytype_conversioninfo * memoryTypeConversion = getConverstionInfo(MemoryType);
    pfc_conversion_error result = PFC_CONVERSION_ERROR_NONSET;

    if(memoryTypeConversion != NULL)
    {
        if(memoryTypeConversion->BasicType == BasicType)
        {
            int len = memoryTypeConversion->ConversionFunction(PFC_CONVERSION_TOBASIC, Value, ConvertedValue, ConvertedValueSize, NULL);

            if(len > 0)
            {
                result = PFC_CONVERSION_ERROR_NOERROR;
            }
            else
            {
                result = PFC_INT_TO_CERROR(len);
            }
        }
        else
        {
            result = PFC_CONVERSION_ERROR_UNSUPPORTED;
        }
    }
    else
    {
        result = PFC_CONVERSION_ERROR_UNSUPPORTED;
    }

    return result;
}


pfc_conversion_error PFC_Convert_PFCValueToFloat(pfc_memorytype MemoryType, const void * Value, float * ConvertedValue)
{
    return PFC_Convert_PFCValueToBasic(PFC_BASICTYPE_FLOAT, MemoryType, Value, ConvertedValue, sizeof(*ConvertedValue));
}

pfc_conversion_error PFC_Convert_PFCValueToInt(pfc_memorytype MemoryType, const void * Value, int * ConvertedValue)
{
    return PFC_Convert_PFCValueToBasic(PFC_BASICTYPE_INT, MemoryType, Value, ConvertedValue, sizeof(*ConvertedValue));
}


