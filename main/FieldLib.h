#ifndef _FIELDLIB_H_
#define _FIELDLIB_H_

#define _lineX _textX

#include <Adafruit_ILI9341.h>
#include <u8g2_fonts.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <tempdebug.h>

#include <avr/pgmspace.h>

#define USETOUCH
#include <LCDWIKI_ALIASES.h>

#ifndef SIZE1
#define SIZE1 u8g2_font_6x10_tf
#endif

#ifndef SIZE2
#define SIZE2 u8g2_font_10x20_t_greek
#endif

#define COLORBARLEFT 0
#define COLORBARRIGHT 1

const uint8_t* fonts[2] = { SIZE1, SIZE2 };

#define DRAWCURSOR(color) tft.drawFastVLine(cursor* _textWidth + _lineX - 1, _lineY - _textSize * 10, _textSize * 10 - 1, color)
#define RADIOPIN 11

extern U8G2_FOR_ADAFRUIT_GFX u8g2;
extern Adafruit_ILI9341 tft;
extern LCDWIKI_TOUCH touch;

#define PGMSTR(con) const char con[] PROGMEM = #con

namespace _FieldLib{
  namespace Cons{
    const char nul[] PROGMEM = "";
    PGMSTR(And);
    PGMSTR(Ant);
    PGMSTR(Aps);
    PGMSTR(Aqr);
    PGMSTR(Aql);
    PGMSTR(Ara);
    PGMSTR(Ari);
    PGMSTR(Aur);
    PGMSTR(Boo);
    PGMSTR(Cae);
    PGMSTR(Cam);
    PGMSTR(Cnc);
    PGMSTR(CVn);
    PGMSTR(CMa);
    PGMSTR(CMi);
    PGMSTR(Cap);
    PGMSTR(Car);
    PGMSTR(Cas);
    PGMSTR(Cen);
    PGMSTR(Cep);
    PGMSTR(Cet);
    PGMSTR(Cha);
    PGMSTR(Cir);
    PGMSTR(Col);
    PGMSTR(Com);
    PGMSTR(CrA);
    PGMSTR(CrB);
    PGMSTR(Crv);
    PGMSTR(Crt);
    PGMSTR(Cru);
    PGMSTR(Cyg);
    PGMSTR(Del);
    PGMSTR(Dor);
    PGMSTR(Dra);
    PGMSTR(Equ);
    PGMSTR(Eri);
    PGMSTR(For);
    PGMSTR(Gem);
    PGMSTR(Gru);
    PGMSTR(Her);
    PGMSTR(Hor);
    PGMSTR(Hya);
    PGMSTR(Hyi);
    PGMSTR(Ind);
    PGMSTR(Lac);
    PGMSTR(Leo);
    PGMSTR(LMi);
    PGMSTR(Lep);
    PGMSTR(Lib);
    PGMSTR(Lup);
    PGMSTR(Lyn);
    PGMSTR(Lyr);
    PGMSTR(Men);
    PGMSTR(Mic);
    PGMSTR(Mon);
    PGMSTR(Mus);
    PGMSTR(Nor);
    PGMSTR(Oct);
    PGMSTR(Oph);
    PGMSTR(Ori);
    PGMSTR(Pav);
    PGMSTR(Peg);
    PGMSTR(Per);
    PGMSTR(Phe);
    PGMSTR(Pic);
    PGMSTR(Psc);
    PGMSTR(PsA);
    PGMSTR(Pup);
    PGMSTR(Pyx);
    PGMSTR(Ret);
    PGMSTR(Sge);
    PGMSTR(Sgr);
    PGMSTR(Sco);
    PGMSTR(Scl);
    PGMSTR(Sct);
    PGMSTR(Ser);
    PGMSTR(Sex);
    PGMSTR(Tau);
    PGMSTR(Tel);
    PGMSTR(Tri);
    PGMSTR(TrA);
    PGMSTR(Tuc);
    PGMSTR(UMa);
    PGMSTR(UMi);
    PGMSTR(Vel);
    PGMSTR(Vir);
    PGMSTR(Vol);
    PGMSTR(Vul);
    const char* const cons_table[] PROGMEM = {Eri, nul, nul, nul, nul, nul, nul, nul, nul, Lup, Cir, nul, nul, nul, Peg, nul, nul, Vir, nul, nul, nul, nul, nul, nul, nul, Per, nul, Mic, nul, nul, nul, nul, Ori, nul, nul, Ind, nul, Vul, nul, Hor, nul, nul, nul, nul, nul, Gem, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Tau, Mus, nul, nul, nul, nul, nul, nul, nul, nul, nul, CVn, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Scl, nul, nul, Sco, nul, nul, nul, nul, Sct, nul, nul, Her, nul, nul, nul, nul, nul, nul, nul, nul, nul, And, nul, nul, nul, nul, nul, nul, nul, nul, Gru, nul, PsA, nul, Psc, UMa, Lac, Ant, nul, nul, nul, nul, nul, UMi, nul, Ret, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Phe, nul, nul, nul, nul, nul, Cae, nul, nul, nul, nul, LMi, nul, nul, Cam, nul, nul, Cap, nul, Car, Cas, nul, nul, nul, nul, nul, nul, nul, CMa, nul, nul, nul, nul, nul, nul, nul, CMi, nul, nul, nul, nul, nul, Lyn, Ser, nul, Pic, Lyr, nul, nul, Sex, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Cyg, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Aps, nul, nul, nul, Cnc, nul, Boo, nul, nul, nul, nul, Pup, nul, nul, nul, nul, nul, nul, nul, nul, nul, Tel, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Aql, nul, nul, nul, nul, nul, Aqr, nul, nul, nul, nul, Sge, nul, nul, nul, nul, nul, nul, nul, nul, Col, Com, nul, nul, Sgr, nul, nul, Vol, nul, nul, nul, nul, nul, nul, nul, nul, Ara, nul, nul, nul, nul, nul, nul, TrA, Ari, nul, nul, nul, nul, nul, nul, Tri, nul, nul, Mon, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Leo, Lep, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Dor, nul, nul, nul, nul, nul, nul, Cen, nul, Cep, nul, nul, Vel, Cet, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Pav, nul, Nor, nul, nul, nul, nul, nul, nul, Men, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Del, nul, nul, Oct, nul, CrA, CrB, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Pyx, nul, Crt, Cru, Crv, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, nul, Tuc, nul, nul, nul, nul, nul, nul, nul, Aur, nul, nul, For, nul, nul, nul, nul, nul, nul, nul, nul, nul, Dra, nul, Oph, nul, nul, nul, nul, nul, nul, nul, nul, Cha, nul, nul, nul, nul, nul, nul, nul, Hya, nul, nul, nul, nul, nul, nul, nul, Hyi, nul, nul, Equ, nul, nul, nul, Lib};
  }

