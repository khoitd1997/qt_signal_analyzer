#include "nrf_conn_params.hpp"

#include <cstdint>
#include <cstring>

#include "app_error.h"

#include "nrf_ble_conf.hpp"
#include "nrf_log.h"

namespace nrf_ble {
namespace nrf_conn_params {
void init(ble_gap_conn_params_t*        gapParam,
          ble_conn_params_evt_handler_t evtHandler,
          ble_srv_error_handler_t       errorHandler) {
  ble_conn_params_init_t cp_init;

  memset(&cp_init, 0, sizeof(cp_init));

  cp_init.p_conn_params = gapParam;

  cp_init.first_conn_params_update_delay = nrf_ble_conf::FIRST_CONN_PARAMS_UPDATE_DELAY;
  cp_init.next_conn_params_update_delay  = nrf_ble_conf::NEXT_CONN_PARAMS_UPDATE_DELAY;
  cp_init.max_conn_params_update_count   = nrf_ble_conf::MAX_CONN_PARAMS_UPDATE_COUNT;
  cp_init.start_on_notify_cccd_handle    = nrf_ble_conf::CCCD_HANDLE;
  cp_init.disconnect_on_fail             = nrf_ble_conf::DISCONNECT_ON_FAIL;

  cp_init.evt_handler   = evtHandler;
  cp_init.error_handler = errorHandler;

  auto err_code = ble_conn_params_init(&cp_init);
  APP_ERROR_CHECK(err_code);
}

}  // namespace nrf_conn_params
}  // namespace nrf_ble