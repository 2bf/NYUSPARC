#include <Wire.h> //library allows communication with I2C/TWI devices
#include <math.h> //includes mathematical functions

const int MPU=0x68; //I1C address of the MPU-6050
int16_t AcX, AcY, AcZ; //16-bit integers
int AcXcal, AcYcal, AcZcal; //calibration variables
double pitch, roll, yaw;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //initiate wire library and I2C
  Wire.beginTransmission(MPU); //begin transmission to I2C slave device
  Wire.write(0x6B); //PWR_MGMT_1 register
  Wire.write(0); //set to zero (wakes up the MPU-6050);
  Wire.endTransmission(true); //ends transmission to I2C slave devices
  Serial.begin(9600); //serial communication at 9600 bauds
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(MPU); //begin transmission to I2C slave device
  Wire.write(0x3B); //starting with register 0x38 (ACCEL_XOUT_H)
  Wire.endTransmission(false); //restarts transmission to I2C slave device
  Wire.requestFrom(MPU,14,true); //request 14 registers in total

  //Acceleration data correction
  AcXcal = -950;
  AcYcal = -300;
  AcZcal = 0;

  //read accelerometer data
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  //get pitch/roll
  getAngle(AcX, AcY, AcZ);

  //printing values to serial port
  Serial.print("Angle: ");
  Serial.print ("Pitch = "); Serial.print (pitch);
  Serial.print (" Roll = "); Serial.print (roll);
  Serial.print (" Yaw = "); Serial.println (yaw);

  // Serial.print("Accelerometer: ");
  // Serial.print("X= "); Serial.print(AcX + AcXcal);
  // Serial.print(" Y= "); Serial.print(AcY + AcYcal);
  // Serial.print(" Z= "); Serial.println(AcZ + AcZcal);
  // Serial.println("----------------------------------------------");
  // Serial.println("----------------------------------------------");
  delay(20);
} 

//function to convert acccelerometer values into pitch and roll
void getAngle(int Ax, int Ay, int Az) {
  double x = Ax;
  double y = Ay;
  double z = Az;

  pitch = atan(x/sqrt((y*y)+ (z*z))); //pitch calculation
  roll = atan(y/sqrt((x*x) + (z*z))); //roll calculation
  yaw = atan(z/sqrt((x*x) + (y*y))); 
  //converting radians into degrees
  pitch = pitch * (180.0/3.14);
  roll = roll * (180.0/3.14);
  yaw = yaw * (180.0/3.14);
}
