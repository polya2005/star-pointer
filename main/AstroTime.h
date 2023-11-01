#ifndef ASTRO_TIME_H
#define ASTRO_TIME_H

#define _SECINYEAR 31536000

#define frac(x) ((x)-int(x))

#include "Arduino.h"
//#include <bits/stdc++.h>

#define DEBUG

enum month {Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec};

class base60{
  protected:
    int _1{},_2{},_3{};
  public:
    base60(){}
    base60(int __1,int __2,int __3);
    base60(float radian,float convToRad);
    virtual void print();
    virtual ~base60() = default;
};

class HMS : public base60{
  public:
    HMS():base60(){}
    HMS(int hh,int mm,int ss);
    HMS(float radian);
    float toRad();
    void getHMS(int* h,int* m,int* s);
    virtual ~HMS() = default;
};

class dms : public base60{
  public:
    dms():base60(){}
    dms(int dd,int mm,int ss);
    dms(float radian);
    float toRad();
    void getdms(int* d,int* m,int* s);
    virtual ~dms() = default;
};

class SiderealTime{
  private:
    uint16_t _sec{};//from 1 Jan 2021 00:00 mod86400
    static uint32_t numsecs(month m);
  public:
    SiderealTime(int d,month m,int y,int hh,int mm,int ss);
    friend HMS HMSDiff(SiderealTime& t1,SiderealTime& t2);
};

float radiansDiff(HMS t1,HMS t2);
float radiansDiff(dms t1,dms t2);

float absRadiansDiff(HMS t1,HMS t2);
float absRadiansDiff(dms t1,dms t2);

#endif