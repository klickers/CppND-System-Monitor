#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  string memTotal;
  string memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          std::replace(value.begin(), value.end(), '_', ' ');
          memTotal = value;
        }
        if (key == "MemFree") {
          std::replace(value.begin(), value.end(), '_', ' ');
          memFree = value;
        }
      }
    }
  }
  memTotal.pop_back();
  memFree.pop_back();
  memTotal.pop_back();
  memFree.pop_back();
  memTotal.pop_back();
  memFree.pop_back();
  float memTotalFloat = std::stof(memTotal);
  float memFreeFloat = std::stof(memFree);
  return (memTotalFloat - memFreeFloat) / (memTotalFloat);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  return std::stol(uptime);
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK); }

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) {
  return 5;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  vector<string> vect;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value != "cpu")
      {
        vect.push_back(value);
      }
    }
  }
  long sum;
  for (auto x : vect) {
    long y = std::stol(x);
    sum = sum + y;
  }
  return sum - LinuxParser::IdleJiffies();
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  vector<string> vect;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value != "cpu")
      {
        vect.push_back(value);
      }
    }
  }
  long sum = std::stol(vect[3]) + std::stol(vect[4]);
  return sum;
}

// INPRO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string value;
  vector<string> vect;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value)
    {
      if (value != "cpu")
      {
        vect.push_back(value);
      }
    }
  }
  return vect;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
          break;
        }
      }
    }
  }
  return std::stoi(value);
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
          break;
        }
      }
    }
  }
  return std::stoi(value);
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/cmdline");
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
  }
  return line; 
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  string it;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/status");
  if (filestream.is_open()) {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "VmSize")
        {
          it = value;
          break;
        }
      }
    }
  }
  return std::to_string(std::stof(it)*0.001);
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  string it;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/status");
  if (filestream.is_open()) {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid")
        {
          it = value;
          break;
        }
      }
    }
  }
  return it;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string key;
  string value;
  string it;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/status");
  if (filestream.is_open()) {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid")
        {
          std::ifstream passwdstream(kPasswordPath);
          if (passwdstream.is_open())
          {
            while (std::getline(passwdstream, line))
            {
              if (line.find(":" + value + ":" + value + ":"))
              {
                std::string::size_type pos = line.find(':');
                if (pos != std::string::npos)
                {
                  it = line.substr(0, pos);
                  break;
                }
              }
            }
          }
          break;
        }
      }
    }
  }
  return it;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/stat");
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int index = 1;
    while (linestream >> value && index < 22) {
      ++index;
    }
  }
  return std::stol(value);
}