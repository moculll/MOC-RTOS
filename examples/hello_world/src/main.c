#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nrf.h"
#include <nrf_gpio.h>
#include <nrfx_uarte.h>
#include "shellMgr.h"
#define GPIO_PIN(x, y)      ((x << 5) | y)
void uart_init(){
    nrf_gpio_cfg_output(6);
    NRF_UART0->PSEL.TXD = 6;
    NRF_UART0->PSEL.RXD = GPIO_PIN(0, 8);
    NRF_UART0->CONFIG = 0;
    NRF_UART0->BAUDRATE = (UART_BAUDRATE_BAUDRATE_Baud115200 << UART_BAUDRATE_BAUDRATE_Pos);
    NRF_UART0->ENABLE           = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
    NRF_UART0->TASKS_STARTTX    = 1;
    NRF_UART0->TASKS_STARTRX    = 1;
    NRF_UART0->EVENTS_RXDRDY    = 0;
    
    

    NRF_UART0->PSEL.RTS = 0xffffffff;
    NRF_UART0->PSEL.CTS = 0xffffffff;
    

}
void simple_uart_put(uint8_t cr)
{
  NRF_UART0->TXD = (uint8_t)cr;

  while (NRF_UART0->EVENTS_TXDRDY!=1)
  {
    // Wait for TXD data to be sent
  }

  NRF_UART0->EVENTS_TXDRDY=0;
}

void simple_uart_putstring(const uint8_t *str)
{
  uint_fast8_t i = 0;
  uint8_t ch = str[i++];
  while (ch != '\0')
  {
    simple_uart_put(ch);
    ch = str[i++];
  }
}

void simple_uart_test(){
    uart_init();
    simple_uart_putstring("123");
}
static char buffer;
nrfx_uarte_t shitinstance = NRFX_UARTE_INSTANCE(0);
void main()
{

    

    while(1){
        nrfx_uarte_rx(&shitinstance, &buffer, 1);
        

    }

    
}
