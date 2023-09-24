//define variables, etc
#define PWMA 5 //right motor speed
#define PWMB 6 //left motor speed
#define BIN 7 //right motor direction
#define AIN 8 //left motor direction
#define EN 3 //enable pin -on/off
#define trigPin 13
#define echoPin 12
#define IR_L A2 //Left IR
#define IR_M A1 //Middle IR
#define IR_R A0 //Right IR
long duration;
int distance;
int speed = 110;
int speedSlow = 50;
int findAngle = 0;

//servo setup
#include <Servo.h>
Servo myservo; //create servo object to control a servo
int servoPos = 0; //store servo position

void setup() {
  // put your setup code here, to run once:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN, OUTPUT);
  pinMode(AIN, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  Serial.begin(9600);
  //delay(2000);
  //servo();
  //delay(3000);
  myservo.attach(10); //attach servo to pin 10, or whichever number is in the parentheses
}

void loop() {
  //autoDrive();
  //servo();
  servoFind();
}

//automatic driving mode. robot drives forward, while using distance measurements from the ultrasonic sensor to avoid crashing into obstacles
void autoDrive() {
  forward(speed);
  antiCrash();
  printDist();
}

//turns all motors forwards, so the robot moves forward
void forward(int n) {
  digitalWrite(EN, HIGH);
  //right motors
  analogWrite(PWMA, n);
  digitalWrite(BIN, HIGH);
  //left motors
  analogWrite(PWMB, n);
  digitalWrite(AIN,HIGH);
  Serial.println("Forward");
  printDist();
}

//turns right motors forward and left motors backwards, so robot rotates to the left
void left(int n) {
  digitalWrite(EN, HIGH);
  //right motors
  analogWrite(PWMA, n);
  digitalWrite(BIN, HIGH);
  //left motors
  analogWrite(PWMB, n);
  digitalWrite(AIN,LOW);
  Serial.println("Left");
  printDist();
}

//turns left motors forward and right motors backwards, so robot rotates to the right
void right(int n) {
  digitalWrite(EN, HIGH);
  //right motors
  analogWrite(PWMA, n);
  digitalWrite(BIN, LOW);
  //left motors
  analogWrite(PWMB, n);
  digitalWrite(AIN,HIGH);
  Serial.println("Right.");
  printDist();
}

//reverses robot
void reverse(int n) {
  digitalWrite(EN, HIGH);
  //right motors
  analogWrite(PWMA, n);
  digitalWrite(BIN, LOW);
  //left motors
  analogWrite(PWMB, n);
  digitalWrite(AIN,LOW);
  Serial.println("Reverse.");
  printDist();
}

//stops robot
void stop() {
  digitalWrite(EN, LOW);
  Serial.println("Stopped.");
  printDist();
}

//detects distance using the ultrasound
int ultrasound() {
  digitalWrite(trigPin, LOW); //make sure that trigger starts off
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //make trigger send signal
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); //turns trigger off
  duration = pulseIn(echoPin, HIGH); //checks time from echo
  distance = duration * 0.034 / 2; //use speed of sound to calculate distance
  return distance;
}

//uses motors to move the robot away from obstacles
void avoid() {
  stop();
  //delay(500);
  reverse(speed);
  delay(300);
  randomDirection();
  delay(300);
  //delay(300);
}

//method that detects distance, then calls the avoid method to avoid obstaces
void antiCrash() {
  if(ultrasound() < 30) {
    avoid();
  }
}

//
void follow() {
  if (ultrasound() < 100) {
    
  }
  if (ultrasound() > 100) {
    
  }
}

//print distance from ultrasound into serial monitor
void printDist() {
  Serial.print(ultrasound()); //call ultrasound method
  Serial.println(" cm");
}

//randomizes the direction which the robot changes
void randomDirection() {
  int e = random(3);
  if (e == 1){
    right(speed);
  }
  else {
    left(speed);
  }
}

void servo() {
  for (int i = 1; i == 1; i += 1) {
    for(servoPos = 0; servoPos <= 180; servoPos += 1){ //goes from 0 degrees to 180 degrees
    myservo.write(servoPos); //tells servo to go to position in variable 'pos'
    delay(6); //waits 15ms for the servo to reach the position
    }
    for(servoPos = 180; servoPos >= 0; servoPos -= 1){
      myservo.write(servoPos);
      delay(6);
    }
  }
  /**
  for(servoPos = 0; servoPos <= 180; servoPos += 1){ //goes from 0 degrees to 180 degrees
    myservo.write(servoPos); //tells servo to go to position in variable 'pos'
    delay(6); //waits 15ms for the servo to reach the position
  }
  for(servoPos = 180; servoPos >= 0; servoPos -= 1){
    myservo.write(servoPos);
    delay(6);
  }
  **/
  if (servoPos = 90) {
    
  }
  /**
  for(servoPos = 90; servoPos >=0; servoPos +=0) {
    myservo.write(servoPos);
    delay(6);
  } **/
}

void stopServo() {
  servoPos += 0;
}

int e = 25;
int angleAdd = 5;
void servoFind() {
  //printDist();
  myservo.write(findAngle);
  delay(20);
  findAngle += angleAdd;
  Serial.println(findAngle);
  //Serial.print(findAngle);
  if (findAngle == 180) {
    angleAdd = angleAdd * -1;
  } else if (findAngle == 0) {
    angleAdd = angleAdd * -1;
  }
  while (ultrasound() <= e) {
    Serial.println(findAngle);
    //printDist();
    faceObject();
  }
}



void faceObject() {
  int accelDist = 200;
  int d = 20;
  int quitDist = 320;
  while (ultrasound() > quitDist) {stop();}
  if (findAngle > 90) {
    myservo.write(90);
    delay(200);
    while (ultrasound() >= e && ultrasound() <= quitDist) {
      left(speedSlow);
      Serial.println(findAngle);
    }
    if (ultrasound() <= e) {
      stop();
      delay(300);
      while (ultrasound() > d) {
      if (ultrasound() >= quitDist) {
        stop();
        servoFind();
      } else if (ultrasound() > accelDist) {
        forward(speed);
      } else if (ultrasound() < accelDist) {
        forward(speedSlow);
      }
    } while (ultrasound() < d) {
      reverse(speedSlow);
    } 
    stop();
    }
  }
  else if (findAngle < 90) {
    myservo.write(90);
    delay(200);
    while (ultrasound() >= e && ultrasound() <= quitDist) {
      right(speedSlow);
      Serial.println(findAngle);
    }
    if (ultrasound() <= e) {
      stop();
      delay(300);
      while (ultrasound() > d) {
      if (ultrasound() >= quitDist) {
        stop();
        servoFind();
      } else if (ultrasound() > accelDist) {
        forward(speed);
      } else if (ultrasound() < accelDist) {
        forward(speedSlow);
      }
    } while (ultrasound() < d) {
      reverse(speedSlow);
    } 
      stop();
    }
  }
  else if (findAngle == 90) {
    myservo.write(90);
    delay(200);
    while (ultrasound() >= e && ultrasound() <= quitDist) {
      forward(speedSlow);
      Serial.println(findAngle);
    }
    if (ultrasound() <= e) {
      stop();
      delay(300);
      while (ultrasound() > d) {
      if (ultrasound() >= quitDist) {
        stop();
        servoFind();
      } else if (ultrasound() > accelDist) {
        forward(speed);
      } else if (ultrasound() < accelDist) {
        forward(speedSlow);
      }
    } while (ultrasound() < d) {
      reverse(speedSlow);
    } 
      stop();
    }
  }
  if (ultrasound() >= e) {
    servoFind();
  }
}