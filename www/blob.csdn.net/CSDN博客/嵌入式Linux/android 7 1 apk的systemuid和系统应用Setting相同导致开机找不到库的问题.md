
# android 7.1 apk的systemuid和系统应用Setting相同导致开机找不到库的问题 - 嵌入式Linux - CSDN博客

2019年03月13日 00:46:12[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：92



# 前言
![](https://img-blog.csdnimg.cn/20190313001705285.png)
![](https://img-blog.csdnimg.cn/20190313001929366.png)
![](https://img-blog.csdnimg.cn/20190313002201625.png)
## Android apk加载机制
这就是我最近在解决的一个问题，32位的apk可以用32位的库，那64位的apk可以用64位的库，如果我想64位的apk同时使用32和64位的库呢？
**android加载so文件的机制**
apk在安装的过程中，系统就会对apk进行解析根据里面so文件类型，确定这个apk安装是在32 还是 64位的虚拟机上，如果是32位虚拟机那么就不能使用64位so，如果是64位虚拟机也不能使用32位so。而64位设备可以提供32和64位两种虚拟机，根据apk选择开启哪一种，因此说64位设备兼容32的so库。
具体来说就是apk在安装的时候，apk解包的时候，就已经确定要加载多少位的库了，如果apk里面放有两种库，实际用的时候，也是只会加载一种库。
## 我遇到的问题
我们遇到的问题是，我们自己编写的应用是系统应用，使用了系统的systemuid,就是和setting的uid是一样的，而且我们是32位apk，setting是64位apk,这样开机的时候经常遇到setting到不到lib库。对应的日志如下。
> 03-12 10:48:50.245 1381 1381 D AndroidRuntime: Shutting down VM

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: FATAL EXCEPTION: main

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: Process: com.android.settings, PID: 1381

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: java.lang.RuntimeException: Unable to instantiate application com.android.settings.SettingsApplication: java.lang.ClassNotFoundException: Didn’t find class “com.android.settings.SettingsApplication” on path: DexPathList[[zip file “/system/priv-app/Settings/Settings.apk”],nativeLibraryDirectories=[/system/priv-app/Settings/lib/arm64, /system/priv-app/Settings/Settings.apk!/lib/armeabi-v7a, /system/lib, /vendor/lib, /system/lib, /vendor/lib]]

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.app.LoadedApk.makeApplication(LoadedApk.java:802)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.app.ActivityThread.handleBindApplication(ActivityThread.java:5379)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.app.ActivityThread.-wrap2(ActivityThread.java)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.app.ActivityThreadH.handleMessage(ActivityThread.java:1546)03−1210:48:50.24713811381EAndroidRuntime:atandroid.os.Handler.dispatchMessage(Handler.java:102)03−1210:48:50.24713811381EAndroidRuntime:atandroid.os.Looper.loop(Looper.java:154)03−1210:48:50.24713811381EAndroidRuntime:atandroid.app.ActivityThread.main(ActivityThread.java:6121)03−1210:48:50.24713811381EAndroidRuntime:atjava.lang.reflect.Method.invoke(NativeMethod)03−1210:48:50.24713811381EAndroidRuntime:atcom.android.internal.os.ZygoteInit H.handleMessage(ActivityThread.java:1546)03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.os.Handler.dispatchMessage(Handler.java:102)03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.os.Looper.loop(Looper.java:154)03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.app.ActivityThread.main(ActivityThread.java:6121)03-12 10:48:50.247 1381 1381 E AndroidRuntime: at java.lang.reflect.Method.invoke(Native Method)03-12 10:48:50.247 1381 1381 E AndroidRuntime: at com.android.internal.os.ZygoteInitH.handleMessage(ActivityThread.java:1546)03−1210:48:50.24713811381EAndroidRuntime:atandroid.os.Handler.dispatchMessage(Handler.java:102)03−1210:48:50.24713811381EAndroidRuntime:atandroid.os.Looper.loop(Looper.java:154)03−1210:48:50.24713811381EAndroidRuntime:atandroid.app.ActivityThread.main(ActivityThread.java:6121)03−1210:48:50.24713811381EAndroidRuntime:atjava.lang.reflect.Method.invoke(NativeMethod)03−1210:48:50.24713811381EAndroidRuntime:atcom.android.internal.os.ZygoteInitMethodAndArgsCaller.run(ZygoteInit.java:912)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:802)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: Caused by: java.lang.ClassNotFoundException: Didn’t find class “com.android.settings.SettingsApplication” on path: DexPathList[[zip file “/system/priv-app/Settings/Settings.apk”],nativeLibraryDirectories=[/system/priv-app/Settings/lib/arm64,
> /system/priv-app/Settings/Settings.apk!/lib/armeabi-v7a, /system/lib, /vendor/lib, /system/lib, /vendor/lib]]

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at dalvik.system.BaseDexClassLoader.findClass(BaseDexClassLoader.java:56)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at java.lang.ClassLoader.loadClass(ClassLoader.java:380)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at java.lang.ClassLoader.loadClass(ClassLoader.java:312)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.app.Instrumentation.newApplication(Instrumentation.java:992)

> 03-12 10:48:50.247 1381 1381 E AndroidRuntime: at android.app.LoadedApk.makeApplication(LoadedApk.java:796)

> 因为我们自己的应用是32位，又使用了系统的uid，这样引导系统也去找对应的32位lib。

> 当然了，很不幸，setting是没有32位Lib的，所以，就出现了问题，设置挂逼，出现问题的时候，把我们的应用删除掉就可以了。

> 03-12 10:48:34.351 634 634 I SystemServiceManager: Starting com.android.server.pm.Installer

> 03-12 10:48:34.359 634 634 I SystemServiceManager: Starting com.android.server.am.ActivityManagerServiceKaTeX parse error: Expected ‘}’, got ‘EOF’ at end of input: …id.view.DisplayHdrCapabilities@a69d6308, density 280, 213.0 x 213.0 dpi, appVsyncOff 1000000, presDeadline 16129032, touch INTERNAL, rotation 0, type BUILT_IN, state UNKNOWN, FLAG_DEFAULT_DISPLAY, FLAG_ROTATES_WITH_CONTENT, FLAG_SECURE, FLAG_SUPPORTS_PROTECTED_BUFFERS}

> 03-12 10:48:34.359 634 634 I SystemServiceManager: Starting com.android.server.am.ActivityManagerServiceKaTeX parse error: Expected ‘}’, got ‘EOF’ at end of input: …id.view.DisplayHdrCapabilities@a69d6308, density 280, 213.0 x 213.0 dpi, appVsyncOff 1000000, presDeadline 16129032, touch INTERNAL, rotation 0, type BUILT_IN, state UNKNOWN, FLAG_DEFAULT_DISPLAY, FLAG_ROTATES_WITH_CONTENT, FLAG_SECURE, FLAG_SUPPORTS_PROTECTED_BUFFERS}

