// for debug only
#define DEBUG
#define DEBUG_SERIAL Serial

#include <tempdebug.h>

#include <SPI.h>
#include <Wire.h>
#include <u8g2_fonts.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <LinkedList.h>
#include "AstroTime.h"
#include "FieldReader.h"
#include "Pointer.h"
#include "Starpointer.h"
#include "FieldLib.h"

#ifndef USETOUCH
#define USETOUCH
#include <LCDWIKI_ALIASES.h>
#endif

// for UART
#define GPS Serial3

// Azimuthal Motor
#define AZ_1 2
#define AZ_2 3
#define AZ_3 4
#define AZ_4 5

// Altitudinal Motor
#define AL_1 6
#define AL_2 7
#define AL_3 8
#define AL_4 9

// tft SPI
#define tft_CS A5
#define tft_CD A3
#define tft_RST A4
#define tft_LED 10

#define CEN_X 160
#define CEN_Y 120

#define SCREEN_W 320
#define SCREEN_H 240

// TOUCH SPI
#define T_CLK 46
#define T_CS 45
#define T_DIN 48
#define T_DOUT 47
#define T_IRQ 49

//font size
#define SIZE1 u8g2_font_6x10_tf
#define SIZE2 u8g2_font_10x20_t_greek

const float tolerance = 0.1f;  //position tolerance in radians

/*-----end define-----*/

/*------------------------------Don't edit this------------------------------*/
#define CATALOGSIZE 1312

// uint16_t oldPotVal = 0, potVal;

#define createReader(type, identifier) fieldReader<type> identifier
#define beginReader(id) id.begin(#id)

Starpointer starptr;
Adafruit_ILI9341 tft = Adafruit_ILI9341(tft_CS, tft_CD, tft_RST);
U8G2_FOR_ADAFRUIT_GFX u8g2;
LCDWIKI_TOUCH touch = LCDWIKI_TOUCH(T_CS, T_CLK, T_DOUT, T_DIN, T_IRQ);

createReader(float, b_v);
createReader(char*, bayer);
createReader(char*, cons);
createReader(char*, constell);
createReader(uint8_t, decarcm);
createReader(uint8_t, decarcs);
createReader(uint8_t, decdeg);
createReader(char, decsign);
createReader(char*, gen_con);
createReader(float, mag);
createReader(char*, name);
createReader(uint8_t, RAh);
createReader(uint8_t, RAm);
createReader(float, RAs);
/*-----------------------------Now you can edit!-----------------------------*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);
  digitalWrite(LED_BUILTIN, LOW);
  debugln("Hi");
  //  pinMode(A5, OUTPUT);
  //  digitalWrite(A5, HIGH);
  beginSD();
  digitalWrite(LED_BUILTIN, LOW);
  beginScr();  //Start the screen
  u8g2.setCursor(115, 126);
  u8g2.print(F("Initializing..."));  //Show init message
  begin();                           //Initialize other things
}

void loop() {
  drawMainScr();
  while (!touch.scan(0))
    ; // wait for touch
  uint16_t x{ touch.x }, y{ touch.y };
  uint32_t prev{ millis() };
  if (x > SCREEN_W || y > SCREEN_H) {
    return;
  }
  while (touch.scan(0)) {
    if (millis() - prev >= 5000) {  //if screen is pressed for at least 5 sec
      showCoor();                   //show pointer coordinates menu
      return;
    }
  }
  if (y < 200) {      //top menu
    if (x < CEN_X) {  //left menu
      goTo();
    } else {  //right menu
      identify();
    }
  } else {  //bottom menu
    calibrate();
  }
}

/*---------select SPI device----------*/
void selectSPI(short pin) {
  static const short SPI_cs[]{ SD_CS, tft_CS };
  for (auto i : SPI_cs) {  //bring all pins high
    digitalWrite(i, HIGH);
  }
  digitalWrite(pin, LOW);  //set desired pin low
}

//FFFFFFD4, 4912, FFFFFFFF, 0, FFFFF813, 3B9

/*---------initialize SD card---------*/
void beginSD() {
  selectSPI(SD_CS);
  beginReader(b_v);
  beginReader(bayer);
  beginReader(cons);
  beginReader(constell);
  beginReader(decdeg);
  beginReader(decarcm);
  beginReader(decarcs);
  beginReader(decsign);
  beginReader(gen_con);
  beginReader(mag);
  beginReader(name);
  beginReader(RAh);
  beginReader(RAm);
  beginReader(RAs);
}

