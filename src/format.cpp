#include <string>
#include <sstream>
#include <iomanip> 
#include <iostream>

#include "format.h"

using std::string;
using std::stringstream;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::stringstream ss;
    int minutes, hours;
    minutes = seconds / 60;
    hours = minutes / 60;
    ss << std::setw(2) << std::setfill('0') << int(hours) << ':'; 
    ss << std::setw(2) << std::setfill('0') << int(minutes % 60) << ':'; 
    ss << std::setw(2) << std::setfill('0') << int(seconds % 60); 
    return ss.str(); 
}