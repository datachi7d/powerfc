#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>

#include "pfc_memoryregistry.h"
#include "pfc_memorytypes.h"
#include "pfc_memoryconfig.h"
#include "pfc_recorder.h"


class PFC_Recorder : public testing::Test
{
    void SetUp() {  }
    void TearDown() {  }
};


TEST_F(PFC_Recorder, test_record) 
{
    PFC_MemoryConfig * memoryConfig = PFC_MemoryConfig_New("test_memory_config.xml", false);
    ASSERT_TRUE(memoryConfig != NULL);
    ASSERT_EQ(PFC_MemoryConfig_Load(memoryConfig), PFC_ERROR_NONE);
    PFC_Memory_LoadFCPRO(PFC_MemoryConfig_GetMemory(memoryConfig), "FCPRO.hd1");

    PFC_Memory * memory = PFC_MemoryConfig_GetMemory(memoryConfig);
    ASSERT_TRUE(memory != NULL);

    uint8_t buffer[100];
    PFC_Recording * recording = PFC_Recording_New(buffer, sizeof(buffer));
    ASSERT_TRUE(recording != NULL);

    ASSERT_EQ(PFC_Recording_AddEntry(recording, memory, 0xf0, 1), PFC_ERROR_NONE);
    ASSERT_EQ(PFC_Recording_AddEntry(recording, memory, 0xda, 222222222), PFC_ERROR_NONE);
    ASSERT_EQ(PFC_Recording_AddEntry(recording, memory, 0xf0, 344241), PFC_ERROR_MEMORY);

    PFC_Recording_Entry * entry = PFC_Recording_GetEntry(recording, 0);
    ASSERT_TRUE(entry != NULL);
    ASSERT_EQ(PFC_Recording_Entry_GetID(entry), 0xf0);
    ASSERT_EQ(PFC_Recording_Entry_GetTimestamp(entry), 1);
    
    entry = PFC_Recording_GetEntry(recording, 1);
    ASSERT_TRUE(entry != NULL);
    ASSERT_EQ(PFC_Recording_Entry_GetID(entry), 0xda);
    ASSERT_EQ(PFC_Recording_Entry_GetTimestamp(entry), 222222222);

    entry = PFC_Recording_GetEntry(recording, 2);
    ASSERT_TRUE(entry == NULL);

    PFC_Recording_Free(recording);

    PFC_MemoryConfig_Free(memoryConfig);
}