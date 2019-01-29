/* AirbotDisplay

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

#include <AirbotDisplay.h>

AirbotDisplay::AirbotDisplay(U8G2 * u8g2) {
  _u8g2 = u8g2;
}

AirbotDisplay::~AirbotDisplay() {
  // currently nothing to do
}

void AirbotDisplay::powerOn() {
  _u8g2->begin();
}

void AirbotDisplay::render(AirbotAggregate * a) {
  _u8g2->firstPage();
  do {
    _u8g2->setFont(u8g2_font_ncenB08_tf);

    _u8g2->drawStr( 0,  8, a->temperatureToa("%.1fC"));
    _u8g2->drawStr(50,  8, a->humidityToa("%.1f%%"));

    _u8g2->drawLine(0, 9, 83, 9);

    _u8g2->setFont(u8g2_font_4x6_tf);
    // Ethanol C2H6OH 10 – 500ppm
    _u8g2->drawStr( 0, 16, a->c2h5ohToa("C2H5OH:%d"));
    // Iso-butane C4H10 >1000ppm
    _u8g2->drawStr( 0, 22, a->c4h10Toa("C4H10:%d"));
    // Propane C3H8 >1000ppm
    _u8g2->drawStr( 0, 28, a->c3h8Toa("C3H8:%.0f"));
    // Methane CH4 >1000ppm
    _u8g2->drawStr( 0, 34, a->ch4Toa("CH4:%.0f"));
    // Dust pm2.5 0 - 999.9um/m3
    _u8g2->drawStr( 0, 40, a->pm2_5Toa("PM2.5:%.0f"));
    // Dust pm10 0 - 999.9um/m3
    _u8g2->drawStr( 0, 46, a->pm10Toa("PM10:%.0f"));
    // Carbon dioxide CO2 1 – 10000ppm
    _u8g2->drawStr(50, 16, a->co2Toa("CO2:%.0f"));
    // Carbon monoxide CO 1 – 1000ppm
    _u8g2->drawStr(50, 22, a->coToa("CO:%.0f"));
    // Nitrogen dioxide NO2 0.05 – 10ppm
    _u8g2->drawStr(50, 28, a->no2Toa("NO2:%.0f"));
    // Ammonia NH3 1 – 500ppm
    _u8g2->drawStr(50, 34, a->nh3Toa("NH3:%.0f"));
    // Hydrogen H2 1 – 1000ppm
    _u8g2->drawStr(50, 40, a->h2Toa("H2:%.0f"));

    _u8g2->setFont(u8g2_font_u8glib_4_tf);

    _u8g2->drawStr(50, 47, "ok");

  } while ( _u8g2->nextPage() );
}
