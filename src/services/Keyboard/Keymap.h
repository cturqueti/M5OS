#ifndef KEYMAP_H
#define KEYMAP_H

#include <string>
#include <vector>

struct Chart_t {
    uint8_t value;
    uint8_t x_1;
    uint8_t x_2;
};

struct Point2D_t {
    int x;
    int y;
};

struct KeysState {
    bool tab = false;
    bool fn = false;
    bool fnHold = false;
    bool shift = false;
    bool caps = false;
    bool ctrl = false;
    bool opt = false;
    bool alt = false;
    bool del = false;
    bool enter = false;
    bool space = false;
    uint8_t modifiers = 0;

    std::vector<std::string> word;
    std::vector<std::string> hid_keys;
    std::vector<std::string> modifier_keys;

    void reset() {
        tab = false;
        fn = false;
        shift = false;
        // caps = false;
        ctrl = false;
        opt = false;
        alt = false;
        del = false;
        enter = false;
        space = false;
        modifiers = 0;
        word.clear();
        hid_keys.clear();
        modifier_keys.clear();
    }
};

const int rows = 8;
const int cols = 7;

// Declaração externa das variáveis

extern const std::string keymap_normal[4][14];
extern const std::string keymap_shift[4][14];
extern const std::string keymap_fn[4][14];
extern bool keyState[rows][cols];
extern Chart_t X_map_chart[7];

#endif  // KEYMAP_H
