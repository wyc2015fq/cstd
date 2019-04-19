# 系统管理员应该知道的 20 条 Linux 命令 - 文章 - 伯乐在线
原文出处： [OpenSource](https://opensource.com)   译文出处：[开源中国/局长](https://my.oschina.net/editorial-story/blog/1499026)
![203755_92u8_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/7599b213c6d67d39e4bb2391c276fbb5.png)
在这个全新的工具和多样化的开发环境井喷的大环境下，任何开发者和工程师都有必要学习一些基本的系统管理命令。特定的命令和工具包可帮助开发者组织、排查故障并优化他们的应用程序，而且当出现错误时，也可以为运维人员和系统管理员提供有价值的分类信息。
无论你是新手开发者还是希望管理自己的应用程序，下面 20 条基本的系统管理命令都可以帮助您更好地了解您的应用程序。它们还可以帮助解决为什么应用程序可在本地正常工作但不能在远程主机上工作这类的系统故障。这些命令适用于 Linux 开发环境、容器和虚拟机。
# 1. curl
curl 用于传输一个 URL。可以使用这条命令用于测试应用程序的端点或与上游服务端点的连接。curl 还可用于检查你的应用程序是否能连接到其他服务，例如数据库，或检查您的服务是否处于健康的状态。
举个例子，假如你的应用程序抛出一个 HTTP 500 错误，表示无法访问 MongoDB 数据库：


```
$ curl -I -s myapplication:5000
HTTP/1.0 500 INTERNAL SERVER ERROR
```
**-I **选项用于显示头信息，**-s **选项表示使用静默模式，不显示错误和进度。检查数据库的端点是否正确：


```
$ curl -I -s database:27017
HTTP/1.0 200 OK
```
那么可能是什么问题呢？ 检查您的应用程序是否可以访问数据库以外的其他位置：


```
$ curl -I -s https://opensource.com
HTTP/1.1 200 OK
```
看起来这没问题，现在尝试访问数据库。您的应用程序正在使用数据库的主机名，因此请先尝试：


```
$ curl database:27017
curl: (6) Couldn't resolve host 'database'
```
这表示您的应用程序无法解析数据库，因为数据库的 URL 不可用或主机（容器或VM）没有可用于解析主机名的域名服务器。
# 2. python -m json.tool / jq
使用 curl 后，API 调用的输出可读性可能较差。有时候，你希望将生成的 JSON 数据格式化输出以查找特定的条目。Python 有一个内置的库可帮助您实现这个需求。可以使用 **python -m json.tool **来缩进和组织 JSON。要使用 Python 的 JSON 模块，需要使用管道机制，将 JSON 文件的输出作为输入，写入到 **python -m json.tool** 命令行。


```
$ cat test.json
{"title":"Person","type":"object","properties":{"firstName":{"type":"string"},"lastName":{"type":"string"},"age":{"description":"Age in years","type":"integer","minimum":0}},"required":["firstName","lastName"]}
```
要使用 Python 库，使用 **-m** (module) 选项将输出内容和 Python 库组合成管道。


```
$ cat test.json | python -m json.tool
{
    "properties": {
        "age": {
            "description": "Age in years",
            "minimum": 0,
            "type": "integer"
        },
        "firstName": {
            "type": "string"
        },
        "lastName": {
            "type": "string"
        }
    },
    "required": [
        "firstName",
        "lastName"
    ],
    "title": "Person",
    "type": "object"
}
```
对于更高级的 JSON 解析，可以安装 **[jq](https://stedolan.github.io/jq/download/)。j****q **提供了一些从 JSON 输入中提取特定值的选项。要像上面的 Python 模块一样将 JSON 输出格式化，只需将 jq 应用到输出即可。


```
$ cat test.json | jq
{
  "title": "Person",
  "type": "object",
  "properties": {
    "firstName": {
      "type": "string"
    },
    "lastName": {
      "type": "string"
    },
    "age": {
      "description": "Age in years",
      "type": "integer",
      "minimum": 0
    }
  },
  "required": [
    "firstName",
    "lastName"
  ]
}
```
# 3. ls
**ls **用于列出目录中的文件，系统管理员和开发者会经常使用这个命令。在容器空间中，这条命令可以帮助确定容器镜像中的目录和文件。除了查找文件，**ls **还可以用于检查权限。下面的示例中，由于权限问题，你不能运行 myapp。当你使用 **ls -l **检查权限时，你会发现它的权限在 **-rw-r–r–** 中没有”x”，只有读写的权限。


```
$ ./myapp
bash: ./myapp: Permission denied
$ ls -l myapp
-rw-r--r--. 1 root root 33 Jul 21 18:36 myapp
```
# 4. tail
**tail **显示文件的最后一部分内容。通常情况下，你不需要浏览每行日志以进行故障排除。而是需要检查日志中对应用程序的最新请求的说明。例如，当你向 Apache HTTP 服务器发起请求时，可以使用 **tail** 来检查日志中发生的情况。
![205506_GAYc_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/d3bb1474cbffe499ef82ea809c54c078.png)
*使用 **tail -f **来跟踪日志文件并在发起请求时查看它们。*
**-f** 选项表示跟随的意思，它可在日志被写入文件时输出它们。下面的示例具有每隔几秒访问端点的后台脚本，日志会记录请求。除了实时跟踪日志，还可以使用 **tail **带上 **-n **选项来查看文件的最后 100 行。


```
$ tail -n 100 /var/log/httpd/access_log
```
# 5. cat
**cat **主要用于查看文件内容和合并文件。你可能会使用 **cat **来检查依赖项文件的内容，或确认已在本地构建的应用程序的版本。


```
$ cat requirements.txt
flask
flask_pymongo
```
上面的示例检查您的 Python Flask 应用程序是否已将 Flask 列为依赖项。
# 6. grep
grep 能使用特定模式匹配（包括正则表达式）搜索文本。如果你在另一条命令的输出中寻找特定的模式，**grep **会高亮显示相关的行。可使用这条命令来搜索日志文件以及特定的进程等。如果想查看 Apache Tomcat 是否启动，你可能会命令行的数量给淹没。但讲输出的内容和 **grep **命令组合成管道，可以将表示服务器已启动的行独立出来。


```
$ cat tomcat.log | grep org.apache.catalina.startup.Catalina.start
01-Jul-2017 18:03:47.542 INFO [main] org.apache.catalina.startup.Catalina.start Server startup in 681 ms
```
# 7. ps
**ps **用于查看进程的各种状态信息。使用该命令可确定正在运行的应用程序或确认预期的进程。例如，如果要检查正在运行的 Tomcat Web 服务器，可使用带有选项的 **ps **来获取 Tomcat 的进程 ID。


```
$ ps -ef
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  2 18:55 ?        00:00:02 /docker-java-home/jre/bi
root        59     0  0 18:55 pts/0    00:00:00 /bin/sh
root        75    59  0 18:57 pts/0    00:00:00 ps -ef
```
为了更好的易读性，可使用 **grep **和 **ps **组合成管道。


```
$ ps -ef | grep tomcat
root         1     0  1 18:55 ?        00:00:02 /docker-java-home/jre/bi
```
# 8. env
**env **用于列出所有环境变量及为其赋值。在故障排除期间，你可能会发现需要检查是否有错误的环境变量来阻止应用程序启动。在下面的示例中，该命令用于检查程序主机上设置的环境变量。


```
$ env
PYTHON_PIP_VERSION=9.0.1
HOME=/root
DB_NAME=test
PATH=/usr/local/bin:/usr/local/sbin
LANG=C.UTF-8
PYTHON_VERSION=3.4.6
PWD=/
DB_URI=mongodb://database:27017/test
```
请注意，该应用程序正在使用 Python 3，并具有连接到 MongoDB 数据库的环境变量。
# 9. top
**top **用于显示系统中各个进程的信息和资源占用状况，类似于 Windows 的任务管理器。使用该命令可确定哪些进程正在运行，以及它们消耗了多少的内存和 CPU。一种常见的情况是当你运行一个应用程序时，它在一分钟后挂掉。这时，你首先检查应用程序的返回错误，发现是一个内存错误。


```
$ tail myapp.log
Traceback (most recent call last):
MemoryError
```
你的应用是否真的内存不足？要确认这个问题，可使用 **top **来查看应用程序消耗多少 CPU 和内存。当使用 **top **命令后，您注意到一个 Python 应用程序使用了大部分的 CPU，其内存使用量也迅速攀升。当它运行时，如果进程是你的应用程序，则按”C”键来查看完整命令并进行逆向工程。发现原来是你的内存密集型应用程序(**memeater.py**)。当你的应用程序已经用尽内存，系统会杀掉它并返回一个内存不足(OOM)的错误。
![205506_GAYc_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/eeb3863a3e031513712e9556d1c6eeed.png)
*应用程序的内存和 CPU 使用量增加，最终因为内存不足而被杀掉。*
![205506_GAYc_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/8cef41f3180be447f1405efebfc8420c.png)
*通过按下”C”键，可以看到启动该应用程序的完整命令*
除了检查应用程序，还可以使用 **top **来调试其他使用 CPU 或内存的进程。
# 10. netstat
**netstat **用于显示网络状态信息。该命令可显示正在使用的网络端口及其传入连接。但是，**netstat **在 Linux 中不能开箱即用。如果需要安装它，需要在 **[net-tools](https://wiki.linuxfoundation.org/networking/net-tools)**包中找到它。作为在本地进行试验或将应用程序推送到主机的开发者，可能会收到端口已被分配或地址已被使用的错误。使用 **netstat** 得到协议、进程和端口这些信息，下图表明 Apache HTTP 服务器已经在下面的主机上使用了 80 端口。
![205506_GAYc_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/da463bd1dfe1c75df76fff92f6fa4842.png)
*使用 **netstat -tulpn **表明 Apache 已经在这台机器上使用了 80 端口。*
# 11. ip address
如果 **ip address **在你的主机上不能使用，必须使用 **[iproute2](https://wiki.linuxfoundation.org/networking/iproute2)**包进行安装。**i****p address **用于显示应用程序的主机接口和 IP 地址。可使用 **i****p address **来验证你的容器或主机的 IP 地址。例如，当你的容器连接到两个网络时，**i****p address **可显示哪个接口连接到了哪个网络。对于简单的检查，可以随时使用 **ip address **命令获取主机的 IP 地址。下面的示例展示了在接口 eth0 上的 Web 层容器的 IP 地址为 172.17.0.2
![205506_GAYc_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/f47acafef2423ba78048bffb330f5a06.png)
*使用 **ip address **显示 eth0 接口的 IP 地址为 172.17.0.2*
# 12. lsof
**lsof **用于列出当前系统打开的文件(list open files)。在某些 Linux 系统中，可能需要使用 [lsof](http://www.linuxfromscratch.org/blfs/view/svn/general/lsof.html) 包来安装**lsof**。在 Linux 中，几乎任何和系统的交互都被视为一个文件。因此，如果你的应用程序写入文件或代开网络连接，**lsof **将会把这个交互映射为一个文件。与 **netstat **类似，可使用 **lsof **来检查侦听端口。例如，如果要检查 80 端口是否正在被使用，可使用 **lsof **来检查哪个进程正在使用它。下面的示例中，可以看到 httpd (Apache) 在 80 端口上侦听。还可以使用 **lsof **来检查 httpd 的进程ID，检查 Web 服务器的二进制文件所在位置(**/usr/sbin/httpd**)。
![205506_GAYc_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/c03a535f9ce77f7c52d661cbdd907bfe.png)
***Lsof **表明了 httpd 在 80 端口上侦听。检查 httpd 的进程ID还可以显示所有需要运行的文件httpd。*
打开文件列表中的打开文件的名称有助于确定进程的来源，特别是 Apache。
# 13. df
可以使用 **df** 显示空闲的磁盘空间(display free disk space)以排查磁盘空间问题。挡在容器管理器上运行应用程序时，可能会收到一条错误信息，提示容器主机上缺少可用空间。虽然磁盘空间应该由系统管理程序来管理和优化，你仍可以使用 **df **找出目录中的现有空间并确认是否没有空间。
![205506_GAYc_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/d4981e341da22b6f3dd72ffc98003b59.png)
***Df** 显示每个文件系统的磁盘空间、绝对空间以及其可用性。*
**-h **选项表示以可读性较高的方式来显示信息，上面的例子表示这个主机具有大量的磁盘空间。
# 14. du
**du** 命令也是用于查看使用空间的，但是与 df 命令不同的是 du 命令是对文件和目录磁盘使用的空间的查看，要获取有关哪些文件在目录中使用磁盘空间的更多详细信息，可以使用 du 命令，和 df 命令还是有一些区别的。例如，你想了解那个日志文件占用 **/var/log **目录最多的空间，可以使用 **du** 命令加上 **-h** 选项和用于获取总大小的 **-s **选项。


```
$ du -sh /var/log/*
1.8M  /var/log/anaconda
384K  /var/log/audit
4.0K  /var/log/boot.log
0 /var/log/chrony
4.0K  /var/log/cron
4.0K  /var/log/maillog
64K /var/log/messages
```
上面的示例中显示了 **/var/log **下的的最大目录为 **/var/log/audit**。可以将 **du** 和 **df** 搭配使用，以确定在应用程序的主机上使用的磁盘空间。
# 15. id
要检查运行应用程序的用户，可使用 **id **命令来返回用户身份。**id **命令可以显示真实有效的用户ID(UID)和组ID(GID)。下面的示例使用 [Vagrant](https://www.vagrantup.com/) 来测试应用程序并隔离其开发环境。登录进 Vagrant 盒子后，如果尝试安装 Apache HTTP Server（依赖关系），系统会提示你需要以 root 身份执行该命令。要检查你的用户ID和组ID，使用 **id **命令，会发现你正在”vagrant”组中以”vagrant”用户身份运行。


```
$ yum -y install httpd
Loaded plugins: fastestmirror
You need to be root to perform this command.
$ id
uid=1000(vagrant) gid=1000(vagrant) groups=1000(vagrant) context=unconfined_u:unconfined_r:unconfined_t:s0-s0:c0.c1023
```
要解决此问题，必须以超级用户的身份运行该命令，这将提供提升的权限。
# 16. chmod
**c****hmod** 命令用来变更文件或目录的权限。当你在主机上首次运行应用程序的二进制文件时，可能会收到错误提示信息“拒绝访问”。如 **ls **的示例所示，可以用于检查应用程序二进制文件的权限。


```
$ ls -l
total 4
-rw-rw-r--. 1 vagrant vagrant 34 Jul 11 02:17 test.sh
```
这表明您没有权限（没有“x”）来运行二进制文件。**c****hmod **可以修改权限，使的用户能够运行二进制文件。


```
$ chmod +x test.sh
[vagrant@localhost ~]$ ls -l
total 4
-rwxrwxr-x. 1 vagrant vagrant 34 Jul 11 02:17 test.sh
```
如例子所示，这将更新权限，使其具有可执行的权限。现在当你尝试执行二进制文件时，应用程序不会抛出拒绝访问的错误。当将二进制文件加载到容器时，**Chmod** 可能很有用。它能保证容器具有合适的权限以执行二进制文件。
# 17. dig / nslookup
**dig **命令是常用的域名查询工具，可以用来测试域名系统工作是否正常。域名服务器(DNS)有助于将 URL 解析为一组应用程序服务器。然而，你会发现有些 URL 不能被解析，这会导致应用程序的连接问题。例如，假如你尝试从应用程序的主机访问你的数据库。你收到一个”不能解析”的错误。要进行故障排查，你尝试使用 **dig**（DNS 查询工具）或 **nslookup**（查询 Internet 域名服务器）来确定应用程序似乎无法解析数据的原因。


```
$ nslookup mydatabase
Server:   10.0.2.3
Address:  10.0.2.3#53
** server can't find mydatabase: NXDOMAIN
```
使用 **nslookup **显示无法解析 **mydatabase**。尝试使用 **dig **解决，但仍是相同的结果。


```
$ dig mydatabase
; <<>> DiG 9.9.4-RedHat-9.9.4-50.el7_3.1 <<>> mydatabase
;; global options: +cmd
;; connection timed out; no servers could be reached
```
这些错误可能是由许多不同的问题引起的。如果无法调试出根本原因，与系统管理员联系以进行更多的调查。对于本地测试，这些问题可能表示你的主机的域名服务器未正确配置。要使用这些命令，需要安装 **[BIND Utilities](http://www.linuxfromscratch.org/blfs/view/svn/basicnet/bind-utils.html)**包**。**
# 18. iptables
iptables 用于阻止或允许 Linux 主机上的流量，用于 IP 包过滤器管理，类似于网络防火墙。此工具可阻止某些应用程序接收或发送请求。更具体地说，如果您的应用程序难以访问另一个端点，可能已被 iptables 拒绝流量访问该端点。例如，假设您的应用程序的主机无法访问 Opensource.com，您使用 **curl **来测试连接。


```
$ curl -vvv opensource.com
* About to connect() to opensource.com port 80 (#0)
*   Trying 54.204.39.132...
* Connection timed out
* Failed connect to opensource.com:80; Connection timed out
* Closing connection 0
curl: (7) Failed connect to opensource.com:80; Connection timed out
```
连接超时。您怀疑某些东西可能会阻塞流量，因此您使用 **-S **选项显示 **iptables **规则。


```
$ iptables -S
-P INPUT DROP
-P FORWARD DROP
-P OUTPUT DROP
-A INPUT -p tcp -m tcp --dport 22 -j ACCEPT
-A INPUT -i eth0 -p udp -m udp --sport 53 -j ACCEPT
-A OUTPUT -p tcp -m tcp --sport 22 -j ACCEPT
-A OUTPUT -o eth0 -p udp -m udp --dport 53 -j ACCEPT
```
前三个规则显示，默认情况下流量已被丢弃。剩下的规则表示允许 SSH 和 DNS 流量。在这种情况下，如果需要允许流量到外部端点的规则，请跟上 sysadmin。如果这是用于本地开发或测试的主机，可使用 **iptables **命令来允许合适的流量。添加允许到主机的流量的规则时一定要谨慎。
# 19. sestatus
通常会在企业管理的应用程序主机上使用 SELinux（一个 Linux 安全模块）。SELinux 对主机上运行的进程提供最低权限的访问，防止潜在的恶意进程访问系统上的重要文件。某些情况下，应用程序需要访问特定文件，但可能会发生错误。要检查 SELinux 是否阻止了应用程序，使用 **tail **和 **grep **在 **/var/log/audit **日志记录中查找”denied”（被拒绝）的信息。否则，使用 **sestatus **来检查是否启动了 SELinux。


```
$ sestatus
SELinux status:                 enabled
SELinuxfs mount:                /sys/fs/selinux
SELinux root directory:         /etc/selinux
Loaded policy name:             targeted
Current mode:                   enforcing
Mode from config file:          enforcing
Policy MLS status:              enabled
Policy deny_unknown status:     allowed
Max kernel policy version:      28
```
上面的输出表示应用程序的主机已启用 SELinux。在本地开发环境中，可以更新 **SELinux** 使得权限更宽松。
# 20. history
当你使用大量的命令进行测试和调试时，可能会忘记有用的命令。每个 shell 都有一个 **history **命令的变体。它可显示自会话开始以来使用的命令的历史记录。可以使用 **history **来记录用来排除应用程序故障的命令。history 命令用于显示指定数目的指令命令，读取历史命令文件中的目录到历史命令缓冲区和将历史命令缓冲区中的目录写入命令文件。


```
$ history
    1  clear
    2  df -h
    3  du
```
如果希望执行之前历史记录中的命令，但又不想重新输入，该怎么办？使用符号 **!** 即可，可以使用符号 **!** 执行指定序号的历史命令。例如，要执行第 2 个历史命令，则输入**!**2，
![205506_GAYc_2918182](http://jbcdn2.b0.upaiyun.com/2017/08/84a8a291f13fb68c3468a03906458adb.png)
在需要重新执行的命令的指定编号前添加 **! **即可重新执行
这些基本命令能增强排查故障的专业技能，可检查为什么应用程序可以在一个开发环境中工作，而在另一个开发环境中则不可以。许多系统管理员使用这些命令来调试系统问题。了解一些有用的故障排查命令可帮助解决应用程序的问题。
