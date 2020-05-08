
#include <stdio.h>
#include <teavpn2/server/debugger.h>

void print_server_config(teavpn_server_config *config)
{
  #define DPRINT(A, B) \
    printf("  "#A" = "B"\n", A)

  printf("===== Config Debug Info =====\n");
  DPRINT(config->iface.dev, "%s");
  DPRINT(config->iface.mtu, "%d");
  DPRINT(config->iface.inet4, "%s");
  DPRINT(config->iface.inet4_bcmask, "%s");
  DPRINT(config->socket_type, "%d");
  DPRINT(config->socket.bind_addr, "%s");
  DPRINT(config->socket.bind_port, "%d");
  printf("================================\n");
  fflush(stdout);
}