/*-------initialize main system-------*/
void begin() {
  debugln(F("Initializing..."));

  GPS.begin(9600);
  debugln(F("GPS Initialized"));
  Wire.flush();
  starptr.begin(GPS, AZ_1, AZ_3, AZ_2, AZ_4, CLOCKWISE, AL_1, AL_3, AL_2, AL_4, CLOCKWISE);

  debugln(F("Initializing done!"));
  selectSPI(tft_CS);
  tft.fillScreen(BLACK);
}

/*-----------initialize UI-----------*/
void beginScr() {
  selectSPI(tft_CS);
  tft.begin();  //initialize entire screen
  tft.setRotation(3);
  u8g2.begin(tft);
  u8g2.setFont(SIZE1);
  touch.initTouch(3, tft.width(), tft.height());  //initialize touchscreen
  u8g2.setForegroundColor(RED);
  tft.fillScreen(BLACK);
}

/*-------------MAIN MENU-------------*/
void drawMainScr() {
  selectSPI(tft_CS);
  tft.fillScreen(RED);
  u8g2.setForegroundColor(BLACK);
  u8g2.setBackgroundColor(RED);
  u8g2.setFont(SIZE2);
  u8g2.setCursor(200, 110);
  u8g2.print(F("IDENTIFY"));
  u8g2.setCursor(60, 110);
  u8g2.print(F("GOTO"));
  tft.drawFastVLine(CEN_X, 0, 200, BLACK);
  tft.drawFastHLine(0, 200, 320, BLACK);
  u8g2.setCursor(115, 230);
  u8g2.print(F("CALIBRATE"));
}

/*------------format text------------*/
void formatText(uint16_t fg, uint16_t bg, const uint8_t* font) {
  u8g2.setForegroundColor(fg);
  u8g2.setBackgroundColor(bg);
  u8g2.setFont(font);
}

/*-------------print text------------*/
template<typename T> void printText(int16_t x, int16_t y, T text) {
  u8g2.setCursor(x, y);
  u8g2.print(text);
}

/*----------SHOW COOR MENU-----------*/
void showCoor() {
  selectSPI(tft_CS);
  tft.fillScreen(BLACK);
  formatText(RED, BLACK, SIZE2);
  printText(50, 105, F("Press anywhere to exit"));
  printText(70, 130, F("Azimuth:"));
  printText(60, 155, F("Altitude:"));

  uint16_t count{};
  float az{}, al{};
  while (touch.scan(0))
    ;  //wait until the screen is unpressed
  uint32_t prev{ millis() };
  u8g2.setForegroundColor(WHITE);
  char _disp[7];
  while (!touch.scan(0)) {
    if (millis() - prev < 250) {  //if .1 second has not passed
      az += starptr._getAz();
      debugln(az);
      delay(10);
      al += starptr._getAl();
      debugln(al);
      ++count;
      delay(10);
    } else {  //.25 sec passed
      prev = millis();
      tft.fillRect(CEN_X, 110, 60, 20, BLACK);
      tft.fillRect(CEN_X, 135, 60, 20, BLACK);
      printText(CEN_X, 130, dtostrf(degrees(az / count), 6, 2, _disp));
      printText(CEN_X, 155, dtostrf(degrees(al / count), 5, 2, _disp));
      al = 0;
      az = 0;
      count = 0;
    }
  }
}

/*----------CALIBRATE MENU-----------*/
void calibrate() {
  selectSPI(tft_CS);
  tft.fillScreen(BLACK);
  formatText(RED, BLACK, SIZE2);

  //countdown
  printText(45, 130, F("Calibration starts in"));
  u8g2.setForegroundColor(WHITE);
  for (auto i = 5; i > 0; i--) {
    printText(265, 130, char(i + 48));
    delay(995);
  }
  tft.fillRect(25, 110, 270, 20, BLACK);

  printText(25, 130, F("Keep moving the device..."));
  starptr.calibrate_compass();
  tft.fillRect(25, 110, 270, 20, BLACK);
  printText(45, 120, F("Calibration finished"));
  printText(30, 140, F("Press anywhere to continue"));
  while (!touch.scan(0))
    ;
}

