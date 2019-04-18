# WSL 服务自动启动的正确方法 - 知乎
# 



2018年 Windows 10 下的 WSL 已经可以保留后台进程了，从此后，用了十多年的 cygwin 基本失去了存在的价值了。网上有很多 WSL 自动启动服务的方法，但是都有些大大小小的问题，很多又是针对最老的 ubuntu16.04 发行版（输入 bash启动哪个），你如用商店里下载的最新的 WSL 版本 Debian9/Ubuntu18.04 就会出错。

所以正确在 WSL 里自动启动服务的方式有必要记录一下。

创建启动脚本：

进入任意 WSL 发行版中，创建并编辑文件：/etc/init.wsl


```bash
#! /bin/sh
/etc/init.d/cron $1
/etc/init.d/ssh $1
/etc/init.d/supervisor $1
```


里面调用了我们希望启动的三个服务的启动脚本，设置权限为可执行，所有者为 root，这时候可以通过：


```bash
sudo /etc/init.wsl [start|stop|restart]
```


来启停我们需要的服务，但是 sudo 时还是会需要我们输入密码，于是我们在 /etc/sudoers.d 目录下面建立 nopasswd 文件，内容为：


```bash
%sudo ALL=NOPASSWD: /etc/init.wsl
```


这时，sudo 调用该脚本就无需要输入密码了。

那么最后剩下在 Windows 启动或者登陆的时候执行该脚本的事情了，在 Windows 中，开始-运行，输入：

`shell: startup`

按照你 WSL 使用的 Linux 发行版创建启动脚本，比如我创建的 Debian.vbs 文件：


```
Set ws = CreateObject("Wscript.Shell")
ws.run "debian run sudo /etc/init.wsl start", vbhide
```


这个脚本就会在你登陆的时候自动在 debian/wsl 中执行 /etc/init.wsl 启动我们的服务了。

如果你用的是 ubuntu18.04 的发行版，那么修改上面脚本为 ubuntu1804.vbs：


```
Set ws = CreateObject("Wscript.Shell")
ws.run "ubuntu1804 run sudo /etc/init.wsl start", vbhide
```


而如果你用的是 wsl 下面最老的 ubuntu 16.04 ，就是以前输入 "bash" 启动的那个的话，它的启动方法和其他发行版稍微有点不一样，编辑 ubuntu1604.vbs：


```
Set ws = CreateObject("Wscript.Shell") 
ws.run "bash -c 'sudo /etc/init.wsl start'", vbhide
```


如此，不管你用最初的 bash (ubuntu 16.04) 还是商店里下载的 debian/ubuntu1804 都能顺利启动服务了。

WSL 中有很多有用的服务，你可以按需删改 /etc/init.wsl ，但没必要塞很多东西进去影响你的启动速度，比如 mysql/mongodb 这些重度服务，可以需要的时候再启动，用完就停了。

我自己用的比较多的服务就三个：sshd（启动了以后支持终端软件登陆wsl，像远程服务器一样操作），supervisord（用于托管其他服务，比如 ssr），crond（crontab，自由定制定时任务），大部分时候，用上面三个足以。




