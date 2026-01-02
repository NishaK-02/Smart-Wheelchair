//Motor Pins
#define IN1 PB1    // Left Motor
#define IN2 PB15
#define IN3 PA4    // Right Motor (reverse wired)
#define IN4 PA5

// LEDs
#define LED1 PB10
#define LED2 PB11

// Buzzer
#define BUZZER PA2

// Ultrasonic
#define TRIG1 PB6
#define ECHO1 PB7
#define TRIG2 PB8
#define ECHO2 PB9

#define OBSTACLE_DISTANCE 20   // cm

// Joystick
#define JOY_X PA0
#define JOY_Y PA1

#define DEADZONE 300

int joyX_center = 2048;
int joyY_center = 2048;

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  delay(1000);               // joystick stabilize
  joyX_center = analogRead(JOY_X);
  joyY_center = analogRead(JOY_Y);

  stopMotors();
}

void loop() {

  // Obstacle Detection
  int d1 = readUltrasonic(TRIG1, ECHO1);
  int d2 = readUltrasonic(TRIG2, ECHO2);

  if ((d1 > 0 && d1 < OBSTACLE_DISTANCE) ||
      (d2 > 0 && d2 < OBSTACLE_DISTANCE)) {
    stopMotors();
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(BUZZER, HIGH);
    return;
  } else {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(BUZZER, LOW);
  }

  // Joystick Control
  int xVal = analogRead(JOY_X);
  int yVal = analogRead(JOY_Y);

  int xDiff = xVal - joyX_center;
  int yDiff = yVal - joyY_center;

  if (abs(yDiff) > DEADZONE) {
    if (yDiff > 0) moveForward();
    else moveBackward();
  }
  else if (abs(xDiff) > DEADZONE) {
    if (xDiff > 0) turnRight();
    else turnLeft();
  }
  else {
    stopMotors();
  }

  delay(30);
}

// Motor Control 
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Ultrasonic Function
int readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}