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

    PFC_MemoryRegister * Register = PFC_MemoryRegister_New(memory, 0, 10, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_Memory_Free(memory);
}


TEST_F(PFC_MemoryRegistry, test_MemoryRegister_AddValue)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_MemoryRegister_New(memory, 0, 10, "Test");

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

    PFC_MemoryRegister * Register = PFC_MemoryRegister_New(memory, 0, 2, "Test");

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

    PFC_MemoryRegister * Register = PFC_MemoryRegister_New(memory, 0, PFC_SIZE_SHORT * 2, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_MemoryValue * memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test1");

    ASSERT_TRUE(memoryValue != NULL);

    ASSERT_EQ(PFC_SIZE_SHORT, PFC_MemoryRegister_GetSize(Register));

    memoryValue = PFC_MemoryRegister_AddValue(Register, PFC_MEMORYTYPE_SHORT, "Test2");

    ASSERT_TRUE(memoryValue != NULL);

    ASSERT_EQ(PFC_SIZE_SHORT*2, PFC_MemoryRegister_GetSize(Register));

    ASSERT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, 0) != NULL);
    ASSERT_EQ(PFC_SIZE_SHORT * 2, PFC_Memory_GetMemoryRegisterSize(memory, 0));


    PFC_Memory_Free(memory);
}





