#include "servo_move.h"
#include "mpu6050.h"
/******************
Left1 S1-S0       S6-S7 Right1
            \   /      
            /   \
Left2 S3-S2       S4-S5 Right2
********************/

int angle[SERVO_NUM] = {ANGLE0,ANGLE1,ANGLE2,ANGLE3,ANGLE4,ANGLE5,ANGLE6,ANGLE7,ANGLE8,ANGLE9,ANGLE10,ANGLE11,ANGLE12};
Servo myservo[SERVO_NUM];

bool isMoveing = 0;

void Servo_Setup(void)
{
  // Attach all servos to their z` pins
  int servoPins[SERVO_NUM] = {S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12};

  for(int i = 0;i<SERVO_NUM;i++) {
    myservo[i].attach(servoPins[i]);
    myservo[i].write(angle[i]);
  }
}

void Servo_Angle(int servo_id, int angle)
{
  if (servo_id >= SERVO_NUM || servo_id < 0)
    return;
  angle = constrain(angle, 0, 180);
  myservo[servo_id].write(angle);
}

void moveStop()
{
  isMoveing = 0;
  for(int i = 0;i<SERVO_NUM;i++)
    myservo[i].write(angle[i]);
}


void moveForward() 
{
  //Lift the first set of legs and step forward.
  lift_lower_Leg_L1_R2(1);
  if(!isMoveing) {
    isMoveing = 1;
    for (int i = 0; i <= STEP_ANGLE; i++) {
      myservo[0].write(angle[0] + i);
      myservo[4].write(angle[4] - i);

      myservo[2].write(angle[2] - i);
      myservo[6].write(angle[6] + i);
      delay(stepDelay);
    }
  }
  else {
    for (int i = 0; i <= STEP_ANGLE; i++) {
      myservo[0].write(angle[0] - STEP_ANGLE + i*2);
      myservo[4].write(angle[4] + STEP_ANGLE - i*2);

      myservo[2].write(angle[2] + STEP_ANGLE - i*2);
      myservo[6].write(angle[6] - STEP_ANGLE + i*2);
      delay(stepDelay);
    }
  }

  //Lower the first set of legs.
  lift_lower_Leg_L1_R2(0);
  lift_lower_Leg_L2_R1(1);

  //Lift the second set of legs and step forward, while simultaneously swinging the first set of legs backward.
  for (int i = 0; i <= STEP_ANGLE; i++) {
    myservo[2].write(angle[2] - STEP_ANGLE + i*2);
    myservo[6].write(angle[6] + STEP_ANGLE - i*2);

    myservo[0].write(angle[0] + STEP_ANGLE - i*2);
    myservo[4].write(angle[4] - STEP_ANGLE + i*2);
    delay(stepDelay);
  }

  //Lower the second set of legs.
  lift_lower_Leg_L2_R1(0);
}


void moveBackward() 
{
  //Lift the first set of legs and step forward.
  lift_lower_Leg_L1_R2(1);
  if(!isMoveing) {
    isMoveing = 1;
    for (int i = 0; i <= STEP_ANGLE; i++) {
      myservo[0].write(angle[0] - i);
      myservo[4].write(angle[4] + i);

      myservo[2].write(angle[2] + i);
      myservo[6].write(angle[6] - i);
      delay(stepDelay); 
    }
  }
  else {
    for (int i = 0; i <= STEP_ANGLE; i++) {
      myservo[0].write(angle[0] + STEP_ANGLE - i*2);
      myservo[4].write(angle[4] - STEP_ANGLE + i*2);

      myservo[2].write(angle[2] - STEP_ANGLE + i*2);
      myservo[6].write(angle[6] + STEP_ANGLE - i*2);
      delay(stepDelay);
    }
  }

  //Lower the first set of legs.
  lift_lower_Leg_L1_R2(0);
  lift_lower_Leg_L2_R1(1);

  //Lift the second set of legs and step forward, while simultaneously swinging the first set of legs backward.
  for (int i = 0; i <= STEP_ANGLE; i++) {
    myservo[2].write(angle[2] + STEP_ANGLE - i*2);
    myservo[6].write(angle[6] - STEP_ANGLE + i*2);

    myservo[0].write(angle[0] - STEP_ANGLE + i*2);
    myservo[4].write(angle[4] + STEP_ANGLE - i*2);
    delay(stepDelay);
  }

  //Lower the second set of legs.
  lift_lower_Leg_L2_R1(0);
}


