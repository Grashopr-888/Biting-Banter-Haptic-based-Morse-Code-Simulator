#include "HD44780_LCD_PCF8574.h"

// LCD Configuration
#define DISPLAY_DELAY_INIT 50 // mS
HD44780LCD myLCD(2, 16, 0x27, &Wire); // Instantiate an object

// FSR and Morse Code Variables
int fsrAnalogPin = 0;
int fsrReading;
unsigned long pressStartTime = 0, pressEndTime = 0, pressDuration = 0, lastPressEndTime = millis();
bool isPressed = false;
String pressSequence = "";
String displayString = ""; // String to hold the translated Morse code

// Morse Code Mapping
String morseCode[26] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", 
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", 
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);

  delay(DISPLAY_DELAY_INIT);
  myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOn);
  myLCD.PCF8574_LCDClearScreen();
  myLCD.PCF8574_LCDBackLightSet(true);
  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
}

String translateMorseCode(String morse) {
  for (int i = 0; i < 26; i++) {
    if (morseCode[i] == morse) {
      return String(char('A' + i));
    }
  }
  return ""; // Return empty string if not found
}

void loop() {
  fsrReading = analogRead(fsrAnalogPin);

  if (fsrReading >= 100) {
    digitalWrite(8, HIGH);

    if (!isPressed) {
      pressStartTime = millis();
      isPressed = true;
    }
  } else if (isPressed) {
    digitalWrite(8, LOW);

    pressEndTime = millis();
    pressDuration = pressEndTime - pressStartTime;
    isPressed = false;

    if (pressDuration >= 100 && pressDuration < 500) {
      pressSequence += ".";
    } else if (pressDuration >= 500 && pressDuration <= 1500) {
      pressSequence += "-";
    } else if (pressDuration > 3000) {
      myLCD.PCF8574_LCDClearScreen();
      displayString = ""; // Clear the display string
      Serial.println("LCD Cleared, Ready for New Input");
      lastPressEndTime = millis(); // Reset the last press end time
      return; // Exit the current loop iteration
    }

    lastPressEndTime = millis();
  }

  if (millis() - lastPressEndTime > 2000 && pressSequence.length() > 0) {
    String letter = translateMorseCode(pressSequence);
    if (letter != "") {
      displayString += letter; // Append the letter to the display string
      myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
      myLCD.PCF8574_LCDSendString(displayString.c_str()); // Display the updated string
      Serial.print("Morse Code: ");
      Serial.print(pressSequence);
      Serial.print(" -> Letter: ");
      Serial.println(letter);
    } else {
      Serial.println("Invalid Morse Code");
    }
    pressSequence = "";
  }

  delay(100);
}






