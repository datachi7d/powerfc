#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <signal.h>


#include <fstream>
#include <iostream>

#include "process.h"

#include "serial.h"

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


	class PFC_Serial : public testing::Test
	{
		pid_t _pid;
		std::string TestSerialPath;
		std::string SerialPath;
protected:

		std::fstream SerialStream;

		PFC_Serial(): _pid(-1), TestSerialPath("/tmp/PFCTestSerial"), SerialPath("/tmp/PFCSerial") {}

		void SetUp()
		{
			char pty1[256] = {0};
			sprintf(pty1, "pty,raw,echo=0,link=%s", TestSerialPath.c_str());

			char pty2[256] = {0};
			sprintf(pty2, "pty,raw,echo=0,link=%s", SerialPath.c_str());

			std::vector<const char *> commandVector { "/usr/bin/socat", "-d -d -d -d", pty1, pty2 };
			_pid = SpawnProcess(commandVector, false, false);

			int counter = 0;


			while ((!SerialStream.is_open()) && counter < 10)
			{
				SerialStream.open(TestSerialPath.c_str(), std::ios::in | std::ios::out | std::ios::binary);
				usleep(5000);
				counter++;
			}

			ASSERT_TRUE(SerialStream);
		}
		void TearDown()
		{
			TerminateProcess(_pid);
		}
	};


	TEST_F(PFC_Serial, test_Serial_NewFree_no_path)
	{
		Serial * serial = Serial_New("");

		ASSERT_TRUE(serial == NULL);

		Serial_Free(serial);
	}

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

		ASSERT_TRUE(memcmp(expectedData, testReadData, sizeof(expectedData)) != 0);

		Serial_Free(serial);
	}

}
