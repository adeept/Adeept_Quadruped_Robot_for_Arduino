#ifndef __SERVO_MOVE_H__
#define __SERVO_MOVE_H__
#include <Arduino.h>
#include <Servo.h>
#include "angle.h"
#define S0 4   
#define S1 5   
#define S2 6   
#define S3 7   
#define S4 8   
#define S5 9   
#define S6 10   
#define S7 11  
#define S8 12   
#define S9 13   
#define S10 A0   
#define S11 A1   
#define S12 A2  
#define SERVO_NUM 13
/******************
Left1 S1-S0       S6-S7 Right1
            \   /      
            /   \
Left2 S3-S2       S4-S5 Right2
********************/
#define STEP_ANGLE 15  //步幅的一半
#define stepDelay  20
const float PITCH_GAIN = 2.0f;   // 前后平衡力度
const float ROLL_GAIN  = 1.6f;   // 左右平衡力度
const float dt = 0.001f;

void Leg(int Hip_id, int Hip_angle, int Knee_id, int Knee_angle);
void Servo_Setup(void);
void Servo_Angle(int servo_id, int angle);
void moveStop();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void selfBalanced(); 
void restore(void);
void lift_lower_Leg_L1_R2(int lift_lower);
void lift_lower_Leg_L2_R1(int lift_lower);
void danceUpDown();
void danceStep();
void danceTwist();
void danceCircle();
void dance_show();
#endif
