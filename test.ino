#include "Adafruit_NeoPixel.h"
//基地台使用
#include <LWiFi.h>

#ifdef __AVR__
#include <avr/power.h>
#endif
//oled
#include <Wire.h>
#include <SeeedOLED.h>

// Led pin
#define PIN 2

// My wifi
char ssid[] = "e520";    // your network SSID (name)
char pass[] = "nkfuste520wifi"; // your network password
int keyIndex = 0;         // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80); // set 80 port

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup()
{
  //rgb led
  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
  audi();
  setRGBmode("waiting");
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //oled
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  setUpOled();
  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("[init]Setup..."); //Print the String

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined(__AVR_ATtiny85__)
  if (F_CPU == 16000000)
  {
    clock_prescale_set(clock_div_1);
  }
#endif
  // End of trinket special code
  // wifi
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
  setRGBmode("warm");
  delay(500);
  setRGBmode("success");
  delay(1000);
}

void loop()
{
  rainbowCycle(20);
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void setUpOled() {
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
}
void printWifiStatus()
{
  SeeedOled.clearDisplay();
  // print the SSID of the network you're attached to:
  SeeedOled.setTextXY(1, 0);          //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("SSID:");       //Print the String
  SeeedOled.putString(WiFi.SSID());   //Print the String
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  SeeedOled.setTextXY(2, 0);          //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("IP:");       //Print the String
  IPAddress ip = WiFi.localIP();
  String myIP = String() + ip[0] + "." + ip[1] + "." + ip[2] + "." + ip[3];
  char Buf[50];
  myIP.toCharArray(Buf, 50);
  SeeedOled.setTextXY(3, 0);          //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString(Buf);   //Print the String
  Serial.print("IP Address: ");
  Serial.println(myIP);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
void setRGBmode(String mode) {
  int NUMPIXELS = 30;
  if (mode == "success") {
    for (int i = 0; i < NUMPIXELS; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      strip.setPixelColor(i, strip.Color(0, 150, 0)); // Moderately bright green color.
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  } else if ( mode == "waiting")
  {
    for (int i = 0; i < NUMPIXELS; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      strip.setPixelColor(i, strip.Color(255, 150, 0)); // Moderately bright green color.
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  }else if ( mode == "warm")
  {
    for (int i = 0; i < NUMPIXELS; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      strip.setPixelColor(i, strip.Color(246, 205, 139)); // Moderately bright green color.
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  }

}
void showAudiLight(String logic) {
  char Buf[30];
  logic.toCharArray(Buf, 30);
  for (int l = 0; l < 30; l++) {
    if (Buf[l] == '1') {
      strip.setPixelColor(l, strip.Color(67, 228, 217));
    } else if (Buf[l] == '0') {
      strip.setPixelColor(l, strip.Color(0, 0, 0));
    }
  }
  strip.show();
  delay(30);
}
void showRedAudiLight(String logic) {
  char Buf[30];
  logic.toCharArray(Buf, 30);
  for (int l = 0; l < 30; l++) {
    if (Buf[l] == '1') {
      strip.setPixelColor(l, strip.Color(255, 0, 0));
    } else if (Buf[l] == '0') {
      strip.setPixelColor(l, strip.Color(0, 0, 0));
    }
  }
  strip.show();
  delay(30);
}
void audi() {
  int NUMPIXELS = 30;
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(67, 228, 217)); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(30);
  }
  for (int i = NUMPIXELS; i > 0; i--) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(30);
  }
  showAudiLight("111000000000000000000000000000");
  showAudiLight("011100000000000000000000000000");
  showAudiLight("001110000000000000000000000000");
  showAudiLight("000111000000000000000000000000");
  showAudiLight("000011100000000000000000000000");
  showAudiLight("000001110000000000000000000000");
  showAudiLight("000000111000000000000000000000");
  showAudiLight("000000011100000000000000000000");
  showAudiLight("000000001110000000000000000000");
  showAudiLight("000000000111000000000000000000");
  showAudiLight("000000000011100000000000000000");
  showAudiLight("000000000001110000000000000000");
  showAudiLight("000000000000111000000000000000");
  showAudiLight("000000000000011100000000000000");
  showAudiLight("000000000000001110000000000000");
  showAudiLight("000000000000000111000000000000");
  showAudiLight("000000000000000011100000000000");
  showAudiLight("000000000000000001110000000000");
  showAudiLight("000000000000000000111000000000");
  showAudiLight("000000000000000000011100000000");
  showAudiLight("000000000000000000001110000000");
  showAudiLight("000000000000000000000111000000");
  showAudiLight("000000000000000000000011100000");
  showAudiLight("000000000000000000000001110000");
  showAudiLight("000000000000000000000000111000");
  showAudiLight("000000000000000000000000011100");
  showAudiLight("000000000000000000000000001110");
  showAudiLight("000000000000000000000000000111");
  showAudiLight("111000000000000000000000000111");
  showAudiLight("011100000000000000000000000111");
  showAudiLight("001110000000000000000000000111");
  showAudiLight("000111000000000000000000000111");
  showAudiLight("000011100000000000000000000111");
  showAudiLight("000001110000000000000000000111");
  showAudiLight("000000111000000000000000000111");
  showAudiLight("000000011100000000000000000111");
  showAudiLight("000000001110000000000000000111");
  showAudiLight("000000000111000000000000000111");
  showAudiLight("000000000011100000000000000111");
  showAudiLight("000000000001110000000000000111");
  showAudiLight("000000000000111000000000000111");
  showAudiLight("000000000000011100000000000111");
  showAudiLight("000000000000001110000000000111");
  showAudiLight("000000000000000111000000000111");
  showAudiLight("000000000000000011100000000111");
  showAudiLight("000000000000000001110000000111");
  showAudiLight("000000000000000000111000000111");
  showAudiLight("000000000000000000011100000111");
  showAudiLight("000000000000000000001110000111");
  showAudiLight("000000000000000000000111000111");
  showAudiLight("000000000000000000000011100111");
  showAudiLight("000000000000000000000001110111");
  showAudiLight("111000000000000000000000111111");
  showAudiLight("011100000000000000000000111111");
  showAudiLight("001110000000000000000000111111");
  showAudiLight("000111000000000000000000111111");
  showAudiLight("000011100000000000000000111111");
  showAudiLight("000001110000000000000000111111");
  showAudiLight("000000111000000000000000111111");
  showAudiLight("000000011100000000000000111111");
  showAudiLight("000000001110000000000000111111");
  showAudiLight("000000000111000000000000111111");
  showAudiLight("000000000011100000000000111111");
  showAudiLight("000000000001110000000000111111");
  showAudiLight("000000000000111000000000111111");
  showAudiLight("000000000000011100000000111111");
  showAudiLight("000000000000001110000000111111");
  showAudiLight("000000000000000111000000111111");
  showAudiLight("000000000000000011100000111111");
  showAudiLight("000000000000000001110000111111");
  showAudiLight("000000000000000000111000111111");
  showAudiLight("000000000000000000011100111111");
  showAudiLight("000000000000000000001110111111");
  showAudiLight("000000000000000000000111111111");
  showAudiLight("111000000000000000000111111111");
  showAudiLight("011100000000000000000111111111");
  showAudiLight("001110000000000000000111111111");
  showAudiLight("000111000000000000000111111111");
  showAudiLight("000011100000000000000111111111");
  showAudiLight("000001110000000000000111111111");
  showAudiLight("000000111000000000000111111111");
  showAudiLight("000000011100000000000111111111");
  showAudiLight("000000001110000000000111111111");
  showAudiLight("000000000111000000000111111111");
  showAudiLight("000000000011100000000111111111");
  showAudiLight("000000000001110000000111111111");
  showAudiLight("000000000000111000000111111111");
  showAudiLight("000000000000011100000111111111");
  showAudiLight("000000000000001110000111111111");
  showAudiLight("000000000000000111000111111111");
  showAudiLight("000000000000000011100111111111");
  showAudiLight("000000000000000001110111111111");
  showAudiLight("000000000000000000111111111111");
  showAudiLight("111000000000000000111111111111");
  showAudiLight("011100000000000000111111111111");
  showAudiLight("001110000000000000111111111111");
  showAudiLight("000111000000000000111111111111");
  showAudiLight("000011100000000000111111111111");
  showAudiLight("000001110000000000111111111111");
  showAudiLight("000000111000000000111111111111");
  showAudiLight("000000011100000000111111111111");
  showAudiLight("000000001110000000111111111111");
  showAudiLight("000000000111000000111111111111");
  showAudiLight("000000000011100000111111111111");
  showAudiLight("000000000001110000111111111111");
  showAudiLight("000000000000111000111111111111");
  showAudiLight("000000000000011100111111111111");
  showAudiLight("000000000000001110111111111111");
  showAudiLight("000000000000000111111111111111");
  showAudiLight("111000000000000111111111111111");
  showAudiLight("011100000000000111111111111111");
  showAudiLight("001110000000000111111111111111");
  showAudiLight("000111000000000111111111111111");
  showAudiLight("000011100000000111111111111111");
  showAudiLight("000001110000000111111111111111");
  showAudiLight("000000111000000111111111111111");
  showAudiLight("000000011100000111111111111111");
  showAudiLight("000000001110000111111111111111");
  showAudiLight("000000000111000111111111111111");
  showAudiLight("000000000011100111111111111111");
  showAudiLight("000000000001110111111111111111");
  showAudiLight("000000000000111111111111111111");
  showAudiLight("111000000000111111111111111111");
  showAudiLight("011100000000111111111111111111");
  showAudiLight("001110000000111111111111111111");
  showAudiLight("000111000000111111111111111111");
  showAudiLight("000011100000111111111111111111");
  showAudiLight("000001110000111111111111111111");
  showAudiLight("000000111000111111111111111111");
  showAudiLight("000000011100111111111111111111");
  showAudiLight("000000001110111111111111111111");
  showAudiLight("000000000111111111111111111111");
  showAudiLight("111000000111111111111111111111");
  showAudiLight("011100000111111111111111111111");
  showAudiLight("001110000111111111111111111111");
  showAudiLight("000111000111111111111111111111");
  showAudiLight("000011100111111111111111111111");
  showAudiLight("000001110111111111111111111111");
  showAudiLight("000000111111111111111111111111");
  showAudiLight("111000111111111111111111111111");
  showAudiLight("011100111111111111111111111111");
  showAudiLight("001110111111111111111111111111");
  showAudiLight("000111111111111111111111111111");
  showAudiLight("111111111111111111111111111111");

  showRedAudiLight("000000000000000000000000000000");
  showRedAudiLight("000000000000001100000000000000");
  showRedAudiLight("000000000000011110000000000000");
  showRedAudiLight("000000000000111111000000000000");
  showRedAudiLight("000000000001111111100000000000");
  showRedAudiLight("000000000011111111110000000000");
  showRedAudiLight("000000000111111111111000000000");
  showRedAudiLight("000000001111111111111100000000");
  showRedAudiLight("000000011111111111111110000000");
  showRedAudiLight("000000111111111111111111000000");
  showRedAudiLight("000001111111111111111111100000");
  showRedAudiLight("000011111111111111111111110000");
  showRedAudiLight("000111111111111111111111111000");
  showRedAudiLight("001111111111111111111111111100");
  showRedAudiLight("011111111111111111111111111110");
  showRedAudiLight("111111111111111111111111111111");


  for (int i = NUMPIXELS; i > 0; i--) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(30);
  }
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(67, 228, 217)); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(30);
  }
  for (int i = 0; i <= 40; i++) {
    for (int j = 0; j < NUMPIXELS; j++) {
      strip.setPixelColor(j, strip.Color(67 - 40 + i, 228 - 40 + i, 217 - 40 + i)); // Moderately bright green color.
    }
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(30);
  }
  delay(300);
}
