
// 10.1.106.239 : 60000
//#include "cstd.h"
//#include "str.h"
#include "nmsClient.h"
//#include "lib\pthread-win32-master\pthread-win32.inl"


static int32_t test_nms_cmd(ry_sock* s, int dst_modid, cJSON* pJsInput)
{
  char cmd[256];
  cJSON* pJsItem = cJSON_GetObjectItem(pJsInput, "msg");

  if (pJsItem && pJsItem->valuestring && 0 == memcmp(pJsItem->valuestring, "what", 4)) {
    _snprintf(cmd, 256, "{\"cmd\":\"test_nms_cmd\", \"msg\":\"%s\"}", (char*)s->user);
    NMSC_SendCmd(s, 18, dst_modid, cmd);
  }

  return 0;
}

int test_nms_ProtocolMsgProc(struct ry_sock* s, SMSGBUFF* pInMsg) {
  static SNetCmd RegistNetCmd[] = {
    {"test_nms_cmd", test_nms_cmd, (void*)s->user},
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

int test_nms_impl(int src, int dst, const char* name)
{
  cServerip = "10.1.100.87";
  cServerport = "60000";
  //char* cmd = "{\"cmd\": \"test_nms_cmd\", \"data\":10}";
  char cmd[256];
  _snprintf(cmd, 256, "{\"cmd\":\"test_nms_cmd\", \"msg\":\"what you name?\"}", src, dst);
  ry_sock* s = NMSC_Init(18, src, test_nms_ProtocolMsgProc, (void*)name);
  int runing = 1;
  printf("%d %d\n", s->hostid, s->modid);

  while (runing) {
    char ch;
    ch = getchar();

    switch (ch) {
    case 'q':
      runing = 0;
      break;

    case 's':
      //send_cmd(cmd);
      NMSC_SendCmd(s, 18, dst, cmd);
      break;

    default:
      break;
    }

    //SendData();
  }

  NMSC_Close(s);
  return 0;
}

int test_nms() {
  test_nms_impl(3, 2, "333");
  return 0;
}

int test_nms2() {//CPCODE;
  test_nms_impl(2, 3, "222");
  return 0;
}
