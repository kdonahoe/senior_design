void record();
void camera();

#include<SPI.h>
#include<RF24.h>
#include<string.h>

//Setting up the CE and CSN pins
RF24 radio(9,10);

//keeps track of last potentiomer value for comparison with new potentiometer values
int old_potValue0 = 0;
int old_potValue1 = 0;
int but0 = 5; // Red button-Record button
int but1 = 6;  // White button-Picture Button

//Three LEDs for battery monitoring
int led0 = 7; // Green LED
int led1 = 8;  // Yellow LED
int led2 = 2; // Red LED

//Two LEDs are camera functioning LEDs
int led3 = 3; // Red Record LED
int led4 = 4; //Yellow Picture taken LED


int read_but0; //Red button
int read_but1; //White button

int pot_num; 
int bright;

void setup(void){

  //Buttons as inputs
  pinMode (but0, INPUT); //Red button
  pinMode (but1, INPUT); //White button
  
  //battery monitoring LEDs
  pinMode (led0, OUTPUT); //Green LED
  pinMode (led1, OUTPUT); //Yellow LED
  pinMode (led2, OUTPUT); //Red LED

  digitalWrite(led0, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  //Battery monitoring pin
  pinMode(A5, INPUT); // battery voltage input
  pinMode(A5, INPUT_PULLUP); //pullup resistor enables on input
  
  //Record and Camera lights as outputs
  pinMode (led3, OUTPUT); //Red Record LED
  pinMode (led4, OUTPUT);//Yellow Picture taken LED
  
  //Begin serial monitor for debugging
  Serial.begin(9600); 
  
  //Antenna setup
  radio.begin();//Start the radio
  radio.setPALevel(RF24_PA_MAX);//Setting the Power Amplification Level
  radio.setChannel(0x76);//Setting channel
  radio.openWritingPipe(0xF0F0F0F0E1LL);//Open the sending address
  radio.enableDynamicPayloads();//Enable dynamic payloads
  radio.powerUp();//Power up the radio
}
void loop(void) {
  record();
  camera();
  batteryMonitor();
  //read the potentiometer values
  int potValue0 = analogRead(A0);
  int potValue1 = analogRead(A1);
  
  //converts the values to range from 3-12 (which is representative of duty cycle) instead of 0-1023, and appends a 1 or 2 
  //to the front, based on whether it is potentiometer 1 or 2
  int angleValue0 = 100 + map(potValue0, 0, 1023, 3, 12);
  int angleValue1 = 200 + map(potValue1, 0, 1020, 3, 12);
  
  //prints for debugging purposes
 // Serial.println("New av0 value: " + (String)potValue0 + "Old av0 value: " + (String)old_potValue0);
 // Serial.println("New av1 value: " + (String)potValue1 + "Old av1 value: " + (String)old_potValue1);
  
  //Sends a signal over radio if the new potentiometer 0 value is different than the old one (with a small range built in)
  if(potValue0 > old_potValue0 + 5 || potValue0 < old_potValue0-5)
  {
      radio.write(&angleValue0, sizeof(angleValue0));//Send the message
      delay(300);
  }
  
  //Sends a signal over radio if the new potentiometer 1 value is different than the old one (with a small range built in)
  if(potValue1 > old_potValue1 + 5 || potValue1 < old_potValue1-5)
  {
      radio.write(&angleValue1, sizeof(angleValue1));//Send the message
      delay(300);
  }
  //float angleValue1 = (13.0/1023.0)*potValue1;

  old_potValue0 = potValue0;
  old_potValue1 = potValue1;
}
void record()
{
  
  read_but0 = digitalRead(but0);
  Serial.println((String)read_but0);
  if(read_but0 == 1)//Pressing the record button
  {
    delay(100);
    read_but0 = digitalRead(but0);
    if (read_but0 == 0)
    {
      int recordValue1 = 500;
      radio.write(&recordValue1, sizeof(recordValue1));//Send the message
    }
     
    while(read_but0 == 0){
        digitalWrite(led3, HIGH);
        delay(100);       
        digitalWrite(led3, LOW);        
        delay(100);       
        digitalWrite(led3, HIGH);       
        delay(100);       
        digitalWrite(led3, LOW);
        read_but0 = digitalRead(but0);
        Serial.println((String)read_but0);
        delay(100);

      if (read_but0 == 1)
      {
        int recordValue1 = 600;
        radio.write(&recordValue1, sizeof(recordValue1));//Send the message
        digitalWrite(led3, LOW);
        delay(500);
      }
    }
    
  }
}


void camera()
{
  read_but1 = digitalRead(but1);//Camera button(white)
  if (1&&read_but1)//Pressing camera button flashes twice
  {
    int cameraValue1 = 300 + map(1, 0, 0, 0,0);
     radio.write(&cameraValue1, 1);//Send the message
    digitalWrite(led4, HIGH);
    delay(400);
    digitalWrite(led4, LOW);    
  }
}

void batteryMonitor()
{
  delay(1000);
  int sensorValue = analogRead(A5); //read the A5 pin value, which is 0-1023
  Serial.println((String)sensorValue);
  float voltage = sensorValue * (5.0 / 1023.0); //convert the value to a true voltage, 0-5 V.
  Serial.println((String)voltage);
 
 //green LED on
  if (voltage > 4) //set the voltage considered low battery here
  {
    digitalWrite(led0, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
  
  //yellow LED on
  if (voltage <= 4 && voltage >= 2.5)
  {
    digitalWrite(led1,HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led0, LOW);
  }
  
  //red LED on
  if (voltage < 2.5)
  {
    digitalWrite(led2,HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led0, LOW);
  }
}
