
# RK方案 manifest.xml编译问题，并解决方案 - 嵌入式Linux - CSDN博客

2018年10月23日 15:59:49[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：218


编译出错
Install: out/target/product/rk3288/system/priv-app/TeleService/TeleService.apk
Install: out/target/product/rk3288/system/priv-app/Telecom/Telecom.apk
Install: out/target/product/rk3288/system/priv-app/TelephonyProvider/TelephonyProvider.apk
Install: out/target/product/rk3288/system/priv-app/VpnDialogs/VpnDialogs.apk
Install: out/target/product/rk3288/system/priv-app/WallpaperCropper/WallpaperCropper.apk
Install: out/target/product/rk3288/system/priv-app/projectX/projectX.apk
Copy xml: out/target/product/rk3288/system/manifest.xml
Copy: out/target/product/rk3288/systembbperformance_runtime.so
manifest.xml:1: parser error : Document is empty
make: *** [out/target/product/rk3288/system/manifest.xml] Error 1
make: *** Waiting for unfinished jobs....
Copy: out/target/product/rk3288/systemb/hw/gpu.rk30board.so
\#\#\#\# make failed to build some targets (01:34 (mm:ss)) \#\#\#\#
修改device/rockchip/common/BoardConfig.mk
SYSTEM_WITH_MANIFEST ?= true   修改成 false 即可
---------------------
作者：fmc088
来源：CSDN
原文：https://blog.csdn.net/fmc088/article/details/80333625
版权声明：本文为博主原创文章，转载请附上博文链接！

