#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int, long);
  int Pid();
  string User();
  string Command();
  float CpuUtilization();
  float RawRam();
  string Ram();
  long int UpTime();

 private:
  int pid_;
  vector<float> cpuNumbers_;
  long Hertz_;
  float uptime_;
};

#endif