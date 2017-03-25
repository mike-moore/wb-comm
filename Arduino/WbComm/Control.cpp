#include "Control.h"

void Control::Execute() {
    State.ResponseSignal = State.ControlSignal*0.95;
    /// - Simulate control by waiting 500 cycles (10 seconds at 50 Hz) 
    ///   before setting target reached.
    /// - TODO : fully implement
    if(ControlWaitCycles <= 500){
        ControlWaitCycles++;
        State.TargetReached = false;
    }else{
        ControlWaitCycles = 0;
        State.TargetReached = true;
    }
}

