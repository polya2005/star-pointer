#include "Arduino.h"
#include "Starpointer.h"
#include "tempdebug.h"

float Starpointer::_getAl() {
  sensors_event_t event{};
  _accel.getEvent(&event);
  debug(F("ok "));
  delay(10);
  return float(atan(event.acceleration.x / sqrt(event.acceleration.z * event.acceleration.z + event.acceleration.y * event.acceleration.y)));
}

float Starpointer::_getAz() {
  _compass.read();
  float temp{atan2(_compass.getX(), _compass.getY())}; //this is due to the sensor installation orientation
  return temp >= PI ? temp - PI : temp + PI;
}

void Starpointer::readAzAl() {  //Use before ra and dec method
  updatetime();
  _Az = _getAz();
  _Al = _getAl();
}

HMS Starpointer::_LST() {
  while (!(_stream->available()))
    ;
  while (_stream->available()) {
    _gps.encode(_stream->read());
    if (_gps.time.isValid() && _gps.time.isUpdated()) {
      SiderealTime now(_gps.date.day(), (month)(_gps.date.month()), _gps.date.year(), _gps.time.hour() + int(_lon / 15), _gps.time.minute() + int(_lon * 4 - int(_lon / 15) * 60), _gps.time.second() + int(_lon * 240 - int(_lon / 15) * 3600 - int(_lon * 4 - int(_lon / 15) * 60) * 60));
      return HMSDiff(now, _ref);
    }
  }
}

void Starpointer::updatetime() {
  _LSTnow = _LST();
}

float Starpointer::_eqToAl(HMS &ra, dms &ddec) {
  float dec = ddec.toRad();
  float ha = radiansDiff(_LSTnow, ra);
  double sinAl = sin(_lat) * sin(dec) + cos(_lat) * cos(dec) * cos(ha);
  return trigValid(sinAl) ? (float)asin(sinAl) : TRIG_ERROR;
}

float Starpointer::_eqToAz(HMS &ra, dms &ddec) {
  float dec = ddec.toRad();
  float ha = radiansDiff(_LSTnow, ra);
  double sinAl = sin(_lat) * sin(dec) + cos(_lat) * cos(dec) * cos(ha);
  if (trigValid(sinAl)) {
    double cosAz = sin(dec) / sqrt(1 - sinAl * sinAl) / cos(ha) - tan(ha) * sinAl / sqrt(1 - sinAl * sinAl);
    return trigValid(cosAz) ? (1 - 2 * (ha < PI)) * (float)acos(cosAz) : TRIG_ERROR;
  } else {
    return TRIG_ERROR;
  }
}

dms Starpointer::declination() const {
  float sinDec = sin(_lat) * sin(_Al) + cos(_lat) * cos(_Al) * cos(_Az);
  return trigValid(sinDec) ? dms(asin(sinDec)) : dms(TRIG_ERROR);
}

HMS Starpointer::rightAscension() const {
  float sinDec = sin(_lat) * sin(_Al) + cos(_lat) * cos(_Al) * cos(_Az);
  if (trigValid(sinDec)) {
    float cosHA = sin(_Az) / sqrt(1 - sinDec * sinDec) / cos(_lat) - tan(_lat) * sinDec / sqrt(1 - sinDec * sinDec);
    if (trigValid(cosHA)) {
      HMS haHMS = HMS((1 - 2 * (_Az < PI)) * (float)acos(cosHA));
      HMS(radiansDiff(_LSTnow, haHMS)).print();
      return HMS(radiansDiff(_LSTnow, haHMS));
    } else {
      return HMS(TRIG_ERROR);
    }
  } else {
    return HMS(TRIG_ERROR);
  }
}

