
#include <osImpl/irqImpl.h>
#include <osImpl/systickImpl.h>
#include <nrf_systick.h>
void mocSystickInitImpl(uint32_t ms)
{

    

    nrf_systick_load_set(NRF_SYSTICK_VAL_MASK);
    nrf_systick_csr_set(
        NRF_SYSTICK_CSR_CLKSOURCE_CPU |
        NRF_SYSTICK_CSR_TICKINT_ENABLE |
        NRF_SYSTICK_CSR_ENABLE);
}
