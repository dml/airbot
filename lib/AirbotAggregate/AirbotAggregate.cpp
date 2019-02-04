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


void AirbotAggregate::setNH3(float mt) { _setGasMetric(&_nh3, mt); }
void AirbotAggregate::setCO(float mt) { _setGasMetric(&_co, mt); }
void AirbotAggregate::setNO2(float mt) { _setGasMetric(&_no2, mt); }
void AirbotAggregate::setC3H8(float mt) { _setGasMetric(&_c3h8, mt); }
void AirbotAggregate::setC4H10(float mt) { _setGasMetric(&_c4h10, mt); }
void AirbotAggregate::setCH4(float mt) { _setGasMetric(&_ch4, mt); }
void AirbotAggregate::setH2(float mt) { _setGasMetric(&_h2, mt); }
void AirbotAggregate::setC2H5OH(float mt) { _setGasMetric(&_c2h5oh, mt); }

void AirbotAggregate::_setGasMetric(float * dst, float mt) {
  if (mt >= 0) *dst = mt;
}

void AirbotAggregate::setCO2(int c) {

  if (0 < c && c < 10000) _co2 = float(c);
}

void AirbotAggregate::setTemperature(float c) {
  if (-99.9 < c && c < 99.9) _temperature = c;
}

void AirbotAggregate::setHumidity(float c) {
  if (0.0 < c && c < 100.0) _humidity = c;
}

void AirbotAggregate::setPM2_5(float c) {
  _pm2_5 = c;
}

void AirbotAggregate::setPM10(float c) {
  _pm10 = c;
}


float AirbotAggregate::temperature() { return _temperature; }
float AirbotAggregate::humidity() { return _humidity; }
float AirbotAggregate::c2h5oh() { return _c2h5oh; }
float AirbotAggregate::c4h10() { return _c4h10; }
float AirbotAggregate::c3h8() { return _c3h8; }
float AirbotAggregate::ch4() { return _ch4; }
float AirbotAggregate::pm2_5() { return _pm2_5; }
float AirbotAggregate::pm10() { return _pm10; }
float AirbotAggregate::co2() { return _co2; }
float AirbotAggregate::co() { return _co; }
float AirbotAggregate::no2() { return _no2; }
float AirbotAggregate::nh3() { return _nh3; }
float AirbotAggregate::h2() { return _h2; }
