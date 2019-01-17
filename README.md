# airbot


## on macos

```bash
brew install platformio
```


## build and flash

Register on [https://io.adafruit.com](https://io.adafruit.com).

Get your [AIO KEY](https://learn.adafruit.com/mqtt-adafruit-io-and-you/getting-started-on-adafruit-io#where-to-find-your-adafruit-dot-io-key-3-7).

Export environment variables

```bash
export PLATFORMIO_BUILD_FLAGS='-DAIO_USERNAME=\"aio username\" -DAIO_KEY=\"aiu key\" -DWIFI_SSID=\"WIFI_SSID\" -DWIFI_SECRET=\"WIFI_SECRET\"'
```

Flash your WeMoS D1 mini connected to the USB.

```bash
pio run -t upload
```
