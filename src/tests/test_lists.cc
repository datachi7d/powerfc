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

TEST_F(PFC_List, test_List_GetFirst)
{
    PFC_ValueList * list = PFC_ValueList_New();

    ASSERT_TRUE(list != NULL);

    int i1 = 0;

    ASSERT_EQ(PFC_ERROR_NONE, PFC_ValueList_AddItem(list, &i1));

    int i2 = 0;

    ASSERT_EQ(PFC_ERROR_NONE, PFC_ValueList_AddItem(list, &i2));

    PFC_ValueList * first = PFC_ValueList_GetFirst(list);

    ASSERT_TRUE(first != NULL);

    ASSERT_EQ(&i1, PFC_ValueList_GetValue(first));

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

TEST_F(PFC_List, test_List_RemoveItem)
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


    PFC_ValueList_RemoveItem(base, i1_ptr);

    EXPECT_EQ(PFC_ValueList_Count(base), 1);

    PFC_ValueList_RemoveItem(base, i2_ptr);

    EXPECT_EQ(PFC_ValueList_Count(base), 0);

    PFC_ValueList_Free(base);
}

TEST_F(PFC_List, test_List_RemoveItem_Reverse)
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

    PFC_ValueList_RemoveItem(base, i2_ptr);

    EXPECT_EQ(PFC_ValueList_Count(base), 1);

    PFC_ValueList_RemoveItem(base, i1_ptr);

    EXPECT_EQ(PFC_ValueList_Count(base), 0);

    PFC_ValueList_Free(base);
}
