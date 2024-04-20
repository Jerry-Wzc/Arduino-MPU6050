#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.



float angle_around_x, angle_around_y;
float a_x_t, a_y_t, a_x_offset, a_y_offset, a_x, a_y, a_z, v_x, v_y, d_x, d_y=0;
int buttonPin = 8;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  pinMode(buttonPin, INPUT_PULLUP);
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  a_x_t = (Wire.read()<<8 | Wire.read()) *9.8/16384.0; // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  a_y_t = (Wire.read()<<8 | Wire.read()) *9.8/16384.0; // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  a_z = (Wire.read()<<8 | Wire.read()) *9.8/16384.0; // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  a_x = 0.94*a_x + 0.06*a_x_t - a_x_offset;
  a_y = 0.94*a_y + 0.06*a_y_t - a_y_offset;
  angle_around_x = atan(a_y / sqrt(pow(a_x, 2) + pow(a_z, 2))); // in radian
  angle_around_y = atan(-1 * a_x / sqrt(pow(a_y, 2) + pow(a_z, 2))); // in radian

  v_x = v_x + a_x * 0.016 * cos(angle_around_y);
  v_y = v_y + a_y * 0.016 * cos(angle_around_x);
  d_y = d_y + v_y * 0.016;

  
  // print out data
  Serial.print(" aX = "); Serial.print(a_x);
  Serial.print(" aY = "); Serial.print(a_y);
  Serial.print(" aZ = "); Serial.print(a_z);
  Serial.println();
  /*
  Serial.print(" v_x = "); Serial.print(v_x);
  Serial.print(" v_y = "); Serial.print(v_y);
  Serial.print(" d_y = "); Serial.print(d_y);
  Serial.println();*/

  if( digitalRead(buttonPin) == LOW ){
    Serial.print("Click");
    a_x_offset = a_x;
    a_y_offset = a_y;
    delay(500);
  }



  
  // delay
  delay(16);
}