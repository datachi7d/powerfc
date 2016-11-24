/*
 * main.c
 *
 *  Created on: 1/11/2016
 *      Author: sean
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>

#include "serial.h"


#include "PFC_Datastructures.h"

PFC_Data globalData = { 0

//            .UnkownF4 = { .Unkown = 0x04 },
//
//            .TypeString = { .Type = "SR20DET1" },
//            .VersionString = { .Version = "5.03 " },
//            .TunerString = { .Tuner = "        " },
//
//            .SensorStrings = { 0x41, 0x46, 0x2d, 0x31, 0x54, 0x48, 0x52, 0x4f, 0x42, 0x4f, 0x53, 0x54, 0x57, 0x54, 0x52, 0x54, 0x4f, 0x32, 0x2d, 0x31, 0x45, 0x58, 0x54, 0x4d, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x49, 0x47, 0x4e, 0x53, 0x54, 0x52, 0x41, 0x2f, 0x43, 0x50, 0x57, 0x53, 0x4e, 0x54, 0x52, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x45, 0x43, 0x43, 0x57, 0x52, 0x4e, 0x45, 0x58, 0x54, 0x46, 0x2f, 0x50, 0x56, 0x54, 0x43, 0x4f, 0x32, 0x48, 0x41, 0x43, 0x52, 0x46, 0x41, 0x4e },
//
//            .RevIdle = { 0xee1b, 0x7e04, 0xb004, 0x2003, 0x5203, 0x4416 },
//
//            .Ignition = { 0x05, 0x05, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x05, 0x0a, 0x0a, 0x0a, 0x05, 0x05, 0x05, 0x05, 0x05, 0x0a, 0x0a, 0x0a, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x05, 0x0c, 0x0a, 0x08, 0x05, 0x03, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x05, 0x0c, 0x0a, 0x08, 0x05, 0x03, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0f, 0x05, 0x0c, 0x0a, 0x08, 0x05, 0x03, 0x00, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x14, 0x14, 0x14, 0x14, 0x14, 0x0f, 0x0b, 0x05, 0x0d, 0x0b, 0x09, 0x05, 0x03, 0x00, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x18, 0x18, 0x18, 0x18, 0x16, 0x0f, 0x0a, 0x08, 0x0d, 0x0b, 0x09, 0x05, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x18, 0x15, 0x14, 0x0f, 0x0a, 0x0a, 0x0d, 0x0b, 0x09, 0x05, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x16, 0x15, 0x17, 0x15, 0x15, 0x13, 0x12, 0x10, 0x0e, 0x09, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x17, 0x15, 0x17, 0x19, 0x16, 0x13, 0x12, 0x10, 0x0e, 0x07, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x18, 0x16, 0x17, 0x19, 0x18, 0x14, 0x12, 0x10, 0x0e, 0x07, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1a, 0x19, 0x17, 0x17, 0x17, 0x18, 0x15, 0x13, 0x10, 0x06, 0x03, 0x01, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1e, 0x1c, 0x19, 0x16, 0x18, 0x18, 0x16, 0x14, 0x11, 0x07, 0x05, 0x04, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1d, 0x19, 0x18, 0x1a, 0x1a, 0x18, 0x16, 0x13, 0x08, 0x05, 0x04, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1f, 0x1d, 0x1a, 0x18, 0x1a, 0x1a, 0x18, 0x16, 0x13, 0x07, 0x05, 0x04, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1e, 0x1b, 0x1a, 0x1c, 0x1a, 0x18, 0x16, 0x13, 0x09, 0x07, 0x05, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1e, 0x1c, 0x1a, 0x19, 0x15, 0x18, 0x16, 0x13, 0x09, 0x07, 0x06, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1e, 0x1d, 0x19, 0x17, 0x12, 0x12, 0x10, 0x0d, 0x09, 0x06, 0x04, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1e, 0x1d, 0x1c, 0x1a, 0x10, 0x12, 0x10, 0x0d, 0x09, 0x06, 0x04, 0x21, 0x21, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1d, 0x1b, 0x1a, 0x19, 0x17, 0x10, 0x12, 0x10, 0x0d, 0x09, 0x06, 0x04, 0x21, 0x21, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1d, 0x1b, 0x1a, 0x19, 0x17, 0x10, 0x12, 0x10, 0x0d, 0x09, 0x06, 0x04 },
//
//            .InjectorCorrection = { 0x80, 0x80, 0x80, 0x87, 0x86, 0x86, 0x87, 0x8a, 0x90, 0x9c, 0x9b, 0xa1, 0xa3, 0x9e, 0xa8, 0xa1, 0x81, 0x81, 0xab, 0xab, 0x8c, 0x8c, 0x8c, 0x85, 0x82, 0x82, 0x82, 0x82, 0x7d, 0x7d, 0x79, 0x97, 0x99, 0x94, 0x9e, 0x97, 0x77, 0x77, 0xa1, 0xa1, 0x87, 0x87, 0x82, 0x85, 0x85, 0x82, 0x82, 0x80, 0x7e, 0x7d, 0x94, 0x97, 0x97, 0x92, 0x9e, 0x97, 0x77, 0x77, 0xa1, 0xa1, 0x8c, 0x8c, 0x85, 0x85, 0x85, 0x85, 0x85, 0x7e, 0x83, 0x8a, 0x9a, 0xcd, 0xeb, 0xe6, 0xf2, 0xeb, 0xcb, 0xcb, 0xbf, 0xa1, 0x87, 0x87, 0x85, 0x82, 0x85, 0x85, 0x82, 0x89, 0x85, 0x8b, 0x9a, 0xca, 0xef, 0xed, 0xfc, 0xfd, 0xdd, 0xdd, 0xd1, 0xb3, 0x85, 0x85, 0x82, 0x85, 0x85, 0x82, 0x7b, 0x76, 0x85, 0x87, 0x9e, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xdb, 0x85, 0x85, 0x80, 0x85, 0x7e, 0x7e, 0x7b, 0x7b, 0x85, 0x85, 0x99, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe5, 0x80, 0x80, 0x7e, 0x7b, 0x79, 0x7b, 0x7b, 0x7b, 0x7b, 0x85, 0x91, 0xe9, 0xfa, 0xec, 0xe0, 0xff, 0xec, 0xf8, 0xff, 0xec, 0x7e, 0x7e, 0x7b, 0x7e, 0x79, 0x79, 0x7b, 0x7b, 0x7e, 0x87, 0x9c, 0xe1, 0xff, 0xef, 0xef, 0xe8, 0xe9, 0xff, 0xff, 0xff, 0x7b, 0x7b, 0x79, 0x7b, 0x79, 0x79, 0x7b, 0x7b, 0x80, 0x80, 0x68, 0xaa, 0xd8, 0xef, 0xf1, 0xea, 0xeb, 0xff, 0xff, 0xff, 0x85, 0x85, 0x7e, 0x7e, 0x79, 0x79, 0x79, 0x8c, 0x97, 0x90, 0x86, 0xbc, 0xe5, 0xea, 0xec, 0xea, 0xeb, 0xff, 0xff, 0xff, 0x85, 0x85, 0x7e, 0x7e, 0x79, 0x79, 0x8a, 0x97, 0x8d, 0x7a, 0x91, 0xc2, 0xe5, 0xed, 0xee, 0xe8, 0xf1, 0xff, 0xff, 0xff, 0x85, 0x85, 0x7e, 0x7b, 0x79, 0x8a, 0x94, 0xb0, 0x9c, 0x84, 0x96, 0xc2, 0xe5, 0xf4, 0xf1, 0xf4, 0xf5, 0xff, 0xff, 0xff, 0x80, 0x80, 0x82, 0x79, 0x85, 0x8c, 0x99, 0xc2, 0xa7, 0x8b, 0x98, 0xc2, 0xe5, 0xf4, 0xf3, 0xf4, 0xf5, 0xff, 0xff, 0xff, 0x85, 0x85, 0x85, 0x8a, 0x8f, 0x97, 0xa8, 0xcf, 0xb6, 0x94, 0xc6, 0xf4, 0xff, 0xf9, 0xfd, 0xf8, 0xe6, 0xff, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xb7, 0x96, 0xc8, 0xf6, 0xff, 0xf3, 0xf3, 0xf3, 0xe0, 0xff, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xb7, 0xbe, 0xc8, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xd5, 0xdc, 0xc8, 0xdc, 0xdc, 0xd5, 0xe1, 0xdf, 0xbf, 0xd5, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xd5, 0xf3, 0xf3, 0xf3, 0xff, 0xf5, 0xfc, 0xf5, 0xd5, 0xd5, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xd5, 0xf3, 0xf3, 0xf3, 0xff, 0xf5, 0xfc, 0xf5, 0xd5, 0xd5, 0xff, 0xff },
//
//            .MapReference = {
//                    .Boost = { 0x0000, 0x8b05, 0xde08, 0x170b, 0x4d0d, 0x850f, 0xbc11, 0x2c16, 0x9b1a, 0x0c1f, 0x7923, 0xe827, 0x572c, 0x3635, 0x143e, 0xf246, 0xd04f, 0xaf58, 0x8d61, 0x6b6a },
//                    .RPM = {   0x2003, 0xb004, 0x4006, 0xd007, 0x6009, 0xf00a, 0x800c, 0x420e, 0xa00f, 0x3011, 0xc012, 0x5014, 0xe015, 0x7017, 0x0019, 0x901a, 0x201c, 0xb01d, 0x401f, 0xd020 }
//            },
//
//
//            .Advanced = {
//                    .RPM = 0x0000,
//                    .PIM = 0x0000,
//                    .PIMVoltage = 0x1400,
//                    .TPSVoltage = 0x0000,
//                    .InjectorFP = 0x0000,
//                    .Injector = 0x0000,
//                    .Ignition = 0x00,
//                    .Dwell = 0x00,
//                    .Boost = 0x00,
//                    .Duty = 0x00,
//                    .Unkown1 = 0x6400,
//                    .WaterTemperature = 0xff,
//                    .AirTemperature = 0xff,
//                    .Knock = 00,
//                    .BatteryVoltage = 0x71,
//                    .Speed = 0x0000,
//                    .Unkown2 = 0x0000,
//                    .O2Sensor1 = 0x01,
//                    .O2Sensor2 = 0x00,
//                    .Unkown3 = 0x0000,
//            },
//
//            .WarningControlFlagsStrings = {
//                .Control = {
//                        "1.Boost cntl kit",
//                        "2.Air-Flow Warn.",
//                        "3.Injector Warn.",
//                        "4.Knock Warning ",
//                        "5.O2 F/B Control", },
//                .Flag = { 0xFF, 0, 0, 0, 0 }
//            },

    };




void CheckSum(uint8_t * buffer, uint8_t length)
{
    uint8_t * ptr = buffer;
    uint8_t * sum = buffer+length;

    *sum = 0xFF;

    for(; ptr < sum; ptr++)
        *sum -= *ptr;


}

uint8_t PFC_CreatePacket(uint8_t ID, uint8_t * data, uint8_t dataLength, uint8_t * buffer)
{
    PFC_Header * header = (PFC_Header *)buffer;

    header->ID = ID;
    header->Length = sizeof(*header);

    if(data != NULL)
    {
        header->Length += dataLength;
        memcpy(buffer + sizeof(*header), data, dataLength);
    }

    CheckSum(buffer, header->Length);

    return header->Length + 1;
}

int PFC_ProcessPacket(uint8_t * ID, uint8_t * data)
{
    int result = 0;
    uint8_t data_buffer[0xff] = {0};
    uint8_t data_size = 0;
    PFC_Header * header = (PFC_Header *)data_buffer;
    uint8_t header_pos = 0;
    uint8_t RecvChecksum = 0;

    header_pos = serial_read((uint8_t *)header, 1);


    if(header->ID != 0xFF && header_pos == 1)
    {
        serial_read((uint8_t *)header + header_pos, sizeof(*header) - header_pos);

        if(header->Length >= sizeof(*header))
        {
            if(header->Length > sizeof(*header))
            {
                data_size = serial_read(&data_buffer[sizeof(*header)], (header->Length - sizeof(*header)));
            }
            else
            {
                data_size = 0;
            }


            if(serial_read(&RecvChecksum, 1) == 1)
            {
                CheckSum(data_buffer, header->Length);

                if(RecvChecksum == data_buffer[header->Length])
                {
                    if(data_size > 0)
                        memcpy(data, &data_buffer[sizeof(*header)], data_size);

                    *ID = header->ID;
                    result = data_size;
                }
                else
                {
                    result = -EIO;
                }
            }
            else
            {
                result = -EIO;
            }
        }
        else
        {
            result = -EIO;
        }
    }
    else
    {
            result = -EIO;
    }

    return result;
}

uint8_t PFC_GetData(PFC_Data * data, uint8_t ID, uint8_t * buffer)
{
    uint8_t packet_size = 0;
    uint8_t * dataPtr = NULL;

    dataPtr = PFC_PointerOfID(data, ID);

    if(dataPtr != NULL)
    {
        packet_size = PFC_CreatePacket(ID, dataPtr, PFC_SizeOfID(ID), buffer);
    }

    return packet_size;
}

uint8_t PFC_SetData(PFC_Data * data, uint8_t ID, uint8_t * dataPtr, uint8_t dataSize)
{
    uint8_t writeSize = 0;
    uint8_t * pfcDataPtr = NULL;

    pfcDataPtr = PFC_PointerOfID(data, ID);

    if(pfcDataPtr != NULL &&  dataPtr != NULL)
    {
        if(PFC_SizeOfID(ID) == dataSize)
        {
            writeSize = dataSize;
            memcpy(pfcDataPtr, dataPtr, dataSize);
        }
    }

    return writeSize;
}


void printHex(uint8_t * buffer, uint8_t len)
{
    uint8_t pos = 0;

    for(; pos < len; pos++)
        printf("%02x ", buffer[pos]);
}

int PFC_Fetch(uint8_t * ID, uint8_t * data)
{
    uint8_t buffer[0x3] = { 0 };
    uint8_t cmd_length = 0;

    cmd_length = PFC_CreatePacket(ID, NULL, 0, buffer);
    serial_write(buffer, cmd_length);

    return PFC_ProcessPacket(ID, data);
}


bool PFC_Init()
{
    char * Version[0xff] = { 0 };

    if (PFC_Fetch(0xf3, Version) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ProcessLine(char * line)
{
    char * ptr = line;
    uint8_t ID = 0;
    uint8_t data[0xff] = { 0 };
    uint8_t dataLength = 0;

    if(sscanf(ptr, "%02x ", &ID) == 1)
    {
        ptr = strchr(line, '=')+2;

        if(ptr != NULL && *ptr != NULL)
        {

            while(*ptr != NULL)
            {
                if (sscanf(ptr, "%02x", &data[dataLength]))
                {
                    dataLength++;
                }
                ptr = strchr(ptr, ' ') + 1;
            }
        }
    }


    if(dataLength > 0)
    {
        if(PFC_SetData(&globalData, ID, data, dataLength))
        {
            if(ID == 0xf0)
            {
                PFC_PrintID(&globalData, ID);
            }
        }
        else
        {
            printf("\n\nID: %02x not set\n\n", ID);
        }
    }

}




int main()
{
    int settingsfd = open("settings", O_RDONLY);
    char line[4096] = {0};
    char * linePtr = line;
    bool done = false;


    while(!done)
    {
        char data = 0;
        int readsize = read(settingsfd, &data, 1);

        if(readsize == 1)
        {

            if(data == '\n')
            {
                ProcessLine(line);

                memset(line, 0, sizeof(line));
                linePtr = line;
            }
            else
            {
                *linePtr = data;
                linePtr++;
            }
        }
        else
        {
            done = true;
        }
    }




    //return 0;


    if (serial_open("/dev/ttyUSB0") < 0)
    {
        printf("could not open serial\n");
        exit(-1);
    }


    do
    {
        uint8_t data[0xff] = {0};
        int dataLength = 0;
        uint8_t ID = 0;

        dataLength = PFC_ProcessPacket(&ID, data);

        if(dataLength == 0)
        {
            dataLength = PFC_GetData(&globalData, ID, data);

            if(dataLength > 0)
            {
                if (serial_write(data, dataLength) != dataLength)
                {
                    perror("Write failed");
                }


                if(ID == 0xf0)
                {
                    PFC_PrintID(&globalData, ID);


                }
                //printf("<<<");
                //printHex(data,dataLength);
                //printf("\n");

                //PFC_PrintID(&globalData, ID);
                printHex(&ID, 1);
                printf (">>\n");

            }
            else
            {
                uint8_t UnkownCMD[] = { 0xff };

                printf("Unable to process read:");
                printHex(&ID, 1);
                printf("\n");

                serial_write(&UnkownCMD, sizeof(UnkownCMD));

                printf(">>>");
                printHex(&UnkownCMD, sizeof(UnkownCMD));
                printf("\n");

            }
        }
        else if (dataLength > 0)
        {
            printf("Unable to process write:");
            printHex(&ID, 1);
            printf("\n");
            printHex(data, dataLength);
            printf("\n");
        }


    }while(1);


//    do
//    {
//
//        uint8_t data_buffer[0xff] = {0};
//        uint8_t data_size = 0;
//        PFC_Header * header = (PFC_Header *)data_buffer;\
//        uint8_t RecvChecksum = 0;
//
//        serial_read((uint8_t *)header, sizeof(*header));
//
//        if(header->Length > sizeof(*header))
//        {
//            data_size = serial_read(&data_buffer[sizeof(*header)], (header->Length - sizeof(*header)));
//        }
//        else if(header->Length < sizeof(*header))
//        {
//            printf("length incorrect\n");
//            exit(-1);
//        }
//
//        if(serial_read(&RecvChecksum, 1) != 1)
//        {
//            printf("no checksum\n");
//            exit(-1);
//        }
//        else
//        {
//            CheckSum(data_buffer, header->Length);
//
//            if(RecvChecksum != data_buffer[header->Length])
//                break;
//        }
//
//        if(header->Length == sizeof(*header))
//        {
//            uint8_t buffer[0xff];
//            uint8_t buffer_len = 0;
//
////            if(header->ID == PFCID_Initialiaztion && !(first_init % 2))
////            {
////                header->ID = PFCID_InitialiaztionResponse;
////                first_init++;
////            }
////            else
////            {
////                first_init++;
////            }
//
//            buffer_len = PFC_GetData(header->ID, buffer);
//
//            if(buffer_len > 0)
//            {
//                printf(">>>");
//                printHex(data_buffer, header->Length+1);
//                printf("\n");
//
//                usleep(10*1000);
//                serial_write(buffer, buffer_len);
//
//                printf("<<<");
//                printHex(buffer,buffer_len);
//                printf("\n");
//            }
//            else
//            {
//                uint8_t UnkownCMD[] = { 0xff };
//
//                printf("Unable to process read:");
//                printHex(data_buffer, 1);
//                printf("\n");
//
//                serial_write(&UnkownCMD, sizeof(UnkownCMD));
//
//                printf(">>>");
//                printHex(&UnkownCMD, sizeof(UnkownCMD));
//                printf("\n");
//
//            }
//
//        }
//        else
//        {
//
//            if(PFC_SetData(header->ID, &data_buffer[sizeof(*header)], data_size) == data_size)
//            {
//                uint8_t buffer[sizeof(*header)+1];
//                uint8_t buffer_len = 0;
//
//                printf(">>>");
//                printHex(data_buffer, header->Length+1);
//                printf("\n");
//
//                buffer_len = PFC_CreatePacket(PFCID_Acknowledged, NULL, 0, buffer);
//                usleep(10*1000);
//                serial_write(buffer, buffer_len);
//
//                printf("<<<");
//                printHex(buffer,buffer_len);
//                printf("\n");
//            }
//            else
//            {
//                printf("Unable to process write:");
//                printHex(data_buffer, header->Length+1);
//                printf("\n");
//            }
//        }
//        fflush(stdout);
//
//    }while(1);

//    int index = 1;
//    for(; index < (sizeof(data)/sizeof(PFC_Data)); index++)

//    int init_failed = 0;
//    int i = 0x80;
//
//    while(i < 0xff)
//    {
//        if (serial_open("/dev/ttyUSB0") < 0)
//        {
//            printf("could not open serial\n");
//            exit(-1);
//        }
//
//            if(PFC_Init())
//            {
//
//                init_failed = 0;
//                uint8_t buffer[0xff] = { 0 };
//                uint8_t cmd_length = 0;
//                int data_length = 0;
//                //        PFCID ID = data[index].ID;
//
//                PFCID ID = i;
//
//                cmd_length = PFC_CreatePacket(ID, NULL, 0, buffer);
//                //        printf(">>>");
//                //        printHex(buffer,cmd_length);
//                //        printf("\n");
//                serial_write(buffer, cmd_length);
//
//                data_length = PFC_ProcessPacket(buffer);
//
//                if(data_length > 0)
//                {
//                    uint8_t tempbuffer[0xff];
//                    if(PFC_GetData(ID, tempbuffer) == 0)
//                    {
//                        printf("%02x = ", ID);
//                        printHex(buffer, data_length);
//                        printf("\n");
//                    }
//                }
//
//                i++;
//            }
//            else
//            {
//                if(init_failed > 20)
//                {
//                    printf("Init failed!\n");
//                    exit(0);
//                }
//                else
//                {
//                    sleep(1);
//                    init_failed++;
//                }
//            }
//
//        serial_close();
//    }



}



//#include "PFC_Datastructures.h"
//
//int main()
//{
//    PFC_Data test = {
//
//            .RevIdle = { 0xee1b, 0x7e04, 0xb004, 0x2003, 0x5203, 0x4416 },
//
//
//            .Ignition = { 0x05, 0x05, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x05, 0x0a, 0x0a, 0x0a, 0x05, 0x05, 0x05, 0x05, 0x05, 0x0a, 0x0a, 0x0a, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x05, 0x0c, 0x0a, 0x08, 0x05, 0x03, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x05, 0x0c, 0x0a, 0x08, 0x05, 0x03, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0f, 0x05, 0x0c, 0x0a, 0x08, 0x05, 0x03, 0x00, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x14, 0x14, 0x14, 0x14, 0x14, 0x0f, 0x0b, 0x05, 0x0d, 0x0b, 0x09, 0x05, 0x03, 0x00, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x18, 0x18, 0x18, 0x18, 0x16, 0x0f, 0x0a, 0x08, 0x0d, 0x0b, 0x09, 0x05, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x18, 0x15, 0x14, 0x0f, 0x0a, 0x0a, 0x0d, 0x0b, 0x09, 0x05, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x16, 0x15, 0x17, 0x15, 0x15, 0x13, 0x12, 0x10, 0x0e, 0x09, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x17, 0x15, 0x17, 0x19, 0x16, 0x13, 0x12, 0x10, 0x0e, 0x07, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x18, 0x16, 0x17, 0x19, 0x18, 0x14, 0x12, 0x10, 0x0e, 0x07, 0x03, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1a, 0x19, 0x17, 0x17, 0x17, 0x18, 0x15, 0x13, 0x10, 0x06, 0x03, 0x01, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1e, 0x1c, 0x19, 0x16, 0x18, 0x18, 0x16, 0x14, 0x11, 0x07, 0x05, 0x04, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1d, 0x19, 0x18, 0x1a, 0x1a, 0x18, 0x16, 0x13, 0x08, 0x05, 0x04, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1f, 0x1d, 0x1a, 0x18, 0x1a, 0x1a, 0x18, 0x16, 0x13, 0x07, 0x05, 0x04, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1e, 0x1b, 0x1a, 0x1c, 0x1a, 0x18, 0x16, 0x13, 0x09, 0x07, 0x05, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1e, 0x1c, 0x1a, 0x19, 0x15, 0x18, 0x16, 0x13, 0x09, 0x07, 0x06, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1e, 0x1d, 0x19, 0x17, 0x12, 0x12, 0x10, 0x0d, 0x09, 0x06, 0x04, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x1e, 0x1d, 0x1c, 0x1a, 0x10, 0x12, 0x10, 0x0d, 0x09, 0x06, 0x04, 0x21, 0x21, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1d, 0x1b, 0x1a, 0x19, 0x17, 0x10, 0x12, 0x10, 0x0d, 0x09, 0x06, 0x04, 0x21, 0x21, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1d, 0x1b, 0x1a, 0x19, 0x17, 0x10, 0x12, 0x10, 0x0d, 0x09, 0x06, 0x04 },
//
//            .InjectorCorrection = { 0x80, 0x80, 0x80, 0x87, 0x86, 0x86, 0x87, 0x8a, 0x90, 0x9c, 0x9b, 0xa1, 0xa3, 0x9e, 0xa8, 0xa1, 0x81, 0x81, 0xab, 0xab, 0x8c, 0x8c, 0x8c, 0x85, 0x82, 0x82, 0x82, 0x82, 0x7d, 0x7d, 0x79, 0x97, 0x99, 0x94, 0x9e, 0x97, 0x77, 0x77, 0xa1, 0xa1, 0x87, 0x87, 0x82, 0x85, 0x85, 0x82, 0x82, 0x80, 0x7e, 0x7d, 0x94, 0x97, 0x97, 0x92, 0x9e, 0x97, 0x77, 0x77, 0xa1, 0xa1, 0x8c, 0x8c, 0x85, 0x85, 0x85, 0x85, 0x85, 0x7e, 0x83, 0x8a, 0x9a, 0xcd, 0xeb, 0xe6, 0xf2, 0xeb, 0xcb, 0xcb, 0xbf, 0xa1, 0x87, 0x87, 0x85, 0x82, 0x85, 0x85, 0x82, 0x89, 0x85, 0x8b, 0x9a, 0xca, 0xef, 0xed, 0xfc, 0xfd, 0xdd, 0xdd, 0xd1, 0xb3, 0x85, 0x85, 0x82, 0x85, 0x85, 0x82, 0x7b, 0x76, 0x85, 0x87, 0x9e, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xdb, 0x85, 0x85, 0x80, 0x85, 0x7e, 0x7e, 0x7b, 0x7b, 0x85, 0x85, 0x99, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe5, 0x80, 0x80, 0x7e, 0x7b, 0x79, 0x7b, 0x7b, 0x7b, 0x7b, 0x85, 0x91, 0xe9, 0xfa, 0xec, 0xe0, 0xff, 0xec, 0xf8, 0xff, 0xec, 0x7e, 0x7e, 0x7b, 0x7e, 0x79, 0x79, 0x7b, 0x7b, 0x7e, 0x87, 0x9c, 0xe1, 0xff, 0xef, 0xef, 0xe8, 0xe9, 0xff, 0xff, 0xff, 0x7b, 0x7b, 0x79, 0x7b, 0x79, 0x79, 0x7b, 0x7b, 0x80, 0x80, 0x68, 0xaa, 0xd8, 0xef, 0xf1, 0xea, 0xeb, 0xff, 0xff, 0xff, 0x85, 0x85, 0x7e, 0x7e, 0x79, 0x79, 0x79, 0x8c, 0x97, 0x90, 0x86, 0xbc, 0xe5, 0xea, 0xec, 0xea, 0xeb, 0xff, 0xff, 0xff, 0x85, 0x85, 0x7e, 0x7e, 0x79, 0x79, 0x8a, 0x97, 0x8d, 0x7a, 0x91, 0xc2, 0xe5, 0xed, 0xee, 0xe8, 0xf1, 0xff, 0xff, 0xff, 0x85, 0x85, 0x7e, 0x7b, 0x79, 0x8a, 0x94, 0xb0, 0x9c, 0x84, 0x96, 0xc2, 0xe5, 0xf4, 0xf1, 0xf4, 0xf5, 0xff, 0xff, 0xff, 0x80, 0x80, 0x82, 0x79, 0x85, 0x8c, 0x99, 0xc2, 0xa7, 0x8b, 0x98, 0xc2, 0xe5, 0xf4, 0xf3, 0xf4, 0xf5, 0xff, 0xff, 0xff, 0x85, 0x85, 0x85, 0x8a, 0x8f, 0x97, 0xa8, 0xcf, 0xb6, 0x94, 0xc6, 0xf4, 0xff, 0xf9, 0xfd, 0xf8, 0xe6, 0xff, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xb7, 0x96, 0xc8, 0xf6, 0xff, 0xf3, 0xf3, 0xf3, 0xe0, 0xff, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xb7, 0xbe, 0xc8, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xd5, 0xdc, 0xc8, 0xdc, 0xdc, 0xd5, 0xe1, 0xdf, 0xbf, 0xd5, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xd5, 0xf3, 0xf3, 0xf3, 0xff, 0xf5, 0xfc, 0xf5, 0xd5, 0xd5, 0xff, 0xff, 0x85, 0x85, 0x8a, 0x97, 0xa8, 0xb5, 0xc0, 0xcf, 0xd5, 0xf3, 0xf3, 0xf3, 0xff, 0xf5, 0xfc, 0xf5, 0xd5, 0xd5, 0xff, 0xff },
//
//            .MapReference = {
//                    .Boost = { 0x0000, 0x8b05, 0xde08, 0x170b, 0x4d0d, 0x850f, 0xbc11, 0x2c16, 0x9b1a, 0x0c1f, 0x7923, 0xe827, 0x572c, 0x3635, 0x143e, 0xf246, 0xd04f, 0xaf58, 0x8d61, 0x6b6a },
//                    .RPM = {   0x2003, 0xb004, 0x4006, 0xd007, 0x6009, 0xf00a, 0x800c, 0x420e, 0xa00f, 0x3011, 0xc012, 0x5014, 0xe015, 0x7017, 0x0019, 0x901a, 0x201c, 0xb01d, 0x401f, 0xd020 }
//            },
//
//
//            .WarningControlFlagsStrings = {
//
//    .Control = {
//            "1.Boost cntl kit",
//            "2.Air-Flow Warn.",
//            "3.Injector Warn.",
//            "4.Knock Warning ",
//            "5.O2 F/B Control",
//    },
//
//    .Flag = { 0xFF, 0, 0, 0, 0 }
//
//    },
//
//    };
//
//    PFC_PrintID(&test, 0x86);
//
//}



