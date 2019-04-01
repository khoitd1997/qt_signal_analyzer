#include "nrf_service.hpp"

#include "app_error.h"
#include "nrf_sdh_ble.h"

#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "ble_hci.h"
#include "ble_srv_common.h"

#include "bsp_btn_ble.h"

#include "fds.h"
#include "nordic_common.h"

#include "nrf.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_pwr_mgmt.h"

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"

bool NRFService::_isInitialized = false;

NRF_BLE_QWR_DEF(_qwr);
nrf_ble_qwr_t& NRFService::qwr = _qwr;

NRFService::NRFService() { init(); }

void NRFService::errorHandler(uint32_t nrf_error) { APP_ERROR_HANDLER(nrf_error); }

void NRFService::init() {
  if (!_isInitialized) {
    nrf_ble_qwr_init_t qwr_init = {0};

    qwr_init.error_handler = NRFService::errorHandler;

    auto err_code = nrf_ble_qwr_init(&qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);
    _isInitialized = true;
  }
}