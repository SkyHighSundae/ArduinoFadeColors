#include <EEPROM.h>

const int redPin = 9;    // Pin connected to the Red wire
const int greenPin = 10; // Pin connected to the Green wire
const int bluePin = 11;  // Pin connected to the Blue wire

int mode = 0;  // Default mode

// Function prototypes
void fadeInOut(int pin, bool combine = false);
void fadeOut();
void randomMixUp();
void blinkColor(int pin);
void onlyWhite();
void clearLEDs();
void checkSerialInput();
void softReset();

void setup() {
  // Start Serial communication
  Serial.begin(9600);

  // Reading EEPROM data
  Serial.print("Reading EEPROM...");

  // Retrieve the mode from EEPROM
  mode = EEPROM.read(0);

  // Log the EEPROM data
  Serial.println("done");
  Serial.print("EEPROM data: ");
  Serial.println(mode);

  // If mode is not valid (0-5), reset to 0
  if (mode < 0 || mode > 5) {
    mode = 0;  // Default to normal mode
    EEPROM.write(0, mode);  // Save default mode to EEPROM
  } else {
    // Increment mode for next startup
    mode = (mode + 1) % 6; // Increment and wrap around to 0 after 5
    EEPROM.write(0, mode); // Save the new mode back to EEPROM
  }

  // Log the current mode
  Serial.print("Current mode: ");
  Serial.println(mode);

  // Set RGB pins as OUTPUT
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Wait for 1 second before starting
  delay(1000);
}

void loop() {
  // Check for serial input (setmode command)
  checkSerialInput();

  switch (mode) {
    case 0:
      Serial.println("Mode: Normal");
      // Normal mode (fading colors)
      fadeInOut(redPin);
      fadeInOut(greenPin);
      fadeInOut(bluePin);
      fadeInOut(redPin, true); // Yellow
      fadeInOut(greenPin, true); // Cyan
      fadeInOut(bluePin, true); // Magenta
      fadeOut();
      break;

    case 1:
      Serial.println("Mode: Random Mix Up");
      // Random mix up of colors
      randomMixUp();
      break;

    case 2:
      Serial.println("Mode: Blinking Red");
      // Blink red LED
      blinkColor(redPin);
      break;

    case 3:
      Serial.println("Mode: Blinking Green");
      // Blink green LED
      blinkColor(greenPin);
      break;

    case 4:
      Serial.println("Mode: Blinking Blue");
      // Blink blue LED
      blinkColor(bluePin);
      break;

    case 5:
      Serial.println("Mode: Only-White");
      // Only-white mode
      onlyWhite();
      break;

    default:
      Serial.println("Unknown mode");
      break;
  }
}

// Function to fade in/out a color
void fadeInOut(int pin, bool combine) {
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(pin, brightness);
    delay(5);
  }
  if (!combine) {
    delay(1000);
    for (int brightness = 255; brightness >= 0; brightness--) {
      analogWrite(pin, brightness);
      delay(5);
    }
  }
  delay(1000);
}

// Function to fade out all colors
void fadeOut() {
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(redPin, brightness);
    analogWrite(greenPin, brightness);
    analogWrite(bluePin, brightness);
    delay(5);
  }
}

// Function for random mix up mode
void randomMixUp() {
  for (int i = 0; i < 10; i++) {
    int red = random(0, 256);
    int green = random(0, 256);
    int blue = random(0, 256);

    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);

    Serial.print("Random Color -> R: ");
    Serial.print(red);
    Serial.print(", G: ");
    Serial.print(green);
    Serial.print(", B: ");
    Serial.println(blue);

    delay(1000); // Wait before changing the color
  }
}

// Function to blink a specific color
void blinkColor(int pin) {
  for (int i = 0; i < 10; i++) {
    analogWrite(pin, 255);  // Turn on the color
    delay(500);
    analogWrite(pin, 0);    // Turn off the color
    delay(500);
  }
}

// Function for only-white mode
void onlyWhite() {
  analogWrite(redPin, 255);  // Full brightness for Red
  analogWrite(greenPin, 255); // Full brightness for Green
  analogWrite(bluePin, 255);  // Full brightness for Blue
  delay(5000);  // Stay white for 5 seconds
  clearLEDs();
}

// Function to clear all LEDs (turn off)
void clearLEDs() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

// Function to check for "setmode {mode}" command from Serial input
void checkSerialInput() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove any leading/trailing spaces or newlines

    if (input.startsWith("setmode ")) {
      int newMode = input.substring(8).toInt();  // Extract the mode number

      if (newMode >= 0 && newMode <= 5) {
        Serial.print("Setting new mode: ");
        Serial.println(newMode);

        // Save the new mode to EEPROM
        EEPROM.write(0, newMode);

        // Trigger soft reset to restart with new mode
        softReset();
      } else {
        Serial.println("Invalid mode. Mode must be between 0 and 5.");
      }
    }
  }
}

// Function to trigger soft reset (restart the program)
void softReset() {
  Serial.println("Restarting...");
  delay(1000);  // Small delay before restarting
  asm volatile ("jmp 0");  // Jump to start (reset the Arduino)
}
