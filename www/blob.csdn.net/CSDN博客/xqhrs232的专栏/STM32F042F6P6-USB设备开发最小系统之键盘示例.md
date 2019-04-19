# STM32F042F6P6-USB设备开发最小系统之键盘示例 - xqhrs232的专栏 - CSDN博客
2018年02月08日 23:17:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1454
原文地址::[http://www.stm32cube.com/article/143](http://www.stm32cube.com/article/143)
![](https://img-blog.csdn.net/20180208231736529?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHFocnMyMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最常见的USB设备要数鼠标和键盘了，STM32F042F6P6这块小板（这里可购买，留言有优惠：[购买链接](https://item.taobao.com/item.htm?spm=a230r.1.14.28.qIrUHl&id=550650264987&ns=1&abbucket=13#detail)）的鼠标示例已经写了，现在来写下模拟键盘的示例.
不多说，开始配置，打开STM32CubeMX，选择STM32F042F6P6芯片，开启pin脚映射，打开USB设备，选择USB设备为HID，设置按键和LED对应引脚的输入输出模式：
![](https://hcabba.dm2301.livefilestore.com/y4m757YERmMQrE0c4AVMsyl7jfBe3R8zPrwD4IrF_hTjmym7tRWVRK2RFZvHJRbQnhOk12sx0QP-fqyvn1NDhFnV-XBHkb7cI8bE9j5K8AqHmUv_xNflkHXRiImpSzN6irQCFlyFxcdWLtttYiiQa7qI4XUVALSD6Lp02FlLh_65rvaBZ_CZYuNhaIc7knWCxBCLD1Azy54HqQhuNo_GsBmNw?width=1111&height=803&cropmode=none)
配置按键和LED引脚的状态及备注名：
![](https://imagba.dm2301.livefilestore.com/y4mTbeB-LCHyHuk9fjp2DA2NDfv26DE3t7iNhwU1yASvVCmE8KpkvGWRZ-gDEY7f4wjxW8wV_FuukkMocterVKHSS50mcOJvLtyfHlpEZbq0AzdL5_9-8ICp6CyiPxb4O8XIiVNGXYXNHUdsx19-Sa-nn-2Eew5-JJBtDzz_gVDaZZdlLoqdYD1VTTuEOle2a9pJVssAKv-C6JNnqYPbAmJgQ?width=716&height=639&cropmode=none)
可以在此修改USB设备名称、厂家、PID\VID等：
![](https://hcz8ba.dm2301.livefilestore.com/y4mbrSXKql4X-bxuJTuXepgMoCtyxELtA-LFKv5HFR0AcZB0b8DfR88tqEMkPRxI1mHSw4p7xiHpEaQjIimtDyoujC8jqadlZvQpGgLLfW5KzsbkusFhVNaWL8F3tw67PMTJCOH4d07V7pnQStTHxHY4uLEkzVe0VotseeiGrlgkxVrH985ZSPB3t8Tx96bv-A3L-C0SbEK2hhf4-kTJfY1Ow?width=1099&height=752&cropmode=none)
配置工程输出参数，然后生成并打开工程：
![](https://hszpba.dm2301.livefilestore.com/y4m7KKfAGIxVVRmeoZ98nGFU1lBeEdfOGMQuMVYMC3TsfgJugkH3lOGEFfbnUYHeM2XZBRrj9k_-Lz8PWcqogMgZ1vmbSlBGVDnJDXLcVd9XUtLpHVMLROjUNqfuNbNlnP7Tjycx7wpgbClxC5qWf8pS5rkvOOTv9vRm-PSYcahFPD7YHoBTD51--KKTszffMSDZSrzTdHic7z62gjcu5VcZg?width=1044&height=781&cropmode=none)
按图中所示打开usbd_hid.c文件，找到HID_MOUSE_ReportDesc数组定义处（默认生产HID设备为Mouse，这里数组名不影响，只要里面的描述符是键盘的就行），修改成如下数据：
 0x05, 0x01, // USAGE_PAGE (Generic Desktop) //63
 0x09, 0x06, // USAGE (Keyboard)
 0xa1, 0x01, // COLLECTION (Application)
 0x05, 0x07, // USAGE_PAGE (Keyboard)
 0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
 0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
 0x15, 0x00, // LOGICAL_MINIMUM (0)
 0x25, 0x01, // LOGICAL_MAXIMUM (1)
 0x75, 0x01, // REPORT_SIZE (1)
 0x95, 0x08, // REPORT_COUNT (8)
 0x81, 0x02, // INPUT (Data,Var,Abs)
 0x95, 0x01, // REPORT_COUNT (1)
 0x75, 0x08, // REPORT_SIZE (8)
 0x81, 0x03, // INPUT (Cnst,Var,Abs)
 0x95, 0x05, // REPORT_COUNT (5)
 0x75, 0x01, // REPORT_SIZE (1)
 0x05, 0x08, // USAGE_PAGE (LEDs)
 0x19, 0x01, // USAGE_MINIMUM (Num Lock)
 0x29, 0x05, // USAGE_MAXIMUM (Kana)
 0x91, 0x02, // OUTPUT (Data,Var,Abs)
 0x95, 0x01, // REPORT_COUNT (1)
 0x75, 0x03, // REPORT_SIZE (3)
 0x91, 0x03, // OUTPUT (Cnst,Var,Abs)
 0x95, 0x06, // REPORT_COUNT (6)
 0x75, 0x08, // REPORT_SIZE (8)
 0x15, 0x00, // LOGICAL_MINIMUM (0)
 0x25, 0x65, // LOGICAL_MAXIMUM (101)
 0x05, 0x07, // USAGE_PAGE (Keyboard)
 0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
 0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
 0x81, 0x00, // INPUT (Data,Ary,Abs)
 0xc0, // END_COLLECTION
![](https://hcadba.dm2301.livefilestore.com/y4m1uWW9G4ffSRBS_tEERwaRXT5afLx6Bv3WSWOADkbJKM_MxCuJjNmqYA_SPoTIJkwJLNs4ME0LM3hso9FTaR7pbc8HBxr54wiVVsF6aB98cavdjiiHcqvQoOmQ8xJxfWlnNEY1Nokcba3SK-T0-6bt2u7xT5r2anJpOc0WiHgvHmI9v6l3umdWQBN_uKWCOmGyca3YSN1dDpFxn9PU-6fig?width=1239&height=927&cropmode=none)
再打开usbd_hid.h文件，修改HID_MOUSE_REPORT_DESC_SIZE的值为63.
![](https://hcz9ba.dm2301.livefilestore.com/y4mN1UeSJpBqNA6_lqiVvRkpRS5_VbdSrPblO13KXQVsm6YXk6lKLnMmbYVhtVPCm26cekpDqKTAOFzVamtq-TO5sb5Krl2qn4UEeqXmuThXv8ll8t5EuGko-5kpVJr4iap_xobI2esKXuaY_nQHCl5pynxIi5s59W7HTdPxpKQiVzWN1St6nVal0Z6CLq7B-kdvtnKd45uBZRmKXMbDX7zmg?width=968&height=825&cropmode=none)
编译工程，下载到板子上，插上USB线连接到电脑上，是不是识别出为键盘设备了呢？
![](https://hczqba.dm2301.livefilestore.com/y4mUd-YvgKFn6a0zbq6M8tQdH3nh6UrK4ypxDHrr4xpWoir662EX62DOU0ukFCLutIMD0y1V_1lcf_19Aswx_dEWmqkpYYVBBSMGdiRPsyyDHUziSP_-Nb9AzAfy6hZRJXJvc0F7YMopwc-YpNGzlVFs8MCt3ery1mxFdloSASVo0UYlna4lc4O41iqaLsjTdBw9vhV3FIzFlpt6KWElHPFAQ?width=994&height=616&cropmode=none)
继续修改main.c文件，添加头文件
#include "usbd_hid.h"
![](https://imahba.dm2301.livefilestore.com/y4m-ByNLpd--nyJCQ1g_YBL72fvvYQjqtDXJa23uCNbQQthjRqkZwdPmYVLMdGXLe7A3g3lW3BYFyGOSdnFVzWrMVIa33ewpHxsdGitm5BZ85n6BebjuJQSCEdgc470zbzQW9fPhci01duKu65GLf_T2S9jrZmpl7ln-46DixuyzXNRHZzsKqZ2huzBWEIXXoERsefVIfHQNmmCrYzGkdFHaA?width=814&height=586&cropmode=none)
添加一个数组变量，用于传输键盘参数的，byte0是传控制键；byte1是保留键，不用改；byte3~byte7都可以存放传输的按键值
 /*
 * buffer[0] - bit0: Left CTRL
 *           -bit1: Left SHIFT
 *           -bit2: Left ALT
 *           -bit3: Left GUI
 *           -bit4: Right CTRL
 *           -bit5: Right SHIFT
 *           -bit6: Right ALT
 *           -bit7: Right GUI 
 * buffer[1] - Padding = Always 0x00
 * buffer[2] - Key 1
 * buffer[3] - Key 2
 * buffer[4] - Key 3
 * buffer[5] - Key 4
 * buffer[6] - Key 5
 * buffer[7] - Key 6
 */
 uint8_t buffer[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
![](https://hcacba.dm2301.livefilestore.com/y4m9r5YLEUqxbQhTLWVtb4hG2KYdKmIw_eLX1zvIuetNUD__vRIy8Yyv7Jx93MEZ5F-FYzz-Wx-yghPXiftZjNgiRrH0D4P2-5iNWmsBhdJ5vNMqea7rbC2rLasVtddOP4wA6NOfHo4LfytRSpKzP5UdzocKgLLJ9hqUHgn04TxZ1yMqhB-9gyU0POSAPxxT_rPEAylgKjJBpWrXITsZvt6ig?width=946&height=678&cropmode=none)
添加按键检测及传输键值到电脑的代码：
 if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0)
 {
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
 
  buffer[0] = 0x02; //shift
  buffer[2] = 0x04; // a
  USBD_HID_SendReport(&hUsbDeviceFS, buffer, 8); //send
  HAL_Delay(15); //delay
 
  buffer[0] = 0x00;
  buffer[2] = 0x00;
  USBD_HID_SendReport(&hUsbDeviceFS, buffer, 8);
  HAL_Delay(15);
 
  while(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0)
  HAL_Delay(15);
 }
 else
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
![](https://hcaaba.dm2301.livefilestore.com/y4mqtjObdM85_GoBBBoP88tdu2mgLeSC0RiWL4z1wvKYDLUMWu5NqIv2syksteLOTuHkuhfptfD70n2tMYZnEVZmPWjO7BNVf2Sm4DI_y5_s6EMd0nGpTe-Ona5NnvzXWgqjcjJtrkhzqriShnYX4u65RmmEainRIQL7iRtKCgZl_UC2vCObws3kcZnqyT018xv9PKAjTFfKmUgkSif9M4blQ?width=862&height=548&cropmode=none)
对应键值可和此文件中的HID Usage ID对应，是16进制（如字符‘a’对应键值为0x04）,pdf文件在附件中有（![](http://www.stm32cube.com/static/js/ueditor/dialogs/attachment/fileTypeImages/icon_pdf.gif)[USB_HIDtoKBScanCodeTranslationTable.pdf](http://www.stm32cube.com/ueditor/php/upload/file/20170603/1496419825124523.pdf)）：
![](https://hcahba.dm2301.livefilestore.com/y4muTiZc01hUM2ge8uVMhBKvOOJyDmNUAzTH_4oYK8Cx88NEm8F_OuVgrd5sPxCkJEyvv-uLfprkcfrT5A__FeHLeh-BwfAPE-TEPw-JPHKtZ_x7UCRncujUsJQhQ6KfgF1BPExRsDuaWICFzKC1sOlXX_wv7ePNiarNtIJyKxvBtF5nVojePl8bfQZHYYKEc6JMkL_mZAJRWpJ_vnidUz4Cg?width=1164&height=854&cropmode=none)
再编译文件下载到板子上，试试按下按键时是不是在电脑上就输入了一个大写的‘A’呢？
![](https://hcagba.dm2301.livefilestore.com/y4mKID5mlBeinj8w0SI9HchPC_aZK0vDhr6qtH3CxQRZIEBGRjmPrmt99n-OKDmwXDzXq-z7rHJBQmaAT_jl0iLQPqPo_N0G5SAi6K4Nw3UCTlWCPazozxvZSAPJVMiYwPZZBC_dZgghRZendRxuDnl6Ks8kli5i3TX2MbSNE9mY2hxjV-b2g8Y5IAvEtPRGAhlXh-v3WODxxgTGOmd_2FOJA?width=943&height=568&cropmode=none)
