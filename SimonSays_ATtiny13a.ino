/*

 @file SimonSays_ATtiny13a.ino
 
 Memo:
 Arduino IDE 1.0.6 + core13_022_arduino_1_6.zip
 true is defined as 1.
 false is defined as 0 (zero). 
 
 Multiple switch connections using the analog inputs .  Pin ・・・PB2(A1)
 
 VCC  ---+
 R1 10K
 A1   ---+--------+-----SW---+
 |        R2  1.1k   |
 +--------+-----SW---+
 |        R3  2.4k   |
 +--------+-----SW---+
 |        R4  8.2k   |
 +--------+-----SW---+
 |
 GND  -----------------------+
 
 GREEN  LED                                             Pin ・・・PB3
 BLUE   LED                                             Pin ・・・PB4
 YELLOW LED                                             Pin ・・・PB1
 RED    LED                                             Pin ・・・PB0
 
 @brief Simon Says (ATtiny13a)
 @author Kei Takagi
 @date 2016.8.17
 
 Copyright (c) 2016 Kei Takagi
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 
 */

#define  STAGEMAX 12
#define  WAIT   STAGEMAX * 2000

const byte pins[]  = {
  PB3, PB4, PB1, PB0
};
byte StageBuf[STAGEMAX];
byte Stage = 0;
byte Step = 0;
unsigned long t = 0;

void setup() {
  short i;
  boolean flg=true;
  for (i = 0; i < 4; i++)pinMode(pins[i], OUTPUT);
  while (true) {
    LedAnime(i++&0x0001);
    if ( analogRead(A1) < 1000 )break;
  };
  NextStage();
}

void LED(short i, int wait) {
  digitalWrite(pins[i], HIGH);
  delay(wait);
  digitalWrite(pins[i], LOW);
  delay(100);
}

void LED_ALL(boolean flg) {
  for (short i = 0; i < 4; i++)digitalWrite(pins[i], flg);
}

void loop() {
  short Btn = 9;
  if ( millis()  > t ) {
    delay(1000);
    ((void (*)())0x00)();// fail:Softwear Reset
  }
  else {
    int aout = analogRead(A1);
    if ( aout < 50 )Btn = 0;      //GREEN
    else if ( aout < 150 )Btn = 1;//BLUE
    else if ( aout < 300 )Btn = 2;//YELLOW
    else if ( aout < 600 )Btn = 3;//RED

    if ( Btn != 9 ) {
      LED(Btn, 400);
      if (Btn == StageBuf[Step])
      {
        Step++;
        if (Step == Stage)NextStage();
      }
      else {
        // fail
        for (short i = 0; i < 8; i++) {
          LED_ALL(i & 0x0001);
          LED(Btn, 300);
        }
        t = 0;
      }
    }
  }
}

void LedAnime(boolean i)
{
  digitalWrite(pins[i], HIGH);
  digitalWrite(pins[i + 2], HIGH);
  delay(150);
  digitalWrite(pins[i], LOW);
  digitalWrite(pins[i + 2], LOW);
}

void NextStage()
{
  short i;
  Step = 0;
  if (Stage < STAGEMAX)
  {
    //Next Stage
    t = millis() + WAIT;
    StageBuf[Stage] = t % 4;
    Stage++;
    delay(500);
    for (i = 0; i < Stage; i++)LED(StageBuf[i], 500);
  }
  else {
    //Winner
    for (i = 0; i < 18; i++) {
      LED_ALL(true);
      delay(150);
      LED_ALL(false);
      LedAnime(i&0x0001);
    }
    ((void (*)())0x00)();// Softwear Reset
  }
}


