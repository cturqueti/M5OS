#ifndef CALCULADORA_H
#define CALCULADORA_H

#include "../../App.h"

class Calculadora : public App {
   public:
    Calculadora();
    virtual ~Calculadora();

    void tick() override;
    void draw() override;
};

#endif  // CALCULADORA_H
