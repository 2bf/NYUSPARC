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
bool once = true;

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
  delay(1000);
  armDown();
  delay(1000);
  clawClose();
  delay(1000);
  clawOpen();
  delay(1000);
  //arm goes up a little so sensor doesnt see it
  clawAngleServo.write(120); //arm goes up
  delay(300);
  clawAngleServo.write(0);
  Serial.println("Arm up.");
  delay(500);  
  **/

  Serial.begin(9600); //begin serial monitor
}

void loop() {
  ultrasound();
  if (ultrasound() < 16 && once) {
    stop();
    delay(100);
    liftAndPlace();
    turnBack();
    lineTrack();
    once = false;
  } else {
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
  clawAngleServo.write(80); //arm goes down
  delay(300);
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
  delay(100);
  Serial.println("Claw closed.");
}

void liftDetect() {
  ultrasound();
  if (ultrasound() < 15) {
    liftAndPlace();
    turnBack();
  }
}

void liftAndPlace() { //robot lifts object, turns to the right, and places it down
  Serial.println("LIFT AND PLACE METHOD");
  armDown();
  delay(100);
  clawClose();
  delay(200);
  armUp();
  delay(1000);
  // right(speed2);
  // delay(2800);
  stop();
  delay(500);
}

void turnBack2() { //after lifting the object, the robot turns back to its original direction
  Serial.println("TURN BACK METHOD");
  armUp();
  left(speed2);
  delay(2100);
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
  right(200);
  delay(2000);

  // goofy = analogRead(IR_M);
  // while(!(goofy >= 700)){
  //   left(speed2);
  //   delay(20);
  //   goofy = analogRead(IR_M);
  // }

  stop();
  /**
  delay(500);
  armDown();
  delay(1000);
  clawOpen();
  delay(1000); **/
  //arm goes up a little so sensor doesnt see it
  /**
  clawAngleServo.write(120); //arm goes up
  delay(300);
  clawAngleServo.write(0);
  Serial.println("Arm up.");
  delay(500);   **/
}