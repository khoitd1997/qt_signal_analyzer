#include "nrf_ble.hpp"

#include "app_error.h"

#include "nrf_ble_conf.hpp"

#include "ble_gap.h"
#include "ble_gatt.h"

#include "bsp_btn_ble.h"
#include "nrf_ble_qwr.h"

#include "../nrf_service.hpp"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

namespace nrf_ble {
void init(void) {
  NRF_LOG_INFO("init nrf_ble");
  auto err_code = nrf_sdh_enable_request();
  APP_ERROR_CHECK(err_code);

  // Configure the BLE stack using the default settings.
  // Fetch the start address of the application RAM.
  uint32_t ram_start = 0;
  err_code           = nrf_sdh_ble_default_cfg_set(nrf_ble_conf::APP_BLE_CONN_CFG_TAG, &ram_start);
  APP_ERROR_CHECK(err_code);

  // Enable BLE stack.
  err_code = nrf_sdh_ble_enable(&ram_start);
  APP_ERROR_CHECK(err_code);
}
}  // namespace nrf_ble