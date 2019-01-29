/* AirbotAggregate

Copyright 2019 Dmitry Larkin <dmitry.larkin@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <ESP8266WiFi.h>
#include <AirbotAggregate.h>

AirbotAggregate::AirbotAggregate() {
  _reset();
}

AirbotAggregate::~AirbotAggregate() {
  // currently nothing to do
}

void AirbotAggregate::reset() {
  _reset();
}

void AirbotAggregate::_reset() {
  _temperature = 0.0;
  _humidity = 0.0;
  _co2 = 0.0;
  _nh3 = 0.0;
  _co = 0.0;
  _no2 = 0.0;
  _c3h8 = 0.0;
  _c4h10 = 0.0;
  _ch4 = 0.0;
  _h2 = 0.0;
  _c2h5oh = 0.0;
  _pm2_5 = 0;
  _pm10 = 0;
}


void AirbotAggregate::setNH3(float mt) {
  _setGasMetric(&_nh3, mt);
}

void AirbotAggregate::setCO(float mt) {
  _setGasMetric(&_co, mt);
}

void AirbotAggregate::setNO2(float mt) {
  _setGasMetric(&_no2, mt);
}

void AirbotAggregate::setC3H8(float mt) {
  _setGasMetric(&_c3h8, mt);
}

void AirbotAggregate::setC4H10(float mt) {
  _setGasMetric(&_c4h10, mt);
}

void AirbotAggregate::setCH4(float mt) {
  _setGasMetric(&_ch4, mt);
}

void AirbotAggregate::setH2(float mt) {
  _setGasMetric(&_h2, mt);
}

void AirbotAggregate::setC2H5OH(float mt) {
  _setGasMetric(&_c2h5oh, mt);
}

void AirbotAggregate::_setGasMetric(float * dst, float mt) {
  if (mt >= 0) *dst = mt;
}

const char* AirbotAggregate::temperatureToa(const char *fmt) {
  return String("23.23").c_str();
}

const char* AirbotAggregate::humidityToa(const char *fmt) {
  return String(_humidity, 1).c_str();
}

const char* AirbotAggregate::c2h5ohToa(const char *fmt) {
  return String(_c2h5oh).c_str();
}

const char* AirbotAggregate::c4h10Toa(const char *fmt) {
  return String(_c4h10, 1).c_str();
}

const char* AirbotAggregate::c3h8Toa(const char *fmt) {
  return String(_c3h8, 1).c_str();
}

const char* AirbotAggregate::ch4Toa(const char *fmt) {
  return String(_ch4, 1).c_str();
}

const char* AirbotAggregate::pm2_5Toa(const char *fmt) {
  return String(_pm2_5, 1).c_str();
}

const char* AirbotAggregate::pm10Toa(const char *fmt) {
  return String(_pm10, 1).c_str();
}

const char* AirbotAggregate::co2Toa(const char *fmt) {
  return String(_co2, 0).c_str();
}

const char* AirbotAggregate::coToa(const char *fmt) {
  return String(_co, 1).c_str();
}

const char* AirbotAggregate::no2Toa(const char *fmt) {
  return String(_no2, 1).c_str();
}

const char* AirbotAggregate::nh3Toa(const char *fmt) {
  return String(_nh3, 1).c_str();
}

const char* AirbotAggregate::h2Toa(const char *fmt) {
  return String(_h2, 1).c_str();
}
