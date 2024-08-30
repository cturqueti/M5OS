// Globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <M5GFX.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <semphr.h>

#include <vector>

#include "Utils.h"
// extern SemaphoreHandle_t openSemaphore;
// extern SemaphoreHandle_t serviceOpenSemaphore;

// bool takeAppOpenSemaphore(TickType_t ticksToWait);
// void giveAppOpenSemaphore();

// bool takeServiceOpenSemaphore(TickType_t ticksToWait);
// void giveServiceOpenSemaphore();

// extern SemaphoreHandle_t canvasSemaphore;
// extern M5GFX display;   // Declaração externa da variável global
// extern M5Canvas canva;  // Declaração externa da variável global

#endif  // GLOBALS_H
