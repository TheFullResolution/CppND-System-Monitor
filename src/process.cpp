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

Process::Process(int pid, long Hertz) : pid_(pid), Hertz_(Hertz) {
  string line, skip;
  float value;
  vector<float> cpuNumbers;
  std::ifstream stream(ParserConsts::kProcDirectory + to_string(pid_) +
                       ParserConsts::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value >> skip >> skip;
    while (linestream >> value) {
      cpuNumbers.push_back(value);
      if(to_string(pid_) == "2536") {
        std::cout << "\n" << value;
      }
    }
  }

  cpuNumbers_ = cpuNumbers;
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  float uptime = LinuxParser::UpTime();

  float utime = cpuNumbers_[10];
  float stime = cpuNumbers_[11];
  float cutime = cpuNumbers_[12];
  float cstime = cpuNumbers_[13];
  float starttime = cpuNumbers_[18];

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

float Process::RawRam() {
  float memInKB = ParserHelper::GetValueByKey<long>(
      ParserConsts::filterProcMem,
      to_string(pid_) + ParserConsts::kStatusFilename);
  return memInKB;
}

string Process::Ram() {
  float memInKB = RawRam();
  return Format::KBisMB(memInKB);
}



string Process::User() {
  int UID = ParserHelper::GetValueByKey<int>(
      ParserConsts::filterUID, to_string(pid_) + ParserConsts::kStatusFilename);

  string user = LinuxParser::UserByUID(UID);
  return user;
}

long int Process::UpTime() {
  long starttime = cpuNumbers_[18];
  long uptime = LinuxParser::UpTime();
  long seconds = uptime - (starttime / Hertz_);

  return seconds;
}
