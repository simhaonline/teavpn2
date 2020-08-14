
#ifndef TEAVPN__GLOBAL__COMMON_H
#define TEAVPN__GLOBAL__COMMON_H

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

/* Socket communication type. */
enum socket_type {
  TEAVPN_SOCK_TCP = 1,
  TEAVPN_SOCK_UDP = 2
};




/* Virtual network interface configuration. */
typedef struct _teavpn_net {
  char        *dev;
  char        *inet4;
  char        *inet4_bcmask;
  uint32_t    mtu;
} teavpn_net;

int teavpn_iface_allocate(char *dev);



/**
 * Debug and error logger.
 */
int __teavpn_debug_log(const char *format, ...);
int __teavpn_error_log(const char *format, ...);

#ifndef GLOBAL_LOGGER_C
extern uint8_t __debug_log_level;
#endif

#define debug_log(LEVEL, ...)                \
  ( (__debug_log_level >= ((uint8_t)LEVEL))  \
    ? __teavpn_debug_log(__VA_ARGS__)        \
    : 0                                      \
  )

#define error_log(...) __teavpn_error_log(__VA_ARGS__)

#endif