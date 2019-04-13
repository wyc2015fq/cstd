
# Service rootservice does not have a SELinux domain defined - 嵌入式Linux - CSDN博客

2018年10月18日 15:49:04[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：205



# 新增一个SELINUX启动的时候出现问题
新增如下
`service rootservice /system/bin/rootservice
    user root
    group root
    disabled
    oneshot
on property:sys.boot_completed=1
    start rootservice`具体补丁请查看文件
链接：[https://pan.baidu.com/s/1AlBuXfdn-xFRwDwmsX4svA](https://pan.baidu.com/s/1AlBuXfdn-xFRwDwmsX4svA)
提取码：bbyi
# 发现问题
最后发现在 device/rockchip/common/里面少打了一行代码
`diff --git a/sepolicy/rootservice.te b/sepolicy/rootservice.te
new file mode 100644
index 0000000..e1b4198
--- /dev/null
+++ b/sepolicy/rootservice.te
@@ -0,0 +1,5 @@
+type rootservice, domain, mlstrustedsubject;
+type rootservice_exec, exec_type, file_type;
+
+diff --git a/sepolicy/rootservice.te b/sepolicy/rootservice.te
new file mode 100644
index 0000000..e1b4198
--- /dev/null
+++ b/sepolicy/rootservice.te
@@ -0,0 +1,5 @@
+type rootservice, domain, mlstrustedsubject;
+type rootservice_exec, exec_type, file_type;
+
+init_daemon_domain(rootservice)
+
+`少打了init_daemon_domain(rootservice)

