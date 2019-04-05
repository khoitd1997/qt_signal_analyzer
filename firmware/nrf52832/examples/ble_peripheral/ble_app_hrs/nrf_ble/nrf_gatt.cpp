#include "nrf_gatt.hpp"

#include "nrf_ble_gatt.h"
#include "nrf_sdh_ble.h"

#include "app_error.h"

NRF_BLE_GATT_DEF(_gattMod);

namespace nrf_ble {
namespace nrf_gatt {
nrf_ble_gatt_t &gattMod = _gattMod;

void init(nrf_ble_gatt_evt_handler_t evtHandler) {
  auto err_code = nrf_ble_gatt_init(&_gattMod, evtHandler);
  APP_ERROR_CHECK(err_code);
}
}  // namespace nrf_gatt
}  // namespace nrf_ble