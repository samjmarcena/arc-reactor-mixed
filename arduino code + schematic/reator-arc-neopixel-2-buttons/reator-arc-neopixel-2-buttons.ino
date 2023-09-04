#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN_1   7
#define BUTTON_PIN_2   8
#define PIXEL_PIN_1    2
#define PIXEL_PIN_2    3
#define PIXEL_COUNT_1    10 // circulo externo
#define PIXEL_COUNT_2    7 // circulo interno


Adafruit_NeoPixel strip1(PIXEL_COUNT_1, PIXEL_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(PIXEL_COUNT_2, PIXEL_PIN_2, NEO_GRB + NEO_KHZ800);

boolean oldState1 = HIGH;
boolean oldState2 = HIGH;
int mode1 = 0;
int mode2 = 0;

void setup() {
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  strip1.begin();
  strip2.begin();
  strip1.show();
  strip2.show();
}

void loop() {
  boolean newState1 = digitalRead(BUTTON_PIN_1);
  boolean newState2 = digitalRead(BUTTON_PIN_2);

  if (newState1 == LOW && oldState1 == HIGH) {
    mode1 = (mode1 + 1) % 4;
    handleMode(strip1, mode1);
  }
  oldState1 = newState1;

  if (newState2 == LOW && oldState2 == HIGH) {
    mode2 = (mode2 + 1) % 4;
    handleMode(strip2, mode2);
  }
  oldState2 = newState2;
}

void handleMode(Adafruit_NeoPixel &strip, int mode) {
  switch (mode) {
    case 0:
      colorWipe(strip, strip.Color(0, 0, 0), 50);
      break;
    case 1:
      colorWipe(strip, strip.Color(0, 0, 255), 50);
      break;
    case 2:
      theaterChase(strip, strip.Color(0, 0, 255), 50);
      break;
    case 3:
      fade(strip, strip.Color(0, 0, 255));
      break;
  }
}

void colorWipe(Adafruit_NeoPixel &strip, uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void theaterChase(Adafruit_NeoPixel &strip, uint32_t color, int wait) {
  for (int a = 0; a < 500; a++) {
    for (int b = 0; b < 3; b++) {
      strip.clear();
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);
      }
      strip.show();
      delay(wait);
    }
  }
}

void fade(Adafruit_NeoPixel &strip, uint32_t color) {
  int steps = 100;
  for (int a = 0; a < 500; a++) {
    for (int s = 0; s <= steps; s++) {
      uint8_t r = (uint8_t)((color >> 16) & 0xFF);
      uint8_t g = (uint8_t)((color >> 8) & 0xFF);
      uint8_t b = (uint8_t)(color & 0xFF);
      r = (r * s) / steps;
      g = (g * s) / steps;
      b = (b * s) / steps;
      uint32_t fadeColor = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
      colorWipe(strip, fadeColor, 1);
    }
  }
}
