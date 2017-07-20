#ifndef SRC_TESTS_SUPPORT_PROCESS_H_
#define SRC_TESTS_SUPPORT_PROCESS_H_

#include <vector>

namespace PFC
{

	pid_t SpawnProcess(const std::vector<const char *> &commandVector, bool wait, bool silent);
	void TerminateProcess(pid_t pid);

} //namespace PFC

#endif /* SRC_TESTS_SUPPORT_PROCESS_H_ */
