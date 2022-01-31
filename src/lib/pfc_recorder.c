#include "pfc_recorder.h"
#include "pfc_memory.h"
#include "pfc_memoryconfig.h"
#include "xmltree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct _PFC_Recording
{
    uint32_t MemoryPosition;
    uint32_t MemorySize;
    int RecordingEntries;
    uint8_t * MemoryBuffer;
};

typedef struct __attribute__((__packed__)) _PFC_RecordingEntry_v1
{
    struct  __attribute__((__packed__))
    {
        uint32_t header;
        uint64_t timestamp;
        PFC_ID type;
        uint8_t length;
    } upper_header;
    uint8_t data[];
} PFC_RecordingEntry_v1;

struct _PFC_Recording_Entry 
{
    union
    {
        PFC_RecordingEntry_v1 v1entry;
    };  
};

#define PFC_RecordingEntryHeaderV1 0xcafe0fc1


PFC_Recording * PFC_Recording_New(uint8_t * MemoryBuffer, uint32_t MemorySize)
{
    PFC_Recording * recordingValue = NULL;
    if(MemoryBuffer != 0 && MemorySize != 0)
    {
        recordingValue = PFC_malloc(sizeof(*recordingValue));
        recordingValue->MemoryBuffer = MemoryBuffer;
        recordingValue->MemoryPosition = 0;
        recordingValue->MemorySize = MemorySize;
        recordingValue->RecordingEntries = 0;
    }

    return recordingValue;
}

pfc_error PFC_Recording_AddEntry(PFC_Recording * Recording, PFC_Memory * Memory, PFC_ID id, uint64_t timestamp)
{
    pfc_error result = PFC_ERROR_UNSET;

    if(Recording != NULL)
    {
        PFC_MemoryRegister * memoryRegister = PFC_Memory_GetMemoryRegister(Memory, id);

        if(memoryRegister != NULL)
        {
            const uint8_t * memoryData = (const uint8_t *)PFC_Memory_GetMemoryRegisterPointer(Memory, id);

            if(memoryData != NULL)
            {
                PFC_RecordingEntry_v1 * entry = (PFC_RecordingEntry_v1 *)&Recording->MemoryBuffer[Recording->MemoryPosition];
                pfc_size entryDataSize = PFC_MemoryRegister_GetSize(memoryRegister);
                uint16_t entrySize = sizeof(entry->upper_header) + entryDataSize;

                if(entrySize + Recording->MemoryPosition < Recording->MemorySize)
                {
                    entry->upper_header.header = PFC_RecordingEntryHeaderV1;
                    entry->upper_header.length = entryDataSize;
                    entry->upper_header.timestamp = timestamp;
                    entry->upper_header.type = id;
                    memcpy(entry->data, memoryData, entryDataSize);
                    Recording->RecordingEntries++;
                    Recording->MemoryPosition += entrySize;

                    result = PFC_ERROR_NONE;
                }
                else
                {
                    result = PFC_ERROR_MEMORY;
                }
            }
            else
            {
                result = PFC_ERROR_NOT_FOUND;
            }
        }
        else
        {
            result = PFC_ERROR_NOT_FOUND;
        }
    }
    else
    {
        result = PFC_ERROR_NULL_PARAMETER;
    }

    return result;
}

pfc_error PFC_Recording_WriteToFile(PFC_Recording * Recording, const char * file)
{
    pfc_error result = PFC_ERROR_NONE;
    if(Recording != NULL)
    {
        int fd = open(file, O_WRONLY | O_TRUNC | O_SYNC | O_CREAT, 0644);
        if(fd >= 0)
        {
            ssize_t totalWritten = 0;
            
            while(totalWritten < Recording->MemoryPosition)
            {
                ssize_t written = write(fd, Recording->MemoryBuffer, Recording->MemoryPosition - totalWritten);

                if(written > 0)
                {
                    totalWritten += written;
                }
                else
                {
                    result = PFC_ERROR_FILEIO;
                    break;
                }
            }

            fsync(fd);

            if(totalWritten != Recording->MemoryPosition)
            {
                result = PFC_ERROR_FILEIO;
            }
            
            close(fd);
        }
        else
        {
            result = PFC_ERROR_FILEIO;
        }
    }
    else
    {
        result = PFC_ERROR_NULL_PARAMETER;
    }

    return result;
}

