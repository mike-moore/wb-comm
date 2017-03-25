///////////////////////////////////////////////////////////////
///  This file defines a class that is used to implement
///  the TODO.
///
/// @author
///         $Author: Mike Moore $
///
/// Contact: mike.moore@so.engineering
///
/// Created on: March 23 2017
///
///////////////////////////////////////////////////////////////
#ifndef CONTROL_H
#define CONTROL_H

#include "RobotState.h"

///////////////////////////////////////////////////////////////
/// @class Control
/// @ingroup WheelBot
/// @brief TODO
///////////////////////////////////////////////////////////////
class Control { 
 public:
    Control(RobotState& state) : State(state) {};
    ~Control(){};

    void Execute();

 private:
    RobotState& State;
};
  

#endif
