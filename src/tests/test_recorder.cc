#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>

#include <cstdio>
#include <iostream>
#include <fstream>

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

TEST_F(PFC_Recorder, test_record_file) 
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

    std::string pfc_recording_file = std::tmpnam(nullptr);
    ASSERT_EQ(PFC_Recording_WriteToFile(recording, pfc_recording_file.c_str()), PFC_ERROR_NONE);

    uint8_t readBuffer[100] = {0}; 
    PFC_Recording * recordingF = PFC_Recording_New_FromFile(pfc_recording_file.c_str(), readBuffer, 5);
    ASSERT_TRUE(recordingF == NULL);

    recordingF = PFC_Recording_New_FromFile(pfc_recording_file.c_str(), readBuffer, sizeof(readBuffer));
    std::remove(pfc_recording_file.c_str());
    ASSERT_TRUE(recordingF != NULL);

    PFC_Recording_Entry * entry = PFC_Recording_GetEntry(recordingF, 0);
    ASSERT_TRUE(entry != NULL);
    ASSERT_EQ(PFC_Recording_Entry_GetID(entry), 0xf0);
    ASSERT_EQ(PFC_Recording_Entry_GetTimestamp(entry), 1);
   
    entry = PFC_Recording_GetEntry(recordingF, 1);
    ASSERT_TRUE(entry != NULL);
    ASSERT_EQ(PFC_Recording_Entry_GetID(entry), 0xda);
    ASSERT_EQ(PFC_Recording_Entry_GetTimestamp(entry), 222222222);

    entry = PFC_Recording_GetEntry(recordingF, 2);
    ASSERT_TRUE(entry == NULL);

    PFC_Recording_Free(recording);
    PFC_Recording_Free(recordingF);

    PFC_MemoryConfig_Free(memoryConfig);
}
