#ifndef POINTER_H
#define POINTER_H

#ifndef ARDUINO
#define ARDUINO 100
#endif

#define CLOCKWISE 1
#define COUNTERCLOCKWISE -1

#define toSteps(rad) ((rad)*radToStep)

#include "Arduino.h"
#include "AccelStepper.h"

class Pointer{
  private:
    AccelStepper *_az{},*_al{};
    int8_t toE{},toZ{};
    const float radToStep{648.67939432709760002386963356506f};
  public:
    /// Initializes the Pointer object.
    void begin(int az_1,int az_3,int az_2,int az_4,int8_t to_east,int al_1,int al_3,int al_2,int al_4,int8_t to_zenith);
  
    /// Set north and horizon reference point.
    void setZero();

    /// Point to the specified altitude
    void setAltitude(float rad);

    /// Point to the specified azimuth
    void setAzimuth(float rad);

    /// Move 50 steps up
    void up();

    /// Move 50 steps down
    void down();

    /// Move 50 steps to east
    void east();

    /// Move 50 steps to west
    void west();

    /// Run stepper motors
    bool run();

    /// Stop movements
    void stop();
    
    ~Pointer();
};

#endif