# mongoDB学习笔记——安装及启动 - 左直拳的马桶_日用桶 - CSDN博客
2014年02月06日 22:04:16[左直拳](https://me.csdn.net/leftfist)阅读数：7224
WINDOWS环境下：
一、安装
步骤一:  下载MongoDB 
url下载地址:  http://downloads.mongodb.org/win32/
步骤二:  设置MongoDB程序存放目录 
例如，将其解压到 c:\，再重命名为mongo，路径为c:\mongo。
步骤三:  设置数据文件设置存放目录 
例如，在c:盘建一个db文件夹，路径c:\db 
步骤四:  启动MongoDB服务 
进入 cmd 提示符控制台，c:\mongo\bin\mongod.exe --dbpath=c:\db 。
可以将c:\mongo\bin\写到环境变量里，不必每次都写这个路径。
--dbpath是一个参数。
可以用 mongod /? 或者 mongod --help查看其他参数。比如
--logpath 日志目录
--journal 应用日志启动。
--repairpath 修复（因为我们安装到默认的目录data\db，所以要指明目录名）
for windows参数
install 安装成服务
remove 卸载服务
mongodb启动以后，如果因为种种意外原因，比如宕机，那么下次mongodb可能就启动不来，就需要修复，用到这个repair或者repairpath参数。据说启动的时候用上这个journal参数，就会严谨很多，避免这个缺陷。
步骤五、连接
新打开一个CMD输入：c:\mongo\bin\mongo，开始MongoDB之旅 
步骤六、断开连接
连接mongoDB后，想断开的话，可以 输入exit或者是ctrl + c
步骤七、关闭数据库
连接的情况下，use admin，切换到admin，然后输入db.shutdownServer()。
补充： 如果想开机即启动，可以将MongoDB作为 
Windows 服务随机启动  先创建C:\mongo\logs\mongodb.log文件，用于存储MongoDB的日志文件, 再安装系统服务。
C:\mongo\bin\mongod  --dbpath=c:\ db --logpath=c:\mongo\lo gs\mongodb.log --journal --install 
卸载的话，将install换成remove
===================================
如果MongoDB已经安装成为WINDOWS的服务，不知为何，、
无论是在mongodb服务中，输入
>use admin
>db.shutdownServer()
还是在WINDOWS的CMD中，输入
net stop MongoDB
都可以将服务停止，但同时会报错。