> 03-12 10:48:34.772 634 655 I DisplayManagerService: Display device changed state: “Built-in Screen”, ON

> 03-12 10:48:34.773 634 634 I SystemServiceManager: Starting phase 100

> 03-12 10:48:34.774 634 634 I SystemServer: StartPackageManagerService

> 03-12 10:48:34.926 634 634 I PackageManager: No settings file; creating initial state

> 03-12 10:48:34.953 634 634 W PackageManager: Library not found: /system/framework/org.apache.http.legacy.jar

> 03-12 10:48:34.976 634 634 W PackageManager: Library not found: /system/framework/org.apache.http.legacy.jar

> 03-12 10:48:34.995 634 634 W PackageManager: Failed to parse /system/framework/arm: Missing base APK in /system/framework/arm

> 03-12 10:48:34.996 634 634 W PackageManager: Failed to parse /system/framework/arm64: Missing base APK in /system/framework/arm64

> 03-12 10:48:35.044 634 634 I PackageManager: /system/framework/framework-res.apk changed; collecting certs

> 03-12 10:48:35.237 634 634 W PackageManager: Failed to parse /system/framework/oat: Missing base APK in /system/framework/oat

> 03-12 10:48:35.249 634 634 I PackageManager: /system/priv-app/BackupRestoreConfirmation changed; collecting certs

