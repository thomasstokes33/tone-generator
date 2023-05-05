#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "main.h"
#include <stdint.h>



int playNote(uint32_t gpio, float frequency) {
     gpio_set_function(buzzerPin, GPIO_FUNC_PWM);
    
    uint32_t slice = pwm_gpio_to_slice_num(buzzerPin);

    pwm_set_wrap(slice, 3);
    pwm_set_chan_level(slice, PWM_CHAN_A, 1);
    
    pwm_set_chan_level(slice, PWM_CHAN_A, 3);
    

    pwm_set_enabled(slice, true);


}
int main() {
   playNote(buzzerPin, 27.5f);
}

