/*
    magic_staff

    This generates different RGB LED effects intended to light the ball on top of
    a magic staff. The effects are selected by gestures, and gestures are detected
    by an accelerometer.

    The circuit:
      Adafruit Feather 32U
      RGB LED on PWM pins 9, 10, 11

    Created 8 September 2019
    by Mike Combs, mike@combsnet.com
*/

#include <AsyncDelay.h>
#include "RGBConverter.h"
#include "hsl_colors.h"
#include "rgb_colors.h"


RGBConverter ledMath = RGBConverter();

// board settings
const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;
const bool COMMON_ANODE = true; // true (anode) = tie to 3.3V, false (cathode) = tie to GND


// pattern config
bool firstPass = true;
AsyncDelay delay_2s;
int state = 1;
int maxState = 3;
bool stateChanged = true;


void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting program.");
  delay_2s.start(2000, AsyncDelay::MILLIS);
}


void loop() {
  unsigned long currentMillis = millis();
  byte rgb[3] = {0, 0, 0};
  double hsl[3] = {0.0, 0.0, 0.0};

  if (firstPass == true) {
    // First run tests
    firstPass = false;

    Serial.print("hsl_red = ");
    PrintHsl(hsl_red);
    ledMath.hslToRgb(hsl_red[0], hsl_red[1], hsl_red[2], rgb);
    PrintRgb(rgb);

    Serial.print("hsl_blue = ");
    PrintHsl(hsl_blue);
    ledMath.hslToRgb(hsl_blue[0], hsl_blue[1], hsl_blue[2], rgb);
    PrintRgb(rgb);

    Serial.print("hsl_green = ");
    PrintHsl(hsl_green);
    ledMath.hslToRgb(hsl_green[0], hsl_green[1], hsl_green[2], rgb);
    PrintRgb(rgb);

    Serial.println("--");
  }

  if (stateChanged == true) {
    stateChanged = false;
    if (state == 1) {
      Serial.print("do it 1: ");
      SetRgbLed(rgb_red);
    } else if (state == 2) {
      Serial.print("do it 2: ");
      SetRgbLed(rgb_green);
    } else if (state == 3) {
      Serial.print("do it 3: ");
      SetRgbLed(rgb_blue);
    }
  }

  if (delay_2s.isExpired()) {
    Serial.print("Delay millis=");
    Serial.println(millis());
    stateChanged = true;
    state = state + 1;
    if (state > maxState) {
      state = 1;
    }
    delay_2s.repeat(); // Count from when the delay expired, not now
  }

}

void PrintHsl(double hsl[]) {
  Serial.print("hsl is ");
  Serial.print("hue=");
  Serial.print(hsl[0], 2);
  Serial.print(", sat=");
  Serial.print(hsl[1], 2);
  Serial.print(", lum=");
  Serial.println(hsl[2], 2);
}

void PrintRgb(byte rgb[]) {
  Serial.print("rgb is ");
  Serial.print("red=");
  Serial.print(rgb[0]);
  Serial.print(", green=");
  Serial.print(rgb[1]);
  Serial.print(", blue=");
  Serial.println(rgb[2]);
}

void SetRgbLed(byte rgb[]) {
  byte r, g, b;
  PrintRgb(rgb);
  if (COMMON_ANODE == false) {
    r = rgb[0];
    g = rgb[1];
    b = rgb[2];
  } else {
    r = 255 - rgb[0];
    g = 255 - rgb[1];
    b = 255 - rgb[2];    
  }
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void SetHslLed(double hsl[]) {
  PrintHsl(hsl);
  byte rgb[3] = {0, 0, 0};
  ledMath.hslToRgb(hsl[0], hsl[1], hsl[2], rgb);
  SetRgbLed(rgb);
}
