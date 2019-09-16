#include "processor.h"
#include <sstream>
#include <string>
#include <vector>

#include "parser_consts.h"
#include "parser_helper.h"

using std::string;
using std::vector;

float Processor::Utilization() {
  vector<float> values = ReadFile();
  float user = values[0];
  float nice = values[1];
  float system = values[2];
  float idle = values[3];
  float iowait = values[4];
  float irq = values[5];
  float softirq = values[6];
  float steal = values[7];

  float PrevIdle = previdle + previowait;
  float Idle = idle + iowait;

  float PrevNonIdle =
      prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  float NonIdle = user + nice + system + irq + softirq + steal;

  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;

  float totald = Total - PrevTotal;

  float idled = Idle - PrevIdle;

  float CPU_Percentage = (totald - idled) / totald;

  AssignPrevValues(values);
  return CPU_Percentage;
}

void Processor::AssignPrevValues(vector<float> newValues) {
  prevuser = newValues[0];
  prevnice = newValues[1];
  prevsystem = newValues[2];
  previdle = newValues[3];
  previowait = newValues[4];
  previrq = newValues[5];
  prevsoftirq = newValues[6];
  prevsteal = newValues[7];
}

vector<float> Processor::ReadFile() {
  string line, key;
  float value;
  vector<float> cpuNumbers;
  std::ifstream stream(ParserConsts::kProcDirectory +
                       ParserConsts::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == ParserConsts::filterCpu) {
          while (linestream >> value) {
            cpuNumbers.push_back(value);
          }
        }
      }
    }
  }
  return cpuNumbers;
}