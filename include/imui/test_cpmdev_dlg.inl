int test_cpmdev_dlg()
{
  GETGUIIO2();
  static float v1[] = {3, 1};
  static int dev_coy = 0;
  static int dev_sel = -1;
  static int dev_hotid = -1;
  static vstr_t dev[1] = {0};
  static vstr_t record[1] = {0};
  static vstr_t log[1] = {0};
  static vstr_t image[1] = {0};
  static char edit_upath[256] = {0};
  static char ipaddress_s[32] = {0};
  static char ipaddress_addr[32] = {0};
  static char ipadress[32] = {0};
  static int RootCACHK = 0;
  static int check_ssl = 0;
  char* aa = edit_upath + 1;
  int n = 50;
  int i;
  set_layout_flags(0);
  if (dev->n != n) {
    char buf[256] = {0};
    strv_setsize(dev, n);
    for (i = 0; i < n; ++i) {
      _snprintf(buf, 256, "µÚ%dÐÐ|aaaa %d|bbbb %d", i, i, i);
      str_setstr(dev->v + i, buf, -1);
    }
  }
  if (linearlayout_begin(NULL, false, 1, countof(v1), v1)) {
    static float v2[] = {1, 3};
    if (linearlayout_begin(NULL, true, 1, countof(v2), v2)) {
      if (autolayout_begin("Device's Information")) {
        set_layout_flags(LF_alighParentBottom);
        if (autolayout_begin("U-Disk")) {
          set_layout_flags(LF_alighParentLeft | LF_alighParentTop);
          edit_ctrl("Data Path", edit_upath, countof(edit_upath));
          button_ctrl(aa++, "Browse");
          button_ctrl(aa++, "Connect");
          linearlayout_end();
        }
        if (autolayout_begin("Network")) {
          set_layout_flags(LF_alighParentLeft | LF_alighParentTop);
          edit_ctrl("Service IP", ipaddress_s, countof(ipaddress_s));
          if (button_ctrl(aa++, "Connect")) {
            // printf("Connect\n");
          }
          button_ctrl(aa++, "DisConnect");
          edit_ctrl("Area addess", ipaddress_addr, countof(ipaddress_addr));
          cbox_ctrl("Root CA", &RootCACHK);
          button_ctrl(aa++, "Upload CA");
          button_ctrl(aa++, "Search");
          edit_ctrl("Device IP", ipadress, countof(ipadress));
          cbox_ctrl("TLS ENCRYPT", &check_ssl);
          button_ctrl(aa++, "Connect");
          button_ctrl(aa++, "DisConnect");
          linearlayout_end();
        }
        set_layout_flags(LF_alighParentTop);
        if (1) {
          static const char* s_dev_column[] = {"IP", "Device Type", "Upper limit of the number"};
          static int width[countof(s_dev_column)] = {0};
          listheader(countof(s_dev_column), s_dev_column, width);
          if (strlist_ctrl(dev, countof(s_dev_column), width, &dev_coy, &dev_sel, &dev_hotid)) {
            printf("selected %d \n", dev_sel);
          }
        }
        linearlayout_end();
      }
      if (1) {
        static float v3[] = {3, 1};
        if (linearlayout_begin("Device's Record Information", true, 1, countof(v3), v3)) {
          static float v4[] = {1, 1, 1};
          if (linearlayout_begin(NULL, false, 0, countof(v4), v4)) {
            if (1) {
              static const char* record_column[] = {"Record-ID", "User-ID", "Record type", "Verified mode", "Record source"};
              static int width[countof(record_column)] = {0};
              static int record_offy = 0;
              static int record_sel = 0;
              static int record_hotid = 0;
              listheader(countof(record_column), record_column, width);
              if (strlist_ctrl(record, countof(record_column), width, &record_offy, &record_sel, &record_hotid)) {
                printf("selected %d \n", dev_sel);
              }
            }
            if (1) {
              static const char* log_column[] = {"Log-ID", "Type", "Operator", "By the operating", "Time"};
              static int width[countof(log_column)] = {0};
              static int log_offy = 0;
              static int log_sel = 0;
              static int log_hotid = 0;
              listheader(countof(log_column), log_column, width);
              if (strlist_ctrl(log, countof(log_column), width, &log_offy, &log_sel, &log_hotid)) {
                printf("selected %d \n", dev_sel);
              }
            }
            if (1) {
              static const char* image_column[] = {"Serial number", "User-ID", "Time"};
              static int width[countof(image_column)] = {0};
              static int image_offy = 0;
              static int image_sel = 0;
              static int image_hotid = 0;
              listheader(countof(image_column), image_column, width);
              if (strlist_ctrl(image, countof(image_column), width, &image_offy, &image_sel, &image_hotid)) {
                printf("selected %d \n", dev_sel);
              }
            }
            linearlayout_end();
          }
          linearlayout_end();
        }
      }
      linearlayout_end();
    }
    if (1) {
      static int check_local = 0;
      static int check_dsp = 0;
      static int check_user_ex = 0;
      if (autolayout_begin("User information")) {
        if (autolayout_begin(NULL)) {
          set_layout_flags(LF_alighParentLeft | LF_alighParentTop);
          if (autolayout_begin(NULL)) {
            set_layout_flags(LF_alighParentLeft);
            button_ctrl(aa++, "New");
            button_ctrl(aa++, "Alter");
            button_ctrl(aa++, "Delete");
            cbox_ctrl("local", &check_local);
            cbox_ctrl("DSP", &check_dsp);
            cbox_ctrl("Batch", &check_user_ex);
            linearlayout_end();
          }
          if (autolayout_begin(NULL)) {
            set_layout_flags(LF_alighParentLeft);
            button_ctrl(aa++, "Time Group");
            button_ctrl(aa++, "Emergency unlocking");
            button_ctrl(aa++, "Remote Open");
            button_ctrl(aa++, "Permission");
            button_ctrl(aa++, "Emergency locking");
            linearlayout_end();
          }
          linearlayout_end();
        }
        if (1) {
          static const char* image_column[] = {"Serial number", "User-ID", "Time"};
          static int width[countof(image_column)] = {0};
          static int image_offy = 0;
          static int image_sel = 0;
          static int image_hotid = 0;
          listheader(countof(image_column), image_column, width);
          if (strlist_ctrl(image, countof(image_column), width, &image_offy, &image_sel, &image_hotid)) {
            printf("selected %d \n", dev_sel);
          }
        }
        linearlayout_end();
      }
    }
    linearlayout_end();
  }
  return 0;
}

