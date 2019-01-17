#include <ESP8266WiFi.h>
#include <TaskScheduler.h>
#include <AsyncMqttClient.h>
#include <SDS011.h>

#define MIN_MESSAGING_INTERVAL 15000

unsigned long time = 0;

const char* aio_broker    = "io.adafruit.com";
const char* aio_username  = AIO_USERNAME;
const char* aio_key       = AIO_KEY;
const char* wifi_ssid     = WIFI_SSID;
const char* wifi_password = WIFI_SECRET;

void connectToMqtt();
void connectToWifi();

Scheduler runner;

SDS011 sds011;
AsyncMqttClient mqttClient;
Task mqttReconnectTask(10000, TASK_FOREVER, &connectToMqtt);

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Task wifiReconnectTask(2000, TASK_FOREVER, &connectToWifi);

bool connected = false;

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
  connected = true;
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  connected = false;
  if (WiFi.isConnected()) {
    runner.addTask(mqttReconnectTask);
  }
}

void onMqttPublish(uint16_t packetId) {
  // blink twice
}

bool messagePublishingAllowed() {
  if (millis() - time > MIN_MESSAGING_INTERVAL) {
    time = millis();
    return true;
  }

  if(millis() - time < 0 ) time = 0;

  return false;
}

void setup() {
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
    if (connected && messagePublishingAllowed()) {
      mqttClient.publish((String(aio_username) + "/f/pm2_5").c_str(), 1, false, String(pm25Value, 1).c_str());
      mqttClient.publish((String(aio_username) + "/f/pm10").c_str(), 1, false, String(pm10Value, 1).c_str());
    }
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
}
