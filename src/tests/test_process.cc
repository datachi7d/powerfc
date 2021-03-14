#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <signal.h>


#include <fstream>
#include <iostream>

#include <pthread.h>

#include "process.h"

#include "pfc_process.h"
#include "serial.h"

using namespace std;

namespace PFC
{


static jmp_buf jmp_env;

static void catch_alarm(int sig)
{
    longjmp(jmp_env, 1);
}

#define ASSERT_USECS(fn, usecs) { \
    const auto val = setjmp(jmp_env); \
    if (val == 0) { \
        signal(SIGALRM, catch_alarm); \
        ualarm((usecs), 0); \
        { fn; }; \
        ualarm(0, 0); \
    } else { \
        GTEST_FATAL_FAILURE_(#usecs " usecs timer tripped for " #fn); \
    } }

	class PFC_ProcessTest : public testing::Test
	{
		pid_t _pidClient;
		pid_t _pidServer;
        std::string TestServerSerialPath;
		std::string TestClientSerialPath;

protected:
        std::string ServerSerialPath;
        std::string ClientSerialPath;
		Serial * ClientSerialStream;
		Serial * ServerSerialStream;

		PFC_ProcessTest(): _pidClient(-1), _pidServer(-1),
		        TestServerSerialPath("/tmp/PFCTestServerSerial"),
		        TestClientSerialPath("/tmp/PFCTestClientSerial"),
		        ServerSerialPath("/tmp/PFCServerSerial"),
		        ClientSerialPath("/tmp/PFCClientSerial"),
				ClientSerialStream(NULL),
				ServerSerialStream(NULL)
				{}


		pid_t Setup_serial(Serial ** SerialStream, const char * serial1, const char * serial2)
		{

		    pid_t _pid;

            char pty1[256] = {0};
            sprintf(pty1, "pty,raw,echo=0,link=%s", serial2);

            char pty2[256] = {0};
            sprintf(pty2, "pty,raw,echo=0,link=%s", serial1);

            // For added debug use: "-d", "-d", "-d", "-d",
            std::vector<const char *> commandVector { "/usr/bin/socat", "-D", "-x", "-lu", pty1, pty2 };
            _pid = SpawnProcess(commandVector, false, false);

            int counter = 0;

            while ((*SerialStream == NULL) && counter < 50)
            {
                *SerialStream = Serial_New(serial1);
                usleep(5000);
                counter++;
            }

            return _pid;
		}


		void SetUp()
		{
            _pidServer = Setup_serial(&ServerSerialStream, TestServerSerialPath.c_str(), ServerSerialPath.c_str());

            ASSERT_TRUE(ServerSerialStream != NULL);

		    _pidClient = Setup_serial(&ClientSerialStream, TestClientSerialPath.c_str(), ClientSerialPath.c_str());

			ASSERT_TRUE(ClientSerialStream != NULL);


		}
		void TearDown()
		{
			Serial_Free(ServerSerialStream);
			Serial_Free(ClientSerialStream);
			TerminateProcess(_pidClient);
			TerminateProcess(_pidServer);
		}


	};

	void * do_process(void * attr)
	{
	    PFC_Process * process = (PFC_Process *)attr;

	    printf("do_process: Starting\n");

	    while(PFC_Process_Running(process))
	    {
	        PFC_Process_Run(process);
	    }

	    printf("do_process: Done\n");

	    return NULL;
	}

	TEST_F(PFC_ProcessTest, test_Process_NewFree)
	{
	    PFC_Process * process = PFC_Process_NewFromConfig("test_memory_config.xml");

	    ASSERT_TRUE(process != NULL);

	    ASSERT_EQ(PFC_Process_AddClient(process, ClientSerialPath.c_str()), PFC_ERROR_NONE);
	    ASSERT_EQ(PFC_Process_SetServer(process, ServerSerialPath.c_str()), PFC_ERROR_NONE);

	    PFC_Process_Free(process);
	}

    TEST_F(PFC_ProcessTest, test_Process_Dump_NewFree)
    {
        PFC_Process * process = PFC_Process_NewFromDump("test_memory_dump.dat");

	    ASSERT_TRUE(process != NULL);

	    ASSERT_EQ(PFC_Process_AddClient(process, ClientSerialPath.c_str()), PFC_ERROR_NONE);

	    ASSERT_EQ(PFC_Process_SetServer(process, ServerSerialPath.c_str()), PFC_ERROR_NONE);

        PFC_Process_Free(process);
    }

    TEST_F(PFC_ProcessTest, test_Process_Server_Unexpected_Write)
    {
        PFC_Process * process = PFC_Process_NewFromConfig("test_memory_config.xml");

        ASSERT_TRUE(process != NULL);

        ASSERT_EQ(PFC_Process_AddClient(process, ClientSerialPath.c_str()), PFC_ERROR_NONE);

        ASSERT_EQ(PFC_Process_SetServer(process, ServerSerialPath.c_str()), PFC_ERROR_NONE);

        uint8_t writeData[] = {0xf2, 0x02, 0x0b};

        ASSERT_EQ(Serial_Write(ServerSerialStream,writeData, sizeof(writeData)), (uint8_t)sizeof(writeData));

        pthread_t processThread;
        pthread_create(&processThread, NULL, do_process, (void *)process);

//        char testReadData[255] = {0};
//
//        ASSERT_USECS(ServerSerialStream.read(testReadData, sizeof(writeData)), 100000);
//
//        ASSERT_TRUE(memcmp(writeData, testReadData, sizeof(writeData)) == 0);

        sleep(1);

        PFC_Process_Halt(process);
        pthread_join(processThread, NULL);

        PFC_Process_Free(process);
    }

