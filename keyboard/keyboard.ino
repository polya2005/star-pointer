//#define RADIOPIN 13
//#include <RH_ASK.h>
//#include <SPI.h>

const byte kbChar[5][11] = {
  {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '1'},
  {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\'', '4'},
  {'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '7'},
  {'+', '-', ' ', ' ', ' ', ' ', ' ', '\b', '\n', '\n',0},
  {'2', '3', '6', '5', '8', '9', '0', '^', '_', '>', '<'}
};

const uint8_t outPin[5] = {2, 3, 4, 5, 6};
const uint8_t inPin[11] = {7, 8, 9, 10, 11, 12, A0, A1, A2, A3, A4};

//RH_ASK sender = RH_ASK(2000, A5, RADIOPIN, A6, false);

void setup() {
  Serial.begin(300);
  // put your setup code here, to run once:
  for (auto r = 0; r < 5; ++r) {
    pinMode(outPin[r], OUTPUT);
  }
  for (auto c = 0; c < 11; ++c) {
    pinMode(inPin[c], INPUT_PULLUP);
  }
//  while (!sender.init())Serial.print(F("."));
}

byte b = 0;
void loop() {
  // put your main code here, to run repeatedly:
  for (auto r = 0; r < 5; ++r) {
    digitalWrite(outPin[r], HIGH);
    for (auto c = 0; c < 11; ++c) {
      if (digitalRead(inPin[c]) == LOW) {
        b = kbChar[r][c];
        if (b) {
          Serial.write((char)b);
//          sender.send((const uint8_t*)&b, 1);
//          sender.waitPacketSent();
          delay(100);
          while (digitalRead(inPin[c]) == LOW);
        }
      }
    }
    digitalWrite(outPin[r], LOW);
  }
}
