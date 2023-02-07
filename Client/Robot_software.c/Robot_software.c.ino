#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
Servo myservo;
Servo myservo2;
int servoPin = 26;
int servoPin2 =27;

typedef struct test_struct {
  bool x,y;
} test_struct;

test_struct myData;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("x: ");
  Serial.println(myData.x);
  Serial.print("y: ");
  Serial.println(myData.y);
}
 
void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    
  myservo.attach(servoPin, 1000, 2000); 
  myservo2.attach(servoPin2, 1000, 2000); 
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);


  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
if(myData.x == 1 && myData.y == 0){
  myservo.write(120);
  myservo2.write(120);
  }else if(myData.x == 0 && myData.y == 1){
  myservo2.write(30);
  myservo.write(30);
    }else if(myData.x == 1 && myData.y == 1){
  myservo2.write(30);
  myservo.write(120);
    }else{
  myservo2.write(120);
  myservo.write(30);
    }
}
