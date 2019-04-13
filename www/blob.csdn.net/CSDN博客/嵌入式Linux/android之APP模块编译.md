
# android之APP模块编译 - 嵌入式Linux - CSDN博客

2015年10月09日 17:46:59[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4164


原文地址：http://blog.csdn.net/yaphet__s/article/details/45640627
一，如何把app编进系统
a.源码编译，在packages/apps目录下有安卓原生的app，以Bluetooth为例，源码根目录下有Android.mk文件：
packages\apps\Bluetooth
**[html]**[view
 plain](http://blog.csdn.net/yaphet__s/article/details/45640627#)[copy](http://blog.csdn.net/yaphet__s/article/details/45640627#)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
\#负责清理除LOCAL_PATH外的其他LOCAL_XXX，这个清理动作是必须的，因为所有的编译控制文件由同一个GNU  Make解析和执行，
\#其变量是全局的。所以清理后才能避免相互影响。
LOCAL_MODULE_TAGS:=optional
LOCAL_SRC_FILES:= \
$(call all-java-files-under, src)
LOCAL_PACKAGE_NAME:=Bluetooth
LOCAL_CERTIFICATE:=platform
\#使用系统签名
LOCAL_JNI_SHARED_LIBRARIES:=libbluetooth_jni
LOCAL_JAVA_LIBRARIES:=javax.obex telephony-common mms-common
LOCAL_STATIC_JAVA_LIBRARIES:=com.android.vcard
LOCAL_JAVA_LIBRARIES += com.broadcom.bt
LOCAL_REQUIRED_MODULES:=libbluetooth_jnibluetooth.default
LOCAL_PROGUARD_ENABLED:=disabled
\#不使用代码混淆的工具进行代码混淆，不设置则默认值为full，即将该工程代码全部混淆
include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))
在packages\apps\Bluetooth下mm，可单独编译，在\out\target\product\xxx\system\app下会生成Bluetooth.apk。
然后修改build下的mk文件，增加蓝牙模块，这样在源码根目录下执行make就会编译到Bluetooth模块
\build\target\product\generic_no_telephony.mk
**[html]**[view
 plain](http://blog.csdn.net/yaphet__s/article/details/45640627#)[copy](http://blog.csdn.net/yaphet__s/article/details/45640627#)

PRODUCT_PACKAGES:= \
DeskClock \
Bluetooth \     \#增加蓝牙模块
Calculator \

b.apk编译，针对没有源码的第三方apk，修改Android.mk：
**[html]**[view
 plain](http://blog.csdn.net/yaphet__s/article/details/45640627#)[copy](http://blog.csdn.net/yaphet__s/article/details/45640627#)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
\# Module name should match apk name to be installed
LOCAL_MODULE:=Animation
LOCAL_MODULE_TAGS:=optional
LOCAL_SRC_FILES:=Animation.apk
LOCAL_MODULE_CLASS:=APPS
LOCAL_MODULE_SUFFIX:= $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_CERTIFICATE:=PRESIGNED
include $(BUILD_PREBUILT)
还需要修改device.mk：**[html]**[view
 plain](http://blog.csdn.net/yaphet__s/article/details/45640627#)[copy](http://blog.csdn.net/yaphet__s/article/details/45640627#)

PRODUCT_PACKAGES +=\
PackageInstallerRenew\
Video\
TlrService\
Animation\   \#添加Animation模块

二，如何删除app，使当前app不会编进系统
在packages/apps目录下有安卓原生的app，有一些app是不需要的。
LOCAL_PACKAGE_NAMEpackages\apps\Bluetooth\Android.mk
**[html]**[view
 plain](http://blog.csdn.net/yaphet__s/article/details/45640627#)[copy](http://blog.csdn.net/yaphet__s/article/details/45640627#)

LOCAL_SRC_FILES:= \
$(call all-java-files-under, src)
LOCAL_PACKAGE_NAME:=Bluetooth5\#将Bluetooth重命名为Bluetooth5。也可更改generic_no_telephony.mk
\#保证和generic_no_telephony.mk增加的模块名字不一致即可
LOCAL_CERTIFICATE:=platform
特别注意，有的模块不一定在generic_no_telephony.mk中增加，可通过find . -name "*.mk" | xargs grep "Bluetooth"来查看在哪个mk里面声明了。由于大部分企业都没有统一的管理规范，一个模块有时候会在多个mk中增加，所以最快的方式就是更改packages\apps\Bluetooth\Android.mk的LOCAL_PACKAGE_NAME
 ，或者置空。


版权声明：本文为博主原创文章，未经博主允许不得转载。


