/*
 * Connections:
 *
 * WeMos D1 Mini   Nokia 5110    Description
 * (ESP8266)       PCD8544 LCD
 *
 * D0 (GPIO16)     0 RST         Output from ESP to reset display
 * D8 (GPIO15)     1 CE          Output from ESP to chip select/enable display
 * D6 (GPIO12)     2 DC          Output from display data/command to ESP
 * D7 (GPIO13)     3 Din         Output from ESP SPI MOSI to display data input
 * D5 (GPIO14)     4 Clk         Output from ESP SPI clock
 * 3V3             5 Vcc         3.3V from ESP to display
 * D0 (GPIO16)     6 BL          3.3V to turn backlight on, or PWM
 * G               7 Gnd         Ground
 *
 *
 * WeMos D1 Mini   Piezoelectric Description
 * (ESP8266)       speaker
 *
 * D8 (GPIO15)     + SND         Piezo +
 * G               - Gnd         Piezo -
 *
 *
 * WeMos D1 Mini   CoZiR         Description
 *
 * (ESP8266)       AH-1
 * G               Gnd           Ground
 * 3V3             3V3           3V3
 * D3 (GPIO0)      RX            Rx
 * D4 (GPIO2)      TX            Tx
 *
 */

#include <cozir.h>
#include <ESP8266WiFi.h>
#include <SDS011.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>
#include <MutichannelGasSensor.h>
#include <AsyncMqttClient.h>
#include <TaskScheduler.h>
#include <AirbotAggregate.h>
#include <AirbotDisplay.h>

#define AIRBOT_POLLING_INTERVAL 15000

#define SENSOR_ADDR             0X04
#define PRE_HEAT_TIME           0

#define SPI_CLOCK               14
#define SPI_DATA                13
#define SPI_CS                  15
#define SPI_DC                  12
#define SPI_RST                 16
#define CO2_RX                  0
#define CO2_TX                  2


const char* aio_broker    = "io.adafruit.com";
const char* aio_username  = AIO_USERNAME;
const char* aio_key       = AIO_KEY;
const char* wifi_ssid     = WIFI_SSID;
const char* wifi_password = WIFI_SECRET;

SoftwareSerial nss(CO2_RX, CO2_TX);
COZIR czr(nss);
SDS011 sds;

U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, SPI_CLOCK, SPI_DATA, SPI_CS, SPI_DC, SPI_RST);


bool readings_in_progress = false;
int readings_loops_count = 0;

AirbotAggregate airbot_aggregate;
AirbotDisplay airbot_display(u8g2);
AsyncMqttClient mqttClient;

bool mqttConnected = false;
uint16_t mqttReportedTime;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

Scheduler runner;
void connectToWifi();
void connectToMqtt();
Task wifiReconnectTask(2000, TASK_FOREVER, &connectToWifi);
Task mqttReconnectTask(10000, TASK_FOREVER, &connectToMqtt);

void connectToWifi() {
  WiFi.begin(wifi_ssid, wifi_password);
}

void connectToMqtt() {
  mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  runner.deleteTask(mqttReconnectTask);
  runner.addTask(wifiReconnectTask);
}

void onMqttConnected(bool sessionPresent) {
  mqttConnected = true;
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  mqttConnected = false;
  if (WiFi.isConnected()) {
    runner.addTask(mqttReconnectTask);
  }
}

void onMqttPublish(uint16_t packetId) {
  // blink twice
}

bool messagePublishingAllowed() {
  if (millis() - mqttReportedTime > AIRBOT_POLLING_INTERVAL) {
    mqttReportedTime = millis();
    return true;
  }

  if(millis() - mqttReportedTime < 0) mqttReportedTime = 0;

  return false;
}

void publish() {
  if (!(mqttConnected && messagePublishingAllowed())) return;

  mqttClient.publish((String(aio_username) + "/f/temperature").c_str(), 1, false, String(airbot_aggregate.temperature(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/humidity").c_str(), 1, false, String(airbot_aggregate.humidity(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/c2h5oh").c_str(), 1, false, String(airbot_aggregate.c2h5oh(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/c4h10").c_str(), 1, false, String(airbot_aggregate.c4h10(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/c3h8").c_str(), 1, false, String(airbot_aggregate.c3h8(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/ch4").c_str(), 1, false, String(airbot_aggregate.ch4(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/pm2_5").c_str(), 1, false, String(airbot_aggregate.pm2_5(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/pm10").c_str(), 1, false, String(airbot_aggregate.pm10(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/co2").c_str(), 1, false, String(airbot_aggregate.co2(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/co").c_str(), 1, false, String(airbot_aggregate.co(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/no2").c_str(), 1, false, String(airbot_aggregate.no2(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/nh3").c_str(), 1, false, String(airbot_aggregate.nh3(), 1).c_str());
  mqttClient.publish((String(aio_username) + "/f/h2").c_str(), 1, false, String(airbot_aggregate.h2(), 1).c_str());
}



void setup() {
  airbot_display.powerOn();

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnected);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(aio_broker, 1883);
  mqttClient.setClientId(aio_username);
  mqttClient.setCredentials(aio_username, aio_key);

  gas.begin(SENSOR_ADDR);
  gas.powerOn();

  nss.begin(9600);

  sds.setup(&Serial);
  sds.onData([](float pm25Value, float pm10Value) {
    airbot_aggregate.setPM2_5(pm25Value);
    airbot_aggregate.setPM10(pm10Value);
    readings_in_progress = false;
  });
}

void loop() {
  sds.setWorkingMode(true);
  sds.setWorkingPeriod(30);

  delay(5000);

  airbot_aggregate.setNH3(gas.measure_NH3());
  airbot_aggregate.setCO(gas.measure_CO());
  airbot_aggregate.setNO2(gas.measure_NO2());
  airbot_aggregate.setC3H8(gas.measure_C3H8());
  airbot_aggregate.setC4H10(gas.measure_C4H10());
  airbot_aggregate.setCH4(gas.measure_CH4());
  airbot_aggregate.setH2(gas.measure_H2());
  airbot_aggregate.setC2H5OH(gas.measure_C2H5OH());

  airbot_aggregate.setCO2(czr.CO2());
  airbot_aggregate.setTemperature(czr.Celsius());
  airbot_aggregate.setHumidity(czr.Humidity());

  readings_in_progress = true;
  readings_loops_count = 5000;
  while (readings_in_progress and readings_loops_count > 0) {
    sds.loop();
    readings_loops_count--;
  }

  sds.setWorkingMode(false);

  airbot_display.render(airbot_aggregate);

  publish();
}
