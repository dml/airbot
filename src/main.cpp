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

#include <AsyncMqttClient.h>
#include <cozir.h>
#include <ESP8266WiFi.h>
#include <MutichannelGasSensor.h>
#include <SDS011.h>
#include <SoftwareSerial.h>
#include <TaskScheduler.h>
#include <U8g2lib.h>


#define SENSOR_ADDR             0X04
#define PRE_HEAT_TIME           0
#define MIN_MESSAGING_INTERVAL  15000

void connectToMqtt();
void connectToWifi();

const char* aio_broker    = "io.adafruit.com";
const char* aio_username  = AIO_USERNAME;
const char* aio_key       = AIO_KEY;
const char* wifi_ssid     = WIFI_SSID;
const char* wifi_password = WIFI_SECRET;

SoftwareSerial nss(0, 2);
COZIR czr(nss);
SDS011 sds011;
U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, 14, 13, 15, 12, 16);

AsyncMqttClient mqttClient;
bool mqttConnected = false;
unsigned long mqttReportedTime = 0;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

Scheduler runner;
Task wifiReconnectTask(2000, TASK_FOREVER, &connectToWifi);
Task mqttReconnectTask(10000, TASK_FOREVER, &connectToMqtt);

char measurement[16];
float temperature = 0.0;
float humidity = 0.0;
uint16_t co2 = 0;

float nh3 = 0.0;
float co = 0.0;
float no2 = 0.0;
float c3h8 = 0.0;
float c4h10 = 0.0;
float ch4 = 0.0;
float h2 = 0.0;
float c2h5oh = 0.0;

float pm2_5 = 0;
float pm10 = 0;

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
  if (millis() - mqttReportedTime > MIN_MESSAGING_INTERVAL) {
    mqttReportedTime = millis();
    return true;
  }

  if(millis() - mqttReportedTime < 0) mqttReportedTime = 0;

  return false;
}

void setup() {
  u8g2.begin();
  nss.begin(9600);
  gas.begin(SENSOR_ADDR);
  gas.powerOn();

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnected);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(aio_broker, 1883);
  mqttClient.setClientId(aio_username);
  mqttClient.setCredentials(aio_username, aio_key);

  sds011.setup(&Serial);
  sds011.onData([](float pm25Value, float pm10Value) {
     pm2_5 = pm25Value;
     pm10 = pm10Value;
    // if (mqttConnected && messagePublishingAllowed()) {
    //   mqttClient.publish((String(aio_username) + "/f/pm2_5").c_str(), 1, false, String(pm25Value, 1).c_str());
    //   mqttClient.publish((String(aio_username) + "/f/pm10").c_str(), 1, false, String(pm10Value, 1).c_str());
    // }
  });
  // sds011.onResponse([](){
  //   // command has been executed
  // });
  sds011.onError([](int8_t error){
    // error happened
    // -1: CRC error
  });
  sds011.setWorkingPeriod(5);

  connectToWifi();
}

void loop() {
  sds011.loop();

  float c;

  c = gas.measure_NH3();
  if(c >= 0) nh3 = c;

  c = gas.measure_CO();
  if(c >= 0) co = c;

  c = gas.measure_NO2();
  if(c >= 0) no2 = c;

  c = gas.measure_C3H8();
  if(c >= 0) c3h8 = c;

  c = gas.measure_C4H10();
  if(c >= 0) c4h10 = c;

  c = gas.measure_CH4();
  if(c >= 0) ch4 = c;

  c = gas.measure_H2();
  if(c >= 0) h2 = c;

  c = gas.measure_C2H5OH();
  if(c >= 0) c2h5oh = c;

  float t = czr.Celsius();
  if (-99.9 < t && t < 99.9) temperature = t;

  float h = czr.Humidity();
  if (0.0 < h && h < 100.0) humidity = h;

  uint16_t cm = czr.CO2();
  if (0 < cm && cm < 10000) co2 = cm;

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tf);
    u8g2.drawStr(0,8,(String(temperature, 1) + String("C")).c_str());
    u8g2.drawStr(50,8,(String(humidity, 1) + String("%")).c_str());

    u8g2.drawLine(0, 9, 83, 9);

    u8g2.setFont(u8g2_font_4x6_tf);
    // Ethanol C2H6OH 10 – 500ppm
    u8g2.drawStr(0,16,(String("C2H5OH:") + String(c2h5oh,1)).c_str());
    // Iso-butane C4H10 >1000ppm
    u8g2.drawStr(0,22,(String("C4H10:") + String(c4h10,1)).c_str());
    // Propane C3H8 >1000ppm
    u8g2.drawStr(0,28,(String("C3H8:") + String(c3h8,1)).c_str());
    // Methane CH4 >1000ppm
    u8g2.drawStr(0,34,(String("CH4:") + String(ch4,1)).c_str());
    // Dust pm2.5 0 - 999.9um/m3
    u8g2.drawStr(0,40,(String("PM2.5:") + String(pm2_5)).c_str());
    // Dust pm10 0 - 999.9um/m3
    u8g2.drawStr(0,46,(String("PM10:") + String(pm10)).c_str());
    // Carbon dioxide CO2 1 – 10000ppm
    u8g2.drawStr(50,16,(String("CO2:") + String(co2)).c_str());
    // Carbon monoxide CO 1 – 1000ppm
    u8g2.drawStr(50,22,(String("CO:") + String(co,1)).c_str());
    // Nitrogen dioxide NO2 0.05 – 10ppm
    u8g2.drawStr(50,28,(String("NO2:") + String(no2,1)).c_str());
    // Ammonia NH3 1 – 500ppm
    u8g2.drawStr(50,34,(String("NH3:") + String(nh3,1)).c_str());
    // Hydrogen H2 1 – 1000ppm
    u8g2.drawStr(50,40,(String("H2:") + String(h2,1)).c_str());

    u8g2.setFont(u8g2_font_u8glib_4_tf);
    u8g2.drawStr(50,47,"ok");

  } while ( u8g2.nextPage() );
}
