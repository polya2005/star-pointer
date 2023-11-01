#ifndef STARPOINTER_H
#define STARPOINTER_H

#ifndef ARDUINO
#define ARDUINO 100
#endif

#ifndef MAXSPEED
#define MAXSPEED 1000
#endif

#ifndef MINSPEED
#define MINSPEED 0
#endif

#ifndef TOLERANCE
#define TOLERANCE 0.011635528346628864 //.75 deg
#endif

#ifndef AZERROR
#define AZERROR 10.0F
#endif

#define TRIG_ERROR -5.0F
#define trigValid(x) ((-1.0<=(x))&&((x)<=1.0))
#define correctAngleSep(x) ((x)<=PI?(x):TWO_PI-(x))

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include <TinyGPS++.h>
#include <QMC5883LCompass.h>
#include "Pointer.h"
#include "AstroTime.h"
#include <EEPROM.h>
#define DEBUG
#include <tempdebug.h>

float angleClockwise(float a1,float a2);

class Starpointer{
  private:
    Adafruit_ADXL345_Unified _accel = Adafruit_ADXL345_Unified(1);
    TinyGPSPlus _gps{};
    QMC5883LCompass _compass{};
    Pointer _pointerobj{};
    Pointer* _pointer{};
    Stream* _stream{};
    HMS _LSTnow{};
    float _lat{};
    float _lon{};
    float _Al{};
    float _Az{};

    SiderealTime _ref = SiderealTime(31,Dec,2020,17,36,43);
    
    HMS _LST();
    float _eqToAl(HMS &ra,dms &dec);
    float _eqToAz(HMS &ra,dms &dec);
    void updatetime();
    //float magdec();
  public:
    /// Initializes the StarPointer object.
    /// \param[in] st serial or SoftwareSerial object connected to GPS Module
    /// \param[in] az_1 Azimuthal stepper motor's IN1 pin.
    /// \param[in] az_3 Azimuthal stepper motor's IN3 pin.
    /// \param[in] az_2 Azimuthal stepper motor's IN2 pin.
    /// \param[in] az_4 Azimuthal stepper motor's IN4 pin.
    /// \param[in] to_east The direction azimuthal stepper motor shoud go to move eastward
    /// \param[in] al_1 Altitudinal stepper motor's IN1 pin.
    /// \param[in] al_3 Altitudinal stepper motor's IN3 pin.
    /// \param[in] al_2 Altitudinal stepper motor's IN2 pin.
    /// \param[in] al_4 Altitudinal stepper motor's IN4 pin.
    /// \param[in] to_zenith The direction altitudinal stepper motor shoud go to move upward 
    void begin(Stream &st,int az_1,int az_3,int az_2,int az_4,int8_t to_east,int al_1,int al_3,int al_2,int al_4,int8_t to_zenith);
    
    /// Points to the specified equatorial coordinate.
    /// \param[in] ra right ascention
    /// \param[in] dec declination
    bool pointTo(HMS &ra,dms &dec);

    /// Provides pointing right ascension. The method readAzAl MUST be called before this method
    /// \return right ascension where the pointer is pointing
    HMS rightAscension() const;

    /// Provides pointing declination. The method readAzAl MUST be called before this method
    /// \return declination where the pointer is pointing
    dms declination() const;

    /// Call this method ONLY when necessary
    void readAzAl();
    
    void up();
    void down();
    void east();
    void west();

    /// Activates movement.
    bool run();

    void stop();

    /// calibrate compass module
    void calibrate_compass();

    ///read the altitude value. use only when necessary
    ///\return altitude
    float _getAl();

    ///read the azimuth value. use only when necessary
    ///\return azimuth
    float _getAz();
};

#endif