#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Definições de pinos
#define LED1_PIN 25
#define LED2_PIN 14
#define LED3_PIN 13
#define BUTTON_PIN 15

// Estados do ciclo de LEDs
volatile int led_state = 0;

// Função de callback para o temporizador
int64_t alarm_callback(alarm_id_t id, void *user_data) {
    led_state++; // Avança o estado

    if (led_state == 1) {
        gpio_put(LED1_PIN, 1);
        gpio_put(LED2_PIN, 1);
        gpio_put(LED3_PIN, 0); // Apaga o LED3
        add_alarm_in_ms(1000, alarm_callback, NULL, false); // Agenda próxima mudança
    } else if (led_state == 2) {
        gpio_put(LED1_PIN, 1);
        gpio_put(LED2_PIN, 0);
        gpio_put(LED3_PIN, 0); // Apenas o LED1 permanece aceso
    }
    return 0; // Não repete o alarme automaticamente
}

// Função principal
int main() {
    stdio_init_all();

    // Inicialização dos LEDs
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_init(LED3_PIN);
    gpio_set_dir(LED3_PIN, GPIO_OUT);

    // Inicialização do botão
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Pull-up interno

    while (true) {
        if (gpio_get(BUTTON_PIN) == 0) { // se o Botão for pressionado
            sleep_ms(50); // rotina Debounce
            if (gpio_get(BUTTON_PIN) == 0) {
                // Acende todos os LEDs
                gpio_put(LED1_PIN, 1);
                gpio_put(LED2_PIN, 1);
                gpio_put(LED3_PIN, 1);

                led_state = 0; // Reseta o estado
                add_alarm_in_ms(1000, alarm_callback, NULL, false); // Inicia temporização

                while (gpio_get(BUTTON_PIN) == 0); // Espera o botão ser solto
            }
        }
    }
}
