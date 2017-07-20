#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>

#include "serial.h"


class PFC_Serial : public testing::Test
{
    void SetUp() {  }
    void TearDown() {  }
};


TEST_F(PFC_Serial, test_Serial_NewFree)
{
    Serial * serial = Serial_New("");

    ASSERT_TRUE(serial == NULL);

    Serial_Free(serial);
}
