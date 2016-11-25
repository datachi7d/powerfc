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


class PFC_Resource_TestSuite : public testing::Test
{
    void SetUp() {  }
    void TearDown() {  }
};


TEST_F(PFC_Resource_TestSuite, test_1)
{
    int i = 1;

    ASSERT_EQ(1, i);
}
