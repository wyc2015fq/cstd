# Window系统小设置 - 长歌行 - CSDN博客





2013年01月25日 17:13:28[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：545
个人分类：[生活](https://blog.csdn.net/iuhsihsow/article/category/1065737)









1.多媒体键盘

最近刚入手了一个罗技k310，但是多媒体按键不支持，虽然不是很需要，但是聊胜于无。




如果使用的是多媒体键盘，但是功能键无效，很有可能是由于多媒体服务没有打开

可以在管理工具中选择服务，打开HID Input Service服务




有可能会提示错误，**错误1083：配置成在该可执行程序中运行的这个服务不能执行该服务。**



**在开始-运行中，输入regedit，进入以下注册表项：HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\SvcHost在右侧定位到netsvcs，右键修改数值，在其数值数据里添加Tapisrv，重启就可以了。**









