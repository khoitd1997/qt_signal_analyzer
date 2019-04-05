#include "nrf_gap.hpp"

#include <cstdint>

#include "app_error.h"

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"

#include "nrf_ble_conf.hpp"

namespace nrf_ble {
namespace nrf_gap {
void init() {
  ble_gap_conn_params_t   gap_conn_params;
  ble_gap_conn_sec_mode_t sec_mode;

  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

  auto err_code = sd_ble_gap_device_name_set(
      &sec_mode, (const uint8_t *)nrf_ble_conf::DEVICE_NAME, strlen(nrf_ble_conf::DEVICE_NAME));
  APP_ERROR_CHECK(err_code);

  /* YOUR_JOB: Use an appearance value matching the application's use case.
     err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_);
     APP_ERROR_CHECK(err_code); */

  memset(&gap_conn_params, 0, sizeof(gap_conn_params));

  gap_conn_params.min_conn_interval = nrf_ble_conf::MIN_CONN_INTERVAL;
  gap_conn_params.max_conn_interval = nrf_ble_conf::MAX_CONN_INTERVAL;
  gap_conn_params.slave_latency     = nrf_ble_conf::SLAVE_LATENCY;
  gap_conn_params.conn_sup_timeout  = nrf_ble_conf::CONN_SUP_TIMEOUT;

  err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
  APP_ERROR_CHECK(err_code);
}
}  // namespace nrf_gap
}  // namespace nrf_ble