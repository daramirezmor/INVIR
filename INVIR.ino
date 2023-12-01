#include <WiFi.h>
#include <HTTPClient.h>
#include <Ultrasonic.h>
#include <DHTesp.h>
#include <DHT.h>

const char *ssid = "moto g22_2599";
const char *password = "Daniel123";

#define FIREBASE_HOST "invernadero-65177-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "pYLVmZLWMljg8DqfHLOAytKffoRCNr0IWwKExuec"
String ruta = "Invernadero";

#define trigPin 2
#define echoPin 4
const int humsuelo = 33;
const int humsuelo2 = 32;
const int humsuelo3 = 35;

float t = 0.0; // Declarar t como variable global
float h = 0.0; // Declarar h como variable global
int u = 0; // Declarar u como variable global
int v1;
int v2;
int v3;

Ultrasonic ultrasonic(trigPin, echoPin);
DHTesp dht;


void setup() {
  // Configurar la conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");
  Serial.begin(115200);

  pinMode(humsuelo, INPUT);
  dht.setup(15, DHTesp::DHT11);

}

void loop() {
  
  Ultrasonido();
  humidsuelo();
  humidsuelo2();
  humidsuelo3();
  temphum();

  // Configuración de Firebase usando HTTPClient
  HTTPClient http;
  http.begin("https://" + String(FIREBASE_HOST) + "/" + ruta + ".json"); // URL de Firebase

  // Crea un objeto JSON para almacenar los datos a enviar a Firebase
  String data = "{\"Temperatura\":" + String(t) + ",";
  data += "\"Humedad\":" + String(h) + ",";
  data += "\"Agua\":" + String(u) + ",";
  data += "\"Humedad de piso\":" + String(v1) + ",";
  data += "\"Humedad de piso2\":" + String(v2) + ",";
  data += "\"Humedad de piso3\":" + String(v3) + "}";

  // Realiza una solicitud PUT para actualizar los datos en Firebase
  int httpCode = http.PUT(data);

  if (httpCode > 0) {
    //Serial.printf("[HTTP] PUT... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      //Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] PUT... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();

  delay(10000); // Espera 10 segundos antes de tomar otra medición
}

void Ultrasonido() {
  long distance = ultrasonic.read();
  int m = map(analogRead(distance), 0, 13, 100, 0);
  u = 100 - m;
}

void humidsuelo(){
  v1 = map(analogRead(humsuelo), 0, 4095, 100, 0);
}

void humidsuelo2(){
  v2 = map(analogRead(humsuelo2), 0, 4095, 100, 0);
}

void humidsuelo3(){
  v3 = map(analogRead(humsuelo3), 0, 4095, 100, 0);
}

void temphum(){
  h = dht.getHumidity();
  t = dht.getTemperature(); 

  t += 28;
}
