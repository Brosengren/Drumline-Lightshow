#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 9);

const byte rxAddr[6] = "00001";
char text[6];
String stringOne = "";
byte index = 0;
volatile int counter = 129;
const int numPattern = 5;

void setup()
{
  Serial.begin(9600);
  attachInterrupt(1, changePattern, RISING);
  radio.begin();
  //radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  
  radio.stopListening();
}

void changePattern(){
  counter++;
  counter %= numPattern;
}

void loop()
{
  stringOne = counter; 
  stringOne.toCharArray(text,6);
  Serial.println(text);
  radio.write(&text, sizeof(text));
  delay(125);
}
