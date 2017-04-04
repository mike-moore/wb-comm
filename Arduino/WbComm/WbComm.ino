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

const long cycleTimeMillis = 100;
unsigned long previousMillis = 0;


#include <Wire.h>
#include <stdint.h>

#define LIS3MDL_ADDRESS      0x1C
#define LIS3MDL_WHO_AM_I     0x0F  // should return 0x3D
#define LIS3MDL_CTRL_REG1    0x20
#define LIS3MDL_CTRL_REG2    0x21
#define LIS3MDL_CTRL_REG3    0x22
#define LIS3MDL_CTRL_REG4    0x23
#define LIS3MDL_CTRL_REG5    0x24
#define LIS3MDL_STATUS_REG   0x27   
#define LIS3MDL_OUT_X_L      0x28  // data
#define LIS3MDL_OUT_X_H      0x29
#define LIS3MDL_OUT_Y_L      0x2A
#define LIS3MDL_OUT_Y_H      0x2B
#define LIS3MDL_OUT_Z_L      0x2C
#define LIS3MDL_OUT_Z_H      0x2D
#define LIS3MDL_TEMP_OUT_L   0x2E
#define LIS3MDL_TEMP_OUT_H   0x2F  // data
#define LIS3MDL_INT_CFG      0x30
#define LIS3MDL_INT_SRC      0x31
#define LIS3MDL_INT_THS_L    0x32
#define LIS3MDL_INT_THS_H    0x33

enum Mscale {
  MFS_4Gauss = 0,  // 0.15 mG per LSB
  MFS_8Gauss,      // 0.30 mG per LSB
  MFS_12Gauss,     // 0.60 mG per LSB
  MFS_16Gauss      // 1.20 mG per LSB
};

enum Mopmode {
  MOM_lowpower = 0,   
  MOM_medperf,       
  MOM_hiperf,      
  MOM_ultrahiperf       
};

enum MSodr {        // magnetometer output data rate when slaved to the MAX21100
  MODR_div1 = 0,    // default, magnetometer ODR is 1/1 of the accel ODR
  MODR_div2,
  MODR_div4,
  MODR_div8,  
  MODR_div16,
  MODR_div32,
  MODR_div64, 
  MODR_div128 
};

enum Modr {         // magnetometer output data rate MAX21100 is bypassed
  MODR_0_625Hz = 0,     
  MODR_1_25Hz,
  MODR_2_5Hz,
  MODR_5Hz,  
  MODR_10Hz,
  MODR_20Hz,
  MODR_40Hz, 
  MODR_80Hz 
};

typedef union {
 int16_t integer;
 byte binary[2];
} binaryFloat;

binaryFloat mag_x;
binaryFloat mag_y;
binaryFloat mag_z;

byte x_low;
byte x_high;
byte y_low;
byte y_high;
byte z_low;
byte z_high;

void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

void initLIS3MDL() {
  // Configure magnetometer
  // Choose device mode (bits 1:0 = 00 = continuous data read, 01 = single conversion, 10 & 11 = default power down)
  writeByte(LIS3MDL_ADDRESS, LIS3MDL_CTRL_REG3, 0x00); // Enable continuous data read mode (bits 1:0 = 00)
  // Enable temperature sensor (bit 7 = 1)
  // Set magnetometer operative mode for x and y axes (bits 6:5)
  // Set magnetometer ODR (bits 4:2)
  writeByte(LIS3MDL_ADDRESS, LIS3MDL_CTRL_REG1, 0x9C);
  writeByte(LIS3MDL_ADDRESS, LIS3MDL_CTRL_REG2, 0x00);  // Set magnetometer full scale range
  writeByte(LIS3MDL_ADDRESS, LIS3MDL_CTRL_REG4, 0x00);  // Set magnetometer operative mode for z axis
  writeByte(LIS3MDL_ADDRESS, LIS3MDL_CTRL_REG5, 0x40); // output registers not updated until both data bytes have been read
}





void setup(){
  // - Serial comm init
  serialComm.InitHw();

  initLIS3MDL();   
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
    // step 1: instruct sensor to read echoes
    Wire.beginTransmission(28); // transmit to device #28 
    // the address specified in the datasheet is 224 (0xE0)
    // but i2c adressing uses the high 7 bits so it's 112
    Wire.write(byte(0x28));      // sets register pointer to the command register (0x00)  // use 0x51 for centimeters
    Wire.endTransmission();      // 
    //  // step 3: instruct sensor to return a particular echo reading
    Wire.requestFrom(28, 6);    // request 6 bytes from slave device #112
    x_low = Wire.read();
    x_high = Wire.read();
    y_low = Wire.read();
    y_high = Wire.read();
    z_low = Wire.read();
    z_high = Wire.read();
    mag_x.integer = (x_low | (x_high << 8));
    mag_y.integer = (y_low | (y_high << 8));
    mag_z.integer = (z_low | (z_high << 8));
    Serial.print("Mag X : ");
    Serial.println(mag_x.integer);
    Serial.print("Mag Y : ");
    Serial.println(mag_y.integer);
    Serial.print("Mag Z : ");
    Serial.println(mag_z.integer);
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

