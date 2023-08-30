#include "Adafruit_NeoPixel.h"
#include "TM1637Display.h"
#include "NTPClient.h"
#include "WiFiManager.h"


// Blue LEDs variables
#define LED_PIN_1   25
#define LED_PIN_2   26

// RGB ring variables
#define PIN         17
#define NUMPIXELS   24
#define RED         00
#define GREEN       20
#define BLUE        255
#define RING_BRIGHTNESS 70

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Display variables
#define DISPLAY_CLK   18
#define DISPLAY_DIO   19
#define DISPLAY_BACKLIGHT   1

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

// NTP Client initialization
const char *ssid     = "YOURS_SSID"; 
const char *password = "YOURS_PASSWORD";

int UTC = 2;
const long utcOffsetInSeconds = 3600;
int flag = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds*UTC);


void setup() {
// Turn on Blue LEDs
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  digitalWrite(LED_PIN_1, 1);
  digitalWrite(LED_PIN_2, 1);

// RGB ring initialization
  pixels.begin();
  pixels.setBrightness(RING_BRIGHTNESS);

// Turn on RGB ring
  ring_blue_light();

// Wifi connection
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay (500);
  }
  timeClient.begin();

// Display initialization
  display.setBrightness(DISPLAY_BACKLIGHT);

  uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);
}

void loop() {
// Update the time on display
  timeClient.update();
  display.showNumberDecEx(timeClient.getHours(), 0x40, true, 2, 0);
  display.showNumberDecEx(timeClient.getMinutes(), 0, true, 2, 2);


// Animation every hour
  if(timeClient.getMinutes()== 00 && flag==0) {
    white_light_animation();
    flag=1;
  }
  if(timeClient.getMinutes()>=01) {
    flag=0;
  }

  ring_blue_light();
}

void ring_blue_light() {
  for(int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, RED, GREEN, BLUE);
    pixels.show();
    delay(50);
  }
}

void white_light_animation() {
  int led_values[] = {0, 255, 0, 255};
  int values_count = 2;

  for(int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, 255, 255, 255);
    pixels.show();
    delay(50);
  }

  for(int value=0; value<values_count; value++){
    for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, led_values[value], led_values[value], led_values[value]);
      pixels.show();
    }
    delay(1000);
  }
  delay(4000);
}