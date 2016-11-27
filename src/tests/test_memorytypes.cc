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

namespace detail
{
    struct TestMemoryTypeFloatResource
    {
        pfc_memorytype memoryType;
        uint16_t rawValue;
        float expectedConvertedValue;

        pfc_conversion_error expectedResult;
    };

    struct TestMemoryTypeIntResource
    {
        pfc_memorytype memoryType;
        uint16_t rawValue;
        int expectedConvertedValue;

        pfc_conversion_error expectedResult;
    };
}

class TestConvertValueFloat : public testing::Test, public ::testing::WithParamInterface< detail::TestMemoryTypeFloatResource>
{
    void SetUp() {  }
    void TearDown() {  }
};

TEST_P(TestConvertValueFloat, test_MemoryTypes)
{
    detail::TestMemoryTypeFloatResource data = GetParam();
    float convertedValue = 0.0f;

    ASSERT_EQ(data.expectedResult, PFC_Convert_PFCValueToFloat(data.memoryType, &data.rawValue, &convertedValue));

    if(data.expectedResult == PFC_CONVERSION_ERROR_NOERROR)
    {
        ASSERT_EQ(data.expectedConvertedValue, convertedValue);
    }
}


INSTANTIATE_TEST_CASE_P(
        TestConvertValueFloat1,
        TestConvertValueFloat,
        ::testing::Values(
                detail::TestMemoryTypeFloatResource { PFC_MEMORYTYPE_SHORTBOOST, 0x8001, 0.01f, PFC_CONVERSION_ERROR_NOERROR },
                detail::TestMemoryTypeFloatResource { PFC_MEMORYTYPE_SHORTBOOST, 0x8000 + (199), 1.99f, PFC_CONVERSION_ERROR_NOERROR },
                detail::TestMemoryTypeFloatResource { PFC_MEMORYTYPE_SHORTBOOST, 0, -760.0f, PFC_CONVERSION_ERROR_NOERROR },
                detail::TestMemoryTypeFloatResource { PFC_MEMORYTYPE_SHORTBOOST, (760-508), -508.0f, PFC_CONVERSION_ERROR_NOERROR }
        ));


class TestConvertValueInt : public testing::Test, public ::testing::WithParamInterface< detail::TestMemoryTypeIntResource>
{
    void SetUp() {  }
    void TearDown() {  }
};

TEST_P(TestConvertValueInt, test_MemoryTypes)
{
    detail::TestMemoryTypeIntResource data = GetParam();
    int convertedValue = 0.0f;

    ASSERT_EQ(data.expectedResult, PFC_Convert_PFCValueToInt(data.memoryType, &data.rawValue, &convertedValue));

    if(data.expectedResult == PFC_CONVERSION_ERROR_NOERROR)
    {
        ASSERT_EQ(data.expectedConvertedValue, convertedValue);
    }
}


INSTANTIATE_TEST_CASE_P(
        TestConvertValueInt1,
        TestConvertValueInt,
        ::testing::Values(
                detail::TestMemoryTypeIntResource { PFC_MEMORYTYPE_SHORTBOOST, 0x8001, 0, PFC_CONVERSION_ERROR_UNSUPPORTED }
        ));
