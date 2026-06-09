#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ===== HC-SR04 =====
#define TRIG_PIN 5
#define ECHO_PIN 18

// ===== MOTOR PINS =====
#define MR1 12
#define MR2 14
#define ML1 27
#define ML2 26

const int freq = 5000;
const int resolution = 8;

int motorSpeed = 150;
bool isGoingForward = false; // Cờ báo hiệu đang tiến

// ===== PWM =====
void setupPWM(int pin) {
  ledcAttach(pin, freq, resolution);
}

// ===== MOTOR =====
void setMotor(int pin1, int pin2, int speed1, int speed2) {
  ledcWrite(pin1, speed1);
  ledcWrite(pin2, speed2);
}

void Forward() {
  setMotor(MR1, MR2, motorSpeed, 0);
  setMotor(ML1, ML2, motorSpeed, 0);
}

void Backward() {
  setMotor(MR1, MR2, 0, motorSpeed);
  setMotor(ML1, ML2, 0, motorSpeed);
}

void Left() {
  setMotor(MR1, MR2, motorSpeed, 0);
  setMotor(ML1, ML2, 0, motorSpeed);
}

void Right() {
  setMotor(MR1, MR2, 0, motorSpeed);
  setMotor(ML1, ML2, motorSpeed, 0);
}

void Stop() {
  setMotor(MR1, MR2, 0, 0);
  setMotor(ML1, ML2, 0, 0);
}

// ===== 4 HÀM ĐI CHÉO MỚI =====
void ForwardLeft() {
  setMotor(MR1, MR2, motorSpeed, 0);       // Bánh phải chạy nhanh
  setMotor(ML1, ML2, motorSpeed / 2, 0);   // Bánh trái chạy chậm -> Chếch trái
}

void ForwardRight() {
  setMotor(MR1, MR2, motorSpeed / 2, 0);   // Bánh phải chạy chậm
  setMotor(ML1, ML2, motorSpeed, 0);       // Bánh trái chạy nhanh -> Chếch phải
}

void BackwardLeft() {
  setMotor(MR1, MR2, 0, motorSpeed);       // Lùi phải nhanh
  setMotor(ML1, ML2, 0, motorSpeed / 2);   // Lùi trái chậm
}

void BackwardRight() {
  setMotor(MR1, MR2, 0, motorSpeed / 2);   // Lùi phải chậm
  setMotor(ML1, ML2, 0, motorSpeed);       // Lùi trái nhanh
}

// ===== HC-SR04 =====
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999; 
  }

  float distance = duration * 0.0343 / 2.0;
  return distance;
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  SerialBT.begin("BVCAR");

  setupPWM(MR1);
  setupPWM(MR2);
  setupPWM(ML1);
  setupPWM(ML2);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Stop();

  Serial.println("Bluetooth Car Ready");
}

// ===== LOOP =====
void loop() {
  float distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  SerialBT.print("Distance: ");
  SerialBT.print(distance);
  SerialBT.println(" cm");

  // Dừng khẩn cấp khi có vật cản VÀ xe đang lao tới
  if (distance <= 10 && isGoingForward == true) {
    Stop();
    isGoingForward = false;
    SerialBT.println("Obstacle detected!");
  }

  if (SerialBT.available()) {
    String data = SerialBT.readStringUntil('\n');
    data.trim(); 

    if (data.length() == 0) return;

    char cmd = data.charAt(0);

    if (data.length() > 1) {
      int spd = data.substring(1).toInt();
      if (spd >= 0 && spd <= 255) {
        motorSpeed = spd;
      }
    }

    switch (cmd) {
      case 'F':
        if (distance > 10) {
          Forward();
          isGoingForward = true;
        } else {
          Stop();
          isGoingForward = false;
          SerialBT.println("Obstacle detected!"); 
        }
        break;

      case 'B':
        Backward();
        isGoingForward = false;
        break;

      case 'L':
        Left();
        isGoingForward = false;
        break;

      case 'R':
        Right();
        isGoingForward = false;
        break;

      case 'S':
        Stop();
        isGoingForward = false;
        break;

      // ===== NHẬN ĐIỀU KHIỂN CHÉO TỪ QT C++ =====
      case 'Q': // Tiến trái chếch
        if (distance > 10) { ForwardLeft(); isGoingForward = true; }
        else { Stop(); isGoingForward = false; SerialBT.println("Obstacle detected!"); }
        break;
      case 'E': // Tiến phải chếch
        if (distance > 10) { ForwardRight(); isGoingForward = true; }
        else { Stop(); isGoingForward = false; SerialBT.println("Obstacle detected!"); }
        break;
      case 'Z': // Lùi trái chếch
        BackwardLeft(); isGoingForward = false; break;
      case 'C': // Lùi phải chếch
        BackwardRight(); isGoingForward = false; break;
    }
  }
  delay(20); 
}
