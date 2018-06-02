
#ifndef _NMS_FACE_INL_
#define _NMS_FACE_INL_

#include "net/nms/nmsClient.h"

#define MAX_CLIENT  256
static int modids[MAX_CLIENT] = {0};
static int nmodids = 0;

static int iarr_find(const int* arr, int len, int x) {
  int i, ret = -1;
  for (i=0; i<len; ++i) {
    if (arr[i]==x) {
      return i;
    }
  }
  return -1;
}
static int iarr_add(int* arr, int maxlen, int* len, int x, bool uniq) {
  int t = uniq ? iarr_find(arr, *len, x) : -1;
  if (t<0) {
    t = *len;
    ++(*len);
  }
  if (t<maxlen) {
    arr[t] = x;
  }
  return (t<maxlen);
}
static int iarr_del(int* arr, int* len, int x) {
  int t = iarr_find(arr, *len, x);
  if (t>=0) {
    --(*len);
    memmove(arr + t, arr + t+1, *len-t);
  }
  return (t>=0);
}

static int32_t nms_cmd_facedetect(ry_sock* s, int dst_modid, cJSON* pJsInput)
{
  char cmd[256];
  cJSON* pJsItem = cJSON_GetObjectItem(pJsInput, "msg");

  if (pJsItem && pJsItem->valuestring && 0 == memcmp(pJsItem->valuestring, "what", 4)) {
    _snprintf(cmd, 256, "{\"cmd\":\"test_nms_cmd\", \"msg\":\"%s\"}");
    NMSC_SendCmd(s, 18, dst_modid, cmd);
  }

  return 0;
}

static int32_t nms_cmd_findhuman(ry_sock* s, int dst_modid, cJSON* pJsInput)
{
  //char cmd[256];
  cJSON* pJsItem = cJSON_GetObjectItem(pJsInput, "data");
  if (pJsItem) {
    cJSON* pJsItem2 = cJSON_GetObjectItem(pJsItem, "subscribe");
    if (pJsItem2) {
      if (pJsItem2->valueint==0) {
        if (!iarr_del(modids, &nmodids, dst_modid)) {
          printf("iarr_del err %d\n", dst_modid);
        }
      }
      if (pJsItem2->valueint==1) {
        if (!iarr_add(modids, MAX_CLIENT, &nmodids, dst_modid, true)) {
          printf("iarr_add err %d\n", dst_modid);
        } 
      }
    }
  }

  return 0;
}

ry_sock* s = NULL;

int nms_face_ProtocolMsgProc(struct ry_sock* s, SMSGBUFF* pInMsg) {
  static SNetCmd RegistNetCmd[] = {
    {"facedetect", nms_cmd_facedetect, (void*)0},
    {"findhuman", nms_cmd_findhuman, (void*)0},
  };
  int dst_modid = 0;
  int errcode = runProtocolMsgProc(s, pInMsg, RegistNetCmd, countof(RegistNetCmd), &dst_modid);
  switch(errcode) {
  case ERR_MODOFFLINE:
    printf("ERR_MODOFFLINE\n");
    if (!iarr_del(modids, &nmodids, dst_modid)) {
      printf("iarr_del err %d\n", dst_modid);
    }
    break;

  }
  return 0;
}

int nms_face_init()
{
  int DMOD = 2;
#ifdef _WIN32
  cServerip = "10.1.100.87";
#else
  cServerip = "127.0.0.1";
#endif
  cServerport = "60000";
  //char* cmd = "{\"cmd\": \"test_nms_cmd\", \"data\":10}";
  char cmd[256];
  _snprintf(cmd, 256, "{\"cmd\":\"test_nms_cmd\", \"msg\":\"what you name?\"}");
  int datalen = strlen(cmd);
  s = NMSC_Init(18, DMOD, nms_face_ProtocolMsgProc, 0);
  int runing = 1;
  printf("%d %d\n", s->hostid, s->modid);
  return s!=NULL;
}
int nms_face_exit() {
  NMSC_Close(s);
  return 0;
}

int nms_face_findhuman_run(int ret) {
  if (s==NULL) {
    nms_face_init();
  }
  int i;
  char cmd[256];
  _snprintf(cmd, 256, "{\"cmd\":\"findhuman\", \"ret\":%d}", ret);
  for (i=0; i<nmodids; ++i) {
    NMSC_SendCmd(s, 18, modids[i], cmd);
  }
  return 0;
}

#endif // _NMS_FACE_INL_
