
# Android bootchart（二） - 嵌入式Linux - CSDN博客

2015年11月05日 10:16:48[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1388



## 这篇文章讲一下MTK8127开机启动的时间
## MTK8127发布版本开机时间大约在２０秒左右，如果发现开机时间变长，大部分是因为加上了客户订制的东西，代码累赘太多了。
# １、下面看一下ＭＴＫ开机花时间的是哪些动作
![](https://img-blog.csdn.net/20151105101203511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
１－１　kernel初始化
１－２　加载ＮＡＮＤ分区
１－３　启动Android
１－３－１　加载类　在zygote里面加了　时间发现　这个时间确实很长
１－３－２　包扫描
１－３－３　应用扫描
如何查看启动时间的相关时间点呢？看下面
root@mid713l_lp_lvds:/ \# cat /proc/bootprof
----------------------------------------
0        BOOT PROF (unit:msec)
----------------------------------------
4098        : preloader
722        : lk
1829        : lk->Kernel
----------------------------------------
8743.419000 : Kernel_init_done
8758.075154 : SElinux start.
14921.341308 : SElinux end.
15750.858462 : INIT: on init start
16147.198077 : INIT:NAND:Mount_START
32627.899693 : INIT:NAND:Mount_END
32630.724232 : INIT:PROTECT:fat start
32733.226540 : INIT:PROTECT:fat end
32736.114693 : INIT:PROTECT_RAW_DISK:Mount_START
33582.732924 : INIT:PROTECT_RAW_DISK:Mount_END
34489.614386 : INIT: eng build setting
38913.817386 : BOOT_Animation:START
57958.511387 : Zygote:Preload Start
61800.738080 : Zygote:Preload 2774 classes in 3827ms
62764.249541 : Zygote:Preload 274 obtain resources in 941ms
62800.117541 : Zygote:Preload 31 resources in 35ms
62890.361695 : Zygote:Preload End
63355.167926 : Android:SysServerInit_START
64077.987388 : Android:PackageManagerService_Start
64640.584772 : Android:PMS_scan_START
70069.473157 : Android:PMS_scan_done:/custom/framework
70141.781234 : Android:PMS_scan_done:/system/framework
71685.662234 : Android:PMS_scan_done:/system/priv-app
75316.111234 : Android:PMS_scan_done:/system/app
75327.165004 : Android:PMS_scan_done:/system/vendor/app
75328.890158 : Android:PMS_scan_done:/system/vendor/operator/app
75330.559850 : Android:PMS_scan_done:/custom/app
75337.836927 : Android:PMS_scan_data_start
75418.824465 : Android:PMS_scan_data_done:/data/app
75420.457542 : Android:PMS_scan_data_done:/data/app-private
75422.520927 : Android:PMS_scan_END
75840.712234 : Android:PMS_READY
80126.832927 : AP_Init:[service]:[com.android.systemui]:[com.android.keyguard/.KeyguardService]:pid:809
82539.565158 : AP_Init:[service]:[com.mediatek.security]:[com.mediatek.security/.service.PermControlService]:pid:851
83699.413696 : AP_Init:[content provider]:[android.process.acore]:[com.android.providers.contacts/.ContactsProvider2]:pid:920
85690.594004 : AP_Init:[service]:[com.google.android.inputmethod.latin]:[com.google.android.inputmethod.latin/com.android.inputmethod.latin.La
87394.339466 : AP_Init:[broadcast]:[com.google.android.gms]:[com.google.android.gms/com.google.android.location.internal.NlpNetworkProviderSet
91038.825466 : AP_Init:[added application]:[com.android.phone]:[com.android.phone]:pid:990:(PersistAP)
91111.352389 : AP_Init:[added application]:[com.mediatek.voicecommand]:[com.mediatek.voicecommand]:pid:1003:(PersistAP)
91179.826158 : AP_Init:[added application]:[com.mediatek.bluetooth]:[com.mediatek.bluetooth]:pid:1018:(PersistAP)
91248.128928 : AP_Init:[added application]:[com.mediatek.dongle]:[com.mediatek.dongle]:pid:1036:(PersistAP)
91372.823851 : AP_Init:[activity]:[com.android.launcher3]:[com.android.launcher3/.Launcher]:pid:1052
91390.517158 : Android:SysServerInit_END
91448.375543 : AP_Init:[service]:[com.android.printspooler]:[com.android.printspooler/.PrintSpoolerService]:pid:1067
97597.181851 : AP_Init:[content provider]:[android.process.media]:[com.android.providers.downloads/.DownloadProvider]:pid:1100
97846.665697 : AP_Init:[content provider]:[com.google.process.gapps]:[com.google.android.gsf/.gservices.GservicesProvider]:pid:1119
103043.537698 : AP_Init:[broadcast]:[com.google.process.location]:[com.google.android.gms/com.google.android.location.network.LocationProviderC
108608.975006 : BOOT_Animation:END



# ２、再来看一下　各个阶段的时间　可以通过平板上显示的图标看出来
![](https://img-blog.csdn.net/20151105102805851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
16147.198077 : INIT:NAND:Mount_START
32627.899693 : INIT:NAND:Mount_END
从这个日志看　NAND 挂载花了１６秒多　这个时间肯定要优化一下，要不然开机时间消耗太大

# ３、从日志分析时间
![](https://img-blog.csdn.net/20151105113450194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


