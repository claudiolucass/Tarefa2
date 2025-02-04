#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "callback_temp.h"
#include "alarme.h"

#define LED_PIN 25
#define BUTTON_PIN 15


int64_t alarm_callback(alarm_id_t id, void *user_data) {
    gpio_put(LED_PIN, !gpio_get(LED_PIN)); // Alterna o estado do LED
    return 0; // Retorna 0 para que o alarme não se repita
}

// Função principal
int main() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Pull-up interno ativado

    while (true) {
        if (gpio_get(BUTTON_PIN) == 0) { 
            sleep_ms(50); 
            if (gpio_get(BUTTON_PIN) == 0) {
                add_alarm_in_ms(3000, alarm_callback, NULL, false); 
                while (gpio_get(BUTTON_PIN) == 0); 
            }
        }
    }
}