PFC_Recording * PFC_Recording_New_FromFile(const char * file, uint8_t * MemoryBuffer, uint32_t MemorySize)
{
    PFC_Recording * recording = NULL;
    if(MemoryBuffer != NULL && MemorySize > 0)
    {
        int fd = open(file, O_RDONLY);

        if(fd >= 0)
        {
            struct stat fd_info = {0};
            int status = 0;
            status = fstat(fd, &fd_info);
            if(status == 0)
            {
                if(fd_info.st_size <= MemorySize)
                {
                    recording = PFC_Recording_New(MemoryBuffer, MemorySize);
                    recording->RecordingEntries = -1;
                    ssize_t totalRead = 0;
                    ssize_t currentRead = 0;

                    while(totalRead < fd_info.st_size)
                    {
                        currentRead = read(fd, MemoryBuffer, MemorySize - totalRead);
                        if(currentRead > 0)
                        {
                            totalRead += currentRead;
                        }
                        else
                        {
                            PFC_Recording_Free(recording);
                            recording = NULL;
                            break;
                        }
                    }
                }
                else
                {
                    printf("Size too small %ld\n", fd_info.st_size);
                }
            }
            else
            {
                printf("Status error\n");
            }

            close(fd);
        }
        else
        {
            printf("File error: %s\n", file);
        }
    }
    
    return recording;
}

pfc_error PFC_Recording_Reset(PFC_Recording * Recording)
{
    pfc_error result = PFC_ERROR_UNSET;

    if(Recording != NULL)
    {
        Recording->MemoryPosition = 0;
        Recording->RecordingEntries = 0;
        result = PFC_ERROR_NONE;
    }
    else
    {
        result = PFC_ERROR_NULL_PARAMETER;
    }


    return result;
}

PFC_Recording_Entry * PFC_Recording_GetEntry(PFC_Recording * Recording, uint16_t entryNumber)
{    
    PFC_Recording_Entry * entry = NULL;
    if(Recording != NULL && (entryNumber < Recording->RecordingEntries || Recording->RecordingEntries < 0))
    { 
        uint16_t currentEntry = 0;
        uint32_t  memoryPos = 0;
        do
        {
            entry = (PFC_Recording_Entry *)&Recording->MemoryBuffer[memoryPos];
            if(entry->v1entry.upper_header.header == PFC_RecordingEntryHeaderV1)
            {
                uint16_t headerSize =  sizeof(entry->v1entry.upper_header);
                memoryPos += entry->v1entry.upper_header.length + headerSize;
                if(currentEntry == entryNumber)
                {
                    memoryPos = Recording->MemorySize;
                } 
                else
                {
                    currentEntry++;
                }
                
            }
            else
            {
                memoryPos = Recording->MemorySize;
                entry = NULL;
            }
        } while(memoryPos < Recording->MemorySize);
    }

    return entry;
}

uint64_t PFC_Recording_Entry_GetTimestamp(PFC_Recording_Entry * Entry)
{
    uint64_t value = 0;
    if(Entry != NULL)
    {
        if(Entry->v1entry.upper_header.header == PFC_RecordingEntryHeaderV1)
        {
            value = Entry->v1entry.upper_header.timestamp;
        }
    }

    return value;
}

PFC_ID PFC_Recording_Entry_GetID(PFC_Recording_Entry * Entry)
{
    PFC_ID value = 0;
    if(Entry != NULL)
    {
        if(Entry->v1entry.upper_header.header == PFC_RecordingEntryHeaderV1)
        {
            value = Entry->v1entry.upper_header.type;
        }
    }

    return value;
}

const uint8_t * PFC_Recording_Entry_GetData(PFC_Recording_Entry * Entry)
{
    const uint8_t * value = NULL;
    if(Entry != NULL)
    {
        if(Entry->v1entry.upper_header.header == PFC_RecordingEntryHeaderV1)
        {
            value = Entry->v1entry.data;
        }
    }

    return value;
}

pfc_size PFC_Recording_Entry_GetDataLength(PFC_Recording_Entry * Entry)
{
    pfc_size value = 0;
    if(Entry != NULL)
    {
        if(Entry->v1entry.upper_header.header == PFC_RecordingEntryHeaderV1)
        {
            value = Entry->v1entry.upper_header.length;
        }
    }

    return value;
}

void PFC_Recording_Free(PFC_Recording * ptr)
{
    if(ptr != NULL)
    {
        PFC_free(ptr);
    }
}
