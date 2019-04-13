
# android 7.1 apk的systemuid相同导致问题[2] - 嵌入式Linux - CSDN博客

2019年03月22日 18:18:24[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：3


![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPddtGC4PgjQLRiar47gHFJBX4jgntl9sJPjMXj6gOdqdZqYAN1xpjdvEwyBDMAicrjYLEn8lA719snw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPeiauStJMDaoxoicA9rgjdWtkKj2ZZA5bCEcib9Z8bkfqBBZoO2eItzHChicibqZlWc4dP9LHIJIUGGLicg/640?wx_fmt=jpeg)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPd3pDeEtePDsNGbO6Q6AhH7vSYG8ygKrWwk9jPucDicgJVWfuO0BZVian5VPqZc3NCrAlHfwsMaTANw/640?wx_fmt=png)
*1*
上周发的一篇文章《android 7.1 apk的systemuid和系统应用Setting相同导致开机找不到库的问题》，然后有个小伙伴找我讨论了，觉得自己的脑洞了打开了很多，所以针对这个问题再次做一次总结，如果大家有好的建议也可以在文末留言。
*2*
先说一个命令**adb shell dumpsys package p > dumpsys_p**，这个命令可以查看apk的一些状态，包括apk属于32位还是64位，保存成文件后查找里面对应的包名，就可以看到对应应用的信息。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPceYLI1FxpricSiacR7VZ0216zczBEufGotlmspAWNoMkib8qqv6xbKAt8jjQnjlA9dsRenHRYf1Yf8Q/640?wx_fmt=png)*3*
我们先是分析了出现问题时候的Settings，我们用原来的固件复现问题，在出现问题的时候用上面的命令查看，发现Settings已经变成了32位。
> Package com.android.settings:

> userId=1000

> sharedUser=SharedUserSetting{a6835fb android.uid.system/1000}

> pkg=Package{6d89bbd com.android.settings}

> codePath=/system/priv-app/Settings

> resourcePath=/system/priv-app/Settings

> legacyNativeLibraryDir=/system/priv-app/Settings/lib

> primaryCpuAbi=armeabi-v7a

*4*
上篇文章提到，我们报错是因为没有找到库文件，还有下面的日志引起32位和64位不匹配的原因。
> PackageManager: Instruction set mismatch, PackageSetting{1e08467

> com.xxxxxx.jjjjj.titket.oceanFlower/1000} requires arm whereas

> PackageSetting{51ca86c android.rockchip.update.service/1000} requires

> arm64 PackageManager: Instruction set mismatch, PackageSetting{1e08467

> com.xxxxxx.jjjjj.titket.oceanFlower/1000} requires arm whereas

> PackageSetting{992cb54 android/1000} requires arm64
我们的第一个尝试是，直接去掉原来修改的两个宏，然后在Setings安装位置system/priv-app/xxx/目录下面建立一个lib/arm64文件夹。这样复测20次，没有出现问题。原因是，我们的apk，发现当前安装位置下面有lib文件夹，优先从文件夹里去找库，而且是lib/arm64说明是64位的，让Settings运行在64位模式，我们的apk运行在32位模式。
*5*
添加前面文章的两个宏之后，在生成固件的apk里面多了一个class.dex，我们尝试把这个class.dex放到原来的Settings里面，然后再压缩把原来的Settings替换掉，这样复测20次，也是没有问题。所以这个class.dex起作用了。
rm /data/system/packages.xml
恢复出厂设置
测试20次没有出现问题。
**然后再查看Setting的运行模式**
> userId=1000

> sharedUser=SharedUserSetting{842777 android.uid.system/1000}

> pkg=Package{7785b91 com.android.settings}

> codePath=/system/priv-app/Settings

> resourcePath=/system/priv-app/Settings

> legacyNativeLibraryDir=/system/priv-app/Settings/lib

> primaryCpuAbi=armeabi-v7a
Seting变成了32位，就因为我们加了class.dex的原因。
*6*
后面大神给我了一个补丁，用来解决这个问题，补丁代码如下，希望对有这方面问题的同学有参考意义
```python
diff --git a/services/core/java/com/android/server/pm/PackageManagerService.java b/services/core/java/com/android/server/pm/PackageManagerService.java
```

```python
index
```
```python
12901
```
```python
ba..
```
```python
6
```
```python
d4e6fa
```
```python
100755
```

```python
--- a/services/core/java/com/android/server/pm/PackageManagerService.java
```

```python
+++ b/services/core/java/com/android/server/pm/PackageManagerService.java
```

```python
@@ -
```
```python
2572
```
```python
,
```
```python
7
```
```python
+
```
```python
2572
```
```python
,
```
```python
7
```
```python
@@
```
```python
public
```
```python
class
```
```python
PackageManagerService
```
```python
extends
```
```python
IPackageManager
```
```python
.
```
```python
Stub
```
```python
{
```

```python
//
```
```python
NOTE:
```
```python
We ignore potential failures here during a system scan (like
```

```python
// the rest of the commands above) because there's precious little we
```

```python
// can do about it. A settings error is reported, though.
```

