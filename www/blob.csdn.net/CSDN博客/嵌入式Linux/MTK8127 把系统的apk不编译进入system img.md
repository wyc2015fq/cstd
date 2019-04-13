
# MTK8127 把系统的apk不编译进入system.img - 嵌入式Linux - CSDN博客

2015年09月21日 15:58:14[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1082


+++ b/packages/apps/Email/Android.mk
@@ -27,7 +27,7 @@ emailcommon_dir := emailcommon
gridlayout_dir := ../../../frameworks/support/v7/gridlayout/res
res_dir := res $(unified_email_dir)/res $(chips_dir) $(photo_dir) $(emailcommon_dir)/res $(gridlayout_dir)
-LOCAL_MODULE_TAGS := optional
+LOCAL_MODULE_TAGS := tests
LOCAL_SRC_FILES := $(call all-java-files-under, $(unified_email_dir)/src)
LOCAL_SRC_FILES += $(call all-java-files-under, src/com/android)


LOCAL_MODULE_TAGS 这个属性决定是否编译进去，然后我们就按着这个来添加。


