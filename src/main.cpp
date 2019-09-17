#include <iostream>
#include "ncurses_display.h"
#include "system.h"
#include "process.h"



int main() {
  System system;

  vector<Process>& stats = system.Processes().GetProcesses();

  for (size_t i = 0; i < stats.size(); i++)
  {
    auto stuff = stats[i].CpuUtilization();
    std::cout << "\n" << stuff;
  }
  

  // NCursesDisplay::Display(system, 20);
}