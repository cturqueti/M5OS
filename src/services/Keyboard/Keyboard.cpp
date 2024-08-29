#include "Keyboard.h"

#include "esp_log.h"

const char* Keyboard::TAG = "Keyboard";

Keyboard::Keyboard() : lastMillis(millis()) {
    for (auto i : output_list) {
        gpio_reset_pin((gpio_num_t)i);
        gpio_set_direction((gpio_num_t)i, GPIO_MODE_OUTPUT);
        gpio_set_pull_mode((gpio_num_t)i, GPIO_PULLUP_PULLDOWN);
        digitalWrite(i, 0);
    }

    for (auto i : input_list) {
        gpio_reset_pin((gpio_num_t)i);
        gpio_set_direction((gpio_num_t)i, GPIO_MODE_INPUT);
        gpio_set_pull_mode((gpio_num_t)i, GPIO_PULLUP_ONLY);
    }
    _keys_state_buffer.reset();
    _set_output(output_list, 0);
}

Keyboard::~Keyboard() {
}

void Keyboard::onServiceOpen() {
    // if (xSemaphoreTake(ServicesManager::getInstance().onServiceOpenSemaphore, portMAX_DELAY) == pdTRUE) {
    //     xSemaphoreGive(ServicesManager::getInstance().onServiceOpenSemaphore);
    // }
    // tempCanvas.createSprite(30, 13);
    // tempCanvas.fillSprite(TFT_GREEN);
}

void Keyboard::onServiceTick() {
    updateKeyList();
    updateKeysState();
    if (isChange()) {
        if (isKeyPressed("UP")) {
            ESP_LOGI(TAG, "Pressionado Up");
        }
    }
}

void Keyboard::onServiceClose() {
    // Limpa a tela ao fechar
    // M5Cardputer.Display.clear();
}

void Keyboard::draw() {
    // tempCanvas.fillSprite(TFT_GREEN);
    // tempCanvas.fillCircle(7, 7, 5, TFT_ORANGE);
}

// ------- Public Keyboard ------ //

std::string Keyboard::getKey(Point2D_t keyCoor) {
    std::string ret = "";

    if ((keyCoor.x < 0) || (keyCoor.y < 0)) {
        return 0;
    }

    // Obtém a string da chave
    std::string keyValue = getKeyValue(keyCoor);

    // Verifica se a string não está vazia e pega o primeiro caractere
    if (!keyValue.empty()) {
        ret = keyValue;
    }

    return ret;
}

void Keyboard::updateKeyList() {
    _key_list_buffer.clear();
    Point2D_t coor;
    uint8_t input_value = 0;

    for (int i = 0; i < 8; i++) {
        _set_output(output_list, i);
        input_value = _get_input(input_list);
        /* If key pressed */

        if (input_value != 0) {
            /* Get X */
            // ESP_LOGI(TAG, "Pressionado algo");
            for (int j = 0; j < 7; j++) {
                if (input_value & (0x01 << j)) {
                    coor.x = (i > 3) ? X_map_chart[j].x_1 : X_map_chart[j].x_2;

                    /* Get Y */
                    coor.y = (i > 3) ? (i - 4) : i;
                    vTaskDelay(pdMS_TO_TICKS(50));

                    /* Keep the same as picture */
                    coor.y = -coor.y;
                    coor.y = coor.y + 3;

                    _key_list_buffer.push_back(coor);
                    // ESP_LOGI(TAG, "%d,%d\t valor %s", coor.x, coor.y, getKeyValue(coor).c_str());
                }
            }
        }
    }
}

std::string Keyboard::getKeyValue(const Point2D_t& keyCoor) {
    if (_keys_state_buffer.caps || _keys_state_buffer.shift) {
        return keymap_shift[keyCoor.y][keyCoor.x];
    } else if (_keys_state_buffer.fn || _keys_state_buffer.fnHold) {
        return keymap_fn[keyCoor.y][keyCoor.x];
    } else {
        return keymap_normal[keyCoor.y][keyCoor.x];
    }
}

uint8_t Keyboard::isPressed() {
    return _key_list_buffer.size();
}

bool Keyboard::isChange() {
    if (_last_key_size != _key_list_buffer.size()) {
        _last_key_size = _key_list_buffer.size();
        return true;
    } else {
        return false;
    }
}

bool Keyboard::isKeyPressed(std::string c) {
    if (_key_list_buffer.size()) {
        for (const auto& i : _key_list_buffer) {
            if (getKey(i) == c) return true;
        }
    }
    return false;
}

