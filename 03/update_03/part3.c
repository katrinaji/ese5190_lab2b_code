#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "ws2812.pio.h"
#include "adafruit_qtpy_rp2040.h"

#define IS_RGBW true
#define NUM_PIXELS 150
#define QTPY_BOOT_PIN 21

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 12 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 12
#endif

#ifdef PICO_DEFAULT_WS2812_POWER_PIN
#define QTPY_BOOT_POWER_PIN PICO_DEFAULT_WS2812_POWER_PIN
#else
// default to pin 11 if the board doesn't have a power WS2812 pin defined
#define QTPY_BOOT_POWER_PIN 11
#endif

// Create an array to record the input
int record[100];
int counter = 0;

void main() {
    stdio_init_all();
    gpio_init(QTPY_BOOT_PIN);
    gpio_init(QTPY_BOOT_POWER_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    gpio_set_dir(QTPY_BOOT_POWER_PIN, GPIO_OUT);

    // From previous helloLED.c
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, PICO_DEFAULT_WS2812_PIN, 800000, IS_RGBW);

    //gpio_put function need a const uint input
    const uint gpio = QTPY_BOOT_POWER_PIN;
    //gpio_init(gpio);
    //gpio_set_dir(gpio, GPIO_OUT);
    
    while(stdio_usb_connected()!=true);
    printf("Press the button to start.\n");
    
    while(1){
        if (gpio_get(QTPY_BOOT_PIN) == 0){
            printf("You can press the button to create pattern now.\n");
            sleep_ms(50); // buffer for rebounce
            while(counter < 100){
                record[counter] = !(gpio_get(QTPY_BOOT_PIN)); // because pressing down is 0
                counter ++;
                sleep_ms(50); // buffer for rebounce
            }
            printf("The recording is done.\n");
            break;
        }
    }
    
    printf("Start to play the recorded pattern.\n");
        for(int idx = 0; idx < 100; idx ++){
            gpio_put(gpio, record[idx]);
            set_neopixel_color(0x00FF00);
            sleep_ms(50);
        }
    
    printf("Display ends.\n");

}                  
