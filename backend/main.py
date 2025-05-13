from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI(title="ESP32 LED Control")

# Habilitar CORS para que el frontend pueda comunicarse
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # En producción, limitar a dominios específicos
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Estado del LED y contador de botón
led_status = {"state": "OFF"}
button_counter = {"count": 0}

@app.get("/")
def read_root():
    return {"message": "API de control de LED para ESP32"}

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