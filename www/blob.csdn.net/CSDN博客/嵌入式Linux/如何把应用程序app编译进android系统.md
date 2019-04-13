
# 如何把应用程序app编译进android系统 - 嵌入式Linux - CSDN博客

2015年09月21日 14:42:57[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：504个人分类：[Android																](https://blog.csdn.net/weiqifa0/article/category/1752017)


转载：
http://ywxiao66.blog.163.com/blog/static/175482055201152710441106/
------------------------------------------------------------------
把常用的应用程序编译到img文件中，就成了系统的一部分，用户不必自己安装，当然也卸载不了；
同时也可以删减系统自带的应用程序，精简系统；
1.\build\target\product 目录下generic.mk文件：
Java代码![收藏代码](http://koliy.iteye.com/images/icon_star.png)
PRODUCT_PACKAGES := \
AccountAndSyncSettings \
DeskClock \
AlarmProvider \
Bluetooth \
Calculator \
Calendar \
Camera \
testMid \
CertInstaller \
DrmProvider \
Email \
Gallery3D \
LatinIME \
Launcher2 \
Mms \
Music \

我们添加一个testMid \ 应用名称。
2.把testMid包放入
\packages\apps 目录下,修改android.mk文件。
Java代码![收藏代码](http://koliy.iteye.com/images/icon_star.png)
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-subdir-java-files)
LOCAL_PACKAGE_NAME := testMid
LOCAL_CERTIFICATE := platform
include $(BUILD_PACKAGE)

注：LOCAL_PACKAGE_NAME := testMid (包名必须和generic.mk中添加的相同)
编译源码，可以看到在
\out\target\product\smdkv210\system\app
目录下生存了testMid.apk了。这时system.img也包含了此应用。
-------------------------------------------------------------------
特殊情况：有时，应用需要包含jar包，这时的app导入源码时会出现问题：
**MODULE.TARGET.JAVA_LIBRARIES.libarity already defined by ... stop**
由于 LOCAL_STATIC_JAVA_LIBRARIES := libarity  会引发错误信息。
目前解决方法是：
\build\core 目录下修改base_rules.mk
注释掉错误信息：
ifdef $(module_id)
\#$(error $(LOCAL_PATH): $(module_id) already defined by $($(module_id)))
endif
$(module_id) := $(LOCAL_PATH)
--重新编译，这时可以通过了。
(2)、删除原厂（Telchips）带源码的应用程序，如DTV_DVBT
在/device/telechips/m801/device.mk
注释掉相应语句：
\# PRODUCT_PACKAGES += \
\#    SampleDVBTPlayer \
同时，在/out/target/product/m801/system/app 找到相应的.APK包，并删除