void Keyboard::updateKeysState() {
    _keys_state_buffer.reset();
    _key_pos_print_keys.clear();
    _key_pos_hid_keys.clear();
    _key_pos_modifier_keys.clear();

    unsigned long currentMillis = millis();

    for (auto& i : _key_list_buffer) {
        std::string key = getKeyValue(i);  // Armazena o valor da tecla

        if (key == "FN") {
            _keys_state_buffer.caps = false;
            if (!_fnPressedLast) {
                // Primeira pressão do FN
                _fnPressTime = currentMillis;
                _fnPressedLast = true;
            } else if (currentMillis - _fnPressTime <= _doubleClickThreshold) {
                // Pressão dupla do FN
                _keys_state_buffer.fnHold = !_keys_state_buffer.fnHold;
                ESP_LOGI(TAG, "FN: %d", _keys_state_buffer.fnHold);
                _fnPressedLast = false;
            }
            continue;
        }
        if (key == "SHIFT") {
            _keys_state_buffer.fnHold = false;
            if (!_shiftPressedLast) {
                // Primeira pressão do SHIFT
                _shiftPressTime = currentMillis;
                _shiftPressedLast = true;
            } else if (currentMillis - _shiftPressTime <= _doubleClickThreshold) {
                // Pressão dupla do SHIFT
                _keys_state_buffer.caps = !_keys_state_buffer.caps;
                _shiftPressedLast = false;
            }
            continue;
        }
        if (currentMillis - _fnPressTime > _doubleClickThreshold) {
            _fnPressedLast = false;
        }

        if (currentMillis - _shiftPressTime > _doubleClickThreshold) {
            _shiftPressedLast = false;
        }
        if (key == "OPT") {
            _keys_state_buffer.opt = true;
            continue;
        }
        if (key == "CTRL") {
            _keys_state_buffer.ctrl = true;
            _key_pos_modifier_keys.push_back(i);
            continue;
        }
        if (key == "ALT") {
            _keys_state_buffer.alt = true;
            _key_pos_modifier_keys.push_back(i);
            continue;
        }
        if (key == "TAB") {
            _keys_state_buffer.tab = true;
            _key_pos_hid_keys.push_back(i);
            continue;
        }
        if (key == "BSPC") {
            _keys_state_buffer.del = true;
            _key_pos_hid_keys.push_back(i);
            continue;
        }
        if (key == "ENT") {
            _keys_state_buffer.enter = true;
            _key_pos_hid_keys.push_back(i);
            continue;
        }
        if (key == "SPC") {
            _keys_state_buffer.space = true;
        }
    }

    for (auto& i : _key_pos_modifier_keys) {
        std::string key = getKeyValue(i);
        _keys_state_buffer.modifier_keys.push_back(key);
    }

    for (auto& i : _key_pos_hid_keys) {
        std::string k = getKeyValue(i);
        if (k == "TAB" || k == "BSPC" || k == "ENT") {
            _keys_state_buffer.hid_keys.push_back(k);
        }
    }

    for (auto& i : _key_pos_print_keys) {
        _keys_state_buffer.word.push_back(getKeyValue(i));
    }
    if (millis() - 1000 > lastMillis) {
        lastMillis = millis();
    }
}

// ------------ Private ----------//

void Keyboard::setShiftPressed(bool pressed) {
    _keys_state_buffer.caps = pressed;
}

void Keyboard::setFnPressed(bool pressed) {
    _keys_state_buffer.fn = pressed;
}

void Keyboard::_set_output(const std::vector<int>& pinList, uint8_t output) {
    output = output & 0B00000111;

    digitalWrite(pinList[0], (output & 0B00000001));
    digitalWrite(pinList[1], (output & 0B00000010));
    digitalWrite(pinList[2], (output & 0B00000100));
}

uint8_t Keyboard::_get_input(const std::vector<int>& pinList) {
    uint8_t buffer = 0x00;
    uint8_t pin_value = 0x00;

    for (int i = 0; i < 7; i++) {
        pin_value = (digitalRead(pinList[i]) == 1) ? 0x00 : 0x01;
        pin_value = pin_value << i;
        buffer = buffer | pin_value;
    }

    return buffer;
}

std::string Keyboard::vectorToString(const std::vector<std::string>& vec) {
    std::string result;
    for (const auto& str : vec) {
        if (!result.empty()) {
            result += ", ";
        }
        result += str;
    }
    return result;
}