/*-----------IDENTIFY MENU-----------*/
void identify() {
  selectSPI(tft_CS);
  tft.fillScreen(BLACK);
  uint16_t x{}, y{};  //variable for touch coordinate
  formatText(RED, BLACK, SIZE1);
  printText(82, 27, F("Point the laser pointer to"));
  printText(70, 40, F("the star you want to identify."));
  tft.fillCircle(160, 120, 30, GREEN);
  formatText(BLACK, GREEN, SIZE2);
  printText(152, 128, F("OK"));
  tft.fillTriangle(125, 135, 125, 105, 108, 120, RED);  //left button
  tft.fillTriangle(195, 135, 195, 105, 212, 120, RED);  //right button
  tft.fillTriangle(145, 85, 175, 85, 160, 68, RED);     //down button
  tft.fillTriangle(145, 155, 175, 155, 160, 172, RED);  //up button
  debugln(F("Screen drawn"));
  while (1) {
    starptr.run();        //activate pointer movement
    if (touch.scan(0)) {  //when screen touched
      x = touch.x >> 5;
      y = (touch.y - 8U) >> 4;
      debug(F("x: "));
      debug(touch.x);
      debug(F(" y: "));
      debugln(touch.y);
      /*...0   32  64  92 128 160 192 224 256 288 320
        ...+-0-+-1-+-2-+-3-+-4-+-5-+-6-+-7-+-8-+-9-+0
        ..0|   |   |   |   |   |   |   |   |   |   |
        ...|   |   |   |   |   |   |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+24
        ..1|   |   |   |   |   |   |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+40
        ..2|   |   |   |   |   |   |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+56
        ..3|   |   |   |   |       |   |   |   |   |
        ...+---+---+---+---+   ^   +---+---+---+---+72
        ..4|   |   |   |   |       |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+88
        ..5|   |   |   |   |       |   |   |   |   |
        ...+---+---+---+---+       +---+---+---+---+104
        ..6|   |   |   |   |       |   |   |   |   |
        ...+---+---+---+ < +  OK   + > +---+---+---+120
        ..7|   |   |   |   |       |   |   |   |   |
        ...+---+---+---+---+       +---+---+---+---+136
        ..8|   |   |   |   |       |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+152
        ..9|   |   |   |   |       |   |   |   |   |
        ...+---+---+---+---+   v   +---+---+---+---+168
        .10|   |   |   |   |       |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+184
        .11|   |   |   |   |   |   |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+200
        .12|   |   |   |   |   |   |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+216
        .13|   |   |   |   |   |   |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+232
        .14|   |   |   |   |   |   |   |   |   |   |
        ...+---+---+---+---+---+---+---+---+---+---+240*/
      switch (x) {
        case 6:
          starptr.west();
          debugln("west");
          break;
        case 4:
        case 5:
          switch (y) {
            case 5:
            case 6:
            case 7:
            case 8:
              debugln("OK");

              tft.fillScreen(BLACK);
              formatText(RED, BLACK, SIZE2);
              printText(90, 130, F("Calculating..."));

              LinkedList<int16_t> indices;
              starptr.readAzAl();
              HMS ra{ starptr.rightAscension() };
              dms dec{ starptr.declination() };
              dec.print();
              int8_t sign{};
              int d{}, H{};
              int* _nothing{ new int };  //dummy variable
              selectSPI(SD_CS);
              dec.getdms(&d, _nothing, _nothing);
              ra.getHMS(&H, _nothing, _nothing);
              delete _nothing;
              d = abs(d);
              int16_t* med{ decdeg.indicesOf(d) };
              int16_t currInd{ *med };
              while (currInd != -1) {
                sign = decsign[currInd] == '-' ? -1 : 1;
                if (absRadiansDiff(dec, dms(sign * decdeg[currInd], sign * decarcm[currInd], sign * decarcs[currInd])) <= tolerance) {
                  if (absRadiansDiff(ra, HMS(RAh[currInd], RAm[currInd], RAs[currInd])) <= tolerance) {
                    indices.add(currInd);
                    debugln(RAh[currInd], DEC);
                    debugln(absRadiansDiff(ra, HMS(RAh[currInd], RAm[currInd], RAs[currInd])));
                  }
                }
                ++med;
                currInd = *med;
              }
              int16_t* upper{ decdeg.indicesOf(d + 1) };
              currInd = *upper;
              while (currInd != -1) {
                sign = decsign[currInd] == '-' ? -1 : 1;
                if (absRadiansDiff(dec, dms(sign * decdeg[currInd], sign * decarcm[currInd], sign * decarcs[currInd])) <= tolerance) {
                  if (absRadiansDiff(ra, HMS(RAh[currInd], RAm[currInd], RAs[currInd])) <= tolerance) {
                    indices.add(currInd);
                  }
                }
                ++upper;
                currInd = *upper;
              }
              int16_t* lower{ decdeg.indicesOf(d - 1) };
              currInd = *lower;
              while (currInd != -1) {
                sign = decsign[currInd] == '-' ? -1 : 1;
                if (absRadiansDiff(dec, dms(sign * decdeg[currInd], sign * decarcm[currInd], sign * decarcs[currInd])) <= tolerance) {
                  if (absRadiansDiff(ra, HMS(RAh[currInd], RAm[currInd], RAs[currInd])) <= tolerance) {
                    indices.add(currInd);
                  }
                }
                ++lower;
                currInd = *lower;
              }
              selectSPI(tft_CS);
              tft.fillScreen(BLACK);
              if (indices.size() > 0) {
                debugln("Star found");
                dispStarInfo(indices.shift());
                printText(1, 165, F("This seems wrong?(e.g. the color seems strange)"));
                tft.fillRoundRect(285, 160, 34, 22, 6, RED);
                u8g2.setBackgroundColor(RED);
                u8g2.setForegroundColor(BLACK);
                printText(293, 171, F("TRY"));
                printText(287, 181, F("AGAIN"));
                tft.fillRoundRect(5, 200, 310, 30, 6, GREEN);
                u8g2.setBackgroundColor(GREEN);
                u8g2.setFont(SIZE2);
                printText(75, 225, F("Back to main menu"));
                while (true) {
                  if (touch.scan(0)) {
                    if ((touch.x < 285 || touch.x > 319 || touch.y < 160 || touch.y > 182) && (touch.x < 5 || touch.x > 315 || touch.y < 200 || touch.y > 230))  //not pressed on button
                      continue;
                    if (touch.x >= 5 && touch.x <= 315 && touch.y >= 200 && touch.y <= 230)  //back to main menu
                      return;
                    if (!indices.size()) {
                      tft.fillScreen(BLACK);
                      formatText(RED, BLACK, SIZE2);
                      printText(55, 90, F("Sorry, nothing found."));
                      tft.fillRoundRect(143, 124, 34, 21, 6, GREEN);
                      formatText(BLACK, GREEN, SIZE2);
                      printText(150, 144, F("OK"));
                      while (!(touch.scan(0) && (touch.x >= 143) && (touch.x <= 177) && (touch.y >= 124) && (touch.y <= 145)))
                        ;
                      return;
                    }
                  } else {
                    continue;
                  }
                  dispStarInfo(indices.shift());
                  selectSPI(tft_CS);
                  printText(1, 165, F("This seems wrong?(e.g. the color seems strange)"));
                  tft.fillRoundRect(285, 160, 34, 22, 6, RED);
                  u8g2.setBackgroundColor(RED);
                  u8g2.setForegroundColor(BLACK);
                  printText(293, 171, F("TRY"));
                  printText(287, 181, F("AGAIN"));
                  tft.fillRoundRect(5, 200, 310, 30, 6, GREEN);
                  u8g2.setBackgroundColor(GREEN);
                  u8g2.setFont(SIZE2);
                  printText(75, 225, F("Back to main menu"));
                }
              } else {
                debugln("Star not found");
                printText(50, 95, F("Sorry, star not found"));
                tft.fillRoundRect(143, 140, 34, 21, 6, GREEN);
                u8g2.setForegroundColor(BLACK);
                u8g2.setBackgroundColor(GREEN);
                printText(150, 160, F("OK"));
                formatText(RED, BLACK, SIZE1);
                printText(22, 125, F("Maybe you're pointing planet, or even nothing!"));
                touch.scan(0);
                while (!(touch.isPressed && touch.x >= 143 && touch.x <= 177 && touch.y >= 145 && touch.y <= 165)) {
                  touch.scan(0);
                }  //end while
                while (touch.scan(0))
                  ;
              }  //end ifelse
              return;
            case 3:
            case 4:
              starptr.up();
              debugln("up");
              break;
            case 9:
            case 10:
              starptr.down();
              debugln("down");
              break;
          }
          break;
        case 3:
          starptr.east();
          debugln("east");
          break;
      }
    }  //end if
  }
}


