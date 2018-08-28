#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "pfc_types.h"
#include "pfc_memorytypes.h"


typedef int (*PFC_ConversionFunction)(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format);
typedef struct
{
    pfc_memorytype MemoryType;
    pfc_size Size;
    pfc_basictype BasicType;
    PFC_ConversionFunction ConversionFunction;
    const char * Units;
    const char * Format;
} pfc_memorytype_conversioninfo;

const pfc_memorytype_conversioninfo * getConverstionInfo(pfc_memorytype MemoryType);

const char * pfc_memorytype_str [] = {
            "PFC_MEMORYTYPE_BYTE",
            "PFC_MEMORYTYPE_BYTERPM",
			"PFC_MEMORYTYPE_BYTEMILLISECOND",
			"PFC_MEMORYTYPE_BYTERPMCRANK",
			"PFC_MEMORYTYPE_BYTELOAD",
            "PFC_MEMORYTYPE_BYTETEMPERATURE",
            "PFC_MEMORYTYPE_BYTEDEGREE",
            "PFC_MEMORYTYPE_BYTEBATTERYVOLTAGE",
            "PFC_MEMORYTYPE_BYTEVOLTAGE",
            "PFC_MEMORYTYPE_BYTEBOOST",
			"PFC_MEMORYTYPE_BYTEBOOSTSOLENOID",
            "PFC_MEMORYTYPE_BYTEFLOAT",
            "PFC_MEMORYTYPE_BYTEFLOAT17",
            "PFC_MEMORYTYPE_BYTEFLOAT26",
			"PFC_MEMORYTYPE_CONTROLFLAG",
            "PFC_MEMORYTYPE_SHORT",
            "PFC_MEMORYTYPE_SHORTFLOAT",
            "PFC_MEMORYTYPE_SHORTFLOAT88",
            "PFC_MEMORYTYPE_SHORTSPEED",
            "PFC_MEMORYTYPE_SHORTRPM",
            "PFC_MEMORYTYPE_SHORTVOLTAGE",
            "PFC_MEMORYTYPE_SHORTBOOST",
            "PFC_MEMORYTYPE_SHORTMILLISECOND",
			"PFC_MEMORYTYPE_SHORTMILLIVOLTAGE",
            "PFC_MEMORYTYPE_SHORTPERCENTAGE",
			"PFC_MEMORYTYPE_SHORTLAG",
			"PFC_MEMORYTYPE_SHORTCORRECTION",
			"PFC_MEMORYTYPE_STRINGSENSOR",
			"PFC_MEMORYTYPE_STRINGCONTROL",
			"PFC_MEMORYTYPE_STRINGTYPE",
			"PFC_MEMORYTYPE_STRINGVERSION",
			"PFC_MEMORYTYPE_TPS_SETTING",
			"PFC_MEMORYTYPE_IGNITIONTEMPERATURE_SETTING",
			"PFC_MEMORYTYPE_IGNITIONVOLTAGE_SETTING",
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

static int Convert_Float(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
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

static int Convert_Int(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
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

static int Convert_String(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    switch(conversion)
    {
        case PFC_CONVERSION_TOSTRING:
            result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_UNSUPPORTED);
            break;
        case PFC_CONVERSION_TOSTRING_WITHUNIT:
        case PFC_CONVERSION_TOBASIC:
            if(outputLength > valueSize)
            {
            	memset(output, 0, outputLength);
                memcpy(output, value, valueSize);
                result = valueSize;
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

int Convert_ControlFlag(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        const char * controlValue = (*((const uint8_t *)value)) == 0xFF ? &Units[0] : &Units[3];
        result = Convert_String(conversion, controlValue, strlen(controlValue), output, outputLength, Units, Format);
    }

    return result;
}

int Convert_Byte(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = *((uint8_t *)value);
        result = Convert_Int(conversion, &intValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}


int Convert_ByteTemperature(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = (*((uint8_t *)value)) - 80;
        result = Convert_Int(conversion, &intValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteRPM(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = ((int)(*((uint8_t *)value)))*40;
        result = Convert_Int(conversion, &intValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}


int Convert_ByteMillisecond(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = ((int)(*((uint8_t *)value)))*20;
        result = Convert_Int(conversion, &intValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteRPMCrank(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = ((int)(*((uint8_t *)value)))*10;
        result = Convert_Int(conversion, &intValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteLoad(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = ((uint32_t)(*((uint8_t *)value))) << 8;
        result = Convert_Int(conversion, &intValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteBatteryVoltage(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value)) * 0.1f;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteVoltage(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value)) * 0.02f;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteBoostSolenoid(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value)) * 0.05f;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteFloat(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        //float floatValue = (((*((uint8_t *)value))*0.3898f)+49.697f);
    	float floatValue = floorf((((float)((*((uint8_t *)value))))/0.256f) + 500.0f) / 1000.0f;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteFloat17(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value))/64.0f ;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ByteFloat26(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (*((uint8_t *)value))/128.0f;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}


int Convert_Short(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = *((uint16_t *)value);
        result = Convert_Int(conversion, &intValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortFloat(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = (( ((float)(*((uint16_t*)value))) * 4.0f) / 1000.0f) ;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortFloat88(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        //float floatValue = (((float)(*((uint16_t*)value))) / 256.0f) ;
        float floatValue = (((float)(*((uint16_t*)value))) / 32768.0f) ;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortPercentage(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = ( ((float)(*((uint16_t*)value))) / 200.0f) ;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortLag(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        uint16_t pvalue = (*((uint16_t*)value)) & 0x7FFF;
        float floatValue = 0.0f;

        if(pvalue == (*((uint16_t*)value)))
        {
        	floatValue = (0x8000 - pvalue)/(-250.0f);
        }
        else
        {
        	floatValue = (pvalue)/(250.0f);
        }
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortCorrection(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = floorf( (((float)(*((uint16_t*)value))) / (3.27680f)) / 10.0f ) / 1000.0f;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortVoltage(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        float floatValue = ((float)(*((uint16_t*)value)))/10.0f;
        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortMilliVoltage(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT ||
         conversion == PFC_CONVERSION_TOBASIC)
    {
        int intValue = ((int)*((uint16_t *)value));
        result = Convert_Int(conversion, &intValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}

int Convert_ShortBoost(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
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

        result = Convert_Float(conversion, &floatValue, valueSize, output, outputLength, Units, Format);
    }

    return result;
}


int Convert_TPS(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);
    const pfc_memorytype_conversioninfo * byteTemperatureConversion = getConverstionInfo(PFC_MEMORYTYPE_BYTETEMPERATURE);
    const pfc_memorytype_conversioninfo * float88Conversion = getConverstionInfo(PFC_MEMORYTYPE_SHORTFLOAT88);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT)
    {

    	if(float88Conversion != NULL && byteTemperatureConversion != NULL)
    	{
    		const uint8_t * byteValues = (const uint8_t *)value;
    		char byteTemperatureString[256] = {0};

			result = Convert_ByteTemperature(conversion, &byteValues[0], PFC_SIZE_BYTE, byteTemperatureString, sizeof(byteTemperatureString), byteTemperatureConversion->Units, byteTemperatureConversion->Format);

			if(result > 0)
			{
				char float88String[256] = {0};

				result = Convert_ShortFloat(conversion, &byteValues[1], PFC_SIZE_SHORT, float88String, sizeof(float88String), float88Conversion->Units, float88Conversion->Format);

				if(result > 0)
				{
					 result = snprintf((char *)output, outputLength, Format, byteTemperatureString, float88String);
				}
			}
    	}
    }

    return result;
}

int Convert_IgnitionTemperature(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);
    const pfc_memorytype_conversioninfo * byteTemperatureConversion = getConverstionInfo(PFC_MEMORYTYPE_BYTETEMPERATURE);
    const pfc_memorytype_conversioninfo * byteDegreeConversion = getConverstionInfo(PFC_MEMORYTYPE_BYTEDEGREE);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT)
    {

    	if(byteDegreeConversion != NULL && byteTemperatureConversion != NULL)
    	{
    		const uint8_t * byteValues = (const uint8_t *)value;
    		char byteTemperatureString[256] = {0};

			result = Convert_ByteTemperature(conversion, &byteValues[0], PFC_SIZE_BYTE, byteTemperatureString, sizeof(byteTemperatureString), byteTemperatureConversion->Units, byteTemperatureConversion->Format);

			if(result > 0)
			{
				char byteDegreeString[256] = {0};

				result = Convert_Byte(conversion, &byteValues[1], PFC_SIZE_BYTE, byteDegreeString, sizeof(byteDegreeString), byteDegreeConversion->Units, byteDegreeConversion->Format);

				if(result > 0)
				{
					 result = snprintf((char *)output, outputLength, Format, byteTemperatureString, byteDegreeString);
				}
			}
    	}
    }

    return result;
}


int Convert_IgnitionVoltage(pcf_conversion conversion, const void * value, int valueSize, void * output, int outputLength, const char * Units, const char * Format)
{
    int result = PFC_CERROR_TO_INT(PFC_CONVERSION_ERROR_NONSET);
    const pfc_memorytype_conversioninfo * byteVoltageConversion = getConverstionInfo(PFC_MEMORYTYPE_BYTEVOLTAGE);
    const pfc_memorytype_conversioninfo * byteDegreeConversion = getConverstionInfo(PFC_MEMORYTYPE_BYTEDEGREE);

    if  (conversion == PFC_CONVERSION_TOSTRING ||
         conversion == PFC_CONVERSION_TOSTRING_WITHUNIT)
    {

    	if(byteDegreeConversion != NULL && byteVoltageConversion != NULL)
    	{
    		const uint8_t * byteValues = (const uint8_t *)value;
    		char byteVoltageString[256] = {0};

			result = Convert_ByteVoltage(conversion, &byteValues[0], PFC_SIZE_BYTE, byteVoltageString, sizeof(byteVoltageString), byteVoltageConversion->Units, byteVoltageConversion->Format);

			if(result > 0)
			{
				char byteDegreeString[256] = {0};

				result = Convert_Byte(conversion, &byteValues[1], PFC_SIZE_BYTE, byteDegreeString, sizeof(byteDegreeString), byteDegreeConversion->Units, byteDegreeConversion->Format);

				if(result > 0)
				{
					 result = snprintf((char *)output, outputLength, Format, byteVoltageString, byteDegreeString);
				}
			}
    	}
    }

    return result;
}

/***********************************************************************************************************************************************
 * API Functions
 */

static const pfc_memorytype_conversioninfo conversionTable[] = {
		{
				.MemoryType = PFC_MEMORYTYPE_BYTE,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_INT,
				.ConversionFunction = Convert_Byte,
				.Units = NULL,
				.Format = "%d",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTERPM,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_INT,
				.ConversionFunction = Convert_ByteRPM,
				.Units = "rpm",
				.Format = "%d %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEMILLISECOND,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_INT,
				.ConversionFunction = Convert_ByteMillisecond,
				.Units = "ms",
				.Format = "%d %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTETEMPERATURE,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_INT,
				.ConversionFunction = Convert_ByteTemperature,
				.Units = "°C",
				.Format = "%d %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTERPMCRANK,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_INT,
				.ConversionFunction = Convert_ByteRPMCrank,
				.Units = "rpm",
				.Format = "%d %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTELOAD,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_INT,
				.ConversionFunction = Convert_ByteLoad,
				.Units = NULL,
				.Format = "%d",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEDEGREE,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_INT,
				.ConversionFunction = Convert_Byte,
				.Units = "°",
				.Format = "%d %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEBATTERYVOLTAGE,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ByteBatteryVoltage,
				.Units = "V",
				.Format = "%2.1f %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEVOLTAGE,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ByteVoltage,
				.Units = "V",
				.Format = "%1.2f %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEBOOST,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ByteFloat17,
				.Units = "kg/cm²",
				.Format = "%1.5f %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEBOOSTSOLENOID,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ByteBoostSolenoid,
				.Units = "kg/cm²",
				.Format = "%1.2f %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEFLOAT,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ByteFloat,
				.Units = NULL,
				.Format = "%1.3f",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEFLOAT17,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ByteFloat17,
				.Units = NULL,
				.Format = "%1.5f",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_BYTEFLOAT26,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ByteFloat26,
				.Units = NULL,
				.Format = "%1.5f",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_CONTROLFLAG,
				.Size = PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_STRING,
				.ConversionFunction = Convert_ControlFlag,
				.Units = "on\0off",
				.Format = "%s",
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
				.MemoryType = PFC_MEMORYTYPE_SHORTMILLIVOLTAGE,
				.Size = PFC_SIZE_SHORT,
				.BasicType = PFC_BASICTYPE_INT,
				.ConversionFunction = Convert_ShortMilliVoltage,
				.Units = "mV",
				.Format = "%d %s",
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
				.MemoryType = PFC_MEMORYTYPE_SHORTLAG,
				.Size = PFC_SIZE_SHORT,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ShortLag,
				.Units = "ms",
				.Format = "%1.3f %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_SHORTCORRECTION,
				.Size = PFC_SIZE_SHORT,
				.BasicType = PFC_BASICTYPE_FLOAT,
				.ConversionFunction = Convert_ShortCorrection,
				.Units = "",
				.Format = "%1.3f",
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
		},
		{
				.MemoryType = PFC_MEMORYTYPE_STRINGSENSOR,
				.Size = 4,
				.BasicType = PFC_BASICTYPE_STRING,
				.ConversionFunction = Convert_String,
				.Units = "",
				.Format = "%s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_STRINGCONTROL,
				.Size = 16,
				.BasicType = PFC_BASICTYPE_STRING,
				.ConversionFunction = Convert_String,
				.Units = "",
				.Format = "%s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_STRINGTYPE,
				.Size = 8,
				.BasicType = PFC_BASICTYPE_STRING,
				.ConversionFunction = Convert_String,
				.Units = "",
				.Format = "%s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_STRINGVERSION,
				.Size = 5,
				.BasicType = PFC_BASICTYPE_STRING,
				.ConversionFunction = Convert_String,
				.Units = "",
				.Format = "%s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_TPS_SETTING,
				.Size = PFC_SIZE_BYTE + PFC_SIZE_SHORT,
				.BasicType = PFC_BASICTYPE_STRING,
				.ConversionFunction = Convert_TPS,
				.Units = "",
				.Format = "%s,%s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_IGNITIONTEMPERATURE_SETTING,
				.Size = PFC_SIZE_BYTE + PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_STRING,
				.ConversionFunction = Convert_IgnitionTemperature,
				.Units = "",
				.Format = "%s, %s",
		},
		{
				.MemoryType = PFC_MEMORYTYPE_IGNITIONVOLTAGE_SETTING,
				.Size = PFC_SIZE_BYTE + PFC_SIZE_BYTE,
				.BasicType = PFC_BASICTYPE_STRING,
				.ConversionFunction = Convert_IgnitionVoltage,
				.Units = "",
				.Format = "%s, %s",
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
            int len = memoryTypeConversion->ConversionFunction(conversionType, Value, memoryTypeConversion->Size, ConvertedValue, ConvertedValueSize, memoryTypeConversion->Units, memoryTypeConversion->Format);

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


