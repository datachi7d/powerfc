#include "pfc_memoryconfig.h"
#include "pfc_memory.h"
#include "pfc_memoryregistry.h"
#include "xmltree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct _PFC_MemoryConfig
{
	char * FileName;
	FILE * FileP;
	PFC_Memory * Memory;
};

char * XML_ToLower(const char * str)
{
    char * p = PFC_strdup(str);
    char * result = p;

    if(p != NULL)
    {
        for ( ; *p; ++p)
        {
            *p = tolower(*p);
        }
    }

    return result;
}

bool XML_NodeNameIs(TreeNode node, const char * name)
{
    bool result = false;
    char * lowerStr = XML_ToLower(TreeNode_GetName(node));

    if(TreeNode_GetName(node) != NULL && lowerStr != NULL && strcmp(name, lowerStr) == 0)
    {
        result = true;
    }

    if(lowerStr != NULL)
    {
        PFC_free(lowerStr);
    }

    return result;
}

TreeNode XML_GetChild(TreeNode node, const char * name)
{
    TreeNode result = NULL;
    TreeNode child = NULL;
    int child_number = 0;

    while ((child = TreeNode_GetChild(node, child_number)))
    {
        if(XML_NodeNameIs(child, name))
        {
            result = child;
            break;
        }

        child_number++;
    }

    return result;
}

const char * XML_GetChildValue(TreeNode node, const char * name)
{
    const char * value = NULL;

    TreeNode child = XML_GetChild(node,name);

    if(child != NULL)
    {
         value = TreeNode_GetValue(child);
    }

    return value;
}

pfc_error XML_GetChildValueAsHex(TreeNode node, const char * name, uint16_t * hex)
{
    pfc_error Result = PFC_ERROR_UNSET;
    const char * value = XML_GetChildValue(node, name);

    if(value != NULL)
    {
        unsigned int i = 0;
        int res = sscanf(value, "0x%x", &i);

        if(res == 1)
        {
            *hex = i;
            Result = PFC_ERROR_NONE;
        }
        else
        {
            Result = PFC_ERROR_XML;
        }
    }
    else
    {
        Result = PFC_ERROR_XML;
    }

    return Result;
}

int XML_GetChildValueAsRawInt(TreeNode node, const char * name)
{
    int Result = 0;
    const char * value = XML_GetChildValue(node, name);

    if(value != NULL)
    {
        sscanf(value, "%d", &Result);
    }

    return Result;
}

PFC_MemoryConfig * PFC_MemoryConfig_New(const char * fileName)
{
    PFC_MemoryConfig * memoryConfig = NULL;

    if(fileName != NULL)
    {
    	if((memoryConfig = (PFC_MemoryConfig *)PFC_malloc(sizeof(*memoryConfig))) != NULL)
    	{
    		memoryConfig->FileName = (char *)PFC_strdup(fileName);
    		memoryConfig->FileP = NULL;
    		memoryConfig->Memory = NULL;

    		if(memoryConfig->FileName == NULL)
    		{
    			PFC_free(memoryConfig);
    			memoryConfig = NULL;
    		}
    	}
    }

    return memoryConfig;
}

void XML_PrintErrorChild(TreeNode parent, const char * name, const char * error)
{
    TreeNode errorNode = XML_GetChild(parent, name);

    printf("Node %s line:%d:%d\n", TreeNode_GetName(errorNode), TreeNode_GetLine(errorNode), TreeNode_GetCharacter(errorNode));

}

pfc_error MemoryConfig_LoadConfig_MemoryMap(PFC_Memory * Memory, TreeNode root)
{
    pfc_error Result = PFC_ERROR_UNSET;

    if(Memory != NULL && XML_NodeNameIs(root, XML_PFC_MEMORY_MAP))
    {
        const char * Name = XML_GetChildValue(root, XML_NAME);
        const char * MemroyTypeRaw = XML_GetChildValue(root, XML_MEMORY_TYPE);
        pfc_memorytype MemoryType = PFC_MemoryType_FromString(MemroyTypeRaw);
        uint16_t RegisterIDMin = 0;
        uint16_t RegisterIDMax = 0;
        PFC_MemoryMap * memoryMap = NULL;

        int Columns = XML_GetChildValueAsRawInt(root, XML_COLUMNS);
        int Rows = XML_GetChildValueAsRawInt(root, XML_ROWS);

        Result = XML_GetChildValueAsHex(root, XML_REGISTERIDMIN, &RegisterIDMin);

        if(Result == PFC_ERROR_NONE && MemoryType != PFC_MEMORYTYPE_LAST && Name != NULL && Columns > 0 && Rows > 0)
        {
            Result = XML_GetChildValueAsHex(root, XML_REGISTERIDMAX, &RegisterIDMax);

            if(Result == PFC_ERROR_NONE)
            {
                memoryMap = PFC_Memory_NewMap(Memory, RegisterIDMin, RegisterIDMax, MemoryType, Columns, Rows, Name);

                if(memoryMap != NULL)
                {
                    Result = PFC_ERROR_NONE;
                }
                else
                {
                    Result = PFC_ERROR_XML;
                }
            }
            else
            {
                Result = PFC_ERROR_XML;
            }
        }
        else
        {
            if(MemoryType == PFC_MEMORYTYPE_LAST)
            {
                //TODO:
                printf("Unkown memory type: %s for %s:\n", MemroyTypeRaw, Name);
                TreeNode error = XML_GetChild(root, XML_MEMORY_TYPE);
                XML_PrintErrorChild(root, XML_MEMORY_TYPE, "test");

                Result = PFC_ERROR_XML;
            }
            else
            {
                Result = PFC_ERROR_XML;
            }
        }
    }
    else
    {
        if(Memory != NULL)
        {
            Result = PFC_ERROR_XML;
        }
        else
        {
            Result = PFC_ERROR_NULL_PARAMETER;
        }
    }

    return Result;
}