  bool initFlag{};
  // uint8_t len = 1;

  template<typename T>
  T _constrain(T num, T lower, T upper) {
    return constrain(num, lower, upper);
  }

  namespace FieldUtil{
    uint16_t colorFromCI(float b_v) {
      float T = 46.0f * (1 / (0.92f * b_v + 1.7f) + 1 / (0.92f * b_v + 0.62f));
      uint16_t r, g, b;
      r = (T <= 66) * 0b11111 + (T > 66) * ((uint16_t)(_constrain(329.698727446f * (float)pow(T - 60, -0.1332047592f), 0.0f, 255.0f) + 0.5f) >> 3);
      g = ((T <= 66) * (uint16_t)(_constrain(99.4708025861f * (float)log(T) - 161.1195681661f, 0.0f, 255.0f) + 0.5f) + (T > 66) * (uint16_t)(_constrain(288.1221695283f * (float)pow(T - 60, -0.0755148492f), 0.0f, 255.0f) + 0.5f)) >> 2;
      b = (T >= 66) * 0b11111 + (T < 66 && T > 19) * ((uint16_t)(_constrain(138.5177312231f * (float)log(T - 10) - 305.0447927307f, 0.0f, 255.0f) + 0.5f) >> 3);
      return (r << 11) | (g << 5) | b;
    }

