  /**********************************************************************
  Product     : Adeept Hexpod for Arduino
  Auther      : www.adeept.com
  Modification: 2025/10/20
**********************************************************************/
#include <IRremote.hpp>
#include "Adeept_Hexpod_For_Arduino.h" 
#include "servo_move.h"
#include "mpu6050.h"

#define RECV_PIN   2        // Infrared receiving pin
int move_flag = 0; //0 indicates stop, 1 indicates move forward, 2 indicates move backward, 3 indicates turning left, and 4 indicates turning right.

#define BREATHING   0       // 呼吸灯
#define RAINBOW     1       // 彩虹灯
#define RUNNING     2       // 流水灯
#define ALARM       3       // 警报灯
int ws2812Mode = -1; 
int r = 0, g = 0, b = 0; 
int stat_steady = 0;
String comdata = "";

void setup()
{  
  Serial.begin(115200);
  IR_Setup();
  delay(100);  
  Servo_Setup();               //Servo initialization
  delay(100);   

  WS2812_Setup();                //WS2812 LED initialization   
  WS2812ColorAll(0,255,255);

  imuInit(); 

  OLED_Setup();                //OLED initialization
  OLED(1, 0, 0, "www.adeept.com");
  OLED(1, 0, 3, "   Quadraqued Robot");

  Buzzer_Setup();                //Buzzer initialization
  Buzzer_Alert(2, 1);
}

void loop()
{
  if (IrReceiver.decode()) {          // Waiting for decoding
    comdata = getIRValue();
  }
  judgement();
  control();
  delay(1);
  show_Battery_ratio();
}

void judgement()
{
  if(comdata == "UP"){//move forward
    move_flag = 1;
  }
  else if(comdata == "DOWN"){//move backward
    move_flag = 2;
  }
  else if(comdata == "LEFT"){//trun left
    move_flag = 3;
  }
  else if(comdata == "RIGHT"){//trun right
    move_flag = 4;
  }
  else if(comdata == "1"){ // danceStep
    move_flag = 11;
  }
  else if(comdata == "2"){ // danceTwist
    move_flag = 12;
  }
  else if(comdata == "3"){ // danceCircle
    move_flag = 13;
  }
  else if(comdata == "4"){ // danceUpDown
    move_flag = 14;
  }
  else if(comdata == "5"){ // dance_show
    move_flag = 15;
  }
  else if(comdata == "OK"){//move stop
    move_flag = 0;
    moveStop();
  }
  else if(comdata == "A"){
    ws2812Mode = 5;
    r = 255, g = 255, b = 0;
  }
  else if(comdata == "B"){
    ws2812Mode = 6;
    r = 0, g = 255, b = 255;
  }
  else if(comdata == "C"){
    ws2812Mode = 7;
    r = 255, g = 0, b = 255;
  }
  else if(comdata == "D"){
    ws2812Mode = 8;
    r = 255, g = 0, b = 0;
  }
  else if(comdata == "E"){
    ws2812Mode = 9;
    WS2812ColorAll(0,0,0);
  }
  else if(comdata == "0"){
    stat_steady = 0;
    restore();
  }
  else if(comdata == "F"){
    stat_steady = 1;
  }
  comdata = "";
}

void control()
{
  switch (move_flag) 
  {
    case 1:
      moveForward();
      break;
    case 2:
      moveBackward();
      break;
    case 3:
      turnLeft();
      break;
    case 4:
      turnRight();
      break;
    case 11:
      danceStep();
      break;
    case 12:
      danceTwist();
      break;
    case 13:
      danceCircle();
      break;
    case 14:
      danceUpDown();
      break;
    case 15:
      dance_show();
      break;
  }
  switch(ws2812Mode) 
  {
    case 5:
      breathingEffect(r, g, b);
      break;
    case 6:
      rainbowEffect();
      break;
    case 7:
      runningLightEffect(r, g, b);
      break;
    case 8:
      alarmEffect(r, g, b);
      break;
  }

  if(stat_steady && !move_flag) 
  {
    selfBalanced();
  }
}


int show_Battery_ratio()
{
  static unsigned long previousMillis = -60000;
  char buffer[4];
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= 60000) {
    previousMillis = currentMillis;

    sprintf(buffer, "%d%%", Get_Battery_ratio()); // 将整数转换为字符串
    OLED(1, 110, 0, buffer);
  }
}

////////////////////////IR area/////////////////////////////////
void IR_Setup()
{
  IrReceiver.begin(RECV_PIN);        // Start the receiver
}

String switch_irr(unsigned long irr_data)
{
  switch(irr_data)
  {
    case 69: return "A";    
    case 70: return "B";    
    case 71: return "C";    
    case 68: return "D";    
    case 64: return "UP";   
    case 67: return "E";    
    case 7: return "LEFT"; 
    case 21: return "OK";   
    case 9: return "RIGHT";
    case 22: return "0";    
    case 25: return "DOWN"; 
    case 13: return "F";    
    case 12: return "1";    
    case 24: return "2";    
    case 94: return "3";    
    case 8: return "4";    
    case 28: return "5";    
    case 90: return "6";    
    case 66: return "7";    
    case 82: return "8";    
    case 74: return "9";    
    default: return "NONE";
  }
}

String getIRValue()
{
  unsigned long value = IrReceiver.decodedIRData.command;
  String command_ir = switch_irr(value);
  // Serial.print(value);
  // Serial.print(" ==> ");
  // Serial.println(command_ir);
  IrReceiver.resume();                        // Release the IRremote. Receive the next value
  return command_ir;
}
