
#include "imgui/imgui.inl"
#include "imgui/ctrl.inl"

#define BAUDRATEDEF_DEF(BAUDRATEDEF) \
  BAUDRATEDEF(110) \
  BAUDRATEDEF(300) \
  BAUDRATEDEF(600) \
  BAUDRATEDEF(1200) \
  BAUDRATEDEF(2400) \
  BAUDRATEDEF(4800) \
  BAUDRATEDEF(9600) \
  BAUDRATEDEF(14400) \
  BAUDRATEDEF(19200) \
  BAUDRATEDEF(38400) \
  BAUDRATEDEF(56000) \
  BAUDRATEDEF(57600) \
BAUDRATEDEF(115200)

int test_comportui_ctrl(ctrl_t* c) {
  static ctrl_t cc[10] = {0};
  int i = 0;
  ctrl_t* c_send = cc + i++;
  ctrl_t* c_clear = cc + i++;
  ctrl_t* c_hex = cc + i++;
  ctrl_t* c_devname = cc + i++;
  ctrl_t* c_DataBits = cc + i++;
  ctrl_t* c_Parity = cc + i++;
  ctrl_t* c_StopBits = cc + i++;
  ctrl_t* c_BaudRate = cc + i++;
  IRECT rc = c->rc, rcL;
  static dev_desc m_listComPort[8] = {0};
  static const int BaudRates[] = {
#define BAUDRATEDEF(x)   x,
    BAUDRATEDEF_DEF(BAUDRATEDEF)
#undef BAUDRATEDEF
  };
  static const char* s_BaudRate = "|"
#define BAUDRATEDEF(x)   #x "|"
    BAUDRATEDEF_DEF(BAUDRATEDEF)
#undef BAUDRATEDEF
    ;
  static int inited = 0;
  static int ncom = 0;
  static int idevname = 0;
  static int iBaudRate = 100;
  static int iDataBits = 0;
  static int iParity = 0;
  static int iStopBits = 0;
  static int ihex = 0;
  if (!inited) {
    inited = 1;
    ncom = sys_enum_comport(m_listComPort, countof(m_listComPort));
  }
  color_background_ctrl(rc, _RGB(122, 122, 122), _RGB(222, 222, 222), 1, 1);
  iRectCutL(&rc, 200, 2, &rcL);
  flexboxlay(rcL, countof(cc), cc, 0, flex_column|flex_wrap);
  button_ctrl(c_send, "发送");
  button_ctrl(c_clear, "清除");
  cbox_ctrl(c_hex, "16进制", &ihex);
  
  rbox_ctrl3(c_devname, "串口", ncom, m_listComPort->name, sizeof(*m_listComPort), &idevname);
  rbox_ctrl(c_BaudRate, "波特率", s_BaudRate, &iBaudRate);
  rbox_ctrl(c_DataBits, "数据位", "|8|7|5|6|", &iDataBits);
  rbox_ctrl(c_Parity, "校验位", "|NO|ODD|EVEN|MASK|SPACE|", &iParity);
  rbox_ctrl(c_StopBits, "停止位", "|1|1.5|2|", &iStopBits);
  return 0;
}

int test_comportui() {
  ctrl_t* c = NULL;
  for (; c = host_wait(100);) {
    test_comportui_ctrl(c);
  }
}