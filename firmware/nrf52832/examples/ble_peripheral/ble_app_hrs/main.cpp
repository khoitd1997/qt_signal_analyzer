/**
 * Copyright (c) 2014 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup ble_sdk_app_template_main main.c
 * @{
 * @ingroup ble_sdk_app_template
 * @brief Template project main file.
 *
 * This file contains a template for creating a new application. It has the code necessary to wakeup
 * from button, advertise, get a connection restart advertising on disconnect and if no new
 * connection created go back to system-off mode.
 * It can easily be used as a starting point for creating a new application, the comments identified
 * with 'YOUR_JOB' indicates where and how you can customize.
 */

#include <cstdbool>
#include <cstdint>
#include <cstring>

#include "app_error.h"
#include "app_timer.h"

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

#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "sensorsim.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_bsp.hpp"
#include "nrf_power.hpp"
#include "nrf_service.hpp"
#include "nrf_timer.hpp"

#include "nrf_ble/nrf_advertise.hpp"
#include "nrf_ble/nrf_ble.hpp"
#include "nrf_ble/nrf_ble_conf.hpp"
#include "nrf_ble/nrf_conn_params.hpp"
#include "nrf_ble/nrf_gap.hpp"
#include "nrf_ble/nrf_gatt.hpp"
#include "nrf_ble/nrf_peer_manager.hpp"

#define MANUFACTURER_NAME "KhoiTrinh"

#define DEAD_BEEF 0xDEADBEEF

static uint16_t currConnection = BLE_CONN_HANDLE_INVALID;

void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name) {
  NRF_LOG_INFO("Assert callback");
  app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

static void pm_evt_handler(pm_evt_t const *p_evt) {
  pm_handler_on_pm_evt(p_evt);
  pm_handler_flash_clean(p_evt);

  switch (p_evt->evt_id) {
    case PM_EVT_PEERS_DELETE_SUCCEEDED:
      NRF_LOG_INFO("PM peer delete succeeded");
      nrf_ble::nrf_advertise::start(false, BLE_ADV_MODE_FAST);
      break;

    default:
      break;
  }
}

static void on_adv_evt(ble_adv_evt_t ble_adv_evt) {
  ret_code_t err_code;

  switch (ble_adv_evt) {
    case BLE_ADV_EVT_FAST:
      NRF_LOG_INFO("Fast advertising.");
      err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
      APP_ERROR_CHECK(err_code);
      break;

    case BLE_ADV_EVT_IDLE:
      NRF_LOG_INFO("Sleeping");
      nrf_power::sleep();
      break;

    default:
      break;
  }
}

static void on_conn_params_evt(ble_conn_params_evt_t *p_evt) {
  ret_code_t err_code;

  if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED) {
    NRF_LOG_INFO("Conn Param Event Failed");
    err_code = sd_ble_gap_disconnect(p_evt->conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
    APP_ERROR_CHECK(err_code);
  }
}

static void conn_params_error_handler(uint32_t nrf_error) { APP_ERROR_HANDLER(nrf_error); }

void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context) {
  ret_code_t err_code = NRF_SUCCESS;
  NRF_LOG_INFO("event handler");

  switch (p_ble_evt->header.evt_id) {
    case BLE_GAP_EVT_DISCONNECTED:
      NRF_LOG_INFO("Disconnected.");
      // LED indication will be changed when advertising starts.
      break;

    case BLE_GAP_EVT_CONNECTED:
      NRF_LOG_INFO("Connected.");
      err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
      APP_ERROR_CHECK(err_code);
      currConnection = p_ble_evt->evt.gap_evt.conn_handle;
      err_code       = nrf_ble_qwr_conn_handle_assign(&NRFService::qwr, currConnection);
      APP_ERROR_CHECK(err_code);
      break;

    case BLE_GAP_EVT_PHY_UPDATE_REQUEST: {
      NRF_LOG_DEBUG("PHY update request.");
      ble_gap_phys_t const phys = {
          .tx_phys = BLE_GAP_PHY_AUTO,
          .rx_phys = BLE_GAP_PHY_AUTO,
      };
      err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
      APP_ERROR_CHECK(err_code);
    } break;

    case BLE_GATTC_EVT_TIMEOUT:
      // Disconnect on GATT Client timeout event.
      NRF_LOG_DEBUG("GATT Client Timeout.");
      err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                       BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      APP_ERROR_CHECK(err_code);
      break;

    case BLE_GATTS_EVT_TIMEOUT:
      // Disconnect on GATT Server timeout event.
      NRF_LOG_DEBUG("GATT Server Timeout.");
      err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                       BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      APP_ERROR_CHECK(err_code);
      break;

    default:
      // No implementation needed.
      break;
  }
}

static void bsp_event_handler(bsp_event_t event) {
  ret_code_t err_code;

  switch (event) {
    case BSP_EVENT_SLEEP:
      NRF_LOG_INFO("Sleep");
      nrf_power::sleep();
      break;  // BSP_EVENT_SLEEP

    case BSP_EVENT_DISCONNECT:
      NRF_LOG_INFO("Disconnect");
      err_code = sd_ble_gap_disconnect(currConnection, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      if (err_code != NRF_ERROR_INVALID_STATE) { APP_ERROR_CHECK(err_code); }
      break;  // BSP_EVENT_DISCONNECT

    case BSP_EVENT_WHITELIST_OFF:
      NRF_LOG_INFO("Whitelist OFF");
      if (currConnection == BLE_CONN_HANDLE_INVALID) {
        err_code = ble_advertising_restart_without_whitelist(&nrf_ble::nrf_advertise::advertiseMod);
        if (err_code != NRF_ERROR_INVALID_STATE) { APP_ERROR_CHECK(err_code); }
      }
      break;  // BSP_EVENT_KEY_0

    default:
      break;
  }
}

static void idle_state_handle(void) {
  if (NRF_LOG_PROCESS() == false) { nrf_pwr_mgmt_run(); }
}

static void testHandler(void *context) {
  // NRF_LOG_INFO("Got Number: ");
}

int main(void) {
  bool erase_bonds;

  // log init
  auto err_code = NRF_LOG_INIT(nullptr);
  APP_ERROR_CHECK(err_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  NRFTimer nrfTimer(5, nullptr, APP_TIMER_MODE_REPEATED, testHandler);

  nrf_bsp::init(&erase_bonds, bsp_event_handler);
  nrf_power::init();

  nrf_ble::init();
  // register here since it won't work in init function
  // the handler needs to be static func in same file
  NRF_SDH_BLE_OBSERVER(
      m_ble_observer, nrf_ble::nrf_ble_conf::APP_BLE_OBSERVER_PRIO, ble_evt_handler, nullptr);

  nrf_ble::nrf_gap::init();
  nrf_ble::nrf_gatt::init(nullptr);

  nrf_ble::nrf_advertise::init(on_adv_evt);
  NRFService nrfService;
  nrf_ble::nrf_conn_params::init(nullptr, on_conn_params_evt, conn_params_error_handler);
  nrf_ble::nrf_peer_manager::init(pm_evt_handler);

  // Start execution.
  NRF_LOG_INFO("Template example started.");
  nrfTimer.start();

  nrf_ble::nrf_advertise::start(erase_bonds, BLE_ADV_MODE_FAST);

  // Enter main loop.
  for (;;) { idle_state_handle(); }
}
