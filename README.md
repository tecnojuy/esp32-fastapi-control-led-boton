# Control de ESP32 con FastAPI

Este proyecto implementa un sistema de control para un ESP32 utilizando FastAPI como backend y una interfaz web simple. Permite controlar un LED y contar pulsaciones de un botón físico.

## Características

- Control remoto de un LED en el ESP32
- Contador de pulsaciones de botón físico
- API REST con FastAPI
- Interfaz web responsive

## Estructura del proyecto

- `backend/`: Servidor FastAPI
- `frontend/`: Interfaz web
- `esp32/`: Código para el ESP32 (Arduino)

## Requisitos

### Backend
- Python 3.11+
- FastAPI
- Uvicorn

### ESP32
- ESP32 DevKit
- Arduino IDE o PlatformIO
- Bibliotecas: WiFi, HTTPClient

## Instalación

### Backend

```bash
cd backend
pip install -r requirements.txt
python -m uvicorn main:app --host 0.0.0.0 --port 8000
```

### Frontend

Simplemente abre `frontend/simple.html` en un navegador, o usa un servidor web simple:

```bash
cd frontend
python -m http.server 5000
```

### ESP32

1. Abre el archivo `esp32/arduino_code/arduino_code.ino` en el IDE de Arduino
2. Configura tu SSID y contraseña WiFi
3. Actualiza la IP del servidor en el código
4. Sube el código a tu ESP32

## Conexión física

### LED
- Conecta un LED al pin GPIO2 del ESP32
- Añade una resistencia de 220-330 ohm en serie

### Botón
- Conecta un botón entre el pin GPIO4 y 3.3V
- Utiliza una resistencia pull-down de 10K entre GPIO4 y GND

## API REST

| Endpoint | Método | Descripción |
|----------|--------|-------------|
| `/led/status` | GET | Obtener estado del LED |
| `/led/on` | POST | Encender LED |
| `/led/off` | POST | Apagar LED |
| `/button/count` | GET | Obtener contador de botón |
| `/button/increment` | POST | Incrementar contador |
| `/button/reset` | POST | Reiniciar contador |

## Autor

0xBlazy

## Licencia

MIT 