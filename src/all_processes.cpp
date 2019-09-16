#include "all_processes.h"
#include "linux_parser.h"
#include "process.h"

#include <vector>
#include <unistd.h>   

using std::vector;

All_Processes::All_Processes() {
  Hertz = sysconf(_SC_CLK_TCK);
}

vector<int> All_Processes::ReadFolders() { return LinuxParser::Pids(); };

vector<Process> All_Processes::GetProcesses() {
  vector<int> allPids = ReadFolders();

  vector<Process> allProcesses;

  for (std::size_t i = 0; i < allPids.size(); ++i) {
    Process process(allPids[i], Hertz);

    allProcesses.push_back(process);
  }

  return allProcesses;
}