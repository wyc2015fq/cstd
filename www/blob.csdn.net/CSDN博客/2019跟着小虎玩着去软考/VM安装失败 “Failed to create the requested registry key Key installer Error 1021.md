# VM安装失败 “Failed to create the requested registry key Key:installer Error:1021" - 2019跟着小虎玩着去软考 - CSDN博客
2011年06月09日 13:23:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：12488

## **VM****安装失败 “Failed tocreate the requested registry key Key:installer Error:1021"**
# **问题描述：**
安装虚拟机VMWare Workstation7.1时出现failed to create the requested registry key key installer error 1021
点击“确定”后退出
# **解决方法：**
在注册表（开始--运行--输入regedit）中找到HKEY_LOCAL_MACHINESOFTWAREVMware,
Inc
将VMware, Inc直接删除即可
