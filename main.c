#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "main.h"
#include <stdint.h>



int playNote(uint32_t gpio, float frequency) {
     gpio_set_function(buzzerPin, GPIO_FUNC_PWM);
    
    uint32_t slice = pwm_gpio_to_slice_num(buzzerPin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint64_t divider = 1;
    uint32_t wrap = clock_freq /(divider * frequency);


    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, PWM_CHAN_A, 1);
    
    
    

    pwm_set_enabled(slice, true);


}
int main() {
   playNote(buzzerPin, 1000.5f);
}

