#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>

#include "pfc_memoryregistry.h"
#include "pfc_memorytypes.h"
#include "pfc_memoryconfig.h"


class PFC_MemoryConfiguration : public testing::Test
{
    void SetUp() {  }
    void TearDown() {  }
};


TEST_F(PFC_MemoryConfiguration, test_MemoryConfig_NewFree)
{
    PFC_MemoryConfig * memoryConfig = PFC_MemoryConfig_New("src/tests/test_memory_config.xml");

    ASSERT_TRUE(memoryConfig != NULL);

    PFC_MemoryConfig_Free(memoryConfig);
}


TEST_F(PFC_MemoryConfiguration, test_MemoryConfig_Load)
{
    PFC_MemoryConfig * memoryConfig = PFC_MemoryConfig_New("src/tests/test_memory_config.xml");

    ASSERT_TRUE(memoryConfig != NULL);

    PFC_MemoryConfig_Load(memoryConfig);

    PFC_Memroy_LoadFCPRO(PFC_MemoryConfig_GetMemory(memoryConfig), "src/tests/FCPRO.hd1");

    PFC_Memory_Dump(PFC_MemoryConfig_GetMemory(memoryConfig));

    PFC_MemoryConfig_Free(memoryConfig);
}