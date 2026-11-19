
/*
 * Project: PID-Based Line Following Robot
 *
 * Controller: Arduino
 * Motor Driver: L298N Dual H-Bridge
 * Motors: 2x N20 DC Motors (400 RPM)
 * Sensors: TCRT5000L 5-Channel IR Sensor Array
 * Control Algorithm: PID
 *
 * Description:
 * An autonomous line following robot that uses five
 * infrared sensors to detect a line and PID control
 * to adjust the speed of the two DC motors.
 */

// Motor Pins
#define enA 3 // Right Motor Enable Pin
#define in1 4 // Right Motor IN1
#define in2 5 // Right Motor IN2
#define enB 9 // Left Motor Enable Pin
#define in3 6 // Left Motor IN3
#define in4 7 // Left Motor IN4

// Sensor Pins
#define ir1 13 // Leftmost Sensor
#define ir2 12 // Left Sensor
#define ir3 11 // Middle Sensor
#define ir4 10 // Right Sensor
#define ir5 8  // Rightmost Sensor

// Motor Speed Parameters
int baseSpeed = 140;
int maxSpeed = 200;

// PID Constants
float Kp = 10.0;
float Ki = 0.9;
float Kd = 5.0;

// PID Variables
int previousError = 0;
float integral = 0;
int lastKnownError = 0;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);

  Serial.begin(9600);
}

void loop() {

  // Read IR Sensor Values
  int s1 = digitalRead(ir1) == LOW ? 1 : 0;
  int s2 = digitalRead(ir2) == LOW ? 1 : 0;
  int s3 = digitalRead(ir3) == LOW ? 1 : 0;
  int s4 = digitalRead(ir4) == LOW ? 1 : 0;
  int s5 = digitalRead(ir5) == LOW ? 1 : 0;

  // Calculate Position Error
  int weights[5] = {-2, -1, 0, 1, 2};

  int error = 0;
  int totalActiveSensors = 0;

  int sensors[5] = {s1, s2, s3, s4, s5};

  for (int i = 0; i < 5; i++) {
    error += sensors[i] * weights[i];
    totalActiveSensors += sensors[i];
  }

  // Line Loss Detection and Recovery
  if (totalActiveSensors == 0) {

    if (lastKnownError < 0)
      setMotors(-baseSpeed / 2, baseSpeed / 2);

    else if (lastKnownError > 0)
      setMotors(baseSpeed / 2, -baseSpeed / 2);

    else
      setMotors(-baseSpeed / 2, baseSpeed / 2);

    return;
  }

  // Calculate Average Error
  error /= totalActiveSensors;
  lastKnownError = error;

  // PID Calculation
  float proportional = error * Kp;

  integral = constrain(integral + error, -50, 50);
  float integralTerm = integral * Ki;

  float derivative = (error - previousError) * Kd;

  int pid = proportional + integralTerm + derivative;

  previousError = error;

  // Dynamic Base Speed Adjustment
  int dynamicBaseSpeed = map(abs(error), 0, 2, baseSpeed, baseSpeed / 2);

  // Calculate Motor Speeds
  int leftSpeed = constrain(dynamicBaseSpeed + pid, 0, maxSpeed);
  int rightSpeed = constrain(dynamicBaseSpeed - pid, 0, maxSpeed);

  // Apply Motor Speeds
  setMotors(leftSpeed, rightSpeed);

  // Serial Monitor Output
  Serial.print("Error: ");
  Serial.print(error);

  Serial.print(" | PID: ");
  Serial.print(pid);

  Serial.print(" | Left: ");
  Serial.print(leftSpeed);

  Serial.print(" | Right: ");
  Serial.println(rightSpeed);
}

// Motor Control Function
void setMotors(int leftSpeed, int rightSpeed) {

  // Right Motor Control
  analogWrite(enA, abs(rightSpeed));

  digitalWrite(in1, rightSpeed > 0 ? HIGH : LOW);
  digitalWrite(in2, rightSpeed > 0 ? LOW : HIGH);

  // Left Motor Control
  analogWrite(enB, abs(leftSpeed));

  digitalWrite(in3, leftSpeed > 0 ? HIGH : LOW);
  digitalWrite(in4, leftSpeed > 0 ? LOW : HIGH);
}
