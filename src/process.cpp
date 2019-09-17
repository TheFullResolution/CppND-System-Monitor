#include <unistd.h>
#include <cctype>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"
#include "parser_consts.h"
#include "parser_helper.h"
#include "process.h"

using namespace std;

Process::Process(int pid, long Hertz) : pid_(pid), Hertz_(Hertz) {

      // std::cout << "\n pid-  " << pid_;
  vector<string> cpuNumbers = ReadFile(pid);
      // std::cout << "\n cpuNum" << cpuNumbers.size();

  utime_ = stof(cpuNumbers[13]);
  stime_ = stof(cpuNumbers[14]);
  cutime_ = stof(cpuNumbers[15]);
  cstime_ = stof(cpuNumbers[16]);
  starttime_ = stof(cpuNumbers[21]);
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  float uptime = LinuxParser::UpTime();

  float total_time = utime_ + stime_ + cutime_ + cstime_;

  float seconds = uptime - (starttime_ / Hertz_);
  float cpu_usage = (total_time / Hertz_) / seconds;

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
  long uptime = LinuxParser::UpTime();
  long seconds = uptime - (starttime_ / Hertz_);

  return seconds;
}

vector<string> Process::ReadFile(int pid) {
  string line, skip;

  std::ifstream stream(ParserConsts::kProcDirectory + to_string(pid) +
                       ParserConsts::kStatFilename);

  getline(stream, line);
  istringstream linestream(line);
  istream_iterator<string> beg(linestream), end;
  vector<string> cpuNumbers(beg, end);
  return cpuNumbers;
};