
#ifndef _CJSON_HPP_
#define _CJSON_HPP_

#include "base64.h"
#include "cJSON.h"
#include <string>
#include <vector>
using namespace std;
typedef cJSON CJSON;

template <typename T>
void cJSON_AddNumberArrayToObject(cJSON* object, const char* string, const T* arr, int n) {
  int i;
  if (n > 0) {
    cJSON * json_arr = cJSON_CreateArray();
    for (i = 0; i < n; ++i) {
      //cJSON_AddNumberToObject(json_layer, "num_output", convolution_param.num_output());
      cJSON_AddItemToArray(json_arr, cJSON_CreateNumber(arr[i]));
    }
    cJSON_AddItemToObject(object, string, json_arr);
  }
  return;
}
void cJSON_AddStringArrayToObject(cJSON* object, const char* string, const std::string* arr, int n) {
  int i;
  if (n > 0) {
    cJSON * json_arr = cJSON_CreateArray();
    for (i = 0; i < n; ++i) {
      //cJSON_AddNumberToObject(json_layer, "num_output", convolution_param.num_output());
      cJSON_AddItemToArray(json_arr, cJSON_CreateString(arr[i].c_str()));
    }
    cJSON_AddItemToObject(object, string, json_arr);
  }
  return;
}
void cJSON_AddStringArrayToObject(cJSON* object, const char* string, const vector<std::string>& arr) {
  if (arr.size() > 0) {
    cJSON_AddStringArrayToObject(object, string, &arr[0], arr.size());
  }
}

void cJSON_AddBinaryDataToObject(cJSON* object, const char* string, const void* data, int size) {
  char* buf = (char*)malloc(size * 3);
  int len = 0;
  base64_encode((char*)data, size, buf, &len);
  buf[len] = 0;
  cJSON_AddStringToObject(object, string, buf);
  free(buf);
}
CJSON* cJSON_gcObjectItem(cJSON* object, const char* string, int type = cJSON_Object) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, string);
  if (NULL == item) {
    //item = cJSON_CreateObject();
    item = cJSON_New_Item();
    if (item) {
      //item->type = cJSON_Object;
      item->type = type;
    }
    cJSON_AddItemToObject(object, string, item);
  }
  return item;
}
CJSON* cJSON_pushArrayItem(cJSON* object) {
  CJSON *item = cJSON_CreateObject();
  cJSON_AddItemToArray(object, item);
  return item;
}
CJSON* cJSON_gcArrayItem(cJSON* object, int index) {
  int size = cJSON_GetArraySize(object);
  for (; size <= index; ++size) {
      cJSON_AddItemToArray(object, cJSON_CreateArray());
  }
  CJSON* item = cJSON_GetArrayItem((cJSON*)object, index);
  assert(item);
  return item;
}
int cJSON_GetObjectBinaryDataSize(cJSON* object, const char* string) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, string);
  int len = 0;
  if (item) {
    len = strlen(item->valuestring)*3/4 + 4;
  }
  return len;
}
int cJSON_GetObjectBinaryData(cJSON* object, const char* string, void* data, int size) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, string);
  if (item) {
    int len = strlen(item->valuestring);
    base64_decode((char*)item->valuestring, len, data, &size);
  } else {
    size = 0;
  }
  return size;
}

int cJSON_GetObjectArraySize(const cJSON* object, const char* name) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, name);
  if (item) {
    return cJSON_GetArraySize(item);
  }
  return 0;
}

char* cJSON_GetObjectString(const cJSON* object, const char* name, const char* default_string) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, name);
  if (item) {
    default_string = item->valuestring;
  }
  return (char*)default_string;
}

double cJSON_GetObjectNumber(const cJSON* object, const char* name, double default_double) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, name);
  if (item) {
    default_double = item->valuedouble;
  }
  return default_double;
}
int cJSON_GetObjectInt(const cJSON* object, const char* name, int default_int) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, name);
  if (item) {
    default_int = item->valueint;
  }
  return default_int;
}
template <typename T>
int cJSON_GetNumberArray(cJSON* object, T* arr, int n, double default_value) {
  int i;
  int size = cJSON_GetArraySize(object);
  size = MIN(size, n);
  for (i = 0; i < size; ++i) {
    arr[i] = cJSON_GetArrayItem(object, i)->valuedouble;
  }
  for (; i < n; ++i) {
    arr[i] = default_value;
  }
  return size;
}

template <typename T>
int cJSON_GetObjectNumberArray(const cJSON* object, const char* name, T* arr, int n, double default_value) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, name);
  int i = 0;
  if (item) {
    i = cJSON_GetNumberArray(item, arr, n, default_value);
  }
  for (; i < n; ++i) {
    arr[i] = default_value;
  }
  return i;
}
template <typename T>
int cJSON_GetObjectNumberArray(cJSON* object, const char* name, vector<T>& arr) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, name);
  int size = 0;
  if (item) {
    size = cJSON_GetArraySize(item);
    arr.resize(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = (T)cJSON_GetArrayItem(item, i)->valuedouble;
    }
  }
  return size;
}
int cJSON_GetObjectStringArray(cJSON* object, const char* name, vector<string>& arr) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, name);
  int size = 0;
  if (item) {
    size = cJSON_GetArraySize(item);
    arr.resize(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = cJSON_GetArrayItem(item, i)->valuestring;
    }
  }
  return size;
}

#define CJSON_GETOBJECTENUM(object, name, default_enum, type)  (type)cJSON_GetObjectEnum(object, name, default_enum, type ##_Name, countof( type ##_Name ))
int cJSON_GetObjectEnum(const cJSON* object, const char* name, int default_enum, const char** enum_map, int enum_map_len) {
  CJSON* item = cJSON_GetObjectItem((cJSON*)object, name);
  if (item) {
    for (int i=0; i<enum_map_len; ++i) {
      if (0==strcmp(item->valuestring, enum_map[i])) {
        return i;
      }
    }
  }
  return default_enum;
}


#endif // _CJSON_HPP_
