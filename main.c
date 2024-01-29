#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blink.pio.h"

int main() {

    static const uint led0_pin = 18;
    static const uint led1_pin = 24;
    static const float pio_freq = 2000;

    // Initialize chosen serial port
    stdio_init_all();

    // Choose PIO instance (0 or 1)
    PIO pio = pio0;

    // Get first free state machine in PIO 0
    uint sm0 = pio_claim_unused_sm(pio, true);
    uint sm1 = pio_claim_unused_sm(pio, true);

    // Add PIO program to PIO instruction memory. SDK will find location and
    // return with the memory offset of the program.
    uint offset = pio_add_program(pio, &blink_program);

    // Calculate the PIO clock divider
    float div = (float)clock_get_hz(clk_sys) / pio_freq;

    // Initialize the program using the helper function in our .pio file
    blink_program_init(pio, sm0, offset, led0_pin, div);
    blink_program_init(pio, sm1, offset, led1_pin, div);

    // Start running our PIO program in the state machine
    // pio_sm_set_enabled(pio, sm1, true);
    // pio_sm_set_enabled(pio, sm2, true);

    // Do nothing
    while (true) {
        sleep_ms(500);
        pio_sm_set_enabled(pio, sm0, true);
        pio_sm_set_enabled(pio, sm1, false);
        sleep_ms(500);
        pio_sm_set_enabled(pio, sm0, false);
        pio_sm_set_enabled(pio, sm1, true);
        printf("Serious debug message.\r\n");
    }
}