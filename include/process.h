#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  
  int pid_;
  string user_;
  string command_;
  float cpuUtilization_;
  string ram_;
  long int upTime_;
  
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  
  // DONE: Declare any necessary private members
 private:
  
};

#endif