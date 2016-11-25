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

#include "pfc_registry.h"


class PFC_Registry : public testing::Test
{
    void SetUp() {  }
    void TearDown() {  }
};


TEST_F(PFC_Registry, test_Memory_NewFree)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_Memory_Free(memory);
}

TEST_F(PFC_Registry, test_Memory_Invalid)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    EXPECT_TRUE(PFC_Memory_GetMemoryRegister(memory, 0) == NULL);
    EXPECT_TRUE(PFC_Memory_GetMemoryRegisterPointer(memory, 0) == NULL);
    EXPECT_TRUE(PFC_Memory_GetMemoryRegisterSize(memory, 0) == 0);

    PFC_Memory_Free(memory);
}

TEST_F(PFC_Registry, test_MemoryRegister_New)
{
    PFC_Memory * memory = PFC_Memory_New();

    ASSERT_TRUE(memory != NULL);

    PFC_MemoryRegister * Register = PFC_MemoryRegister_New(memory, 0, 10, "Test");

    ASSERT_TRUE(Register != NULL);

    PFC_Memory_Free(memory);
}
