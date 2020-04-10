#include <dirent.h>
#include <iomanip>
#include <unistd.h>
#include <string>
#include <vector>

#include "format.h"
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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  //calculates total used memory: MemTotal - MemFree
  string line;
  string key;
  float value;
  float MemTotal{-1};
  float MemFree{-1};
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") { MemTotal = value;}
      if (key == "MemFree:") { MemFree = value;}
      if ((MemTotal != -1) && (MemFree != -1)) {
        return ((MemTotal - MemFree)/MemTotal);
        }
    }
  }
  return 0.0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long up_time;
  string line;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> up_time; 
    return up_time;
  }
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string stat_member;
  long utime, stime, cutime, cstime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i{0};
    while (linestream >> stat_member) {
      i++;
      if (i == 14) {utime =  std::stol(stat_member);} 
      if (i == 15) {stime =  std::stol(stat_member);} 
      if (i == 16) {cutime =  std::stol(stat_member);} 
      if (i == 17) {cstime =  std::stol(stat_member); break;} 
    }
    return (utime + stime + cutime + cstime);
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string cpuField;
  vector<string> cpuFieldVector;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> cpuField) {
      cpuFieldVector.push_back(cpuField);
    }
    cpuFieldVector.erase(cpuFieldVector.begin());
  }
  return cpuFieldVector;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return value;
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); 
        return line;    
    }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string RAM;
  float fRAM;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> RAM;
      if (key == "VmSize:") {
        fRAM = stof(RAM);
        std::stringstream sRAM;
        sRAM << std::fixed << std::setprecision(2) << fRAM/1000;
        return sRAM.str();
      }
    }
  }
  return string("0"); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string UID;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> UID;
      if (key == "Uid:") {
        return UID;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string user;
  string dummy_x;
  string UID;
  string UID_user = LinuxParser::Uid(pid); 
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> dummy_x >> UID;
      if (UID == UID_user) {
        return user;
      }
    }
  }
  return string();  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) { 
  string line;
  string stat_member;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i{0};
    while (linestream >> stat_member) {
      i++;
      if (i == 22) {return (LinuxParser::UpTime() - std::stol(stat_member)/sysconf(_SC_CLK_TCK));}
    }
  }
  return 0;
  }