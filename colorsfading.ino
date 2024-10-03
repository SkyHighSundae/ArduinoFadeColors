const int redPin = 9;    // Pin connected to the Red wire
const int greenPin = 10; // Pin connected to the Green wire
const int bluePin = 11;  // Pin connected to the Blue wire

// Function prototypes
void fadeInOut(int pin, bool combine = false);
void fadeOut();

void setup() {
  // Start Serial communication
  Serial.begin(9600);
  
  // Log the version
  Serial.println("FadeColors - v1.1");
  
  // Set the RGB pins as OUTPUT
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  // Log LED initialization
  Serial.println("Initializing LEDs...");
  
  // Wait for 1 second before starting
  delay(1000);
}

void loop() {
  // Fade in Red
  fadeInOut(redPin);
  
  // Fade in Green
  fadeInOut(greenPin);
  
  // Fade in Blue
  fadeInOut(bluePin);
  
  // Fade in Yellow (Red + Green)
  fadeInOut(redPin, true);
  fadeInOut(greenPin, true);
  
  // Fade in Cyan (Green + Blue)
  fadeInOut(greenPin, true);
  fadeInOut(bluePin, true);
  
  // Fade in Magenta (Red + Blue)
  fadeInOut(redPin, true);
  fadeInOut(bluePin, true);
  
  // Fade out all colors
  fadeOut();
}

void fadeInOut(int pin, bool combine = false) {
  // Fade in
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(pin, brightness); // Set the brightness
    // Log brightness every 51 levels (approximately every 20% of the range)
    if (brightness % 51 == 0) {
      Serial.print("Brightness: ");
      Serial.print(brightness);
      Serial.print(" | Color: ");
      Serial.println(pin == redPin ? "Red" : (pin == greenPin ? "Green" : "Blue"));
    }
    delay(5); // Small delay to create the fade effect
  }

  // If combining colors, do not turn off the pin yet
  if (!combine) {
    delay(1000); // Wait for 1 second at full brightness

    // Fade out
    for (int brightness = 255; brightness >= 0; brightness--) {
      analogWrite(pin, brightness); // Set the brightness
      // Log brightness every 51 levels
      if (brightness % 51 == 0) {
        Serial.print("Brightness: ");
        Serial.print(brightness);
        Serial.print(" | Color: ");
        Serial.println(pin == redPin ? "Red" : (pin == greenPin ? "Green" : "Blue"));
      }
      delay(5); // Small delay to create the fade effect
    }
  } else {
    delay(1000); // Wait for 1 second at full brightness
  }
}

void fadeOut() {
  // Fade out all colors
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(redPin, brightness);
    analogWrite(greenPin, brightness);
    analogWrite(bluePin, brightness);
    
    // Log brightness every 51 levels for all colors
    if (brightness % 51 == 0) {
      Serial.print("Brightness: ");
      Serial.print(brightness);
      Serial.print(" | Color: Red, Green, Blue");
      Serial.println();
    }
    
    delay(5); // Small delay to create the fade effect
  }
}
