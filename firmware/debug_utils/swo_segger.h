// source: https://www.segger.com/products/debug-probes/j-link/tools/j-link-swo-viewer/

#ifndef _SWO_SEGGER_H
#define _SWO_SEGGER_H

#ifdef __cplusplus
extern "C" {
#endif

void SWO_PrintChar(char c);
void SWO_PrintString(const char *s);
void SWO_PrintStringLine(const char *s);

/*********************************************************************
 *
 *       Defines for Cortex-M debug unit
 */
#define ITM_STIM_U32 (*(volatile unsigned int *)0xE0000000)  // Stimulus Port 0 Register word acces
#define ITM_STIM_U8 (*(volatile char *)0xE0000000)           // Stimulus Port 0 Register byte acces
#define ITM_ENA (*(volatile unsigned int *)0xE0000E00)       // Trace Enable Ports Register
#define ITM_TCR (*(volatile unsigned int *)0xE0000E80)       // Trace control register

#ifdef __cplusplus
}
#endif

#endif