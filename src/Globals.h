// Globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <semphr.h>

#include <vector>

#include "Utils.h"

extern SemaphoreHandle_t canvasSemaphore;
extern std::vector<TaskInfo> taskTable;

#endif  // GLOBALS_H
