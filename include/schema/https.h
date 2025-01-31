#ifndef HTTPS_H
#define HTTPS_H

#include <stdint.h>

typedef struct {

  uint16_t port;
  char *certificate;
  char *private_key;

} https_t;

#endif