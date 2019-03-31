#ifndef _NRF_BSP_HPP
#define _NRF_BSP_HPP

#include "bsp_btn_ble.h"

namespace nrf_bsp {
void init(bool *p_erase_bonds, bsp_event_callback_t callback);
}  // namespace nrf_bsp

#endif