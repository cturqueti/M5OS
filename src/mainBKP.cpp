// #include <Arduino.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/semphr.h>
// #include <freertos/task.h>

// #include <cstdio>
// #include <map>
// #include <string>

// #include "App.h"
// #include "apps/Calculadora/Calculadora.h"

// #define NUM_APPS 3
// #define TASK_STACK_SIZE 2048
// #define TASK_PRIORITY 1
// #define TASK_DELAY_MS 1000

// // Estrutura para armazenar informações sobre cada tarefa

// // Criação do mutex global
// SemaphoreHandle_t printMutex;
// std::map<String, App*> apps;

// App* getApp(const String& appName) {
//     auto it = apps.find(appName);
//     if (it != apps.end()) {
//         return it->second;
//     }
//     return nullptr;
// }

// // Função da tarefa, representando o aplicativo
// void appTask(void* pvParameters) {
//     TaskData* data = (TaskData*)pvParameters;
//     const std::string& appName = data->appName;
//     TickType_t delayTime = pdMS_TO_TICKS(TASK_DELAY_MS);

//     printf("Iniciando o aplicativo: %s\n", appName.c_str());

//     // Loop da tarefa
//     while (true) {
//         // Tenta obter o mutex
//         if (xSemaphoreTake(printMutex, portMAX_DELAY) == pdTRUE) {
//             // Se obteve o mutex, pode usar printf
//             printf("Aplicativo %s está rodando...\n", appName.c_str());

//             // Libera o mutex
//             xSemaphoreGive(printMutex);
//         }
//         vTaskDelay(delayTime);  // Delay de 1 segundo
//     }

//     // Nunca chega aqui
//     vTaskDelete(NULL);
//     vTaskDelay(delayTime);
//     printf("Aplicativo %s está parado...\n", appName.c_str());
// }

// void setup() {
//     Serial.begin(115200);
//     // Cria o mutex para a impressão
//     printMutex = xSemaphoreCreateMutex();
//     if (printMutex == NULL) {
//         // Se não conseguir criar o mutex, imprime um erro
//         Serial.println("Falha ao criar o mutex!");
//         while (true) {
//             delay(1000);
//         }  // Loop infinito para indicar falha
//     }

//     App* app = getApp("Calculadora");
//     delay(2000);

//     // Criação das tarefas
//     for (int i = 0; i < NUM_APPS; i++) {
//         // Usa std::string para o nome do aplicativo
//         appData[i].appName = "Calculadora";

//         appData[i].taskHandle = NULL;
//         printf("Nome do app: %s\n", appData[i].appName.c_str());

//         xTaskCreatePinnedToCore(
//             appTask,                     // Função da tarefa
//             appData[i].appName.c_str(),  // Nome da tarefa
//             TASK_STACK_SIZE,             // Tamanho da stack
//             app,                         // Dados da tarefa (parâmetro)
//             TASK_PRIORITY,               // Prioridade da tarefa
//             &appData[i].taskHandle,      // Handle da tarefa
//             i % 2                        // Núcleo em que executar (0 ou 1)
//         );
//         if (appData[i].taskHandle) {
//             vTaskDelay(pdMS_TO_TICKS(10));
//             printf("Task %s criada com sucesso, ponteiro: %p\n", appData[i].appName.c_str(), appData[i].taskHandle);
//         } else {
//             printf("Falha ao criar a task para %s\n", appData[i].appName.c_str());
//         }
//         delay(3000);
//     }

//     // Aguardar um tempo antes de fechar as tarefas
//     delay(5000);

//     // Fechar e deletar as tarefas
//     for (int i = 0; i < NUM_APPS; i++) {
//         if (appData[i].taskHandle != NULL) {
//             printf("Finalizando o aplicativo: %s\n", appData[i].appName.c_str());
//             vTaskDelete(appData[i].taskHandle);  // Deleta a tarefa
//         }
//         // std::string lida automaticamente da memória
//     }
// }

// void loop() {
//     // Loop vazio
// }
