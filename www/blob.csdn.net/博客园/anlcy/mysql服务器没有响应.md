
# mysql服务器没有响应 - anlcy - 博客园






# [mysql服务器没有响应](https://www.cnblogs.com/camilla/p/7616074.html)
第一步
删除c:\windowns下面的my.ini（可以先改成其它的名字也行）
第二步
打开对应安装目录下\mysql\bin\winmysqladmin.exe 输入用户名 和密码（也可以忽略此步）
第三步 在dos下 ，进入mysql的bin目录。
例：安装目录为d盘的PC_webserver\mysql\bin
开始-运行-cmd
d:     //先切换到d盘下
D:\>cd PC_webserver\mysql\bin //通过cd目录进入bin下
然后输入 mysqld -remove 删除服务
接着输入 mysqld -install
第四步  再重新启动下mysql服务 一般可以修复报错的问题
Win7的80端口默认是被System进程占用的。（我的电脑是win10也是这个原因）
用如下方法可以解决System进程占用80端口的问题：
打开RegEdit:开始-运行-输入regedit-调出注册表  找到HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\HTTP 找到一个DWORD值Start，将其改为0 重启电脑，System进程将不会占用80端口 就可以打开你的apache了
1.png
（下面这段话还没亲身实践，摘自助我解决问题大神的原稿）
这种方法确实暂时解决了我的问题，应该就是这样的，但是后面又来问题了。Apache又无缘无故的打不开了。于是，最后终于发现了进程有个 SQL Server Reporting  Services ，这个是sql servers带有的，而且自动运行，关闭它，就可以了。但是切记，可能这个进程会自动打开，所以先把apache打开，这个的话后面SQL Server Reporting Services运行也不会占用80





