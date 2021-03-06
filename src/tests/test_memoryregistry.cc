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

    EXPECT_TRUE(PFC_Memory_GetMemoryRegister(memory, 0) == NULL);
    EXPECT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, 0) == NULL);
    EXPECT_TRUE(PFC_Memory_GetMemoryRegisterSize(memory, 0) == 0);

    PFC_Memory_Free(memory);
}

TEST_F(PFC_MemoryRegistry, test_MemoryRegister_New)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, 0, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_Memory_Free(memory);
}


TEST_F(PFC_MemoryRegistry, test_MemoryRegister_AddValue)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, 0, "Test");

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

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, 0, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_MemoryValue * memoryValue = PFC_MemoryRegister_AddValue(NULL, PFC_MEMORYTYPE_SHORT, "Test");

    ASSERT_TRUE(memoryValue == NULL);

    memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test");

    ASSERT_TRUE(memoryValue != NULL);

    PFC_Memory_Free(memory);
}


TEST_F(PFC_MemoryRegistry, test_Memory_GetMemoryRegister)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, 0, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_MemoryValue * memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test1");

    ASSERT_TRUE(memoryValue != NULL);

    ASSERT_EQ(PFC_SIZE_SHORT, PFC_MemoryRegister_GetSize(Register));

    memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test2");

    ASSERT_TRUE(memoryValue != NULL);

    ASSERT_EQ(PFC_SIZE_SHORT*2, PFC_MemoryRegister_GetSize(Register));

    ASSERT_EQ(PFC_SIZE_SHORT*2, PFC_MemoryRegister_Malloc(Register));

    ASSERT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, 0) != NULL);
    ASSERT_EQ(PFC_SIZE_SHORT * 2, PFC_Memory_GetMemoryRegisterSize(memory, 0));


    PFC_Memory_Free(memory);
}

TEST_F(PFC_MemoryRegistry, test_Memory_NewMap)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryMap * memoryMap = PFC_Memory_NewMap(memory, 0, 3, PFC_MEMORYTYPE_BYTEDEGREE, 20, 20, "InjectorCorrection");

    ASSERT_TRUE(memoryMap != NULL);

    PFC_Memory_Free(memory);
}

TEST_F(PFC_MemoryRegistry, test_MemoryMap_GetXY)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryMap * memoryMap = PFC_Memory_NewMap(memory, 0, 3, PFC_MEMORYTYPE_BYTEDEGREE, 20, 20, "InjectorCorrection");

    ASSERT_TRUE(memoryMap != NULL);

    int row = 0, column = 0;

    for(row = 0; row < 20; row++)
    {
        for(column = 0; column < 20; column++)
        {
            PFC_MemoryValue * value = PFC_MemoryMap_GetMemoryValue(memoryMap, row, column);

            ASSERT_TRUE(value != NULL);

            //printf("%s = [%d][%d]\n", PFC_MemoryValue_GetName(value), row, column);
        }
    }



    PFC_Memory_Free(memory);
}



