#include "Service.h"

#include "esp_log.h"
static const char* TAG = "Services";

TaskHandle_t Service::taskHandle = nullptr;

// Construtor
Service::Service() : isRunning(false),
                     serviceName(""),
                     priority(99),
                     _start(false),
                     _open(false),
                     _close(false),
                     _pause(false) {}

// Destrutor
Service::~Service() {
}