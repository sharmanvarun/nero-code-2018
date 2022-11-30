/*
 This code basically sets up an ESP8266 board as a MQTT client and helps in subscribing to desired topics that are published.

 It is required that the board in the 'Tools' section must be selected to NodeMCU and the ESP8266 drivers are installed.
 The baud rate must be set at 115200
 Here we are operating three servo motors using the nodeMCU based on the values received via the MQTT topic
 
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
// Creating objects for the 3 servo motors
Servo servo;
Servo servo1;
Servo servo2;
// The pins we have used for the servo motors are D1,D2 and D3
// Update these with values suitable for your network.

const char* ssid = "CDI";
const char* password = "Cdi*1717";
const char* mqtt_server = "172.16.73.4";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char old ;

void setup() {
  //Initial Setup Code
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(1000);
}
//Code for connecting to the wifi
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
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

//This function is responsible for subscribing to a topic and receiving the values from it 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
 
  old = (char)payload[0];
  //detach each time to avoid jitter of the servo motors
  servo2.detach();
  servo.detach();
  servo1.detach();
 for (int i = 0; i < length; i++) {
  Serial.println("Received message is :");
  Serial.println((char)payload[i]);
  servo.detach();
  servo1.detach();
  servo2.detach();
  if((char)payload[i] == '1')
  {

    servo.detach();
    servo2.detach();
    servo1.detach();
    servo.attach(0);
    servo.write(89);
    delay(1000);
    servo.detach();
    servo1.attach(4);
    servo1.write(89);
    delay(1000);
    servo1.detach();
    servo2.detach();
    old='1';
    break;
  }
if((char)payload[i] == '2')
  {
    servo.detach();
    servo2.detach();
    servo1.detach();
    servo1.attach(4);
    servo1.write(89);
    delay(1000);
    servo1.detach();
    servo2.detach();
    old='2';
    break;
  }
 if((char)payload[i] == '3')
  {
    servo.detach();
    servo2.detach();
    servo1.detach();
    servo.attach(0);
    servo.write(89);
    delay(1000);
    servo.detach();
    servo2.detach();
    old='3';
    break;
  }
 if((char)payload[i] == '4')
  {
    servo.detach();
    servo1.detach();
    servo2.detach();
    servo.attach(0);
    servo.write(180);
    delay(1000);
    servo.detach();
    servo1.attach(4);
    servo1.write(180);
    delay(1000);
    servo1.detach();
    servo2.detach();
    old='4';
    break;
  }
  if((char)payload[i]=='5')
  {
    servo2.detach();
    servo2.attach(5);
    servo2.write(179);
    delay(1000);
    servo2.detach();
  }
  if((char)payload[i]=='6')
  { servo2.detach();
    servo2.attach(5);
    servo2.write(89);
    delay(1000);
    servo2.detach();
   }
  if((char)payload[i]=='7')
  { servo2.detach();
    servo2.attach(5);
    servo2.write(2);
    delay(1000);
    servo2.detach();
  }
    servo2.detach();
  }

    servo.detach();
    servo1.detach();
    servo2.detach();
}
//This function is to reconnect once the connection is lost
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
      // ... and resubscribe
      client.subscribe("Ultrasonic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

 
}