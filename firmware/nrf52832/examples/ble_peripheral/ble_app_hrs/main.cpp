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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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
#include "nrf_ble/nrf_ble_conf.hpp"
#include "nrf_ble/nrf_conn_params.hpp"
#include "nrf_ble/nrf_peer_manager.hpp"

#define DEVICE_NAME "Signal_Analyzer"
#define MANUFACTURER_NAME "KhoiTrinh"

#define MIN_CONN_INTERVAL \
  MSEC_TO_UNITS(100, UNIT_1_25_MS) /**< Minimum acceptable connection interval (0.1 seconds). */
#define MAX_CONN_INTERVAL \
  MSEC_TO_UNITS(200, UNIT_1_25_MS) /**< Maximum acceptable connection interval (0.2 second). */
#define SLAVE_LATENCY 0            /**< Slave latency. */
#define CONN_SUP_TIMEOUT \
  MSEC_TO_UNITS(4000, UNIT_10_MS) /**< Connection supervisory timeout (4 seconds). */

#define DEAD_BEEF 0xDEADBEEF

NRF_BLE_GATT_DEF(m_gatt); /**< GATT module instance. */

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current connection. */

void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name) {
  NRF_LOG_INFO("Assert callback");
  app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
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

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void) {
  ret_code_t              err_code;
  ble_gap_conn_params_t   gap_conn_params;
  ble_gap_conn_sec_mode_t sec_mode;

  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

  err_code =
      sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)DEVICE_NAME, strlen(DEVICE_NAME));
  APP_ERROR_CHECK(err_code);

  /* YOUR_JOB: Use an appearance value matching the application's use case.
     err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_);
     APP_ERROR_CHECK(err_code); */

  memset(&gap_conn_params, 0, sizeof(gap_conn_params));

  gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
  gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
  gap_conn_params.slave_latency     = SLAVE_LATENCY;
  gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

  err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void) {
  ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt) {
  ret_code_t err_code;

  switch (ble_adv_evt) {
    case BLE_ADV_EVT_FAST:
      NRF_LOG_INFO("Fast advertising.");
      err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
      APP_ERROR_CHECK(err_code);
      break;

    case BLE_ADV_EVT_IDLE:
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

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context) {
  ret_code_t err_code = NRF_SUCCESS;

  switch (p_ble_evt->header.evt_id) {
    case BLE_GAP_EVT_DISCONNECTED:
      NRF_LOG_INFO("Disconnected.");
      // LED indication will be changed when advertising starts.
      break;

    case BLE_GAP_EVT_CONNECTED:
      NRF_LOG_INFO("Connected.");
      err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
      APP_ERROR_CHECK(err_code);
      m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
      err_code      = nrf_ble_qwr_conn_handle_assign(&NRFService::qwr, m_conn_handle);
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

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void) {
  ret_code_t err_code;

  err_code = nrf_sdh_enable_request();
  APP_ERROR_CHECK(err_code);

  // Configure the BLE stack using the default settings.
  // Fetch the start address of the application RAM.
  uint32_t ram_start = 0;
  err_code = nrf_sdh_ble_default_cfg_set(nrf_ble::nrf_ble_conf::APP_BLE_CONN_CFG_TAG, &ram_start);
  APP_ERROR_CHECK(err_code);

  // Enable BLE stack.
  err_code = nrf_sdh_ble_enable(&ram_start);
  APP_ERROR_CHECK(err_code);

  // Register a handler for BLE events.
  NRF_SDH_BLE_OBSERVER(
      m_ble_observer, nrf_ble::nrf_ble_conf::APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
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
      err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      if (err_code != NRF_ERROR_INVALID_STATE) { APP_ERROR_CHECK(err_code); }
      break;  // BSP_EVENT_DISCONNECT

    case BSP_EVENT_WHITELIST_OFF:
      NRF_LOG_INFO("Whitelist OFF");
      if (m_conn_handle == BLE_CONN_HANDLE_INVALID) {
        err_code = ble_advertising_restart_without_whitelist(&nrf_ble::nrf_advertise::advertiseMod);
        if (err_code != NRF_ERROR_INVALID_STATE) { APP_ERROR_CHECK(err_code); }
      }
      break;  // BSP_EVENT_KEY_0

    default:
      break;
  }
}

/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void) {
  if (NRF_LOG_PROCESS() == false) { nrf_pwr_mgmt_run(); }
}

static void testHandler(void *context) {
  // NRF_LOG_INFO("Got Number: ");
}

int main(void) {
  bool erase_bonds;

  // log init
  auto err_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(err_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  NRFTimer nrfTimer(5, NULL, APP_TIMER_MODE_REPEATED, testHandler);

  nrf_bsp::init(&erase_bonds, bsp_event_handler);
  nrf_power::init();

  ble_stack_init();
  gap_params_init();
  gatt_init();
  nrf_ble::nrf_advertise::init(on_adv_evt);
  NRF_LOG_INFO("nrf service main");
  NRFService nrfService;
  //   services_init();
  nrf_ble::nrf_conn_params::init(NULL, on_conn_params_evt, conn_params_error_handler);
  //   conn_params_init();
  nrf_ble::nrf_peer_manager::init(pm_evt_handler);

  // Start execution.
  NRF_LOG_INFO("Template example started.");
  nrfTimer.start();

  nrf_ble::nrf_advertise::start(erase_bonds, BLE_ADV_MODE_FAST);

  // Enter main loop.
  for (;;) { idle_state_handle(); }
}