    char* toIAU(char* con){
      static char res[4];
      strcpy_P(res,(char*)pgm_read_word(&Cons::cons_table[((uint16_t)(*con)*57 + (uint16_t)(*(con+1))*47 + *(con+2) + 78) % 538] )); //57=47*47 %538
      return res;
    }
  }

  namespace FieldClass{
    class _Field {
      public:
        virtual _Field* draw() = 0;
        virtual bool isFieldPressed() const = 0;
        virtual int8_t activate() = 0;
        virtual _Field* getFieldData(char* dest) = 0;
        virtual _Field* reset() = 0;
        virtual ~_Field() = default;
    };

    class _BoxField : public _Field {
      protected:
        char _dat;
        const uint16_t _x, _y, _width, _height, _activeColor, _inactiveColor, _textX, _textY, _textColor;
        const uint8_t _textSize;
        virtual void show(unsigned char chr) const = 0;

      public:
        //----------------------------------------------constructor----------------------------------------------
        _BoxField(uint16_t x, uint16_t y, uint16_t textColor = RED, uint16_t textSize = 2, uint16_t wid = 22,
                  uint16_t heig = 22, uint16_t activeColor = GREEN, uint16_t inactiveColor = WHITE)
          : _x(x), _y(y), _width(wid), _height(heig), _activeColor(activeColor), _inactiveColor(inactiveColor), _textColor(textColor), _textX(x + (wid >> 1) - ((textSize * 5) >> 1)), _textY(y + (heig >> 1) + 2 * textSize + 2) {
          if (!initFlag)
            Serial1.begin(300);
          initFlag = true;
        }

        //-------------------------------------------------reset-------------------------------------------------
        _Field* reset() override {
          _dat = 0;
          return this;
        }

        //--------------------------------------------------draw-------------------------------------------------
        _Field* draw() override {
          debugln(F("Drawing..."));
          tft.drawRect(_x, _y, _width, _height, _inactiveColor);
          return this;
        }

        //---------------------------------------------isFieldpressed--------------------------------------------
        bool isFieldPressed() const override {
          if (touch.scan(0)) {
            return touch.x >= _x && touch.x <= _x + _width && touch.y >= _y && touch.y <= _y + _height;
          }
          return false;
        }

        //------------------------------------------------activate-----------------------------------------------
        int8_t activate() override {
          debugln(F("Enter"));
          tft.drawRect(_x, _y, _width, _height, _activeColor);
          uint8_t buf = 0;
          u8g2.setFont(fonts[_textSize - 1]);
          while (!touch.scan(0) || isFieldPressed()) {
            if (Serial1.available()) {
              buf = Serial1.read();
              debugln(buf);
              //tft.fillRect(_textX, _textY, 2 | _textSize << 2, 10 * _textSize, BLACK);
              if (buf == '\n')  //enter
                break;
              else if (isalpha(buf)) {  //letter
                _dat = buf;
                debugln(F("Exiting loop"));
                show(_dat);
                draw();
                debugln(F("Finished"));
                return 2;
              } else if (buf == '\b')  //backspace
                _dat = 0;
              else if (buf == '>') {  //right arrow
                draw();
                return 2;
              } else if (buf == '<') {  //left arrow
                draw();
                return -2;
              } else if (buf == '_') {  //down arrow
                draw();
                return 1;
              } else if (buf == '^') {  //up arrow
                draw();
                return -1;
              }  //other cases are mistakes
              //end if
              show(_dat);
            }  //end if
            touch.scan(0);
          }  //end while
          debugln(F("Exiting loop"));
          draw();
          u8g2.setFont(fonts[_textSize - 1]);
          show(_dat);
          debugln(F("Finished"));
          Serial.flush();
          return 0;
        }

        //----------------------------------------------getFieldData---------------------------------------------
        _Field* getFieldData(char* dest) override {
          getFieldData((int8_t*)dest);
          return this;
        }

