#include "all_processes.h"
#include "linux_parser.h"
#include "process.h"

#include <unistd.h>
#include <vector>

using std::sort;
using std::vector;

bool compareProcesses(Process& p1, Process& p2) {
  return (p1.RawRam() > p2.RawRam());
};

All_Processes::All_Processes() {
  Hertz = sysconf(_SC_CLK_TCK);
  CreateProcesses();
}

vector<int> All_Processes::ReadFolders() { return LinuxParser::Pids(); };

void All_Processes::CreateProcesses() {
  vector<int> allPids = ReadFolders();

  vector<Process> allProcesses;

  for (std::size_t i = 0; i < allPids.size(); ++i) {
    Process process(allPids[i], Hertz);

    allProcesses.push_back(process);
  }
  sort(allProcesses.begin(), allProcesses.end(), compareProcesses);
  all_processes_ = allProcesses;
}

vector<Process>& All_Processes::GetProcesses() {
  CreateProcesses();
  return all_processes_;
}