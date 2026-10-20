/*
  Checkpoint 05 - Edge Computing & Computer Systems (FIAP)
  Vinheria Agnello - IoT com FIWARE (IoT Agent MQTT / Ultralight 2.0)

  Hardware: ESP32 + DHT11 (D4) + LDR (D34) + LED1 (D2)
  Bibliotecas (Gerenciador de Bibliotecas):
    - PubSubClient (Nick O'Leary)
    - DHT sensor library (Adafruit) + Adafruit Unified Sensor
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <time.h>

// ================== CONFIGURACOES ==================
const char* SSID          = "FIAP_IOT2";
const char* PASSWORD      = "F!@p25.IOT";

const char* BROKER_MQTT   = "3.135.234.101";   // IP publico da VM com FIWARE/Mosquitto
const int   BROKER_PORT   = 1883;

const char* TOPICO_PREFIXO = "/TEF";          // padrao: /<apikey>/<device_id>/...
const char* DEVICE_ID      = "device031";     // ex.: "agnello001"
const char* ID_MQTT        = "fiware_agnello_031";

// Pinos (conforme slide 10)
#define DHTPIN    4
#define DHTTYPE   DHT11
#define LDR_PIN   34
#define LED_PIN   2

// Calibracao do LDR (ajuste conforme o ambiente)
const int LDR_MIN = 0;
const int LDR_MAX = 4095;

// Intervalo de publicacao (ms)
const unsigned long INTERVALO = 5000;
// ================================================================

char topicoAttrs[64];    // /TEF/device001/attrs      (publica leituras)
char topicoCmd[64];      // /TEF/device001/cmd        (recebe comandos)
char topicoCmdExe[64];   // /TEF/device001/cmdexe     (resposta aos comandos)

WiFiClient espClient;
PubSubClient MQTT(espClient);
DHT dht(DHTPIN, DHTTYPE);

unsigned long ultimoEnvio = 0;
bool ledLigado = false;

// ---------- Wi-Fi ----------
void conectaWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.print("Conectando ao Wi-Fi ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWi-Fi conectado. IP: ");
  Serial.println(WiFi.localIP());
}

// ---------- NTP (timestamp) ----------
void iniciaNTP() {
  // Hora em UTC (o IoT Agent / Orion trabalham com ISO 8601 em UTC)
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Sincronizando hora (NTP)");
  struct tm t;
  while (!getLocalTime(&t, 1000)) {
    Serial.print(".");
  }
  Serial.println(" OK");
}

// Retorna timestamp ISO 8601 UTC, ex.: 2026-10-19T14:30:00.000Z
String timestampISO() {
  struct tm t;
  if (!getLocalTime(&t, 2000)) return "";
  char buf[32];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S.000Z", &t);
  return String(buf);
}

// ---------- MQTT ----------
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  Serial.print("Comando recebido: ");
  Serial.println(msg);

  // Formato Ultralight: "device001@on|" ou "device001@off|"
  String cmdOn  = String(DEVICE_ID) + "@on|";
  String cmdOff = String(DEVICE_ID) + "@off|";

  if (msg.equals(cmdOn)) {
    ledLigado = true;
    digitalWrite(LED_PIN, HIGH);
    MQTT.publish(topicoCmdExe, (String(DEVICE_ID) + "@on|LED ligado").c_str());
  } else if (msg.equals(cmdOff)) {
    ledLigado = false;
    digitalWrite(LED_PIN, LOW);
    MQTT.publish(topicoCmdExe, (String(DEVICE_ID) + "@off|LED desligado").c_str());
  }
}

void conectaMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Conectando ao broker MQTT ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("MQTT conectado!");
      MQTT.subscribe(topicoCmd);
    } else {
      Serial.print("Falha (rc=");
      Serial.print(MQTT.state());
      Serial.println("). Nova tentativa em 2s");
      delay(2000);
    }
  }
}

// ---------- Leitura e envio ----------
void publicaLeituras() {
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();

  if (isnan(temp) || isnan(umid)) {
    Serial.println("Falha ao ler o DHT11");
    return;
  }

  int ldrRaw = analogRead(LDR_PIN);
  int luz = map(ldrRaw, LDR_MIN, LDR_MAX, 0, 100);
  luz = constrain(luz, 0, 100);

  String ts = timestampISO();

  // Payload Ultralight 2.0: chave|valor|chave|valor...
  // t = temperatura, h = umidade, l = luminosidade, TimeInstant = timestamp
  String payload = "t|" + String(temp, 1) +
                   "|h|" + String(umid, 1) +
                   "|l|" + String(luz);
  if (ts.length() > 0) payload += "|TimeInstant|" + ts;

  MQTT.publish(topicoAttrs, payload.c_str());

  Serial.println("Publicado em " + String(topicoAttrs) + ": " + payload);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  analogReadResolution(12);   // 0 - 4095
  dht.begin();

  snprintf(topicoAttrs,  sizeof(topicoAttrs),  "%s/%s/attrs",  TOPICO_PREFIXO, DEVICE_ID);
  snprintf(topicoCmd,    sizeof(topicoCmd),    "%s/%s/cmd",    TOPICO_PREFIXO, DEVICE_ID);
  snprintf(topicoCmdExe, sizeof(topicoCmdExe), "%s/%s/cmdexe", TOPICO_PREFIXO, DEVICE_ID);

  conectaWiFi();
  iniciaNTP();

  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

void loop() {
  conectaWiFi();
  if (!MQTT.connected()) conectaMQTT();
  MQTT.loop();

  if (millis() - ultimoEnvio >= INTERVALO) {
    ultimoEnvio = millis();
    publicaLeituras();
  }
}
