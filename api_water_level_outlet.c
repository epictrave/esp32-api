// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "api_water_level_outlet.h"

extern const char ca_pem_start[] asm("_binary_ca_pem_start");
extern const char ca_pem_end[] asm("_binary_ca_pem_end");

static const char *TAG = "api water level outlet";
static char buffer[10240];
static WaterLevelOutletUrl water_level_outlet_url;
static WaterLevelOutlet *water_level_outlets = NULL;
static size_t water_level_outlet_num = 0;

static esp_err_t http_event_handler(esp_http_client_event_t *evt) {
  switch (evt->event_id) {
  case HTTP_EVENT_ERROR:
    ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
    break;
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADER_SENT:
    ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key,
             evt->header_value);
    break;
  case HTTP_EVENT_ON_DATA:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
    if (!esp_http_client_is_chunked_response(evt->client)) {
      // Write out data
      strncat(buffer, (char *)evt->data, evt->data_len);
    }
    break;
  case HTTP_EVENT_ON_FINISH:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
    break;
  }
  return ESP_OK;
}

esp_err_t api_water_level_outlet_set_device_id(char *device_id) {
  if (device_id == NULL) {
    ESP_LOGE(TAG, "Device id can not be null.");
    return ESP_ERR_INVALID_ARG;
  }
  memset(water_level_outlet_url.device_id, 0,
         sizeof(water_level_outlet_url.device_id));
  strncpy(water_level_outlet_url.device_id, device_id, strlen(device_id) + 1);
  ESP_LOGI(TAG, "Device id : %s.", water_level_outlet_url.device_id);
  return ESP_OK;
}

esp_err_t api_water_level_outlet_set_url(char *url) {
  if (url == NULL) {
    ESP_LOGE(TAG, "Url can not be null.");
    return ESP_ERR_INVALID_ARG;
  }
  memset(water_level_outlet_url.url, 0, sizeof(water_level_outlet_url.url));
  strncpy(water_level_outlet_url.url, url, strlen(url) + 1);
  ESP_LOGI(TAG, "Url : %s.", water_level_outlet_url.url);
  return ESP_OK;
}

void api_water_level_outlet_run() {

  api_wait_connection();
  api_connection_open();
  memset(buffer, 0, sizeof(buffer));

  if (water_level_outlet_url.url == NULL ||
      water_level_outlet_url.device_id == NULL)
    return false;

  char url[300];
  memset(url, 0, sizeof(url));
  strncat(url, water_level_outlet_url.url, strlen(water_level_outlet_url.url));
  strncat(url, "?deviceId=", strlen("?deviceId="));
  strncat(url, water_level_outlet_url.device_id,
          strlen(water_level_outlet_url.device_id));

  esp_http_client_config_t config = {
      .url = url,
      .event_handler = http_event_handler,
      .cert_pem = ca_pem_start,
  };

  esp_http_client_handle_t client = esp_http_client_init(&config);
  esp_err_t err = esp_http_client_perform(client);
  water_level_outlet_num = 0;

  if (err == ESP_OK) {
    ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d.",
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
    JSON_Value *root_value = json_parse_string(buffer);
    JSON_Object *root_object = json_value_get_object(root_value);

    if (json_object_get_value(root_object, "waterLevelOuters") != NULL) {
      JSON_Array *json_array =
          json_object_get_array(root_object, "waterLevelOuters");
      water_level_outlet_num = json_array_get_count(json_array);
      ESP_LOGI(TAG, "water level outlet num: %d", water_level_outlet_num);
      if (water_level_outlets != NULL) {
        free(water_level_outlets);
        water_level_outlets = NULL;
      }
      water_level_outlets = (WaterLevelOutlet *)malloc(
          sizeof(WaterLevelOutlet) * water_level_outlet_num);
      memset(water_level_outlets, 0,
             sizeof(WaterLevelOutlet) * water_level_outlet_num);

      for (int i = 0; i < water_level_outlet_num; i++) {

        JSON_Object *json_water_level_outlet =
            json_array_get_object(json_array, i);
        if (json_object_get_value(json_water_level_outlet, "id") != NULL) {
          water_level_outlets[i].id =
              (int)json_object_get_number(json_water_level_outlet, "id");
        }

        if (json_object_get_value(json_water_level_outlet, "waterLevelName") !=
            NULL) {
          const char *water_level_name =
              json_object_get_string(json_water_level_outlet, "waterLevelName");

          if (water_level_name != NULL) {
            strncpy(water_level_outlets[i].water_level_name, water_level_name,
                    strlen(water_level_name) + 1);
          }
        }

        if (json_object_get_value(json_water_level_outlet, "level") != NULL) {
          water_level_outlets[i].level =
              (int)json_object_get_number(json_water_level_outlet, "level");
        }

        if (json_object_get_value(json_water_level_outlet, "state") != NULL) {
          water_level_outlets[i].state =
              (WaterLevelState)json_object_get_number(json_water_level_outlet,
                                                      "state");
        }

        if (json_object_get_value(json_water_level_outlet, "waterLevelWatch") !=
            NULL) {
          bool water_level_watch =
              json_object_get_boolean(json_water_level_outlet,
                                      "waterLevelWatch") > 0
                  ? true
                  : false;
          water_level_outlets[i].water_level_watch = water_level_watch;
        }

        if (json_object_get_value(json_water_level_outlet, "outletDeviceId") !=
            NULL) {
          const char *outlet_device_id =
              json_object_get_string(json_water_level_outlet, "outletDeviceId");

          if (outlet_device_id != NULL) {
            strncpy(water_level_outlets[i].outlet_device_id, outlet_device_id,
                    strlen(outlet_device_id) + 1);
          }
        }

        if (json_object_get_value(json_water_level_outlet, "outletName") !=
            NULL) {
          const char *outlet_name =
              json_object_get_string(json_water_level_outlet, "outletName");

          if (outlet_name != NULL) {
            strncpy(water_level_outlets[i].outlet_name, outlet_name,
                    strlen(outlet_name) + 1);
          }
        }

        if (json_object_get_value(json_water_level_outlet, "power") != NULL) {
          bool power =
              json_object_get_boolean(json_water_level_outlet, "power") > 0
                  ? true
                  : false;
          water_level_outlets[i].power = power;
        }
      }
    }

    json_value_free(root_value);
  } else {
    ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
  }
  esp_http_client_cleanup(client);
  api_connection_close();
}

WaterLevelOutlet *api_water_level_outlet_get_data(size_t *num) {
  api_water_level_outlet_run();
  *num = water_level_outlet_num;
  return water_level_outlets;
}