#include <I2cDiscreteIoExpander.h>

#define led1 3
#define led2 5
#define led3 6
#define led4 9
#define led5 10
#define led6 11

char amountleds = 10;
char leds[] = {3, 5, 6, 9, 10, 11, 2, 4, 7, 8};
// 3, 5, 6, 8, 10, 11 hwpwm, 2, 4, 7, 8 softpwm

//char amountleds = 4;
//char leds[] = {6, 9, 10, 11};

int brightness = 255;
int fade = -5;
long rand1; // random led
long rand2; // random timing


// i2c

I2cDiscreteIoExpander expander;

void flicker(int n) {
char dir = 0;
char cycle = 0;
if (n<6){
while (!cycle){
  
  analogWrite(leds[n], brightness);

  brightness = brightness + fade;

  if (brightness <= 100 || brightness == 255) {
    fade = -fade;
    if (dir == 1){ cycle = 1; }
    dir = 1;
  }
  
  delay(5);
}
} else {
  unsigned long time0 = 0;
  unsigned long time1 = millis();
  Serial.println("jeej");
  Serial.println(time1);
    
  digitalWrite(leds[n], HIGH);
  int i = 0;
  while (!cycle){
//Serial.println(millis());
    if (i>= brightness){
      digitalWrite(leds[n], LOW);  
    }
    time1 = millis();
    if (time1 - time0 >= 5) {
      time0 = time1;
      //Serial.println(brightness);    
      brightness = brightness + fade;
      if (brightness <= 100 || brightness == 255) {
        fade = -fade;
        if (dir == 1){ cycle = 1; }
        dir = 1;
      }
    }  
  //delay(5);
    i++;
    if (i==255){
      digitalWrite(leds[n], HIGH);
      i=0;
    }
  }
}
  //Serial.print("brightness: ");
  //Serial.println(brightness);
  //Serial.println("jeej2");
  digitalWrite(leds[n], HIGH); // fixes bug which leaves led off
}

void flicker_i2c(int n){

  uint8_t status;
  static uint16_t data;
  char dir = 0;
  char cycle = 0;
  unsigned long time0 = 0;
  unsigned long time1 = millis();

  int i = 0;
  while (!cycle){
//Serial.println(millis());
    if (i>= brightness){
  //status = expander.digitalWrite(data);
//  status = expander.digitalWrite(0xFF & ~(1<<n));
    status = expander.digitalWrite(0xFE);
  //Serial.println(data);
    }
    time1 = millis();
    if (time1 - time0 >= 5) {
      time0 = time1;
      //Serial.println(brightness);    
      brightness = brightness + fade;
      if (brightness <= 100 || brightness == 255) {
        fade = -fade;
        if (dir == 1){ cycle = 1; }
        dir = 1;
      }
    }  
  //delay(5);
    i++;
    if (i==255){
      status = expander.digitalWrite(0xFF);
      i=0;
    }
  }
  //status = expander.digitalWrite(1);



  // display device information on serial console
/*
  Serial.print("Loop ");
  Serial.print(i, DEC);
  Serial.print(", address ");
  Serial.print(expander.getAddress(), DEC);
  Serial.print(", ");
*/
  //while(1){
//  status = expander.digitalWrite(data);
//  Serial.println(data);
//  delay (500);
  //expander.digitalWrite(2);
  //delay(500);
  //}

/*  
  if (TWI_SUCCESS == status)
  {
    // display success information on serial console
    Serial.print("write 0x");
    Serial.print(i, HEX);
    Serial.print(", ");
  }
  else
  {
    // display error information on serial console
    Serial.print("write error ");
    Serial.print(status, DEC);
    Serial.print(", ");
  }
*/

status = expander.digitalWrite(0xFF);
}

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
Wire.begin();

delay(50);

for(int i=0;i<amountleds;i++){
  pinMode(leds[i], OUTPUT);

  if(i>=6){
    analogWrite(leds[i], brightness);
  } else {
    digitalWrite(leds[i], HIGH);
  }
}

randomSeed(analogRead(0));
}

void loop() {

rand1 = random(amountleds);
//rand2 = random(10000);  
rand2 = 500;
//rand1 = 8;

//flicker(rand1);
flicker_i2c(1);
delay(rand2);



/*
for (int i=0;i<amountleds;i++){
flicker(leds[i]);
delay(1000);
}
*/
  //digitalWrite(led1, HIGH);
}