TEST_F(PFC_MemoryRegistry, test_Memory_MemoryRegister_Advanced)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, 0, "Advanced");

    ASSERT_TRUE(Register != NULL);

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

    ASSERT_EQ(30, PFC_MemoryRegister_Malloc(Register));
    ASSERT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, 0) != NULL);
    ASSERT_EQ(30, PFC_Memory_GetMemoryRegisterSize(memory, 0));

    PFC_MemoryValue * value = PFC_MemoryRegister_GetFirstValue(Register);

    ASSERT_TRUE(value != NULL);

    int Count = PFC_MemoryRegister_GetCount(Register);

    ASSERT_EQ(19, Count);

    uint8_t * RegisterMemory = (uint8_t*)PFC_Memory_GetMemoryRegisterPointer(memory, 0);

    ASSERT_TRUE(RegisterMemory != NULL);

    memset(RegisterMemory, 1, 30);

    for(int i = 0; i < Count; i ++)
    {
        int offset = PFC_MemoryRegister_GetOffsetOfValue(Register, value);
        char valueBuffer[256] = {0};

        PFC_Convert_PFCValueToString(PFC_MemoryValue_GetType(value), true, RegisterMemory + offset, valueBuffer, sizeof(valueBuffer));


        printf("\t%s = %s\n", PFC_MemoryValue_GetName(value), valueBuffer);

        if(i != (Count-1))
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



TEST_F(PFC_MemoryRegistry, test_Memory_MemoryRegister_Mirror)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, 0, "Register");

    ASSERT_TRUE(Register != NULL);

    ASSERT_TRUE(PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORTRPM,          "RPM")              != NULL);

    ASSERT_EQ(2, PFC_MemoryRegister_Malloc(Register));
    ASSERT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, 0) != NULL);
    ASSERT_EQ(2, PFC_Memory_GetMemoryRegisterSize(memory, 0));

    PFC_MemoryValue * value = PFC_MemoryRegister_GetFirstValue(Register);
    ASSERT_TRUE(value != NULL);

    int Count = PFC_MemoryRegister_GetCount(Register);
    ASSERT_EQ(1, Count);


    PFC_MemoryRegister * RegisterMirror = PFC_Memory_NewMirrorRegister(memory, 0, 1, "Mirror");
    ASSERT_TRUE(PFC_MemoryRegister_AddValue(RegisterMirror, PFC_MEMORYTYPE_SHORTRPM,          "RPM")              != NULL);

    ASSERT_EQ(2, PFC_MemoryRegister_Malloc(RegisterMirror));
    ASSERT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, 1) != NULL);
    ASSERT_EQ(2, PFC_Memory_GetMemoryRegisterSize(memory, 1));

    PFC_MemoryValue * mirrorValue = PFC_MemoryRegister_GetFirstValue(RegisterMirror);
    ASSERT_TRUE(mirrorValue != NULL);

    int MirrorCount = PFC_MemoryRegister_GetCount(RegisterMirror);
    ASSERT_EQ(1, MirrorCount);




    uint8_t * RegisterMemory = (uint8_t*)PFC_Memory_GetMemoryRegisterPointer(memory, 0);
    ASSERT_TRUE(RegisterMemory != NULL);

    uint8_t * mirrorRegisterMemory = (uint8_t*)PFC_Memory_GetMemoryRegisterPointer(memory, 1);
    ASSERT_TRUE(mirrorRegisterMemory != NULL);

//
//    memset(RegisterMemory, 1, 30);
//
//    for(int i = 0; i < Count; i ++)
//    {
//        int offset = PFC_MemoryRegister_GetOffsetOfValue(Register, value);
//        char valueBuffer[256] = {0};
//
//        PFC_Convert_PFCValueToString(PFC_MemoryValue_GetType(value), true, RegisterMemory + offset, valueBuffer, sizeof(valueBuffer));
//
//
//        printf("\t%s = %s\n", PFC_MemoryValue_GetName(value), valueBuffer);
//
//        if(i != (Count-1))
//        {
//            ASSERT_EQ(PFC_ERROR_NONE, PFC_MemoryRegister_GetNextValue(Register, &value));
//            ASSERT_TRUE(value != NULL);
//        }
//        else
//        {
//            ASSERT_EQ(PFC_ERROR_NOT_FOUND, PFC_MemoryRegister_GetNextValue(Register, &value));
//            ASSERT_TRUE(value == NULL);
//        }
//
//    }

    PFC_Memory_Free(memory);
}



TEST_F(PFC_MemoryRegistry, test_Memory_MemoryRegister_MapReference)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_Memory_NewRegister(memory, 0, "MapReference");

    ASSERT_TRUE(Register != NULL);



    ASSERT_TRUE(PFC_MemoryRegister_AddValueArray(Register, PFC_MEMORYTYPE_SHORTBOOST,          "Boost", 20)              != NULL);
    ASSERT_TRUE(PFC_MemoryRegister_AddValueArray(Register, PFC_MEMORYTYPE_SHORTRPM,          "RPM", 20)              != NULL);

    ASSERT_EQ(80, PFC_MemoryRegister_Malloc(Register));

    uint8_t * RegisterMemory = (uint8_t*)PFC_Memory_GetMemoryRegisterPointer(memory, 0);

    ASSERT_TRUE(RegisterMemory != NULL);

    memset(RegisterMemory, 1, 80);

    ASSERT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, 0) != NULL);
    ASSERT_EQ(80, PFC_Memory_GetMemoryRegisterSize(memory, 0));

    PFC_MemoryValue * value = PFC_MemoryRegister_GetFirstValue(Register);

    ASSERT_TRUE(value != NULL);


    int Count = PFC_MemoryRegister_GetCount(Register);

    ASSERT_EQ(40, Count);

    for(int i = 0; i < Count; i ++)
    {
        int offset = PFC_MemoryRegister_GetOffsetOfValue(Register, value);
        char valueBuffer[256] = {0};

        PFC_Convert_PFCValueToString(PFC_MemoryValue_GetType(value), true, RegisterMemory + offset, valueBuffer, sizeof(valueBuffer));


        printf("\t%s[%d] = %s\n", PFC_MemoryValue_GetName(value), PFC_MemoryValue_GetIndex(value), valueBuffer);

        if(i != (Count-1))
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