```python
-                adjustCpuAbisForSharedUserLPw(setting.packages,
```
```python
null
```
```python
/* scanned package */
```
```python
,
```

```python
+                adjustCpuAbisForSharedUserLPw(setting.sameProcessPackages,
```
```python
null
```
```python
/* scanned package */
```
```python
,
```

```python
false
```
```python
/* boot complete */
```
```python
);
```

```python
}
```

```python
@@ -
```
```python
7222
```
```python
,
```
```python
12
```
```python
+
```
```python
7222
```
```python
,
```
```python
18
```
```python
@@
```
```python
public
```
```python
class
```
```python
PackageManagerService
```
```python
extends
```
```python
IPackageManager
```
```python
.
```
```python
Stub
```
```python
{
```

```python
}
```

```python
private
```
```python
static
```
```python
String
```
```python
fixProcessName
```
```python
(String defProcessName,
```

```python
-            String processName,
```
```python
int
```
```python
uid)
```
```python
{
```

```python
-
```
```python
if
```
```python
(processName ==
```
```python
null
```
```python
) {
```

```python
-
```
```python
return
```
```python
defProcessName;
```

```python
-        }
```

```python
-
```
```python
return
```
```python
processName;
```

```python
-    }
```

```python
+        String processName,PackageSetting ps) {
```

```python
+            String returnName = processName;
```

```python
+
```
```python
if
```
```python
(returnName ==
```
```python
null
```
```python
) {
```

```python
+                returnName =  defProcessName;
```

```python
+            }
```

```python
+
```
```python
if
```
```python
(returnName!=
```
```python
null
```
```python
&& returnName.equals(
```
```python
"system"
```
```python
)) {
```

```python
+
```
```python
if
```
```python
(ps!=
```
```python
null
```
```python
&& ps.sharedUser !=
```
```python
null
```
```python
)
```

```python
+                ps.sharedUser.addSameProcessPackage(ps);
```

```python
+            }
```

```python
+
```
```python
return
```
```python
returnName;
```

```python
+        }
```

```python
+
```

```python
private
```
```python
void
```
```python
verifySignaturesLP
```
```python
(PackageSetting pkgSetting, PackageParser.Package pkg)
```

```python
throws
```
```python
PackageManagerException
```
```python
{
```

```python
@@ -
```
```python
8508
```
```python
,
```
```python
7
```
```python
+
```
```python
8514
```
```python
,
```
```python
7
```
```python
@@
```
```python
public
```
```python
class
```
```python
PackageManagerService
```
```python
extends
```
```python
IPackageManager
```
```python
.
```
```python
Stub
```
```python
{
```

```python
pkg.applicationInfo.processName = fixProcessName(
```

```python
pkg.applicationInfo.packageName,
```

```python
pkg.applicationInfo.processName,
```

```python
-                pkg.applicationInfo.uid);
```

```python
+                pkgSetting);
```

```python
if
```
```python
(pkg != mPlatformPackage) {
```

```python
// Get all of our default paths setup
```

```python
@@ -
```
```python
8774
```
```python
,
```
```python
7
```
```python
+
```
```python
8780
```
```python
,
```
```python
7
```
```python
@@
```
```python
public
```
```python
class
```
```python
PackageManagerService
```
```python
extends
```
```python
IPackageManager
```
```python
.
```
```python
Stub
```
```python
{
```

```python
for
```
```python
(i=
```
```python
0
```
```python
; i<N; i++) {
```

```python
PackageParser.Provider p = pkg.providers.get(i);
```

```python
p.info.processName = fixProcessName(pkg.applicationInfo.processName,
```

```python
-                        p.info.processName, pkg.applicationInfo.uid);
```

```python
+                        p.info.processName,(PackageSetting)pkg.mExtras);
```

```python
mProviders.addProvider(p);
```

```python
p.syncable = p.info.isSyncable;
```

```python
if
```
```python
(p.info.authority !=
```
```python
null
```
```python
) {
```

```python
@@ -
```
```python
8833
```
```python
,
```
```python
7
```
```python
+
```
```python
8839
```
```python
,
```
```python
7
```
```python
@@
```
```python
public
```
```python
class
```
```python
PackageManagerService
```
```python
extends
```
```python
IPackageManager
```
```python
.
```
```python
Stub
```
```python
{
```

```python
for
```
```python
(i=
```
```python
0
```
```python
; i<N; i++) {
```

```python
PackageParser.Service s = pkg.services.get(i);
```

```python
s.info.processName = fixProcessName(pkg.applicationInfo.processName,
```

```python
-                        s.info.processName, pkg.applicationInfo.uid);
```

```python
+                        s.info.processName,(PackageSetting)pkg.mExtras);
```

```python
mServices.addService(s);
```

```python
if
```
```python
((policyFlags&PackageParser.PARSE_CHATTY) !=
```
```python
0
```
```python
) {
```

```python
if
```
```python
(r ==
```
```python
null
```
```python
) {
```

```python
@@ -
```
```python
8853
```
```python
,
```
```python
7
```
```python
+
```
```python
8859
```
```python
,
```
```python
7
```
```python
@@
```
```python
public
```
```python
class
```
```python
PackageManagerService
```
```python
extends
```
```python
IPackageManager
```
```python
.
```
```python
Stub
```
```python
{
```

