#ifndef SETTINGS_H
#define SETTINGS_H

#include "../../App.h"

class Settings : public App {
   public:
    Settings();
    virtual ~Settings();

    void tick() override;
    void draw() override;
};

#endif  // SETTINGS_H
