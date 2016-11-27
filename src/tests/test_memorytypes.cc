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
    struct TestMemoryTypeResource
    {
        pfc_memorytype memoryType;
        uint16_t rawValue;
        float expectedConvertedValue;

        pfc_conversion_error expectedResult;
    };
}

class TestConvertValueFloat : public testing::Test, public ::testing::WithParamInterface< detail::TestMemoryTypeResource>
{
    void SetUp() {  }
    void TearDown() {  }
};

TEST_P(TestConvertValueFloat, test_MemoryTypes)
{
    detail::TestMemoryTypeResource data = GetParam();
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
                detail::TestMemoryTypeResource { PFC_MEMORYTYPE_SHORTBOOST, 0x8001, 0.01f, PFC_CONVERSION_ERROR_NOERROR },
                detail::TestMemoryTypeResource { PFC_MEMORYTYPE_SHORTBOOST, 0x8000 + (199), 1.99f, PFC_CONVERSION_ERROR_NOERROR },
                detail::TestMemoryTypeResource { PFC_MEMORYTYPE_SHORTBOOST, 0, -760.0f, PFC_CONVERSION_ERROR_NOERROR },
                detail::TestMemoryTypeResource { PFC_MEMORYTYPE_SHORTBOOST, (760-508), -508.0f, PFC_CONVERSION_ERROR_NOERROR }
        ));


