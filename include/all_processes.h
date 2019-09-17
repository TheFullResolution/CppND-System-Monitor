#ifndef ALL_PROCESSES_H
#define ALL_PROCESSES_H

#include "process.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

class All_Processes {
 private:
  vector<int> ReadFolders();
  vector<Process> all_processes_;
  void CreateProcesses();

 public:
  All_Processes();
  vector<Process>& GetProcesses();
  long Hertz;
};

#endif