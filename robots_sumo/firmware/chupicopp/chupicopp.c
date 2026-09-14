#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "stdio.h"

// ==========================
// MOTOR A
// ==========================
#define IN1 2
#define IN2 3
#define ENA 6

// ==========================
// MOTOR B
// ==========================
#define IN3 4
#define IN4 5
#define ENB 7

// ==========================
// CONFIGURAR PWM
// ==========================
void configurarPWM(uint pin) {

    gpio_set_function(pin, GPIO_FUNC_PWM);

    uint slice = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();

    // PWM
    pwm_config_set_clkdiv(&config, 4.0f);

    pwm_init(slice, &config, true);

    // 100% de PWM
    pwm_set_gpio_level(pin, 65535);
}

// ==========================
// ADELANTE
// ==========================
void adelante() {

    gpio_put(IN1, 1);
    gpio_put(IN2, 0);

    gpio_put(IN3, 1);
    gpio_put(IN4, 0);
}

// ==========================
// ATRÁS
// ==========================
void atras() {

    gpio_put(IN1, 0);
    gpio_put(IN2, 1);

    gpio_put(IN3, 0);
    gpio_put(IN4, 1);
}

// ==========================
// PARAR
// ==========================
void parar() {

    // PWM = 0
    pwm_set_gpio_level(ENA, 0);
    pwm_set_gpio_level(ENB, 0);

    gpio_put(IN1, 0);
    gpio_put(IN2, 0);

    gpio_put(IN3, 0);
    gpio_put(IN4, 0);
}

// ==========================
// INICIALIZAR MOTORES
// ==========================
void inicializarMotores() {

    gpio_init(IN1);
    gpio_init(IN2);
    gpio_init(IN3);
    gpio_init(IN4);

    gpio_set_dir(IN1, GPIO_OUT);
    gpio_set_dir(IN2, GPIO_OUT);
    gpio_set_dir(IN3, GPIO_OUT);
    gpio_set_dir(IN4, GPIO_OUT);

    configurarPWM(ENA);
    configurarPWM(ENB);
}

// ==========================
// MAIN
// ==========================
int main() {

    stdio_init_all();

    inicializarMotores();

    while (true) {

        adelante();

        sleep_ms(3000);

        parar();

        sleep_ms(1000);

        atras();

        sleep_ms(3000);

        parar();

        sleep_ms(1000);
    }
}