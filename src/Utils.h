#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>

#include <vector>

#define KEY_UP ';'
#define KEY_DOWN '.'
#define KEY_LEFT ','
#define KEY_RIGHT '/'

struct TaskInfo {
    std::string appName;
    UBaseType_t priority;
    uint8_t coreId;
};

struct LCDPin {
    const uint8_t data = 35;
    const uint8_t sck = 36;
    const uint8_t rs = 34;
    const uint8_t rst = 33;
    const uint8_t cs = 37;
    const uint8_t bl = 38;
};

struct SDCardPin {
    const int sck = 40;
    const int miso = 39;
    const int mosi = 14;
    const int cs = 12;
};

class Utils {
   public:
    // initialize the canvas
    // static void initCanvas();

   private:
};

#endif  // UTILS_H