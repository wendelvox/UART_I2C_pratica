/*
 * Por: Wilton Lacerda Silva
 *    Comunicação serial com I2C *  
 * Uso da interface I2C para comunicação com o Display OLED * 
 * Estudo da biblioteca ssd1306 com PicoW na Placa BitDogLab. *  
 * Este programa escreve uma mensagem no display OLED.
 * 
 * 
*/


#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include <stdio.h>

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define UART_ID uart0
#define UART_TX_PIN 0
#define UART_RX_PIN 1


typedef struct {
    ssd1306_t ssd;
} Display;

void init_uart() {
    uart_init(UART_ID, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_fifo_enabled(UART_ID, false);
}

void init_display(Display *display) {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&display->ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&display->ssd);
    ssd1306_fill(&display->ssd, false);
    ssd1306_send_data(&display->ssd);
}

void update_display(Display *display, char c) {
    ssd1306_fill(&display->ssd, false);
    char text[2] = {c, '\0'};
    ssd1306_draw_string(&display->ssd, text, 40, 30);
    ssd1306_send_data(&display->ssd);
}

int main() {
    stdio_init_all();
    Display display;
    init_uart();
    init_display(&display);
    
    while (true) {
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            update_display(&display, c);
            printf("Recebido: %c\n", c);
        }
    }
}