// Required library for the LED strip
#include "FastLED.h"           

// --- LED Strip Configuration ---
#define DATA_PIN 3            
#define CLOCK_PIN 2           
#define LED_TYPE DOTSTAR
#define COLOR_ORDER BGR        
const int NUM_LEDS = 16;       // Number of LEDs in the strip
CRGB leds[NUM_LEDS];           // Define the array of LEDs
#define BRIGHTNESS 200         // Set a reasonable brightness

// --- Paint Input Pins ---
const int redPaint = 13;
const int yellowPaint = 12;
const int bluePaint = 11;
const int whitePaint = 10;
const int blackPaint = 9;

// --- Color Utility Function ---
// Function to darken a CRGB color by a scaling factor
CRGB darkenColor(CRGB color, float factor) {
  // factor should be between 0.0 (black) and 1.0 (no change)
  if (factor >= 1.0) return color;
  
  // Scale down the Red, Green, and Blue components
  byte r = (byte)(color.r * factor);
  byte g = (byte)(color.g * factor);
  byte b = (byte)(color.b * factor);
  
  return CRGB(r, g, b);
}

void setup() {
  // Initialize LED strip
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  // Set all paint pins as INPUT
  pinMode(redPaint, INPUT);
  pinMode(yellowPaint, INPUT);
  pinMode(bluePaint, INPUT);
  pinMode(whitePaint, INPUT);
  pinMode(blackPaint, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // Read the state of all "paint" pins 
  int redState = digitalRead(redPaint);
  int yellowState = digitalRead(yellowPaint);
  int blueState = digitalRead(bluePaint);
  int whiteState = digitalRead(whitePaint);
  int blackState = digitalRead(blackPaint);
  
  //Determine the Base Color
  CRGB baseColor = CRGB::Black; 
  int activeColors = 0; // Count active color pins
  int comboKey = 0;
  
  if (redState == HIGH) {
    activeColors++;
    comboKey |= 1 << 0; // Bit 0 for Red
  }
  if (yellowState == HIGH) {
    activeColors++;
    comboKey |= 1 << 1; // Bit 1 for Yellow
  }
  if (blueState == HIGH) {
    activeColors++;
    comboKey |= 1 << 2; // Bit 2 for Blue
  }
  if (whiteState == HIGH) {
    activeColors++;
    comboKey |= 1 << 3; // Bit 3 for White
  }
  
  switch (comboKey) {
    case 0: // None active
      baseColor = CRGB::Black;
      break;
      
    // --- Single Colors ---
    case 1: // Red only (0001)
      baseColor = CRGB::Red;
      break;
    case 2: // Yellow only (0010)
      baseColor = CRGB::Yellow;
      break;
    case 4: // Blue only (0100)
      baseColor = CRGB::Blue;
      break;
    case 8: // White only (1000)
      baseColor = CRGB::White;
      break;
      
    // --- Two-Color Mixes ---
    case 3: // Red + Yellow (0011)
      baseColor = CRGB::Orange;
      break;
    case 5: // Red + Blue (0101)
      baseColor = CRGB::Purple;
      break;
    case 6: // Yellow + Blue (0110)
      baseColor = CRGB::Green;
      break;
      
    // --- Three or More Colors (Magenta Rule) --- 
    // Check for 3 or more active colors using the activeColors count
    default:
      if (activeColors >= 3) {
        baseColor = CRGB::Magenta;
      }
      else if (activeColors == 2) {
          if (comboKey == 9) { // Red + White (1001)
              baseColor = CRGB::HotPink; // A white-tinted Red
          } else if (comboKey == 10) { // Yellow + White (1010)
              baseColor = CRGB::PaleGoldenrod; // A white-tinted Yellow
          } else if (comboKey == 12) { // Blue + White (1100)
              baseColor = CRGB::SkyBlue; // A white-tinted Blue
          } else {
              // Catch-all for any missed 2-color mix.
              // Fall back to a simple average if needed, or define more colors.
              baseColor = CRGB::DimGray; 
          }
      }
      else {
          // This should ideally not be hit if all single colors are handled above.
          baseColor = CRGB::Black; 
      }
      break;
  }
  
  // --- Apply Black Paint Effect ---
  CRGB finalColor;
  
  if (blackState == HIGH) {
    // If Black is active, darken the resulting color.
    // Use a factor of 0.3 for a significant darkening effect.
    finalColor = darkenColor(baseColor, 0.3); 
  } else {
    // If Black is not active, use the base color as is.
    finalColor = baseColor;
  }

  // --- Update LED Strip ---
  // Set all LEDs to the calculated finalColor
  fill_solid(leds, NUM_LEDS, finalColor); 
  FastLED.show();
  
  // --- Serial Output (Kept from original) ---
  Serial.print(redState);
  Serial.print(",");
  Serial.print(yellowState);
  Serial.print(",");
  Serial.print(blueState);
  Serial.print(",");
  Serial.print(whiteState);
  Serial.print(",");
  Serial.println(blackState); 
  
  delay(50);
}