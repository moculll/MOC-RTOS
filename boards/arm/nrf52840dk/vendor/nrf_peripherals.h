/*

Copyright (c) 2010 - 2021, Nordic Semiconductor ASA

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form, except as embedded into a Nordic
   Semiconductor ASA integrated circuit in a product or a software update for
   such product, must reproduce the above copyright notice, this list of
   conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

3. Neither the name of Nordic Semiconductor ASA nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

4. This software, with or without modification, must only be used with a
   Nordic Semiconductor ASA integrated circuit.

5. Any software provided in binary form under this license must not be reverse
   engineered, decompiled, modified and/or disassembled.

THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef NRF_PERIPHERALS_H__
#define NRF_PERIPHERALS_H__

/*lint ++flb "Enter library region */

#if defined(NRF51)
    #include "nrf51_peripherals.h"

#elif defined (NRF52805_XXAA)
    #include "nrf52805_peripherals.h"
#elif defined(NRF52810_XXAA)
    #include "nrf52810_peripherals.h"
#elif defined(NRF52811_XXAA)
    #include "nrf52811_peripherals.h"
#elif defined(NRF52820_XXAA)
    #include "nrf52820_peripherals.h"
#elif defined(NRF52832_XXAA) || defined(NRF52832_XXAB)
    #include "nrf52832_peripherals.h"
#elif defined (NRF52833_XXAA)
    #include "nrf52833_peripherals.h"
#elif defined(NRF52840_XXAA)
    #include <vendor/nrf52840_peripherals.h>

#elif defined (NRF5340_XXAA_APPLICATION)
    #include "nrf5340_application_peripherals.h"
#elif defined (NRF5340_XXAA_NETWORK)
    #include "nrf5340_network_peripherals.h"

#elif defined(NRF9160_XXAA)
    #include "nrf9160_peripherals.h"

#else
    #error "Device must be defined. See nrf.h."
#endif

/*lint --flb "Leave library region" */

#endif // NRF_PERIPHERALS_H__