> 03-12 10:48:35.266 634 634 I PackageManager: /system/priv-app/DefaultContainerService changed; collecting certs

> 03-12 10:48:35.288 634 634 I PackageManager: /system/priv-app/DeviceTest changed; collecting certs

> 03-12 10:48:35.329 634 634 I PackageManager: /system/priv-app/ExtServices changed; collecting certs

> 03-12 10:48:35.344 634 634 I PackageManager: /system/priv-app/ExternalStorageProvider changed; collecting certs

> 03-12 10:48:35.357 634 634 I PackageManager: /system/priv-app/FusedLocation changed; collecting certs

> 03-12 10:48:35.371 634 634 I PackageManager: /system/priv-app/InputDevices changed; collecting certs

> 03-12 10:48:35.384 634 634 I PackageManager: /system/priv-app/KeyChain changed; collecting certs

> 03-12 10:48:35.401 634 634 I PackageManager: /system/priv-app/ManagedProvisioning changed; collecting certs

> 03-12 10:48:35.419 634 634 I PackageManager: /system/priv-app/MediaProvider changed; collecting certs

> 03-12 10:48:35.431 634 634 I PackageManager: /system/priv-app/MtpDocumentsProvider changed; collecting certs

> 03-12 10:48:35.442 634 634 I PackageManager: /system/priv-app/OneTimeInitializer changed; collecting certs

> 03-12 10:48:35.476 634 634 I PackageManager: /system/priv-app/PackageInstaller changed; collecting certs

> 03-12 10:48:35.509 634 634 I PackageManager: /system/priv-app/Provision changed; collecting certs

> 03-12 10:48:35.521 634 634 I PackageManager: /system/priv-app/ProxyHandler changed; collecting certs

> 03-12 10:48:35.535 634 634 I PackageManager: /system/priv-app/RetailDemo changed; collecting certs

> 03-12 10:48:35.661 634 634 I PackageManager: /system/priv-app/Settings changed; collecting certs

> 03-12 10:48:35.817 634 634 I PackageManager: /system/priv-app/SettingsProvider changed; collecting certs

> 03-12 10:48:35.831 634 634 I PackageManager: /system/priv-app/SharedStorageBackup changed; collecting certs

> 03-12 10:48:35.848 634 634 I PackageManager: /system/priv-app/Shell changed; collecting certs

> 03-12 10:48:35.863 634 634 I PackageManager: /system/priv-app/StatementService changed; collecting certs

> 03-12 10:48:35.889 634 634 I PackageManager: /system/priv-app/StorageManager changed; collecting certs

> 03-12 10:48:35.889 634 634 I PackageManager: /system/priv-app/StorageManager changed; collecting certs

> 03-12 10:48:35.931 634 634 I PackageManager: /system/priv-app/StressTest changed; collecting certs

> 03-12 10:48:36.049 634 634 I PackageManager: /system/priv-app/SystemUI changed; collecting certs

> 03-12 10:48:36.154 634 634 I PackageManager: /system/app/Bluetooth changed; collecting certs

> 03-12 10:48:36.173 634 634 I PackageManager: /system/app/BluetoothMidiService changed; collecting certs

> 03-12 10:48:36.201 634 634 I PackageManager: /system/app/Camera2 changed; collecting certs

> 03-12 10:48:36.242 634 634 I PackageManager: /system/app/CertInstaller changed; collecting certs

> 03-12 10:48:36.261 634 634 I PackageManager: /system/app/DownloadProviderUi changed; collecting certs

> 03-12 10:48:36.275 634 634 I PackageManager: /system/app/ExtShared changed; collecting certs

> 03-12 10:48:36.295 634 634 I PackageManager: /system/app/FactoryTest changed; collecting certs

> 03-12 10:48:36.345 634 634 I PackageManager: /system/app/jjjjj110 changed; collecting certs

> 03-12 10:48:37.598 634 634 W PackageManager: Permission android.permission.FLASHLIGHT from package com.xxxxxx.jjjjj.titket.oceanFlower in an unknown group android.permission-group.HARDWARE_CONTROLS

> 03-12 10:48:37.689 634 634 I PackageManager: /system/app/LatinIME changed; collecting certs

