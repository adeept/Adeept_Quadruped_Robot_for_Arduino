  /**********************************************************************
  Product     : Adeept Quadruped Robot for Arduino
  Auther      : www.adeept.com
  Modification: 2025/10/20
**********************************************************************/
#include "Adeept_Hexpod_For_Arduino.h" 
#include "servo_move.h"
#include "mpu6050.h"

#define Move_UP "forward"
#define Move_Down "backward"
#define Move_Left "left"
#define Move_Right "right"
#define Move_Stop "DTS"

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

  Serial.println("AT+CWMODE=3\r\n");//set to softAP+station mode
  delay(3000);     //delay 3s  
  Serial.println("AT+CWSAP=\"Adeept_ADA039\",\"12345678\",8,2\r\n");   //TCP Protocol, server IP addr, port
  delay(1000);     //delay 1s
  Serial.println("AT+RST\r\n");     //reset wifi
  delay(1000);     //delay 1s
  Serial.println("AT+CIPMUX=1\r\n");//set to multi-connection mode
  delay(1000);
  Serial.println("AT+CIPSERVER=1,4000\r\n");//set as server
  delay(1000);
  Serial.println("AT+CIPSTO=7000\r\n");//keep the wifi connecting 7000 seconds
  delay(1000);

  Servo_Setup();               //Servo initialization

  WS2812_Setup();                //WS2812 LED initialization   
  WS2812ColorAll(0,255,255);

  imuInit(); 

  OLED_Setup();                //OLED initialization
  OLED(1, 0, 0, "www.adeept.com");
  OLED(1, 0, 3, "WIFI:Adeept_ADA039");
  OLED(1, 0, 5, "PWD:12345678");
  OLED(1, 0, 7, "IP: 192.168.4.1");

  Buzzer_Setup();                //Buzzer initialization
  Buzzer_Alert(2, 1);
}

void loop()
{
  while(Serial.available()>0)
  {  
    comdata += char(Serial.read());
    delay(1);
  }
  judgement();
  control();
  delay(1);
  show_Battery_ratio();
}

void judgement()
{
  if(comdata.indexOf(Move_UP)>=0){//move forward
    move_flag = 1;
  }
  else if(comdata.indexOf(Move_Down)>=0){//move backward
    move_flag = 2;
  }
  else if(comdata.indexOf(Move_Left)>=0){//trun left
    move_flag = 3;
  }
  else if(comdata.indexOf(Move_Right)>=0){//trun right
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
  else if(comdata.indexOf(Move_Stop)>=0 || comdata.indexOf("danceOff") > 0){//move stop
    move_flag = 0;
    moveStop();
  }
  else if(comdata.indexOf("dance") > 0){ // dance_show
    move_flag = 15;
  }
  else if (comdata.indexOf("lightMode")>0)
  {
    String dataArrayString = comdata.substring(comdata.indexOf("[")+1, comdata.indexOf("]"));
    int firstComma = dataArrayString.indexOf(",");
    int secondComma = dataArrayString.indexOf(",", firstComma + 1);
    r = dataArrayString.substring(0, firstComma).toInt();
    g = dataArrayString.substring(firstComma + 1, secondComma).toInt();
    b = dataArrayString.substring(secondComma + 1).toInt();
    if(comdata.indexOf("breath")>0){
      ws2812Mode = 5;
    }
    else if(comdata.indexOf("flowing")>0){
      ws2812Mode = 6;
    }
    else if(comdata.indexOf("rainbow")>0){
      ws2812Mode = 7;
    }
    else if(comdata.indexOf("police")>0){
      ws2812Mode = 8;
    }
  } 
  else if(comdata.indexOf("steadyOff")>0){
    stat_steady = 0;
    restore();
  }
  else if(comdata.indexOf("steady")>0){
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
      runningLightEffect(r, g, b);
      break;
    case 7:
      rainbowEffect();
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

