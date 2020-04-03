#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"

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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  float MemTotal, MemFree;
  string key,value,kb,line;  

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {

    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while ( linestream >> key >> value >> kb ) {
        if ( key == "MemTotal" ) { MemTotal = std::stof(value); }
        if ( key == "MemFree" ) { MemFree = std::stof(value); }
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime_long;  
  string uptime, idletime, line;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime  >> idletime;
    uptime_long = std::stol(uptime);
  }

  return uptime_long;
 }

// TODO: Read and return the number of jiffies for the system
/*
long LinuxParser::Jiffies() { 
  
  string line, cpu, sUser, sNice, sSystem, sIdle, sIOwait, sIRQ, sSoftIRQ, sSteal, sGuest, sGuestNice; 
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> sUser >> sNice >> sSystem >> sIdle >> sIOwait >> sIRQ >> sSoftIRQ >> sSteal >> sGuest >> sGuestNice;
  }

  return (std::stol(sUser) + std::stol(sNice) + std::stol(sSystem) + std::stol(sIdle) + std::stol(sIOwait) + std::stol(sIRQ) + std::stol(sSoftIRQ) + std::stol(sSteal) + std::stol(sGuest) + std::stol(sGuestNice));
  
}
*/

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
 
  string line, trash, utime, stime, cutime, cstime; 
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for( int i = 0; i < 13; i++) { linestream >> trash; }
    linestream >> utime >> stime >> cutime >> cstime ;
    
  }

  return (std::stol(utime) + std::stol(stime) + std::stol(cutime) + std::stol(cstime));
  
}

// TODO: Read and return the number of active jiffies for the system

/*
long LinuxParser::ActiveJiffies() { 
  string line, cpu, sUser, sNice, sSystem, sIdle, sIOwait, sIRQ, sSoftIRQ, sSteal, sGuest, sGuestNice; 
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> sUser >> sNice >> sSystem >> sIdle >> sIOwait >> sIRQ >> sSoftIRQ >> sSteal >> sGuest >> sGuestNice;
  }

  return (std::stol(sUser) + std::stol(sNice) + std::stol(sSystem) + std::stol(sIRQ) + std::stol(sSoftIRQ) + std::stol(sSteal) + std::stol(sGuest) + std::stol(sGuestNice));
  

 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line, cpu, sUser, sNice, sSystem, sIdle, sIOwait, sIRQ, sSoftIRQ, sSteal, sGuest, sGuestNice; 
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> sUser >> sNice >> sSystem >> sIdle >> sIOwait >> sIRQ >> sSoftIRQ >> sSteal >> sGuest >> sGuestNice;
  }

  return ( std::stol(sIdle) + std::stol(sIOwait) );
  
 }

 */

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
  string value, line, key;
  vector<string> jiffies_list;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  	std::istringstream linestream(line);
   	while (linestream >> key){
    	if (key != "cpu"){
          jiffies_list.push_back(key);}
    }
    }
  return jiffies_list;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
  string line, key,  value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") { return std::stoi(value); }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 

  string line, key,  value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "proc_running") { return std::stoi(value); }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    return line;
  }
  
  return ""; 
  
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string key,value,kb,line;  

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value >> kb;
      if (key == "VmSize:") { return to_string(std::stoi(value)/1000); }
    }
  }
  return "";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string key,value,line;  

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") { return value; }
    }
  } 
  return "";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string user, password, uid, line;
  string uid_ = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> user >> password >> uid;
      if (uid_ == uid) { return user; }
    }
  }
  return "root";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function

long int LinuxParser::UpTime(int pid) { 
  string line, time; 
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for( int i = 0; i < 21; i++) { linestream >> time;}
    long int starttime =  std::stol(time)/sysconf(_SC_CLK_TCK);
    long int uptime =  LinuxParser::UpTime() - starttime;
    return uptime;
  }

  return 0;
}

