#ifndef _NRF_BLE_CONF_HPP
#define _NRF_BLE_CONF_HPP

#include <cstdint>

#include "ble_gat.h"

namespace nrf_ble {
namespace nrf_ble_conf {
static const uint32_t FIRST_CONN_PARAMS_UPDATE_DELAY = 5000;
static const uint32_t NEXT_CONN_PARAMS_UPDATE_DELAY  = 3000;
static const uint8_t  MAX_CONN_PARAMS_UPDATE_COUNT   = 3;
static const uint16_t CCCD_HANDLE                    = BLE_GATT_HANDLE_INVALID;
static const bool     DISCONNECT_ON_FAIL             = false
}  // namespace nrf_ble_conf
}  // namespace nrf_ble

#endif