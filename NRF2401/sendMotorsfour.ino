#include<SPI.h>
#include<RF24.h>
#include<string.h>

//Setting up the CE and CSN pins
RF24 radio(9,10);

//keeps track of last potentiomer value for comparison with new potentiometer values
int old_potValue0 = 0;
int old_potValue1 = 0;

void setup(void){
  Serial.begin(9600); // begin the serial monitor for debugging purposes
  radio.begin();//Start the radio
  radio.setPALevel(RF24_PA_MAX);//Setting the Power Amplification Level
  radio.setChannel(0x76);//Setting channel
  radio.openWritingPipe(0xF0F0F0F0E1LL);//Open the sending address
  radio.enableDynamicPayloads();//Enable dynamic payloads
  radio.powerUp();//Power up the radio
}
void loop(void) {
  //read the potentiometer values
  int potValue0 = analogRead(A0);
  int potValue1 = analogRead(A1);
  
  //converts the values to range from 3-12 (which is representative of duty cycle) instead of 0-1023, and appends a 1 or 2 
  //to the front, based on whether it is potentiometer 1 or 2
  int angleValue0 = 100 + map(potValue0, 0, 1023, 3, 12);
  int angleValue1 = 200 + map(potValue1, 0, 1020, 3, 12);
  
  //prints for debugging purposes
  Serial.println("New av0 value: " + (String)potValue0 + "Old av0 value: " + (String)old_potValue0);
  Serial.println("New av1 value: " + (String)potValue1 + "Old av1 value: " + (String)old_potValue1);
  
  //Sends a signal over radio if the new potentiometer 0 value is different than the old one (with a small range built in)
  if(potValue0 > old_potValue0 + 5 || potValue0 < old_potValue0-5)
  {
      radio.write(&angleValue0, sizeof(angleValue0));//Send the message
      delay(1200);
  }
  
  //Sends a signal over radio if the new potentiometer 1 value is different than the old one (with a small range built in)
  if(potValue1 > old_potValue1 + 5 || potValue1 < old_potValue1-5)
  {
      radio.write(&angleValue1, sizeof(angleValue1));//Send the message
      delay(1200);
  }
  //float angleValue1 = (13.0/1023.0)*potValue1;

  old_potValue0 = potValue0;
  old_potValue1 = potValue1;
}
