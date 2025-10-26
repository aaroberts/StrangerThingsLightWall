include "FastLED.h"// This is the library that does all the work with the LEDs
define NUM_LEDS 28 // The number of LEDs in the string
define RELAY_PIN 3 // This is if you want to add a relay to pin 3 to turn on or off a mains voltage for plug in christmas lights (like the ones strung across the Byer's living room
define DATA_PIN 2 // The PIN that connects to the data signal on the LEDs
define RX1 3 //RX1 - This PIN goes to the TX PIN of the ESP32
define TX1 1 //TX1 - This PIN goes to the RX PIN of the ESP32
CRGB leds[NUM_LEDS]; // Define array with NUM_LEDS elements
CRGB led_color[NUM_LEDS]= // Define array to store LED colors

//**/
{
// NOTE: I had to match my LEDs to the C9 Replacement Christmas Light Bulb covers (since I accidentally installed these first) If you match the colors
// to the bulb cover, it makes for a deep color.
led_color[0] = CRGB::Yellow,
led_color[1] = CRGB::Orange,
led_color[2] = CRGB::Red,
led_color[3] = CRGB::Green,
led_color[4] = CRGB::Blue,
led_color[5] = CRGB::Yellow,
led_color[6] = CRGB::Orange,
led_color[7] = CRGB::Red,
led_color[8] = CRGB::Green,
led_color[9] = CRGB::Blue,
led_color[10] = CRGB::Yellow,
led_color[11] = CRGB::Orange,
led_color[12] = CRGB::Red,
led_color[13] = CRGB::Green,
led_color[14] = CRGB::Blue,
led_color[15] = CRGB::Yellow,
led_color[16] = CRGB::Orange,
led_color[17] = CRGB::Red,
led_color[18] = CRGB::Blue,
led_color[19] = CRGB::Green,
led_color[20] = CRGB::Yellow,
led_color[21] = CRGB::Orange,
led_color[22] = CRGB::Red,
led_color[23] = CRGB::Green,
led_color[24] = CRGB::Blue,
led_color[25] = CRGB::Yellow,
led_color[26] = CRGB::Orange,
led_color[27] = CRGB::Red,
};

void setup()
{
Serial.begin(2400); // start serial peripheral
//NOTE: I had to lower the baud rate to 2400 because data was getting corrupted at higher speeds.
//I assume because with a breadbord there's lots of noise. This isn't a problem because each letter lights up fairly slowly.

FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS); // instantiate LED library with our information
FastLED.clear(); // clear the LED display before doing anything else
pinMode(RELAY_PIN, OUTPUT);
RGB_Blink();
chase();
allon();

}

void loop()
{
if(Serial.available()) // If there is serial data waiting in the buffer
{
ascii2ledIndex(Serial.read()); // read the data and convert each letter from ascii to the appropriate LED index in the string
}
}

// Convert a letter from ascii to the appropriate LED index in the string
void ascii2ledIndex(int asciiLetter)
{
Serial.println(asciiLetter);
int ledIndex = -1; // create variable to store led index
int indexMap[]={20,21,22,23,24,25,26,27,18,17,16,15,14,13,12,11,10,0,1,2,3,4,5,6,7,8}; // When I hung the LED string on the wall the first letters in the string are R S T U V W X Y Z then it skips one LED and winds around to Q P O N M L K J I then it skips one LED and winds to A B C D E F G H

if(asciiLetter &gt;= 97 &amp;&amp; asciiLetter &lt;= 122) // It's an ASCII Lowercase Letter
&#123;
  ledIndex = indexMap[asciiLetter - 97];
&#125;
else if (asciiLetter &gt;= 65 &amp;&amp; asciiLetter &lt;= 90) // It's an ASCII Uppercase Letter
&#123;
  ledIndex = indexMap[asciiLetter - 65];
&#125;
Serial.println(ledIndex);

if(ledIndex &gt;= 0) // valid LED index converted from either uppercase or lowercase letter
&#123;
  digitalWrite(RELAY_PIN, HIGH);
  FastLED.clear();
  leds[ledIndex] = led_color[ledIndex];      
  FastLED.show();
  delay(700);
  FastLED.clear();
  FastLED.show();
  delay(500);
&#125;
else // invalid led index. display blank. this is useful for spaces and other punctuation. 
&#123;
  digitalWrite(RELAY_PIN, HIGH);
  FastLED.clear();
  FastLED.show();
  delay(900);
  Serial.println(&quot;Invalid LED Index&quot;);
&#125;

if (asciiLetter == 44) // , for all on
&#123;
  allon();
&#125;

if (asciiLetter == 46) // . for all off
&#123;
  alloff();
&#125;

if (asciiLetter == 49) // 1 for chase
&#123;
  chase();
&#125;

if(asciiLetter == 50) // 2 for Triple Flash
&#123;
  fastFlash();
  delay(100);
  fastFlash();
  delay(100);
  fastFlash();
&#125;

if(asciiLetter == 51) // 3 for Seven Flashes
&#123;
  fastFlash();
  delay(100);
  fastFlash();
  delay(50);
  fastFlash();
  delay(75);
  fastFlash();
  delay(100);
  fastFlash();
  delay(60);
  fastFlash();
  delay(30);
  fastFlash();
&#125;    
}