```python
for
```
```python
(i=
```
```python
0
```
```python
; i<N; i++) {
```

```python
PackageParser.Activity a = pkg.receivers.get(i);
```

```python
a.info.processName = fixProcessName(pkg.applicationInfo.processName,
```

```python
-                        a.info.processName, pkg.applicationInfo.uid);
```

```python
+                        a.info.processName,(PackageSetting)pkg.mExtras);
```

```python
mReceivers.addActivity(a,
```
```python
"receiver"
```
```python
);
```

```python
if
```
```python
((policyFlags&PackageParser.PARSE_CHATTY) !=
```
```python
0
```
```python
) {
```

```python
if
```
```python
(r ==
```
```python
null
```
```python
) {
```

```python
@@ -
```
```python
8873
```
```python
,
```
```python
7
```
```python
+
```
```python
8879
```
```python
,
```
```python
7
```
```python
@@
```
```python
public
```
```python
class
```
```python
PackageManagerService
```
```python
extends
```
```python
IPackageManager
```
```python
.
```
```python
Stub
```
```python
{
```

```python
for
```
```python
(i=
```
```python
0
```
```python
; i<N; i++) {
```

```python
PackageParser.Activity a = pkg.activities.get(i);
```

```python
a.info.processName = fixProcessName(pkg.applicationInfo.processName,
```

```python
-                        a.info.processName, pkg.applicationInfo.uid);
```

```python
+                        a.info.processName,(PackageSetting)pkg.mExtras);
```

```python
mActivities.addActivity(a,
```
```python
"activity"
```
```python
);
```

```python
if
```
```python
((policyFlags&PackageParser.PARSE_CHATTY) !=
```
```python
0
```
```python
) {
```

```python
if
```
```python
(r ==
```
```python
null
```
```python
) {
```

```python
diff --git a/services/core/java/com/android/server/pm/SharedUserSetting.java b/services/core/java/com/android/server/pm/SharedUserSetting.java
```

```python
old mode
```
```python
100644
```

```python
new
```
```python
mode
```
```python
100755
```

```python
index
```
```python
06e020
```
```python
a..
```
```python
01897f
```
```python
b
```

```python
--- a/services/core/java/com/android/server/pm/SharedUserSetting.java
```

```python
+++ b/services/core/java/com/android/server/pm/SharedUserSetting.java
```

```python
@@ -
```
```python
31
```
```python
,
```
```python
6
```
```python
+
```
```python
31
```
```python
,
```
```python
7
```
```python
@@
```
```python
final
```
```python
class
```
```python
SharedUserSetting
```
```python
extends
```
```python
SettingBase
```
```python
{
```

```python
int
```
```python
uidPrivateFlags;
```

```python
final
```
```python
ArraySet<PackageSetting> packages =
```
```python
new
```
```python
ArraySet<PackageSetting>();
```

```python
+
```
```python
final
```
```python
ArraySet<PackageSetting> sameProcessPackages =
```
```python
new
```
```python
ArraySet<PackageSetting>();
```

```python
final
```
```python
PackageSignatures signatures =
```
```python
new
```
```python
PackageSignatures();
```

```python
@@ -
```
```python
66
```
```python
,
```
```python
7
```
```python
+
```
```python
67
```
```python
,
```
```python
11
```
```python
@@
```
```python
final
```
```python
class
```
```python
SharedUserSetting
```
```python
extends
```
```python
SettingBase
```
```python
{
```

```python
}
```

```python
}
```

```python
}
```

```python
-
```

```python
+
```

```python
+
```
```python
void
```
```python
addSameProcessPackage
```
```python
(PackageSetting packageSetting)
```
```python
{
```

```python
+        sameProcessPackages.add(packageSetting);
```

```python
+    }
```

```python
+
```

```python
void
```
```python
addPackage
```
```python
(PackageSetting packageSetting)
```
```python
{
```

```python
if
```
```python
(packages.add(packageSetting)) {
```

```python
setFlags(
```
```python
this
```
```python
.pkgFlags | packageSetting.pkgFlags);
```

*7*
参考我之前的文章
还有一些尝试方法还没有论证，喜欢研究的同学可以继续跟进
至于为何有同一个uid需要运行位数一致，大神找到了这样的解释
> Adjust instruction sets for shared UID apps.

> Since shared UID apps are run in the same process,

> we'll need to make sure they're compiled for the same

> instruction set.


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPceYLI1FxpricSiacR7VZ02166adO3Imgy9m3fFXnsEl9dbVGZEeLFxrRQ5ssWs7XyD2LNyml69LIWg/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPd3pDeEtePDsNGbO6Q6AhH7vSYG8ygKrWwk9jPucDicgJVWfuO0BZVian5VPqZc3NCrAlHfwsMaTANw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPdXdyYbmtOKz0y00NnSLa5ufTZnjGyW9YWmh4YJGRHcIpuDH6yXV9qe7cE40cV0ibicZfrN8UboDS8A/640?wx_fmt=png)





