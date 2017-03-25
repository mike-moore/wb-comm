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
#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H

#include "comm_packet.pb.h"
#include "QueueList.h"


///////////////////////////////////////////////////////////////
/// @class RobotState
/// @ingroup WheelBot
/// @brief TODO
///////////////////////////////////////////////////////////////
class RobotState { 
 public:
  RobotState(){};
  ~RobotState(){};

  /// - Sensor readings
  float SensedHeading;
  float SensedDistance;

  /// - Robot guidance
  WayPoint ActiveWayPoint;
  QueueList <WayPoint> WayPointQueue;
  float HeadingError;
  float DistanceError;

  /// - Robot control
  float ControlSignal;
  bool TargetReached;
  /// - TEMP variable. Will go away once control is implemented
  ///   fully. DELETE
  uint32_t ControlWaitCycles;

  /// - Telemetry
  float ResponseSignal;


 protected:
 	/// - None yet
};

#endif