void Starpointer::begin(Stream &st, int az_1, int az_3, int az_2, int az_4, int8_t to_east, int al_1, int al_3, int al_2, int al_4, int8_t to_zenith) {
  _pointer = &_pointerobj;
  _pointer->begin(az_1, az_3, az_2, az_4, to_east, al_1, al_3, al_2, al_4, to_zenith);
  _stream = &st;
  _compass.init();
  while (!_accel.begin())
    ;
  _accel.setRange(ADXL345_RANGE_16_G);
  debugln(F("receiving GPS data..."));
  while (!_stream->available())
    ;
  while (_stream->available()) {
    _gps.encode(_stream->read());
    if (_gps.location.isValid()) {
      _lat = (float)_gps.location.lat();
      debug(F("lat, lon = "));
      debugln(String(_lat) + F(", ") + String(_lon));
      _lon = (float)_gps.location.lng();
      break;
    }
  }
  debugln(F("GPS data complete."));
  updatetime();
  debugln(F("Time updated."));
  // int16_t caldat[3][2];
  // uint16_t chk=0xffff;
  // for(auto i=0;i<3;i++){
  //   for(auto j=0;j<2;j++){
  //     EEPROM.get((i<<1|j)<<1,caldat[i][j]);
  //     debugln(caldat[i][j],HEX);
  //     chk&=caldat[i][j];
  //   }
  // }
  // if(chk!=0xffff)
  //   _compass.setCalibration(caldat[0][0],caldat[0][1],caldat[1][0],caldat[1][1],caldat[2][0],caldat[2][1]);
  debugln(-_getAl());
  debugln(-_getAz());
  _pointer->setZero();
  debugln(F("setZero called"));
  _pointer->setAltitude(-_getAl());
  debugln(F("setAltitude called"));
  _pointer->setAzimuth(-_getAz());
  debugln(F("setAzimuth called"));
  while (_pointer->run())
    debug(F("."));
  _pointer->setZero();
}

bool Starpointer::pointTo(HMS &ra, dms &dec) {
  updatetime();
  float al = _eqToAl(ra, dec);
  if (al >= 0) {
    _pointer->setAltitude(al);
    _pointer->setAzimuth(_eqToAz(ra, dec));
    return true;
  } else {
    return false;
  }
}

void Starpointer::up() {
  _pointer->up();
}

void Starpointer::down() {
  _pointer->down();
}

void Starpointer::east() {
  _pointer->east();
}

void Starpointer::west() {
  _pointer->west();
}

bool Starpointer::run() {
  return _pointer->run();
}

void Starpointer::stop() {
  _pointer->stop();
}

void Starpointer::calibrate_compass() {
  int16_t calibrationData[3][2];
  bool changed = false;
  int t = 0;
  int c = 0;
  int16_t x, y, z;
  while (true) {
    // Read compass values
    debug(F("."));
    _compass.read();
    delay(10);

    // Return XYZ readings
    x = _compass.getX();
    y = _compass.getY();
    z = _compass.getZ();

    changed = false;

    if (x < calibrationData[0][0]) {
      calibrationData[0][0] = x;
      changed = true;
    }
    if (x > calibrationData[0][1]) {
      calibrationData[0][1] = x;
      changed = true;
    }

    if (y < calibrationData[1][0]) {
      calibrationData[1][0] = y;
      changed = true;
    }
    if (y > calibrationData[1][1]) {
      calibrationData[1][1] = y;
      changed = true;
    }

    if (z < calibrationData[2][0]) {
      calibrationData[2][0] = z;
      changed = true;
    }
    if (z > calibrationData[2][1]) {
      calibrationData[2][1] = z;
      changed = true;
    }

    if (changed) {
      debugln("CALIBRATING... Keep moving your sensor around.");
      c = millis();
    }
    t = millis();
    if (t - c > 5000) {
      debugln(F("completed"));
      _compass.setCalibration(calibrationData[0][0], calibrationData[0][1], calibrationData[1][0], calibrationData[1][1], calibrationData[2][0], calibrationData[2][1]);
      // for(auto i=0;i<3;i++){
      //   for(auto j=0;j<2;j++){
      //     EEPROM.put((i<<1|j)<<1,calibrationData[i][j]);
      //   }
      // }
      debug(calibrationData[0][0], HEX);
      debug(", ");
      debug(calibrationData[0][1], HEX);
      debug(", ");
      debug(calibrationData[1][0], HEX);
      debug(", ");
      debug(calibrationData[1][1], HEX);
      debug(", ");
      debug(calibrationData[2][0], HEX);
      debug(", ");
      debugln(calibrationData[2][1], HEX);
      return;
    }
  }
}
//2F8, 4BD4, FFFFB0C6, 0, FFFFEB4A, 24944