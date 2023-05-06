#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "main.h"
#include <stdint.h>


float* freq;
void playNote(uint32_t gpio, float frequency) {
     gpio_set_function(buzzerPin, GPIO_FUNC_PWM);
    
    uint32_t slice = pwm_gpio_to_slice_num(buzzerPin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t divider = 1;
    
    uint32_t wrap = clock_freq /(frequency);
    while (wrap>65535) {
        divider *= 2;
        wrap = clock_freq/(frequency) /divider;

    }

    pwm_set_wrap(slice, wrap); //sets the value to wrap to 0 at
    pwm_set_clkdiv(slice, divider); //sets the divider which is when the counter increments: sysclock/divider
    pwm_set_chan_level(slice, PWM_CHAN_A, wrap/2); //counter compare value for a channel. the time spent at high is proportional to the this value. 
    
    
    
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

void changeFreq(uint gpio, uint32_t event_mask) {
    if (gpio==20) {
        freq--;
        if (freq< &freqs[0]) {
            freq = &freqs[0];
        }
        
    } else if (gpio==21) {
        freq++;
        if (freq>&freqs[freqSize-1]) {
            freq = &freqs[freqSize-1];
        }

    } else {
        freq = &freqs[middleCIndex]; //last button resets
        
    }
    playNote(buzzerPin, *freq);
}

int main() {
   stdio_init_all();
   sleep_ms(3000);
   
   gpio_init(16);
   gpio_set_dir(16, GPIO_OUT);


   gpio_init(20);
   gpio_set_dir(20,  GPIO_IN);
   gpio_init(21);
   gpio_set_dir(21,  GPIO_IN);
   gpio_init(22);
   gpio_set_dir(22,  GPIO_IN); 
   freq = &freqs[middleCIndex];
   printf("freq: %f\n",*freq);
   playNote(buzzerPin, *freq);
   gpio_set_irq_enabled_with_callback(20,GPIO_IRQ_LEVEL_LOW,true,&changeFreq);
   struct repeating_timer timer;
   add_repeating_timer_ms(1000,&flash,NULL,&timer);
   int x =0;
    while(1){
        
        }
    

   
}

