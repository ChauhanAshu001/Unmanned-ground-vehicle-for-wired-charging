#include <VirtualWire.h>

#define IR_LEFT   A0
#define IR_RIGHT  A1

// Motor pins
#define EN_LEFT   5
#define EN_RIGHT  6
#define IN1       7
#define IN2       8
#define IN3       9
#define IN4       10

#define BASE_SPEED 255
#define TURN_SPEED 222

bool forwardMode = false;
bool returnMode = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Line Follower with RF Ready");

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(EN_LEFT, OUTPUT);
  pinMode(EN_RIGHT, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  vw_set_rx_pin(11);
  vw_setup(2000);
  vw_rx_start();

  stopMotors();
}

void loop() {
  // --- Check for RF message ---
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) {
    char c = buf[0];
    Serial.print("Received: ");
    Serial.println(c);

    if (c == 'a') {
      Serial.print(c);
      forwardMode = true;
      returnMode = false;
    } 
    else if (c == 'b') {
      Serial.print(c);
      forwardMode = false;
      returnMode = true;
      turnAround();
    }
  }

  // --- Line following behavior ---
  if (forwardMode || returnMode) {
    followLine();
  }
}

// ------------------ Functions ------------------

void followLine() {
  int leftIR = digitalRead(IR_LEFT);
  int rightIR = digitalRead(IR_RIGHT);

  // LOW = black line, HIGH = white surface
  if (leftIR == HIGH && rightIR == HIGH) {
    forward(BASE_SPEED, BASE_SPEED);
  }
  else if (leftIR == HIGH && rightIR == LOW) {
    turnLeft();
  }
  else if (leftIR == LOW && rightIR == HIGH) {
    turnRight();
  }
  else {
    stopMotors();
  }
}

void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_LEFT, leftSpeed);
  analogWrite(EN_RIGHT, rightSpeed);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_LEFT, 0);
  analogWrite(EN_RIGHT, TURN_SPEED);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(EN_LEFT, TURN_SPEED);
  analogWrite(EN_RIGHT, 0);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(EN_LEFT, 0);
  analogWrite(EN_RIGHT, 0);
}

void turnAround() {
  // Spin roughly 180 degrees
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_LEFT, TURN_SPEED);
  analogWrite(EN_RIGHT, TURN_SPEED);
  delay(3000); // adjust to your motor speed
  stopMotors();
}
