#include "Calculadora.h"

#include <M5Cardputer.h>

Calculadora::Calculadora() {
    // Inicialização do aplicativo
}

Calculadora::~Calculadora() {
    // Limpeza dos recursos
}

void Calculadora::tick() {
    // Lógica de atualização do aplicativo
}

void Calculadora::draw() {
    M5.Lcd.clear();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print("Calculadora");
    // Desenhe a interface da calculadora aqui
}
