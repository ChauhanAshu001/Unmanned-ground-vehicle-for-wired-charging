
#include <VirtualWire.h>
//
//const int BATTERY_PIN = A0;
//const float R1 = 10000.0;
//const float R2 = 10000.0;
#define BUTTON 2

bool state = false;  // toggle flag
int lastButton = HIGH;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON, INPUT_PULLUP);
  vw_set_tx_pin(12);
  vw_setup(2000);

  Serial.println("RF Transmitter Ready");
}

void loop() {
  // --- Battery reading ---
//  int adc = analogRead(BATTERY_PIN);
//  float vA0 = adc * (5.0 / 1023.0);
//  float batteryVoltage = vA0 * ((R1 + R2) / R2);
//  float percentage = constrain((batteryVoltage - 3.2) * 100 / 1.0, 0, 100);
//
//  Serial.print("Battery: ");
//  Serial.print(batteryVoltage, 2);
//  Serial.print("V | Charge: ");
//  Serial.print(percentage);
//  Serial.println("%");

  // --- Button logic ---
  int currentButton = digitalRead(BUTTON);
  if (currentButton == LOW && lastButton == HIGH) { // Button pressed
    state = !state;  // toggle mode
    const char *msg = state ? "a" : "b";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
    Serial.print("Sent: ");
    Serial.println(msg);
    delay(300);  // debounce
  }
  lastButton = currentButton;

  delay(200);
}
