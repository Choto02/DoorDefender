#include <Keypad.h>

//-----------Keypad---------------
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {3, 4, 5, 6};
byte colPins[COLS] = {7, 8, 9, 10};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

const String passcode = "123A";  // The correct passcode
String inputCode = "";           // Variable to store entered keys
const int maxInputLength = 4;    // Maximum length of the input
int numRetry = 0;
//-----------Keypad---------------

//----AlarmStatus----
#define AlarmStatus 46
//----AlarmStatus----

//----DoorStatus----
#define DoorStatus 48
//----DoorStatus----

//----Buzzer----
#define Buzzer 50
bool Buzz = false;
//----Buzzer----

//----Lock Module----
#define lockModule 52
bool ModuleState = false;
//----Lock Module----

//----LED----
#define greenLED 22
#define redLED 24
//----LED----

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(Buzzer, OUTPUT);
  pinMode(lockModule, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(DoorStatus, OUTPUT);
  pinMode(AlarmStatus, OUTPUT);
  digitalWrite(DoorStatus, LOW);
  digitalWrite(AlarmStatus, LOW);
}

void loop()
{

  char customKey = customKeypad.getKey();

  if (customKey)
  {
    Serial.println(customKey);
    inputCode += customKey;      // Append the pressed key to the input code

    if (inputCode.length() == maxInputLength)
    {
      if (checkPasscode(inputCode)) //IF CORRECT PASSWORD
      {
        Serial.println("Correct Passcode");
        digitalWrite(Buzzer, LOW);        //Start of the code set buzzer to off
        digitalWrite(AlarmStatus, LOW);
        
        if (ModuleState == false) //IF DOOR LOCK MAKE IT UNLOCK
        {
          digitalWrite(lockModule, HIGH);
          digitalWrite(greenLED, HIGH);
          digitalWrite(redLED, LOW);
          ModuleState = true;  //invert current state to execute the next else code block
          Serial.println("DOOR UNLOCKING");          
          digitalWrite(DoorStatus, HIGH);
        }
        else  //IF DOOR UNLOCK PRESS THEN MAKE IT LOCK
        {
          digitalWrite(lockModule, LOW);
          digitalWrite(redLED, HIGH);
          digitalWrite(greenLED, LOW);
          ModuleState = false;  //invert current state to execute the if code block
          Serial.println("DOOR LOCKING");         
          digitalWrite(DoorStatus, LOW);
        }

        numRetry = 0; //Clear number of retires from the incorrect password block
      }

      else //IF INCORRECT PASSWORD
      {
        Serial.println("Incorrect Passcode");

        numRetry += 1;  //INCREMENT THE NUMBER OF RETRIES

        if (numRetry > 2)
        {
          digitalWrite(Buzzer, HIGH);
          Serial.println("BUZZING");
          digitalWrite(AlarmStatus, HIGH);
        }
        Serial.print("Numretry: ");
        Serial.println(numRetry);
      }
      inputCode = "";  // Reset the input code
    }
  }
}

bool checkPasscode(String input)
{
  return input == passcode;
}