void turnLeft() 
{
  //Lift the first set of legs and step forward.
  lift_lower_Leg_L1_R2(1);
  if(!isMoveing) {
    isMoveing = 1;
    for (int i = 0; i <= STEP_ANGLE; i++) {
      myservo[0].write(angle[0] - i);
      myservo[4].write(angle[4] - i);

      myservo[2].write(angle[2] + i);
      myservo[6].write(angle[6] + i);
      delay(stepDelay);
    }
  }
  else {
    for (int i = 0; i <= STEP_ANGLE; i++) {
      myservo[0].write(angle[0] + STEP_ANGLE - i*2);
      myservo[4].write(angle[4] + STEP_ANGLE - i*2);

      myservo[2].write(angle[2] - STEP_ANGLE + i*2);
      myservo[6].write(angle[6] - STEP_ANGLE + i*2);
      delay(stepDelay);
    }
  }

  //Lower the first set of legs.
  lift_lower_Leg_L1_R2(0);
  lift_lower_Leg_L2_R1(1);
  //Lift the second set of legs and step forward, while simultaneously swinging the first set of legs backward.
  for (int i = 0; i <= STEP_ANGLE; i++) {
    myservo[2].write(angle[2] + STEP_ANGLE - i*2);
    myservo[6].write(angle[6] + STEP_ANGLE - i*2);

    myservo[0].write(angle[0] - STEP_ANGLE + i*2);
    myservo[4].write(angle[4] - STEP_ANGLE + i*2);
    delay(stepDelay);
  }

  //Lower the second set of legs.
  lift_lower_Leg_L2_R1(0);
}

void turnRight() 
{
  //Lift the first set of legs and step forward.
  lift_lower_Leg_L1_R2(1);
  if(!isMoveing) {
    isMoveing = 1;
    for (int i = 0; i <= STEP_ANGLE; i++) {

      myservo[0].write(angle[0] + i);
      myservo[4].write(angle[4] + i);

      myservo[2].write(angle[2] - i);
      myservo[6].write(angle[6] - i);
      delay(stepDelay);
    }
  }
  else {
    for (int i = 0; i <= STEP_ANGLE; i++) {

      myservo[0].write(angle[0] - STEP_ANGLE + i*2);
      myservo[4].write(angle[4] - STEP_ANGLE + i*2);

      myservo[2].write(angle[2] + STEP_ANGLE - i*2);
      myservo[6].write(angle[6] + STEP_ANGLE - i*2);
      delay(stepDelay);
    }
  }

  //Lower the first set of legs.
  lift_lower_Leg_L1_R2(0);

  lift_lower_Leg_L2_R1(1);
  //Lift the second set of legs and step forward, while simultaneously swinging the first set of legs backward.
  for (int i = 0; i <= STEP_ANGLE; i++) {
    myservo[2].write(angle[2] - STEP_ANGLE + i*2);
    myservo[6].write(angle[6] - STEP_ANGLE + i*2);

    myservo[0].write(angle[0] + STEP_ANGLE - i*2);
    myservo[4].write(angle[4] + STEP_ANGLE - i*2);
    delay(stepDelay);
  }

  //Lower the second set of legs.
  lift_lower_Leg_L2_R1(0);
}

void lift_lower_Leg_L1_R2(int lift_lower){
  for (int i = 0; i <= STEP_ANGLE; i++) {
    if(lift_lower == 1){
      myservo[1].write(angle[1] + i);
      myservo[5].write(angle[5] + i);
    } else if(lift_lower == 0){
      myservo[1].write(angle[1] + STEP_ANGLE - i);
      myservo[5].write(angle[5] + STEP_ANGLE - i);
    }
    delay(stepDelay/2);
  }
}

void lift_lower_Leg_L2_R1(int lift_lower){
  for (int i = 0; i <= STEP_ANGLE; i++) {
    if(lift_lower == 1){
      myservo[3].write(angle[3] - i);
      myservo[7].write(angle[7] - i);
    } else if(lift_lower == 0){
      myservo[3].write(angle[3] - STEP_ANGLE + i);
      myservo[7].write(angle[7] - STEP_ANGLE + i);
    }
    delay(stepDelay/2);
  }
}

void setAngleData(int angle1, int angle3, int angle5, int angle7)
{
  Servo_Angle(1, angle[1] + angle1);
  Servo_Angle(3, angle[3] + angle3);
  Servo_Angle(5, angle[5] + angle5);
  Servo_Angle(7, angle[7] + angle7);
}

void restore(){
  setAngleData(ANGLE1, ANGLE3, ANGLE5, ANGLE7); 
  for(int i = 0;i<SERVO_NUM;i++) {
    myservo[i].write(angle[i]);
  }
}

void selfBalanced(){
  imuUpdate(dt);

  // 1. 获取IMU姿态
  ImuState imu = imuGetState();
  float roll = imu.pitchDeg;  // 前后倾斜
  float pitch  = imu.rollDeg;   // 左右倾斜
  // Serial.print("pitch:");
  // Serial.print(pitch);
  // Serial.print("  roll:");
  // Serial.print(roll);

  // 3. 计算四条腿 膝盖需要修正的角度
  int knee_FL =  -pitch * PITCH_GAIN  -  roll * ROLL_GAIN;  // 左前膝
  int knee_FR =  pitch * PITCH_GAIN  -  roll * ROLL_GAIN;  // 右前膝
  int knee_BL = -pitch * PITCH_GAIN  +  roll * ROLL_GAIN;  // 左后膝
  int knee_BR = pitch * PITCH_GAIN  +  roll * ROLL_GAIN;  // 右后膝
  // Serial.print("  knee_FL:");
  // Serial.print(knee_FL);

  // Serial.print("  knee_FR:");
  // Serial.print(knee_FR);

  // Serial.print("  knee_BL:");
  // Serial.print(knee_BL);

  // Serial.print("  knee_BR:");
  // Serial.println(knee_BR);

  setAngleData(knee_FL, knee_BL, knee_BR, knee_FR);
}

