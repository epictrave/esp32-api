// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef API_COMMON_H
#define API_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

void api_wait_connection(void);
void api_connection_open(void);
void api_connection_close(void);

#ifdef __cplusplus
}
#endif

#endif /* API_COMMON_H */