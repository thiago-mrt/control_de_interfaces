#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

// motor a
#define IN1 2
#define IN2 3
#define ENA 6

// motor b
#define IN3 4
#define IN4 5
#define ENB 7

// configurar pwm
void configurarPWM(uint pin) {

    gpio_set_function(pin, GPIO_FUNC_PWM);

    uint slice = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, 4.0f);

    pwm_init(slice, &config, true);

    // 100% de pwm
    pwm_set_gpio_level(pin, 65535);
}

// adelante
void adelante() {

    pwm_set_gpio_level(ENA, 65535);
    pwm_set_gpio_level(ENB, 65535);

    gpio_put(IN1, 1);
    gpio_put(IN2, 0);

    gpio_put(IN3, 1);
    gpio_put(IN4, 0);

    printf("motor: adelante\n");
}

// atras
void atras() {

    pwm_set_gpio_level(ENA, 65535);
    pwm_set_gpio_level(ENB, 65535);

    gpio_put(IN1, 0);
    gpio_put(IN2, 1);

    gpio_put(IN3, 0);
    gpio_put(IN4, 1);

    printf("motor: atras\n");
}

// parar
void parar() {

    pwm_set_gpio_level(ENA, 0);
    pwm_set_gpio_level(ENB, 0);

    gpio_put(IN1, 0);
    gpio_put(IN2, 0);

    gpio_put(IN3, 0);
    gpio_put(IN4, 0);

    printf("motor: parado\n");
}

// inicializar motores
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

    parar();
}

// main
int main() {

    stdio_init_all();

    inicializarMotores();

    printf("\nautito iniciado\n");

    while (true) {

        printf("motor: adelante\n");
        adelante();
        sleep_ms(3000);

        printf("motor: parado\n");
        parar();
        sleep_ms(1000);

        printf("motor: atras\n");
        atras();
        sleep_ms(3000);

        printf("motor: parado\n");
        parar();
        sleep_ms(1000);
    }
}