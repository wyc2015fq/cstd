
# android 7.1 apk的systemuid [2] - 嵌入式Linux - CSDN博客

2019年03月19日 20:07:31[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：101



# 1
上周发的一篇文章《android 7.1 apk的systemuid和系统应用Setting相同导致开机找不到库的问题》，然后有个小伙伴找我讨论了，觉得自己的脑洞了打开了很多，所以针对这个问题再次做一次总结，如果大家有好的建议也可以在文末留言。
# 2
先说一个命令**adb shell dumpsys package p > dumpsys_p**，这个命令可以查看apk的一些状态，包括apk属于32位还是64位，保存成文件后查找里面对应的包名，就可以看到对应应用的信息。
![](https://img-blog.csdnimg.cn/2019031919243696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
看到primaryCpuAbi=arm64-v8a，说明我们的settings应用是64位的。
[
](https://img-blog.csdnimg.cn/2019031919243696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
# 3
[
](https://img-blog.csdnimg.cn/2019031919243696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)我们先是分析了出现问题时候的Settings，我们用原来的固件复现问题，在出现问题的时候用上面的命令查看，发现Settings已经变成了32位。
[
](https://img-blog.csdnimg.cn/2019031919243696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
> Package [com.android.settings] (27afcae):
userId=1000
sharedUser=SharedUserSetting{a6835fb android.uid.system/1000}
pkg=Package{6d89bbd com.android.settings}
codePath=/system/priv-app/Settings
resourcePath=/system/priv-app/Settings
legacyNativeLibraryDir=/system/priv-app/Settings/lib
primaryCpuAbi=armeabi-v7a
[
](https://img-blog.csdnimg.cn/2019031919243696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
# 4
[
](https://img-blog.csdnimg.cn/2019031919243696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)上篇文章提到，我们报错是因为没有找到库文件，还有下面的日志引起32位和64位不匹配的原因。
[
](https://img-blog.csdnimg.cn/2019031919243696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
> PackageManager: Instruction set mismatch, PackageSetting{1e08467

> com.xxxxxx.jjjjj.titket.oceanFlower/1000} requires arm whereas

> PackageSetting{51ca86c android.rockchip.update.service/1000} requires

> arm64 PackageManager: Instruction set mismatch, PackageSetting{1e08467

> com.xxxxxx.jjjjj.titket.oceanFlower/1000} requires arm whereas

> PackageSetting{992cb54 android/1000} requires arm64

> 我们的第一个尝试是，直接去掉原来修改的两个宏，然后在Setings安装位置system/priv-app/xxx/目录下面建立一个lib/arm64文件夹。这样复测20次，没有出现问题。原因是，我们的apk，发现当前安装位置下面有lib文件夹，优先从文件夹里去找库，而且是lib/arm64说明是64位的，让Settings运行在64位模式，我们的apk运行在32位模式。

> 5

> 添加前面文章的两个宏之后，在生成固件的apk里面多了一个class.dex，我们尝试把这个class.dex放到原来的Settings里面，然后再压缩把原来的Settings替换掉，这样复测20次，也是没有问题。所以这个class.dex起作用了。

> rm /data/system/packages.xml

> 恢复出厂设置

> 测试20次没有出现问题。

> 然后再查看Setting的运行模式

> userId=1000

> sharedUser=SharedUserSetting{842777 android.uid.system/1000}

> pkg=Package{7785b91 com.android.settings}

> codePath=/system/priv-app/Settings

> resourcePath=/system/priv-app/Settings

> legacyNativeLibraryDir=/system/priv-app/Settings/lib

> primaryCpuAbi=armeabi-v7a

> Seting变成了32位，就因为我们加了class.dex的原因。

> 6

> 后面大神给我了一个补丁，用来解决这个问题，补丁代码如下，希望对有这方面问题的同学有参考意义

> diff --git a/services/core/java/com/android/server/pm/PackageManagerService.java b/services/core/java/com/android/server/pm/PackageManagerService.java
index 12901ba..6d4e6fa 100755
--- a/services/core/java/com/android/server/pm/PackageManagerService.java
+++ b/services/core/java/com/android/server/pm/PackageManagerService.java
@@ -2572,7 +2572,7 @@ public class PackageManagerService extends IPackageManager.Stub {
                 // NOTE: We ignore potential failures here during a system scan (like
                 // the rest of the commands above) because there's precious little we
                 // can do about it. A settings error is reported, though.
-                adjustCpuAbisForSharedUserLPw(setting.packages, null /* scanned package */,
+                adjustCpuAbisForSharedUserLPw(setting.sameProcessPackages, null /* scanned package */,
                         false /* boot complete */);
             }
 
@@ -7222,12 +7222,18 @@ public class PackageManagerService extends IPackageManager.Stub {
     }
 
     private static String fixProcessName(String defProcessName,
-            String processName, int uid) {
-        if (processName == null) {
-            return defProcessName;
-        }
-        return processName;
-    }
+        String processName,PackageSetting ps) {
+        	String returnName = processName;
+        	if (returnName == null) {
+            	returnName =  defProcessName;
+        	}
+			if (returnName!=null && returnName.equals("system")) {
+				if (ps!=null && ps.sharedUser != null)
+				ps.sharedUser.addSameProcessPackage(ps);
+			}
+        return returnName;
+		}
+	
 
     private void verifySignaturesLP(PackageSetting pkgSetting, PackageParser.Package pkg)
             throws PackageManagerException {
@@ -8508,7 +8514,7 @@ public class PackageManagerService extends IPackageManager.Stub {
         pkg.applicationInfo.processName = fixProcessName(
                 pkg.applicationInfo.packageName,
                 pkg.applicationInfo.processName,
-                pkg.applicationInfo.uid);
+                pkgSetting);
 
         if (pkg != mPlatformPackage) {
             // Get all of our default paths setup
@@ -8774,7 +8780,7 @@ public class PackageManagerService extends IPackageManager.Stub {
             for (i=0; i<N; i++) {
                 PackageParser.Provider p = pkg.providers.get(i);
                 p.info.processName = fixProcessName(pkg.applicationInfo.processName,
-                        p.info.processName, pkg.applicationInfo.uid);
+                        p.info.processName,(PackageSetting)pkg.mExtras);
                 mProviders.addProvider(p);
                 p.syncable = p.info.isSyncable;
                 if (p.info.authority != null) {
@@ -8833,7 +8839,7 @@ public class PackageManagerService extends IPackageManager.Stub {
             for (i=0; i<N; i++) {
                 PackageParser.Service s = pkg.services.get(i);
                 s.info.processName = fixProcessName(pkg.applicationInfo.processName,
-                        s.info.processName, pkg.applicationInfo.uid);
+                        s.info.processName,(PackageSetting)pkg.mExtras);
                 mServices.addService(s);
                 if ((policyFlags&PackageParser.PARSE_CHATTY) != 0) {
                     if (r == null) {
@@ -8853,7 +8859,7 @@ public class PackageManagerService extends IPackageManager.Stub {
             for (i=0; i<N; i++) {
                 PackageParser.Activity a = pkg.receivers.get(i);
                 a.info.processName = fixProcessName(pkg.applicationInfo.processName,
-                        a.info.processName, pkg.applicationInfo.uid);
+                        a.info.processName,(PackageSetting)pkg.mExtras);
                 mReceivers.addActivity(a, "receiver");
                 if ((policyFlags&PackageParser.PARSE_CHATTY) != 0) {
                     if (r == null) {
@@ -8873,7 +8879,7 @@ public class PackageManagerService extends IPackageManager.Stub {
             for (i=0; i<N; i++) {
                 PackageParser.Activity a = pkg.activities.get(i);
                 a.info.processName = fixProcessName(pkg.applicationInfo.processName,
-                        a.info.processName, pkg.applicationInfo.uid);
+                        a.info.processName,(PackageSetting)pkg.mExtras);
                 mActivities.addActivity(a, "activity");
                 if ((policyFlags&PackageParser.PARSE_CHATTY) != 0) {
                     if (r == null) {
diff --git a/services/core/java/com/android/server/pm/SharedUserSetting.java b/services/core/java/com/android/server/pm/SharedUserSetting.java
old mode 100644
new mode 100755
index 06e020a..01897fb
--- a/services/core/java/com/android/server/pm/SharedUserSetting.java
+++ b/services/core/java/com/android/server/pm/SharedUserSetting.java
@@ -31,6 +31,7 @@ final class SharedUserSetting extends SettingBase {
     int uidPrivateFlags;
 
     final ArraySet<PackageSetting> packages = new ArraySet<PackageSetting>();
+	final ArraySet<PackageSetting> sameProcessPackages = new ArraySet<PackageSetting>();
 
     final PackageSignatures signatures = new PackageSignatures();
 
@@ -66,7 +67,11 @@ final class SharedUserSetting extends SettingBase {
             }
         }
     }
-
+	
+    void addSameProcessPackage(PackageSetting packageSetting) {
+		sameProcessPackages.add(packageSetting);
+    }
+		
     void addPackage(PackageSetting packageSetting) {
         if (packages.add(packageSetting)) {
             setFlags(this.pkgFlags | packageSetting.pkgFlags);
> 7

> 参考我之前的文章

> [https://blog.csdn.net/weiqifa0/article/details/88432165](https://blog.csdn.net/weiqifa0/article/details/88432165)

> 还有一些尝试方法还没有论证，喜欢研究的同学可以继续跟进

> 至于为何有同一个uid需要运行位数一致，大神找到了这样的解释

> Adjust instruction sets for shared UID apps.

> Since shared UID apps are run in the same process,
we'll need to make sure they're compiled for the same
instruction set.
![](https://img-blog.csdnimg.cn/20190319200523128.png)
> 欢迎加我的微信公众号/嵌入式Linux ，欢迎跟我讨论各种问题，人生，篮球，职业生涯 and so on…


