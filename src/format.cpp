#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {  
    long HH = seconds / 3600;
    long MM = (seconds%3600) / 60;
    long SS = (seconds%3600) % 60;

    return (to_string(HH) + ":" + to_string(MM) + ":" + to_string(SS));
}