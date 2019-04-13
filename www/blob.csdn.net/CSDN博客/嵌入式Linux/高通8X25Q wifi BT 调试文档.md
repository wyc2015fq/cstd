
# 高通8X25Q wifi BT 调试文档 - 嵌入式Linux - CSDN博客

2015年02月12日 08:37:08[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：3920


1、蓝牙调试
主要是蓝牙的I2C地址跟 地磁仪的I2C地址冲突，被地磁仪给占用了，改回来就好了。
--- a/android-lte/kernel/arch/arm/mach-msm/msm8x25/goso-msm7627a-bt.c
+++ b/android-lte/kernel/arch/arm/mach-msm/msm8x25/goso-msm7627a-bt.c
@@ -981,8 +981,7 @@ static struct marimba_platform_data marimba_pdata = {
static struct i2c_board_info bahama_devices[] = {
{
-       //I2C_BOARD_INFO("marimba", 0x0c),
-       I2C_BOARD_INFO("marimba", 0xff),
+       I2C_BOARD_INFO("marimba", 0x0c),
.platform_data = &marimba_pdata,
},
};

2、wifi调试
之前打开wifi 一直打印出下面的log.
<6>[  164.734984] [cpuid: 0] WLAN power-down success
<3>[  164.750889] [cpuid: 0] android_readwrite_file: ret=2
<3>[  164.751176] [cpuid: 0] android_readwrite_file: ret=2
<6>[  164.772464] [cpuid: 0] mmc2: card 0001 removed
<6>[  168.223351] [cpuid: 1] Calling CRDA to update world regulatory domain
<6>[  168.223491] [cpuid: 1] World regulatory domain updated:
<6>[  168.223501] [cpuid: 1]     (start_freq - end_freq @ bandwidth), (max_antenna_gain, max_eirp)
<6>[  168.223514] [cpuid: 1]     (2402000 KHz - 2472000 KHz @ 40000 KHz), (300 mBi, 2000 mBm)
<6>[  168.223526] [cpuid: 1]     (2457000 KHz - 2482000 KHz @ 20000 KHz), (300 mBi, 2000 mBm)
<6>[  168.223538] [cpuid: 1]     (2474000 KHz - 2494000 KHz @ 20000 KHz), (300 mBi, 2000 mBm)
<6>[  168.223548] [cpuid: 1]     (5140000 KHz - 5360000 KHz @ 40000 KHz), (N/A, 3000 mBm)
<6>[  168.223559] [cpuid: 1]     (5460000 KHz - 5860000 KHz @ 40000 KHz), (N/A, 3000 mBm)
<3>[  168.260969] [cpuid: 1] ath6kl_pm_probe () enter and will invoke power-up sequence
<6>[  168.262981] [cpuid: 1] WLAN power-up success
<3>[  168.263789] [cpuid: 1] android_readwrite_file: ret=2
<3>[  168.263829] [cpuid: 1] android_readwrite_file: ret=2
<4>[  169.269134] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x01 (3 bytes)
<4>[  169.291253] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x1a (5 bytes)
<4>[  169.301561] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x1b (8 bytes)
<4>[  169.303486] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x14 (0 bytes)
<4>[  169.313814] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x80 (1 bytes)
<4>[  169.314171] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x81 (1 bytes)
<4>[  169.314509] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x82 (1 bytes)
<6>[  169.314744] [cpuid: 0] mmc2: new high speed SDIO card at address 0001
<4>[  169.324446] [cpuid: 0] SDIO: Enabling device mmc2:0001:1...
<4>[  169.427058] [cpuid: 0] SDIO: Failed to enable device mmc2:0001:1
<3>[  169.427098] [cpuid: 0] ath6kl: Unable to enable sdio func: -62)
<3>[  169.427208] [cpuid: 0] ath6kl: Failed to init ath6kl core
<4>[  169.427271] [cpuid: 0] ath6kl_sdio: probe of mmc2:0001:1 failed with error -62
<3>[  171.288666] [cpuid: 0] timeout while waiting for init operation
<3>[  171.291416] [cpuid: 0] ath6kl_pm_remove () enter and will invoke power-down sequence
<6>[  171.294266] [cpuid: 0] WLAN power-down success
<3>[  171.309801] [cpuid: 0] android_readwrite_file: ret=2
<3>[  171.310098] [cpuid: 0] android_readwrite_file: ret=2
<6>[  171.332013] [cpuid: 0] mmc2: card 0001 removed
<6>[  171.909398] [cpuid: 0] rmt_storage_event_write_iovec_cb: write iovec callback received
<6>[  172.407964] [cpuid: 0] rmt_storage_ioctl: send status ioctl
从ath6kl: Unable to enable sdio func 可以看出来是 SDIO初始化没有成功，高通打电话的回复是，我们代码中设置的SDIO模式和接口跟硬件连接对应不上。
please check code
kernel/arch/arm/mach-msm/board-msm7x27.c (does your board use this file,please confirm it)
check sdcc related configuration. eg msm7x2x_sdc2_data
然后硬件把主板上面WIFI的晶振去掉后，使用8X25Q自带的晶振，后面打出下面的LOG.
<4>[  253.786184] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x01 (3 bytes)
<4>[  253.804823] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x1a (5 bytes)
<4>[  253.812796] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x1b (8 bytes)
<4>[  253.814494] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x14 (0 bytes)
<4>[  253.819098] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x80 (1 bytes)
<4>[  253.819209] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x81 (1 bytes)
<4>[  253.819301] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x82 (1 bytes)
<6>[  253.819361] [cpuid: 0] mmc2: new high speed SDIO card at address 0001
<4>[  253.820496] [cpuid: 0] SDIO: Enabling device mmc2:0001:1...
<4>[  253.825038] [cpuid: 0] SDIO: Enabled device mmc2:0001:1
<3>[  254.836579] [cpuid: 0] ath6kl: bmi communication timeout
<3>[  254.836593] [cpuid: 0] ath6kl: Unable to send get target info: -110
<3>[  254.836718] [cpuid: 0] ath6kl: Failed to init ath6kl core
<4>[  254.836779] [cpuid: 0] ath6kl_sdio: probe of mmc2:0001:1 failed with error -110
<3>[  255.408479] [cpuid: 0] [audmgr.c:process_audmgr_callback] rpc DEVICE_CONFIG
<6>[  255.424401] [cpuid: 0] ping all the CPU from CPU0
<3>[  256.006806] [cpuid: 0] timeout while waiting for init operation
<3>[  256.012251] [cpuid: 0] ath6kl_pm_remove () enter and will invoke power-down sequence
<6>[  256.015116] [cpuid: 0] WLAN power-down success
<3>[  256.035513] [cpuid: 0] android_readwrite_file: ret=2
<3>[  256.035786] [cpuid: 0] android_readwrite_file: ret=2
<6>[  256.051774] [cpuid: 0] mmc2: card 0001 removed
<3>[  258.661936] [cpuid: 0] [audmgr.c:process_audmgr_callback] DISABLED
<3>[  273.027316] [cpuid: 1] gsm_modem_ioctl not get data ,then set value to 0 ,go on
<6>[  273.027391] [cpuid: 1] gsm_modem_ioctl over [0x105]
<3>[  279.369689] [cpuid: 0] [audmgr.c:process_audmgr_callback] rpc DEVICE_CONFIG
<6>[  279.410954] [cpuid: 0] ping all the CPU from CPU0
<3>[  288.607991] [cpuid: 0] [audmgr.c:process_audmgr_callback] DISABLED
<6>[  289.747849] [cpuid: 0] ping all the CPU from CPU0
<3>[  293.054236] [cpuid: 0] gsm_modem_ioctl not get data ,then set value to 0 ,go on
<6>[  293.054744] [cpuid: 0] gsm_modem_ioctl over [0x105]
这样说明SDIO起来的。但是还是不成功，用示源器量了adio_clk引脚也是有波形的。
因为我们只有一块主板上面接了LCD和TP可以点击连接WIFI。
后面有个同事查到 用命令也可以打开WIFI。
打开WIFI:svc wifi enable
关闭WIFI:svc wifi disable
然后再打开另一个终端可以看到log. 确认WIFI打开正常。
<3>[   82.753669] [cpuid: 0] [audmgr.c:process_audmgr_callback] DISABLED
<3>[   86.961204] [cpuid: 0] [audmgr.c:process_audmgr_callback] DISABLED
<6>[   88.801906] [cpuid: 0] ping all the CPU from CPU0
<6>[   88.858278] [cpuid: 1] Calling CRDA to update world regulatory domain
<6>[   88.858521] [cpuid: 1] World regulatory domain updated:
<6>[   88.858543] [cpuid: 1]     (start_freq - end_freq @ bandwidth), (max_antenna_gain, max_eirp)
<6>[   88.858566] [cpuid: 1]     (2402000 KHz - 2472000 KHz @ 40000 KHz), (300 mBi, 2000 mBm)
<6>[   88.858589] [cpuid: 1]     (2457000 KHz - 2482000 KHz @ 20000 KHz), (300 mBi, 2000 mBm)
<6>[   88.858613] [cpuid: 1]     (2474000 KHz - 2494000 KHz @ 20000 KHz), (300 mBi, 2000 mBm)
<6>[   88.858634] [cpuid: 1]     (5140000 KHz - 5360000 KHz @ 40000 KHz), (N/A, 3000 mBm)
<6>[   88.858656] [cpuid: 1]     (5460000 KHz - 5860000 KHz @ 40000 KHz), (N/A, 3000 mBm)
<3>[   89.056016] [cpuid: 1] ath6kl_pm_probe () enter and will invoke power-up sequence
<4>[   89.056106] [cpuid: 1] smps3: Failed to create debugfs directory
<6>[   89.058519] [cpuid: 1] WLAN power-up success
<3>[   89.059624] [cpuid: 1] android_readwrite_file: ret=2
<3>[   89.059664] [cpuid: 1] android_readwrite_file: ret=2
<4>[   89.985994] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x01 (3 bytes)
<4>[   90.007124] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x1a (5 bytes)
<4>[   90.016228] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x1b (8 bytes)
<4>[   90.018101] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x14 (0 bytes)
<4>[   90.028399] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x80 (1 bytes)
<4>[   90.028681] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x81 (1 bytes)
<4>[   90.028963] [cpuid: 0] mmc2: queuing unknown CIS tuple 0x82 (1 bytes)
<6>[   90.029138] [cpuid: 0] mmc2: new high speed SDIO card at address 0001
<4>[   90.097564] [cpuid: 0] ath6kl: MAC from EEPROM 00:03:7F:05:C0:CA
<4>[   90.097584] [cpuid: 0] ath6kl: MAC from nv item 20:59:A0:53:A6:2F
<3>[   90.115609] [cpuid: 0] ath6kl: temporary war to avoid sdio crc error
<6>[   90.856384] [cpuid: 0] ath6kl: ar6003 hw 2.1.1 sdio fw 3.4.0.130.SMARTPHONE api 4
<6>[   90.952363] [cpuid: 0] ADDRCONF(NETDEV_UP): p2p0: link is not ready
<6>[   91.051781] [cpuid: 0] ADDRCONF(NETDEV_UP): wlan0: link is not ready
<6>[   92.405459] [cpuid: 0] ADDRCONF(NETDEV_CHANGE): wlan0: link becomes ready
<6>[   92.729418] [cpuid: 0] ping all the CPU from CPU0
<3>[   97.964668] [cpuid: 0] [audmgr.c:process_audmgr_callback] rpc DEVICE_CONFIG
<6>[   98.189668] [cpuid: 0] ping all the CPU from CPU0
<3>[  101.193399] [cpuid: 0] [audmgr.c:process_audmgr_callback] DISABLED
<7>[  103.373299] [cpuid: 0] wlan0: no IPv6 routers present
<6>[  112.144468] [cpuid: 0] ADDRCONF(NETDEV_CHANGE): wlan0: link becomes ready
<6>[  112.145318] [cpuid: 0] Calling CRDA to update world regulatory domain
<6>[  112.145556] [cpuid: 0] World regulatory domain updated:
<6>[  112.145569] [cpuid: 0]     (start_freq - end_freq @ bandwidth), (max_antenna_gain, max_eirp)
<6>[  112.145583] [cpuid: 0]     (2402000 KHz - 2472000 KHz @ 40000 KHz), (300 mBi, 2000 mBm)
<6>[  112.145596] [cpuid: 0]     (2457000 KHz - 2482000 KHz @ 20000 KHz), (300 mBi, 2000 mBm)
<6>[  112.145609] [cpuid: 0]     (2474000 KHz - 2494000 KHz @ 20000 KHz), (300 mBi, 2000 mBm)
<6>[  112.145621] [cpuid: 0]     (5140000 KHz - 5360000 KHz @ 40000 KHz), (N/A, 3000 mBm)
<6>[  112.145634] [cpuid: 0]     (5460000 KHz - 5860000 KHz @ 40000 KHz), (N/A, 3000 mBm)
<6>[  112.177013] [cpuid: 0] ping all the CPU from CPU0
<3>[  112.217603] [cpuid: 1] init: no such service 'dhcpcd_eth0'
<4>[  113.370918] [cpuid: 0] wpa_supplicant used greatest stack depth: 5236 bytes left
<3>[  113.760791] [cpuid: 0] ath6kl_pm_remove () enter and will invoke power-down sequence
<6>[  113.763756] [cpuid: 0] WLAN power-down success
<3>[  113.777101] [cpuid: 0] android_readwrite_file: ret=2
<3>[  113.778741] [cpuid: 0] android_readwrite_file: ret=2
<6>[  113.796304] [cpuid: 0] mmc2: card 0001 removed



