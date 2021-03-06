#include "RobotState.h"

RobotState::RobotState() 
 :
 SensedHeading(0.0),
 SensedDistance(0.0),
 ActiveWayPoint(),
 WayPointQueue(),
 HeadingError(0.0),
 DistanceError(0.0),
 ControlSignal(0.0),
 TargetReached(true),
 ResponseSignal(0.0)
{
	strncpy(ActiveWayPoint.Name,"StartWayPoint", 15);
	ActiveWayPoint.Heading = 0.0;
	ActiveWayPoint.Distance = 0.0;
}

