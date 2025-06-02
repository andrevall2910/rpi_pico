#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

// Perform initialisation
int pico_led_init(void) {

// this not just defines led pin but checks for pico/stdlib.h
// likely the extention picks different versions of the stdlib.h depending on pico selected during project creation
// note also PICO_DEFAULT_LED_PIN expands to (pin 25)
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
// #elif defined(CYW43_WL_GPIO_LED_PIN)
//     // For Pico W devices we need to initialise the driver etc
//     return cyw43_arch_init();
#endif
}



int main()
{
    bool led_state = true;
    pico_led_init();
    stdio_init_all();

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    while (true) {
        printf("Hello, world!\n");
        //this properly toggles an led not state = ~state or state ^= state
        led_state = !led_state;
        gpio_put(PICO_DEFAULT_LED_PIN, led_state);
        sleep_ms(1000);
    }
}
