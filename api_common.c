// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "api_common.h"

#ifndef BIT0
#define BIT0 (0x1 << 0)
#endif

static const char *TAG = "api common";
static EventGroupHandle_t api_event_group;
static const int API_CONNECTED_BIT = BIT0;

void api_init_event_group(void) {
  if (api_event_group == NULL) {
    api_event_group = xEventGroupCreate();
    api_connection_close();
    ESP_LOGI(TAG, "API event group is initialized.");
  }
}

void api_wait_connection(void) {
  api_init_event_group();
  if (api_event_group == NULL) {
    ESP_LOGE(TAG, "API event group is not initialized.");
    return;
  }
  xEventGroupWaitBits(api_event_group, API_CONNECTED_BIT, false, true,
                      portMAX_DELAY);
}

void api_connection_open(void) {
  xEventGroupClearBits(api_event_group, API_CONNECTED_BIT);
}
void api_connection_close(void) {
  xEventGroupSetBits(api_event_group, API_CONNECTED_BIT);
}
