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

#include "pfc_memorytypes.h"


class PFC_MemoryTypes : public testing::Test
{
    void SetUp() {  }
    void TearDown() {  }
};


TEST_F(PFC_MemoryTypes, test_MemoryTypes_Boost)
{
    uint16_t rawValue = 0x8001;
    float convertedValue = 0.0f;

    ASSERT_EQ(PFC_CONVERSION_ERROR_NOERROR, PFC_Convert_PFCValueToFloat(PFC_MEMORYTYPE_SHORTBOOST, &rawValue, &convertedValue));
    ASSERT_EQ(0.01f, convertedValue);

    rawValue = 0x8000 + (199);

    ASSERT_EQ(PFC_CONVERSION_ERROR_NOERROR, PFC_Convert_PFCValueToFloat(PFC_MEMORYTYPE_SHORTBOOST, &rawValue, &convertedValue));
    ASSERT_EQ(1.99f, convertedValue);

    rawValue = 0;

    ASSERT_EQ(PFC_CONVERSION_ERROR_NOERROR, PFC_Convert_PFCValueToFloat(PFC_MEMORYTYPE_SHORTBOOST, &rawValue, &convertedValue));
    ASSERT_EQ(-760.0f, convertedValue);

    rawValue = (760-508);

    ASSERT_EQ(PFC_CONVERSION_ERROR_NOERROR, PFC_Convert_PFCValueToFloat(PFC_MEMORYTYPE_SHORTBOOST, &rawValue, &convertedValue));
    ASSERT_EQ(-508.0f, convertedValue);
}




