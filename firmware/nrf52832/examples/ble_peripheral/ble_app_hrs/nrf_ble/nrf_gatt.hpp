#ifndef _NRF_GATT_HPP
#define _NRF_GATT_HPP

#include "nrf_ble_gatt.h"

namespace nrf_ble {
namespace nrf_gatt {
void init(nrf_ble_gatt_evt_handler_t evtHandler);

extern nrf_ble_gatt_t &gattMod;
}  // namespace nrf_gatt
}  // namespace nrf_ble
#endif