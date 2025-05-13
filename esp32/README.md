# Código ESP32 para Control IoT

Este directorio contiene el código Arduino para ESP32 que permite la comunicación con el servidor backend y el control físico del LED y el botón.

## Estructura 

esp32/
└── arduino_code/
└── arduino_code.ino # Código principal para el ESP32

## Componentes principales del código

### `arduino_code.ino`

Este archivo contiene la implementación completa para el ESP32, que incluye:

#### Configuración de pines y variables

```cpp
// Configuración del LED y BOTÓN
const int ledPin = 2;       // El LED integrado en muchos ESP32 está en el pin 2
const int buttonPin = 4;    // Botón conectado al pin GPIO4

// Variables para manejar el estado del botón
int buttonState = LOW;      // Estado actual del botón
int lastButtonState = LOW;  // Estado anterior del botón
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
```

- **ledPin**: Pin GPIO donde se conecta el LED (GPIO2 por defecto)
- **buttonPin**: Pin GPIO donde se conecta el botón (GPIO4 por defecto)
- **Variables de debounce**: Para evitar lecturas múltiples al presionar el botón una vez

#### Configuración de WiFi y servidor

```cpp
// Configuración WiFi
const char* ssid = "TU_WIFI";
const char* password = "TU_CONTRASEÑA";

// Configuración del servidor - ASEGÚRATE DE PONER LA IP CORRECTA
// Para saber cual es la ip de tu pc, puedes usar el comando:
// ipconfig en windows o ifconfig en linux
const char* serverUrl = "http://TU_IP:8000";
```

- **ssid y password**: Credenciales de la red WiFi 
- **serverUrl**: URL del servidor backend (debe incluir la IP de tu computadora)

#### Función de inicialización (setup)

```cpp
void setup() {
  Serial.begin(115200);
  
  // Configurar pines
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);  // Para pull-down externo
  
  // Prueba inicial del botón
  Serial.println("Probando botón...");
  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando a WiFi...");
  
  // ...
  
  // Verificar conexión al servidor
  testServerConnection();
}
```

- **Configuración de pines**: Define los modos de los pines (OUTPUT para LED, INPUT para botón)
- **Comunicación Serial**: Para depuración a 115200 baudios
- **Conexión WiFi**: Inicia la conexión a la red WiFi configurada
- **Prueba de conectividad**: Verifica que pueda conectarse al servidor backend

#### Bucle principal (loop)

```cpp
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
```

- **Verificación de WiFi**: Comprueba que la conexión WiFi esté activa
- **Control del LED**: Periódicamente (cada segundo) consulta al servidor por el estado del LED
- **Lectura del botón**: Constantemente monitorea el estado del botón
- **Reconexión**: Si se pierde la conexión WiFi, intenta reconectar

#### Funciones principales

##### `testServerConnection()`

```cpp
void testServerConnection() {
  HTTPClient http;
  String url = String(serverUrl);
  
  Serial.println("Probando conexión al servidor: " + url);
  
  http.begin(url);
  int httpCode = http.GET();
  
  // ... verificación de respuesta ...
  
  http.end();
}
```

- Realiza una solicitud GET inicial al servidor para verificar conectividad
- Muestra mensajes de depuración con el resultado

##### `checkLedStatus()`

```cpp
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
```

- Consulta el endpoint `/led/status` del servidor
- Según la respuesta ("ON" u "OFF"), cambia el estado del LED físico
- Maneja errores de comunicación

##### `readButton()`

```cpp
void readButton() {
  // Leer el estado actual
  int reading = digitalRead(buttonPin);
  
  // ... depuración ...
  
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
```

- Implementa un algoritmo de "debounce" para evitar falsos positivos al presionar el botón
- Cuando detecta una pulsación válida, llama a `incrementButtonCounter()`
- Está configurado para una resistencia pull-down (detecta HIGH cuando el botón está presionado)

##### `incrementButtonCounter()`

```cpp
void incrementButtonCounter() {
  HTTPClient http;
  String url = String(serverUrl) + "/button/increment";
  
  Serial.println("Enviando incremento a: " + url);
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST("{}");
  
  // ... verificación de respuesta ...
  
  http.end();
}
```

- Envía una solicitud POST al endpoint `/button/increment` del servidor
- Notifica el resultado por Serial para depuración

## Conexión física

### LED

El LED debe conectarse así:

ESP32 GPIO2 ------ Resistencia (220-330 ohm) ------ LED ánodo (pata larga) 
                                                     |
                                                     |
ESP32 GND    ------------------------------------ LED cátodo (pata corta)

### Botón con resistencia pull-down

El botón debe conectarse con una resistencia pull-down así:

ESP32 GPIO4 ------ Botón ------ ESP32 3.3V
            |
            |
            R (10K Ohm)
            |
            |
ESP32 GND -----------

## Modificación del código

Antes de cargar el código, asegúrate de modificar:

1. Las credenciales WiFi:
   ```cpp
   const char* ssid = "TU_WIFI";        // Cambiar a tu SSID
   const char* password = "TU_CONTRASEÑA"; // Cambiar a tu contraseña
   ```

2. La dirección IP del servidor:
   ```cpp
   const char* serverUrl = "http://TU_IP:8000"; // Cambiar a la IP de tu PC
   ```

   Para obtener la IP de tu computadora:
   - En Windows: Abre CMD y escribe `ipconfig`
   - En Linux/Mac: Abre Terminal y escribe `ifconfig` o `ip addr`

## Carga del código

1. Conecta tu ESP32 a tu computadora mediante USB
2. Abre el archivo `arduino_code.ino` en el IDE de Arduino
3. Selecciona la placa ESP32 en el menú Herramientas > Placa
4. Selecciona el puerto correcto en Herramientas > Puerto
5. Haz clic en el botón de carga (flecha)

## Depuración

Una vez cargado el código, puedes monitorear el funcionamiento mediante:

1. Abre el Monitor Serial en el IDE de Arduino (botón de la lupa)
2. Asegúrate de que esté configurado a 115200 baudios
3. Observa los mensajes de depuración
   - Deberías ver mensajes de conexión WiFi
   - Mensajes de estado del LED
   - Mensajes cuando se presiona el botón

