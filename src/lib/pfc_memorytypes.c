#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pfc_types.h"
#include "pfc_memorytypes.h"


const char * pfc_memorytype_str [] = {
            "PFC_MEMORYTYPE_BYTE",
            "PFC_MEMORYTYPE_BYTERPM",
            "PFC_MEMORYTYPE_BYTETEMPERATURE",
            "PFC_MEMORYTYPE_BYTEDEGREE",
            "PFC_MEMORYTYPE_BYTEBATTERYVOLTAGE",
            "PFC_MEMORYTYPE_BYTEVOLTAGE",
            "PFC_MEMORYTYPE_BYTEBOOST",
            "PFC_MEMORYTYPE_BYTEFLOAT",
            "PFC_MEMORYTYPE_BYTEFLOAT17",
            "PFC_MEMORYTYPE_BYTEFLOAT26",
            "PFC_MEMORYTYPE_SHORT",
            "PFC_MEMORYTYPE_SHORTFLOAT",
            "PFC_MEMORYTYPE_SHORTFLOAT88",
            "PFC_MEMORYTYPE_SHORTSPEED",
            "PFC_MEMORYTYPE_SHORTRPM",
            "PFC_MEMORYTYPE_SHORTVOLTAGE",
            "PFC_MEMORYTYPE_SHORTBOOST",
            "PFC_MEMORYTYPE_SHORTMILLISECOND",
            "PFC_MEMORYTYPE_SHORTPERCENTAGE"
};

const char * PFC_MemoryType_ToString(pfc_memorytype memory_type)
{
    const char * result = NULL;

    if(memory_type < PFC_MEMORYTYPE_LAST)
        result = pfc_memorytype_str[memory_type];

    return result;
}

pfc_memorytype PFC_MemoryType_FromString(const char * name)
{
    pfc_memorytype result = 0;

    while (result < PFC_MEMORYTYPE_LAST)
    {
        if(strcmp(pfc_memorytype_str[result], name) == 0)
            break;

        result++;
    }

    return result;
}



/***********************************************************************************************************************************************
 * Conversion Functions
 */

