# IoT para Smart Cities: gestión inteligente de residuos

Proyecto de Tecnologías Emergentes (CCC504), CEUTEC.

## Qué hace
Simulamos cuatro contenedores de basura con un ESP32 y un sensor ultrasónico HC-SR04 en Wokwi. Cada contenedor mide su nivel de llenado y lo manda cada 5 segundos por MQTT a Node-RED, que clasifica el estado y lo muestra en un panel con la prioridad de recolección.

## Arquitectura
Wokwi (ESP32 + HC-SR04) → MQTT (broker.hivemq.com, tópico smartcity/residuos/lecturas) → Node-RED → Dashboard

## Contenedores
- CONT-01: Mercado Municipal
- CONT-02: Parque Central
- CONT-03: Centro
- CONT-04: Colonia Palmira

## Reglas de estado
- Normal: menos de 50 %
- Atención: de 50 % a 89 %
- Crítico: 90 % o más
- Lectura inválida: fuera del rango de 0 a 100 %
- Sin datos: el sensor deja de reportar

## Cómo ejecutarlo
1. Abrir los proyectos de Wokwi y presionar Start.
2. En Node-RED instalar @flowfuse/node-red-dashboard (Manage palette).
3. Importar nodered/flujo_residuos.json y presionar Deploy.
4. Abrir http://localhost:1880/dashboard/smart-city
5. Desactivar el nodo "Simulación automática" para ver solo los datos de Wokwi.

## Proyectos en Wokwi
- CONT-01: [enlace]
- CONT-02: [enlace]
- CONT-03: [enlace]
- CONT-04: [enlace]

## Estructura del repositorio
- wokwi/: código y circuito de cada contenedor
- nodered/: flujo y dashboard
- pruebas/: casos de prueba y resultados
- capturas/: evidencias
- docs/: bitácora

## Equipo
Tania Nayeli Oliva Murillo:Organizacion, repositorio y documento
Wilmer Josue Sanchez Gomez: Node-RED y reglas
Julio Cesar Guevara : pruebas y evidencias
Daniel Eduardo Rodriguez Rápalo: simulación en Wokwi