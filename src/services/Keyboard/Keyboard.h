#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>

#include <string>
#include <vector>

#include "Keymap.h"
#include "Service.h"
#include "ServicesManager.h"

class Keyboard : public Service {
   public:
    Keyboard();
    ~Keyboard();

    virtual void onServiceOpen() override;
    virtual void onServiceTick() override;
    virtual void onServiceClose() override;
    virtual void draw() override;
    const uint8_t* getIcon() { return nullptr; }
    size_t getIconSize() { return 0; }

    std::string getKey(Point2D_t keyCoor);
    void updateKeyList();
    inline std::vector<Point2D_t>& keyList() { return _key_list_buffer; }

    std::string getKeyValue(const Point2D_t& keyCoor);

    uint8_t isPressed();
    bool isChange();
    bool isKeyPressed(std::string c);

    void updateKeysState();
    inline KeysState& keysState() { return _keys_state_buffer; }

   private:
    static const char* TAG;

    const std::vector<int> output_list = {8, 9, 11};
    const std::vector<int> input_list = {13, 15, 3, 4, 5, 6, 7};
    std::vector<Point2D_t> _key_list_buffer;
    std::vector<Point2D_t> _key_pos_modifier_keys;  // modifier key: eg  ctrl, alt
    std::vector<Point2D_t> _key_pos_hid_keys;       // print key + space, enter, del
    std::vector<Point2D_t> _key_pos_print_keys;     // only text: eg A,B,C

    KeysState _keys_state_buffer;
    uint8_t _last_key_size;

    unsigned long lastMillis;
    bool _fnPressedLast = false;
    bool _shiftPressedLast = false;
    unsigned long _fnPressTime = 0;
    unsigned long _shiftPressTime = 0;
    const unsigned long _doubleClickThreshold = 1000;

    void setShiftPressed(bool pressed);
    void setFnPressed(bool pressed);

    void _set_output(const std::vector<int>& pinList, uint8_t output);
    uint8_t _get_input(const std::vector<int>& pinList);
    std::string vectorToString(const std::vector<std::string>& vec);
};

#endif  // KEYBOARD_H
