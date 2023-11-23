#ifndef _ATOM_PRINTER_H
#define _ATOM_PRINTER_H

#include <Arduino.h>
#include "data.h"

typedef enum
{
    UPC_A = 0x41,
    UPC_E,
    JAN13_EAN13,
    JAN8_EAN8,
    CODE39,
    ITF,
    CODABAR,
    CODE93,
    CODE128
} BarCode_t;

typedef enum
{
    HIDE = 0x00,
    ABOVE,
    BELOW,
    BOTH
} BarCodePos_t;

typedef enum
{
    LEVEL_L = 0x48,
    LEVEL_M,
    LEVEL_Q,
    LEVEL_H
} QRCode_EC_Level_t;
// Error correction level

class ATOM_PRINTER
{
private:
    HardwareSerial *_serial;
    bool _debug;
    bool waitMsg(unsigned long timerout = 500);
    void sendCMD(uint8_t *data, size_t size);
    void cleanBuffer();
    uint8_t buffer[256] = {0};

public:
    void begin(HardwareSerial *serial = &Serial2, int baud = 9600,
               uint8_t RX = 33, uint8_t TX = 23, bool debug = false);
    void init();
    // void printPos(uint16_t posx);
    void fontSize(uint8_t font_size);
    void WriteCMD(uint8_t *buff, uint8_t buff_size);
    void newLine(uint8_t count);
    // void setBarCodeHRI(BarCodePos_t pos);
    // void enableBarCode(bool state);
    // void printBarCode(BarCode_t type, String barcode);
    // void setQRCodeECL(QRCode_EC_Level_t level);
    void printQRCode(String qrcode);
    void printASCII(String data);
    // void printBYTE(uint8_t *data, size_t size);
    void printBMP(uint8_t mode, uint16_t xdot, uint16_t ydot, uint8_t *buffer);
    /**
     * comporession.pyで圧縮したbmpデータを印刷する関数。
     */
    void printZippedBMP(uint8_t mode, uint16_t xdot, uint16_t ydot, Data *buffer);
};

#endif