pfc_error MemoryConfig_LoadConfig_MemoryValue(PFC_MemoryRegister * MemoryRegister, TreeNode root)
{
    pfc_error Result = PFC_ERROR_UNSET;

    if(MemoryRegister != NULL && XML_NodeNameIs(root, XML_PFC_MEMORY_VALUES))
    {
        int child_number = 0;
        TreeNode child;

        while ((child = TreeNode_GetChild(root, child_number)))
        {
            if(XML_NodeNameIs(child, XML_PFC_MEMORY_VALUE) || XML_NodeNameIs(child, XML_PFC_MEMORY_VALUE_ARRAY))
            {
                const char * Name = XML_GetChildValue(child, XML_NAME);
                const char * MemroyTypeRaw = XML_GetChildValue(child, XML_MEMORY_TYPE);
                pfc_memorytype MemoryType = PFC_MemoryType_FromString(MemroyTypeRaw);
                PFC_MemoryValue * memoryValue = NULL;

                if(MemoryType == PFC_MEMORYTYPE_LAST)
                {
                    //TODO:
                    printf("Unkown memory type: %s\n", MemroyTypeRaw);
                    XML_PrintErrorChild(child, XML_MEMORY_TYPE, "test");
                    Result = PFC_ERROR_NONE;
                    break;
                }


                if(XML_NodeNameIs(child, XML_PFC_MEMORY_VALUE_ARRAY))
                {
                    int Size = XML_GetChildValueAsRawInt(child, XML_COUNT);

                    if(Size > 0)
                    {
                        memoryValue = PFC_MemoryRegister_AddValueArray(MemoryRegister, MemoryType, Name, Size);

                        if(memoryValue != NULL)
                        {
                            Result = PFC_ERROR_NONE;
                        }
                        else
                        {
                            //TODO
                            Result = PFC_ERROR_UNSET;
                        }
                    }
                    else
                    {
                        Result = PFC_ERROR_XML;
                        break;
                    }
                }
                else
                {
                    memoryValue = PFC_MemoryRegister_AddValue(MemoryRegister, MemoryType, Name);
                    Result = PFC_ERROR_NONE;
                }

                if(memoryValue == NULL)
                {
                    Result = PFC_ERROR_XML;
                    break;
                }
            }
            else
            {
                Result = PFC_ERROR_XML;
                break;
            }

            child_number++;
        }
    }
    else
    {
        if(MemoryRegister != NULL)
        {
            Result = PFC_ERROR_XML;
        }
        else
        {
            Result = PFC_ERROR_NULL_PARAMETER;
        }
    }

    return Result;
}

