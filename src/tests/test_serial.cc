#include <gtest/gtest.h>
#include <string>
#include <stdio.h>
#include <stdint.h>

#include <fstream>
#include <iostream>

#include "process.h"

#include "serial.h"

namespace PFC
{

	class PFC_Serial : public testing::Test
	{
		pid_t _pid;
		std::string TestSerialPath;
		std::string SerialPath;
		std::fstream SerialStream;

protected:

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

			EXPECT_TRUE(SerialStream);
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

}
