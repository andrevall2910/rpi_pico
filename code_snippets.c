/*
* code snippets for the rpi pico 1 and 2
* uses the vs code extention to generate / pull code from
* dumped here for more centeralized repo
*/


/* 
* uart code (some in cmake files and some in main)
*/

// cmake uart
pico_enable_stdio_usb($name_of_project 1) // 1 likely means enable

//main.c
// includes
#include "hardware/uart.h"


//defines
#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 8 
#define UART_RX_PIN 9

// in main function

stdio_init_all();
uart_init(UART_ID, BAUD_RATE);
gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);    


// now you can call printf directly
// (lets put some printf examples in too)
printf("testing \n");

//here are some values in c we can print a certain amount of decimal points by using a .# on the %f symbol like so $.2f only 2 decimal print whole number
float pi = 3.1419;
int number = 12345;
printf("hello world %d, %f.4 \n", number, pi);

// ===================================================================