void dispStarInfo(int16_t ind) {
  /*+--------------------------------------+
    |          Name(if any)/Bayer          |
    |                                      |
    |Bayer Designation                     |
    |Constellation                         |
    |apparent mag                          |
    |Right Ascention                       |
    |Declination                           |
    |Color              |||||||||||||||||| |
    |                                      |
    +--------------------------------------+*/
  selectSPI(tft_CS);
  tft.fillScreen(BLACK);
  u8g2.setForegroundColor(RED);
  u8g2.setBackgroundColor(BLACK);
  //-----------------------------------------title-----------------------------------------
  u8g2.setFont(SIZE2);
  char _name[18]{}, _bayer[8]{}, _disp[23]{}, _disp2[5]{}, _disp3[25]{}, _gen_con[20]{};
  selectSPI(SD_CS);
  strcpy(_gen_con, gen_con[ind]);
  *_gen_con = toupper(*_gen_con);
  strcpy(_name, name[ind]);
  *_name = toupper(*_name);
  strcpy(_bayer, bayer[ind]);
  strcpy(_disp, strcmp(_name, "                 ") ? _name : strcat(strcat(toGreek(_bayer), " "), _gen_con));
  selectSPI(tft_CS);
  printText(160 - 5 * strlen(_disp), 45, _disp);
  //-----------------------------------------bayer-----------------------------------------
  u8g2.setFont(SIZE1);
  if (strcmp(_name, "                 ")) {  //if name is not empty
    printText(3, 65, F("Bayer Designation:"));
    u8g2.setFont(SIZE2);
    printText(140, 65, toGreek(_bayer));
    u8g2.setFont(SIZE1);
    printText(153, 65, _gen_con);
  }
  //-------------------------------------constellation-------------------------------------
  printText(3, 85, "Constellation:");
  selectSPI(SD_CS);
  strcpy(_disp3, constell[ind]);
  _disp3[0] = toupper(_disp3[0]);
  strcat(_disp3, "(");
  strcat(_disp3, toIAU(cons[ind]));
  strcat(_disp3, ")");
  selectSPI(tft_CS);
  printText(140, 85, _disp3);
  //----------------------------------------App.Mag----------------------------------------
  printText(3, 100, F("Apparent Magnitude:"));
  selectSPI(SD_CS);
  dtostrf(mag[ind], 4, 2, _disp);
  //sprintf(_disp, "%.2f", mag[ind]);
  selectSPI(tft_CS);
  printText(140, 100, _disp);
  //------------------------------------------RA------------------------------------------
  printText(3, 115, F("Right Ascension"));
  selectSPI(SD_CS);
  dtostrf(RAs[ind], 4, 1, _disp2);
  sprintf(_disp, "%02hhuh%02hhum%ss", RAh[ind], RAm[ind], _disp2);
  selectSPI(tft_CS);
  printText(140, 115, _disp);
  //------------------------------------------dec------------------------------------------
  printText(3, 130, F("Declination"));
  selectSPI(SD_CS);
  sprintf(_disp, "%c%02hhu%c%02hhu'%02hhu\"", decsign[ind], decdeg[ind], 176, decarcm[ind], decarcs[ind]);
  selectSPI(tft_CS);
  printText(140, 130, _disp);
  //-----------------------------------------color-----------------------------------------
  printText(3, 150, F("Color:"));
  selectSPI(SD_CS);
  uint16_t _b_v = b_v[ind];
  selectSPI(tft_CS);
  tft.fillRect(140, 135, 177, 20, colorFromCI(_b_v));
}

