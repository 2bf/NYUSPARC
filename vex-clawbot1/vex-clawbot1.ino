//define variables, etc
#define IR_L A2 //Left IR
#define IR_M A1 //Middle IR
#define IR_R A0 //Right IR
#define ENB 8 //right motor speed
#define ENA 9 //left motor speed
#define claw 10 //open and close claw
#define clawAngle 7 //move claw/arm up or down
//left motor direction
#define LN1 2  
#define LN2 3

//right motor direction
#define LN3 4
#define LN4 5

//ultrasonic pins
#define trigPin 12
#define echoPin 13

long duration;
int distance;
int speed = 150;
int speed2 = 150;
int findAngle = 0;
int initialAngle = 1;

//for line tracking
int lv, mv, rv;
bool L, M, R, turn = false;

int leftIR = analogRead(IR_L);
int midIR = analogRead(IR_M);
int rightIR = analogRead(IR_R);


//servo setup
#include <Servo.h>
Servo clawServo; //create servo object to control a servo
Servo myservo;
int servoPos = 0; //store servo position
Servo clawAngleServo;

void setup() {
  //attach servo object to pins
  clawServo.attach(10);
  clawAngleServo.attach(7);

  //set up IR pins
  pinMode(IR_L, INPUT);
  pinMode(IR_M, INPUT);
  pinMode(IR_R, INPUT);

  //set up motor pins
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LN1, OUTPUT);
  pinMode(LN2, OUTPUT);
  pinMode(LN3, OUTPUT);
  pinMode(LN4, OUTPUT);
  
  //set up ultrasound pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //set up arm pins
  pinMode(claw, OUTPUT);
  pinMode(clawAngle, OUTPUT);

  /**
  armUp();
  delay(50);
  armDown();
  delay(50); **/
  /**
  clawClose();
  delay(50);
  clawOpen();
  delay(50); **/
  //arm goes up a little so sensor doesnt see it
  /**
  clawAngleServo.write(120); //arm goes up
  delay(200);
  clawAngleServo.write(0);
  Serial.println("Arm up.");
  delay(40); **/

  Serial.begin(9600); //begin serial monitor
}

void loop() {
  if (ultrasound() < 16) {
    stop();
    pickUp();
    //turnBack();
  } else if (ultrasound() <= 45 && ultrasound() >= 25) {
    speed = 250;
    lineTrack();
  } else {
    speed = 150;
    lineTrack();
  }
}

//turns all motors forwards, so the robot moves forward
void forward(int n) {

  analogWrite(ENA, n);
  analogWrite(ENB, n);
  //left forward
  digitalWrite(LN1, HIGH);
  digitalWrite(LN2, LOW);
  //right forward
  digitalWrite(LN3, HIGH);
  digitalWrite(LN4, LOW);
  Serial.println("Forward");
}

//turns right motors forward and left motors backwards, so robot rotates to the left
void left(int n) {
  analogWrite(ENA, n);
  analogWrite(ENB, n);
  //left
  digitalWrite(LN1, LOW);
  digitalWrite(LN2, HIGH);
  //right
  digitalWrite(LN3, HIGH);
  digitalWrite(LN4, LOW);
  Serial.println("Right");
}

//turns left motors forward and right motors backwards, so robot rotates to the right
void right(int n) {
  analogWrite(ENA, n);
  analogWrite(ENB, n);
  //left
  digitalWrite(LN1, HIGH);
  digitalWrite(LN2, LOW);
  //right
  digitalWrite(LN3, LOW);
  digitalWrite(LN4, HIGH);
  Serial.println("Left");
} 

//reverses robot
void reverse(int n) {
  analogWrite(ENA, n);
  analogWrite(ENB, n);
  //left
  digitalWrite(LN1, LOW);
  digitalWrite(LN2, HIGH);
  //right
  digitalWrite(LN3, LOW);
  digitalWrite(LN4, HIGH);
  Serial.println("Reverse");
}

//stops robot
void stop() {
  int e = 0;
  analogWrite(ENA, e);
  analogWrite(ENB, e);
  //left
  digitalWrite(LN1, LOW);
  digitalWrite(LN2, LOW);
  //right
  digitalWrite(LN3, LOW);
  digitalWrite(LN4, LOW);
  Serial.println("Stopped.");
}

void infrared() {
  leftIR = analogRead(IR_L);
  midIR = analogRead(IR_M);
  rightIR = analogRead(IR_R);
  Serial.print(leftIR); Serial.print(" || "); Serial.print(midIR); Serial.print(" || "); Serial.println(rightIR);
}

void lineTrack2(){
  //lineAvoid();
  int angle = 600; //70 - 1000
  if(midIR >= angle && leftIR <= angle && rightIR <= angle){
    forward(speed);
    //lineAvoid();
  }
  else if(leftIR >= angle){
    left(speed);
    //lineAvoid();
  }
  else if(rightIR >= angle){
    right(speed);
    //lineAvoid();
  }
  else if(midIR <= angle && leftIR <= angle && rightIR <= angle){
    reverse(speed);
    //lineAvoid();
  }
}

