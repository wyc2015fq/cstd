
# Android Adb 源码分析 - 嵌入式Linux - CSDN博客

2018年10月23日 17:39:03[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：291个人分类：[Android																](https://blog.csdn.net/weiqifa0/article/category/1752017)



## 扭起屁股得意洋洋
最近，我负责的项目因为临近量产，把之前的userdebug版本关闭，转成了user版本，增加selinux的权限，大家都洋溢在项目准备量产的兴奋和喜悦之中不能自拔
谁知，好景不长，user版本发布之后，各种bug接踵而来，但是因为user版本权限的原因，我们之前保留在/data/logs/下面的日志不能pull出来，定位问题非常困难

## 不得不想到的解决方案
**第一个办法：**我们想到的第一个办法就是更改data目录的权限，改成system用户，但是因为data下面的logs目录的文件是root权限，获取日志是需要root权限的，日志还是不能pullg出来。
![](https://pic2.zhimg.com/80/v2-51975c03ab9bdfb9994df1f8c30de2bd_hd.jpg)
**第二个办法:**我想到的第二个办法就是给我们的adb命令增加一个后门，正常我们是adb root获取root权限，我修改成adb aaa.bbb.ccc.root 这样不容易被别人窃取我们的后门，也不至于影响到我们的开发。
## 梳理Android ADB知识点
所以就加强了adb 的相关知识
**google的adb 代码位置在（system/core/adb）目录下面**
**我上传了一份在github上面，链接如下**
[Android ADB源码​github.com](http://link.zhihu.com/?target=https%3A//github.com/weiqifa0/android-adb)
ADB是Android系统提供的调试工具，整个ADB工具由三部分组成：adb client、adb service、adb daemon。
**1、ADB client**
提供HOST端运行的命令
**2、ADB service**
HOST端上的一个后台进程
**3、ADB daemom**
DEVICE端（真实的机器或者模拟器）的守护进程
这三部分都是从（system/core/adb）里面编译出来的，我们很多时候去网上下载adb.exe来用，实际上我们的SDK代码下面就有adb,而且代码是可以修改的。
ADB代码位于/system/core/adb目录下，**[通过查看Android.mk](http://link.zhihu.com/?target=https%3A//github.com/weiqifa0/android-adb/blob/master/Android.mk)**，可以知道，该目录下的代码生成了两个MODULE，分别是**adb**和**adbd**,**adb client和adb service都是由adb这个可执行文件实现**，**adb daemon由adbd实现**。adb和adbd的一些源代码文件是用同一个的，编译时通过**LOCAL_CFLAGS**的参数**ADB_HOST**来区分，这种你中有我我中有你的关系，对于初次接触的朋友们，多少增加了些困扰。理清了ADB几部分的关系，以及源代码的结构，对ADB的认识已经有一个飞越了。

## 使用方案2来解决问题
**代码修改如下**
`diff --git a/adb/commandline.cpp b/adb/commandline.cpp
old mode 100644
new mode 100755
index 51d828a..32b2c09
--- a/adb/commandline.cpp
+++ b/adb/commandline.cpp
@@ -83,6 +83,7 @@ static void help() {
     fprintf(stderr, "%s\n", adb_version().c_str());
     // clang-format off
     fprintf(stderr,
+    	"ADB use for weiqifa nan Product\n"
         " -a                            - directs adb to listen on all interfaces for a connection\n"
         " -d                            - directs command to the only connected USB device\n"
         "                                 returns an error if more than one USB device is present.\n"
@@ -1083,6 +1084,7 @@ static bool adb_root(const char* command) {
     std::string error;
     ScopedFd fd;
 
+	fprintf(stderr, "weiqifa adb root \n");
     fd.Reset(adb_connect(android::base::StringPrintf("%s:", command), &error));
     if (!fd.valid()) {
         fprintf(stderr, "adb: unable to connect for %s: %s\n", command, error.c_str());
@@ -1625,12 +1627,12 @@ int adb_commandline(int argc, const char **argv) {
         } else if (argc == 2 && !strcmp(argv[1], "-l")) {
             listopt = argv[1];
         } else {
-            fprintf(stderr, "Usage: adb devices [-l]\n");
+            fprintf(stderr, "weiqifa Usage: adb devices [-l]\n");
             return 1;
         }
 
         std::string query = android::base::StringPrintf("host:%s%s", argv[0], listopt);
-        printf("List of devices attached\n");
+        printf("weiqifa List of devices attached\n");
         return adb_query_command(query);
     }
     else if (!strcmp(argv[0], "connect")) {
@@ -1732,7 +1734,7 @@ int adb_commandline(int argc, const char **argv) {
             command = android::base::StringPrintf("%s:", argv[0]);
         }
         return adb_connect_command(command);
-    } else if (!strcmp(argv[0], "root") || !strcmp(argv[0], "unroot")) {
+    } else if (!strcmp(argv[0], "weiqifa.nan.root") || !strcmp(argv[0], "unroot")) {
         return adb_root(argv[0]) ? 0 : 1;
     } else if (!strcmp(argv[0], "bugreport")) {
         Bugreport bugreport;
diff --git a/adb/services.cpp b/adb/services.cpp
old mode 100644
new mode 100755
index 3b212e9..5a82246
--- a/adb/services.cpp
+++ b/adb/services.cpp
@@ -69,7 +69,7 @@ static void service_bootstrap_func(void* x) {
 
 void restart_root_service(int fd, void *cookie) {
     if (getuid() == 0) {
-        WriteFdExactly(fd, "adbd is already running as root\n");
+        WriteFdExactly(fd, "weiqifa.nan adbd is already running as root\n");
         adb_close(fd);
     } else {
         char value[PROPERTY_VALUE_MAX];
@@ -81,7 +81,7 @@ void restart_root_service(int fd, void *cookie) {
         }
 
         property_set("service.adb.root", "1");
-        WriteFdExactly(fd, "restarting adbd as root\n");
+        WriteFdExactly(fd, "weiqifa.nan restarting adbd as root\n");
         adb_close(fd);
     }
 }
@@ -327,7 +327,8 @@ int service_to_fd(const char* name, const atransport* transport) {
         void* arg = strdup(name + 7);
         if (arg == NULL) return -1;
         ret = create_service_thread(reboot_service, arg);
-    } else if(!strncmp(name, "root:", 5)) {
+    } else if(!strncmp(name, "weiqifa.nan.root:", 17)) {
+    	fprintf(stderr, "services adb root");
         ret = create_service_thread(restart_root_service, NULL);
     } else if(!strncmp(name, "unroot:", 7)) {
         ret = create_service_thread(restart_unroot_service, NULL);
diff --git a/adb/sockets.cpp b/adb/sockets.cpp
index 63b7df6..1cb0b5e 100644
--- a/adb/sockets.cpp
+++ b/adb/sockets.cpp
@@ -418,11 +418,11 @@ asocket* create_local_service_socket(const char* name, const atransport* transpo
 
 #if !ADB_HOST
     char debug[PROPERTY_VALUE_MAX];
-    if (!strncmp(name, "root:", 5)) {
+    if (!strncmp(name, "weiqifa.nan.root:", 17)) {
         property_get("ro.debuggable", debug, "");
     }
 
-    if ((!strncmp(name, "root:", 5) && getuid() != 0 && strcmp(debug, "1") == 0) ||
+    if ((!strncmp(name, "weiqifa.nan.root:", 17) && getuid() != 0 && strcmp(debug, "1") == 0) ||
         (!strncmp(name, "unroot:", 7) && getuid() == 0) ||
         !strncmp(name, "usb:", 4) ||
         !strncmp(name, "tcpip:", 6)) {`
## 编译
**Android sdk编译请看链接**
[嵌入式Linux：Android7.1 howto_build_SDK​zhuanlan.zhihu.com](https://zhuanlan.zhihu.com/p/47447296)
**1、一个是编译生成adb.exe，这个拷贝到windows下面使用**
**Android 7.1使用 编译指令使用" make host_cross_adb -j40 "**
**Android 7.0 之前使用 make USE_MINGW=y adb**
**但是之前要先**
**source build/envsetup.sh**
**lunch**
**建立Android 编译环境**
![](https://pic2.zhimg.com/80/v2-ec58cec4e6fce26832b77fe1db0a56c5_hd.jpg)
**2、编译adbd 服务，这个是烧录到机器里面去，直接编译整个固件就好了**
**source build/envsetup.sh; lunch rk3399_mid-userdebug; make -j128**
adbd 在init.rc里面初始化，具体代码在devices/rockchip/下面找
`# for Internet adb 
on property:persist.internet.adb.enable=1
    setprop service.adb.tcp.port 5555
    restart adbd
# for Internet adb 
on property:persist.internet.adb.enable=0
    setprop service.adb.tcp.port 0
    restart adbd
# for telephony function
on property:ro.boot.noril=true
    setprop ro.radio.noril true
    stop ril-daemon`
**这一章先大概说下代码，只有写下root的原理~~**
**如果觉得不错，帮忙关注微信公众号，嵌入式Linux**
![](https://pic3.zhimg.com/80/v2-f2126f778832e43009ce36129459d2e2_hd.jpg)

