#ifndef _NRF_SERVICE_HPP
#define _NRF_SERVICE_HPP

#include <cstdint>

#include "nrf_ble_qwr.h"

class NRFService {
 public:
  NRFService();

  static void           errorHandler(uint32_t nrf_error);
  static nrf_ble_qwr_t& qwr;

  virtual ~NRFService() {}

 private:
  void init();

  static bool _isInitialized;
};

#endif