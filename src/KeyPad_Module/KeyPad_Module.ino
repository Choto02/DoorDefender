#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

const String passcode = "123A";  // The correct passcode
String inputCode = "";           // Variable to store entered keys
const int maxInputLength = 4;    // Maximum length of the input

void setup(){
  Serial.begin(115200);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    inputCode += customKey;      // Append the pressed key to the input code

    if (inputCode.length() == maxInputLength) {
      if (checkPasscode(inputCode)) {
        Serial.println("Correct Passcode");
      } else {
        Serial.println("Incorrect Passcode");
      }
      inputCode = "";            // Reset the input code
    }
  }
}

bool checkPasscode(String input) {
  return input == passcode;
}