        virtual _Field* getFieldData(int8_t* dest) = 0;
        //virtual void getFieldData(uint8_t* dest) = 0;
    };

    class Field : public _Field {
      private:
        void _strins(char* subject, const char& insert, int pos) {
          if (pos < 0) {
            pos = 0;
          }
          char* buf = new char[strlen(subject) + 2];
          memset(buf, 0, strlen(subject) + 2);
          strncpy(buf, subject, pos);  // copy at most first pos characters
          int len = strlen(buf);
          *(buf + len) = insert;             // copy insert[] to the end
          ++len;                             // increase the length by length of insert[]
          strcpy(buf + len, subject + pos);  // copy the rest
          strcpy(subject, buf);              // copy it back to subject
          delete[] buf;
        }
        void _strbck(char* str, const uint8_t& ind) {
          memmove(str + ind - 1, str + ind, strlen(str) - ind);
          memset(str + strlen(str) - 1, 0, 1);
        }
        const uint16_t _textX, _textY, _lineY, _textColor, _activeColor, _inactiveColor, _linelen;
        const uint8_t _textSize, _maxChar, _textWidth;
        char* _fieldText;

      public:
        //---------------------------------------------constructor---------------------------------------------
        Field(
          uint16_t textX,
          uint16_t textY,
          uint8_t textSize,
          uint8_t maxChar,
          uint16_t textColor,
          uint16_t activeColor = GREEN,
          uint16_t inactiveColor = WHITE)
          : _textX(textX), _textY(textY + textSize * 10 - 1), _textSize(textSize), _lineY(textY + textSize * 10), _maxChar(maxChar), _textColor(textColor), _activeColor(activeColor), _inactiveColor(inactiveColor), _linelen((uint16_t)(2 | textSize << 2) * maxChar), _textWidth(2 | textSize << 2) {
          if (!initFlag)
            Serial1.begin(300);
          initFlag = true;
          _fieldText = new char[maxChar + 1];
          memset(_fieldText, 0, maxChar + 1);
        }

        //-----------------------------------------------reset------------------------------------------------
        _Field* reset() override {
          //      tft.fillRect(_lineX, _lineY - 10 * _textSize, _linelen, 10 * _textSize, BLACK);
          //      u8g2.setFont(fonts[_textSize - 1]);
          //      u8g2.setCursor(_lineX, _textY);
          //      u8g2.setForegroundColor(_inactiveColor);
          //      u8g2.print(_fieldText);
          memset(_fieldText, 0, strlen(_fieldText));
          return this;
        }

        //------------------------------------------------draw------------------------------------------------
        _Field* draw() override {
          tft.drawFastHLine(_lineX, _lineY, _linelen, _inactiveColor);
          return this;
        }

        //-------------------------------------------isFieldpressed--------------------------------------------
        bool isFieldPressed() const override{
          touch.scan(0);
          if (touch.isPressed) {
            return touch.x >= _lineX && touch.x <= _lineX + _linelen && touch.y <= _lineY && touch.y >= _lineY - _textSize * 10;
          }
          return false;
        }

