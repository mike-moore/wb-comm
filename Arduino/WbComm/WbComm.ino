#include "RobotState.h"
#include "ProtobuffSerial.h"
#include "CommandAndDataHandler.h"
#include "Navigation.h"
#include "Guidance.h"
#include "Control.h"

// - Initialize an instance of the robot's state registry
RobotState robotState;
// - Initialize an instance of the protbuff serial class to do communication
ProtobuffSerial serialComm;
// - Initialize an instance of the command and data handler
CommandAndDataHandler cmdAndDataHandler(serialComm.Commands, serialComm.Telemetry, robotState);
// - Initiliaze an instance of the robot navigation
Navigation navigation(robotState);
// - Initiliaze an instance of the robot guidance
Guidance guidance(robotState);
// - Initiliaze an instance of the robot control
Control control(robotState);

const long cycleTimeMillis = 20;
unsigned long previousMillis = 0;

void setup(){
  // - Serial comm init
  serialComm.InitHw();
}

void loop(){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= cycleTimeMillis) {
    /// - Save off current millis. Used for control loop timing.
    previousMillis = currentMillis;
    /// - Read commands from the serial port.
    serialComm.Rx();
    /// - Forward received commands on to C&DH
    if (serialComm.NewCommandsArrived()){
      cmdAndDataHandler.ProcessCmds();
    }
    /// Execute the robot navigation
    navigation.Execute();
    /// Execute the robot guidance
    guidance.Execute();
    /// Execute the robot control logic 
    control.Execute();
    /// Have C&DH prepare the robot telemetry for transmission
    cmdAndDataHandler.LoadTelemetry();
    /// - Send the telemetry over the serial port
    serialComm.Tx();
  }
  
}

