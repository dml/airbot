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

#ifndef AirbotAggregate_h
#define AirbotAggregate_h

class AirbotAggregate {
 public:
  AirbotAggregate();
  ~AirbotAggregate();
  void reset();

  float temperature();
  float humidity();
  float c2h5oh();
  float c4h10();
  float c3h8();
  float ch4();
  float pm2_5();
  float pm10();
  float co2();
  float co();
  float no2();
  float nh3();
  float h2();

  void setNH3(float c);
  void setCO(float c);
  void setNO2(float c);
  void setC3H8(float c);
  void setC4H10(float c);
  void setCH4(float c);
  void setH2(float c);
  void setC2H5OH(float c);

  void setCO2(int c);
  void setTemperature(float c);
  void setHumidity(float c);

  void setPM2_5(float c);
  void setPM10(float c);

 private:
  void _reset();

  void _setGasMetric(float * dst, float mt);

  float _temperature;
  float _humidity;
  float _co2;
  float _nh3;
  float _co;
  float _no2;
  float _c3h8;
  float _c4h10;
  float _ch4;
  float _h2;
  float _c2h5oh;
  float _pm2_5;
  float _pm10;
};

#endif
