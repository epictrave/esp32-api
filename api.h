// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef API_H
#define API_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdlib.h"
#include "string.h"

#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include "parson.h"

typedef struct DEVICE_CONNECTION_TAG {
  char url[256];
  char device_id[30];
} DeviceConnection;

esp_err_t api_set_device_connection_device_id(char *device_id);
esp_err_t api_set_device_connection_url(char *url);
bool api_get_device_connection(void);
#endif /* API_H */