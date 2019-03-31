#include "battery_manager.hpp"

#define BATTERY_LEVEL_MEAS_INTERVAL \
  APP_TIMER_TICKS(2000)       /**< Battery level measurement interval (ticks). */
#define MIN_BATTERY_LEVEL 81  /**< Minimum simulated battery level. */
#define MAX_BATTERY_LEVEL 100 /**< Maximum simulated battery level. */
#define BATTERY_LEVEL_INCREMENT \
  1 /**< Increment between each simulated battery level measurement. */

BatteryManager::BatteryManager(void) {
  m_battery_sim_cfg.min          = MIN_BATTERY_LEVEL;
  m_battery_sim_cfg.max          = MAX_BATTERY_LEVEL;
  m_battery_sim_cfg.incr         = BATTERY_LEVEL_INCREMENT;
  m_battery_sim_cfg.start_at_max = true;

  sensorsim_init(&m_battery_sim_state, &m_battery_sim_cfg);

  ret_code_t err_code;

  // Initialize timer module.
  err_code = app_timer_init();
  APP_ERROR_CHECK(err_code);

  // Start application timers.
  err_code = app_timer_create(
      &m_battery_timer_id, APP_TIMER_MODE_REPEATED, BatteryManager::timeout_handler);
  APP_ERROR_CHECK(err_code);

  ret_code_t         err_code;
  ble_bas_init_t     bas_init;
  ble_dis_init_t     dis_init;
  nrf_ble_qwr_init_t qwr_init = {0};

  // Initialize Battery Service.
  memset(&bas_init, 0, sizeof(bas_init));

  bas_init.evt_handler          = NULL;
  bas_init.support_notification = true;
  bas_init.p_report_ref         = NULL;
  bas_init.initial_batt_level   = 100;

  // Here the sec level for the Battery Service can be changed/increased.
  bas_init.bl_rd_sec        = SEC_OPEN;
  bas_init.bl_cccd_wr_sec   = SEC_OPEN;
  bas_init.bl_report_rd_sec = SEC_OPEN;

  err_code = ble_bas_init(&m_bas, &bas_init);
  APP_ERROR_CHECK(err_code);
}

void BatteryManager::startTimer(void) {
  ret_code_t err_code;

  // Start application timers.
  err_code = app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);
  APP_ERROR_CHECK(err_code);
}

void BatteryManager::update(void) {
  ret_code_t err_code;
  uint8_t    battery_level;

  battery_level = (uint8_t)sensorsim_measure(&m_battery_sim_state, &m_battery_sim_cfg);

  err_code = ble_bas_battery_level_update(&m_bas, battery_level, BLE_CONN_HANDLE_ALL);
  if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_INVALID_STATE) &&
      (err_code != NRF_ERROR_RESOURCES) && (err_code != NRF_ERROR_BUSY) &&
      (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)) {
    APP_ERROR_HANDLER(err_code);
  }
}

void BatteryManager::timeout_handler(void *p_context) {
  UNUSED_PARAMETER(p_context);
  update();
}