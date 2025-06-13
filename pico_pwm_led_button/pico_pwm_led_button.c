#include <stdio.h>

#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define LED_PIN 25
#define BUTTON_PIN 22
#define PWM_OUTPUT_PIN 21

#define DEBOUNCE_TIME_MS 250



// global (static) variable to hold last time the button was pressed
static uint64_t last_interrupt_time = 0;

void toggle_led()
{   
    // this is a blocking function very bad in irq / isr responses
    //sleep_ms(50);

    // this is better we make it record current time stamp and ensure we have elapsed the defined ms
    // if we havent then we return out of the function
    uint64_t now = time_us_64(); // current time in microseconds
    if (now - last_interrupt_time < DEBOUNCE_TIME_MS * 1000) {
        // too soon, ignore as bounce
        return;
    }

    last_interrupt_time = now;

    static bool led_state = true; // make it static we want it to stay the same between func calls
    led_state = !led_state; // toggle led
    gpio_put(25, led_state);
    
    printf("Button on 22 pressed! led_state: %d\n", led_state);
}


int main()
{

    uint slice = 0;
    uint channel = 0;
    stdio_init_all();

    //we are gonna use the on board LED (on pin 25)
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);


    //lets use gp22 as a button input (pull up)
    gpio_init(BUTTON_PIN);
    //gpio_set_function(22, GPIO_FUNC_SIO); // gpio init does this for us now i guess :) well as gpio_put
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    //gpio_pull_up(BUTTON_PIN); // if we do this its always 1 *facepalm*
    gpio_pull_down(BUTTON_PIN); // lets pull down the 3v3 here 


    //pwm output on pin gp21
    gpio_set_function(PWM_OUTPUT_PIN, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(PWM_OUTPUT_PIN);
    channel = pwm_gpio_to_channel(PWM_OUTPUT_PIN);

    //default clock is 125mhz lets get a 10khz output for now with a 65% duty cycle 
    //also note that gp21 is on slice 2 channel B
    // 12500 wrap value gets us 10khz period and if we count up to 8125 thats 65% duty
    pwm_set_wrap(slice, 12500); // (slice, warp_value)
    pwm_set_chan_level(slice, 1, 8125); // (slice, (0 for A || 1 for B), level_value) 
    pwm_set_enabled(slice, true); // enable (slice, enable bool)

    // we can read the gpio events such as high low / rising falling edge:
    // GPIO_IRQ_LEVEL_LOW
    // GPIO_IRQ_LEVEL_HIGH
    // GPIO_IRQ_EDGE_FALL
    // GPIO_IRQ_EDGE_RISE


    // enable interupts works like so (pin, event type, enable(?), &reference to another function )
    gpio_set_irq_enabled_with_callback(22, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, toggle_led);

    // debugging var to read what the button looks like in the terminal
    //int button_state = 0;

    while (true) {
 
        sleep_ms(500);
        printf("Main printing every 0.5 sec.\n");

       // tight_loop_contents();
    }
}

/*
// old school version of button polling we now use the irq :)
// printf("Hello, world!\n");
// sleep_ms(1000);

// //read gpio state if it sees a logical 1 light up onboard led 
// if (gpio_get(22) == 0x01)
// {
//     button_state = gpio_get(22);
//     printf("%d\n", button_state);
//     gpio_put(25, 1);
// }
// //if logical 0 then led is off
// else
// {
//     button_state = gpio_get(22);
//     printf("%d\n", button_state);
//     gpio_put(25, 0);
// } 
*/