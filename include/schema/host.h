#ifndef HOST_H
#define HOST_H

#include "http.h"
#include "https.h"

typedef struct {

  https_t *https;
  http_t *http;
  char *addresses;
  char *name;
  char *root;

} host_t;

#endif