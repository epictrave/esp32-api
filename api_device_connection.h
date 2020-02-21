// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef API_DEVICE_CONNECTION_H
#define API_DEVICE_CONNECTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"
#include "string.h"

#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include "api_common.h"
#include "parson.h"

typedef struct DEVICE_CONNECTION_URL_TAG {
  char url[256];
  char device_id[30];
} DeviceConnectionUrl;

esp_err_t api_device_connection_set_device_id(char *device_id);
esp_err_t api_device_connection_set_url(char *url);
bool api_device_connection_get_connection(void);

#ifdef __cplusplus
}
#endif

#endif /* API_DEVICE_CONNECTION_H */