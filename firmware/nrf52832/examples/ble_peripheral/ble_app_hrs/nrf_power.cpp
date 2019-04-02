#include "nrf_power.hpp"

#include "app_error.h"

#include "bsp_btn_ble.h"

namespace nrf_power {
void init(void) {
  auto err_code = nrf_pwr_mgmt_init();
  APP_ERROR_CHECK(err_code);
}

void sleep(void) {
  auto err_code = bsp_indication_set(BSP_INDICATE_IDLE);
  APP_ERROR_CHECK(err_code);

  // Prepare wakeup buttons.
  err_code = bsp_btn_ble_sleep_mode_prepare();
  APP_ERROR_CHECK(err_code);

#ifdef DEBUG_NRF
  // Go to system-off mode (this function will not return; wakeup will cause a reset).
  (void)sd_power_system_off();
  while (1)
    ;
#else
  APP_ERROR_CHECK(sd_power_system_off());
#endif  // DEBUG_NRF
}
}