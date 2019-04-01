#ifndef _NRF_TIMER_HPP
#define _NRF_TIMER_HPP

#include <cstdint>

#include "app_error.h"
#include "app_timer.h"

class NRFTimer {
 public:
  NRFTimer(uint32_t                    intervalMS,
           void*                       context,
           app_timer_mode_t            mode,
           app_timer_timeout_handler_t timeoutHandler);

  void start();
  void stop();

 private:
  app_timer_id_t _timer;
  app_timer_t    _timer_data;

  uint32_t _interval;
  void*    _context;

  static bool _isInitialized;
  void        init();
};

#endif