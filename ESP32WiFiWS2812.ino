#include <WiFi.h>
#include "ws2812.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

const int DATA_PIN = 15;
const uint16_t NUM_PIXELS = 100;
rgbVal *pixels;
char command = 1;
bool light = 1;
const char* ssid     = "TP-LINK_346D92";
const char* password = "1F346D92";
//const char* ssid     = "Vodafone27";
//const char* password = "KONGIRENE102777";

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  ws2812_init(DATA_PIN, LED_WS2812);
  pixels = (rgbVal*)malloc(sizeof(rgbVal) * NUM_PIXELS);
  displayOff();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
  else
  {
    displayOff();
  }
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("connection lost..."));
  display.display();      // Show initial text
    delay(500);
  }
   Serial.println("");
   Serial.println("WiFi connected.");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   display.clearDisplay();
   display.setTextSize(2); // Draw 2X-scale text
   display.setTextColor(WHITE);
   display.setCursor(10, 0);
   display.println(WiFi.localIP());
   display.display();      // Show initial text
   server.begin();
}

void loop() {

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    String req = client.readStringUntil('\r');
    client.flush();
    if (req.indexOf("/on") != -1) {
      command = 1;
      light = 0;
    }
    else if (req.indexOf("/off") != -1) {
      command = 2;
      light = 0;
    }
    else{
      displayOff();
    }
  }


  if (command == 1 && light == 0) {
    displayOff();
    for (int i = 0; i < 10; i++) {
      pixels[i] = makeRGBVal (255, 255, 255);
      turnon();
    }
    ws2812_setColors(NUM_PIXELS, pixels);
    light = 1;
  }
  if (command == 2 && light == 0) {
    displayOff();
    turnoff();
    light = 1;
  }
}

void displayOff() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels[i] = makeRGBVal(0, 0, 0);
  }
  ws2812_setColors(NUM_PIXELS, pixels);
}

void turnon(void) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("LED ON"));
  display.display();      // Show initial text
  delay(100);
}

void turnoff(void) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("LED OFF"));
  display.display();      // Show initial text
  delay(100);
}
