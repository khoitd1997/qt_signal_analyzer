#ifndef _SIGNAL_ANALYZER_BATTERY_MANAGER_HPP
#define _SIGNAL_ANALYZER_BATTERY_MANAGER_HPP

#include <cstdint>
#include <cstring>

#include "ble_dis.h"

#include "sensorsim.h"  // only when testing

#include "app_timer.h"

#include "ble_bas.h"
#include "ble_err.h"
#include "ble_srv_common.h"

class BatteryManager {
 public:
  BatteryManager(void);

  static void update(void);
  static void timeout_handler(void *p_context);
  static void startTimer(void);

 private:
  BLE_BAS_DEF(m_bas);
  static app_timer_id_t m_battery_timer_id;

  //   APP_TIMER_DEF(m_battery_timer_id);
  // only when testing
  static sensorsim_cfg_t   m_battery_sim_cfg;
  static sensorsim_state_t m_battery_sim_state;
};

#endif