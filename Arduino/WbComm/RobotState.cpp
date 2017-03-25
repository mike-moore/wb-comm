#include "RobotState.h"

void RobotState::RobotState() 
 :
 SensedHeading(0.0),
 SensedDistance(0.0),
 ActiveWayPoint(),
 WayPointQueue(),
 HeadingError(0.0),
 DistanceError(0.0),
 ControlSignal(0.0),
 TargetReached(false),
 ResponseSignal(0.0)
{
    /// - Nothing to do
}