static int Convert_Float(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    switch(conversion)
    {
        case PFC_CONVERSION_TOSTRING:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
            break;
        case PFC_CONVERSION_TOSTRING_WITHUNIT:
            result = snprintf((char *)output, outputLength, Format, *((float *)value), Units);
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

static int Convert_Int(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    switch(conversion)
    {
        case PFC_CONVERSION_TOSTRING:
            //result = snprintf((char *)output, outputLength, Format, *((int *)value));
            break;
        case PFC_CONVERSION_TOSTRING_WITHUNIT:
            result = snprintf((char *)output, outputLength, Format, *((int *)value), Units);
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


int Convert_Byte(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = *((uint8_t *)value);
        result = Convert_Int(conversion, &intValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteTemperature(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = (*((uint8_t *)value)) - 80;
        result = Convert_Int(conversion, &intValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteBatteryVoltage(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value)) * 0.1f;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteVoltage(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value)) * 0.02f;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteFloat(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (((*((uint8_t *)value))*0.3898f)+49.697f);
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteFloat17(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value))/64.0f ;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}
int Convert_ByteFloat26(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value))/128.0f;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}


int Convert_Short(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = *((uint16_t *)value);
        result = Convert_Int(conversion, &intValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortFloat(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (( ((float)(*((uint16_t*)value))) * 4.0f) / 1000.0f) ;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortFloat88(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (((float)(*((uint16_t*)value))) / 256.0f) ;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortPercentage(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = ( ((float)(*((uint16_t*)value))) / 200.0f) ;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortVoltage(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = ((float)(*((uint16_t*)value)))/10.0f;
        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortBoost(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format)
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
            Units += strlen(Units) + 1;
            Format += strlen(Format) + 1;
        }
        else
        {
            floatValue = ((float)(*((uint16_t*)value)&0x3ff))-760.0f;
        }

        result = Convert_Float(conversion, &floatValue, output, outputLength, Units, Format);
    }

    return result;
}

/***********************************************************************************************************************************************
 * API Functions
 */

typedef int (*PFC_ConversionFunction)(pcf_conversion conversion, const void * value, void * output, int outputLength, const char * Units, const char * Format);
typedef struct
{
    pfc_memorytype MemoryType;
    pfc_size Size;
    pfc_basictype BasicType;
    PFC_ConversionFunction ConversionFunction;
    const char * Units;
    const char * Format;
} pfc_memorytype_conversioninfo;

static const pfc_memorytype_conversioninfo conversionTable[] = {
        { .MemoryType = PFC_MEMORYTYPE_BYTE,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Byte,
          .Units = NULL,
          .Format = "%d",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTETEMPERATURE,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_ByteTemperature,
          .Units = "°C",
          .Format = "%d %s",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTERPM, //TODO
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Byte,
          .Units = NULL,
          .Format = "%d",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTEDEGREE,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Byte,
          .Units = "°",
          .Format = "%d %s",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTEBATTERYVOLTAGE,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ByteBatteryVoltage,
          .Units = "V",
          .Format = "%2.1f %s",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTEVOLTAGE,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ByteVoltage,
          .Units = "V",
          .Format = "%1.2f %s",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTEBOOST,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ByteFloat17,
          .Units = "kg/cm²",
          .Format = "%1.5f %s",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTEFLOAT,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ByteFloat,
          .Units = NULL,
          .Format = "%3.1f",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTEFLOAT17,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ByteFloat17,
          .Units = NULL,
          .Format = "%1.5f",
        },
        { .MemoryType = PFC_MEMORYTYPE_BYTEFLOAT26,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ByteFloat26,
          .Units = NULL,
          .Format = "%1.5f",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORT,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Short,
          .Units = NULL,
          .Format = "%d",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTFLOAT,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortFloat,
          .Units = NULL,
          .Format = "%3.3f",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTFLOAT88,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortFloat88,
          .Units = NULL,
          .Format = "%3.3f",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTMILLISECOND,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortFloat,
          .Units = "ms",
          .Format = "%3.3f %s",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTPERCENTAGE,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortPercentage,
          .Units = "%",
          .Format = "%3.3f %s",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTRPM,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Short,
          .Units = " RPM",
          .Format = "%d %s",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTSPEED,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Short,
          .Units = " km/hr",
          .Format = "%d %s",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTVOLTAGE,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortVoltage,
          .Units = "V",
          .Format = "%2.1f %s",
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTBOOST,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortBoost,
          .Units = "mmHg\0kg/cm²",
          .Format = "%3.0f %s\0%1.2f %s",
        }
};

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

pfc_conversion_error PFC_Convert_PFCValueTo(pcf_conversion conversionType,pfc_basictype BasicType, pfc_memorytype MemoryType, const void * Value, void * ConvertedValue, int ConvertedValueSize)
{
    const pfc_memorytype_conversioninfo * memoryTypeConversion = getConverstionInfo(MemoryType);
    pfc_conversion_error result = PFC_CONVERSION_ERROR_NONSET;

    if(memoryTypeConversion != NULL)
    {
        if(memoryTypeConversion->BasicType == BasicType || conversionType == PFC_CONVERSION_TOSTRING_WITHUNIT || conversionType == PFC_CONVERSION_TOSTRING_WITHUNIT)
        {
            int len = memoryTypeConversion->ConversionFunction(conversionType, Value, ConvertedValue, ConvertedValueSize, memoryTypeConversion->Units, memoryTypeConversion->Format);

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
    return PFC_Convert_PFCValueTo(PFC_CONVERSION_TOBASIC, PFC_BASICTYPE_FLOAT, MemoryType, Value, ConvertedValue, sizeof(*ConvertedValue));
}

pfc_conversion_error PFC_Convert_PFCValueToInt(pfc_memorytype MemoryType, const void * Value, int * ConvertedValue)
{
    return PFC_Convert_PFCValueTo(PFC_CONVERSION_TOBASIC, PFC_BASICTYPE_INT, MemoryType, Value, ConvertedValue, sizeof(*ConvertedValue));
}

pfc_conversion_error PFC_Convert_PFCValueToString(pfc_memorytype MemoryType, bool Unit, const void * Value, char * ConvertedValue, int ConvertedValueLength)
{
    return PFC_Convert_PFCValueTo(Unit ? PFC_CONVERSION_TOSTRING_WITHUNIT : PFC_CONVERSION_TOSTRING_WITHUNIT, PFC_BASICTYPE_NONE, MemoryType, Value, ConvertedValue, ConvertedValueLength);
}


