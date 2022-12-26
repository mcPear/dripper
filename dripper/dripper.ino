#include <Servo.h>
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//pins
#define HEATER_PIN 11
#define GRINDER_PIN 12
#define PUMP_PIN 8
#define ONE_WIRE_BUS 2

// servo
Servo myservo;
#define SERVO_DEGREE_PIPE 0
#define SERVO_DEGREE_GRINDER 180
#define SERVO_MOVEMENT_TIME 5

void move_servo(int degree) {
  myservo.write(degree);
  delay(SERVO_MOVEMENT_TIME * 1000);
}

// temperature
float TARGET_TEMPERATURE = 80.0; //92.0; +-2 deg. so 92 = [90,94] which is ok

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp_sensors(&oneWire);
DeviceAddress thermometerDeviceAdress = { 0x28, 0xD1, 0xBE, 0x49, 0xF6, 0xEB, 0x3C, 0x64 }; // found using code from Dallas examples

int get_temp() {
  temp_sensors.requestTemperatures();
  return temp_sensors.getTempC(thermometerDeviceAdress);
}

void set_temp() {
  Serial.print("Initial temp: ");
  Serial.println(get_temp());
  if (get_temp() >= TARGET_TEMPERATURE) {
    Serial.print("Reached temp: ");
    Serial.print(get_temp());
    Serial.print(" Target temp: ");
    Serial.println(TARGET_TEMPERATURE);
    return;
  }

  digitalWrite(HEATER_PIN, HIGH);
  Serial.print("HEATING ");
  while (get_temp() < TARGET_TEMPERATURE) {
    Serial.print(get_temp());
    delay(1 * 1000); // DS18B20 increases its temperature slightly with every read, so delay is crucial!
    Serial.print("|");
  }
  digitalWrite(HEATER_PIN, LOW);

  while(true){
    Serial.println(get_temp());
  }
}

//scale
HX711 coffee_scale;
HX711 water_scale;
#define COFFEE_SCALE_RATIO 420.0  // TODO put known weigth, then scale.get_value(5) / real kg
#define WATER_SCALE_RATIO 420.0   // same as above

//brew settings
float target_coffee_weight = 0.01;
float target_water_weight = 21.0 * target_coffee_weight;

//setup
void setup() {
  Serial.begin(9600);
  myservo.attach(3);
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(GRINDER_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  coffee_scale.begin(A4, A3);
  water_scale.begin(A2, A1);
  coffee_scale.set_scale(COFFEE_SCALE_RATIO);
  water_scale.set_scale(WATER_SCALE_RATIO);
  temp_sensors.begin();
  temp_sensors.setResolution(thermometerDeviceAdress, 9);
}

//loop
bool LOOP_DONE = false;

void loop() {
  if (LOOP_DONE) {
    return;
  }

  set_temp();

  move_servo(SERVO_DEGREE_PIPE);

  digitalWrite(PUMP_PIN, HIGH);
  delay(3 * 1000);
  digitalWrite(PUMP_PIN, LOW);

  move_servo(SERVO_DEGREE_GRINDER);

  coffee_scale.tare();
  digitalWrite(GRINDER_PIN, HIGH);
  while (coffee_scale.get_units(5) < target_coffee_weight) {
    delay(1 * 1000);
  }
  digitalWrite(GRINDER_PIN, LOW);

  water_scale.tare();
  while (water_scale.get_units(5) < target_water_weight) {
    set_temp();
    digitalWrite(PUMP_PIN, HIGH);
    delay(3 * 1000);
    digitalWrite(PUMP_PIN, LOW);
    delay(3 * 1000);
  }

  move_servo(SERVO_DEGREE_PIPE);
  LOOP_DONE = true;
}