> 03-12 10:48:37.868 634 634 I PackageManager: /system/app/Launcher3 changed; collecting certs

> 03-12 10:48:37.879 634 634 W PackageManager: Permission com.android.launcher3.permission.READ_SETTINGS from package com.android.launcher3 in an unknown group android.permission-group.SYSTEM_TOOLS

> 03-12 10:48:37.879 634 634 W PackageManager: Permission com.android.launcher3.permission.WRITE_SETTINGS from package com.android.launcher3 in an unknown group android.permission-group.SYSTEM_TOOLS

> 03-12 10:48:37.879 634 634 W PackageManager: Permission com.android.launcher.permission.INSTALL_SHORTCUT from package com.android.launcher3 in an unknown group android.permission-group.SYSTEM_TOOLS

> 03-12 10:48:37.895 634 634 I PackageManager: /system/app/LiveWallpapersPicker changed; collecting certs

> 03-12 10:48:37.915 634 634 I PackageManager: /system/app/MediaFloat changed; collecting certs

> 03-12 10:48:37.938 634 634 I PackageManager: /system/app/NfcNci changed; collecting certs

> 03-12 10:48:37.964 634 634 I PackageManager: /system/app/PrintRecommendationService changed; collecting certs

> 03-12 10:48:37.983 634 634 I PackageManager: /system/app/PrintSpooler changed; collecting certs

> 03-12 10:48:38.000 634 634 I PackageManager: /system/app/RFTestTool changed; collecting certs

> 03-12 10:48:38.015 634 634 I PackageManager: /system/app/RKUpdateService changed; collecting certs

> 03-12 10:48:38.044 634 634 I PackageManager: /system/app/RkExplorer changed; collecting certs

> 03-12 10:48:38.064 634 634 I PackageManager: /system/app/SecretCode changed; collecting certs

> 03-12 10:48:38.080 634 634 I PackageManager: /system/app/UserDictionaryProvider changed; collecting certs

> 03-12 10:48:38.095 634 634 I PackageManager: /system/app/WallpaperPicker changed; collecting certs

> 03-12 10:48:38.118 634 634 I PackageManager: /system/app/webview changed; collecting certs

> 03-12 10:48:39.617 634 634 W PackageManager: Protected action; cap priority to 0; package: com.android.packageinstaller activity: com.android.packageinstaller.PackageInstallerActivity origPrio: 1

> 03-12 10:48:39.617 634 634 I PackageManager: Adjusting ABI for com.android.providers.media to arm64-v8a (requirer=com.android.mtp, scannedPackage=null)

> 03-12 10:48:39.619 634 634 I PackageManager: Adjusting ABI for com.android.providers.downloads.ui to arm64-v8a (requirer=com.android.mtp, scannedPackage=null)

> 03-12 10:48:39.620 634 634 W PackageManager: Instruction set mismatch, PackageSetting{1e08467 com.xxxxxx.jjjjj.titket.oceanFlower/1000} requires arm whereas PackageSetting{51ca86c android.rockchip.update.service/1000} requires arm64

> 03-12 10:48:39.620 634 634 W PackageManager: Instruction set mismatch, PackageSetting{1e08467 com.xxxxxx.jjjjj.titket.oceanFlower/1000} requires arm whereas PackageSetting{992cb54 android/1000} requires arm64

> 03-12 10:48:39.620 634 634 I PackageManager: Adjusting ABI for com.android.location.fused to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.621 634 634 I PackageManager: Adjusting ABI for com.android.rk to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.622 634 634 I PackageManager: Adjusting ABI for com.DeviceTest to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.623 634 634 I PackageManager: Adjusting ABI for com.cghs.stresstest to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.623 634 634 I PackageManager: Adjusting ABI for com.android.keychain to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.624 634 634 I PackageManager: Adjusting ABI for com.android.rk.mediafloat to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.625 634 634 I PackageManager: Adjusting ABI for com.xingluo.engineermode.test to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.626 634 634 I PackageManager: Adjusting ABI for com.android.inputdevices to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.626 634 634 I PackageManager: Adjusting ABI for com.android.providers.settings to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.628 634 634 I PackageManager: Adjusting ABI for com.android.settings to armeabi-v7a (requirer=com.xxxxxx.jjjjj.titket.oceanFlower, scannedPackage=null)

