#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "ssid";
const char* password = "pwd";

DHT dht(19, DHT11);
WebServer server(80);

float temperature = 0;
float humidity = 0;

unsigned long lastGetTemperatureTime = 0;
unsigned long lastGetHumidityTime = 0;

float getTemperatureCelcius(){
  //zeby nie bralo za czesto to sprawdzanie czy minelo 2 sekundy od ostatniego pobrania
  if (millis() - 2000 > lastGetTemperatureTime) {
    temperature = dht.readTemperature();
    lastGetTemperatureTime = millis();
  }
  
  return temperature;
}

float getHumidity(){
  //zeby nie bralo za czesto to sprawdzanie czy minelo 2 sekundy od ostatniego pobrania
  if (millis() - 2000 > lastGetHumidityTime) {
    humidity = dht.readHumidity();
    lastGetHumidityTime = millis();
  }
  
  return humidity;
}

void handleRoot() {
  float temperature = getTemperatureCelcius();
  float humidity = getHumidity();
  String message = "Temperatura: " + String(temperature) + "C" + "\n" + "Wilgotnosc: " + String(humidity) + "%";
  server.send(200, "text/plain", message);
}

void setup() {
  dht.begin();

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Laczenie z wifi");
  }

  Serial.println("Polaczono z wifi");
  Serial.println("Adres stronki: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println("Http serwer wystartowal");
}

void loop() {
  server.handleClient();
  delay(2);
}
