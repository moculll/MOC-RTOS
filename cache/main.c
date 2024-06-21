#include <stdio.h>
#include <nrfx_gpiote.h>
#include <nrfx_uart.h>
int main(void)
{
    nrfx_gpiote_init();
    nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);

    nrfx_gpiote_out_init(13, &out_config);

    /* nrfx_gpiote_out_toggle(LEDS[self->count++]); */

    nrfx_gpiote_out_clear(13);


    nrfx_uart_t uart = NRFX_UART_INSTANCE(0);
    nrfx_uart_config_t cfg = NRFX_UART_DEFAULT_CONFIG;
    nrfx_uart_init(&uart, &cfg, NULL);


    while(1){
        nrfx_uart_tx(&uart, "fudasdas\r\n", 11);

    }

    
}