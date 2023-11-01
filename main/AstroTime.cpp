#include "AstroTime.h"
#include "Arduino.h"
#include <tempdebug.h>
#ifdef DEBUG
#define DEBUG_DEF 1
#else
#define DEBUG
#define DEBUG_DEF 0
#endif

#ifdef fabsf
#undef fabsf
#endif

float fabsf(float x){
  return (x>=0) ? x : -x;
}

uint32_t SiderealTime::numsecs(month m){
  const uint32_t secinmonth[12]={2678400, 2419200, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400};
  uint32_t res{0};
  for(int i=Jan;i<=m;i++){
    res+=secinmonth[i];
  }
  return res;
}

base60::base60(int __1,int __2,int __3): _1(__1),_2(__2),_3(__3) {}
base60::base60(float radian,float convToRad){
  float h=radian/convToRad;
  if(h>=0){
    _1=int(h);
    _2=int(frac(h)*60);
    _3=int(frac(frac(h)*60)*60);
  }else{
    _1=-int(-h);
    _2=-int(frac(-h)*60);
    _3=-int(frac(frac(-h)*60)*60);
  }
}

void base60::print(){
  debug(_1);
  debug(F("."));
  debug(_2);
  debug(F("."));
  debugln(_3);
}

HMS::HMS(int hh,int mm,int ss): base60(hh,mm,ss){}

HMS::HMS(float radian): base60(radian,PI/12){}

float HMS::toRad(){
  return radians(float(this->_1*15) + float(this->_2)/4 + float(this->_3)/240);
}

void HMS::getHMS(int* h,int* m,int* s){
  *h=_1;
  *m=_2;
  *s=_3;
}

dms::dms(int dd,int mm,int ss): base60(dd,mm,ss){}

dms::dms(float radian): base60(radian,PI/180){}

float dms::toRad(){
  return radians(float(this->_1)+float(this->_2)/60+float(this->_3)/3600);
}

void dms::getdms(int* d,int* m,int* s){
  *d=_1;
  *m=_2;
  *s=_3;
}

float radiansDiff(HMS t1,HMS t2){
  float res= t1.toRad() - t2.toRad();
  return (res>=0)?res:res+TWO_PI;
}

float radiansDiff(dms t1,dms t2){
  float res= t1.toRad() - t2.toRad();
  return (res>=0)?res:res+TWO_PI;
}

float absRadiansDiff(HMS t1,HMS t2){
  float res= t1.toRad() - t2.toRad();
  if (fabsf(res)<=PI)
  {
    debug(res);
    debug(F(" -> "));
    debugln(fabsf(res));
    return fabsf(res);
  }
  else if (res>0)
  {
    return TWO_PI-res;
  }
  else
  {
    return TWO_PI+res;
  }
}

float absRadiansDiff(dms t1,dms t2){
  float res= t1.toRad() - t2.toRad();
  debug(res);
  debug(F(" -> "));
  debugln(fabsf(res));
  if (fabsf(res)<=PI) {
    return fabsf(res);
  }
  else if (res>0) {
    return TWO_PI-res;
  }
  else {
    return TWO_PI+res;
  }
}

SiderealTime::SiderealTime(int d,month m,int y,int hh,int mm,int ss){
  const int dy{y-2000};
  int64_t _second{};
  _second=86400*(d-( ( (y%4!=0) || ((y%100==0) && (y%400!=0)) )?1:0))+numsecs(m)+(dy-1)*_SECINYEAR+dy/4-dy/100+dy/400-5+hh*3600+mm*60+ss;
  _second+=(int64_t)(float(_second)*1.0027379093307922);//1.0027379093307922=366.24219/365.24219
  _sec=_second%86400;
}

HMS HMSDiff(SiderealTime& t1,SiderealTime& t2){
  int16_t sec = t1._sec - t2._sec;
  if(sec<0) sec+=86400;
  return HMS(sec/3600,(sec/60)%60,sec%3600);
}

#if !DEBUG_DEF
#undef DEBUG
#endif

#undef DEBUG_DEF