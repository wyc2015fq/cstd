# 如何用 sysdig 监控和排查 Linux 服务器 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Panblack](http://www.jobbole.com/members/upanblack) 翻译，[野峰](http://www.jobbole.com/members/prowyh) 校稿。未经许可，禁止转载！
英文出处：[Gabriel Cánepa](http://xmodulo.com/monitor-troubleshoot-linux-server-sysdig.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
如果你需要跟踪一个进程发出和接收的系统调用，第一个想到是的什么？你很可能想到了 strace，你是对的。 从命令行监控原始网络通信你会用什么工具？如果你想到了 tcpdump，你又做了明智的决定。如果你碰巧需要跟踪打开的文件（换句 Unix 语言来讲就是：一切皆文件），很可能你会用 lsof。
strace、tcpdump 和 lsof 确实了不起的工具，应该是每个系统管理员工具箱里的一部分。这恰恰是你会喜欢 sysdig 的原因，一个强大的系统级[探索]和调试工具，开源的，并且被创始人称作“strace + tcpdump + lsof ，顶上浇了带有 Lua 樱桃的美妙酱汁”。好吧，先把幽默放一边，sysdig 的一个很棒的特性在于，它不仅能够分析Linux 系统的 “现场”状态，还能够把系统状态保存到一个转储文件以用于离线检测。而且，你可以用内置（或者自己编写）的小脚本 —— 名为“chisels（凿子）” —— 来定制 sysdig 的行为甚至增强 sysdig 的能力。各种 chisels 都以各自不同的脚本特性来分析 sysdig 捕获的事件流。
在本教程里，我们将研究 sysdig 在 Linux 上的安装和基本的系统监控和调试用法。
## 安装Sysdig
为了简单、明了和Linux发行版无关方面的考虑，本教程选择官网描述的自动安装流程，安装脚本自动探查操作系统并安装必要的依赖包。
以 root 身份运行如下指令，从官方 apt/yum 源安装 sysdig。

Shell
```
# curl -s https://s3.amazonaws.com/download.draios.com/stable/install-sysdig | bash
```
![curl_15334678580_99c5d38328_z](http://jbcdn2.b0.upaiyun.com/2015/09/fc6f1ba956a0e78f25c808691786ea64.jpg)
安装完成后，我们可以像这样运行 sysdig，来感受一下。

Shell
```
# sysdig
```
屏幕上立刻充斥这系统内发生的一切信息，我们没办法利用这些信息，所以我们应该运行：

Shell
```
# sysdig -cl | less
```
来查看已有的 chisels 列表：
![to_see_a_list_of_available_chisels_15334678610_e5956a26e8_z](http://jbcdn2.b0.upaiyun.com/2015/09/8a1667691556852253ad89f2d90a682d.jpg)
下列类别是默认可用的，每一类都有多个内置的 chisels。

Shell
```
CPU Usage     CPU 使用情况
    Errors        错误
    I/O           输入/输出
    Logs          日志
    Misc          杂项
    Net           网络
    Performance   性能
    Security      安全
    System State  系统状态
```
显示一个特定chisel的信息（含详细的命令行用法），运行：

Shell
```
# sysdig -cl [chisel_name]
```
比如，查看“Net”类别下的 spy_port chisel，可以运行：

Shell
```
# sysdig -i spy_port
```
![sysdig_i_spy_port_15521424095_0365bf20c3_z](http://jbcdn2.b0.upaiyun.com/2015/09/e513f487350947492519c80285fe9411.jpg)
chisel 可以跟过滤器结合以便获得更有用的输出内容，过滤器可以用于现场数据也可以用于跟踪文件。
过滤器遵循“类别.字段”的结构，比如：
- fd.cip: 客户端 IP 地址
- evt.dir: 事件方向，可以是 ‘>’，代表进入事件；或 ‘<’，代表退出事件。
下面的命令显示所有的过滤器：

Shell
```
# sysdig -l
```
在剩下的教程里，我将演示一些 sysdig 的用例。
## Sysdig 示例：服务器性能调试
假设你的服务器遇到性能问题（比如无反应或严重的反应延迟），你可以用 bottlenecks chisel 来显示此时最慢的 10 个系统调用。
下面的命令可以在运行中的服务器上实时做检测。“-c” 标记后面跟着 chisel名，是告诉 sysidg 运行指定的 chisel。

Shell
```
# sysdig -c bottlenecks
```
或者，你可以执行离线服务器性能分析。这种情况下，你可以把完整的 sysdig 跟踪数据保存到一个文件里，然后对跟踪文件运行 bottlenecks chisel ，方法如下：
首先，保存 sysdig 跟踪文件（用 Ctrl+c 终止数据收集）：

Shell
```
# sysdig -w trace.scap
```
收集完跟踪数据，用如下命令就可以检查跟踪期间最慢的系统调用：

Shell
```
# sysdig -r trace.scap -c bottlenecks
```
![sysdig_r_trace.scap_c_bottlenecks_15334678670_ebbe93265e_z](http://jbcdn2.b0.upaiyun.com/2015/09/ce509f15708fe652ba8904f83ed412bd.jpg)
需要注意 #2 #3 和 #4 列，它们分别代表执行时间、进程名和 PID。
## Sysdig 示例：监控用户行为
假设你作为系统管理员想要监控系统的用户行为（比如，用户在命令行敲了什么指令、进入了哪些目录），那么 spy_user chisel 就派上用场了。
我们首先用一些额外选项来收集 sysdig 跟踪文件：

Shell
```
# sysdig -s 4096 -z -w /mnt/sysdig/$(hostname).scap.gz
    "-s 4096" 告诉sysdig 对每个事件捕捉4096个字节。
    "-z" （跟 -w 一起使用）压缩跟踪文件。
    "-w <trace-file>" 保存 sysdig 跟踪数据到指定文件。
```
在上面的例子里，我们给压缩的跟踪文件按照主机名来命名。记住，你可以在任何时候敲Ctrl + c终止 sysdig 的运行。
我们采集了足够多的数据后，可以用如下指令查看系统内每个用户的交互行为：

Shell
```
# sysdig -r /mnt/sysdig/debian.scap.gz -c spy_users
```
![sysdig_r_mnt_sysdig_debian.scap.gz_c_spy_users_15518254291_5c9671ca41_z](http://jbcdn2.b0.upaiyun.com/2015/09/3de05aa3ae56cdc47fce4adac9dc0a19.jpg)
上面的输出里，第一列代表与用户活动相关的进程PID。
如果你想把目标指向一个特定用户，并只监控该用户的活动该怎么办呢？你可以通过用户名来过滤 spy_users chisel 的结果：

Shell
```
# sysdig -r /mnt/sysdig/debian.scap.gz -c spy_users "user.name=xmodulo"
```
![sysdig_r_mnt_sysdig_debian.scap.gz_c_spy_users_user.name=xmodulo_15498248556_66d15422b1_z](http://jbcdn2.b0.upaiyun.com/2015/09/68bc85f1cb2571a93d45c4549e30c0f7.jpg)
## Sysdig 示例：监控文件I/O
我们可以用 “-p” 标记来定制 sysdig 跟踪的输出格式，在双引号内包含所需的字段（比如： 用户名、进程名以及文件或套接字名）。在下面的例子里，我们创建一个跟踪文件，只包括对 home 目录的写入事件（过后我们可以用”sysdig -r writetrace.scap.gz” 来查看）。

Shell
```
# sysdig -p "%user.name %proc.name %fd.name" "evt.type=write and fd.name contains /home/" -z -w writetrace.scap.gz
```
![sysdig_p_15498248586_de5f5fc93d_z](http://jbcdn2.b0.upaiyun.com/2015/09/fda9bcbf371ba3416b05acae5dd6497a.jpg)
## Sysdig 示例：监控网络I/O
作为服务器调试的一部分，你可能需要窥探网络流量，而这一般都是用 tcpdump。用 sysdig 呢，流量嗅探同样很容易，而且方式更加用户友好。
例如，你可以查看服务器的某个特定进程（比如 apache2）与一个特定 IP 地址的数据交换（ASCII形式）:

Shell
```
# sysdig -s 4096 -A -c echo_fds fd.cip=192.168.0.100 -r /mnt/sysdig/debian.scap.gz proc.name=apache2
```
如果你想监控原始数据传输（二进制形式），把 “-A” 改为 “-X”。

Shell
```
# sysdig -s 4096 -X -c echo_fds fd.cip=192.168.0.100 -r /mnt/sysdig/debian.scap.gz proc.name=apache2
```
想要更多的信息、示例和案例研究可以登录项目官网。相信我，sysdig 拥有无限的可能性。别光听我说，现在就安装 sysdig，开始挖掘吧！
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/03/bb6b0431912b5b441aac6477e67bd0d22.png)![](http://www.jobbole.com/wp-content/uploads/2016/03/381b4bd995d3cff3c32be73b9d0571b82.jpg)
