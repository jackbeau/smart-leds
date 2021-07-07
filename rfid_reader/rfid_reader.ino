// SIMPLIFIED FIRMWARE FOR ESP8266 WITH MFRC522 WITHOUT LEDS  

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DOOR2_ALIAS "Open Gate"
#define MQTT_DOOR2_STATUS_TOPIC "front/switch/gate/status"
#define DOOR2_STATUS_PIN D1
#define DOOR2_STATUS_SWITCH_LOGIC "NO"
int debounceTime = 2000;
const char* door2_alias = DOOR2_ALIAS;
const char* mqtt_door2_status_topic = MQTT_DOOR2_STATUS_TOPIC;;
const int door2_statusPin = DOOR2_STATUS_PIN;
const char* door2_statusSwitchLogic = DOOR2_STATUS_SWITCH_LOGIC;
int door2_lastStatusValue = 2;
unsigned long door2_lastSwitchTime = 0;
const int redPin = D0;
const int greenPin = D2;

// Wifi Connection details
const char* ssid = "";
const char* password = "";

const char* mqtt_server = "";  // mqtt IP
const char* mqtt_user = "";  // mqtt user
const char* mqtt_password = "";  // mqtt password
const char* clientID = "RFID";  // rfid name
const char* rfid_topic = "home/rfid";  // rfid path

WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(door2_statusPin, INPUT_PULLUP);
  door2_lastStatusValue = digitalRead(door2_statusPin);
  Serial.begin(115200);
  delay(100);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
  delay(2000);
}

// Connect to Wifi
void setup_wifi() {
  //Turn off Access Point
  WiFi.mode(WIFI_STA);
  analogWrite(redPin, 238);
  analogWrite(greenPin, 130);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
}
// Check for incoming messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("");
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char s[20];
  
  sprintf(s, "%s", payload);

    if ( (strcmp(topic,"home/alarm/rfid")==0))
  {
    payload[length] = '\0';
    String sp = String((char*)payload);

  // Alarm is off
    if (sp == "accept")
    {
    Serial.println();
    Serial.print("RFID Accepted");
    Serial.println();  
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    delay(2000);  
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
  }   
  // Alarm is Armed Home    
    else if (sp == "refuse")
    {
    Serial.println();
    Serial.print("RFID Refused");
    Serial.println(); 
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    delay(2000);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
  }  
 }
}
// Reconnect to wifi if connection lost
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {;
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("home/rfid", "connected");
      // ... and resubscribe
      client.subscribe("home/alarm/rfid");
     analogWrite(redPin, 0);
     analogWrite(greenPin, 200);
     delay(500);
     analogWrite(redPin, 0);
     analogWrite(greenPin, 0);
     publish_door2_status();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Main functions
void loop() {
  check_door2_status();
  client.loop();
  delay(10); 
  }

void publish_door2_status() {
  if (digitalRead(door2_statusPin) == LOW) {
    if (door2_statusSwitchLogic == "NO") {
      Serial.print(door2_alias);
      Serial.print(" closed! Publishing to ");
      Serial.print(mqtt_door2_status_topic);
      Serial.println("...");
      client.publish(mqtt_door2_status_topic, "closed", true);
    }
    else if (door2_statusSwitchLogic == "NC") {
      Serial.print(door2_alias);
      Serial.print(" open! Publishing to ");
      Serial.print(mqtt_door2_status_topic);
      Serial.println("...");
      client.publish(mqtt_door2_status_topic, "open", true);      
    }
    else {
      Serial.println("Error! Specify only either NO or NC for DOOR1_STATUS_SWITCH_LOGIC in config.h! Not publishing...");
    }
  }
  else {
    if (door2_statusSwitchLogic == "NO") {
      Serial.print(door2_alias);
      Serial.print(" open! Publishing to ");
      Serial.print(mqtt_door2_status_topic);
      Serial.println("...");
      client.publish(mqtt_door2_status_topic, "open", true);
    }
    else if (door2_statusSwitchLogic == "NC") {
      Serial.print(door2_alias);
      Serial.print(" closed! Publishing to ");
      Serial.print(mqtt_door2_status_topic);
      Serial.println("...");
      client.publish(mqtt_door2_status_topic, "closed", true);      
    }
    else {
      Serial.println("Error! Specify only either NO or NC for DOOR1_STATUS_SWITCH_LOGIC in config.h! Not publishing...");
    }
  }
}

void check_door2_status() {
  int currentStatusValue = digitalRead(door2_statusPin);
  if (currentStatusValue != door2_lastStatusValue) {
    unsigned int currentTime = millis();
    if (currentTime - door2_lastSwitchTime >= debounceTime) {
      publish_door2_status();
      door2_lastStatusValue = currentStatusValue;
      door2_lastSwitchTime = currentTime;
    }
  }
}
