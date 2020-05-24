#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
  user_ =    LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
  cpuUtilization_ = Process::CpuUtilization();
  ram_=   LinuxParser::Ram(pid_);
  upTime_= LinuxParser::UpTime(pid_);
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
  string line;
  string value;
  vector<string> vect;
  
  std::ifstream filestream("/proc/" + to_string(pid_) + "/stat");
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      vect.push_back(value);
    }
  }
  float totalTime = std::stof(vect[15]) + std::stof(vect[16]) + std::stof(vect[17]) + std::stof(vect[18]);
  totalTime = totalTime / sysconf(_SC_CLK_TCK);
  return totalTime / LinuxParser::UpTime(pid_);
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return ram_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return (this->cpuUtilization_ > a.cpuUtilization_);
}