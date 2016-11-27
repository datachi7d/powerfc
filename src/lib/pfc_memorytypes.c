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

int Convert_Byte(pcf_conversion conversion, const void * value, void * output, int outputLength);
int Convert_Short(pcf_conversion conversion, const void * value, void * output, int outputLength);
int Convert_ShortBoost(pcf_conversion conversion, const void * value, void * output, int outputLength);

static const pfc_memorytype_conversioninfo conversionTable[] = {
        { .MemoryType = PFC_MEMORYTYPE_BYTE,
          .Size = PFC_SIZE_BYTE,
          .BasicType = PFC_BASICTYPE_INT,
          .ConversionFunction = Convert_Byte,
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORT,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_Short,
        },
        {
          .MemoryType = PFC_MEMORYTYPE_SHORTBOOST,
          .Size = PFC_SIZE_SHORT,
          .BasicType = PFC_BASICTYPE_FLOAT,
          .ConversionFunction = Convert_ShortBoost,
        }
};

int Convert_Byte(pcf_conversion conversion, const void * value, void * output, int outputLength)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    return result;
}

int Convert_Short(pcf_conversion conversion, const void * value, void * output, int outputLength)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    return result;
}

int Convert_ShortBoost(pcf_conversion conversion, const void * value, void * output, int outputLength)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);
    float boost = 0.0f;

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        if( *((uint16_t*)value) & 0x8000 )
        {
            boost = (((float)(*((uint16_t*)value)&0xff))/100.0f);
        }
        else
        {
            boost = ((float)(*((uint16_t*)value)&0x3ff))-760.0f;
        }

    }

    switch(conversion)
    {
        case PFC_CONVERSION_TOSTRING:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
            break;
        case PFC_CONVERSION_TOSTRING_WITHUNIT:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
            break;
        case PFC_CONVERSION_TOBASIC:
            if(outputLength == sizeof(boost))
            {
                memcpy(output, &boost, sizeof(boost));
                result = sizeof(boost);
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

        default:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);

    }

    return result;
}


const pfc_memorytype_conversioninfo * getConverstionInfo(pfc_memorytype MemoryType)
{
    const pfc_memorytype_conversioninfo * memoryTypeConversion = NULL;

    for(int i = 0; i < (sizeof(conversionTable) / sizeof(pfc_memorytype_conversioninfo)); i++)
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

pfc_conversion_error PFC_Convert_PFCValueToFloat(pfc_memorytype MemoryType, const void * Value, float * ConvertedValue)
{
    const pfc_memorytype_conversioninfo * memoryTypeConversion = getConverstionInfo(MemoryType);
    pfc_conversion_error result = PFC_CONVERSION_ERROR_NONSET;

    if(memoryTypeConversion != NULL)
    {
        if(memoryTypeConversion->BasicType == PFC_BASICTYPE_FLOAT)
        {
            int len = memoryTypeConversion->ConversionFunction(PFC_CONVERSION_TOBASIC, Value, ConvertedValue, sizeof(*ConvertedValue));

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


