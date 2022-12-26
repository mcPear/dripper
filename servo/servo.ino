#include <Servo.h>

// servo
Servo myservo;
#define SERVO_DEGREE_PIPE 0
#define SERVO_DEGREE_GRINDER 180
#define SERVO_MOVEMENT_TIME 1

void move_servo(int degree){
  myservo.write(degree);
  delay(SERVO_MOVEMENT_TIME*1000);
}

//setup
void setup() {
  myservo.attach(3);
}

void loop() {
  move_servo(SERVO_DEGREE_PIPE);
  move_servo(SERVO_DEGREE_GRINDER);
}
