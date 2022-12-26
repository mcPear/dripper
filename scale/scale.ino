#include "HX711.h"

HX711 coffee_scale;
#define COFFEE_SCALE_RATIO 1025.5 // TODO put known weigth, then scale.get_value(5) / real kg

float target_coffee_weight = 0.01;

void setup() {
  Serial.begin(9600);
  coffee_scale.begin(A1, A2);
  coffee_scale.set_scale(COFFEE_SCALE_RATIO);
  delay(200);
  coffee_scale.tare();
}

void loop() { 
  Serial.print(coffee_scale.get_units(10));
  Serial.print('\n');
  // Serial.print(coffee_scale.get_value(5));
  // Serial.print('\n');
  // Serial.print('\n');
}
