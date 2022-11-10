/*********
Saurabh Ritu 
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>

// Replace the next variables with your SSID/Password combination

const char* ssid = "MIMAX2";
const char* password = "Sabudh123";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.43.173"; 

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//dummy varibales
float value_1 = 0;
float value_2 = 0;

bool statusBMP = false; // for BMP status
bool statusBME = false;

Adafruit_BME280 bme; // uses I2C communication protocol by default

Adafruit_BMP280 bmp; // I2C


void setup() {
  Serial.begin(115200);
  // default settings
  statusBME = bme.begin(0x76);// address 0*76 passed to .begin method to start communication with bme sensor
  statusBMP = bmp.begin(0x76);// address 0*76 passed to .begin method to start communication with bme sensor

  
  if (statusBME) {
  Serial.println("BME sensor found");
  } 
  else if(statusBMP)
  {
  Serial.println("BMP280 sensor found");
    }
  else
  {
    Serial.println("Could not find a valid sensor, check wiring!");
  }
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

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

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add GPIOs with MQTT here on callback

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
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

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    
//    value_1 = random(50,100);

    if(statusBME){
      value_1 = bme.readTemperature();
      value_2 = bme.readHumidity();

    // Convert the value to a char array
    char value_1_buffer[20];
    dtostrf(value_1, 1, 2, value_1_buffer);
    Serial.println("BME280");
    Serial.print("Temp: ");
    Serial.println(value_1_buffer);
    client.publish("esp32/BME280/Temp", value_1_buffer);

//    value_2 = random(50);
    
    // Convert the value to a char array
    char value_2_buffer[20];
    dtostrf(value_2, 1, 2, value_2_buffer);
    Serial.print("Humidity: ");
    Serial.println(value_2_buffer);
    client.publish("esp32/BME280/Humidity", value_2_buffer);
    }
      
    if(statusBMP){
      value_1 = bmp.readTemperature();
      value_2 = bmp.readPressure()/100;

    // Convert the value to a char array
    char value_1_buffer[20];
    dtostrf(value_1, 1, 2, value_1_buffer);
    Serial.println("BMP280");
    Serial.print("Temp: ");
    Serial.println(value_1_buffer);
    client.publish("esp32/BMP280/Temp", value_1_buffer);

//    value_2 = random(50);
    
    // Convert the value to a char array
    char value_2_buffer[20];
    dtostrf(value_2, 1, 2, value_2_buffer);
    Serial.print("Pressure: ");
    Serial.println(value_2_buffer);
    client.publish("esp32/BMP280/Pressure", value_2_buffer);
    }
  }
}
