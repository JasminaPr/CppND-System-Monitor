#include <algorithm>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    cpuFields_ = LinuxParser::CpuUtilization(); 
    cpuFieldsFloats_.reserve(cpuFields_.size());
    std::transform(cpuFields_.begin(), cpuFields_.end(), cpuFieldsFloats_.begin(), [](const std::string& val)
    {
        return std::stof(val);
    }); 
    // idle + iowait
    cpuIdle_ = cpuFieldsFloats_[3] + cpuFieldsFloats_[4]; 
    // user + nice + system + irq + softirq + steal
    cpuNonIdle_ = cpuFieldsFloats_[0] + cpuFieldsFloats_[1] + cpuFieldsFloats_[2]
         + cpuFieldsFloats_[5] + cpuFieldsFloats_[6] + cpuFieldsFloats_[7];
    cpuTotal_ = cpuIdle_ + cpuNonIdle_;
    cpuPercentage_ = ((cpuTotal_ - cpuTotalPrevious_) - (cpuIdle_ - cpuIdlePrevious_)) / (cpuTotal_ - cpuTotalPrevious_);
    //cpuPercentage_ = (cpuTotal_ - cpuIdle_) / cpuTotal_;
    cpuTotalPrevious_ = cpuTotal_;
    cpuIdlePrevious_ = cpuIdle_;
    return cpuPercentage_; 
}

float Processor::cpuIdlePrevious_ = 0;
float Processor::cpuTotalPrevious_ = 0;
