#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blink.pio.h"
#include "conditionalblink.pio.h"

int main() {

    static const uint led0_pin = 18;
    static const uint led1_pin = 24;
    static const float pio_freq0 = 2000;
    static const float pio_freq1 = 5000;
    static const float pio_freq2 = 7000;

    // Initialize chosen serial port
    stdio_init_all();

    // Choose PIO instance (0 or 1)
    PIO pio = pio0;

    // Get first free state machine in PIO 0
    uint sm0 = pio_claim_unused_sm(pio, true);
    uint sm1 = pio_claim_unused_sm(pio, true);
    uint sm2 = pio_claim_unused_sm(pio, true);

    // Add PIO program to PIO instruction memory. SDK will find location and
    // return with the memory offset of the program.
    uint offset0 = pio_add_program(pio, &blink_program);
    uint offset1 = pio_add_program(pio, &conditionalblink_program);

    // Calculate the PIO clock divider
    float div0 = (float)clock_get_hz(clk_sys) / pio_freq0;
    float div1 = (float)clock_get_hz(clk_sys) / pio_freq1;
    float div2 = (float)clock_get_hz(clk_sys) / pio_freq2;

    // Initialize the program using the helper function in our .pio file
    blink_program_init(pio, sm0, offset0, led0_pin, div0);
    blink_program_init(pio, sm1, offset0, led1_pin, div1);
    conditionalblink_program_init(pio, sm2, offset1, led0_pin, led1_pin, div2);

    // Start running our PIO program in the state machine
    // pio_sm_set_enabled(pio, sm1, true);
    // pio_sm_set_enabled(pio, sm2, true);

    // Do nothing
    while (true) {
        pio_sm_set_enabled(pio, sm2, false);
        pio_sm_set_enabled(pio, sm0, true);
        sleep_ms(500);
        pio_sm_set_enabled(pio, sm0, false);
        pio_sm_set_enabled(pio, sm1, true);
        sleep_ms(500);
        pio_sm_set_enabled(pio, sm1, false);
        pio_sm_set_enabled(pio, sm2, true);
        printf("Serious debug message.\r\n");
        sleep_ms(1500);
    }
}