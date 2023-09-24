const int trigPin = 10; //constant number declaration that we use for the pin number - does not declare that it is a pin
const int echoPin = 11;
const int bluePin = 7;
const int yellowPin = 8;
const int redPin = 9;
long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  analogWrite(redPin, LOW);
  analogWrite(bluePin, LOW);
  analogWrite(yellowPin, LOW);
  
}

int highDist = 20;
int medDist = 10;
int lowDist = 5;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(ultrasonic()); //call ultrasonic method
  Serial.println(" cm");

  if(ultrasonic() >= highDist){
    analogWrite(bluePin, 100);
    analogWrite(yellowPin, 100);
    analogWrite(redPin, 100);
  }
  else if(highDist >= ultrasonic() && ultrasonic() >= medDist){
    analogWrite(redPin, LOW);
    analogWrite(bluePin, 100);
    analogWrite(yellowPin, 100);
  }
  else if(lowDist <= ultrasonic() && ultrasonic() <= medDist){
    analogWrite(redPin, LOW);
    analogWrite(bluePin, 100);
    analogWrite(yellowPin, LOW);
  }
  else{
    analogWrite(redPin, LOW);
    analogWrite(bluePin, LOW);
    analogWrite(yellowPin, LOW);
  }
}

int ultrasonic(){
  digitalWrite(trigPin, LOW); //make sure that trigger starts off
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //make trigger send signal
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); //turns trigger off
  duration = pulseIn(echoPin, HIGH); //checks time from echo
  distance = duration * 0.034 / 2; //use speed of sound to calculate distance
  return distance;
}