// Function that turns all lights on. The normal state of a set of Christmas lights.
void allon()
{
for(int dot = 0; dot < NUM_LEDS; dot++)
{
leds[dot] = led_color[dot];
FastLED.show();
digitalWrite(RELAY_PIN, LOW);
}
}
// Turn all bulbs off - like before you send a message
void alloff()
{
for(int dot = 0; dot < NUM_LEDS; dot++)
{
leds[dot] = CRGB::Black;
FastLED.show();
digitalWrite(RELAY_PIN, HIGH);
}
FastLED.clear();
}

// Test function that displays one LED at a time. This appears like the LED is chasing down the string.
void chase()
{
for(int dot = 0; dot < NUM_LEDS; dot++)
{
leds[dot] = led_color[dot];
FastLED.show();
// clear this led for the next time around the loop
leds[dot] = CRGB::Black;
delay(50);
}
FastLED.clear();
}

// Test function that blinks all the LEDs
void RGB_Blink()
{
// Set all the colors
leds[0] = led_color[0];
leds[1] = led_color[1];
leds[2] = led_color[2];
leds[3] = led_color[3];
leds[4] = led_color[4];
leds[5] = led_color[5];
leds[6] = led_color[6];
leds[7] = led_color[7];
leds[8] = led_color[8];
leds[9] = led_color[9];
leds[10] = led_color[10];
leds[11] = led_color[11];
leds[12] = led_color[12];
leds[13] = led_color[13];
leds[14] = led_color[14];
leds[15] = led_color[15];
leds[16] = led_color[16];
leds[17] = led_color[17];
leds[18] = led_color[18];
leds[19] = led_color[19];
leds[20] = led_color[20];
leds[21] = led_color[21];
leds[22] = led_color[22];
leds[23] = led_color[23];
leds[24] = led_color[24];
leds[25] = led_color[25];
leds[26] = led_color[26];
leds[27] = led_color[27];
FastLED.show();
delay(200);
// Now turn the LED off, then pause
digitalWrite(RELAY_PIN, LOW);
FastLED.clear();
FastLED.show();
delay(200);
digitalWrite(RELAY_PIN, HIGH);
}

void fastFlash()
{
// Set all the colors
leds[0] = led_color[0];
leds[1] = led_color[1];
leds[2] = led_color[2];
leds[3] = led_color[3];
leds[4] = led_color[4];
leds[5] = led_color[5];
leds[6] = led_color[6];
leds[7] = led_color[7];
leds[8] = led_color[8];
leds[9] = led_color[9];
leds[10] = led_color[10];
leds[11] = led_color[11];
leds[12] = led_color[12];
leds[13] = led_color[13];
leds[14] = led_color[14];
leds[15] = led_color[15];
leds[16] = led_color[16];
leds[17] = led_color[17];
leds[18] = led_color[18];
leds[19] = led_color[19];
leds[20] = led_color[20];
leds[21] = led_color[21];
leds[22] = led_color[22];
leds[23] = led_color[23];
leds[24] = led_color[24];
leds[25] = led_color[25];
leds[26] = led_color[26];
leds[27] = led_color[27];
FastLED.show();
delay(30);
digitalWrite(RELAY_PIN, LOW);
FastLED.clear();
delay(30);
FastLED.show();
digitalWrite(RELAY_PIN, HIGH);
}
