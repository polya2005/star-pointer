#ifndef FIELDREADER_H
#define FIELDREADER_H

#ifndef SD_CS
#define SD_CS 53
#endif

#include "Arduino.h"
#include "SD.h"
#include "SPI.h"
#include "tempdebug.h"

uint8_t _sd=0;

/// Class for reading each field from star catalog file.
/// \tparam T type of data to read
template <typename T> class fieldReader{
  private:
    uint8_t size{};
    uint16_t numByte{};
    File file{};
    int16_t* indArr{};
    bool _sizeValid(size_t size){
      return size == sizeof(T);
    }
    bool _isEqual(T a, T b){
      return a == b;
    }
  public:
    void begin(const char* _dir){
      if(!_sd){
        #ifdef DEBUG
        debugln("Initializing SD...");
        if(!SD.begin(SD_CS)){
          debugln("SD init error");
          while(1);
        }
        #endif
        _sd=1;
        debugln("SD init done");
      }
      String dir=String(_dir);
      dir.toUpperCase();
      file=SD.open(dir+".BIN");
      if(file){
        if(file.seek(0)){
          size=file.read();
          if(_sizeValid(size)){
            numByte=(file.available()-1)/size;
            const uint8_t _numByte{numByte};
            indArr=new int16_t[_numByte];
            return;
          }
        }
      }else{
        debug(F(" Open Error, filename:"));
        debugln(dir+".BIN");
      }
      while(1);
    }

    int16_t* indicesOf(T key){
      file.seek(1);
      uint16_t i{0},j{0};
      T temp{};
      while (file.available())
      {
        file.read((uint8_t*)&temp, size);
        if(_isEqual(temp, key)){
          indArr[i++]=j;
        }
        j++;
      }
      indArr[i]=-1;
      return indArr;
    }
    
    T operator[](uint16_t ind){
      file.seek(ind*size+1);
      T res{};
      file.read((uint8_t*)&res,size);
      return res;
    }
};

template<> bool fieldReader<char*>::_sizeValid(size_t size){
  return size > 0;
}

template<> bool fieldReader<char*>::_isEqual(char* a, char* b){
  return !strcmp(a, b);
}

template<> char* fieldReader<char*>::operator[](uint16_t ind){
  file.seek(ind*size+1);
  static char* res=new char[size];
  file.read((uint8_t*)res,size);
  return res;
}

// template<> class fieldReader<char*>{
//   private:
//     uint8_t size{};
//     uint16_t numByte{};
//     File* file{};
//     File fileobj{};
//     int16_t* indArr{};
//   public:
//     void begin(const char* _dir){
//       if(!_sd){
//         #ifdef DEBUG
//         debugln("Initializing SD...");
//         if(!SD.begin(SD_CS)){
//           debugln("SD init error");
//           while(1);
//         }
//         #endif
//         _sd=1;
//         debugln("SD init done");
//       }
//       String dir=String(_dir);
//       dir.toUpperCase();
//       fileobj=SD.open(dir+".BIN");
//       if(fileobj){
//         file=&fileobj;
//         if(file->seek(0)){
//           size=file->read();
//           numByte=(file->available()-1)/size;
//           const uint8_t _numByte{numByte};
//           indArr=new int16_t[_numByte];
//           return;
//         }
//       }
//       while(1);
//     }
//     int16_t* indicesOf(char* key){
//       file->seek(1);
//       uint16_t i{0},j{0};
//       char* temp{};
//       while (file->available())
//       {
//         file->read((uint8_t*)&temp,size);
//         if(!strcmp(temp,key)){
//           indArr[i++]=j;
//         }
//         j++;
//       }
//       indArr[i]=-1;
//       return indArr;
//     }
//     char* operator[](uint16_t ind){
//       file->seek(ind*size+1);
//       static char* res=new char[size];
//       file->read((uint8_t*)res,size);
//       return res;
//     }
// };

#endif