void lift_lower_Leg_L1_R2_dance(int lift_lower, int step_angle){
  for (int i = 0; i <= step_angle; i++) {
    if(lift_lower == 1){
      myservo[1].write(angle[1] + i);
      myservo[5].write(angle[5] + i);
    } else if(lift_lower == 0){
      myservo[1].write(angle[1] + step_angle - i);
      myservo[5].write(angle[5] + step_angle - i);
    }
    delay(5);
  }
}

void lift_lower_Leg_L2_R1_dance(int lift_lower, int step_angle){
  for (int i = 0; i <= step_angle; i++) {
    if(lift_lower == 1){
      myservo[3].write(angle[3] - i);
      myservo[7].write(angle[7] - i);
    } else if(lift_lower == 0){
      myservo[3].write(angle[3] - step_angle + i);
      myservo[7].write(angle[7] - step_angle + i);
    }
    delay(5);
  }
}

void danceTwist() {
  // 左右扭腰摇摆舞
  int delayDance = 5;
  int step_twist = 30;
  for (int i = 0; i <= step_twist; i++) {
    myservo[0].write(angle[0] + i);
    myservo[4].write(angle[4] + i);
    myservo[2].write(angle[2] - i);
    myservo[6].write(angle[6] - i);
    delay(delayDance);
  }
  delay(100);
  for (int i = 0; i <= step_twist; i++) {
    myservo[0].write(angle[0] + step_twist - i);
    myservo[4].write(angle[4] + step_twist - i);
    myservo[2].write(angle[2] - step_twist + i);
    myservo[6].write(angle[6] - step_twist + i);
    delay(delayDance);
  }
  delay(100);
  for (int i = 0; i <= step_twist; i++) {
    myservo[0].write(angle[0] - i);
    myservo[4].write(angle[4] - i);
    myservo[2].write(angle[2] + i);
    myservo[6].write(angle[6] + i);
    delay(delayDance);
  }
  delay(100);
  for (int i = 0; i <= step_twist; i++) {
    myservo[0].write(angle[0] - step_twist + i);
    myservo[4].write(angle[4] - step_twist + i);
    myservo[2].write(angle[2] + step_twist - i);
    myservo[6].write(angle[6] + step_twist - i);
    delay(delayDance);
  }
  delay(100);
}

void danceUpDown() {
  lift_lower_Leg_L1_R2_dance(1, 35);
  lift_lower_Leg_L2_R1_dance(1, 35);
  delay(500);
  lift_lower_Leg_L1_R2_dance(0, 35);
  lift_lower_Leg_L2_R1_dance(0, 35);
  delay(500);
}

void danceStep() {
  lift_lower_Leg_L1_R2_dance(1, 35);
  lift_lower_Leg_L1_R2_dance(0, 35);
  delay(100);
  lift_lower_Leg_L2_R1_dance(1, 35);
  lift_lower_Leg_L2_R1_dance(0, 35);
  delay(100);
}

void danceCircle() {
  // 转圈摇摆舞
  int delayDance = 5;
  int step_twist = 50;
  for (int i = 0; i <= step_twist; i++) {
    myservo[0].write(angle[0] - i);
    myservo[4].write(angle[4] - i);
    myservo[2].write(angle[2] - i);
    myservo[6].write(angle[6] - i);
    delay(delayDance);
  }
  delay(100);
  for (int i = 0; i <= step_twist; i++) {
    myservo[0].write(angle[0] - step_twist + i);
    myservo[4].write(angle[4] - step_twist + i);
    myservo[2].write(angle[2] - step_twist + i);
    myservo[6].write(angle[6] - step_twist + i);
    delay(delayDance);
  }
  delay(100);
  for (int i = 0; i <= step_twist; i++) {
    myservo[0].write(angle[0] + i);
    myservo[4].write(angle[4] + i);
    myservo[2].write(angle[2] + i);
    myservo[6].write(angle[6] + i);
    delay(delayDance);
  }
  delay(100);
  for (int i = 0; i <= step_twist; i++) {
    myservo[0].write(angle[0] + step_twist - i);
    myservo[4].write(angle[4] + step_twist - i);
    myservo[2].write(angle[2] + step_twist - i);
    myservo[6].write(angle[6] + step_twist - i);
    delay(delayDance);
  }
  delay(100);
}

void dance_show(){
  for (int i = 0; i < 4; i++) {
    danceStep();
  }
  for (int i = 0; i < 2; i++) {
    danceTwist();
    danceCircle();
    danceUpDown();
  }
}
