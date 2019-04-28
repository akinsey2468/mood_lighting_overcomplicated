#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(5, 2, NEO_RGB + NEO_KHZ800);
#define TOTAL_LEDS 5
#define CT_PIN 4
void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(9600);
  pinMode(CT_PIN, INPUT_PULLUP);
  pinMode(5, OUTPUT);
}
#define TOTAL_GRADIENTS 5
int gradients[TOTAL_GRADIENTS][2][3] = {{{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
  {{0xFA, 0x70, 0x9A}, {0xFE, 0xE1, 0x40}},
 // {{0x30, 0xcf, 0xd0}, {0x33, 0x08, 0x67}},
 //{{0x5f, 0x72, 0xbd}, {0x9b, 0x23, 0xea}},
  {{0xf4, 0x3b, 0x47}, {0x45, 0x3a, 0x94}},
  //{{0x92, 0xfe, 0x9d}, {0x00, 0xc9, 0xff}},
  {{0xf8, 0x36, 0x00}, {0xf9, 0xd4, 0x23}},
 // {{0x16, 0xa0, 0x85}, {0xf4, 0xd0, 0x3f}},
  {{0x50, 0xcc, 0x7f}, {0xf5, 0xd1, 0x00}}
};
int getMPatIndex(int start, int last, int index, int totalLEDs, bool isPPE) {
  float interval = (float)(last - start) / (float)(totalLEDs - 1);
  float PP = start + (interval * index);
  float MP = pow((PP / 100), 2) * 100;
  //Serial.println(MP / 2.55);
  if(isPPE){
  return MP/2.55;}
  else{
    return PP;
  }
}
void setColors(int gradient[2][3], int totalLEDs) {
  for (int i = 0; i < totalLEDs; i++) {
    pixels.setPixelColor(i, pixels.Color(getMPatIndex(gradient[0][0], gradient[1][0], i, TOTAL_LEDS, 1),
                                         getMPatIndex(gradient[0][1], gradient[1][1], i, TOTAL_LEDS, 1),
                                         getMPatIndex(gradient[0][2], gradient[1][2], i, TOTAL_LEDS, 1)));
  }
}
void loop() {
  pixels.setBrightness(128);
  for (int i = 0; i < TOTAL_GRADIENTS; i++ ) {
    setColors(&gradients[i][0], TOTAL_LEDS);
    pixels.show();
    int b;
    if (i == TOTAL_GRADIENTS - 1) {
      b = 0;
    }
    else {
      b = i + 1;
    }
    while (digitalRead(CT_PIN));
    delay(3);
    digitalWrite(5, HIGH);
    long start = millis();
    int waitTime = 1000;
    while (millis() - start <= waitTime) {
      if (digitalRead(CT_PIN)) {
        digitalWrite(5, LOW);
      }
      int indexValue = (millis() - start);
      int newColor[2][3] = {{getMPatIndex(gradients[i][0][0], gradients[b][0][0], indexValue, waitTime, 0), getMPatIndex(gradients[i][0][1], gradients[b][0][1], indexValue, waitTime, 0), getMPatIndex(gradients[i][0][2], gradients[b][0][2], indexValue, waitTime, 0)},
        {getMPatIndex(gradients[i][1][0], gradients[b][1][0], indexValue, waitTime, 0), getMPatIndex(gradients[i][1][1], gradients[b][1][1], indexValue, waitTime, 0), getMPatIndex(gradients[i][1][2], gradients[b][1][2], indexValue, waitTime, 0)}
      };
      setColors(&newColor[0], TOTAL_LEDS);
      pixels.show();
    }
    while (!digitalRead(CT_PIN));
    delay(3);
    digitalWrite(5, LOW);
    /*Serial.println("r1");
      bool b1 = false, b2 = false, b3 = false;
      while (!(b1 && b2 && b3)) {
      delay(1);
      b1 = digitalRead(CT_PIN);
      delay(1);
      b2 = digitalRead(CT_PIN);
      delay(1);
      b3 = digitalRead(CT_PIN);
      }
      Serial.println("r2");
      b1 = false; b2 = false; b3 = false;
      bool is1 = false;
      bool is0 = false;
      while (!is0&&!is1) {
      for(i=0; i<300; i++){
        Serial.println(digitalRead(4));
        is0 = is0 || !digitalRead(4);
        is1 = is1 || digitalRead(4);
        delay(1);
      }
      }*/
  }
  //setColors(&gradients[1][0], TOTAL_LEDS);
  //pixels.show(); // This sends the updated pixel color to the hardware.
}
