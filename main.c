#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "main.h"

float* freq;

void playNote(uint gpio, float frequency) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    
    uint pwmSlice = pwm_gpio_to_slice_num(gpio);

    float divider = 1.0f;
    uint32_t sys_clk_freq = clock_get_hz(clk_sys);
    uint32_t wrap_value = sys_clk_freq / (divider * frequency);
    while (wrap_value > 65535) {
        divider *= 2.0f;
        wrap_value = sys_clk_freq / (divider * frequency);
    }

    pwm_set_clkdiv(pwmSlice, divider);

    pwm_set_wrap(pwmSlice, wrap_value);

    uint32_t dutyCycle = wrap_value / 2;
    pwm_set_chan_level(pwmSlice, PWM_CHAN_A, dutyCycle);

    pwm_set_enabled(pwmSlice, true);
}

void changeFreq(uint gpio, uint32_t event_mask) {
    switch (gpio) {
        case 20: 
            freq--;
            if (freq < &freqs[0]) {freq = &freqs[0];}
            break;
        case 21:
            freq = &freqs[48];
            break;
        case 22:
            freq++;
            if (freq > &freqs[87]) {freq = &freqs[87];}
            break;
    }
    playNote(buzzerPin, *freq);
}

int main() {
    stdio_init_all();

    uint pwmSlice = pwm_gpio_to_slice_num(buzzerPin);

    // Set up and start the PWM
    freq=&freqs[48]; // A4 frequency
    playNote(buzzerPin, *freq);

    gpio_set_irq_enabled_with_callback(20, GPIO_IRQ_EDGE_FALL, true, &changeFreq);
    gpio_set_irq_enabled_with_callback(21, GPIO_IRQ_EDGE_FALL, true, &changeFreq);
    gpio_set_irq_enabled_with_callback(22, GPIO_IRQ_EDGE_FALL, true, &changeFreq);

    while (true) {
        // Keep the PWM running
        tight_loop_contents();
    }
}

