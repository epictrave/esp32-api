// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "api.h"

extern const char ca_pem_start[] asm("_binary_ca_pem_start");
extern const char ca_pem_end[] asm("_binary_ca_pem_end");

static const char *TAG = "API";
const char buffer[10240];
esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
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
bool api_get_device_connection(char *device_id) {
  memset(buffer, 0, sizeof(buffer));

  if (device_id == NULL)
    return false;

  bool connection = false;
  char url[256] = "https://fn-iothub-centralkr-ig-dev01.azurewebsites.net/api/"
                  "CheckConnection?deviceId=";
  strncat(url, device_id, strlen(device_id));

  esp_http_client_config_t config = {
      .url = url,
      .event_handler = _http_event_handler,
      .cert_pem = ca_pem_start,
  };

  esp_http_client_handle_t client = esp_http_client_init(&config);
  esp_err_t err = esp_http_client_perform(client);

  if (err == ESP_OK) {
    ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d\r\ndata = %s",
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client), buffer);
    esp_http_client_cleanup(client);

    JSON_Value *root_value = json_parse_string(buffer);
    JSON_Object *root_object = json_value_get_object(root_value);
    if (json_object_get_value(root_object, "connection") != NULL) {
      connection = (bool)json_object_get_boolean(root_object, "connection");
    } else {
      connection = false;
    }
    json_value_free(root_value);
    return connection;
  } else {
    ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    esp_http_client_cleanup(client);
    return false;
  }
}