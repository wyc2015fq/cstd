# 注册表修改之USB口启用 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年09月17日 12:54:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3341








建立reg文件自动进行：

1.建立扩展名.reg的文件；



2.编辑：

  Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/USBSTOR]
"Start"=dword:00000004

3.双击运行即可启用USB口；







可手动进行：

对注册表进行以下修改：

第一步：使用超级管理员登陆WINDOWS系统。

第二步：修改注册表键值。HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/USBSTOR   里面的Start 值为 4。  即，   Start值为4 是禁用u盘、 为3是启用u盘。



