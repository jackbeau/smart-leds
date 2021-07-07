/* Add your keys & rename this file to auth.h */

#ifndef _AUTH_DETAILS
#define _AUTH_DETAILS

// Wifi Parameters
#define WIFI_SSID ""
#define WIFI_PASSWORD ""


// Mqtt Parameters
#define MQTT_SERVER ""
#define MQTT_ID "Front Sensors"
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_PORT 1883
char* MQTT_STATE_TOPIC_PREFIX = "led/"; // e.g. led/<deviceName> and led/<deviceName>/set

// OTA Parameters
#define OTApassword "" //the password you will need to enter to upload remotely via the ArduinoIDE
#define OTAport 8266

// LED strip Parameters
#define DATA_PIN_LEDS   D5
#define DATA_PIN_RELAY  D3 

// Relay Parameters

#define ACTIVE_HIGH_RELAY false

// Door 1 Parameters

#define DOOR1_ALIAS "Gate"
#define MQTT_DOOR1_ACTION_TOPIC "front/door/gate/action"
#define MQTT_DOOR1_STATUS_TOPIC "front/door/gate/status"
#define DOOR1_OPEN_PIN D7
#define DOOR1_CLOSE_PIN D7
#define DOOR1_STATUS_PIN D2
#define DOOR1_STATUS_SWITCH_LOGIC "NO"

#define DOOR2_ALIAS "Light Switch"
#define MQTT_DOOR2_STATUS_TOPIC "front/switch/stair_lights/status"
#define DOOR2_STATUS_PIN D1
#define DOOR2_STATUS_SWITCH_LOGIC "NO"

/******************************** CONFIG SETUP *******************************/
#define LED_COUNT_MAXIMUM 800 // Memory allocation. This will reduced the refresh rate so make as small as the maximum number of LED's you have in your config below.
int ledCount = 60; // Default number of leds if no specific config has been set
char* deviceName = "led";
byte maxBrightness = 255;

void setup_config() {
  Serial.println();
  Serial.print("ESP8266 Chip id: ");
  Serial.println(ESP.getChipId());

  if (ESP.getChipId() == 5186042) {
    deviceName = "kitchen";
    ledCount = 246; // 2.14m x 60 per meter
    maxBrightness = 155;  // 60% - Brightness Percentage (This is used to limit the current from the power supply) - 2.4A with 128 leds on RGB White (60ma)
  }

  Serial.print("Device Name: ");
  Serial.println(deviceName);
}

#endif

