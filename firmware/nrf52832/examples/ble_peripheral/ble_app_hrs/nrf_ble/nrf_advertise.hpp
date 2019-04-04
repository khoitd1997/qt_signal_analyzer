#ifndef _NRF_ADVERTISING_HPP
#define _NRF_ADVERTISING_HPP

#include "ble_advertising.h"

namespace nrf_ble {
namespace nrf_advertise {
void init(ble_adv_evt_handler_t evtHandler);

void start(bool eraseBond, ble_adv_mode_t mode);

extern ble_advertising_t &advertiseMod;
}  // namespace nrf_advertise
}  // namespace nrf_ble
#endif