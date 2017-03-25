#include "Control.h"

void Control::Execute() {
    State.ResponseSignal = State.ControlSignal;
    /// - Simulate control by waiting 250 cycles (5 seconds at 50 Hz) 
    ///   before setting target reached.
    /// - TODO : fully implement
    if (!State.TargetReached){
        if(ControlWaitCycles <= 250){
            ControlWaitCycles++;
        }else{
            Serial.print("WayPoint Reached : ");
            Serial.println(State.ActiveWayPoint.Name);
            ControlWaitCycles = 0;
            State.TargetReached = true;
        }
    }
}