        //----------------------------------------------activate----------------------------------------------
        int8_t activate() override {
          debugln(F("Enter"));
          int8_t cursor = strlen(_fieldText);
          tft.drawFastHLine(_lineX, _lineY, _linelen, _activeColor);
          int8_t buf;
          u8g2.setFont(fonts[_textSize - 1]);
          u8g2.setForegroundColor(_textColor);
          while (!touch.scan(0) || isFieldPressed()) {
            if (Serial1.available()) {
              buf = Serial1.read();
              tft.fillRect(_lineX, _lineY - 10 * _textSize, _linelen, 10 * _textSize, BLACK);
              if (buf == '\n') {  //enter
                u8g2.setCursor(_textX, _textY);
                u8g2.setForegroundColor(_inactiveColor);
                u8g2.print(_fieldText);
                break;
              } else if (isalnum(buf) || strchr("+-'/., ", buf)) {  //normal char
                //debug((char)buf);
                _strins(_fieldText, buf, cursor);
                DRAWCURSOR(BLACK);
                ++cursor;
                debugln(_fieldText);
              } else if (buf == '\b') {  //backspace
                if (*_fieldText && cursor) {
                  _strbck(_fieldText, cursor);
                  cursor--;
                }
              } else if (buf == '>') {  //right arrow
                cursor = _constrain(cursor + 1, 0, _maxChar - 1);
                DRAWCURSOR(_inactiveColor);
              } else if (buf == '<') {  //left arrow
                cursor = _constrain(cursor - 1, 0, _maxChar - 1);
                DRAWCURSOR(_inactiveColor);
              } else if (buf == '_')  //down arrow
                return 1;
              else if (buf == '^')  //up arrow
                return -1;
              //other cases are fuckin' mistake
              //end if
              u8g2.setCursor(_lineX, _textY);
              u8g2.print(_fieldText);
            }  //end if
            touch.scan(0);
          }  //end while
          DRAWCURSOR(BLACK);
          draw();
          return 0;
        }

        //--------------------------------------------getFieldData--------------------------------------------
        _Field* getFieldData(char* dest) override {
          strcpy(dest, (const char*)_fieldText);
          return this;
        }

        //---------------------------------------------destructor---------------------------------------------
        virtual ~Field() {
          delete[] _fieldText;
        }
    };

    class LatinBoxField : public _BoxField {
      public:
        LatinBoxField(uint16_t x, uint16_t y, uint16_t textColor = RED, uint16_t textSize = 2, uint16_t wid = 22,
                      uint16_t heig = 22, uint16_t activeColor = GREEN, uint16_t inactiveColor = WHITE)
          : _BoxField(x, y, textColor, textSize, wid, heig, activeColor, inactiveColor) {}

        _Field* getFieldData(int8_t* dest) override {
          *dest = _dat;
          return this;
        }

        virtual ~LatinBoxField() = default;

      protected:
        void show(unsigned char chr) const override {
          debugln(F("Showing..."));
          tft.fillRect(_x + 1, _y + 1, _width - 2, _height - 2, BLACK);
          u8g2.setFont(SIZE2);
          u8g2.setCursor(_textX, _textY);
          debug(_textX, DEC);
          debug(F(","));
          debugln(_textY, DEC);
          u8g2.setForegroundColor(_textColor);
          u8g2.print((char)chr);
          debugln(F("Finish showing"));
        }
    };

    const char* greeks[26] = { "α", "β", "ψ", "δ", "ε", "φ", "γ", "η", "ι", "ξ", "κ", "λ", "μ", "ν", "ο", "π", "", "ρ", "σ", "τ", "θ", "ω", "", "χ", "υ", "ζ" };
    const char* greekKey[26] = { "alpha", "beta", "psi", "delta", "epsilon", "phi", "gamma", "eta", "iota", "xi", "kappa", "lambda", "mu", "nu", "omicron", "pi", "", "rho", "sigma", "tau", "theta", "omega", "", "chi", "upsilon", "zeta" };

    class GreekBoxField : public _BoxField {
      public:
        GreekBoxField(uint16_t x, uint16_t y, uint16_t textColor = RED, uint16_t textSize = 2, uint16_t wid = 22,
                      uint16_t heig = 22, uint16_t activeColor = GREEN, uint16_t inactiveColor = WHITE)
          : _BoxField(x, y, textColor, textSize, wid, heig, activeColor, inactiveColor) {}

        _Field* getFieldData(int8_t* dest) override {
          if (islower(_dat)) {
            strcpy(dest, greekKey[_dat - 'a']);
          } else {
            strcpy(dest, "");
          }
          return this;
        }

        virtual ~GreekBoxField() = default;

