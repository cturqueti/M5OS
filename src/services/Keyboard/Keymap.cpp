#include "keymap.h"

const std::string keymap_normal[4][14] = {
    {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "_", "=", "BSPC"},
    {"TAB", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\\"},
    {"FN", "SHIFT", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "ENT"},
    {"CTRL", "OPT", "ALT", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "SPC"}};

const std::string keymap_shift[4][14] = {
    {"~", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "-", "+", "BSPC"},
    {"TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}", "|"},
    {"FN", "SHIFT", "A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"", "ENT"},
    {"CTRL", "OPT", "ALT", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", "SPC"}};

const std::string keymap_fn[4][14] = {
    {"ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "_", "=", "DEL"},
    {"TAB", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\\"},
    {"FN", "SHIFT", "a", "s", "d", "f", "g", "h", "j", "k", "l", "UP", "'", "ENT"},
    {"CTRL", "OPT", "ALT", "z", "x", "c", "v", "b", "n", "m", "LEFT", "DOWN", "RIGHT", "SPC"}};

bool keyState[rows][cols];

Chart_t X_map_chart[7] = {{1, 0, 1}, {2, 2, 3}, {4, 4, 5}, {8, 6, 7}, {16, 8, 9}, {32, 10, 11}, {64, 12, 13}};
