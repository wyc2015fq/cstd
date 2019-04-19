# Android移植之WIFI - xqhrs232的专栏 - CSDN博客
2012年01月01日 15:32:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：749标签：[android																[output																[bt																[ant																[cmd																[access](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=ant&t=blog)](https://so.csdn.net/so/search/s.do?q=bt&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.chinaunix.net/space.php?uid=741742&do=blog&id=359299](http://blog.chinaunix.net/space.php?uid=741742&do=blog&id=359299)
**1． 前言 **
硬件平台： marvel 310 
       软件平台： maemo4 
       内核： 2.6.28 （经过 marvel 的移植） 
**2． 移植思想 **
1，  WIFI 模块本身和 cpu 之间的接口； 
       我们的模块和 cpu 之间的接口是 sdio 的，也就是说必须要先保证 SDIO 本身是工作的； 
2，  WIFI 模块本身的上电时序； 
       模块都有它自己的规律，所以必须要根据 spec 了解它本身的上电过程，严格遵守； 
3，  以太网接口的创建； 
       我们的 WIFI 模块本身是建立在 SDIO 口之上的，而对上都是提供以太网接口的，所以必须要保证这个接口以及创建； 
4，  特殊处理； 
       不同的模块都有它特别的地方，比如我们用的是 8686 和 compo 也就是说它和蓝牙共用天线，所以需要在初始化的时候做特殊的处理，发送特殊的命令，才能工作；
**3． 移植过程 **
1，  **sdio 本身是通过 gpio 口模拟的，所以需要对 gpio 口进行配置 **； 
       static mfp_cfg_t littleton_mmc3_pins[] = { 
       GPIO7_2_MMC3_DAT0, 
       GPIO8_2_MMC3_DAT1, 
       GPIO9_2_MMC3_DAT2, 
       GPIO10_2_MMC3_DAT3, 
       GPIO103_MMC3_CLK, 
       GPIO105_MMC3_CMD, 
       }; 
       void pxa3xx_enable_mmc3_pins(void) 
{ 
       pxa3xx_mfp_config(ARRAY_AND_SIZE(littleton_mmc3_pins)); 
} 
       在 littleton_init （）函数里面添加下面这一行： 
       pxa3xx_enable_mmc3_pins(); 
2，  **wifi 模块本身的初始化 **； 
#define MFP_WIFI_V18_ENABLE     (GPIO26_GPIO) 
#define MFP_WLAN_RESETN                  (GPIO99_GPIO) 
#define WIFI_WAKEUP_HOST          (GPIO104_GPIO) /*error must be changed*/ 
#define WLAN_ENABLE_PIN          26 
#define WLAN_RESET_PIN            99 
#define M200_B 
#ifdef M200_B 
#define BT_RESET_PIN                 EXT1_GPIO(1) 
#define BT_RESET_GPIO             (GPIO1_2_GPIO) 
#else 
#define BT_RESET_GPIO                  (GPIO6_2_GPIO) 
#define BT_RESET_PIN                 EXT1_GPIO(6) 
#endif 
static  mfp_cfg_t lin2008_wifibt_pins[] = { 
MFP_WIFI_V18_ENABLE, 
MFP_WLAN_RESETN, 
WIFI_WAKEUP_HOST,/*wakeup host*/ 
}; 
static  mfp_cfg_t lin2008_wifibt_pins2[] = { 
MFP_WIFI_V18_ENABLE, 
}; 
static int wifibt_power_status; 
int lin2008_poweron_wifibt_board(void) 
{ 
       if (!wifibt_power_status) { 
              pxa3xx_mfp_config(ARRAY_AND_SIZE(lin2008_wifibt_pins)); 
              gpio_direction_output(WLAN_ENABLE_PIN, 1); 
              gpio_direction_output(WLAN_RESET_PIN, 1); 
                  gpio_direction_output(BT_RESET_PIN,1); 
              mdelay(1); 
                  gpio_direction_output(BT_RESET_PIN, 0); 
                  gpio_direction_output(WLAN_RESET_PIN, 0); 
       // bt need > 5 ms to reset 
                  mdelay(5); 
                  gpio_direction_output(BT_RESET_PIN, 1); 
                  gpio_direction_output(WLAN_RESET_PIN, 1); 
              wifibt_power_status++; 
              return 0; 
       } 
       wifibt_power_status++; 
       return 1; 
} 
int lin2008_poweroff_wifibt_board(void) 
{ 
       wifibt_power_status--; 
       if (!wifibt_power_status) { 
              pxa3xx_mfp_config(ARRAY_AND_SIZE(lin2008_wifibt_pins2)); 
              gpio_direction_output(WLAN_ENABLE_PIN, 0); 
              return 0; 
       } 
       return 1; 
} 
**3， 以太网接口的创建 **
       这里在 android 平台上要做特殊的处理，也就是 firmware 的位置要放好，否则加载 firmware 始终不成功，那么以太网接口就不会被创建了；
       需要把 helper_sd.bin 放在 /etc/firmware/ 下面； 
       把 sd8686.bin 放在 /etc/firmware/mrvl 下面； 
**4， 特殊的处理 **
       对于 8686 模块需要做特殊的处理，在注册完以太网接口以后，需要添加下面这段话 : 
       { 
     #define BCA_CFG_NUM_OF_MODES          4 
     #define BCA_CFG_SINGLE_ANT_WITH_COEX      0 
     #define BCA_CFG_DUAL_ANT_WITH_COEX    1 
     #define BCA_CFG_SINGLE_ANT_FOR_BT_ONLY    2 
     #define BCA_CFG_MRVL_DEFAULT          3 
     #define BCA_CONFIG BCA_CFG_SINGLE_ANT_WITH_COEX 
     static u32 BCACfgTbl[BCA_CFG_NUM_OF_MODES][3] = { 
       //0xA5F0,     0xA58C,  0xA5A0 
       { 0xa027181c, 0x40214, 0xd24d}, //Mode 0: Single ANT with COEX enable 
       { 0xa027181c, 0x40211, 0xd24d}, //Mode 1: Dual ANT with COEX enable 
       { 0xa027181c, 0x40222, 0xd21c}, //Mode 2: single ANT for BT only 
       { 0xa027801d, 0x18000, 0xd21c}, //Mode 3: Marvell default 
     }; 
     wlan_offset_value    RegBuffer;     
     RegBuffer.offset = (0xA5F0); 
     RegBuffer.value  = BCACfgTbl[BCA_CONFIG][0]; 
      wlan_prepare_cmd(priv, 
                     HostCmd_CMD_MAC_REG_ACCESS, 
                     HostCmd_ACT_GEN_SET, 
                     HostCmd_OPTION_WAITFORRSP, 
                     0,&RegBuffer); 
     RegBuffer.offset =(0xA58C); 
     RegBuffer.value  = BCACfgTbl[BCA_CONFIG][1]; 
     wlan_prepare_cmd(priv, 
                     HostCmd_CMD_MAC_REG_ACCESS, 
                     HostCmd_ACT_GEN_SET, 
                     HostCmd_OPTION_WAITFORRSP, 
                     0,&RegBuffer); 
     RegBuffer.offset = (0xA5A0); 
     RegBuffer.value  = BCACfgTbl[BCA_CONFIG][2]; 
     wlan_prepare_cmd(priv, 
                     HostCmd_CMD_MAC_REG_ACCESS, 
                     HostCmd_ACT_GEN_SET, 
                     HostCmd_OPTION_WAITFORRSP, 
                     0,&RegBuffer); 
} 
基本上这样就可以工作了； 
**4．              其它问题 **
              Sdio 口默认是用的 3.2V 这里需要修改相关的地方强制设成是 1.8V ，否则 SDIO 接口将无法工作； 
              Android 平台上需要移植一些上层软件才能验证，比如 iwconfig ， iwlist ， wpa_supplicant 等。
       测试步骤： 
              iwlist mlan0 scan 
              如果能扫描到 AP 那么证明基本已经 OK 了； 
              wpa_supplicant -Dwext –imlan0 -c/etc/wpa.conf & 
              dhcpcd mlan0 ； 
              这样的话，就可以 ping 通 IP 地址了； 
**5． 常见问题 **
1，  WIFI 驱动注册 sdio 驱动的时候出错，也就是 probe 函数未被调用？ 
       需要检查 sdio 设备本身是否被检测到，内核是通过发送特定的命令来检测是 sdio 还是 SD 卡还是 mmc 的， 
       在 mmc_rescan 函数里面可以看到 sdio 设备的加载过程，有可能就是你的电压设置不对； 
2，  以太网接口一直出不来或者说 firmware 加载失败？ 
       这里需要确保 firmware 是否正确被加载，很可能是它找不到对应的 firmware ，可以通过添加打印信息看看，它的路径到底是在什么地方，对于 wifi 来说 helper_sd.bin 需要放在 /etc/firmware/ 下面，而 sd8686.bin 需要放在 /etc/firmware/mrvl 下面；
3，  一切正常，但是调用 iwlist mlan0 scan 的时候，扫描不到任何结果？ 
       这时候需要做特殊的处理，比如共存代码的设置，是否正常等，通常可以判断有没有中断上来，以此区分到底是模块和 cpu 的连接问题还是模块本身的设置问题；
4，  工作一段时间后不正常 
       这个有可能是蓝牙模块需要进行初始化，因为它们是共用天线，有时候不起蓝牙模块对应的 40M 时钟就不起来； 
**6． **后记 
       作者 :wylhistory 
       联系方式： wylhistory@gmail.com 
