#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.


int16_t ax_raw, ay_raw, az_raw;
int16_t ax_raw_offset, ay_raw_offset=0;
float ax, ay, az, roll, pitch, angle_around_y;
float axx, ayy, azz;
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
  az = (az_raw) * 9.8 / 16384.0;
  roll = atan(ay_raw / sqrt(pow(ax_raw, 2) + pow(az_raw, 2))); // rads
  pitch = atan(-1 * ax_raw / sqrt(pow(ay_raw, 2) + pow(az_raw, 2)));

  // az = az_raw * 9.8 / 16384.0; // m/s2

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
  /*
  Serial.print(ax); Serial.print(" / "); 
  Serial.print(ay); Serial.print(" / "); 
  Serial.print(vx); Serial.print(" / "); 
  Serial.print(vy); Serial.println();*/

  axx = (ax * cos(pitch) + sin(pitch) * (ay*sin(roll) + az*cos(roll)));
  ayy = (ay * cos(roll) - az * sin(roll));
  azz = (-ax * sin(pitch) + cos(pitch) * (ay*sin(roll) + az*cos(roll)));

  // Serial.print(ax); Serial.print("/"); Serial.print(ay); Serial.print("/"); Serial.print(az); Serial.print(" - ");
  // Serial.print(roll); Serial.print("/"); Serial.print(pitch); Serial.print("  -  ");
  Serial.print(axx); Serial.print("/"); Serial.print(ayy); Serial.print("/"); Serial.println(azz);


  
  // delay
  //delay(100);
}