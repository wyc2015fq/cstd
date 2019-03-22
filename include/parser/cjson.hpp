
#ifndef _CJSON_HPP_
#define _CJSON_HPP_

#include "base64.h"
#include "cjson.h"
#include <string>
#include <vector>
using namespace std;

static cjson* cjson_SetObjectString(cjson* object, const char* name, const string& str) {
  return cjson_SetObjectString(object, name, str.c_str());
}
template <typename T>
static cjson* cjson_AddNumberArrayToObject(cjson* object, const char* string, const T* arr, int n) {
  int i;
  if (n > 0) {
    cjson * json_arr = cjson_AddArrayToObject(object, string);
    for (i = 0; i < n; ++i) {
      cjson_AddItemToArray(json_arr, cjson_CreateNumber(arr[i]));
    }
    return json_arr;
  }
  return NULL;
}
template <typename T>
static void cjson_AddNumberArrayToObject(cjson* object, const char* string, const vector<T>& arr) {
  cjson_AddNumberArrayToObject(object, string, arr.data(), arr.size());
}

static void cjson_AddStringArrayToObject(cjson* object, const char* string, const std::string* arr, int n) {
  int i;
  if (n > 0) {
    cjson * json_arr = cjson_AddArrayToObject(object, string);
    for (i = 0; i < n; ++i) {
      cjson_AddItemToArray(json_arr, cjson_CreateString(arr[i].c_str()));
    }
  }
  return;
}
static void cjson_AddStringArrayToObject(cjson* object, const char* string, const vector<std::string>& arr) {
  if (arr.size() > 0) {
    cjson_AddStringArrayToObject(object, string, &arr[0], (int)arr.size());
  }
}
static cjson* cjson_CreateBinaryData(const void* data, int size) {
  int len = 0;
  char* buf = (char*)malloc(size * 3);
  base64_encode((char*)data, size, buf, &len);
  buf[len] = 0;
  cjson* item = cjson_CreateString(buf);
  free(buf);
  return item;
}
static cjson* cjson_AddBinaryDataToObject(cjson* object, const char* string, const void* data, int size) {
  return cjson_AddItemToObject(object, string, cjson_CreateBinaryData(data, size));
}
static cjson* cjson_gcObjectItem(cjson* object, const char* string, int type = cjson_Object) {
  cjson* item = cjson_GetObjectItem(object, string);
  if (NULL == item) {
    //item = cjson_CreateObject();
    item = cjson_New_Item();
    if (item) {
      //item->type = cjson_Object;
      item->type = type;
    }
    cjson_AddItemToObject(object, string, item);
  }
  return item;
}
static cjson* cjson_pushArrayItem(cjson* object) {
  cjson *item = cjson_CreateObject();
  cjson_AddItemToArray(object, item);
  return item;
}
static cjson* cjson_gcArrayItem(cjson* object, int index) {
  int size = cjson_GetArraySize(object);
  for (; size <= index; ++size) {
      cjson_AddItemToArray(object, cjson_CreateObject());
  }
  cjson* item = cjson_GetArrayItem(object, index);
  assert(item);
  return item;
}
static int cjson_GetObjectBinaryDataSize(cjson* object, const char* string) {
  cjson* item = cjson_GetObjectItem(object, string);
  int len = 0;
  if (item) {
    len = (int)strlen(item->valuestring)*3/4 + 4;
  }
  return len;
}

static int cjson_GetBinaryData(cjson* item, void* data, int nbytes) {
  if (item) {
    int len = (int)strlen(item->valuestring);
    return base64_decode((char*)item->valuestring, len, data, nbytes);
  }
  return 0;
}

static int cjson_GetObjectBinaryData(cjson* object, const char* string, void* data, int nbytes) {
  cjson* item = cjson_GetObjectItem(object, string);
  return cjson_GetBinaryData(item, data, nbytes);
}
static char* cjson_GetObjectString(cjson* object, const char* name, const char* default_string) {
  if (object) {
    cjson* item = cjson_GetObjectItem(object, name);
    if (item) {
      default_string = item->valuestring;
    }
  }
  return (char*)default_string;
}

static double cjson_GetObjectNumber(cjson* object, const char* name, double default_double) {
  if (object) {
    cjson* item = cjson_GetObjectItem(object, name);
    if (item) {
      default_double = item->valuedouble;
    }
  }
  return default_double;
}
template <typename T>
static int cjson_GetNumberArray(cjson* array, T* arr, int size, double default_value) {
  int i = 0;
  if (array) {
    int n = cjson_GetArraySize(array);
    n = MIN(size, n);
    for (; i < n; ++i) {
      arr[i] = (T)cjson_GetArrayItem(array, i)->valuedouble;
    }
  }
  for (; i < size; ++i) {
    arr[i] = (T)default_value;
  }
  return size;
}
template <typename T>
static int cjson_GetObjectNumberArray(cjson* object, const char* name, T* arr, int n, double default_value) {
  cjson* item = cjson_GetObjectItem(object, name);
  return cjson_GetNumberArray(item, arr, n, default_value);
}

template <typename T>
static int cjson_GetNumberArray(cjson* array, vector<T>& arr) {
  int size = 0;
  if (array) {
    size = cjson_GetArraySize(array);
    arr.resize(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = (T)cjson_GetArrayItem(array, i)->valuedouble;
    }
  }
  return size;
}
template <typename T>
static int cjson_GetObjectNumberArray(cjson* object, const char* name, vector<T>& arr) {
  cjson* item = cjson_GetObjectItem(object, name);
  return cjson_GetNumberArray(item, arr);
}
template <typename T>
static cjson* cjson_CreateNumberArray(const T* arr, int len) {
  if (len > 0) {
    cjson* array = cjson_CreateArray();
    if (array) {
      for (int i = 0; i < len; ++i) {
        cjson_AddNumberToArray(array, arr[i]);
      }
    }
    return array;
  }
  return NULL;
}
template <typename T>
static cjson* cjson_CreateNumberArray(const vector<T>& arr) {
  if (arr.size() > 0) {
    cjson_CreateNumberArray(&arr[0], arr.size());
  }
  return NULL;
}
static int cjson_GetObjectStringArray(cjson* object, const char* name, vector<string>& arr) {
  cjson* item = cjson_GetObjectItem(object, name);
  int size = 0;
  if (item) {
    size = cjson_GetArraySize(item);
    arr.resize(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = cjson_GetArrayItem(item, i)->valuestring;
    }
  }
  return size;
}
static cjson* cjson_CreateStringArray(cjson* object, vector<string>& arr) {
  int size = arr.size();
  if (size > 0) {
    cjson* array = cjson_CreateArray();
    if (array) {
      for (int i = 0; i < arr.size(); ++i) {
        cjson_AddStringToArray(array, arr[i].c_str());
      }
    }
    return array;
  }
  return NULL;
}


#endif // _CJSON_HPP_
