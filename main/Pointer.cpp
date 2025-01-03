#include "Pointer.h"

#include "AccelStepper.h"
#include "Arduino.h"


void Pointer::begin(int az_1, int az_3, int az_2, int az_4, int8_t to_east,
                    int al_1, int al_3, int al_2, int al_4, int8_t to_zenith) {
  _az = new AccelStepper(AccelStepper::HALF4WIRE, az_1, az_3, az_2, az_4);
  _al = new AccelStepper(AccelStepper::HALF4WIRE, al_1, al_3, al_2, al_4);
  _az->setAcceleration(50.0f);
  _al->setAcceleration(50.0f);
  _az->setMaxSpeed(1000.0f);
  _al->setMaxSpeed(1000.0f);
  toE = to_east;
  toZ = to_zenith;
}

void Pointer::setZero() {
  _az->setCurrentPosition(0);
  _al->setCurrentPosition(0);
}

void Pointer::setAltitude(float rad) { _al->moveTo(toZ * (long)toSteps(rad)); }

void Pointer::setAzimuth(float rad) { _az->moveTo(toE * (long)toSteps(rad)); }

void Pointer::up() { _al->move(toZ * 50); }

void Pointer::down() { _al->move(-toZ * 50); }

void Pointer::east() { _az->move(toE * 50); }

void Pointer::west() { _az->move(-toE * 50); }

bool Pointer::run() { return _az->run() || _al->run(); }

void Pointer::stop() {
  _az->stop();
  _al->stop();
}

Pointer::~Pointer() {
  delete _az;
  delete _al;
}