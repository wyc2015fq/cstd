
#include "nmsClient.h"


static int32_t test_nms_face_cmd(ry_sock* s, int dst_modid, cJSON* pJsInput)
{
  char cmd[256];
  cJSON* pJsItem = cJSON_GetObjectItem(pJsInput, "ret");

  if (pJsItem) {
    //_snprintf(cmd, 256, "{\"cmd\":\"test_nms_cmd\", \"msg\":\"%s\"}", (char*)s->user);
    //NMSC_SendCmd(s, 18, dst_modid, cmd);
    printf("ret %d\n", pJsItem->valueint);
  }

  return 0;
}

int test_nms_face_ProtocolMsgProc(struct ry_sock* s, SMSGBUFF* pInMsg) {
  static SNetCmd RegistNetCmd[] = {
    {"findhuman", test_nms_face_cmd, (void*)s->user},
  };
  int dst_modid = 0;
  int errcode = runProtocolMsgProc(s, pInMsg, RegistNetCmd, countof(RegistNetCmd), &dst_modid);
  switch(errcode) {
  case ERR_MODOFFLINE:
    printf("ERR_MODOFFLINE\n");
    break;
  }
  return 0;
}

int test_nms_face()
{
#ifdef _WIN32
  cServerip = "10.1.100.87";
  cServerip = "10.1.106.118";
#else
  cServerip = "127.0.0.1";
#endif
  cServerport = "60000";
  //char* cmd = "{\"cmd\": \"test_nms_cmd\", \"data\":10}";
  char cmd[256];
  ry_sock* s = NMSC_Init(18, 3, test_nms_face_ProtocolMsgProc, (void*)0);
  int runing = 1;
  int subscribe = 1;
  printf("%d %d\n", s->hostid, s->modid);

  while (runing) {
    char ch;
    ch = getchar();

    switch (ch) {
    case 'q':
      runing = 0;
      break;

    case 'd':
    case 's':
      //send_cmd(cmd);
      subscribe = 's'==ch;
      _snprintf(cmd, 256, "{\"cmd\":\"findhuman\", \"data\":{\"subscribe\":%d}}", subscribe);
      NMSC_SendCmd(s, 18, 2, cmd);
      break;

    default:
      break;
    }

    //SendData();
  }

  NMSC_Close(s);
  return 0;
}


