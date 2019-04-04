#ifndef _NRF_PEER_MANAGER_HPP
#define _NRF_PEER_MANAGER_HPP

#include "peer_manager.h"

namespace nrf_ble {
namespace nrf_peer_manager {
void init(pm_evt_handler_t evtHandler);
void deleteBond();
}  // namespace nrf_peer_manager
}  // namespace nrf_ble

#endif