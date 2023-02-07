#include <ESP8266WiFi.h>
#include <espnow.h>


// 94:B9:7E:EA:81:04
uint8_t broadcastAddress[] = {0x94, 0xB9, 0x7E, 0xEA, 0x81, 0x04};
const int SOFT_POT_PIN = 5;
const int SOFT_POT_PIN2 = 4;

typedef struct struct_message {
bool x,y;
} struct_message;


struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 20;  


void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {

  Serial.begin(115200);
  pinMode(SOFT_POT_PIN, INPUT);
  pinMode(SOFT_POT_PIN2, INPUT);
  WiFi.mode(WIFI_STA);


  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
     
    myData.x = digitalRead(SOFT_POT_PIN);
    myData.y = digitalRead(SOFT_POT_PIN2);
    
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}
