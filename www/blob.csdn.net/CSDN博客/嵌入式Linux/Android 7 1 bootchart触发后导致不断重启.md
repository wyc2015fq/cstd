
# Android 7.1 bootchart触发后导致不断重启 - 嵌入式Linux - CSDN博客

2019年03月07日 11:37:09[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：54



## android7.1 bootchart
android 7.1的bootchart问题比较多，之前的版本还是比较稳定的，但是7.1之后出现了问题。
由於7.0 bootchart.cpp中有bug, google在7.1上還沒有修复，当我们执行下面的命令后
`adb shell 'echo 120 > /data/bootchart/start'`手机会不断的重启。
详情请参考：system/core/init/readme.txt
## 修复不断重启的补丁
补丁打在system/core/init 下面的代码
`From d41070cb131d169e0a1606c4b7213bf8b4752308 Mon Sep 17 00:00:00 2001
From: wuliangqing <wlq@rock-chips.com>
Date: Wed, 6 Jun 2018 10:39:08 +0800
Subject: [PATCH] temp: fix bootchart crash
---
 init/bootchart.cpp | 4 ++--
 init/init.c        | 0
 2 files changed, 2 insertions(+), 2 deletions(-)
 mode change 100644 => 100755 init/bootchart.cpp
 create mode 100644 init/init.c
diff --git a/init/bootchart.cpp b/init/bootchart.cpp
old mode 100644
new mode 100755
index 5704d28..9c28ba1
--- a/init/bootchart.cpp
+++ b/init/bootchart.cpp
@@ -129,7 +129,7 @@ static void do_log_procs(FILE* log) {
             snprintf(filename, sizeof(filename), "/proc/%d/cmdline", pid);
             std::string cmdline;
             android::base::ReadFileToString(filename, &cmdline);
-            const char* full_name = cmdline.c_str(); // So we stop at the first NUL.
+           // const char* full_name = cmdline.c_str(); // So we stop at the first NUL.
 
             // Read process stat line.
             snprintf(filename, sizeof(filename), "/proc/%d/stat", pid);
@@ -140,7 +140,7 @@ static void do_log_procs(FILE* log) {
                     size_t open = stat.find('(');
                     size_t close = stat.find_last_of(')');
                     if (open != std::string::npos && close != std::string::npos) {
-                        stat.replace(open + 1, close - open - 1, full_name);
+                       // stat.replace(open + 1, close - open - 1, full_name);
                     }
                 }
                 fputs(stat.c_str(), log);
diff --git a/init/init.c b/init/init.c
new file mode 100644
index 0000000..e69de29
-- 
1.9.1`