> 03-12 10:48:39.632 634 634 I PackageManager: Time to scan packages: 4.701 seconds

> 03-12 10:48:39.634 634 634 W PackageManager: Unknown permission android.permission.SEND_DOWNLOAD_COMPLETED_INTENTS in package com.android.providers.downloads.ui

> 03-12 10:48:39.634 634 634 W PackageManager: Unknown permission android.permission.ACCESS_ALL_DOWNLOADS in package com.android.providers.downloads.ui

> 03-12 10:48:39.634 634 634 W PackageManager: Unknown permission android.permission.ACCESS_ALL_DOWNLOADS in package com.android.certinstaller

> 03-12 10:48:39.635 634 634 W PackageManager: Unknown permission android.permission.NFC_UNLOCK in package com.android.nfc

> 03-12 10:48:39.636 634 634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.launcher3

> 03-12 10:48:39.636 634 634 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.launcher3

> 03-12 10:48:39.636 634 634 W PackageManager: Not granting permission android.permission.BIND_APPWIDGET to package com.android.launcher3 (protectionLevel=18 flags=0x3c4bbe45)

> 03-12 10:48:39.637 634 634 W PackageManager: Unknown permission android.permission.RECORD_VIDEO in package com.xxxxxx.jjjjj.titket.oceanFlower

> 03-12 10:48:39.637 634 634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.onetimeinitializer

> 03-12 10:48:39.637 634 634 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.onetimeinitializer

> 03-12 10:48:39.637 634 634 W PackageManager: Unknown permission com.google.android.permission.INSTALL_WEARABLE_PACKAGES in package com.android.packageinstaller

> 03-12 10:48:39.637 634 634 W PackageManager: Unknown permission android.permission.DOWNLOAD_WITHOUT_NOTIFICATION in package com.android.inputmethod.latin

> 03-12 10:48:39.638 634 634 W PackageManager: Unknown permission android.permission.SENSOR_INFO in package com.xingluo.engineermode.test

> 03-12 10:48:39.638 634 634 W PackageManager: Unknown permission android.permission.SENSOR_ENABLE in package com.xingluo.engineermode.test

> 03-12 10:48:39.639 634 634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.xingluo.engineermode.test

> 03-12 10:48:39.639 634 634 W PackageManager: Unknown permission com.google.android.googleapps.permission.GOOGLE_AUTH in package com.android.settings

> 03-12 10:48:39.639 634 634 W PackageManager: Unknown permission android.permission.ACCESS_DOWNLOAD_MANAGER in package com.android.settings

> 03-12 10:48:39.639 634 634 W PackageManager: Unknown permission android.permission.ACCESS_DOWNLOAD_MANAGER in package com.android.settings

> 03-12 10:48:39.640 634 634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.android.settings

> 03-12 10:48:39.640 634 634 W PackageManager: Unknown permission com.android.launcher.permission.WRITE_SETTINGS in package com.android.settings

> 03-12 10:48:39.644 634 634 W PackageManager: Unknown permission com.android.launcher.permission.READ_SETTINGS in package com.DeviceTest

> 03-12 10:48:39.645 634 634 W PackageManager: Unknown permission com.google.android.gallery3d.permission.GALLERY_PROVIDER in package com.android.bluetooth

> 03-12 10:48:39.645 634 634 W PackageManager: Unknown permission com.android.gallery3d.permission.GALLERY_PROVIDER in package com.android.bluetooth

> 03-12 10:48:39.646 634 634 V PackageManager: reconcileAppsData for null u0 0x3

> 03-12 10:48:39.901 634 634 V PackageManager: reconcileAppsData finished 47 packages

> 03-12 10:48:40.599 634 634 D PackageManager: Ephemeral resolver NOT found; no matching intent filters

> 03-12 10:48:40.600 634 634 I PackageManager: Ephemeral deactivated; missing resolver and installer

> 03-12 10:48:40.652 634 634 I SystemServer: StartUserManagerService

