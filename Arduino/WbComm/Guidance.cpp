#include "Guidance.h"

void Guidance::Execute() {
    switch(Mode)
    {
        case IDLE:
            /// - Zero the errors sent to control
            State.HeadingError = 0.0;
            State.DistanceError = 0.0;
            if (State.WayPointQueue.isEmpty()){
                return;
            }else{
                /// - Check the queue for a valid way-point to be processed.
                WayPoint way_point;
                way_point = State.WayPointQueue.peek();
                /// - Set the active way point and move to trackind mode.
                strncpy(State.ActiveWayPoint.Name, way_point.Name, 15);
                Mode = TRACKING;
            }
        break;

        case TRACKING:
            /// - Pop the waypoint off the queue and start tracking it.
            WayPoint way_point;
            way_point = State.WayPointQueue.pop();
            /// - Compute the errors for control
            State.HeadingError = way_point.Heading - State.SensedHeading;
            State.DistanceError = way_point.Distance - State.SensedDistance;
            if (State.TargetReached){
                Mode = IDLE;
            }
        break;

        case MAPPING:
             /// - To be implemented
             Mode = IDLE;
        break;

        default:
             /// - invalid state - go back to IDLE
             Mode = IDLE;
        break;
    }
    return;
}

