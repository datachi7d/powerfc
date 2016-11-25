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

#include "pfc_list.h"


class PFC_List : public testing::Test
{
    void SetUp() {  }
    void TearDown() {  }
};


TEST_F(PFC_List, test_List_NewFree)
{
    PFC_ValueList * list = PFC_ValueList_New();

    ASSERT_TRUE(list != NULL);

    PFC_ValueList_Free(list);
}

TEST_F(PFC_List, test_List_AddItem)
{
    PFC_ValueList * list = PFC_ValueList_New();

    ASSERT_TRUE(list != NULL);

    int i = 0;

    ASSERT_EQ(PFC_ERROR_NONE, PFC_ValueList_AddItem(list, &i));

    PFC_ValueList_Free(list);
}

TEST_F(PFC_List, test_List_AddItem_Error)
{
    PFC_ValueList * list = PFC_ValueList_New();

    ASSERT_TRUE(list != NULL);

    int i = 0;

    ASSERT_EQ(PFC_ERROR_NULL_PARAMETER, PFC_ValueList_AddItem(list, NULL));

    ASSERT_EQ(PFC_ERROR_NULL_PARAMETER, PFC_ValueList_AddItem(NULL, &i));


    PFC_ValueList_Free(list);
}

TEST_F(PFC_List, test_List_GetValue)
{
    PFC_ValueList * list = PFC_ValueList_New();
    PFC_ValueList * base = list;

    ASSERT_TRUE(list != NULL);

    int i1 = 1;
    int i2 = 2;

    ASSERT_EQ(PFC_ERROR_NONE, PFC_ValueList_AddItem(list, &i1));
    ASSERT_EQ(PFC_ERROR_NONE, PFC_ValueList_AddItem(list, &i2));

    int * i1_ptr = (int *)PFC_ValueList_GetValue(list);

    EXPECT_TRUE(i1_ptr != NULL);
    EXPECT_EQ(&i1, i1_ptr);

    int * i2_ptr = (int *)PFC_ValueList_NextItemValue(&list);

    EXPECT_TRUE(i2_ptr != NULL);
    EXPECT_EQ(&i2, i2_ptr);

    PFC_ValueList_Free(base);
}
