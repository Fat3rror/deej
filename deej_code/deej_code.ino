//This code uses a 128*64 oled display and 5 potentiometers / sliders by default
//This is a modified version from the code from: https://github.com/omriharel/deej

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET); //A4 SDA //A5 SCK

const int NUM_SLIDERS = 5;
int gap=5; //gap between beams
int beam = round((128-((NUM_SLIDERS+1)*gap))/NUM_SLIDERS); //horizontal length of a beam

const int analogInputs[NUM_SLIDERS] = {A0,A1,A2,A3,A6};

int analogSliderValues[NUM_SLIDERS];

void setup() { 
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.display();
  display.setTextColor(WHITE);
  delay(500);
  display.clearDisplay();

  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  display.clearDisplay();
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }

    display.drawRect(beam*i+gap*(i+1), 0, beam, 64, WHITE);
    int currentval = map(analogSliderValues[i],0,1024,64,0); //Nur für das displayen (invertiert)
    display.fillRect(beam*i+gap*(i+1),currentval,beam,64-currentval, WHITE);
    


  }
  display.display();
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}