#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int luz  = 5;    //D1

char message_buff[100];

const char* ssid = "Club Arduino Chile";
const char* password = "club2017";
const char* mqtt_server = "mqtt.cyberplus.cl";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(luz, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  for (i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);

  if (msgString.equals("off")) {
    digitalWrite(luz, LOW);
  } else if (msgString.equals("on")) {
    digitalWrite(luz, HIGH);
  }

}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("NodeMCU_Demo")) {
      client.publish("ClubArduino/Demo", "Hola, soy el NodeMCU de Club Arduino");
      client.subscribe("ClubArduino/Demo");
    } else {
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
