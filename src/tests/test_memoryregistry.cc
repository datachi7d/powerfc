/*
 * test_resource.cc
 *
 *  Created on: 25/11/2016
 *      Author: sean
 */

#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>

#include "pfc_memoryregistry.h"
#include "pfc_memorytypes.h"


class PFC_MemoryRegistry : public testing::Test
{
    void SetUp() {  }
    void TearDown() {  }
};


TEST_F(PFC_MemoryRegistry, test_Memory_NewFree)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_Memory_Free(memory);
}

TEST_F(PFC_MemoryRegistry, test_Memory_Invalid)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    EXPECT_TRUE(PFC_Memory_GetMemoryRegister(memory, PFC_ID_MIN) == NULL);
    EXPECT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, PFC_ID_MIN) == NULL);
    EXPECT_TRUE(PFC_Memory_GetMemoryRegisterSize(memory, PFC_ID_MIN) == 0);

    PFC_Memory_Free(memory);
}

TEST_F(PFC_MemoryRegistry, test_MemoryRegister_New)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, PFC_ID_MIN, 10, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_Memory_Free(memory);
}


TEST_F(PFC_MemoryRegistry, test_MemoryRegister_AddValue)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, PFC_ID_MIN, 10, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_MemoryValue * memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTE, "Test1");

    ASSERT_TRUE(memoryValue != NULL);

    ASSERT_EQ(PFC_SIZE_BYTE , PFC_MemoryRegister_GetSize(Register));

    memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test2");

    ASSERT_TRUE(memoryValue != NULL);

    ASSERT_EQ(PFC_SIZE_BYTE + PFC_SIZE_SHORT, PFC_MemoryRegister_GetSize(Register));

    PFC_Memory_Free(memory);
}

TEST_F(PFC_MemoryRegistry, test_MemoryRegister_AddValue_Invalid)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, PFC_ID_MIN, 2, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_MemoryValue * memoryValue = PFC_MemoryRegister_AddValue(NULL, PFC_MEMORYTYPE_SHORT, "Test");

    ASSERT_TRUE(memoryValue == NULL);

    memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test");

    ASSERT_TRUE(memoryValue != NULL);

    memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test");

    ASSERT_TRUE(memoryValue == NULL);


    PFC_Memory_Free(memory);
}


TEST_F(PFC_MemoryRegistry, test_Memory_GetMemoryRegister)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, PFC_ID_MIN, PFC_SIZE_SHORT * 2, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_MemoryValue * memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test1");

    ASSERT_TRUE(memoryValue != NULL);

    ASSERT_EQ(PFC_SIZE_SHORT, PFC_MemoryRegister_GetSize(Register));

    memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test2");

    ASSERT_TRUE(memoryValue != NULL);

    ASSERT_EQ(PFC_SIZE_SHORT*2, PFC_MemoryRegister_GetSize(Register));

    ASSERT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, PFC_ID_MIN) != NULL);
    ASSERT_EQ(PFC_SIZE_SHORT * 2, PFC_Memory_GetMemoryRegisterSize(memory, PFC_ID_MIN));


    PFC_Memory_Free(memory);
}

TEST_F(PFC_MemoryRegistry, test_Memory_NewMap)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    ASSERT_EQ(PFC_ERROR_NONE, PFC_Memory_NewMap(memory, PFC_ID_InjectorCorrection_0, PFC_ID_InjectorCorrection_3, PFC_MEMORYTYPE_BYTEDEGREE, 20, 20, "InjectorCorrection"));

    PFC_Memory_Free(memory);
}


TEST_F(PFC_MemoryRegistry, test_Memory_MemoryRegister_Advanced)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, PFC_ID_Advanced, 30, "Advanced");

    ASSERT_TRUE(Register != NULL);

    uint8_t * RegisterMemory = (uint8_t*)PFC_Memory_GetMemoryRegisterPointer(memory, PFC_ID_Advanced);

    ASSERT_TRUE(RegisterMemory != NULL);

    memset(RegisterMemory, 1, 30);

    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTRPM,          "RPM")              != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT,             "EngineLoad")       != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTVOLTAGE,      "Airflow1")         != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTVOLTAGE,      "Airflow2")         != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTMILLISECOND,  "Injector")         != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTFLOAT,        "FuelCorrection")   != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTEDEGREE,        "Ignition")         != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTEDEGREE,        "IgnitionDwell")    != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTBOOST,        "Boost")            != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTPERCENTAGE,   "BoostDuty")        != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTETEMPERATURE,   "WaterTemperature") != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTETEMPERATURE,   "AirTemperature")   != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTE,              "KnockLevel")       != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTEBATTERYVOLTAGE,"Battery")          != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTSPEED,        "Speed")            != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT,             "AirflowUsage")     != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTEVOLTAGE,       "O2Sensor1")        != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_BYTEVOLTAGE,       "O2Sensor2")        != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTVOLTAGE,      "Throttle")         != NULL);

    ASSERT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, PFC_ID_Advanced) != NULL);
    ASSERT_EQ(30, PFC_Memory_GetMemoryRegisterSize(memory, PFC_ID_Advanced));

    PFC_MemoryValue * value = PFC_MemoryRegister_GetFirstValue(Register);

    ASSERT_TRUE(value != NULL);

    for(int i = 0; i < 19; i ++)
    {
        int offset = PFC_MemoryRegister_GetOffsetOfValue(Register, value);
        char valueBuffer[256] = {0};

        PFC_Convert_PFCValueToString(PFC_MemoryValue_GetType(value), true, RegisterMemory + offset, valueBuffer, sizeof(valueBuffer));


        printf("\t%s = %s\n", PFC_MemoryValue_GetName(value), valueBuffer);

        if(i != 18)
        {
            ASSERT_EQ(PFC_ERROR_NONE, PFC_MemoryRegister_GetNextValue(Register, &value));
            ASSERT_TRUE(value != NULL);
        }
        else
        {
            ASSERT_EQ(PFC_ERROR_NOT_FOUND, PFC_MemoryRegister_GetNextValue(Register, &value));
            ASSERT_TRUE(value == NULL);
        }

    }




    PFC_Memory_Free(memory);
}
