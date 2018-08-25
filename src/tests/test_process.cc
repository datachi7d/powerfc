#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <signal.h>


#include <fstream>
#include <iostream>

#include "process.h"

#include "pfc_process.h"

namespace PFC
{

/*
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
*/

	class PFC_ProcessTest : public testing::Test
	{
		pid_t _pidClient;
		pid_t _pidServer;
        std::string TestServerSerialPath;
		std::string TestClientSerialPath;

protected:
        std::string ServerSerialPath;
        std::string ClientSerialPath;
		std::fstream ClientSerialStream;
		std::fstream ServerSerialStream;

		PFC_ProcessTest(): _pidClient(-1), _pidServer(-1),
		        TestServerSerialPath("/tmp/PFCTestServerSerial"),
		        TestClientSerialPath("/tmp/PFCTestClientSerial"),
		        ServerSerialPath("/tmp/PFCServerSerial"),
		        ClientSerialPath("/tmp/PFCClientSerial")
		        {}

		pid_t Setup_serial(std::fstream &SerialStream, const char * serial1, const char * serial2)
		{

		    pid_t _pid;

            char pty1[256] = {0};
            sprintf(pty1, "pty,raw,echo=0,link=%s", serial1);

            char pty2[256] = {0};
            sprintf(pty2, "pty,raw,echo=0,link=%s", serial2);

            std::vector<const char *> commandVector { "/usr/bin/socat", "-d -d -d -d", pty1, pty2 };
            _pid = SpawnProcess(commandVector, false, false);

            int counter = 0;

            while ((!SerialStream.is_open()) && counter < 10)
            {
                SerialStream.open(TestClientSerialPath.c_str(), std::ios::in | std::ios::out | std::ios::binary);
                usleep(5000);
                counter++;
            }

            return _pid;
		}


		void SetUp()
		{
		    _pidClient = Setup_serial(ClientSerialStream, TestClientSerialPath.c_str(), ClientSerialPath.c_str());

			ASSERT_TRUE(ClientSerialStream);

            _pidServer = Setup_serial(ServerSerialStream, TestServerSerialPath.c_str(), ServerSerialPath.c_str());

            ASSERT_TRUE(ServerSerialStream);
		}
		void TearDown()
		{
			TerminateProcess(_pidClient);
			TerminateProcess(_pidServer);
		}
	};


	TEST_F(PFC_ProcessTest, test_Process_new_free)
	{
	    PFC_Process * process = PFC_Process_NewFromConfig("src/tests/test_memory_config.xml");

	    ASSERT_TRUE(process != NULL);

	    ASSERT_EQ(PFC_Process_AddClient(process, ClientSerialPath.c_str()), PFC_ERROR_NONE);

	    ASSERT_EQ(PFC_Process_SetServer(process, ServerSerialPath.c_str()), PFC_ERROR_NONE);

	    PFC_Process_Free(process);
	}

	/*
	TEST_F(PFC_Serial, test_Serial_NewFree)
	{
		Serial * serial = Serial_New("/tmp/PFCSerial");

		ASSERT_TRUE(serial != NULL);

		Serial_Free(serial);
	}


	TEST_F(PFC_Serial, test_Serial_Write)
	{
		Serial * serial = Serial_New("/tmp/PFCSerial");

		EXPECT_TRUE(serial != NULL);

		uint8_t writeData[] = {0x01, 0x02, 0x03, 0x13, 0x11, 0xe0};
		char testReadData[sizeof(writeData)] = {0};

		ASSERT_EQ(Serial_Write(serial, writeData, sizeof(writeData)), sizeof(writeData));

		ASSERT_USECS(SerialStream.read(testReadData, sizeof(testReadData)), 100000);

		ASSERT_TRUE(memcmp(writeData, testReadData, sizeof(writeData)) == 0);

		Serial_Free(serial);
	}

	TEST_F(PFC_Serial, test_Serial_Read)
	{
		Serial * serial = Serial_New("/tmp/PFCSerial");

		EXPECT_TRUE(serial != NULL);

		char writeData[] = {0x01, 0x02, 0x03, 0x13, 0x11, -100};
		uint8_t testReadData[sizeof(writeData)] = {0};

		SerialStream.write(writeData, sizeof(writeData));
		SerialStream.flush();

		ASSERT_EQ(Serial_Read(serial, testReadData, sizeof(testReadData)), sizeof(testReadData));

		ASSERT_TRUE(memcmp(writeData, testReadData, sizeof(writeData)) == 0);

		Serial_Free(serial);
	}

	TEST_F(PFC_Serial, test_Serial_Read_Timeout)
	{
		Serial * serial = Serial_New("/tmp/PFCSerial");

		EXPECT_TRUE(serial != NULL);

		char writeData[] = {0x01, 0x02, 0x03, 0x13, 0x11, -100};
		uint8_t testReadData[sizeof(writeData)] = {0};

		SerialStream.write(writeData, sizeof(writeData) - 1);
		SerialStream.flush();

		ASSERT_EQ(Serial_Read(serial, testReadData, sizeof(testReadData)), 0);

		ASSERT_TRUE(memcmp(writeData, testReadData, sizeof(writeData)) != 0);

		Serial_Free(serial);
	}

	TEST_F(PFC_Serial, test_Serial_ReadMessage)
	{
		Serial * serial = Serial_New("/tmp/PFCSerial");

		EXPECT_TRUE(serial != NULL);

		char writeData[] = {0xf6, 0x03, 0x00, 0x06};
		uint8_t testReadData[255] = {0};

		SerialStream.write(writeData, sizeof(writeData));
		SerialStream.flush();

		PFC_ID ID = 0;
		pfc_size size = 0;

		ASSERT_EQ(Serial_ReadPFCMessage(serial, &ID, testReadData, &size), PFC_ERROR_NONE);

		ASSERT_EQ(size, 1);
		ASSERT_EQ(ID, 0xf6);
		ASSERT_EQ(testReadData[0], 0);

		Serial_Free(serial);
	}

	TEST_F(PFC_Serial, test_Serial_WriteMessage)
	{
		Serial * serial = Serial_New("/tmp/PFCSerial");

		EXPECT_TRUE(serial != NULL);

		char expectedData[] = {0xf6, 0x03, 0x00, 0x06};
		char testReadData[255] = {0};

		PFC_ID ID = 0xf6;
		pfc_size size = 1;
		uint8_t writeData = 0;

		ASSERT_EQ(Serial_WritePFCMessage(serial, ID, &writeData, size), PFC_ERROR_NONE);

		ASSERT_USECS(SerialStream.read(testReadData, sizeof(expectedData)), 100000);

		ASSERT_TRUE(memcmp(expectedData, testReadData, sizeof(expectedData)) == 0);

		Serial_Free(serial);
	}
	*/

}
