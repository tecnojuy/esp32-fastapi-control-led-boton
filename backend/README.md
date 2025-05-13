# Backend para Control de ESP32

Este directorio contiene el servidor backend desarrollado con FastAPI para el control de un ESP32. El servidor proporciona una API REST que permite controlar un LED y monitorear las pulsaciones de un botón físico.

## Estructura 

backend/
├── main.py # Archivo principal con la definición de la API
└── requirements.txt # Dependencias del proyecto

## Componentes principales

### `main.py`

Este archivo contiene la implementación del servidor FastAPI con los siguientes componentes:

#### Configuración inicial

```python
app = FastAPI(title="ESP32 LED Control")

# Habilitar CORS para que el frontend pueda comunicarse
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # En producción, limitar a dominios específicos
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
```

- **FastAPI**: Framework web de alto rendimiento para crear APIs con Python.
- **CORS Middleware**: Permite que el frontend (que se ejecuta en un origen diferente) se comunique con el backend.

#### Variables de estado

```python
# Estado del LED y contador de botón
led_status = {"state": "OFF"}
button_counter = {"count": 0}
```

- **led_status**: Almacena el estado actual del LED (ON/OFF)
- **button_counter**: Lleva la cuenta de las veces que se ha presionado el botón físico

#### Endpoints para el LED

```python
@app.get("/led/status")
def get_led_status():
    return led_status

@app.post("/led/on")
def turn_led_on():
    led_status["state"] = "ON"
    return {"message": "LED encendido", "state": led_status["state"]}

@app.post("/led/off")
def turn_led_off():
    led_status["state"] = "OFF"
    return {"message": "LED apagado", "state": led_status["state"]}
```

- **/led/status** (GET): Devuelve el estado actual del LED
- **/led/on** (POST): Cambia el estado del LED a "ON"
- **/led/off** (POST): Cambia el estado del LED a "OFF"

#### Endpoints para el botón

```python
@app.get("/button/count")
def get_button_count():
    return button_counter

@app.post("/button/increment")
def increment_button_count():
    button_counter["count"] += 1
    return {"message": "Contador incrementado", "count": button_counter["count"]}

@app.post("/button/reset")
def reset_button_count():
    button_counter["count"] = 0
    return {"message": "Contador reiniciado", "count": button_counter["count"]}
```

- **/button/count** (GET): Devuelve el contador actual de pulsaciones
- **/button/increment** (POST): Incrementa el contador en 1 (llamado por el ESP32 cuando se presiona el botón)
- **/button/reset** (POST): Reinicia el contador a 0 (usado por el frontend)

### `requirements.txt`

Este archivo lista todas las dependencias necesarias para ejecutar el backend:

```
fastapi==0.104.1
uvicorn==0.23.2
pydantic==2.4.2
```

- **fastapi**: El framework web usado para crear la API
- **uvicorn**: Servidor ASGI de alto rendimiento para ejecutar aplicaciones FastAPI
- **pydantic**: Biblioteca para la validación de datos y gestión de configuraciones

## Flujo de datos

1. El ESP32 consulta periódicamente el endpoint `/led/status` para actualizar el estado del LED físico.
2. Cuando se presiona el botón físico, el ESP32 envía una solicitud POST a `/button/increment`.
3. El frontend consulta `/led/status` y `/button/count` periódicamente para mostrar el estado actual.
4. Cuando el usuario hace clic en los botones de la interfaz web, se envían solicitudes POST a `/led/on` o `/led/off`.

## Ejecución

Para ejecutar el servidor:

```bash
# Instalar dependencias
pip install -r requirements.txt

# Iniciar el servidor en modo desarrollo
python -m uvicorn main:app --host 0.0.0.0 --port 8000 --reload
```

El servidor estará disponible en `http://localhost:8000` y la documentación automática de la API en `http://localhost:8000/docs`.

## Consideraciones para producción

- Limitar los orígenes permitidos en CORS a dominios específicos
- Implementar autenticación
- Utilizar una base de datos para persistir el estado
- Configurar un servidor proxy como Nginx frente a Uvicorn
- Considerar el uso de HTTPS