void lineTrack() {
  lv = analogRead(IR_L);
  mv = analogRead(IR_M);
  rv = analogRead(IR_R);
  L = lv >= 700; //on tape
  M = mv >= 700;
  R = rv >= 700;

  Serial.print(lv); Serial.print(L); Serial.print("||");
  Serial.print(mv); Serial.print(M); Serial.print("||");
  Serial.print(rv); Serial.println(R);

  if ((L && M && R) || (!L && M && !R)){
    forward(speed);
    delay(12);
  } else if ((L)){ //(L && M && !R) ||
    if(M){
    left(speed);
    delay(15);
    } else {
    left(speed);
    delay(18);
    }
  } else if ((R)){ //(!L && M && R) ||
    if(M){
    right(speed);
    delay(15);
    } else {
    right(speed);
    delay(18);
    }
  } else if (!L && !M && !R){
    reverse(speed);
    delay(15);
  }
}

int ultrasound(){
  digitalWrite(trigPin, LOW); //make sure that trigger starts off
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //make trigger send signal
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); //turns trigger off
  duration = pulseIn(echoPin, HIGH); //checks time from echo
  distance = duration * 0.034 / 2; //use speed of sound to calculate distance
  Serial.println(distance);
  return distance;
}

void armUp() {
  clawAngleServo.write(120); //arm goes up
  delay(1000);
  clawAngleServo.write(0);
  Serial.println("Arm up.");
}

void armDown() {
  clawAngleServo.write(60); //arm goes down
  delay(600);
  clawAngleServo.write(0);
  Serial.println("Arm down.");
}

void clawOpen() {
  clawServo.write(120); //opens claw
  delay(1800);
  clawServo.write(0);
  Serial.println("Claw opened.");
}

void clawClose() {
  clawServo.write(0);
  delay(1000);
  clawServo.write(80); //closes claw
  delay(550);
  Serial.println("Claw closed.");
}

void liftDetect() {
  if (ultrasound() < 15) {
    liftAndPlace();
    turnBack();
  } else {
    lineTrack();
  }
}

void liftAndPlace() { //robot lifts object, turns to the right, and places it down
  Serial.println("LIFT AND PLACE METHOD");
  armDown();
  delay(200);
  clawOpen();
  delay(500);
  clawClose();
  delay(500);
  armUp();
  delay(1000);
  right(speed2);
  delay(2800);
  stop(); 
  delay(500);
  armDown();
  delay(400);
  clawOpen();
  delay(400);
}

void pickUp() {
  Serial.println("PICKUP METHOD");
  armDown();
  delay(200);
  //clawOpen();
  //delay(200);
  clawClose();
  delay(500);
  armUp();
  delay(500);
  turnBack3();
}

void turnBack2() { //after lifting the object, the robot turns back to its original direction
  Serial.println("TURN BACK METHOD");
  armUp();
  left(speed2);
  delay(2000);
  stop();
  delay(500);
  armDown();
  delay(1000);
  clawOpen();
  delay(2000);
  //arm goes up a little so sensor doesnt see it
  clawAngleServo.write(120); //arm goes up
  delay(300);
  clawAngleServo.write(0);
  Serial.println("Arm up.");
  delay(500);  
}


void turnBack() { //after lifting the object, the robot turns back to its original direction
  int goofy = 0;
  Serial.println("TURN BACK METHOD");
  armUp();

  goofy = analogRead(IR_M);
  while(!(goofy >= 700)){
    right(speed2);
    delay(20);
    goofy = analogRead(IR_M);
  }

  stop();
  delay(500);
  armDown();
  delay(1000);
  clawOpen();
  delay(1000);
  //arm goes up a little so sensor doesnt see it
  clawAngleServo.write(120); //arm goes up
  delay(300);
  clawAngleServo.write(0);
  Serial.println("Arm up.");
  delay(500);  
}

void turnBack3() {
  int goofy = 0;
  Serial.println("TURN BACK METHOD");
  armUp();

  goofy = analogRead(IR_M);
  while(!(goofy >= 700)){
    right(speed2);
    delay(20);
    goofy = analogRead(IR_M);
  } 
}

/**
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
  /**
  if (servoPos = 90) {
    
  }
  /**
  for(servoPos = 90; servoPos >=0; servoPos +=0) {
    myservo.write(servoPos);
    delay(6);
  } **/

/**
void stopServo() {
  servoPos += 0;
}

int e = 25;
int angleAdd = 5;
void servoFind() {
  //
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
    //
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
      left();
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
        forward();
      } else if (ultrasound() < accelDist) {
        forward();
      }
    } while (ultrasound() < d) {
      reverse();
    } 
    stop();
    }
  }
  else if (findAngle < 90) {
    myservo.write(90);
    delay(200);
    while (ultrasound() >= e && ultrasound() <= quitDist) {
      right();
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
        forward();
      } else if (ultrasound() < accelDist) {
        forward();
      }
    } while (ultrasound() < d) {
      reverse();
    } 
      stop();
    }
  }
  else if (findAngle == 90) {
    myservo.write(90);
    delay(200);
    while (ultrasound() >= e && ultrasound() <= quitDist) {
      forward();
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
        forward();
      } else if (ultrasound() < accelDist) {
        forward();
      }
    } while (ultrasound() < d) {
      reverse();
    } 
      stop();
    }
  }
  if (ultrasound() >= e) {
    servoFind();
  }
} 
**/