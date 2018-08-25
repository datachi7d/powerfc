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
	bool MemoryDump;
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

pfc_error XML_GetChildValueAsHexArray(TreeNode node, const char * name, uint8_t * hex, pfc_size hex_size)
{
    pfc_error Result = PFC_ERROR_UNSET;
    const char * value = XML_GetChildValue(node, name);

    if(value != NULL)
    {
        unsigned int i = 0;

        do
        {
            int res = sscanf(value, "0x%x ", &(i));

			if(res == 1)
			{
				hex_size--;
				*(hex++) = i;
				Result = PFC_ERROR_NONE;
			}
			else
			{
				Result = PFC_ERROR_XML;
			}

            if(strlen(value) >= 5)
            {
            	value+=5;
            }
            else if (hex_size != 0)
            {
            	Result = PFC_ERROR_XML;
            }

        }while(Result == PFC_ERROR_NONE && hex_size > 0);
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

PFC_MemoryConfig * PFC_MemoryConfig_New(const char * fileName, bool memoryDump)
{
    PFC_MemoryConfig * memoryConfig = NULL;

    if(fileName != NULL)
    {
    	if((memoryConfig = (PFC_MemoryConfig *)PFC_malloc(sizeof(*memoryConfig))) != NULL)
    	{
    		memoryConfig->FileName = (char *)PFC_strdup(fileName);
    		memoryConfig->FileP = NULL;
    		memoryConfig->Memory = NULL;
    		memoryConfig->MemoryDump = memoryDump;

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


pfc_error MemoryConfig_LoadConfig_MemoryMap(PFC_Memory * Memory, TreeNode root, int FCPOffset)
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

                if(FCPOffset >= 0)
                	PFC_MemoryMap_SetFCPOffset(memoryMap, FCPOffset);

                if(memoryMap != NULL)
                {
                	PFC_MemoryMap_Malloc(memoryMap);

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


pfc_error MemoryConfig_LoadConfig_MemoryRegister(PFC_Memory * Memory, TreeNode root, int FCPOffset)
{
    pfc_error Result = PFC_ERROR_UNSET;

    const char * Name = XML_GetChildValue(root, XML_NAME);
    uint16_t RegisterID = 0;
    uint16_t MirrorRegisterID;
    pfc_error MirrorRegisterNodeResult = PFC_ERROR_UNSET;
    pfc_size RegisterSize = 0;

    PFC_MemoryRegister * MemoryRegister = NULL;

    TreeNode valuesNode = XML_GetChild(root, XML_PFC_MEMORY_VALUES);

    Result = XML_GetChildValueAsHex(root, XML_REGISTERID, &RegisterID);

    MirrorRegisterNodeResult = XML_GetChildValueAsHex(root, XML_MIRRORREGISTERID, &MirrorRegisterID);

    if(Result == PFC_ERROR_NONE)
    {
        if(MirrorRegisterNodeResult == PFC_ERROR_NONE)
        {
            MemoryRegister = PFC_Memory_NewMirrorRegister(Memory, MirrorRegisterID, RegisterID, Name);
        }
        else
        {
            MemoryRegister = PFC_Memory_NewRegister(Memory, RegisterID, Name);
        }


        if(valuesNode != NULL && MemoryRegister != NULL)
        {
            Result = MemoryConfig_LoadConfig_MemoryValue(MemoryRegister, valuesNode);

            if(FCPOffset >= 0)
                PFC_MemoryRegister_SetFCPOffset(MemoryRegister, FCPOffset);

            RegisterSize = PFC_MemoryRegister_Malloc(MemoryRegister);

            if(XML_GetChild(root, XML_DEFAULTVALUE) != NULL)
            {
                if(RegisterSize > 0)
                {
                    uint8_t * buffer = (uint8_t *)PFC_malloc(RegisterSize);

                    if(XML_GetChildValueAsHexArray(root, XML_DEFAULTVALUE, buffer, RegisterSize) == PFC_ERROR_NONE)
                    {
                        void * dest = PFC_Memory_GetMemoryRegisterPointer(Memory, RegisterID);

                        if(dest != NULL)
                        {
                            memcpy(dest, buffer, RegisterSize);
                        }
                    }

                    PFC_free(buffer);
                }
            }

            if(XML_GetChild(root, XML_FCPREORDER) != NULL && FCPOffset >= 0)
            {
                uint8_t * PFCReorder = (uint8_t *)PFC_malloc(RegisterSize);

                if(PFCReorder)
                {
                    if(XML_GetChildValueAsHexArray(root, XML_FCPREORDER, PFCReorder, RegisterSize) == PFC_ERROR_NONE)
                    {
                        PFC_MemoryRegister_SetFCPReorder(MemoryRegister, PFCReorder, RegisterSize);
                    }
                }

                PFC_free(PFCReorder);
            }

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


pfc_error MemoryConfig_LoadConfig_TableColumns(TreeNode root, pfc_memorytype * Columns,const char ** ColumnNames, int ColumsSize)
{
    pfc_error Result = PFC_ERROR_UNSET;

    if(XML_NodeNameIs(root, XML_PFC_MEMORY_VALUES))
    {
        int child_number = 0;
        TreeNode child;

        while ((child = TreeNode_GetChild(root, child_number)))
        {
            if(XML_NodeNameIs(child, XML_PFC_MEMORY_VALUE))
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
                else if(child_number < ColumsSize)
                {
                	Columns[child_number] = MemoryType;
                	ColumnNames[child_number] = Name;
                	Result = PFC_ERROR_NONE;
                }
                else
                {
                    Result = PFC_ERROR_MEMORY;
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
    	Result = PFC_ERROR_NULL_PARAMETER;
    }

    return Result;
}

pfc_error MemoryConfig_LoadConfig_MemoryTable(PFC_Memory * Memory, TreeNode root, int FCPOffset)
{
    pfc_error Result = PFC_ERROR_UNSET;

    const char * Name = XML_GetChildValue(root, XML_NAME);
    int TableRows =  XML_GetChild(root, XML_ROWS) != NULL ? XML_GetChildValueAsRawInt(root, XML_ROWS) : -1;
    uint16_t RegisterID = 0;
    pfc_size RegisterSize = 0;

    PFC_MemoryTable * MemoryTable = NULL;

    TreeNode valuesNode = XML_GetChild(root, XML_PFC_MEMORY_VALUES);

    Result = XML_GetChildValueAsHex(root, XML_REGISTERID, &RegisterID);

    if(Result == PFC_ERROR_NONE && TableRows >= 1)
    {
		pfc_memorytype Columns[256] = { 0 };
		char * ColumnNames[256] = { 0 };

		Result = MemoryConfig_LoadConfig_TableColumns(valuesNode, &Columns, &ColumnNames[0], 256);

		if(Result == PFC_ERROR_NONE &&
				(MemoryTable = PFC_Memory_NewTable(Memory, RegisterID, Columns[0], Columns[1], TableRows, Name, ColumnNames[0], ColumnNames[1])) != NULL	)
		{
			PFC_MemoryRegister * MemoryRegister = PFC_MemoryTable_GetRegister(MemoryTable);

			if(FCPOffset >= 0)
				PFC_MemoryRegister_SetFCPOffset(MemoryRegister, FCPOffset);

			RegisterSize = PFC_MemoryRegister_Malloc(MemoryRegister);

			if(XML_GetChild(root, XML_DEFAULTVALUE) != NULL)
			{
				if(RegisterSize > 0)
				{
					uint8_t * buffer = (uint8_t *)PFC_malloc(RegisterSize);

					if(XML_GetChildValueAsHexArray(root, XML_DEFAULTVALUE, buffer, RegisterSize) == PFC_ERROR_NONE)
					{
						void * dest = PFC_Memory_GetMemoryRegisterPointer(Memory, RegisterID);

						if(dest != NULL)
						{
							memcpy(dest, buffer, RegisterSize);
						}
					}
				}
			}

			if(XML_GetChild(root, XML_FCPREORDER) != NULL && FCPOffset >= 0)
			{
				uint8_t * PFCReorder = (uint8_t *)PFC_malloc(RegisterSize);

				if(PFCReorder)
				{
					if(XML_GetChildValueAsHexArray(root, XML_FCPREORDER, PFCReorder, RegisterSize) == PFC_ERROR_NONE)
					{
						PFC_MemoryRegister_SetFCPReorder(MemoryRegister, PFCReorder, RegisterSize);
					}
				}

				PFC_free(PFCReorder);
			}

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
                int FCPOffset =  XML_GetChild(child, XML_FCPOFFSET) != NULL ? XML_GetChildValueAsRawInt(child, XML_FCPOFFSET) : -1;

                if(XML_NodeNameIs(child, XML_PFC_MEMORY_REGISTER))
                {
                    Result = MemoryConfig_LoadConfig_MemoryRegister(MemoryConfig->Memory, child, FCPOffset);
                }
                else if(XML_NodeNameIs(child, XML_PFC_MEMORY_TABLE))
                {
                    Result = MemoryConfig_LoadConfig_MemoryTable(MemoryConfig->Memory, child, FCPOffset);
                }
                else if (XML_NodeNameIs(child, XML_PFC_MEMORY_MAP))
                {
                    Result = MemoryConfig_LoadConfig_MemoryMap(MemoryConfig->Memory, child, FCPOffset);
                }
                else
                {
                    Result = PFC_ERROR_XML;
                }

                if(Result != PFC_ERROR_NONE)
                {
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

pfc_error PFC_MemoryConfig_LoadConfigString(PFC_MemoryConfig * MemoryConfig, const char * string, uint32_t length)
{
    pfc_error Result = PFC_ERROR_UNSET;
    TreeNode * root = TreeNode_ParseXML((char *)string, length, true);

    if(root != NULL && XML_NodeNameIs(root, XML_PFC_MEMORY_CONFIG))
    {
        Result = MemoryConfig_LoadConfig_Memory(MemoryConfig, TreeNode_GetChild(root, 0));
    }
    else
    {
        Result = PFC_ERROR_XML;
    }

    if(root != NULL)
    {
        Tree_Delete(root);
    }

    return Result;
}

pfc_error PFC_MemoryConfig_LoadDumpString(PFC_MemoryConfig * MemoryConfig, const char * string, uint32_t length)
{
    pfc_error Result = PFC_ERROR_UNSET;
    MemoryConfig->Memory = PFC_Memory_New();


    if(string != NULL)
    {
        char * token = strtok(string, "\n");

        while(token != NULL)
        {

            int strLen = strlen(token);
            int pos = 0;
            uint8_t * memoryValue = (uint8_t *)calloc(strLen >> 1, sizeof(uint8_t));
            uint8_t memoryLen = 0;

            for(pos = 0; pos < strLen; pos+=2)
            {
                if(pos+1 < strLen)
                {
                    unsigned int value = 0;
                    if(sscanf(&token[pos], "%02x", &value) == 1)
                    {
                        memoryValue[pos >> 1] = value;
                        memoryLen++;
                    }
                }
                else
                {
                    Result = PFC_ERROR_LENGTH;
                    break;
                }
            }


            if(memoryLen == strLen >> 1)
            {
                char valueName[128] = {0};

                if(snprintf(valueName, sizeof(valueName), "0x%0X", memoryValue[0]) > 0)
                {
                    PFC_MemoryRegister * MemoryRegister = PFC_Memory_NewRegister(MemoryConfig->Memory, memoryValue[0], "");

                    PFC_MemoryRegister_AddValueArray(MemoryRegister, PFC_MEMORYTYPE_BYTE, "", memoryLen - 1);

                    if( PFC_MemoryRegister_Malloc(MemoryRegister) == memoryLen - 1)
                    {
                        void * dest = PFC_Memory_GetMemoryRegisterPointer(MemoryConfig->Memory, memoryValue[0]);

                        if(dest != NULL)
                        {
                            memcpy(dest, &memoryValue[1], memoryLen - 1);
                        }
                        else
                        {
                            Result = PFC_ERROR_MEMORY;
                            break;
                        }
                    }
                    else
                    {
                        Result = PFC_ERROR_MEMORY;
                        break;
                    }
                }
                else
                {
                    Result = PFC_ERROR_MEMORY;
                    break;
                }
            }

            free(memoryValue);


            token = strtok(NULL, "\n");
        }


        Result = PFC_ERROR_NONE;
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
                file_buffer = PFC_malloc((uint32_t)file_buffer_size+1);

                if(file_buffer != NULL)
                {
                    if(fread(file_buffer, file_buffer_size, 1, MemoryConfig->FileP) > 0)
                    {
                        if(MemoryConfig->MemoryDump)
                        {
                            Result = PFC_MemoryConfig_LoadDumpString(MemoryConfig, file_buffer, file_buffer_size);
                        }
                        else
                        {
                            Result = PFC_MemoryConfig_LoadConfigString(MemoryConfig, file_buffer, file_buffer_size);
                        }
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
