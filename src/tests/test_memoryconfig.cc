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
    PFC_MemoryConfig * memoryConfig = PFC_MemoryConfig_New("src/tests/test_memory_config.xml", false);

    ASSERT_TRUE(memoryConfig != NULL);

    PFC_MemoryConfig_Free(memoryConfig);
}


TEST_F(PFC_MemoryConfiguration, test_MemoryConfig_LoadFCPro)
{
    PFC_MemoryConfig * memoryConfig = PFC_MemoryConfig_New("src/tests/test_memory_config.xml", false);

    ASSERT_TRUE(memoryConfig != NULL);

    PFC_MemoryConfig_Load(memoryConfig);

    PFC_Memroy_LoadFCPRO(PFC_MemoryConfig_GetMemory(memoryConfig), "src/tests/FCPRO.hd1");

    //PFC_Memory_Dump(PFC_MemoryConfig_GetMemory(memoryConfig));

    uint8_t * memory_data = (uint8_t *)PFC_Memory_GetMemoryRegisterPointer(PFC_MemoryConfig_GetMemory(memoryConfig), 0);
    pfc_size memory_size = PFC_Memory_GetMemoryRegisterSize(PFC_MemoryConfig_GetMemory(memoryConfig), 0);

    ASSERT_TRUE(memory_data != NULL);
    ASSERT_TRUE(memory_size > 0);

    PFC_MemoryConfig_Free(memoryConfig);
}

TEST_F(PFC_MemoryConfiguration, test_MemoryConfig_LoadDump)
{
    PFC_MemoryConfig * memoryConfig = PFC_MemoryConfig_New("src/tests/test_memory_dump.dat", true);

    ASSERT_TRUE(memoryConfig != NULL);

    PFC_MemoryConfig_Load(memoryConfig);


    PFC_Memory_Dump(PFC_MemoryConfig_GetMemory(memoryConfig));

    uint8_t * memory_data = (uint8_t *)PFC_Memory_GetMemoryRegisterPointer(PFC_MemoryConfig_GetMemory(memoryConfig), 0x81);
    pfc_size memory_size = PFC_Memory_GetMemoryRegisterSize(PFC_MemoryConfig_GetMemory(memoryConfig), 0x81);

    ASSERT_TRUE(memory_data != NULL);
    ASSERT_TRUE(memory_size > 0);


}
