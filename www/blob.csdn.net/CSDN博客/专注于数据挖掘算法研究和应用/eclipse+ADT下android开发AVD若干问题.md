# eclipse+ADT下android开发AVD若干问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月04日 08:47:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2409








1、问题：启动AVD时，提示Error: Error parsing /android-sdk-windows/system-images/ndroid-23/android-wear/armeabi-v7a/devices.xml

查阅下，发现是devices.xml内d:skin开头存在无效内容。

解决：/android-sdk-windows/tools/lib/devices.xml覆盖/android-sdk-windows/system-images/ndroid-23/android-wear/armeabi-v7a/devices.xml

重启eclipse和AVD即不再提示，但未发现该错误对开发有影响。




2、问题：已在SDK
 manager中安装Intel X86 Emulator Accelerator(HAXM)，但AVD的CPU\ABI设置为Android TV Intel X86 Atom并启动时还是提示未安装加速器；

解决：HAXM已在\android-sdk-windows\extras\intel\Hardware_Accelerated_Execution_Manager路径下，执行intelhaxm-android.exe安装




3、问题：启动AVD后一直卡着，通过DDMS-logcat查看：emulator without host-side gpu emulation detected

      解决：重新配置AVD，勾选 Use Host GPU即可；

      结果还是一直卡着，通过DDMS-logcat查看：Host Connection get一直停留着，几番折腾都没办法。

      最后换SDK platforms，原来是最高android6.0(API 23)，换成android4.2.2(API17)后即可。

      猜测是否是ADT内AVD Manager版本和API版本关系，开发时还是不要选最新版本。

     模拟器后续还是考虑Genymotion的使用，android avd安装文件极其庞大而且版本之间不兼容，准备开发环境太辛苦。





