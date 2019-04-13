
# Android 默认把触摸屏show touches打开 - 嵌入式Linux - CSDN博客

2015年10月20日 15:08:54[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1892



```python
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ git diff packages/apps/Settings/src/com/android/settings/DevelopmentSettings.java
diff --git a/packages/apps/Settings/src/com/android/settings/DevelopmentSettings.java b/packages/apps/Settings/src/com/android/settings/DevelopmentSettings.java
index 2ac79ce..b87d98b 100644
--- a/packages/apps/Settings/src/com/android/settings/DevelopmentSettings.java
+++ b/packages/apps/Settings/src/com/android/settings/DevelopmentSettings.java
@@ -321,6 +321,9 @@ public class DevelopmentSettings extends RestrictedSettingsFragment
             mAllPrefs.add(hdcpChecking);
             removePreferenceForProduction(hdcpChecking);
         }
+       
+        Settings.System.putInt(getActivity().getContentResolver(),
+                Settings.System.SHOW_TOUCHES, 1);
     }
 
     private ListPreference addListPreference(String prefKey) {
```

Settings.System.putInt这个函数来设置，最后一个参数是０就是不选择，要不然就是选择复选框，

我把这段代码加在了onCreate的函数里面，启动的时候就会调用。