    TEST_F(PFC_ProcessTest, test_Process_Client_Write)
    {
        PFC_Process * process = PFC_Process_NewFromConfig("test_memory_config.xml");

        ASSERT_TRUE(process != NULL);

        ASSERT_EQ(PFC_Process_AddClient(process, ClientSerialPath.c_str()), PFC_ERROR_NONE);

        ASSERT_EQ(PFC_Process_SetServer(process, ServerSerialPath.c_str()), PFC_ERROR_NONE);

        uint8_t writeData[] = {0xf6, 0x03, 0x00, 0x06};

        ASSERT_EQ(Serial_Write(ClientSerialStream,writeData, sizeof(writeData)), (uint8_t)sizeof(writeData));

        pthread_t processThread;
        pthread_create(&processThread, NULL, do_process, (void *)process);

        uint8_t testReadData[255] = {0};

        ASSERT_USECS(Serial_Read(ServerSerialStream, testReadData, sizeof(writeData)), 100000);

        ASSERT_TRUE(memcmp(writeData, testReadData, sizeof(writeData)) == 0);

        PFC_Process_Halt(process);
        pthread_join(processThread, NULL);

        PFC_Process_Free(process);
    }

    TEST_F(PFC_ProcessTest, test_Process_Client_Write_Partial_Failure)
    {
        PFC_Process * process = PFC_Process_NewFromConfig("test_memory_config.xml");

        ASSERT_TRUE(process != NULL);

        ASSERT_EQ(PFC_Process_AddClient(process, ClientSerialPath.c_str()), PFC_ERROR_NONE);

        ASSERT_EQ(PFC_Process_SetServer(process, ServerSerialPath.c_str()), PFC_ERROR_NONE);

        uint8_t writeData[] = {0xf6, 0x03, 0x00, 0x06};

        pthread_t processThread;
        pthread_create(&processThread, NULL, do_process, (void *)process);

        ASSERT_EQ(Serial_Write(ClientSerialStream,writeData, sizeof(writeData)-1), (uint8_t)sizeof(writeData)-1);
        usleep(110000);
        ASSERT_EQ(Serial_Write(ClientSerialStream,writeData, sizeof(writeData)), (uint8_t)sizeof(writeData));

        uint8_t testReadData[255] = {0};

        ASSERT_USECS(Serial_Read(ServerSerialStream, testReadData, sizeof(writeData)), 100000);

        ASSERT_TRUE(memcmp(writeData, testReadData, sizeof(writeData)) == 0);

        PFC_Process_Halt(process);
        pthread_join(processThread, NULL);

        PFC_Process_Free(process);
    }

    TEST_F(PFC_ProcessTest, test_Process_SimMode_Client_Write)
    {
        PFC_Process * process = PFC_Process_NewFromConfig("test_memory_config.xml");

        ASSERT_TRUE(process != NULL);

        ASSERT_EQ(PFC_Process_AddClient(process, ClientSerialPath.c_str()), PFC_ERROR_NONE);

        uint8_t writeData[] = {0xf6, 0x03, 0x00, 0x06};

        ASSERT_EQ(Serial_Write(ClientSerialStream,writeData, sizeof(writeData)), (uint8_t)sizeof(writeData));

        pthread_t processThread;
        pthread_create(&processThread, NULL, do_process, (void *)process);

        uint8_t testReadData[255] = {0};
        uint8_t expectedData[] = {0xf2, 0x02, 0x0b};

        ASSERT_USECS(Serial_Read(ClientSerialStream, testReadData, sizeof(expectedData)), 100000);
        ASSERT_TRUE(memcmp(expectedData, testReadData, sizeof(expectedData)) == 0);

        PFC_Process_Halt(process);
        pthread_join(processThread, NULL);

        PFC_Process_Free(process);
    }

    TEST_F(PFC_ProcessTest, test_Process_SimMode_Client_Write_Server_Queue)
    {
    	PFC_Process * process = PFC_Process_NewFromConfig("test_memory_config.xml");

        ASSERT_TRUE(process != NULL);

        ASSERT_EQ(PFC_Process_AddClient(process, ClientSerialPath.c_str()), PFC_ERROR_NONE);
        ASSERT_EQ(PFC_Process_SetServer(process, ServerSerialPath.c_str()), PFC_ERROR_NONE);

        uint8_t writeData[] = {0xf6, 0x03, 0x00, 0x06};

        ASSERT_EQ(Serial_Write(ClientSerialStream,writeData, sizeof(writeData)), (uint8_t)sizeof(writeData));

        pthread_t processThread;
        pthread_create(&processThread, NULL, do_process, (void *)process);

        uint8_t testReadData[255] = {0};
        uint8_t expectedData[] = {0xf2, 0x02, 0x0b};

        ASSERT_USECS(Serial_Read(ServerSerialStream, testReadData, sizeof(writeData)), 100000);
        ASSERT_TRUE(memcmp(writeData, testReadData, sizeof(writeData)) == 0);

        ASSERT_EQ(Serial_Write(ServerSerialStream,expectedData, sizeof(expectedData)), (uint8_t)sizeof(expectedData));
        ASSERT_USECS(Serial_Read(ClientSerialStream, testReadData, sizeof(expectedData)), 100000);
        ASSERT_TRUE(memcmp(expectedData, testReadData, sizeof(expectedData)) == 0);

        PFC_Process_DumpValue(process, 0x03);

        PFC_Process_Halt(process);
        pthread_join(processThread, NULL);

        PFC_Process_Free(process);
    }

}
