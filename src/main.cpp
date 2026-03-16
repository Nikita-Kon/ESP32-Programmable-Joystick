#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "driver/gpio.h"
#include "esp_sleep.h"

// Use the RAW GPIO numbers for the C3
#define PIN_X 0  // Horizontal
#define PIN_Y 1  // Vertical
#define PIN_B 2  // Button

typedef struct {
  int x;
  int y;
  int btn;
}JoystickData;

void goToSleep() {
  Serial.println("Going to deep sleep...");
  delay(100);
  esp_now_deinit();
  WiFi.mode(WIFI_OFF);
  digitalWrite(4, LOW);
  delay(50);
  esp_deep_sleep_enable_gpio_wakeup(1ULL << PIN_B, ESP_GPIO_WAKEUP_GPIO_LOW);
  esp_deep_sleep_start();

}

uint8_t receiverMAC[] = {0x3C, 0x8A, 0x1F, 0x9D, 0x43, 0xC4};

// sleepTimer
unsigned long startTime = 0;
const unsigned long interval = 60000;

void onSent(const uint8_t * mac, esp_now_send_status_t status){
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sent!" : "Failed!");
}

void setup() {
  Serial.begin(115200);
  delay(2000); 


  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  // // Set attenuation - this ensures the ESP32 can read the full 0-3.3V range
  analogSetAttenuation(ADC_11db); 
  analogReadResolution(12);
  
  // p2p setup
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMAC, 6);
  esp_now_add_peer(&peer);

  // Sleep timer
  startTime = millis();
  delay(100);
  
}

void loop() {
  JoystickData joystick;
  joystick.x = analogRead(PIN_X);
  joystick.y = analogRead(PIN_Y);
  joystick.btn = digitalRead(PIN_B);

  esp_now_send(receiverMAC, (uint8_t *)&joystick, sizeof(joystick));

  // button pressed - update timer
  if(joystick.btn == 0){
    startTime = millis();
    delay(100);
  }
  // check sleep timer
  if (millis() - startTime >= interval){
    Serial.println("Going to deep sleep...");
    goToSleep();
  }
  delay(100);
}

