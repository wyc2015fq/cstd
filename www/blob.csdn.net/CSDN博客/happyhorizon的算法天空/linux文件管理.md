# linux文件管理 - happyhorizon的算法天空 - CSDN博客
2015年11月11日 00:37:06[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：299
# linux文件管理
计算机操作系统都采用了目录树的文件结构。linux中：
|符号|名称|
|----|----|
|/|根目录|
|/bin|常见用户口令|
|/boot|内核和启动文件|
|/dev|设备文件|
|/home|系统默认的普通用户主目录|
|/etc|系统和服务配置文件|
|/lib|系统函数库目录|
|/lost+found|ext3文件系统需要的目录，用于磁盘检查|
|/opt|第三方软件安装目录|
|/proc|虚拟文件|
一般要安装系统时磁盘要分 / ，/boot， /home, /bin, swap分区几部分。