pfc_error MemoryConfig_LoadConfig_Memory(PFC_MemoryConfig * MemoryConfig, TreeNode root)
{
    pfc_error Result = PFC_ERROR_UNSET;

    if(MemoryConfig != NULL && XML_NodeNameIs(root, XML_PFC_MEMORY))
    {
        MemoryConfig->Memory = PFC_Memory_New();

        if(MemoryConfig->Memory != NULL)
        {
            int child_number = 0;
            TreeNode child;

            while ((child = TreeNode_GetChild(root, child_number)))
            {
                if(XML_NodeNameIs(child, XML_PFC_MEMORY_REGISTER))
                {
                    const char * Name = XML_GetChildValue(child, XML_NAME);
                    uint16_t RegisterID = 0;
                    int Size = XML_GetChildValueAsRawInt(child, XML_SIZE);


                    PFC_MemoryRegister * MemoryRegister = NULL;

                    TreeNode valuesNode = XML_GetChild(child, XML_PFC_MEMORY_VALUES);

                    Result = XML_GetChildValueAsHex(child, XML_REGISTERID, &RegisterID);

                    if(Result != PFC_ERROR_NONE && Size > 0)
                    {
                        if(Size <= 0)
                        {
                            Result = PFC_ERROR_XML;
                        }

                        break;
                    }

                    if(valuesNode != NULL && (MemoryRegister = PFC_Memory_NewRegister(MemoryConfig->Memory, RegisterID, Size, Name)) != NULL)
                    {
                        Result = MemoryConfig_LoadConfig_MemoryValue(MemoryRegister, valuesNode);

                        if(Result != PFC_ERROR_NONE)
                        {
                            break;
                        }
                    }
                    else
                    {
                        Result = PFC_ERROR_XML;
                        break;
                    }
                }
                else if (XML_NodeNameIs(child, XML_PFC_MEMORY_MAP))
                {
                    Result = MemoryConfig_LoadConfig_MemoryMap(MemoryConfig->Memory, child);

                    if(Result != PFC_ERROR_NONE)
                    {
                        break;
                    }
                }
                else
                {
                    Result = PFC_ERROR_XML;
                    break;
                }

                child_number++;
            }

            if(Result != PFC_ERROR_NONE)
            {
                PFC_Memory_Free(MemoryConfig->Memory);
                MemoryConfig->Memory = NULL;
            }
        }
        else
        {
            Result = PFC_ERROR_MEMORY;
        }
    }
    else
    {
        if(MemoryConfig != NULL)
        {
            Result = PFC_ERROR_XML;
        }
        else
        {
            Result = PFC_ERROR_NULL_PARAMETER;
        }
    }

    return Result;
}

pfc_error PFC_MemoryConfig_LoadString(PFC_MemoryConfig * MemoryConfig, const char * string, uint32_t length)
{
    pfc_error Result = PFC_ERROR_UNSET;
    TreeNode * root = TreeNode_ParseXML((char *)string, length, true);

    if(root != NULL && XML_NodeNameIs(root, XML_PFC_MEMORY_CONFIG))
    {
        Result = MemoryConfig_LoadConfig_Memory(MemoryConfig, TreeNode_GetChild(root, 0));
        //TODO: free TreeNode
    }
    else
    {
        Result = PFC_ERROR_XML;
    }

    return Result;
}

pfc_error  MemoryConfig_ReadFile(PFC_MemoryConfig * MemoryConfig)
{
    pfc_error Result = PFC_ERROR_UNSET;

    if(MemoryConfig != NULL && MemoryConfig->FileP != NULL)
    {

        char * file_buffer = NULL;

        if(fseek(MemoryConfig->FileP, 0L, SEEK_END) >= 0)
        {
            long file_buffer_size = ftell(MemoryConfig->FileP);

            if(file_buffer_size >= 0)
            {
                rewind(MemoryConfig->FileP);
                file_buffer = PFC_malloc((uint32_t)file_buffer_size);

                if(file_buffer != NULL)
                {
                    if(fread(file_buffer, file_buffer_size, 1, MemoryConfig->FileP) > 0)
                    {
                        Result = PFC_MemoryConfig_LoadString(MemoryConfig, file_buffer, file_buffer_size);
                    }

                    PFC_free(file_buffer);
                }
                else
                {
                    Result = PFC_ERROR_MEMORY;
                }
            }
            else
            {
                Result = PFC_ERROR_FILEIO;
            }
        }
        else
        {
            Result = PFC_ERROR_FILEIO;
        }

    }

    return Result;
}

pfc_error PFC_MemoryConfig_Load(PFC_MemoryConfig * MemoryConfig)
{
    pfc_error Result = PFC_ERROR_UNSET;

    if(MemoryConfig != NULL)
    {
        MemoryConfig->FileP = fopen(MemoryConfig->FileName, "rb");

        if(MemoryConfig->FileP != NULL)
        {
            Result = MemoryConfig_ReadFile(MemoryConfig);
        }
        else
        {
            Result = PFC_ERROR_FILEIO;
        }
    }
    else
    {
        Result = PFC_ERROR_NULL_PARAMETER;
    }

    return Result;
}

PFC_Memory * PFC_MemoryConfig_GetMemory(PFC_MemoryConfig * MemoryConfig)
{
    PFC_Memory * Result = NULL;

    if(MemoryConfig != NULL)
    {
        Result = MemoryConfig->Memory;
    }

    return Result;
}

void PFC_MemoryConfig_Free(PFC_MemoryConfig * MemoryConfig)
{
    if(MemoryConfig != NULL)
    {
        if(MemoryConfig->FileName != NULL)
            PFC_free(MemoryConfig->FileName);

        if(MemoryConfig->FileP != NULL)
            fclose(MemoryConfig->FileP);

        if(MemoryConfig->Memory != NULL)
            PFC_Memory_Free(MemoryConfig->Memory);

        PFC_free(MemoryConfig);
    }
}