char* toGreek(char* latin) {
  static char greek[3];
  switch (*latin) {
    case 'a':
      strcpy(greek, "α");
      break;
    case 'b':
      strcpy(greek, "β");
      break;
    case 'c':
      strcpy(greek, "χ");
      break;
    case 'd':
      strcpy(greek, "δ");
      break;
    case 'e':
      switch (*(latin + 1)) {
        case 'p':
          strcpy(greek, "ε");
          break;
        case 't':
          strcpy(greek, "η");
          break;
      }
      break;
    case 'g':
      strcpy(greek, "γ");
      break;
    case 'i':
      strcpy(greek, "ι");
      break;
    case 'k':
      strcpy(greek, "κ");
      break;
    case 'l':
      strcpy(greek, "λ");
      break;
    case 'm':
      strcpy(greek, "μ");
      break;
    case 'n':
      strcpy(greek, "ν");
      break;
    case 'o':
      switch (*(latin + 2)) {
        case 'i':
          strcpy(greek, "ο");
          break;
        case 'e':
          strcpy(greek, "ω");
          break;
      }
    case 'p':
      switch (*(latin + 1)) {
        case 'i':
          strcpy(greek, "π");
          break;
        case 'h':
          strcpy(greek, "φ");
          break;
        case 's':
          strcpy(greek, "ψ");
          break;
      }
      break;
    case 'r':
      strcpy(greek, "ρ");
      break;
    case 's':
      strcpy(greek, "σ");
      break;
    case 't':
      switch (*(latin + 1)) {
        case 'a':
          strcpy(greek, "τ");
          break;
        case 'h':
          strcpy(greek, "θ");
          break;
      }
      break;
    case 'u':
      strcpy(greek, "υ");
      break;
    case 'x':
      strcpy(greek, "ξ");
      break;
    case 'z':
      strcpy(greek, "ζ");
      break;
  }
  return greek;
}

