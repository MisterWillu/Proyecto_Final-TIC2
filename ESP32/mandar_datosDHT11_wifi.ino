#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "DHT.h"

// Define los pines a los que están conectados los sensores DHT11
#define DHTPIN1 15
#define DHTPIN2 16
#define DHTPIN3 17
#define DHTPIN4 18
#define DHTPIN5 19

#define DHTTYPE DHT11   // El tipo de sensor DHT que estás utilizando

// Define el ancho y el alto de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define la dirección I2C de la pantalla OLED
#define OLED_ADDR 0x3C

// Define el pin de reseteo de la pantalla OLED (en caso de que tu pantalla lo tenga)
#define OLED_RESET    -1

// Crea instancias de los sensores DHT
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
DHT dht5(DHTPIN5, DHTTYPE);

// Crea una instancia para la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configura tu red WiFi
const char* ssid = "LAB.SISTEMAS DE COMUNICACIONES";
const char* password = "Comunicaciones";

// URL del servidor para enviar los datos
const char* serverName = "http://192.168.0.49:5000/datos";

void setup() {
  Serial.begin(115200);

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la WiFi");

  // Inicializa los sensores DHT
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();

  // Inicializa la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Limpia el buffer de la pantalla
  display.clearDisplay();
  display.setTextSize(1);      // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(0,0);     // Cursor en la posición (0,0)
  display.display();
}

void loop() {
  delay(2000);  // Espera 2 segundos entre lecturas

  // Lee la humedad y temperatura de cada sensor
  float humedad1 = dht1.readHumidity();
  float temperatura1 = dht1.readTemperature();

  float humedad2 = dht2.readHumidity();
  float temperatura2 = dht2.readTemperature();

  float humedad3 = dht3.readHumidity();
  float temperatura3 = dht3.readTemperature();

  float humedad4 = dht4.readHumidity();
  float temperatura4 = dht4.readTemperature();

  float humedad5 = dht5.readHumidity();
  float temperatura5 = dht5.readTemperature();

  // Limpia la pantalla antes de mostrar los nuevos datos
  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("Datos sensores DHT11");

  // Comprueba si las lecturas de los sensores fueron exitosas y muestra los valores en la pantalla OLED
  if (isnan(humedad1) || isnan(temperatura1)) {
    display.setCursor(0, 10);
    display.print("Error al leer DHT1");
  } else {
    display.setCursor(0, 10);
    display.print("S1 - T:");
    display.print(temperatura1);
    display.print(" H:");
    display.print(humedad1);
  }

  if (isnan(humedad2) || isnan(temperatura2)) {
    display.setCursor(0, 20);
    display.print("Error al leer DHT2");
  } else {
    display.setCursor(0, 20);
    display.print("S2 - T:");
    display.print(temperatura2);
    display.print(" H:");
    display.print(humedad2);
  }

  if (isnan(humedad3) || isnan(temperatura3)) {
    display.setCursor(0, 30);
    display.print("Error al leer DHT3");
  } else {
    display.setCursor(0, 30);
    display.print("S3 - T:");
    display.print(temperatura3);
    display.print(" H:");
    display.print(humedad3);
  }

  if (isnan(humedad4) || isnan(temperatura4)) {
    display.setCursor(0, 40);
    display.print("Error al leer DHT4");
  } else {
    display.setCursor(0, 40);
    display.print("S4 - T:");
    display.print(temperatura4);
    display.print(" H:");
    display.print(humedad4);
  }

  if (isnan(humedad5) || isnan(temperatura5)) {
    display.setCursor(0, 50);
    display.print("Error al leer DHT5");
  } else {
    display.setCursor(0, 50);
    display.print("S5 - T:");
    display.print(temperatura5);
    display.print(" H:");
    display.print(humedad5);
  }

  // Actualiza la pantalla para mostrar los nuevos datos
  display.display();

  // También imprime los valores en el monitor serial
  if (!isnan(humedad1) && !isnan(temperatura1)) {
    Serial.print("S1-T:");
    Serial.print(temperatura1);
    Serial.print("H:");
    Serial.println(humedad1);
  } else {
    Serial.println("Error al leer el sensor DHT1!");
  }

  if (!isnan(humedad2) && !isnan(temperatura2)) {
    Serial.print("S2-T:");
    Serial.print(temperatura2);
    Serial.print("H:");
    Serial.println(humedad2);
  } else {
    Serial.println("Error al leer el sensor DHT2!");
  }

  if (!isnan(humedad3) && !isnan(temperatura3)) {
    Serial.print("S3-T:");
    Serial.print(temperatura3);
    Serial.print("H:");
    Serial.println(humedad3);
  } else {
    Serial.println("Error al leer el sensor DHT3!");
  }

  if (!isnan(humedad4) && !isnan(temperatura4)) {
    Serial.print("S4-T:");
    Serial.print(temperatura4);
    Serial.print("H:");
    Serial.println(humedad4);
  } else {
    Serial.println("Error al leer el sensor DHT4!");
  }

  if (!isnan(humedad5) && !isnan(temperatura5)) {
    Serial.print("S5-T:");
    Serial.print(temperatura5);
    Serial.print("H:");
    Serial.println(humedad5);
  } else {
    Serial.println("Error al leer el sensor DHT5!");
  }

  // Enviar datos al servidor
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "temp1=" + String(temperatura1) + "&hum1=" + String(humedad1)
                           + "&temp2=" + String(temperatura2) + "&hum2=" + String(humedad2)
                           + "&temp3=" + String(temperatura3) + "&hum3=" + String(humedad3)
                           + "&temp4=" + String(temperatura4) + "&hum4=" + String(humedad4)
                           + "&temp5=" + String(temperatura5) + "&hum5=" + String(humedad5);
    
    int httpResponseCode = http.POST(httpRequestData);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error en la petición: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi desconectado");
  }
}