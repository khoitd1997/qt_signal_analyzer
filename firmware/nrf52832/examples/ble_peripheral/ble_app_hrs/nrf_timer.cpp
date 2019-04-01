#include "nrf_timer.hpp"

NRFTimer::NRFTimer(uint32_t                    intervalMS,
                   void*                       context,
                   app_timer_mode_t            mode,
                   app_timer_timeout_handler_t timeoutHandler)
    : _interval(APP_TIMER_TICKS(intervalMS)),
      _context(context),
      _timer(&_timer_data),
      _timer_data(),
      _isInitialized(false) {
  init();
  ret_code_t err_code = app_timer_create(&_timer, mode, timeoutHandler);
  APP_ERROR_CHECK(err_code);
}

void NRFTimer::start() { app_timer_start(_timer, _interval, _context); }

void NRFTimer::stop() { app_timer_stop(_timer); }

void NRFTimer::init() {
  ret_code_t err_code = app_timer_init();
  APP_ERROR_CHECK(err_code);
  _isInitialized = true;
}