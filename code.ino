#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Servo.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Define your Wi-Fi credentials
const char* ssid = "1902";
const char* password = "42607880";

// Define Firebase credentials
const char* api_key = "AIzaSyBIJYPv5KN0idrkrLEsZGMh8sSmMGXR6QE";
const char* db_url = "https://smart-door-e15bb-default-rtdb.asia-southeast1.firebasedatabase.app/";

// Define GPIO pin for the servo
const int servoPin = D3;

// Create instances for Firebase and Servo
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
Servo myServo;

bool signupOK = false;
unsigned long sendDataPrevMillis = 0;
int servoAngle = 0;

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  Serial.println("Setup started");

  // Attach the servo to the specified pin
  myServo.attach(servoPin);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  int wifi_connect_attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_connect_attempts++;
    if (wifi_connect_attempts > 60) { // Timeout after 30 seconds
      Serial.println("Failed to connect to Wi-Fi");
      return;
    }
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Configure Firebase
  config.api_key = api_key;
  config.database_url = db_url;

  // Sign up for Firebase
  Serial.println("Signing up for Firebase...");
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Sign up successful");
    signupOK = true;
  } else {
    Serial.print("Sign up error: ");
    Serial.println(config.signer.signupError.message.c_str());
    return;
  }

  // Initialize Firebase
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase initialized");
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Read the servo angle from Firebase
    if (Firebase.RTDB.getInt(&fbdo, "/servo/angle")) {
      if (fbdo.dataType() == "int") {
        servoAngle = fbdo.intData();
        Serial.print("Servo angle received from Firebase: ");
        Serial.println(servoAngle);
        // Move the servo to the specified angle
        myServo.write(servoAngle);
      }
    } else {
      Serial.print("Failed to read servo angle: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
