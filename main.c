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
    
    
    
    printf("slice: %d\n", slice);
    printf("clock_freq: %d\n", clock_freq);
    printf("wrap: %d\n", wrap);
    pwm_set_enabled(slice, true);


}

int64_t disablePin(alarm_id_t id, void *user_data) {
    gpio_put(16,0);
    printf("light off\n");
    return 0;
}
bool flash(struct repeating_timer *t) {
    gpio_put(16,1);
    printf("light on\n");
    int pinValue=16;
    add_alarm_in_ms(250,&disablePin,&pinValue,true);
    return true;
}

int main() {
   stdio_init_all();
   
   playNote(buzzerPin, 100000.5f);
   gpio_init(16);
   gpio_set_dir(16, GPIO_OUT);
   
   struct repeating_timer timer;
   add_repeating_timer_ms(1000,&flash,NULL,&timer);
   int x =0;
    while(1){
        
        }
    

   
}

