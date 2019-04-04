#include "nrf_advertise.hpp"

#include "nrf_ble_conf.hpp"
#include "nrf_peer_manager.hpp"

#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"

#include "ble_advdata.h"

#include "app_error.h"

BLE_ADVERTISING_DEF(_advertiseMod);
ble_advertising_t& nrf_ble::nrf_advertise::advertiseMod = _advertiseMod;

namespace nrf_ble {
namespace nrf_advertise {

void init(ble_adv_evt_handler_t evtHandler) {
  ble_advertising_init_t init;

  memset(&init, 0, sizeof(init));

  init.advdata.name_type          = nrf_ble_conf::APP_ADV_NAME;
  init.advdata.include_appearance = nrf_ble_conf::APP_ADV_INCLUDE_APPEARANCE;
  init.advdata.flags              = nrf_ble_conf::APP_ADV_FLAG;
  init.advdata.uuids_complete.uuid_cnt =
      sizeof(nrf_ble_conf::APP_ADV_UUIDS) / sizeof(nrf_ble_conf::APP_ADV_UUIDS[0]);
  init.advdata.uuids_complete.p_uuids = nrf_ble_conf::APP_ADV_UUIDS;

  init.config.ble_adv_fast_enabled  = nrf_ble_conf::APP_ADV_FAST_ENABLED;
  init.config.ble_adv_fast_interval = nrf_ble_conf::APP_ADV_INTERVAL;
  init.config.ble_adv_fast_timeout  = nrf_ble_conf::APP_ADV_DURATION;

  init.evt_handler = evtHandler;

  auto err_code = ble_advertising_init(&_advertiseMod, &init);
  APP_ERROR_CHECK(err_code);

  ble_advertising_conn_cfg_tag_set(&_advertiseMod, nrf_ble_conf::APP_BLE_CONN_CFG_TAG);
}

void start(bool eraseBond, ble_adv_mode_t mode) {
  if (eraseBond) {
    nrf_peer_manager::deleteBond();
    // Advertising is started by PM_EVT_PEERS_DELETED_SUCEEDED event
  } else {
    auto err_code = ble_advertising_start(&_advertiseMod, mode);
    APP_ERROR_CHECK(err_code);
  }
}
}  // namespace nrf_advertise
}  // namespace nrf_ble