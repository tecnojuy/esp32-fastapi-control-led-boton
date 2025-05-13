#include <WiFi.h>
#include <HTTPClient.h>

// Configuración del LED y BOTÓN
const int ledPin = 2;       // El LED integrado en muchos ESP32 está en el pin 2
const int buttonPin = 4;    // Botón conectado al pin GPIO4

// Variables para manejar el estado del botón
int buttonState = LOW;      // Estado actual del botón
int lastButtonState = LOW;  // Estado anterior del botón
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Variables para controlar tiempos
unsigned long lastLedCheck = 0;
const unsigned long ledCheckInterval = 1000;

// Configuración WiFi
const char* ssid = "TU_WIFI";
const char* password = "TU_CONTRASEÑA";

// Configuración del servidor - ASEGÚRATE DE PONER LA IP CORRECTA
// Para saber cual es la ip de tu pc, puedes usar el comando:
// ipconfig en windows o ifconfig en linux
// y buscas la de direccion ipv4

const char* serverUrl = "http://TU_IP:8000";

void setup() {
  Serial.begin(115200);
  
  // Configurar pines
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);  // Para pull-down externo
  
  // Prueba inicial del botón
  Serial.println("Probando botón...");
  Serial.print("Estado inicial del botón: ");
  Serial.println(digitalRead(buttonPin));
  Serial.println("Presiona el botón para verificar la detección..");
  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando a WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  
  // Verificar conexión al servidor
  testServerConnection();
}

void loop() {
  // Comprobar si estamos conectados a WiFi
  if (WiFi.status() == WL_CONNECTED) {
    
    // 1. Verificar estado del LED periódicamente
    unsigned long currentMillis = millis();
    if (currentMillis - lastLedCheck >= ledCheckInterval) {
      lastLedCheck = currentMillis;
      checkLedStatus();
    }
    
    // 2. Leer y procesar el botón
    readButton();
    
  } else {
    Serial.println("WiFi desconectado. Reconectando...");
    WiFi.reconnect();
    delay(5000);
  }
  
  delay(10);
}

// Verificar conexión al servidor
void testServerConnection() {
  HTTPClient http;
  String url = String(serverUrl);
  
  Serial.println("Probando conexión al servidor: " + url);
  
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode > 0) {
    Serial.println("Conexión exitosa al servidor. Código: " + String(httpCode));
    String payload = http.getString();
    Serial.println("Respuesta: " + payload);
  } else {
    Serial.println("Error al conectar al servidor: " + http.errorToString(httpCode));
  }
  
  http.end();
}

// Consultar estado del LED
void checkLedStatus() {
  HTTPClient http;
  String url = String(serverUrl) + "/led/status";
  
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode == 200) {
    String payload = http.getString();
    if (payload.indexOf("ON") >= 0) {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ENCENDIDO");
    } else if (payload.indexOf("OFF") >= 0) {
      digitalWrite(ledPin, LOW);
      Serial.println("LED APAGADO");
    }
  } else {
    Serial.println("Error al consultar LED: " + String(httpCode));
  }
  
  http.end();
}

// Leer el botón con detección mejorada
void readButton() {
  // Leer el estado actual
  int reading = digitalRead(buttonPin);
  
  // Imprimir estado del botón cada 500ms para depuración
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime > 500) {
    Serial.print("Estado actual del botón: ");
    Serial.println(reading);
    lastPrintTime = millis();
  }
  
  // Si el estado cambió, reiniciar el timer de debounce
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  // Verificar si ha pasado suficiente tiempo desde el último cambio
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Si el estado ha cambiado realmente
    if (reading != buttonState) {
      buttonState = reading;
      
      // Si el botón ahora está presionado (HIGH para pull-down)
      if (buttonState == HIGH) {
        Serial.println("¡BOTÓN PRESIONADO! Enviando al servidor...");
        incrementButtonCounter();
      }
    }
  }
  
  // Guardar el último estado leído
  lastButtonState = reading;
}

// Enviar incremento al servidor
void incrementButtonCounter() {
  HTTPClient http;
  String url = String(serverUrl) + "/button/increment";
  
  Serial.println("Enviando incremento a: " + url);
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST("{}");
  
  if (httpCode == 200) {
    String payload = http.getString();
    Serial.println("¡Incremento exitoso! Respuesta: " + payload);
  } else {
    Serial.println("Error al incrementar contador. Código: " + String(httpCode));
    Serial.println("Error: " + http.errorToString(httpCode));
  }
  
  http.end();
}
