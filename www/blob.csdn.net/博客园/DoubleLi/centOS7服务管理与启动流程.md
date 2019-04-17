# centOS7服务管理与启动流程 - DoubleLi - 博客园









- [centOS7启动流程](https://www.cnblogs.com/duzhaoqi/p/7582404.html#centos7启动流程)- [systemd简介](https://www.cnblogs.com/duzhaoqi/p/7582404.html#systemd简介)
- [unit对象](https://www.cnblogs.com/duzhaoqi/p/7582404.html#unit对象)- [unit类型](https://www.cnblogs.com/duzhaoqi/p/7582404.html#unit类型)
- [特性](https://www.cnblogs.com/duzhaoqi/p/7582404.html#特性)

- [service unit文件格式](https://www.cnblogs.com/duzhaoqi/p/7582404.html#service-unit文件格式)- [service unit file文件通常由三部分组成](https://www.cnblogs.com/duzhaoqi/p/7582404.html#service-unit-file文件通常由三部分组成)
- [unit段的常用选项](https://www.cnblogs.com/duzhaoqi/p/7582404.html#unit段的常用选项)
- [Service段的常用选项](https://www.cnblogs.com/duzhaoqi/p/7582404.html#service段的常用选项)
- [Install段的常用选项](https://www.cnblogs.com/duzhaoqi/p/7582404.html#install段的常用选项)

- [管理服务](https://www.cnblogs.com/duzhaoqi/p/7582404.html#管理服务)- [管理系统服务](https://www.cnblogs.com/duzhaoqi/p/7582404.html#管理系统服务)
- [服务查看](https://www.cnblogs.com/duzhaoqi/p/7582404.html#服务查看)
- [chkconfig命令的对应关系](https://www.cnblogs.com/duzhaoqi/p/7582404.html#chkconfig命令的对应关系)
- [其他命令](https://www.cnblogs.com/duzhaoqi/p/7582404.html#其他命令)
- [服务状态](https://www.cnblogs.com/duzhaoqi/p/7582404.html#服务状态)
- [systemctl示例](https://www.cnblogs.com/duzhaoqi/p/7582404.html#systemctl示例)

- [运行级别](https://www.cnblogs.com/duzhaoqi/p/7582404.html#运行级别)- [运行级别与target的对照](https://www.cnblogs.com/duzhaoqi/p/7582404.html#运行级别与target的对照)
- [运行级别的切换](https://www.cnblogs.com/duzhaoqi/p/7582404.html#运行级别的切换)

- [CentOS7引导顺序](https://www.cnblogs.com/duzhaoqi/p/7582404.html#centos7引导顺序)- [设置简单的内核参数](https://www.cnblogs.com/duzhaoqi/p/7582404.html#设置简单的内核参数)
- [简单的启动排错](https://www.cnblogs.com/duzhaoqi/p/7582404.html#简单的启动排错)

- [虚拟机环境下破解centOS7root的口令](https://www.cnblogs.com/duzhaoqi/p/7582404.html#虚拟机环境下破解centos7root的口令)- [方法一](https://www.cnblogs.com/duzhaoqi/p/7582404.html#方法一)
- [方法二](https://www.cnblogs.com/duzhaoqi/p/7582404.html#方法二)

- [centOS7 grub2](https://www.cnblogs.com/duzhaoqi/p/7582404.html#centos7-grub2)- [实验：在centOS7下破坏前446字节并修复](https://www.cnblogs.com/duzhaoqi/p/7582404.html#实验在centos7下破坏前446字节并修复)
- [实验：在centOS7下删除grub2下文件，并修复](https://www.cnblogs.com/duzhaoqi/p/7582404.html#实验在centos7下删除grub2下文件并修复)







## systemd简介

```
POST --> Boot Sequence --> Bootloader --> kernel + initramfs(initrd) -->rootfs --> /sbin/init
```

**init:**

```
CentOS 5: SysV init
CentOS 6: Upstart
CentOS 7: Systemd
```

**`Systemd`**：系统启动和服务器守护进程管理器，负责在系统启动或运行时，激活系统资源，服务器进程和其它进程 

**`Systemd新特性`**：
- **系统引导时实现服务并行启动**
- **按需启动守护进程**
- **自动化的服务依赖关系管理**
- **同时采用socket式与D-Bus总线式激活服务**
- **系统状态快照**

## unit对象

**核心概念**：`unit`
`unit`表示不同类型的systemd对象，通过配置文件进行标识和配置；文件中主要包含了系统服务、监听socket、保存的系统快照以及其它与init相关的信息 
**配置文件**：
- `/usr/lib/systemd/system`:每个服务最主要的启动脚本设置，类似于之前的/etc/init.d/
- `/run/systemd/system`：系统执行过程中所产生的服务脚本，比上面目录优先运行
- `/etc/systemd/system`：管理员建立的执行脚本，类似于/etc/rc.d/rcN.d/Sxx类的功能，比上面目录优先运行



### unit类型

**`Systemctl –t help` 查看unit类型**
- **`Service unit`**: 文件扩展名为.service, 用于定义系统服务
- **`Target unit`**: 文件扩展名为.target，用于模拟实现运行级别
- **`Device unit`**: .device, 用于定义内核识别的设备
- **`Mount unit`**: .mount, 定义文件系统挂载点
- **`Socket unit`**: .socket, 用于标识进程间通信用的socket文件，也可在系统启动时，延迟启动服务，实现按需启动
- **`Snapshot unit`**: .snapshot, 管理系统快照
- **`Swap unit`**: .swap, 用于标识swap设备
- **`Automount unit`**: .automount，文件系统的自动挂载点
- **`Path unit`**: .path，用于定义文件系统中的一个文件或目录使用,常用于当文件系统变化时，延迟激活服务，如：spool 目录

### 特性

**关键特性**：
- 基于socket的激活机制：socket与服务程序分离
- 基于d-bus的激活机制：
- 基于device的激活机制：
- 基于path的激活机制：
- 系统快照：保存各unit的当前状态信息于持久存储设备中向后兼容sysv init脚本

**不兼容**：
- systemctl命令固定不变，不可扩展
- 非由systemd启动的服务，systemctl无法与之通信和控制



## service unit文件格式

在/etc/systemd/system下的unit文件是`系统管理员和用户使用`
在/usr/lib/systemd/system下的供`发行版打包者使用`

在unit文件中，以“#” 开头的行后面的内容会被认为是注释，相关布尔值，1、yes、on、true 都是开启，0、no、off、false 都是关闭，时间单位默认是秒，所以要用毫秒（ms）分钟（m）等须显式说明

### service unit file文件通常由三部分组成
- [Unit]：定义与Unit类型无关的通用选项；用于提供unit的描述信息、unit行为及依赖关系等
- [Service]：与特定类型相关的专用选项；此处为Service类型
- [Install]：定义由“systemctlenable”以及"systemctldisable“命令在实现服务启用或禁用时用到的一些选项

### unit段的常用选项
- Description：描述信息
- After：定义unit的启动次序，表示当前unit应该晚于哪些unit启动，其功能与Before相反
- Requires：依赖到的其它units，强依赖，被依赖的units无法激活时，当前unit也无法激活
- Wants：依赖到的其它units，弱依赖
- Conflicts：定义units间的冲突关系

### Service段的常用选项
- Type：定义影响ExecStart及相关参数的功能的unit进程启动类型 
- simple：默认值，这个daemon主要由ExecStart接的指令串来启动，启动后常驻于内存中
- forking：由ExecStart启动的程序透过spawns延伸出其他子程序来作为此daemon的主要服务。原生父程序在启动结束后就会终止
- oneshot：与simple类似，不过这个程序在工作完毕后就结束了，不会常驻在内存中
- dbus：与simple类似，但这个daemon必须要在取得一个D-Bus的名称后，才会继续运作.因此通常也要同时设定BusNname= 才行
- notify：在启动完成后会发送一个通知消息。还需要配合NotifyAccess 来让Systemd 接收消息
- idle：与simple类似，要执行这个daemon必须要所有的工作都顺利执行完毕后才会执行。这类的daemon通常是开机到最后才执行即可的服务

- EnvironmentFile：环境配置文件
- ExecStart：指明启动unit要运行命令或脚本的绝对路径
- ExecStartPre：ExecStart前运行
- ExecStartPost：ExecStart后运行
- ExecStop：指明停止unit要运行的命令或脚本
- Restart：当设定Restart=1 时，则当次daemon服务意外终止后，会再次自动启动此服务

### Install段的常用选项
- Alias：别名，可使用systemctlcommand Alias.service
- RequiredBy：被哪些units所依赖，强依赖
- WantedBy：被哪些units所依赖，弱依赖
- Also：安装本服务的时候还要安装别的相关服务

注意： 
对于新创建的unit文件，或者修改了的unit文件，要通知systemd重载此配置文件,而后可以选择重启 
`systemctldaemon-reload`

## 管理服务

### 管理系统服务

CentOS 7: service unit 
**注意**：能兼容早期的服务脚本 
**命令**： 
systemctl COMMAND name.service
|-|centOS6|CentOS7|
|----|----|----|
|启动|service name start|systemctl start name.service|
|停止|service name stop|systemctl stop name.service|
|重启|service name restart|systemctl restart name.service|
|状态|service name status|systemctl status name.service|
|条件式重启(已启动才重启，否则不做操作)|service name condrestart|systemctl try-restart name.service|
|重载或重启服务(先加载，再启动)|-|systemctl reload-or-restart name.service|
|重载或条件式重启服务|-|systemctl reload-or-try-restart name.service|
|禁止自动和手动启动|-|systemctl mask name.service|
|取消禁止|-|systemctl unmask name.service|



### 服务查看
|-|centOS6|CentOS7|
|----|----|----|
|查看某服务当前激活与否的状态|-|systemctl is-active name.service|
|查看所有已经激活的服务|-|systemctl list-units --type|
|查看所有服务|-|systemctl list-units --type service --all|



### chkconfig命令的对应关系
|-|centOS6|CentOS7|
|----|----|----|
|设定某服务开机自启|chkconfig name on|systemctl enable name.service|
|设定某服务开机禁止启动|chkconfig name off|systemctl disable name.service|
|查看所有服务的开机自启状态|chkconfig --list|systemctl list-unit-files --type service|
|用来列出该服务在哪些运行级别下启用和禁用|chkconfig sshd –list|ls /etc/systemd/system/*.wants/sshd.service|
|查看服务是否开机自启|-|systemctl is-enabled name.service|



### 其他命令
|-|centOS6|CentOS7|
|----|----|----|
|查看服务的依赖关系|-|systemctl list-dependencies name.service|
|杀掉进程|-|systemctl kill unitname|
|切换至紧急救援模式|-|systemctl rescue|
|切换至emergency模式|-|systemctl emergency|
|关机|-|systemctlhalt、systemctlpoweroff|
|重启|-|systemctl reboot|
|挂起|-|systemctl suspend|
|休眠|-|systemctl hibernate|
|休眠并挂起|-|systemctlhybrid-sleep|



### 服务状态


```
systemctl list-unit-files --type service --all #显示状态
```

- loaded:Unit配置文件已处理
- active(running):一次或多次持续处理的运行
- active(exited):成功完成一次性的配置
- active(waiting):运行中，等待一个事件
- inactive:不运行
- enabled:开机启动
- disabled:开机不启动
- static:开机不启动，但可被另一个启用的服务激活



### systemctl示例

显示所有单元状态

```
systemctl 或systemctl list-units
```

只显示服务单元的状态


```
systemctl --type=service
```


显示sshd服务单元

```
systemctl –l status sshd.service
```

验证sshd服务当前是否活动


```
systemctl is-active sshd
```


启动，停止和重启sshd服务


```
systemctl start sshd.service
systemctl stop sshd.service
systemctl restart sshd.service
```


重新加载配置

```
systemctl reload sshd.service
```

列出活动状态的所有服务单元


```
systemctl list-units --type=service
```


列出所有服务单元


```
systemctl list-units --type=service --all
```


查看服务单元的启用和禁用状态


```
systemctl list-unit-files --type=service
```


列出失败的服务


```
systemctl --failed --type=service
```


列出依赖的单元


```
systemctl list-dependencies sshd
```


验证sshd服务是否开机启动


```
systemctl is-enabled sshd
```


禁用network，使之不能自动启动,但手动可以


```
systemctl disable network
```


启用network


```
systemctl enable network
```


禁用network，使之不能手动或自动启动


```
systemctl mask network
```


启用network

```
systemctl unmask network
```



## 运行级别

在centOS7上运行级别的含义已经和之前不同了，运行级别就是通过开启关闭不同的服务产生的效果，在从netOS7上，已然由.target来代替运行级别，我们可以称target为`目标态`，我们可以通过target定制更符合我们工作运行环境。 
我们可以通过命令：`ls /usr/lib/systemd/system/*.target`查看我们的机器上有多少个target


```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

"grub.cfg"140L,4397C written
[root@CT731 grub2]#ls /usr/lib/systemd/system/*.target
/usr/lib/systemd/system/anaconda.target
/usr/lib/systemd/system/basic.target
/usr/lib/systemd/system/bluetooth.target
/usr/lib/systemd/system/printer.target
...
/usr/lib/systemd/system/reboot.target
/usr/lib/systemd/system/rescue.target
/usr/lib/systemd/system/rpcbind.target
/usr/lib/systemd/system/runlevel0.target
/usr/lib/systemd/system/runlevel1.target
/usr/lib/systemd/system/runlevel2.target
...
/usr/lib/systemd/system/sleep.target
...

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```




使用`systemctl list-unit-files --type target --all`可以查看所有目标态的状态，或者`systemctl list-dependencies xxx.target`命令查看目标态的依赖性。 
![Snipaste_2017-09-23_15-18-51.png-25.3kB](http://static.zybuluo.com/ssh/8uj222jrzcajsqky3zsrg50f/Snipaste_2017-09-23_15-18-51.png)

在centOS7上所谓的目标态，其实就是由各种指定的服务和基础target组合而成的。



### 运行级别与target的对照

0 ==> runlevel0.target, poweroff.target 
1 ==> runlevel1.target, rescue.target 
2 ==> runlevel2.target, multi-user.target 
3 ==> runlevel3.target, multi-user.target 
4 ==> runlevel4.target, multi-user.target 
5 ==> runlevel5.target, graphical.target 
6 ==> runlevel6.target, reboot.target



### 运行级别的切换

在centOS6上，我们切换级别使用`init`，在centOS7上虽然也能使用，但是已经不再是原来的程序了，现在我们使用`systemctlisolate name.target`来切换target。

比如，我们想切换到字符界面，我们就可以使用**systemctlisolate multi-user.target**来进行切换。要想切换运行级别，在`/lib/systemd/system/*.target`文件中**`AllowIsolate=yes`**才可以。（修改文件需执行**systemctldaemon-reload**才能生效） 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195310634-1494599146.png)

在centOS7上如何查看运行的目标态呢，使用命令`systemctl get-default`
使用命令`systemctl set-default name.target`来修改我们的目标态。

我们看一下我们的默认目标态究竟为何物。


```
[root@CT731 ~]#locate default.target
/etc/systemd/system/default.target
```

```
[root@CT731 ~]#ll /etc/systemd/system/default.target
lrwxrwxrwx.1 root root 36Aug2507:06/etc/systemd/system/default.target ->/lib/systemd/system/runlevel3.target
```


它其实就是创建了一个软链接到指定的target上去了 

## CentOS7引导顺序
- UEFi或BIOS初始化，运行POST开机自检
- 选择启动设备
- 引导装载程序, centos7是grub2
- 加载装载程序的配置文件：/etc/grub.d/ /etc/default/grub /boot/grub2/grub.cfg
- 加载initramfs驱动模块
- 加载内核选项
- 内核初始化，centos7使用systemd代替init
- 执行initrd.target所有单元，包括挂载/etc/fstab
- 从initramfs根文件系统切换到磁盘根目录
- systemd执行默认target配置，配置文件/etc/systemd/system/default.target
- systemd执行sysinit.target初始化系统及basic.target准备操作系统
- systemd启动multi-user.target下的本机与服务器服务
- systemd执行multi-user.target下的/etc/rc.d/rc.local
- Systemd执行multi-user.target下的getty.target及登录服务
- systemd执行graphical需要的服务



### 设置简单的内核参数

#### **设置内核参数，只影响当次启动**
- 启动时，在linux16行后添加systemd.unit=desired.target 
- systemd.unit=emergency.target
- systemd.unit=rescue.target

- rescue.target 比emergency 支持更多的功能，例如日志等
- systemctl default 进入默认target



### 简单的启动排错

在centOS7中，文件系统损坏，先尝试自动修复，失败则进入`emergency shell`，提示用户修复
- 在/etc/fstab不存在对应的设备和UUID等一段时间，如不可用，进入emergency shell
- 在/etc/fstab不存在对应挂载点,systemd尝试创建挂载点，否则提示进入emergency shell.
- 在/etc/fstab不正确的挂载选项,提示进入emergency shell



## 虚拟机环境下破解centOS7root的口令



### 方法一

启动时任意键暂停启动 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195339650-957136834.png)


按e键进入编辑模式 
将光标移动linux16开始的行，添加内核参数rd.break 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195405259-1811519780.png)
按ctrl-x启动，然后： 
mount –o remount,rw /sysroot 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195422150-1649416433.png)

![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195429103-971557977.png)
chroot /sysroot 
passwd root 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195437712-703994683.png)
touch /.autorelabel 
exit 
reboot 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195445181-1401279509.png)
然后可以使用设置的密码登录root



### 方法二

启动时任意键暂停启动（同方法1） 
按e键进入编辑模式（同方法1） 
将光标移动linux16开始的行，改为rw init=/sysroot/bin/sh 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195504915-584787923.png)
按ctrl-x启动 
chroot /sysroot 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195512056-1252108361.png)
passwd root 
touch /.autorelabel 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923195549696-1114218757.png)
exit 
reboot 
最后，用新密码登录root



## centOS7 grub2

在centOS6上，我们的grub文件是/boot/grub/grub.conf 
在centOS7上，文件改成/boot/grub2/grub.cfg了，但是功能还是大致一样的都是用于加载内核的，不过在centOS7上设置默认启动项发生了一些变化，假如我们现在有两个内核，我们需要改变默认启动应该如何做到呢？

首先，`vim /etc/default/grub`打开如下文件：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

GRUB_TIMEOUT=5
GRUB_DISTRIBUTOR="$(sed 's, release .*$,,g' /etc/system-release)"
GRUB_DEFAULT=saved
GRUB_DISABLE_SUBMENU=true
GRUB_TERMINAL_OUTPUT="console"
GRUB_CMDLINE_LINUX="crashkernel=auto rhgb quiet net.ifnames=0"
GRUB_DISABLE_RECOVERY="true"
~

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

打开文件后，我们修改`GRUB_DEFAULT`的值，和centOS一样，0代表第一个内核，1代表第二个，以此类推。 
我们在修改完成后，并没有立即生效，使用`grub2-mkconfig -o /boot/grub2/grub.cfg`命令来生成grub2.cfg文件，我们在下次启动的时候就会默认选择新的默认内核。



### 实验：在centOS7下破坏前446字节并修复

第一步：破坏硬盘的前446字节： 
dd if=/dev/zero of=/dev/sda bs=1 count=446


```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

[root@CT731 ~]#dd if=/dev/zero of=/dev/sda bs=1 count=446
446+0 records in
446+0 records out
446 bytes (446 B) copied,0.000758682 s,588 kB/s
[root@CT731 ~]#hexdump -C -n 512/dev/sda
0000000000000000000000000000000000000000|................|
*
000001b000000000000000000000000000008020|...............|
000001c021008335700500080000000040000035|!..5p.......@..5|
000001d0710583 fe ff ff 000840000000400600 fe |q.......@...@...|
000001e0 ff ff 83 fe ff ff 000880060000400600 fe |............@...|
000001f0 ff ff 05 fe ff ff 0008 c0 0c00 f8 3f0c55 aa |............?.U.|
00000200

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```


第二步：重启计算机 
由于我的虚拟机挂载了光盘，所以一重启就进入光盘启动的界面 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923200908353-874220220.png)
上面有三项，我们选择第三项进入troubleshooting 
进入第三项后，点击第二项，进入救援模式的centos的系统 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923200919525-43679005.png)
然后我们进入如下模式： 
选择1，继续进行： 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923200929212-1536405563.png)
接下来，我们就会进入到一个shell模式中，不需要切根，进行系统修复： 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923200939775-74164270.png)

修复过程： 
grub2-install --root-directory=/mnt/sysimage/ /dev/sda 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923200949103-1604973199.png)

我们来查看一下： 
hexdump -C -n 512 /dev/sda 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923200958665-724007315.png)

重启，修复完成。



### 实验：在centOS7下删除grub2下文件，并修复

第一步：删除grub2 
**rm -rf grub2**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

[root@CT731 boot]#rm -rf grub2
[root@CT731 boot]#ls
config-3.10.0-514.el7.x86_64
grub
initramfs-0-rescue-618d3baeb5754f7092242f811c7aa740.img
initramfs-3.10.0-514.el7.x86_64.img
initramfs-3.10.0-514.el7.x86_64kdump.img
initrd-plymouth.img
symvers-3.10.0-514.el7.x86_64.gz
System.map-3.10.0-514.el7.x86_64
vmlinuz-0-rescue-618d3baeb5754f7092242f811c7aa740
vmlinuz-3.10.0-514.el7.x86_64

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

第二步，重启计算机 
进入如下界面： 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923201053681-596392032.png)

接下来，我们重启系统，按Esc，进入光盘救援模式： 
选择第三项，进入光盘救援（前提是挂载光盘） 
以下步骤到进入shell同实验一相同，不在过多演示; 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923201106728-790920389.png)

进入救援模式后： 
第一步：切根 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923201115087-905096315.png)
然后执行命令 
grub2-install 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923201131790-961056079.png)
下图中，我们可以看到在grub2文件夹中，还没有grub.cfg文件，接下来，我们需要生成： 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923201141431-1092072668.png)
生成配置文件： 
进入到grub2下， 
grub2-mkconfig -o grub.cfg 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923201147978-658380198.png)
exit然后， 
重启电脑： 
![](https://images2017.cnblogs.com/blog/1174645/201709/1174645-20170923201153946-1895668281.png)

完成！





from：http://www.cnblogs.com/duzhaoqi/









