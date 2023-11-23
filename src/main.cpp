#include <M5Atom.h>
#include <WiFi.h>
#include <time.h>
#include "ATOM_PRINTER.h"
#include "header_2.h"
#include "novel_1_2.h"
#include "novel_2_2.h"
#include "novel_3_2.h"
#include "novel_4_2.h"
#include "novel_5_2.h"
#include "receipt_10_2.h"
#include "receipt_100_2.h"
#include "pattern_2.h"
#include <driver/adc.h>

#define RED_BUTTON 26
#define BLUE_BUTTON 32

CRGB dispColor(uint8_t r, uint8_t g, uint8_t b)
{
  return (CRGB)((r << 16) | (g << 8) | b);
}

const char *NTPSRV = "ntp.jst.mfeed.ad.jp";
const long GMT_OFFSET = 9 * 3600;
const int DAYLIGHT_OFFSET = 0;
struct tm timeInfo;
char time_string_buffer[60];

bool time_sync()
{
  configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTPSRV); // NTPサーバと同期

  if (getLocalTime(&timeInfo))
  { // timeinfoに現在時刻を格納

    sprintf(time_string_buffer, "          %04d-%02d-%02d %02d:%02d:%02d\n", // 表示内容の編集
            timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
            timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    return true;
  }
  else
  {
    M5.dis.drawpix(0, dispColor(255, 0, 0));
    return false;
  }
}

ATOM_PRINTER printer;

void setup()
{
  M5.begin(true, false, true);
  // https://www.mgo-tec.com/blog-entry-trouble-shooting-esp32-wroom.html/7
  // https://twitter.com/mongonta555/status/1404304353884663813
  // どうやらWiFi.begin()を使うと、M5StackをはじめとしたM5シリーズで時々ボタンをオンにする誤判定が起こるようだ。
  // とりあえず最終盤ではM5Atom liteのボタンは使わないが、使うときは
  // <driver/adc.h>をincludeして、以下の関数を呼び出すと良いらしい。
  adc_power_acquire();
  pinMode(RED_BUTTON, INPUT);
  pinMode(BLUE_BUTTON, INPUT);
  WiFi.begin("Tannakaken's iPhone", "7v527wj65fkvs");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  printer.begin();
  M5.dis.drawpix(0, 0x00ffff);
  printer.init();
  M5.dis.drawpix(0, dispColor(0, 255, 0));
  randomSeed(analogRead(0));
}

bool red_button_pressing = false;
bool redButtonReleased()
{
  int current = digitalRead(RED_BUTTON);
  if (!red_button_pressing && current == 1)
  {
    red_button_pressing = true;
    return false;
  }
  else if (red_button_pressing && current == 0)
  {
    red_button_pressing = false;
    return true;
  }
  return false;
}
bool blue_button_pressing = false;
bool blueButtonReleased()
{
  int current = digitalRead(BLUE_BUTTON);
  if (!blue_button_pressing && current == 1)
  {
    blue_button_pressing = true;
    return false;
  }
  else if (blue_button_pressing && current == 0)
  {
    blue_button_pressing = false;
    return true;
  }
  return false;
}

void loop()
{
  bool print10 = redButtonReleased();
  bool print100 = blueButtonReleased();
  if (print10 || print100)
  {
    if (time_sync())
    {
      printer.init();
      printer.printZippedBMP(0, headerWidth, headerHeight, header);
      printer.printASCII(time_string_buffer);
      int randNumber = random(1, 5);
      switch (randNumber)
      {
      case 1:
        printer.printZippedBMP(0, novel1Width, novel1Height, novel1);
        break;
      case 2:
        printer.printZippedBMP(0, novel2Width, novel2Height, novel2);
        break;
      case 3:
        printer.printZippedBMP(0, novel3Width, novel3Height, novel3);
        break;
      case 4:
        printer.printZippedBMP(0, novel4Width, novel4Height, novel4);
        break;
      case 5:
        printer.printZippedBMP(0, novel5Width, novel5Height, novel5);
        break;
      }
      if (print10)
      {
        printer.printZippedBMP(0, receipt10Width, receipt10Height, receipt10);
      }
      else if (print100)
      {
        printer.printZippedBMP(0, receipt100Width, receipt100Height, receipt100);
      }

      printer.newLine(3);
    }
  }
  if (M5.Btn.wasPressed())
  {
    printer.printQRCode("https://ar-kumajirou.vercel.app/");
    printer.printZippedBMP(0, patternWidth, patternHeight, pattern);
    printer.newLine(3);
  }
  M5.update();
}