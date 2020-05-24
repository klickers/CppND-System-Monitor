#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string hoursString, minutesString, secondsString;
  long secondsLeftover = seconds % (60 * 60);
  long hours = (seconds - secondsLeftover) / (60 * 60);
  secondsLeftover %= 60;
  long minutes = (seconds - secondsLeftover - hours*60*60) / 60;
  long secs = secondsLeftover;
  if (hours < 10)
    hoursString = "0" + std::to_string(hours);
  else
    hoursString = std::to_string(hours);
  if (minutes < 10)
    minutesString = "0" + std::to_string(minutes);
  else
    minutesString = std::to_string(minutes);
  if (secs < 10)
    secondsString = "0" + std::to_string(secs);
  else
    secondsString = std::to_string(secs);
  return hoursString + ":" + minutesString + ":" + secondsString; 
}