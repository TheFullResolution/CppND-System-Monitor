#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"
#include "parser_consts.h"
#include "parser_helper.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, long& Hertz) : pid_(pid), Hertz_(Hertz) {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  string line, skip;
  float value;
  vector<float> cpuNumbers;
  float uptime = LinuxParser::UpTime();

  std::ifstream stream(ParserConsts::kProcDirectory + to_string(pid_) +
                       ParserConsts::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value >> skip >> skip;
    while (linestream >> value) {
      cpuNumbers.push_back(value);
    }
  }

  float utime = cpuNumbers[10];
  float stime = cpuNumbers[11];
  float cutime = cpuNumbers[12];
  float cstime = cpuNumbers[13];
  float starttime = cpuNumbers[18];

  float total_time = utime + stime + cutime + cstime;

  float seconds = uptime - (starttime / Hertz_);
  float cpu_usage = ((total_time / Hertz_) / seconds);

  return cpu_usage;
}

string Process::Command() {
  string cmd = ParserHelper::GetValue<string>(to_string(pid_) +
                                              ParserConsts::kCmdlineFilename);
  return cmd;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  float memInKB = ParserHelper::GetValueByKey<long>(
      ParserConsts::filterProcMem,
      to_string(pid_) + ParserConsts::kStatusFilename);

  return Format::KBisMB(memInKB);;
}

string Process::User() {
  int UID = ParserHelper::GetValueByKey<int>(
      ParserConsts::filterUID, to_string(pid_) + ParserConsts::kStatusFilename);

  string user = LinuxParser::UserByUID(UID);
  return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}