void goTo() {
  Field nameField(36, 40, 1, 18, RED),
    constellationField(90, 80, 1, 20, RED),
    minMagField(162, 100, 1, 5, RED),
    maxMagField(272, 100, 1, 5, RED);
  GreekBoxField bayerField(213, 31);
  LatinBoxField consField[3] = {
    LatinBoxField(237, 31),
    LatinBoxField(261, 31),
    LatinBoxField(285, 31)
  };
  ColorBarField leftBV(40, 120, COLORBARLEFT), rightBV(40, 120, COLORBARRIGHT);
  _Field* mainFields[6] = {
    &nameField,
    &bayerField,
    &constellationField,
    &minMagField,
    &maxMagField,
    &leftBV
  };
  _BoxField* bayerFields[4] = {
    &bayerField,
    consField,
    consField + 1,
    consField + 2
  };
  pair(leftBV, rightBV);
  /*
    +----------------------------------------------------+
    +                    Star lookup                     +
    +                                      +-+ +-+ +-+   +
    +Name ____________________    Bayer __ | | | | | |   +
    +                                      +-+ +-+ +-+   +
    +                               Ex.  α  C   e   t    +
    +Constellation ____________________                  +
    +                                                    +
    +Magnitude  Min(Brightest):_____ Max(Dimmest):_____  +
    +          v                       v                 +
    +Color  |||||||||||||||||||||||||||||||||||||||||||  +
    +          ^                       ^                 +
    + Point [star name]_________ or try more specific search key+
    +----------------------------------------------------+
  */
  //--------------------labels---------------------
  //---------------no runtime change---------------
  selectSPI(tft_CS);
  tft.fillScreen(BLACK);
  formatText(YELLOW, BLACK, SIZE2);
  printText(105, 25, F("Star Lookup"));
  u8g2.setFont(SIZE1);
  u8g2.setForegroundColor(WHITE);
  printText(4, 50, F("Name"));
  printText(178, 50, F("Bayer"));
  printText(190, 73, F("Ex."));
  printText(4, 90, F("Constellation"));
  printText(4, 110, F("Magnitude  Min(Brightest):"));
  printText(192, 110, F("Max(Dimmest):"));
  printText(4, 130, F("Color"));
  printText(61, 190, F("Or try more specific search key"));
  for (auto i = 40; i < 311; i++) {
    tft.drawFastVLine(i, 120, 10, colorFromCI(bvoffset - bvfactor * i /*map((float)i, 40.0f, 310.0f, 2.35f, -0.28f)*/));
  }

  //--------------------fields---------------------
  for (auto i = 0; i < 5; i++) {
    mainFields[i]->draw()->reset();
  }
  for (auto i = 1; i < 4; i++) {
    bayerFields[i]->draw()->reset();
  }
  leftBV.draw();
  rightBV.draw();
  tft.drawRoundRect(28, 145, 264, 30, 5, GREEN);
  u8g2.setFont(SIZE2);
  printText(219, 73, F("α"));
  printText(243, 73, F("C"));
  printText(267, 73, F("e"));
  printText(290, 73, F("t"));
  //---------------------------------------------
  printText(135, 170, F("Point"));
  //---------------select the star---------------
  auto temp = 0;
  auto boxnum = 0;
  int8_t res = 0;
  bool mustUpdate;
  char _name[18], _bayer[8], _constell[20], _minMag[5], _maxMag[5];
  memset(_name, 0, 18);
  memset(_bayer, 0, 8);
  memset(_constell, 0, 20);
  memset(_minMag, 0, 5);
  memset(_maxMag, 0, 5);
  float minBV, maxBV;
  float oldminBV, oldmaxBV;
  char* mainFieldData[6] = { _name, _bayer, _constell, _minMag, _maxMag, (char*)&maxBV };
  char _oldname[18], _oldbayer[8], _oldconstell[20], _oldminMag[5], _oldmaxMag[5];
  char* oldmainFieldData[6] = { _oldname, _oldbayer, _oldconstell, _oldminMag, _oldmaxMag, (char*)&oldmaxBV };
  int8_t _cons[4] = { 0, 0, 0, 0 };
  int8_t* _consref[4] = { _cons, _cons + 1, _cons + 2, _cons + 3 };
  int8_t _oldcons[4] = { 0, 0, 0, 0 };
  int8_t* _oldconsref[4] = { _oldcons, _oldcons + 1, _oldcons + 2, _oldcons + 3 };
  int16_t match, bestMatch = 0;
  uint16_t bestMatchIndex = 0;
  char _btnTxt[25];
  while (1) {
    //check for pressing point button
    selectSPI(tft_CS);
    mustUpdate = false;
    touch.scan(0);
    if (touch.isPressed && touch.x >= 28 && touch.x <= 312 && touch.y >= 145 && touch.y <= 175) {
      break;
    }
    //-------------------------------

    //-check for pressing some field-
    for (auto i = 0; i < 6; i++) {
      if (i == 1) {                     //check box fields
        for (auto j = 1; j < 4; j++) {  //1 to 3
          if (bayerFields[j]->isFieldPressed()) {
            temp = i;  //current active field
            boxnum = j;
            for (auto n = 0; n < 6; n++)
              mainFields[n]->getFieldData(oldmainFieldData[n]);  //read current data from main fields
            for (auto n = 1; n < 4; n++)
              bayerFields[n]->getFieldData(_oldconsref[n - 1]);  //read current data from box fields
            rightBV.getFieldData(&oldminBV);                     //read current rightBV data
            goto activateBox;
          }  //if(bayerFields[j]->isFieldPressed())
        }    //for(boxfields)
      }      //if(i==1)
      if (mainFields[i]->isFieldPressed()) {
        temp = i;  //current field
        for (auto n = 0; n < 6; n++)
          mainFields[n]->getFieldData(oldmainFieldData[n]);  //read current data from main fields
        for (auto n = 1; n < 4; n++)
          bayerFields[n]->getFieldData(_oldconsref[n - 1]);  //read current data from box fields
        rightBV.getFieldData(&oldminBV);                     //read current rightBV data
activate:
        res = mainFields[temp]->activate();
        switch (res) {
          case -1:  //up
            if (temp)
              temp--;
            else
              temp = 5;
            goto activate;
          case 1:  //down
            (++temp) %= 6;
            goto activate;
          case 2:  //right
            boxnum = 1;
activateBox:
            res = bayerFields[boxnum]->activate();
            //debugln(res, DEC);
            switch (res) {
              case -1:  //up
                if (temp)
                  temp--;
                else
                  temp = 5;
                goto activate;
              case 1:  //down
                (++temp) %= 6;
                goto activate;
              case -2:  //left
                if (boxnum)
                  boxnum--;
                else
                  boxnum = 3;
                goto activateBox;
              case 2:  //right
                (++boxnum) %= 4;
                goto activateBox;
              case 0:
                break;
            }  //switch(res)
          case 0:
            break;
        }  //switch(res)
        mustUpdate = false;
        for (auto m = 0; m < 6; m++) {
          mainFields[m]->getFieldData(mainFieldData[m]);
          mustUpdate = mustUpdate || (m == 5 ? maxBV != oldmaxBV : strcmp(mainFieldData[m], oldmainFieldData[m]));
        }
        debugln(F("Getting main field data completed"));
        for (auto m = 1; m < 4; m++) {
          bayerFields[m]->getFieldData(_consref[m - 1]);
          mustUpdate = mustUpdate || (_oldcons[m - 1] != _cons[m - 1]);
        }
        rightBV.getFieldData(&minBV);
      }  //if(mainFields[i]->isFieldPressed)
    }    //for(main fields)
    if (rightBV.isFieldPressed()) {
      rightBV.activate();
      rightBV.getFieldData(&minBV);
      mustUpdate = mustUpdate || (minBV != oldminBV);
    }
    //---update data if necessary----
    if (mustUpdate) {
      debugln(F("Updating..."));
      debug(F("Searching for\tName:\t\t"));
      debugln(_name);
      debug(F("\t\tBayer:\t\t"));
      debugln(_bayer);
      debug(F("\t\tConstellation:\t"));
      debugln(_constell);
      debug(F("\t\tMagnitude Range:\t"));
      debug(atof(_minMag));
      debug(F(" to "));
      debugln(atof(_maxMag));
      debug(F("\t\tB-V range:\t"));
      debug(minBV);
      debug(F(" to "));
      debugln(maxBV);

      //-----search for the star-----
      selectSPI(SD_CS);
      bestMatch = 0;
      float minMag = atof(_minMag), maxMag = atof(_maxMag);
      bool ignoreMag = (minMag == 0 && maxMag == 0);

      //TODO: save new catalog files to the sd card
      for (uint16_t i = 0; i < CATALOGSIZE; ++i) {
#ifdef DEBUG
        if (i % 50 == 0)
          debug(F("."));
#endif
        if ((ignoreMag || (mag[i] >= minMag && mag[i] <= maxMag)) && b_v[i] >= minBV && b_v[i] <= maxBV) {
          match = strbgn(name[i], _name);
          match += !strcmp(bayer[i], _bayer);
          //          if (!strcmp(bayer[i], _bayer))
          //            ++match;
          match += strbgn(cons[i], _cons);
          match += strbgn(constell[i], _constell);
          int16_t diff = match - bestMatch;
          bestMatch = match - ((diff >> 15) & diff);  //if diff<0 then diff>>15=0xffff so bestMatch is unchanged otherwise bestMatch=match
          bestMatchIndex = i * (bestMatch <= match) + bestMatchIndex * (bestMatch > match);
        }
      }
      //update button
      strcpy(_btnTxt, "Point ");
      if (strcmp(name[bestMatchIndex], "                 "))  // name is not empty
        strcat(_btnTxt, name[bestMatchIndex]);
      else {
        debugln(cons[bestMatchIndex]);
        strcat(_btnTxt, toGreek(bayer[bestMatchIndex]));
        strcat(_btnTxt, toIAU(cons[bestMatchIndex]));
      }
      selectSPI(tft_CS);
      tft.fillRect(30, 150, 260, 24, BLACK);
      u8g2.setFont(SIZE2);
      u8g2.setForegroundColor(WHITE);
      printText(160 - strlen(_btnTxt) * 5, 170, _btnTxt);
      debugln(F("Update finished"));
      debug(F("_btnTxt: "));
      debugln(_btnTxt);
      debug(F("bestMatchIndex: "));
      debugln(bestMatchIndex, DEC);
      selectSPI(SD_CS);
      debugln(/*toGreek*/ (bayer[bestMatchIndex]));
    }
  }
  HMS ra(RAh[bestMatchIndex], RAm[bestMatchIndex], RAs[bestMatchIndex]);
  int8_t _sign = decsign[bestMatchIndex] == '+' ? 1 : -1;
  dms dec(_sign * decdeg[bestMatchIndex], _sign * decarcm[bestMatchIndex], _sign * decarcs[bestMatchIndex]);
  starptr.pointTo(ra, dec);
  selectSPI(tft_CS);
  tft.fillScreen(BLACK);
  u8g2.setFont(SIZE1);
  u8g2.setForegroundColor(RED);
  printText(16, 115, F("Press anywhere on the screen if you want to stop"));
  while (starptr.run()) {
    if (touch.scan(0)) {
      starptr.stop();
      tft.fillScreen(BLACK);
      u8g2.setFont(SIZE2);
      printText(90, 98, F("You cancelled"));
      tft.fillRoundRect(145, 123, 30, 20, 5, GREEN);
      u8g2.setBackgroundColor(GREEN);
      u8g2.setCursor(150, 140);
      u8g2.print(F("OK"));
      do {
        if (!touch.scan(0))
          continue;
      } while (touch.x < 145 || touch.x > 175 || touch.y < 123 || touch.y > 143);
      return;
    }
  }
  dispStarInfo(bestMatchIndex);
  selectSPI(tft_CS);
  tft.fillRoundRect(143, 170, 34, 21, 5, GREEN);
  u8g2.setFont(SIZE2);
  u8g2.setBackgroundColor(GREEN);
  u8g2.setCursor(150, 186);
  u8g2.print(F("OK"));
  do {
    if (!touch.scan(0))
      continue;
  } while (touch.x < 143 || touch.x > 177 || touch.y < 170 || touch.y > 191);
}

int16_t strbgn(char* const& str1, char* const& str2) {
  int16_t count = 0;
  for (char *p1 = str1, *p2 = str2; *p1 && *p2; ++p1, ++p2) {
    if (*p1 != *p2) return count;
    ++count;
  }
  return count;
}
//-----------------------------------------------------end------------------------------------------------------------