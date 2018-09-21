#include <iostream>
#include <string>
#include <cstring>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "process.h"

namespace PFC
{

	pid_t SpawnProcess(const std::vector<const char *> &commandVector, bool wait_, bool silent)
	{
		pid_t c_pid, pid;

		std::cerr << "SpawnProcess: " << commandVector.size() << std::endl;
		for (auto it = commandVector.begin(); it != commandVector.end(); ++it)
		{
		  std::cerr << *it << std::endl;
		}

		c_pid = fork();

		if (c_pid == 0)
		{
			// child
			std::cout << "Child: executing " << commandVector[0] << std::endl;

			FILE * fd = NULL;
			if (silent)
			{
				// redirect stdout to /dev/null
				fd = fopen("/dev/null", "at");
				dup2(fileno(fd), STDOUT_FILENO);
			}

			dup2(STDOUT_FILENO, STDERR_FILENO);

			// convert the const char * vector to a char * array, required by execvp
			std::vector<char *> argv(commandVector.size() + 1);    // one extra for the null
			for (std::size_t i = 0; i != commandVector.size(); ++i)
			{
				argv[i] = strdup(&commandVector[i][0]);
			}

			// pass the vector's internal array to execvp - standard guarantees vector elements are stored contiguously
			execvp(argv[0], argv.data());

			// only get here if exec failed
			perror("execve");
			std::cerr << argv[0] << std::endl;
			if (fd != NULL)
			{
				fclose(fd);
			}
			_exit(1);
		}
		else if (c_pid > 0)
		{
			// parent
			if (wait_)
			{
				std::cout << "Parent: wait\n" << std::endl;
				int status = 0;
				if ((pid = wait(&status)) < 0)
				{
					perror("wait");
					c_pid = -1;
				}
				std::cout << "Parent: finished" << std::endl;
			}
		}
		else
		{
			perror("fork");
			c_pid = -1;
		}

		return c_pid;
	}


	void TerminateProcess(pid_t pid)
	{
	    //std::cout << "Terminate pid " << pid_ << std::endl;
	    if (pid > 0)
	    {
	        if (kill(pid, SIGTERM))
	        {
	            perror("kill");
	        }

	        waitpid(pid, NULL, 0);
	        pid = 0;
	    }
	}

} //namesapce PFC
