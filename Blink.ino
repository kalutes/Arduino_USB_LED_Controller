#include "FastLED.h"

#define NUM_LEDS 1

#define DATA_PIN 9
#define CLOCK_PIN 10

int mode;
int delayTime;
int red;
int green;
int blue;
double redD = 0;
double greenD = 0;
double blueD = 0;

long millisTemp = 0;
bool breatheUp = true;

CRGB leds[NUM_LEDS];

void setup() { 
      Serial.begin(9600);
      FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RBG>(leds, NUM_LEDS);
      leds[0] = CRGB::Black;
      FastLED.show();
}

void loop() { 
 if(Serial.available())
 {
    char temp;
    temp = Serial.read();
    if(temp == '*')
    {
      int modeTemp = Serial.parseInt();
      int delayTimeTemp = Serial.parseInt();
      int redTemp = Serial.parseInt();
      int greenTemp = Serial.parseInt();
      int blueTemp = Serial.parseInt();
      int checkSum = Serial.parseInt();
      temp = Serial.read();
      if(temp == '!' && checkSum == (modeTemp + delayTimeTemp + redTemp + greenTemp + blueTemp))
      {
        breatheUp = true;
        redD = 0;
        greenD = 0;
        blueD = 0;
        mode = modeTemp;
        delayTime = delayTimeTemp;
        red = redTemp;
        green = greenTemp;
        blue = blueTemp;
      }
    }
 }
 displayMode();
}

void displayMode()
{
  switch(mode){
    case 0:
      if(leds[0].red != red || leds[0].green != green || leds[0].blue != blue)
      {
        leds[0].red = red;
        leds[0].green = green;
        leds[0].blue = blue;
        FastLED.show();
      }
      break;
    case 1:
      if(millis() - millisTemp >= delayTime)
      {
          if(leds[0].red == 0 && leds[0].green == 0 && leds[0].blue == 0)
          {
            leds[0].red = red;
            leds[0].green = green;
            leds[0].blue = blue;
            millisTemp = millis();
          }

          else
          {
            leds[0] = CRGB::Black;
            millisTemp = millis();
          }
          FastLED.show();
      }
      break;
    case 2:
      if(millis() - millisTemp >= delayTime)
      {
          millisTemp = millis();
          if(breatheUp)
          {
            if((redD + ((double) red) / 100) <= red && (greenD + ((double) green) / 100) <= green && (blueD + ((double) blue) / 100) <= blue)
            {
              redD += ((double) red) / 100;
              greenD += ((double) green) / 100;
              blueD += ((double) blue) / 100;
              leds[0].red = (int) redD;
              leds[0].green = (int) greenD;
              leds[0].blue = (int) blueD;
            }
            else
            {
              leds[0].red = red;
              leds[0].green = green;
              leds[0].blue = blue;
              breatheUp = false;
            }
          }
          else
          {
            if((redD - ((double) red) / 100) >= 0 && (greenD - ((double) green) / 100) >= 0 && (blueD - ((double) blue) / 100) >= 0)
            {
              redD -= ((double) red) / 100;
              greenD -= ((double) green) / 100;
              blueD -= ((double) blue) / 100;
              leds[0].red = (int) redD;
              leds[0].green = (int) greenD;
              leds[0].blue = (int) blueD;
            }
            else
            {
              leds[0].red = 0;
              leds[0].green = 0;
              leds[0].blue = 0;
              millisTemp += delayTime * 2;
              breatheUp = true;
            }
          }
          FastLED.show();
      }
      break;
    case 3:
      if(millis() - millisTemp >= delayTime)
      {
        if(red < 255 && green == 0 && blue == 0)
        {
          red++;
        }

        else if(blue < 255 && green == 0)
        {
          blue++;
        }

        else if(red > 0 && green == 0)
        {
          red--;
        }

        else if(blue == 255 && green < 255)
        {
          green++;
        }

        else if(green == 255 && blue > 0)
        {
          blue--;
        }

        else if(green == 255 && red < 255)
        {
          red++;
        }

        else if(red == 255 && green > 0)
        {
          green --;
        }
        leds[0].red = red;
        leds[0].green = green;
        leds[0].blue = blue;
        millisTemp = millis();
        FastLED.show();
      }
      break;
    case 4:
      if(millis() - millisTemp >= delayTime)
      {
        red = random(0, 256);
        green = random(0, 256);
        blue = random(0, 256);
        leds[0].red = red;
        leds[0].green = green;
        leds[0].blue = blue;
        millisTemp = millis();
        FastLED.show();
      }
      break;
  }
}

