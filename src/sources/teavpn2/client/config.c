
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <inih/ini.h>
#include <teavpn2/client/common.h>

static bool failed = false;

inline static int client_parser_handler(
  void *user,
  const char *section,
  const char *name,
  const char *value,
  int lineno
);


bool tvpn_client_load_config_file(char *file, client_cfg *config)
{
  int ret = ini_parse(file, client_parser_handler, config);

  if (ret < 0) {
    printf("File \"%s\" does not exist\n", file);
    return false;
  }

  if (failed) {
    printf("Error loading config file!\n");
    return false;
  }

  return true;
}


inline static int client_parser_handler(
  void *user,
  const char *section,
  const char *name,
  const char *value,
  int lineno
)
{
  client_cfg *config = (client_cfg *)user;

  #define RMATCH_S(STR) if (!strcmp(section, STR))
  #define RMATCH_N(STR) if (!strcmp(name, STR))

  RMATCH_S("iface") {

    RMATCH_N("dev") {
      config->iface.dev  = t_ar_strndup(value, 256);
    } else
    RMATCH_N("mtu") {
      config->iface.mtu = (uint16_t)atoi(value);
    } else {
      goto invalid_name;
    }

  } else
  RMATCH_S("socket") {

    RMATCH_N("server_addr") {
      config->sock.server_addr = t_ar_strndup(value, 256);
    } else
    RMATCH_N("server_port") {
      config->sock.server_port = (uint16_t)atoi(value);
    } else
    RMATCH_N("sock_type") {
      char targ[4];

      strncpy(targ, value, 3);
      targ[0] = (targ[0] >= 'A' && targ[0] <= 'Z') ? targ[0] + 32 : targ[0];
      targ[1] = (targ[1] >= 'A' && targ[1] <= 'Z') ? targ[1] + 32 : targ[1];
      targ[2] = (targ[2] >= 'A' && targ[2] <= 'Z') ? targ[2] + 32 : targ[2];
      targ[3] = '\0';

      if (!strcmp(targ, "tcp")) {
        config->sock.type = sock_tcp;
      } else
      if (!strcmp(targ, "udp")) {
        config->sock.type = sock_udp;
      } else {
        printf("Invalid socket type: \"%s\"\n", value);
        failed = true;
        return 0;
      }

    } else {
      goto invalid_name;
    }

  } else
  RMATCH_S("other") {
    config->data_dir = t_ar_strndup(value, 256);
  } else {
    printf("Invalid section \"%s\" on line %d\n", section, lineno);
    failed = true;
    return 0;
  }

  return 1;
invalid_name:
  printf("Invalid name: \"%s\" in section \"%s\" on line %d\n", name, section, lineno);
  failed = true;
  return 0;
}