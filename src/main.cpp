#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <WiFiManager.h>
WiFiManager my_wifi;
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);

// RTOS FUNCTION TO MULTITASKING
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
TaskHandle_t task1Handle;
TaskHandle_t task1Handle2;

// USING GPIO AS A BUTTON
#define BUTTON_PIN 0
unsigned int _STATE_ = 0;
bool BT_CS;
bool BT_PS = 1;

// ONBOARD LED PIN (ESP32)
#define BUILTIN_LED 2

// separate loop for button interrupt
void task1(void *pvParameters)
{
  while (true)
  {
    BT_CS = digitalRead(BUTTON_PIN);
    if (BT_CS == 0 && BT_PS != BUTTON_PIN)
    {
      _STATE_ += 1;
      if(_STATE_==12){Serial.println("0");}
      else{Serial.println(_STATE_);}
    }
    BT_PS = BT_CS;
    vTaskDelay(pdMS_TO_TICKS(150)); // Delay for 1 second
  }
}

// separate loop for LED BLINK
void task2(void *pnParameters)
{
  while (true)
  {
    switch (_STATE_)
    {
    case 0:
      digitalWrite(BUILTIN_LED, LOW);
      break;  

    case 1:
      digitalWrite(BUILTIN_LED, HIGH);
      delay(1024);
      digitalWrite(BUILTIN_LED, LOW);
      delay(1024);
      break;

    case 2:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(512);
      digitalWrite(BUILTIN_LED,LOW);
      delay(512);
      break;

    case 3:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(256);
      digitalWrite(BUILTIN_LED,LOW);
      delay(256);
      break;

    case 4:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(128);
      digitalWrite(BUILTIN_LED,LOW);
      delay(128);
      break;

    case 5:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(64);
      digitalWrite(BUILTIN_LED,LOW);
      delay(64);
      break;

    case 6:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(32);
      digitalWrite(BUILTIN_LED,LOW);
      delay(32);
      break;

    case 7:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(16);
      digitalWrite(BUILTIN_LED,LOW);
      delay(16);
      break;

    case 8:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(8);
      digitalWrite(BUILTIN_LED,LOW);
      delay(8);
      break;

    case 9:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(4);
      digitalWrite(BUILTIN_LED,LOW);
      delay(4);
      break;

    case 10:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(2);
      digitalWrite(BUILTIN_LED,LOW);
      delay(2);
      break;              

    case 11:
      digitalWrite(BUILTIN_LED,HIGH);
      delay(1);
      digitalWrite(BUILTIN_LED,LOW);
      delay(1);
      break;

      default : break;
    }
    vTaskDelay(pdMS_TO_TICKS(25)); // Delay for 1 second
  }
}

void setup(void) {
  Serial.begin(115200);
  pinMode(BUILTIN_LED,OUTPUT);
  
  WiFi.mode(WIFI_STA);
  my_wifi.setTimeout(300); // hotspot visibility for 3 minutes
  my_wifi.autoConnect("OTA_TEST", "OTA_TEST");
  Serial.println("Connected via WifiManager.....");
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  
  pinMode(2,OUTPUT);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();
  Serial.println("HTTP server started");

  xTaskCreate(task1, "Task 1", 10000, NULL, 1, &task1Handle);
  xTaskCreate(task2, "Task 2", 10000, NULL, 1, &task1Handle2);
}

void loop(void) {
 if (_STATE_ == 12)
  {
    _STATE_ = 0;
  }
  delay(500);
}