> 03-12 10:48:40.652 634 634 I SystemServiceManager: Starting com.android.server.pm.UserManagerServiceLifeCycle03−1210:48:40.672634634ISystemServiceManager:Startingcom.android.server.BatteryService03−1210:48:40.688634634ISystemServiceManager:Startingcom.android.server.usage.UsageStatsService03−1210:48:40.710634634ISystemServiceManager:Startingcom.android.server.webkit.WebViewUpdateService03−1210:48:40.718634634ISystemServer:Readingconfiguration…03−1210:48:40.718634634ISystemServer:StartSchedulingPolicyService03−1210:48:40.720634634ISystemServer:StartEntropyMixer03−1210:48:40.721634634WEntropyMixer:Noexistingentropyfile−−firstboot?03−1210:48:40.722634634IEntropyMixer:Writingentropy…03−1210:48:40.727634634ISystemServer:CameraService03−1210:48:40.730634634ISystemServiceManager:Startingcom.android.server.camera.CameraService03−1210:48:40.734634685IServiceThread:EnabledStrictModeloggingforCameraServiceproxylooper.03−1210:48:40.735634634ISystemServer:StartAccountManagerService03−

> 看上面的日志，日志是开机的时候打印，过滤掉PackageManager，就可以看到系统开机安装apk的顺序，对我们调试来说也是非常有帮助。

> 里面有两条日志，如下：

> 03-12 10:48:39.620 634 634 W PackageManager: Instruction set mismatch, PackageSetting{1e08467 com.xxxxxx.jjjjj.titket.oceanFlower/1000} requires arm whereas PackageSetting{51ca86c android.rockchip.update.service/1000} requires arm64
03-12 10:48:39.620 634 634 W PackageManager: Instruction set mismatch, PackageSetting{1e08467 com.xxxxxx.jjjjj.titket.oceanFlower/1000} requires arm whereas PackageSetting{992cb54 android/1000} requires arm64
> 大概意思就是说，一个apk需要arm,就是32位的库，一个apk需要arm64，就是64位的库。好吧，这样已经对我们的问题非常明显的。

> 如何查看apk是32位还是64位？

> 使用Zygote来判断应用是32位还是64位，Android生出Zygote，Zygote生出SystemServer，SystemServer又启动PackageManagerService和ActivityManagerService。由此可以知道Zygote在Android系统中的重要性。
![](https://img-blog.csdnimg.cn/20190314190851147.png)
> 如上图，在64位的android机器中，有两个Zygote。
![](https://img-blog.csdnimg.cn/20190314191035886.png)
> 从Zygote64出生出来的都是64位的Apk，从Zygote出生来的都是32位的Apk。

> [
](https://img-blog.csdnimg.cn/20190314191035886.png)
> 如何修改？

> [
](https://img-blog.csdnimg.cn/20190314191035886.png)
> 通过上面的日志，可以想出第一种修改，就是把我们的apk不使用系统的uid。

> 可是我们又需要系统的uid权限，那这个方法被否决。

> 还有一种方法，就是把我们的apk修改为64位的，那他就自己去找64位的库就好了。

> 这个方法也很不幸，我们有一个库是第三方的，就只有32位的库。

> 最后一种方法是从网上找到的，需要修改两个位置。

> 1.在apk对应的android.mk中

> LOCAL_DEX_PREOPT := nostripping

> 2./device/rockchip/rk3399/BoardConfig.mk,

> DEX_PREOPT_DEFAULT := nostripping
![](https://img-blog.csdnimg.cn/20190314193129691.png)

> 隐患

> 我们上面的修改保证了可以在apk的uid相同的情况下使用不同的Lib位库，但是32位的apk，理论上运行的速度肯定没有64位的速度快，CPU跟外设的交互有地址总线，数据总线，和控制总线，如果是64位，说明位宽比较大，理论上运行速度肯定是更加快的。就像拿一个水杯和一个水桶去装水，肯定是水桶能装的水更多。

> 参考

> [https://www.jianshu.com/p/62b3c153ad68](https://www.jianshu.com/p/62b3c153ad68)

> [https://www.jianshu.com/p/3dbe46439359](https://www.jianshu.com/p/3dbe46439359)

> [https://blog.csdn.net/lemonpi/article/details/79620039](https://blog.csdn.net/lemonpi/article/details/79620039)


