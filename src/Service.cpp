#include "Service.h"

#include "esp_log.h"
static const char* TAG = "Services";

TaskHandle_t Service::taskHandle = nullptr;

// Construtor
Service::Service() : isRunning(false), serviceName(""), priority(99) {}

// Destrutor
Service::~Service() {
}