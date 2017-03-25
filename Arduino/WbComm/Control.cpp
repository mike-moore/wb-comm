#include "Control.h"

void Control::Execute() {
    State.ResponseSignal = State.ControlSignal*0.95;
    /// - Simulate control by waiting 500 cycles (10 seconds at 50 Hz) 
    ///   before setting target reached.
    /// - TODO : fully implement
    if(State.ControlWaitCycles <= 500){
        State.ControlWaitCycles++;
        State.TargetReached = true;
        State.ControlWaitCycles = 0;
    }else{
        State.TargetReached = false;
    }
}

