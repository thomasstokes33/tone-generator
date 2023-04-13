#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "main.h"


// static const uint buzzerPin = 18;
float* freq;

// float freqs[] = {27.5f,
// 29.13524f,
// 30.86771f,
// 32.7032f,
// 34.64783f,
// 36.7081f,
// 38.89087f,
// 41.20344f,
// 43.65353f,
// 46.2493f,
// 48.99943f,
// 51.91309f,
// 55.0f,
// 58.27047f,
// 61.73541f,
// 65.40639f,
// 69.29566f,
// 73.41619f,
// 77.78175f,
// 82.40689f,
// 87.30706f,
// 92.49861f,
// 97.99886f,
// 103.8262f,
// 110.0f,
// 116.5409f,
// 123.4708f,
// 130.8128f,
// 138.5913f,
// 146.8324f,
// 155.5635f,
// 164.8138f,
// 174.6141f,
// 184.9972f,
// 195.9977f,
// 207.6523f,
// 220.0f,
// 233.0819f,
// 246.9417f,
// 261.6256f,
// 277.1826f,
// 293.6648f,
// 311.127f,
// 329.6276f,
// 349.2282f,
// 369.9944f,
// 391.9954f,
// 415.3047f,
// 440.0f,
// 466.1638f,
// 493.8833f,
// 523.2511f,
// 554.3653f,
// 587.3295f,
// 622.254f,
// 659.2551f,
// 698.4565f,
// 739.9888f,
// 783.9909f,
// 830.6094f,
// 880.0f,
// 932.3275f,
// 987.7666f,
// 1046.502f,
// 1108.731f,
// 1174.659f,
// 1244.508f,
// 1318.51f,
// 1396.913f,
// 1479.978f,
// 1567.982f,
// 1661.219f,
// 1760.0f,
// 1864.655f,
// 1975.533f,
// 2093.005f,
// 2217.461f,
// 2349.318f,
// 2489.016f,
// 2637.02f,
// 2793.826f,
// 2959.955f,
// 3135.963f,
// 3322.438f,
// 3520.0f,
// 3729.31f,
// 3951.066f,
// 4186.009f};

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

    freq=&freqs[48];
    playNote(buzzerPin, *freq);

    gpio_set_irq_enabled_with_callback(20, GPIO_IRQ_EDGE_FALL, true, &changeFreq);
    gpio_set_irq_enabled_with_callback(21, GPIO_IRQ_EDGE_FALL, true, &changeFreq);
    gpio_set_irq_enabled_with_callback(22, GPIO_IRQ_EDGE_FALL, true, &changeFreq);

    while (true) {
        tight_loop_contents();
    }
}

