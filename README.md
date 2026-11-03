# IoT para Smart Cities: gestión inteligente de residuos
Tecnologías Emergentes (CCC504) · CEUTEC

## Qué hace
Cuatro contenedores simulados (ESP32 + sensor ultrasónico HC-SR04) miden su nivel de llenado y lo envían por MQTT a Node-RED, que clasifica el estado y lo muestra en un panel con prioridad de recolección.

## Arquitectura
Wokwi (ESP32 + HC-SR04) → MQTT (broker.hivemq.com) → Node-RED → Dashboard

## Cómo ejecutarlo
1. Abrir los proyectos de Wokwi (enlaces abajo) y presionar Start.
2. En Node-RED instalar @flowfuse/node-red-dashboard.
3. Importar nodered/flujo_residuos.json y presionar Deploy.
4. Abrir http://localhost:1880/dashboard/smart-city

## Reglas
Normal: menos de 50 % · Atención: 50 a 89 % · Crítico: 90 % o más
Lectura inválida: fuera de 0 a 100 % · Sin datos: sensor sin reportar

## Proyectos Wokwi
- CONT-01: [enlace]
- CONT-02: [enlace]
- CONT-03: [enlace]
- CONT-04: [enlace]

## Equipo
Tania Nayeli Oliva Murillo_62321160
Wilmer Josue Sanchez Gomez 62211430
Julio Cesar Guevara – 62311547
Daniel Eduardo Rodriguez Rápalo -62311656