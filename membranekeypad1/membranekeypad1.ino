#include <Keypad.h>

int ledPin = 13;

const byte ROWS = 4; //const: not altered anywhere in code. byte: 
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8}; //pin numbers
byte colPins[COLS] = {7, 6, 5, 4};

//use the library that comes with the sensor to define the keypad
//define as object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
 
void setup() {
  Serial.begin(9600); //initialize serial
  analogWrite(ledPin, 0);
}
 
  String solution = "1234";
  String input = "";
  String display = "";


void loop() {
  char key = keypad.getKey();
  if(key){
    Serial.println(key);
    if(key != '#'){
      input += key;
      Serial.println(input);
    } else {
      input = "";
      analogWrite(ledPin, 0);
      Serial.println(input);
    }
    /**
    if(input.length() > 4){
      input = "";
    } **/
    if(input == solution){
      input = "";
      analogWrite(ledPin, 255);
      delay(2500);
      analogWrite(ledPin, 0);
      
    }
  }
}
