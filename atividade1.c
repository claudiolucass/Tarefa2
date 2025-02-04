#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial.
#include "hardware/timer.h" // Biblioteca para controle e configuração de temporizadores de hardware.
#include <stdio.h>  

#define LED_PIN_RED 12
 bool led_on = false;

// Definição do pino de controle do LED.
const uint LED_PIN = 12; // Constante para representar o pino GPIO 12 ao qual o LED está conectado.

// Função de callback que será chamada a cada intervalo definido pelo temporizador.

bool repeating_timer_callback(struct repeating_timer *t) {
    
    static bool led_on = false;

    // Alterna o estado do LED: se estava desligado, liga; se estava ligado, desliga.
    led_on = !led_on;

 
    gpio_put(LED_PIN, led_on);

    // Imprime na saída serial o estado atual do LED ("ligado" ou "desligado").
    printf("LED %s\n", led_on ? "ligado" : "desligado");

    // Retorna true para manter o temporizador repetindo esta função.
    return true;
}

int main() {

    stdio_init_all();

    // Inicializa o pino GPIO 12, preparando-o para ser usado como saída digital.
    gpio_init(LED_PIN);
    uint32_t interval = 1000; 
    get_absolute_time(); 

    gpio_set_dir(LED_PIN, GPIO_OUT);    // Configura a direção do pino 12 como saída.

    // Declara uma estrutura para armazenar informações sobre o temporizador repetitivo.
    struct repeating_timer timer;

    delayed_by_us(get_absolute_time(), interval * 3000); 
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

    
    absolute_time_t next_wake_time = delayed_by_us(get_absolute_time(), interval * 3000);

    // Loop principal do programa.
    while (true) {
    if (time_reached(next_wake_time)) {
   
    printf("passaram-se 3 segundos \n");
    
    led_on = !led_on;   //Liga ou desliga o led.
    gpio_put(LED_PIN_RED,led_on);
    
    next_wake_time = delayed_by_us(next_wake_time, interval * 1000);
    }

    sleep_ms(1);
    }

    return 0;
}
