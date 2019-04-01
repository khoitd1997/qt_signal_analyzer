#ifndef _NRF_POWER_HPP
#define _NRF_POWER_HPP

#include <cstdint>

#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"

#include "nrf_pwr_mgmt.h"

#include "bsp_btn_ble.h"

namespace nrf_power {
void init(void);
void sleep(void);
}  // namespace nrf_power

#endif