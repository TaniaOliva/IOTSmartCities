# Casos de prueba

Umbrales: Normal < 50 % · Atención 50–89 % · Crítico ≥ 90 %.
El nivel se controla con los botones del ESP32 (llenar +10 %, vaciar a 0 %).

## Prueba 1: Normal
- Entrada: nivel inicial de 20 %, ubicación Mercado Municipal.
- Esperado: mensaje con valor 20 y estado normal.
- Obtenido: "Nivel: 20%" / "Nivel normal".
  {"deviceId":"CONT-01","location":"Mercado Municipal","metric":"llenado","value":20,"unit":"%","sensorStatus":"operativo"}
- Resultado: correcto.

## Prueba 2: Atención
- Entrada: nivel de 50 %, ubicación Parque Central.
- Esperado: aviso de programar recolección.
- Obtenido: "Nivel: 50%" / "ATENCION: programar recoleccion".
  {"deviceId":"CONT-01","location":"Parque Central","metric":"llenado","value":50,"unit":"%","sensorStatus":"operativo"}
- Resultado: correcto.

## Prueba 3: Crítico
- Entrada: nivel de 92 %, ubicación Centro.
- Esperado: alerta de nivel crítico.
- Obtenido: "Nivel: 92%" / "*** ALERTA: NIVEL CRITICO, REQUIERE RECOLECCION ***".
  {"deviceId":"CONT-01","location":"Centro","metric":"llenado","value":92,"unit":"%","sensorStatus":"operativo"}
  Con el botón de llenar subió a 100 %; con el de vaciar bajó a 0 %.
- Resultado: correcto.

## Prueba 4: Lectura fuera de rango (versión con sensor)
- Entrada: distancia de 117 cm en un contenedor de 100 cm.
- Esperado: la lectura se rechaza.
- Obtenido: "ERROR: Lectura fuera del rango permitido". No se envió mensaje.
- Resultado: correcto.

## Prueba 5: Sensor sin respuesta (versión con sensor)
- Entrada: cable ECHO desconectado.
- Esperado: mensaje con valor vacío.
- Obtenido: {"deviceId":"CONT-01","location":"Primer Nivel","metric":"llenado","value":null,"unit":"%","sensorStatus":"sin_eco"}
- Resultado: correcto.

## Defectos encontrados
- Los primeros programas no enviaban datos por MQTT; se agregó la conexión.
- Los botones no respondían por un error de cableado a tierra; se corrigió.
- Pendiente: detectar un contenedor que deja de enviar mensajes.