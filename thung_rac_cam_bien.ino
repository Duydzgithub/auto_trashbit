#include <Servo.h>        // Thư viện điều khiển Servo
#include <SimpleTimer.h>  // Thư viện SimpleTimer

Servo myServo;            // Khai báo đối tượng Servo
SimpleTimer timer;        // Khai báo đối tượng SimpleTimer
#define INPUT_PIN A0      // Chân A0 để đọc cảm biến

int servoState = 0;       // Trạng thái servo: 0 (chưa xoay), 1 (đã xoay về 0°), 2 (đã xoay về 80°)
unsigned long lastChangeTime = 0; // Thời điểm cuối cùng trạng thái thay đổi
const long waitTime = 3000;       // Thời gian chờ 3 giây

void setup() {
  Serial.begin(9600);
  myServo.attach(9);  // Gắn servo vào chân D9
  myServo.write(100); // Đặt servo về vị trí ban đầu (100°)
}

void moveServoTo(int targetPos, int stepDelay) {
  int currentPos = myServo.read(); // Lấy vị trí hiện tại của servo
  if (currentPos < targetPos) {
    for (int pos = currentPos; pos <= targetPos; pos++) {
      myServo.write(pos);
      delay(stepDelay); // Giữ delay nhỏ để servo xoay mượt
    }
  } else {
    for (int pos = currentPos; pos >= targetPos; pos--) {
      myServo.write(pos);
      delay(stepDelay);
    }
  }
}

void checkSensor() {
  int value = analogRead(INPUT_PIN); // Đọc giá trị cảm biến
  Serial.println(value);

  if (value > 800) {
    if (servoState == 0) { // Nếu servo chưa xoay
      moveServoTo(0, 20);  // Xoay từ 100° về 0°
      servoState = 1;      // Cập nhật trạng thái
      lastChangeTime = millis(); // Lưu thời điểm thay đổi
    } else if (servoState == 2) { // Nếu servo đang ở 80°
      moveServoTo(0, 20);  // Xoay từ 80° về 0°
      servoState = 1;      // Cập nhật trạng thái
      lastChangeTime = millis();
    }
  } else { // Nếu giá trị nhỏ hơn hoặc bằng 800
    if (servoState == 1 && (millis() - lastChangeTime >= waitTime)) {
      // Nếu đã ở trạng thái 1 (0°) và đủ 3 giây
      moveServoTo(80, 15); // Xoay từ 0° về 80°
      servoState = 2;      // Cập nhật trạng thái
      lastChangeTime = millis();
    }
  }
}

void loop() {
  
  checkSensor(); // Kiểm tra cảm biến liên tục
  // Không cần delay ở đây, chương trình chạy mượt mà
}