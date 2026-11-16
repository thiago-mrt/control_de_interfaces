cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    
    if (cyw43_arch_wifi_connect_timeout_ms("Nombre de wifi", "contraseña", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }

    // Configurar Servidor TCP en puerto 80
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) return 1;

    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) return 1;

    pcb = tcp_listen(pcb);
    tcp_accept(pcb, server_accept);
    printf("Servidor web iniciado en el puerto 80.\n");

    // Bucle principal seguro
    while (true) {
        cyw43_arch_poll(); // Mantiene vivo el stack de red

        // Procesamos los comandos fuera de las interrupciones de red usando ifs
        if (current_cmd != CMD_NONE) {
            if (current_cmd == CMD_W) {
                gpio_put(MOTOR1_IN1, 1); gpio_put(MOTOR1_IN2, 0);
                gpio_put(MOTOR2_IN1, 1); gpio_put(MOTOR2_IN2, 0);
                printf("Motores: Adelante (W)\n");
            }
            else if (current_cmd == CMD_S) {
                gpio_put(MOTOR1_IN1, 0); gpio_put(MOTOR1_IN2, 1);
                gpio_put(MOTOR2_IN1, 0); gpio_put(MOTOR2_IN2, 1);
                printf("Motores: Atrás (S)\n");
            }
            else if (current_cmd == CMD_A) {
                gpio_put(MOTOR1_IN1, 0); gpio_put(MOTOR1_IN2, 1);
                gpio_put(MOTOR2_IN1, 1); gpio_put(MOTOR2_IN2, 0);
                printf("Motores: Izquierda (A)\n");
            }
            else if (current_cmd == CMD_D) {
                gpio_put(MOTOR1_IN1, 1); gpio_put(MOTOR1_IN2, 0);
                gpio_put(MOTOR2_IN1, 0); gpio_put(MOTOR2_IN2, 1);
                printf("Motores: Derecha (D)\n");
            }
            else if (current_cmd == CMD_STOP) {
                gpio_put(MOTOR1_IN1, 0); gpio_put(MOTOR1_IN2, 0);
                gpio_put(MOTOR2_IN1, 0); gpio_put(MOTOR2_IN2, 0);
                printf("Motores: Detenidos\n");
            }
            else if (current_cmd == CMD_SPACE) {
                led_state = !led_state;
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state ? 1 : 0);
                printf("LED Interno: %s\n", led_state ? "ENCENDIDO" : "APAGADO");
            }

            current_cmd = CMD_NONE; // Limpiar comando procesado
        }

        sleep_ms(10);
    }

    cyw43_arch_deinit();
    return 0;
}
