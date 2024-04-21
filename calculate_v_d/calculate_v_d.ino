#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.


int16_t ax_raw, ay_raw, az_raw;
int16_t ax_raw_offset, ay_raw_offset=0;
float ax, ay, az, angle_around_x, angle_around_y;
float vx, vy=0.0;
float ax_c, ay_c=0.0;
int buttonPin = 8;
int counter, sum_x, sum_y=0;
float avg=0.0;


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
  ax_raw = (Wire.read()<<8 | Wire.read()); 
  ay_raw = (Wire.read()<<8 | Wire.read());
  az_raw = (Wire.read()<<8 | Wire.read()); 
  ax = (ax_raw - ax_raw_offset) * 9.8 / 16384.0; // m/s2
  ay = (ay_raw - ay_raw_offset) * 9.8 / 16384.0; // m/s2
  // az = az_raw * 9.8 / 16384.0; // m/s2
  vx = vx + ax * 0.1;
  vy = vy + ay * 0.1;

  if (digitalRead(buttonPin) == LOW){
    //Serial.print("calibrating");
    counter++;
    sum_x = sum_x + ax_raw;
    sum_y = sum_y + ay_raw;
    ax_raw_offset = sum_x / counter;
    ay_raw_offset = sum_y / counter;
    vx=0;
    vy=0;
  }

  ax_c = 0.95 * ax_c + 0.05 * ax;
  ay_c = 0.95 * ay_c + 0.05 * ay;

  // print out data
  Serial.print(ax); Serial.print(" / "); Serial.print(ay); Serial.print(" / "); Serial.print(vx); Serial.print(" / "); Serial.print(vy);
  // Serial.print(" aY = "); Serial.print(ay);
  // Serial.print(" aZ = "); Serial.print(az);
  Serial.println();
  /*
  Serial.print(" v_x = "); Serial.print(v_x);
  Serial.print(" v_y = "); Serial.print(v_y);
  Serial.print(" d_y = "); Serial.print(d_y);
  Serial.println();*/




  
  // delay
  delay(100);
}