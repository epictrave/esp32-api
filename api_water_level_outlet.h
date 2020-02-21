
// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef API_WATER_LEVEL_OUTLET_H
#define API_WATER_LEVEL_OUTLET_H

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
#include "water_level_common.h"

typedef struct WATER_LEVEL_OUTLET_URL_TAG {
  char url[256];
  char device_id[30];
} WaterLevelOutletUrl;

typedef struct WATER_LEVEL_OUTLET_DATA_TAG {
  int id;
  char water_level_name[20];
  int level;
  WaterLevelState state;
  bool water_level_watch;
  char outlet_device_id[20];
  char outlet_name[20];
  bool power;
} WaterLevelOutlet;

esp_err_t api_water_level_outlet_set_device_id(char *device_id);
esp_err_t api_water_level_outlet_set_url(char *url);
WaterLevelOutlet *api_water_level_outlet_get_data(size_t *num);

#ifdef __cplusplus
}
#endif

#endif /* API_WATER_LEVEL_OUTLET_H */