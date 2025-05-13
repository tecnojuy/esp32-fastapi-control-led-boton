# Frontend para Control de ESP32

Este directorio contiene una interfaz web simple desarrollada con HTML, CSS y JavaScript puro que permite controlar un LED en un ESP32 y monitorear las pulsaciones de un botón físico.

## Estructura 

frontend/
└── simple.html # Archivo HTML con CSS y JavaScript integrado

## Componentes principales

### `simple.html`

Este archivo contiene la implementación completa de la interfaz de usuario, incluyendo HTML, CSS y JavaScript.

#### Estructura HTML

La interfaz está dividida en dos paneles principales:

```html
<div class="container">
    <h1>Control de ESP32</h1>
    
    <div class="panels">
        <div class="panel">
            <h2>Control de LED</h2>
            <!-- Indicador visual del LED y botones de control -->
        </div>
        
        <div class="panel">
            <h2>Contador de Botón</h2>
            <!-- Visualización del contador y botón de reinicio -->
        </div>
    </div>
    
    <p id="errorMessage" style="display: none;"></p>
</div>
```

- **Panel de Control de LED**: Muestra el estado actual del LED y permite encenderlo o apagarlo
- **Panel de Contador de Botón**: Muestra cuántas veces se ha presionado el botón físico y permite reiniciar el contador

#### Estilos CSS

Los estilos están incluidos dentro de una etiqueta `<style>` y proporcionan:

- **Diseño responsive**: Se adapta a diferentes tamaños de pantalla
- **Visualización del LED**: Representación visual del estado del LED (encendido/apagado)
- **Contador de botón**: Diseño circular para mostrar el número de pulsaciones
- **Botones estilizados**: Diferentes colores según la acción (verde para encender, rojo para apagar, azul para reiniciar)

#### JavaScript

El código JavaScript maneja la lógica de interacción con el backend:

```javascript
// Configuración - Cambiar a la IP de tu máquina
const API_URL = 'http://localhost:8000'; // Cambia a tu IP real si es necesario
```

- **API_URL**: Define la URL base del servidor backend

##### Funciones principales

```javascript
// Obtener estado del LED
async function getLedStatus() {
    // ... consulta el endpoint /led/status
}

// Consultar contador de botón
async function getButtonCount() {
    // ... consulta el endpoint /button/count
}

// Controlar el LED
async function controlLed(action) {
    // ... envía POST a /led/on o /led/off
}

// Reiniciar contador
async function resetCounter() {
    // ... envía POST a /button/reset
}
```

- **getLedStatus()**: Obtiene el estado actual del LED desde el servidor
- **getButtonCount()**: Obtiene el contador de pulsaciones desde el servidor
- **controlLed(action)**: Envía comandos para encender o apagar el LED
- **resetCounter()**: Reinicia el contador de pulsaciones

##### Inicialización y eventos

```javascript
window.addEventListener('DOMContentLoaded', () => {
    // Obtener estados iniciales
    getLedStatus();
    getButtonCount();
    
    // Configurar actualizaciones periódicas
    setInterval(() => {
        getLedStatus();
        getButtonCount();
    }, 2000);
    
    // Configurar eventos de botones
    btnOn.addEventListener('click', () => controlLed('on'));
    btnOff.addEventListener('click', () => controlLed('off'));
    btnReset.addEventListener('click', resetCounter);
});
```

- **Carga inicial**: Obtiene el estado actual del LED y el contador
- **Actualizaciones periódicas**: Cada 2 segundos consulta por cambios
- **Eventos de botones**: Asigna las funciones correspondientes a los botones de la interfaz

## Uso de la interfaz

1. **Panel de LED**:
   - Muestra un círculo que cambia de color según el estado (amarillo cuando está encendido, gris cuando está apagado)
   - Botón "Encender": Envía la orden para encender el LED
   - Botón "Apagar": Envía la orden para apagar el LED

2. **Panel de Contador**:
   - Muestra un círculo con el número de veces que se ha presionado el botón físico
   - Botón "Reiniciar contador": Pone el contador a cero

3. **Mensajes de error**:
   - En la parte inferior aparecerán mensajes de error si hay problemas de comunicación con el servidor

## Personalización

Si necesitas modificar la URL del servidor:

1. Abre el archivo `simple.html` en un editor de texto
2. Busca la línea `const API_URL = 'http://localhost:8000';`
3. Cambia `localhost` por la IP de tu servidor (misma IP usada en el código Arduino)

## Despliegue

Para usar la interfaz:

1. **Opción simple**: Abre el archivo `simple.html` directamente en tu navegador
2. **Con servidor web**:
   - Python: `python -m http.server 5000` en la carpeta frontend
   - Luego accede a `http://localhost:5000/simple.html`

## Compatibilidad

Esta interfaz utiliza JavaScript moderno (async/await, fetch API) y funcionará en:
- Chrome 55+
- Firefox 52+
- Safari 11+
- Edge 15+

No se requiere ninguna biblioteca externa ni proceso de compilación.

