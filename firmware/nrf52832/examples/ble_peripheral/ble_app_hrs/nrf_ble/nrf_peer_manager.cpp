#include "nrf_peer_manager.hpp"

#include "ble_gap.h"

#include "peer_manager.h"
#include "peer_manager_handler.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_error.h"

#include "nrf_ble_conf.hpp"

namespace nrf_ble {
namespace nrf_peer_manager {
void init(pm_evt_handler_t evtHandler) {
  ble_gap_sec_params_t sec_param;

  auto err_code = pm_init();
  APP_ERROR_CHECK(err_code);

  memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

  // Security parameters to be used for all security procedures.
  sec_param.bond           = nrf_ble_conf::SEC_PARAM_BOND;
  sec_param.mitm           = nrf_ble_conf::SEC_PARAM_MITM;
  sec_param.lesc           = nrf_ble_conf::SEC_PARAM_LESC;
  sec_param.keypress       = nrf_ble_conf::SEC_PARAM_KEYPRESS;
  sec_param.io_caps        = nrf_ble_conf::SEC_PARAM_IO_CAPABILITIES;
  sec_param.oob            = nrf_ble_conf::SEC_PARAM_OOB;
  sec_param.min_key_size   = nrf_ble_conf::SEC_PARAM_MIN_KEY_SIZE;
  sec_param.max_key_size   = nrf_ble_conf::SEC_PARAM_MAX_KEY_SIZE;
  sec_param.kdist_own.enc  = 1;
  sec_param.kdist_own.id   = 1;
  sec_param.kdist_peer.enc = 1;
  sec_param.kdist_peer.id  = 1;

  err_code = pm_sec_params_set(&sec_param);
  APP_ERROR_CHECK(err_code);

  err_code = pm_register(evtHandler);
  APP_ERROR_CHECK(err_code);
}

void deleteBond() {
  NRF_LOG_INFO("Erase bonds!");

  auto err_code = pm_peers_delete();
  APP_ERROR_CHECK(err_code);
}
}  // namespace nrf_peer_manager
}  // namespace nrf_ble