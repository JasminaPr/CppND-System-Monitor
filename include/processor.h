#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  static float cpuIdlePrevious_, cpuTotalPrevious_;

  // TODO: Declare any necessary private members
 private:
  std::vector<std::string> cpuFields_;
  std::vector<float> cpuFieldsFloats_;
  float cpuIdle_, cpuNonIdle_, cpuTotal_;
  float cpuPercentage_;
};

#endif