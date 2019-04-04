#ifndef _NRF_BLE_CONF_HPP
#define _NRF_BLE_CONF_HPP

#include <cstdint>

#include "ble_gap.h"
#include "ble_gatt.h"

#include "ble_advdata.h"
#include "ble_advertising.h"

#include "ble_srv_common.h"

namespace nrf_ble {
namespace nrf_ble_conf {
// conn_params
static const uint32_t FIRST_CONN_PARAMS_UPDATE_DELAY = 5000;
static const uint32_t NEXT_CONN_PARAMS_UPDATE_DELAY  = 3000;
static const uint8_t  MAX_CONN_PARAMS_UPDATE_COUNT   = 3;
static const uint16_t CCCD_HANDLE                    = BLE_GATT_HANDLE_INVALID;
static const bool     DISCONNECT_ON_FAIL             = false;

// advertise
static const ble_advdata_name_type_t APP_ADV_NAME               = BLE_ADVDATA_FULL_NAME;
static const bool                    APP_ADV_INCLUDE_APPEARANCE = true;
static const uint8_t                 APP_ADV_FLAG = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

static bool           APP_ADV_FAST_ENABLED = true;
static ble_uuid_t     APP_ADV_UUIDS[]  = {{BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}};
static const uint32_t APP_ADV_INTERVAL = 300;
static const uint32_t APP_ADV_DURATION = 18000;
static const uint8_t  APP_BLE_OBSERVER_PRIO = 3;
static const uint8_t  APP_BLE_CONN_CFG_TAG  = 1;

// peer_manager
static const uint8_t SEC_PARAM_BOND            = 1;
static const uint8_t SEC_PARAM_MITM            = 0;
static const uint8_t SEC_PARAM_LESC            = 0;
static const uint8_t SEC_PARAM_KEYPRESS        = 0;
static const uint8_t SEC_PARAM_IO_CAPABILITIES = BLE_GAP_IO_CAPS_NONE;
static const uint8_t SEC_PARAM_OOB             = 0;
static const uint8_t SEC_PARAM_MIN_KEY_SIZE    = 7;
static const uint8_t SEC_PARAM_MAX_KEY_SIZE    = 16;

}  // namespace nrf_ble_conf
}  // namespace nrf_ble

#endif