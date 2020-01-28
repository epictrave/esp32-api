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

bool api_get_device_connection(char *device_id);
#endif /* API_H */