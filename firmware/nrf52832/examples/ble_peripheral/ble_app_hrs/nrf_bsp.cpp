#include "nrf_bsp.hpp"

#include "app_error.h"

namespace nrf_bsp {
void init(bool *p_erase_bonds, bsp_event_callback_t callback) {
  ret_code_t  err_code;
  bsp_event_t startup_event;

  err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, callback);
  APP_ERROR_CHECK(err_code);

  err_code = bsp_btn_ble_init(NULL, &startup_event);
  APP_ERROR_CHECK(err_code);

  *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}
}  // namespace nrf_bsp