#include <WiFi.h>
#include <PubSubClient.h>

// ---- Datos de este contenedor  ----
const char* DEVICE_ID = "CONT-01";
const char* UBICACION = "Mercado Municipal";
float nivelSimulado   = 20.0;   // nivel inicial (%)
const float RITMO     = 2.0;    // % que sube en cada lectura

// true = se llena solo y usa los botones | false = usa el sensor HC-SR04
const bool MODO_AUTOMATICO = true;

const char* MQTT_SERVER = "broker.hivemq.com";
const int   MQTT_PORT   = 1883;
const char* MQTT_TOPIC  = "smartcity/residuos/lecturas";

const int TRIG_PIN   = 5;
const int ECHO_PIN   = 18;
const int BTN_LLENAR = 25;
const int BTN_VACIAR = 26;

const float ALTURA_UTIL    = 100.0;
const float NIVEL_ATENCION = 50.0;
const float NIVEL_CRITICO  = 90.0;
const unsigned long INTERVALO = 5000;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long ultimoEnvio = 0;
bool llenarPrevio = false;
bool vaciarPrevio = false;

void conectarWiFi() {
  Serial.print("Conectando a WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println(" conectado");
}

void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    String id = String("wokwi-") + DEVICE_ID + "-" + String(random(0xffff), HEX);
    if (client.connect(id.c_str())) {
      Serial.println(" conectado");
    } else {
      Serial.print(" fallo, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void publicar() {
  char mensaje[220];
  float nivel = 0;
  bool hayLectura = true;

  if (MODO_AUTOMATICO) {
    nivel = nivelSimulado;
  } else {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
    if (duracion == 0) {
      hayLectura = false;
    } else {
      nivel = (ALTURA_UTIL - duracion / 58.0) / ALTURA_UTIL * 100.0;
    }
  }

  if (!hayLectura) {
    snprintf(mensaje, sizeof(mensaje),
      "{\"deviceId\":\"%s\",\"location\":\"%s\",\"metric\":\"llenado\",\"value\":null,\"unit\":\"%%\",\"sensorStatus\":\"sin_eco\"}",
      DEVICE_ID, UBICACION);
    Serial.println("ADVERTENCIA: no hay respuesta del sensor.");
  } else {
    snprintf(mensaje, sizeof(mensaje),
      "{\"deviceId\":\"%s\",\"location\":\"%s\",\"metric\":\"llenado\",\"value\":%.0f,\"unit\":\"%%\",\"sensorStatus\":\"operativo\"}",
      DEVICE_ID, UBICACION, nivel);
    Serial.printf("Nivel: %.0f%%\n", nivel);
    if (nivel >= NIVEL_CRITICO) {
      Serial.println("*** ALERTA: NIVEL CRITICO, REQUIERE RECOLECCION ***");
    } else if (nivel >= NIVEL_ATENCION) {
      Serial.println("ATENCION: programar recoleccion.");
    } else {
      Serial.println("Nivel normal.");
    }
  }

  client.publish(MQTT_TOPIC, mensaje);
  Serial.println(mensaje);
  Serial.println("----------------------------------------");
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BTN_LLENAR, INPUT_PULLUP);
  pinMode(BTN_VACIAR, INPUT_PULLUP);
  conectarWiFi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  if (!client.connected()) conectarMQTT();
  client.loop();

  // Botones (solo en modo automático)
  bool llenar = (digitalRead(BTN_LLENAR) == LOW);
  bool vaciar = (digitalRead(BTN_VACIAR) == LOW);

  if (MODO_AUTOMATICO && llenar && !llenarPrevio) {
    nivelSimulado += 10;
    if (nivelSimulado > 100) nivelSimulado = 100;
    Serial.println(">>> Boton LLENAR: +10%");
    publicar();
  }
  if (MODO_AUTOMATICO && vaciar && !vaciarPrevio) {
    nivelSimulado = 0;
    Serial.println(">>> Boton VACIAR: contenedor vaciado");
    publicar();
  }
  llenarPrevio = llenar;
  vaciarPrevio = vaciar;

  // reenvía el nivel actual cada 5 s
  if (millis() - ultimoEnvio >= INTERVALO) {
    ultimoEnvio = millis();
    publicar();
  }
  delay(20);
}