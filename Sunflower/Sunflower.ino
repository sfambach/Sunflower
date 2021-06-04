// Sunflower 
// 4 Light dependent resistors are checking the sun. 
// If the sun changes (earth is more possibly ;) ) the sensors are getting differten values
// depending on that values a movement for the panel is calculated to get the most sun.
//
//
// 2019/06/04 Written by https://somebodys.taobao.com/ https://www.ixigua.com/i6765878731480236547/
// 2021/06/04 Changed by S. Fambach visit www.fambach.net if you want.
// 
// needed libs
// Servo (included in the arduino gui)
//


#include <Servo.h> 

#define SERVOPINH  5 // servopin 1 (Horizontal)
#define SERVOPINV  6 // servopin 2 (Vertical)

#define dtime   50  // delay befor change the direction of the panel（10~100） 
#define tol   50    // tolrance 10~100 to change panel direction

          
// Horizontal Servo
Servo horizontal; // create object
int servoh = 90;   // servo? center

int servohLimitHigh = 175;  // Horizontal servo max value
int servohLimitLow = 5;     // Horizontal servo min value

// Vertical Servo
Servo vertical;     // create object
int servov = 90;     // servo? center

int servovLimitHigh = 180;  // Max value
int servovLimitLow = 90;    //Min value


// LDR Sensors for light measurement 
const int ldrlt = A0; // ldr 1  left / top
const int ldrrt = A1; // ldr 2  right / top
const int ldrld = A2; // ldr 3  left / bottom
const int ldrrd = A3; // ldr 4  right / bottom

void setup()
{
  Serial.begin(9600);
  horizontal.attach(SERVOPINH); 
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
  delay(100);

  // move vertical sensor from lower to upper limit
  for(int i=servovLimitLow;i<servovLimitHigh;i+=10)
  {  
     vertical.write(i);
     delay(40);
  }
  
  // move vertical sensor to center position
  vertical.write((servovLimitLow + servovLimitHigh)/2);
  delay(100);
  
  // move horizontal sensor from lower to upper limit
  for(int i=servohLimitLow;i<servohLimitHigh;i+=10)
   {  horizontal.write(i);
     delay(40);
   }
   
  // move horizontal sensor to center position
  horizontal.write((servohLimitHigh + servohLimitLow)/2);
   

}

void loop() 
{
  // read the sensors
  int lt = analogRead(ldrlt); //
  int rt = analogRead(ldrrt); //
  int ld = analogRead(ldrld); //
  int rd = analogRead(ldrrd); //
  

  // calculate differences      
  int avt = (lt + rt) / 2; // 
  int avd = (ld + rd) / 2; // 
  int avl = (lt + ld) / 2; // 
  int avr = (rt + rd) / 2; //
  
  // calulate movement 
  int dvert = avt - avd; // 
  int dhoriz = avl - avr;//

  // printout data of the sensors
  Serial.print(lt);
  Serial.print(",");
  Serial.print(rt);
  Serial.print(",");
  Serial.print(ld);
  Serial.print(",");
  Serial.print(rd);
  Serial.print ("      |    ");    
  
  Serial.print(avt);
  Serial.print(",");
  Serial.print(avd);
  Serial.print(",");
  Serial.print(avl);
  Serial.print(",");
  Serial.print(avr);
  Serial.print(",   ");
  Serial.print(dtime);
  Serial.print(",   ");
  Serial.println(tol);    
  
  
  // Calculate new position for vertical sensor  
  if (-1*tol > dvert || dvert > tol) 
 {
  if (avt > avd)
  {
    servov = ++servov;
     if (servov > servovLimitHigh) 
     { 
      servov = servovLimitHigh;
     }
  }
  else if (avt < avd)
  {
    servov= --servov;
    if (servov < servovLimitLow)
  {
    servov = servovLimitLow;
  }
  }
  vertical.write(servov); // set the new position
  }

  // calculate the new position of the horizontal sensor
  if (-1*tol > dhoriz || dhoriz > tol) 
  {
  if (avl > avr)
  {
    servoh = --servoh;
    if (servoh < servohLimitLow)
    {
    servoh = servohLimitLow;
    }
  }
  else if (avl < avr)
  {
    servoh = ++servoh;
     if (servoh > servohLimitHigh)
     {
     servoh = servohLimitHigh;
     }
  }
  else if (avl = avr)
  {
    // nothing
  }
  horizontal.write(servoh); // write the new horizontal positon
  }
   delay(dtime);

}
