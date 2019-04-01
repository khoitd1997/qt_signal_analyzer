#ifndef _NRF_CONN_PARAMS_HPP
#define _NRF_CONN_PARAMS_HPP

#include "ble_conn_params.h"

namespace nrf_ble {
namespace nrf_conn_params {
void init(ble_gap_conn_params_t *gapParam);
}
}  // namespace nrf_ble

#endif