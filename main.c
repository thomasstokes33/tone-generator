#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "main.h"



int playNote(uint32_t gpio, float frequency) {
     gpio_set_function(buzzerPin, GPIO_FUNC_PWM);
    
    uint32_t slice = pwm_gpio_to_slice_num(buzzerPin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint64_t divider = 1;
    uint32_t wrap = clock_freq /(divider * frequency);


    pwm_set_wrap(slice, wrap); //sets the value to wrap to 0 at
    pwm_set_clkdiv(slice, divider); //sets the divider which is when the counter increments: sysclock/divider
    pwm_set_chan_level(slice, PWM_CHAN_A, 1); //counter compare value for a channel
    
    
    

    pwm_set_enabled(slice, true);


}
int main() {
   playNote(buzzerPin, 1000.5f);
}

