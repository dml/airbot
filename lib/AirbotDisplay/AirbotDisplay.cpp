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

AirbotDisplay::AirbotDisplay(U8G2 &u8g2) {
  screen = u8g2;
}

AirbotDisplay::~AirbotDisplay() {
  // currently nothing to do
}

void AirbotDisplay::powerOn() {
  screen.begin();
}

int AirbotDisplay::_ar(int dx, int padding = 0) {
  return dx - screen.getStrWidth(msg) - padding;
}

int AirbotDisplay::_al(int dx, int padding = 0) {
  return dx + padding;
}

void AirbotDisplay::render(AirbotAggregate &a) {
  int whole = screen.getDisplayWidth();
  int middle = whole / 2 + 4;
  screen.firstPage();
  do {
    screen.setFont(u8g2_font_ncenB08_tf);

    // Temperatire C
    s = String(a.temperature(), 1) + String("C"); msg = s.c_str();
    screen.drawStr( 0, 8, msg);

    // Humidity %
    s = String(a.humidity(), 1) + String("%"); msg = s.c_str();
    screen.drawStr(_ar(whole), 8, msg);

    screen.drawLine(0, 9, 83, 9);

    screen.setFont(u8g2_font_4x6_tf);

    // Ethanol C2H5OH 10 – 500ppm
    s = String(a.c2h5oh(), 1); msg = s.c_str();
    screen.drawStr( 0, 16, "C2H5OH:");
    screen.drawStr(_ar(middle), 16, msg);

    // Iso-butane C4H10 >1000ppm
    s = String(a.c4h10(), 0); msg = s.c_str();
    screen.drawStr( 0, 22, "C4H10:");
    screen.drawStr(_ar(middle), 22, msg);

    // Propane C3H8 >1000ppm
    s = String(a.c3h8(), 0); msg = s.c_str();
    screen.drawStr( 0, 28, "C3H8:");
    screen.drawStr(_ar(middle), 28, msg);

    // Methane CH4 >1000ppm
    s = String(a.ch4(), 0); msg = s.c_str();
    screen.drawStr( 0, 34, "CH4:");
    screen.drawStr(_ar(middle), 34, msg);

    // Dust pm2.5 0 - 999.9um/m3
    s = String(a.pm2_5(), 1); msg = s.c_str();
    screen.drawStr( 0, 40, "PM2.5:");
    screen.drawStr(_ar(middle), 40, msg);

    // Dust pm10 0 - 999.9um/m3
    s = String(a.pm10(), 1); msg = s.c_str();
    screen.drawStr( 0, 46, "PM10:");
    screen.drawStr(_ar(middle), 46, msg);

    // Carbon dioxide CO2 1 – 10000ppm
    s = String(a.co2(), 0); msg = s.c_str();
    screen.drawStr(_al(middle, 3), 16, "CO2:");
    screen.drawStr(_ar(whole), 16, msg);

    // Carbon monoxide CO 1 – 1000ppm
    s = String(a.co(), 2); msg = s.c_str();
    screen.drawStr(_al(middle, 3), 22, "CO:");
    screen.drawStr(_ar(whole), 22, msg);

    // Nitrogen dioxide NO2 0.05 – 10ppm
    s = String(a.no2(), 2); msg = s.c_str();
    screen.drawStr(_al(middle, 3), 28, "NO2:");
    screen.drawStr(_ar(whole), 28, msg);

    // Ammonia NH3 1 – 500ppm
    s = String(a.nh3(), 2); msg = s.c_str();
    screen.drawStr(_al(middle, 3), 34, "NH3:");
    screen.drawStr(_ar(whole), 34, msg);

    // Hydrogen H2 1 – 1000ppm
    s = String(a.h2(), 2); msg = s.c_str();
    screen.drawStr(_al(middle, 3), 40, "H2:");
    screen.drawStr(_ar(whole), 40, msg);

    screen.setFont(u8g2_font_u8glib_4_tf);

    s = String("ok"); msg = s.c_str();
    screen.drawStr(_ar(whole), 47, msg);

  } while ( screen.nextPage() );
}
