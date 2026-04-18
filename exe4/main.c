/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

const int LED_PIN_Y = 10;
const int LED_PIN_B = 14;

const int BTN_PIN_Y = 26;
const int BTN_PIN_B = 19;

QueueHandle_t xQueueInput;
QueueHandle_t xQueueValue;
SemaphoreHandle_t xSemaphoreLedY;
SemaphoreHandle_t xSemaphoreLedB;

/**
 * NÃO MEXER!
 */
void input_task(void* p) {

    vTaskDelay(pdMS_TO_TICKS(350));
    int test_case;

    test_case = 5;
    xQueueSend(xQueueInput, &test_case, 0);

    test_case = 2;
    xQueueSend(xQueueInput, &test_case, 0);

    while (true) {
        vTaskDelay(portMAX_DELAY);
    }
}

/**
 * Seu código vem aqui!
*/

void btn_callback(uint gpio, uint32_t events) {
    printf("oi\n");
    if (events == GPIO_IRQ_EDGE_RISE) { // RISE edge
        if(gpio == BTN_PIN_B){
            printf("entrou2");
            xSemaphoreGiveFromISR(xSemaphoreLedB, 0);
        } else if(gpio == BTN_PIN_Y){
            xSemaphoreGiveFromISR(xSemaphoreLedY, 0);
        }
    }
}

void led_b_task(void *p) {
    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);
    int cont = 0;
    int delay = 400;
    int qt = xQueueReceive(xQueueInput, &delay, 0);
    if(xSemaphoreTake(xSemaphoreLedB, pdMS_TO_TICKS(500)) == pdTRUE){
        printf("ENTROU!");
        while (cont < qt) {
            gpio_put(LED_PIN_B, 1);
            vTaskDelay(pdMS_TO_TICKS(delay));
            gpio_put(LED_PIN_B, 0);
            vTaskDelay(pdMS_TO_TICKS(delay));
            cont++;
        }
    }
    xSemaphoreGive(xSemaphoreLedY);
}

// void led_y_task(void *p) {
//    gpio_init(LED_PIN_Y);
//    gpio_set_dir(LED_PIN_Y, GPIO_OUT);

//    int cont = 0;
//    int delay = 400;
//    int qt = xQueueReceive(xQueueInput, &delay, 0);


//     if(xSemaphoreTake(xSemaphoreLedY, pdMS_TO_TICKS(500)) == pdTRUE){
//         printf("ENTROU!");
//         while (cont < qt) {
//             gpio_put(LED_PIN_Y, 1);
//             vTaskDelay(pdMS_TO_TICKS(delay));
//             gpio_put(LED_PIN_Y, 0);
//             vTaskDelay(pdMS_TO_TICKS(delay));
//             cont++;
//         }
//     }
// }

int main() {
    stdio_init_all();

    /**
     * manter essas linhas
     */
    xQueueInput = xQueueCreate(32, sizeof(int));
    xTaskCreate(input_task, "Input", 256, NULL, 2, NULL);

    /**
     * Seu código vem aqui!
     */
        
    xSemaphoreLedB = xSemaphoreCreateBinary();
    xSemaphoreLedY = xSemaphoreCreateBinary();
 
     gpio_init(LED_PIN_Y);
     gpio_set_dir(LED_PIN_Y, GPIO_OUT);
 
     gpio_init(LED_PIN_B);
     gpio_set_dir(LED_PIN_B, GPIO_OUT);
 
     gpio_init(BTN_PIN_Y);
     gpio_set_dir(BTN_PIN_Y, GPIO_IN);
     gpio_pull_up(BTN_PIN_Y);
     gpio_set_irq_enabled_with_callback(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
 
     gpio_init(BTN_PIN_B);
     gpio_set_dir(BTN_PIN_B, GPIO_IN);
     gpio_pull_up(BTN_PIN_B);
     gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    xTaskCreate(led_b_task, "Led B", 256, NULL, 2, NULL);
    // xTaskCreate(led_y_task, "Led Y", 256, NULL, 2, NULL);
    vTaskStartScheduler();

    while (1) {}
		return 0;
}
