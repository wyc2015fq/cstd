
#include "cjson.inl"

int json_parse_point(cJSON *pJsObject, int* px, int* py) {
  int ret = 0;
  cJSON *pJsItem;
  if (pJsItem = cJSON_GetObjectItem(pJsObject, "x")) {
    *px = pJsItem->valueint;
    ret++;
  }
  if (pJsItem = cJSON_GetObjectItem(pJsObject, "y")) {
    *py = pJsItem->valueint;
    ret++;
  }
  return 2==ret;
}

int faceplusplus_json_parse(const char* content, faceobj_t* fa) {
  int ret = 0;
  cJSON *pJsRoot = NULL, *pJsFace = NULL, *pJsFaceAttr = NULL, *pJsLandmark = NULL, *pJsObject = NULL, *pJsItem = NULL;
  memset(fa, 0, sizeof(*fa));
  if (pJsRoot = cJSON_Parse(content)) {
    if (pJsFace = cJSON_GetObjectItem(pJsRoot, "faces")) {
      int size = cJSON_GetArraySize(pJsFace);
      if (size>0) {
        pJsFace = pJsFace->child; // 
      }
      if (pJsLandmark = cJSON_GetObjectItem(pJsFace,"landmark")) {
        if (pJsObject = cJSON_GetObjectItem(pJsLandmark, "left_eye_center")) {
          ret += json_parse_point(pJsObject, &fa->leye.x, &fa->leye.y);
        }
        if (pJsObject = cJSON_GetObjectItem(pJsLandmark, "right_eye_center")) {
          ret += json_parse_point(pJsObject, &fa->reye.x, &fa->reye.y);
        }
      }
      if (pJsFaceAttr = cJSON_GetObjectItem(pJsFace,"attributes")) {
        if (pJsObject = cJSON_GetObjectItem(pJsFaceAttr, "glass")) {
          if (pJsItem = cJSON_GetObjectItem(pJsObject, "value")) {
            char* ss = pJsItem->valuestring;
            fa->glass = !!strcmp(pJsItem->valuestring, "None");
            ret++;
          }
        }
        if (pJsObject = cJSON_GetObjectItem(pJsFaceAttr, "smile")) {
          if (pJsItem = cJSON_GetObjectItem(pJsObject, "value")) {
            fa->smile = (pJsItem->valueint > 31);
            ret++;
          }
        }
        if (pJsObject = cJSON_GetObjectItem(pJsFaceAttr, "gender")) {
          if (pJsItem = cJSON_GetObjectItem(pJsObject, "value")) {
            fa->gender = !strcmp(pJsItem->valuestring, "Male");
            ret++;
          }
        }
        if (pJsObject = cJSON_GetObjectItem(pJsFaceAttr, "age")) {
          if (pJsItem = cJSON_GetObjectItem(pJsObject, "value")) {
            fa->age = pJsItem->valueint;
            ret++;
          }
        }
      }
    }
  }
  
  if (pJsRoot)
    cJSON_Delete(pJsRoot);
  fa->inited = 1;
  fa->event = 2;
  return ret>4;
}