      protected:
        void show(unsigned char chr) const override {
          debugln(F("Showing..."));
          tft.fillRect(_x + 1, _y + 1, _width - 2, _height - 2, BLACK);
          u8g2.setForegroundColor(_textColor);
          u8g2.setFont(SIZE2);
          u8g2.setCursor(_textX, _textY);
          debugln(F("cursor set"));
          if (islower(chr)) {
            debugln(greeks[chr - 'a']);
            u8g2.print(greeks[chr - 'a']);
          }
          debugln(F("Finish showing"));
        }
    };

    const float bvfactor = 0.00974074074f, bvoffset = 2.73962962f;
    class ColorBarField : public _Field {
      private:
        const uint16_t _xleft, _y, _height, _activeColor, _inactiveColor, _xright;
        const bool _pos;
        ColorBarField* _myPair;
        uint16_t _xnow;
        _Field* _drawCursor(uint16_t x, uint16_t color) const {
          for (auto i = 0; i <= 4; i++) {
            tft.drawFastHLine(x - 2 + (i >> 1), _y - 5 + i, 5 - i & 0xfe, color);
            tft.drawFastHLine(x - 2 + (i >> 1), _y + _height + 5 - i, (5 - i) & 0xfe, color);
          }
          return this;
          //tft.drawTriangle(x - 2, _y - 5, xoff + _xleft + 2, _y - 5, xoff + _xleft, _y - 1, color);
          //tft.drawTriangle(x - 2, _y + _height + 5, xoff + _xleft + 2, _y + _height + 5, xoff + _xleft, _y + _height + 1, color);
        }
        _Field* _clearCursor(uint16_t x) const {
          tft.fillRect(x - 2, _y - 5, 5, 5, BLACK);
          tft.fillRect(x - 2, _y + _height + 1, 5, 5, BLACK);
          return this;
        }

      public:
        //---------------------------constructor---------------------------
        ColorBarField(uint16_t x, uint16_t y, bool pos, uint16_t hei = 10, uint16_t activeColor = GREEN, uint16_t inactiveColor = WHITE)
          : _xleft(x), _xright(x + 270), _y(y), _height(hei), _activeColor(activeColor), _inactiveColor(inactiveColor), _pos(pos) {}
        //-------------------------------draw------------------------------
        _Field* draw() override {
          _xnow = _xleft + 270 * _pos;
          _drawCursor(_xnow, _inactiveColor);
          return this;
        }

        //-------------------------------------------------reset-------------------------------------------------
        _Field* reset() override {
          return this;
        }

        //--------------------------isFieldPressed-------------------------
        bool isFieldPressed() const override {
          touch.scan(0);
          if (touch.isPressed) {
            if (touch.x >= _xnow - 4 && touch.x <= _xnow + 4 && touch.y >= _y - 5 && touch.y < _y + _height + 5)
              return true;
          }
          return false;
        }

        //-----------------------------activate----------------------------
        int8_t activate() override {
          debugln(F("Enter"));
          _drawCursor(_xnow, _activeColor);
          while (isFieldPressed() && (_pos || touch.x <= _myPair->_xnow - 5) && (!_pos || touch.x >= _myPair->_xnow + 5)) {
            if (_xnow != touch.x) {
              _clearCursor(_xnow);
              _xnow = touch.x;
              _drawCursor(_xnow, _activeColor);
            }
          }
          _drawCursor(_xnow, _inactiveColor);
          return 0;
        }

        //---------------------------getFieldData---------------------------
        _Field* getFieldData(char* dest) override{
          float b_v = bvoffset - bvfactor * _xnow;
          *(float*)dest = b_v;
          return this;
        }
        _Field* getFieldData(float* dest) {
          float b_v = bvoffset - bvfactor * _xnow;
          *dest = b_v;
          return this;
        }

        //-------------------------------pair------------------------------
        friend bool pair(ColorBarField& bar1, ColorBarField& bar2) {
          if (bar1._pos != bar2._pos) {
            bar1._myPair = &bar2;
            bar2._myPair = &bar1;
            return true;
          }
          return false;
        }
    };
  }
}

using namespace _FieldLib::FieldClass;
using namespace _FieldLib::FieldUtil;
using namespace _FieldLib::Cons;

#endif