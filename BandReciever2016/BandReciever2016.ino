#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif


int drum = 4;           //Change this variable depending on the drum
//use this line for the drum light show in order to treat these as one long strip
//int offset =20 * drum; 
int offset = 0; //specifically for chris's proposal

// total number of leds used
//int nLEDs = 180;
int nLEDs = 20; //specifically for chris's proposal

RF24 radio(8, 9);

const byte rxAddr[6] = "00001";

int dataPin  = 7;
int clockPin = 6;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);
volatile int pattern = 0;
int num = 0;
volatile int quit = 0;
int flag = 0;
int parity = 0;


void setup(){
  
  attachInterrupt(0, interrupt, FALLING);
  strip.begin();
  strip.show();
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
  randomSeed(analogRead(0));
}

void interrupt(){
  char text[6] = {0};
  radio.read(&text, sizeof(text));
  String str(text);
  num = str.toInt();
  Serial.println(num);

  if(num != pattern){
    pattern = num;
    quit = 1;
  }
  

  
}
/*
void loop(){
  switch(pattern){
    case 0:
      colorWipe(strip.Color(127, 0, 0), 30);
      break;
    case 1:
      colorChaseThreeOneDrum(strip.Color(127,   0,   127), 45);
      break;
    case 2:
      colorChase(strip.Color(127, 127, 0), 40);
      break;
    case 3:
      cycloneScroll();
      break;
    case 4:
      flashRandom();
      break;
    case 5:
      colorWipeRandom(30);
      break;
    case 6:
      theaterChase(strip.Color(127,   50,   50), 50);
      break;
    case 7:
      colorChaseThree(strip.Color(50,   70,   80), 35);
      break;
    case 8:
      colorWipe(strip.Color(100, 50, 0), 30);
      break;
    case 9:
      theaterChaseRainbow(50);
      break;
    case 10:
      colorWipe(strip.Color(random(128), random(128), random(128)), 30);
      break;
      
    default:
      stripClear;
      break;
  }
}
*/

void loop(){
  int waittime = 100;
  switch(pattern){
    case 0: 
      if(drum == 0){
        colorWipeHold(strip.Color(127, 127, 127), waittime);
      }
      break;
    case 1:
      if(drum == 1){
        colorWipeHold(strip.Color(127, 127, 127), waittime);
      }
      break;
    case 2:
      if(drum == 2){
        colorWipeHold(strip.Color(127, 127, 127), waittime);
      }
      break;
    case 3:
      if(drum == 3){
        colorWipeHold(strip.Color(127, 127, 127), waittime);
      }
      break;
    case 4:
      if(drum == 4){
        fillFromMiddle(strip.Color(127, 127, 127), waittime * 2);
      }
      break;
    case 129:
      stripClear();
      break;
    default:
      break;
  }
}



//This is specifically for Chris's proposal
void fillFromMiddle(uint32_t c, uint8_t wait){
  int lmid = 9;
  int hmid = 10;

  for(int i = 0; i < 10; i++){
    strip.setPixelColor(lmid - i, c);
    strip.setPixelColor(hmid + i, c);
    strip.show();
    delay(wait);
  }
}



// Three yellow, three red, three yellow. then switch the colors
void cycloneScroll(){
  quit = 0;
  parity++; 
  parity %= 2;

  if(drum < 3){
    do{stripClear();}while(0);
    for (int i = 0; i < strip.numPixels(); i++) {
      if(quit == 0){
        if(parity == 0){strip.setPixelColor(i - offset, 127, 0, 0);}
        else      {strip.setPixelColor(i - offset, 100, 50, 0);}
        strip.show();
        delay(10);
      }
      else{
        stripClear();
        return;
      }
    }
  }
  else if(drum > 5){
     do{stripClear();}while(0);
     for (int i=0; i < strip.numPixels(); i++) {
      if(quit == 0){
        if(parity == 0){strip.setPixelColor(i - offset + 120, 127, 0, 0);}
        else      {strip.setPixelColor(i - offset + 120, 100, 50, 0);}
        strip.show();
        delay(10);
      }
      else{
        stripClear();
        return;
      }
    }
  }
  else if (drum > 2 && drum < 6){
    do{stripClear();}while(0);
    for (int i=0; i < strip.numPixels(); i++) {
      if(quit == 0){
        if(parity == 0){strip.setPixelColor(i - offset + 60, 100, 50, 0);}
        else      {strip.setPixelColor(i - offset + 60, 127, 0, 0);}
        strip.show();
        delay(10);
      }
      else{
        stripClear();
        return;
      }
    }
  } 
}



// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  quit = 0;

  do{stripClear();} while(0);
  for (i=0; i < strip.numPixels() + 20; i++) {
    if(quit == 0){
    strip.setPixelColor(i-offset, c);
    strip.show();
    delay(wait);
    }
    else if(quit == 1){
      stripClear();
      return;
    }
  }
}



void colorWipeHold(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


void colorWipeRandom(uint8_t wait) {
  int i;
  quit = 0;

  do{stripClear();} while(0);
  for (i=0; i < strip.numPixels() + 20; i++) {
    if(quit == 0){
    strip.setPixelColor(i-offset, strip.Color(random(128), random(128), random(128)));
    strip.show();
    delay(wait);
    }
    else if(quit == 1){
      stripClear();
      return;
    }
  }
}


void colorChase(uint32_t c, uint8_t wait) {
  int i;
  quit = 0;
  
  for(i=0; i<strip.numPixels(); i++) {
    if(quit == 0){
      strip.setPixelColor(i-offset, c); // Set new pixel 'on'
      strip.show();              // Refresh LED states
      strip.setPixelColor(i-offset, 0); // Erase pixel, but don't refresh!
      delay(wait);
    }
    else{
      stripClear();
    return;
    } 
  }

  strip.show(); // Refresh to turn off last pixel
}


void colorChaseThree(uint32_t c, uint8_t wait) {
  int i;
  quit = 0;
  
  for(i=0; i < strip.numPixels(); i++) {
    if(quit == 0){
      strip.setPixelColor(i - offset, c); // Set new pixel 'on'
      strip.setPixelColor(i - offset - 1, c);
      strip.setPixelColor(i - offset - 2, c);
      strip.setPixelColor(i - offset - 3, 0); // Erase pixel, but don't refresh!
      strip.show();              // Refresh LED states
      delay(wait);
    }
    else{
      stripClear();
    return;
    } 
  }

  strip.show(); // Refresh to turn off last pixel
}


void colorChaseThreeOneDrum(uint32_t c, uint8_t wait) {
  int i;
  quit = 0;
  
  for(i=0; i < 23; i++) {
    if(quit == 0){
      strip.setPixelColor(i, c); // Set new pixel 'on'
      strip.setPixelColor(i - 1, c);
      strip.setPixelColor(i - 2, c);
      strip.setPixelColor(i - 3, 0); // Erase pixel, but don't refresh!
      strip.show();              // Refresh LED states
      delay(wait);
    }
    else{
      stripClear();
    return;
    } 
  }

  strip.show(); // Refresh to turn off last pixel
}


//Flash random colors on the strip
void flashRandom(){
  int i;
  int r = 0;
  int g = 0;
  int b = 0;
  quit = 0;
  
  stripClear();

//  delay(random(80,120));
    
  switch(random(0,4)){
    case 0: // Red
      r = 127;
      g = 0;
      b = 0;
      break;
    case 1: // Green
      r = 0;
      g = 127;
      b = 0;
      break;
    case 2: // Blue
      r = 0;
      g = 0;
      b = 127;
      break;
    case 3: //White
      r = 127;
      g = 127;
      b = 127;
      break;
  }
  
  // set pixel colors
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b); 
  }
  
  strip.show(); 
  delay(random(100, 170));

  if(quit){
    stripClear();
    return;
  }
}


void theaterChase(uint32_t c, uint8_t wait) {
  quit = 0;
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      if(quit == 0){
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, c);    //turn every third pixel on
        }
        strip.show();
      
        delay(wait);
      
        for (int i=0; i < strip.numPixels(); i += 3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
      }
      else{
        stripClear();
        return;
      }
    }
  }
}


void theaterChaseRainbow(uint8_t wait) {
  quit = 0;
  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
    for (int q=0; q < 3; q++) {
      if(quit == 0){
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
      }
      else{
        stripClear();
        return;
      }
    }
  }
}


uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}


void stripClear(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();
}

