# Linux iptables常用命令 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月12日 15:22:30[boonya](https://me.csdn.net/boonya)阅读数：110








原文地址：[https://www.cnblogs.com/ilinuxer/p/6364064.html](https://www.cnblogs.com/ilinuxer/p/6364064.html)

**目录**

[0.安装ipatbles.service](#0.%E5%AE%89%E8%A3%85ipatbles.service)

[1. 基本操作](#start)

[查看 IPTABLES 版本](#11)

[查看当前 IPTABLES 规则](#12)

[查看帮助](#13)

[2. iptables 规则中各指令（command）的含义](#command)

[–APPEND -A](#2a)

[–DELETE -D](#2d)

[–INSERT -I](#2i)

[–REPLACE -R](#2r)

[–LIST -L](#2l)

[–LIST-RULES -S](#2s)

[–FLUSH -F](#2f)

[–ZERO -Z](#2z)

[–NEW -N](#2n)

[–DELETE-CHAIN -X](#2x)

[–POLICY -P](#2p)

[–RENAME-CHAIN -E](#2e)

[3. iptables 规则中各参数的含义](#parameter)

[-P 代表协议（PROTOCOL）](#-P%20%E4%BB%A3%E8%A1%A8%E5%8D%8F%E8%AE%AE%EF%BC%88PROTOCOL%EF%BC%89)

[-S 代表源地址（SOURCE）](#-S%20%E4%BB%A3%E8%A1%A8%E6%BA%90%E5%9C%B0%E5%9D%80%EF%BC%88SOURCE%EF%BC%89)

[-D 代表目的地址（DESTINATION）](#-D%20%E4%BB%A3%E8%A1%A8%E7%9B%AE%E7%9A%84%E5%9C%B0%E5%9D%80%EF%BC%88DESTINATION%EF%BC%89)

[-J 代表跳转（TARGET）](#-J%20%E4%BB%A3%E8%A1%A8%E8%B7%B3%E8%BD%AC%EF%BC%88TARGET%EF%BC%89)

[-I 代表 IN INTERFACE（入站接口）](#-I%20%E4%BB%A3%E8%A1%A8%20IN%20INTERFACE%EF%BC%88%E5%85%A5%E7%AB%99%E6%8E%A5%E5%8F%A3%EF%BC%89)

[-O 代表 OUT INTERFACE（出站接口）](#-O%20%E4%BB%A3%E8%A1%A8%20OUT%20INTERFACE%EF%BC%88%E5%87%BA%E7%AB%99%E6%8E%A5%E5%8F%A3%EF%BC%89)

[4. 其它防火墙参数选项](#options)

[–SPORT 表示 SOURCE PORT (源端口，用于 -P TCP, 或者 -P UDP)](#%E2%80%93SPORT%20%E8%A1%A8%E7%A4%BA%20SOURCE%20PORT%20(%E6%BA%90%E7%AB%AF%E5%8F%A3%EF%BC%8C%E7%94%A8%E4%BA%8E%20-P%20TCP%2C%20%E6%88%96%E8%80%85%20-P%20UDP))

[–DPORT 表示 DESTINATION PORT (目的端口，-P TCP, 或者 -P UDP)](#%E2%80%93DPORT%20%E8%A1%A8%E7%A4%BA%20DESTINATION%20PORT%20(%E7%9B%AE%E7%9A%84%E7%AB%AF%E5%8F%A3%EF%BC%8C-P%20TCP%2C%20%E6%88%96%E8%80%85%20-P%20UDP))

[–TCP-FLAGS 表示 TCP FLAGS (用于 -P TCP)](#%E2%80%93TCP-FLAGS%20%E8%A1%A8%E7%A4%BA%20TCP%20FLAGS%20(%E7%94%A8%E4%BA%8E%20-P%20TCP))

[–ICMP-TYPE 表示 ICMP TYPE (ICMP 类型，用于 -P ICMP)](#%E2%80%93ICMP-TYPE%20%E8%A1%A8%E7%A4%BA%20ICMP%20TYPE%20(ICMP%20%E7%B1%BB%E5%9E%8B%EF%BC%8C%E7%94%A8%E4%BA%8E%C2%A0-P%20ICMP))

[5. 用于仅允许入站 SSH 连接的 iptables 示例](#example)

[1. 删除已有的规则](#1.%20%E5%88%A0%E9%99%A4%E5%B7%B2%E6%9C%89%E7%9A%84%E8%A7%84%E5%88%99)

[2. 仅允许 SSH](#2.%20%E4%BB%85%E5%85%81%E8%AE%B8%20SSH)

[3. 丢弃所有其它包](#3.%20%E4%B8%A2%E5%BC%83%E6%89%80%E6%9C%89%E5%85%B6%E5%AE%83%E5%8C%85)

[4. 查看 SSH 规则和测试](#4.%20%E6%9F%A5%E7%9C%8B%20SSH%20%E8%A7%84%E5%88%99%E5%92%8C%E6%B5%8B%E8%AF%95)

[iptables](http://cnzhx.net/blog/tag/iptables/) 是 Linux 中重要的访问控制手段，是俗称的 [Linux 防火墙](http://cnzhx.net/blog/centos-redhat-iptables-firewall-configuration/)系统的重要组成部分。这里记录了iptables 防火墙规则的一些常用的操作指令。

下面的操作以 CentOS 为基础介绍，应该对不同的 Linux 发行版都差不多。在 CentOS 5.x 和 6.x 中，iptables 是默认安装的（如果没有安装，先[安装 iptables](http://cnzhx.net/blog/iptable-installation-redhat-centos-rhel-fedora/) 即可）。如果对 iptables 的工作流程不太了解，可以先读读这篇 [iptables 工作流程的通俗理解](http://cnzhx.net/blog/intro-to-iptables-processing/)。

下面的操作指的是直接从 Linux 命令行输入的指令，也可以写成 .sh 文件批处理执行。

注意：操作的时候弄清楚指令的作用再执行，操作服务器请谨慎处理!

## 0.安装ipatbles.service

刚刚弄完了Tomcat后，想使用iptables来开放8080端口的时候，设定完之后，想保存规则，发生错误Failed to start ipatbles.service: Unit not found.原因是centos从7开始默认用的是firewalld，这个是基于iptables的，虽然有iptables的核心，但是iptables的服务是没安装的。

停止firewalld服务 停止防火墙，并禁用这个服务

```
sudo systemctl stop firewalld.service
sudo systemctl disable firewalld.service
```



安装iptables

如果你要改用iptables的话，需要安装iptables服务：

```
#安装
sudo yum install iptables-services
#开启iptables
sudo systemctl enable iptables
sudo systemctl enable ip6tables
#启动服务
sudo systemctl start iptables
sudo systemctl start ip6tables
```

---------------------

作者：狂飙的yellowcong

来源：CSDN

原文：https://blog.csdn.net/yelllowcong/article/details/75945339?utm_source=copy

版权声明：本文为博主原创文章，转载请附上博文链接！

## 1. 基本操作

### 查看 IPTABLES 版本
`rpm -q iptables`
这里返回的结果类似于 `iptables-1.4.7-9.el6.i686`。

### 查看当前 IPTABLES 规则
`service iptables status`
或者
`iptables -L --line-numbers`
该命令会以列表的形式显示出当前使用的 iptables 规则，每一条规则前面的编号可以用来做为其它操作——例如后面的删除操作——的参数，很有用。显示的结果类似于，

```
# service iptables status
Table: filter
Chain INPUT (policy ACCEPT)
num target prot opt source destination
1 ACCEPT all -- 0.0.0.0/0 0.0.0.0/0 state RELATED,ESTABLISHED
2 ACCEPT icmp -- 0.0.0.0/0 0.0.0.0/0
3 ACCEPT all -- 0.0.0.0/0 0.0.0.0/0
4 ACCEPT tcp -- 0.0.0.0/0 0.0.0.0/0 state NEW tcp dpt:22
5 REJECT all -- 0.0.0.0/0 0.0.0.0/0 reject-with icmp-host-prohibited

Chain FORWARD (policy ACCEPT)
num target prot opt source destination
1 REJECT all -- 0.0.0.0/0 0.0.0.0/0 reject-with icmp-host-prohibited

Chain OUTPUT (policy ACCEPT)
num target prot opt source destination
```

显示出该 iptables 中有一个表 filter，其中在 INPUT chain 中有 5 条规则，在 FORWARD chain 中有 1 条规则，在 OUTPUT chain 中没有规则。

### 查看帮助

操作指令也是直接在终端输入，
`iptables --help`
可以查看 iptables 的帮助信息：

```
Usage: iptables -[AD] chain rule-specification [options]
iptables -I chain [rulenum] rule-specification [options]
iptables -R chain rulenum rule-specification [options]
iptables -D chain rulenum [options]
iptables -[LS] [chain [rulenum]] [options]
iptables -[FZ] [chain] [options]
iptables -[NX] chain
iptables -E old-chain-name new-chain-name
iptables -P chain target [options]
iptables -h (print this help information)

指令Commands:
Either long（单词，以 -- 做前导） or short（字母，区分大小写，以 - 做前导） options are allowed.
--append -A chain Append to chain
--delete -D chain Delete matching rule from chain
--delete -D chain rulenum
Delete rule rulenum (1 = first) from chain
--insert -I chain [rulenum]
Insert in chain as rulenum (default 1=first)
--replace -R chain rulenum
Replace rule rulenum (1 = first) in chain
--list -L [chain [rulenum]]
List the rules in a chain or all chains
--list-rules -S [chain [rulenum]]
Print the rules in a chain or all chains
--flush -F [chain] Delete all rules in chain or all chains
--zero -Z [chain [rulenum]]
Zero counters in chain or all chains
--new -N chain Create a new user-defined chain
--delete-chain
-X [chain] Delete a user-defined chain
--policy -P chain target
Change policy on chain to target
--rename-chain
-E old-chain new-chain
Change chain name, (moving any references)
参数（选项）Options:
[!] --proto -p proto protocol: by number or name, eg. `tcp'
[!] --source -s address[/mask][...]
source specification
[!] --destination -d address[/mask][...]
destination specification
[!] --in-interface -i input name[+]
network interface name ([+] for wildcard)
--jump -j target
target for rule (may load target extension)
--goto -g chain
jump to chain with no return
--match -m match
extended match (may load extension)
--numeric -n numeric output of addresses and ports
[!] --out-interface -o output name[+]
network interface name ([+] for wildcard)
--table -t table table to manipulate (default: `filter')
--verbose -v verbose mode
--line-numbers print line numbers when listing
--exact -x expand numbers (display exact values)
[!] --fragment -f match second or further fragments only
--modprobe= try to insert modules using this command
 --set-counters PKTS BYTES set the counter during insert/append
 [!] --version -V print package version.
```

## 2. iptables 规则中各指令（command）的含义

各种指令如下（前面的帮助信息中列出来的第一大块内容）。既可以使用长指令（单词，以 — 做前导），也可以使用短指令字母，区分大小写，以 – 做前导），例如，
`iptables --append chain firewall-rule`
等价于，
`iptables -A chain firewall-rule`
下面介绍各种指令。因为形式都一样，下面针对 –append -A 作详细介绍，其它的只介绍其作用。举例时仅使用短指令。

另外，指令后面所带的参数/选项，如果以 [ ] 包括，则表示该参数/选项可省略（有缺省值）。

### –APPEND -A

指令形式，
`iptables -A chain firewall-rule `- -A chain – 指定规则应该添加到的 chain 的名称。例如，使用 INPUT 将会把规则添加到默认的 INPUT（入站）链的末尾，而使用 OUTPUT 则会将规则添加到出站链的末尾。
- firewall-rule – 包含各种参数选项的防火墙规则，一次一条规则

“-A” 的意思是附加（append），并不是添加（Add）的意思。它是将规则添加到已有的防火墙链（chain）的**末尾**的操作指令。这是特别重要的，因为规则在 iptables 中的位置非常重要，所以**千万要记住， -A 的意思是将规则添加到末尾**。因为一般情况下 iptables 中最后一条规则是丢弃所有数据包。如果你已经有了一条这样的规则，使用 -A 参数添加的规则将会在丢弃规则之后，以至于新规则根本无法起作用。**如果要插入到中间位置，需要用 -I 指令。**

如果不理解 chain（链）的含义，最好先读读 [iptables fundamentals](http://wiki.centos.org/HowTos/Network/IPTables)。

### –DELETE -D

从 chain 中删除与指定规则匹配，或指定编号的条目。

形式，
`iptables -D chain firewall-rule`
表示从 chain 中删除对应规则 firewall-rule 的那一条目。这种形式删除规则比较麻烦，通常用下面的形式
`iptables -D chain rulenum`
删除 chain 中编号为 rulenum 的那条规则。1 表示第一条。

### –INSERT -I

形式，
`iptables -I chain [rulenum] firewall-rule`
将 firewall-rule 添加为 chain 中的第 rulenum 条规则，原先的第 rulenum 条及以后各条的需要顺次 +1。如上面一样，1 表示该 chain 中第一条。默认为 1，即，如果没有指定 rulenum 则将该 chain 中第一条规则替换掉。

### –REPLACE -R

形式，
`iptables -R chain [rulenum] firewall-rule`
将 chain 中原来的第 rulenum 条规则替换为 firewall-rule。如果没有指定 rulenum，则替换该 chain 中第一条。

### –LIST -L

形式，
`iptables -L [chain [rulenum]]`
列出 chain 或者所有 chain（当未指定 chain 名称时）中的 第 rulenum 条规则或者所有规则（当未指定 rulenum 时）。不过要注意，如果未指定 chain 则不能带 rulenum 参数。

如果在 -L 后再加上 –line-numbers，则表示在每条规则前面显示序号。如，
`iptables -L --line-numbers`
### –LIST-RULES -S

形式，
`iptables -S [chain [rulenum]]`
打印出 chain 或者所有 chain（当未指定 chain 名称时）中的 第 rulenum 条规则或者所有规则（当未指定 rulenum 时）。不过要注意，如果未指定 chain 则不能带 rulenum 参数。

### –FLUSH -F

形式，
`iptables -F [chain]`
清空 chain 或所有 chain （当未指定 chain 名称时）中的规则。

### –ZERO -Z

形式，
`iptables -Z [chain [rulenum]]`
把 chain 或者所有 chain（当未指定 chain 名称时）的包及字节的计数器清空。

### –NEW -N

形式，
`--new -N chain`
创建一个名称为 chain 的新链。

### –DELETE-CHAIN -X

形式，
`iptables -X [chain]`
删除用户自定义 chain 或者所有用户自定义 chain（当未指定 chain 名称时）。该指令不影响预设规则（如 INPUT、OUTPUT、FORWARD 等）。

### –POLICY -P

形式，
`iptables -P chain target`
改变 chain 的策略为 target。

### –RENAME-CHAIN -E

形式，
`iptables -E old-chain new-chain`
将 old-chain 名称更改为 new-chain。这样做可以使所有引用 old-chain 的规则失效。

## 3. iptables 规则中各参数的含义

各种参数如下（前面的帮助信息中列出来的第二大块内容）。

### -P 代表协议（PROTOCOL）
- 指明当前规则针对的传输协议（如 TCP、UDP、ICMP 等）
- 可能的参数值有：tcp, udp, icmp, all
- 使用 “all” 表示适用于所有协议。而如果在规则中不指定 -p 参数，则默认使用 “all” 参数。一般不使用 “all” 这个值，要么指定某个特定的协议，要么就指定 -p 参数。
- -p 的参数值既可以用名称（如 tcp）也可以用协议对应的数值（如 6 代表 tcp 协议）
- /etc/protocols 文件中包含了所有允许的协议名称和相应数值
- 也可以用长的参数名 –protocol

### -S 代表源地址（SOURCE）
- 指定数据包的源地址
- 可以是 ip 地址，或者网络地址，或者主机名（hostname）
- 例如：-s 192.168.1.101 表示针对特定的 ip 地址
- 对于网络掩码，使用 /mask。例如，“-s 192.168.1.0/24″ 表示网络掩码为 255.255.255.0 的所有 192.168.1.x 地址都匹配。
- 如果不指定 -s 参数，默认匹配所有源地址
- 也可以用长参数名 –src 或者 –source

### -D 代表目的地址（DESTINATION）
- 指定数据包的目的地址
- 使用方式与上面的 “-s” 一样（不同之处仅在于 -s 指源，而 -d 表示目的地址）
- 也可以用长参数名 –dst 或者 –destination

### -J 代表跳转（TARGET）
- j 的意思是 “jump”（跳转） 到目标
- 指定当某个数据包满足该规则的时候的就跳转到的下一个处理规则，而不再顺序执行后面的规则判断
- 可能的值有：ACCEPT, DROP, QUEUE, RETURN，分别表示接受、丢弃、进入队列，返回（跳出，通常是从某个 chain 中跳回到调用该 chain 的上层 chain）
- 也可以跳转到某个自定义的 chain 中，使用该 chain 的名称做为跳转目标

### -I 代表 IN INTERFACE（入站接口）
- i 表示 “input interface”（输入接口，即，指定网络数据处理的网卡，一般 eth0 即表示第一块有线网卡的外网接口，lo 表示局域网接口）
- 可以直接理解为 “-i” 表示接口。不过，-i 和 -o 都表示接口，-i 表示输入时的接口，而 -o 特指输出用的接口。
- 指定数据包进入 INPUT、FORWARD 和 PREROUTING 链时经由的接口。
- 例如：-i eth0 表示该规则应该针对从 eth0 接口进来的数据包。
- 如果不指定 -i 参数，则经由系统中所有可用的接口进入的数据包都可以匹配该规则。
- 也可以使用长参数 –in-interface

### -O 代表 OUT INTERFACE（出站接口）
- o 表示 “output interface”（出站经由接口）
- 指定发送出去的数据包进入 INPUT、FORWARD 和 PREROUTING 链时经由的接口。
- 如果不指定 -o 参数，则经由系统中所有可用的接口发出的数据包都可以匹配该规则。
- 也可以使用长参数 –out-interface

## 4. 其它防火墙参数选项

上面某些防火墙参数还有属于它自己的参数选项，可以与其配合使用。下面是一些常用的选项。

要使用这些参数选项，需要指定相应的[参数](http://cnzhx.net/blog/common-iptables-cli/#parameter)，例如，要使用 “–sport” 选项，应该在规则中指定 “-p tcp” （或者 “-p udp”）等参数。

注意：所有这些选项前面都是 –（2个短横线 – ）。

### –SPORT 表示 SOURCE PORT (源端口，用于 -P TCP, 或者 -P UDP)
- 默认匹配所有端口（未特别指定时）
- 可以指定端口号（数字），也可以指定端口名称。例如，默认的 SSH 端口号码为 22，名称为 ssh，可以表示为 “–sport 22″ 或者 “–sport ssh”。
- /etc/services 文件包含了所有允许的端口名称和对应的端口号码。
- 在规则中使用号码比使用名称要好（效率高些）
- 要匹配一个端口范围，使用英文半角冒号（:），如 22:100 匹配从 22 到 100 的所有端口号。
- 也可以使用长名称 –source-port

### –DPORT 表示 DESTINATION PORT (目的端口，-P TCP, 或者 -P UDP)
- 与 –sport 的用法相同，区别仅在于对象是目的端口
- 也可以使用长名称 –destination-port

### –TCP-FLAGS 表示 TCP FLAGS (用于 -P TCP)
- 可以使用英文半角逗号（,）来指定多个 TCP 状态标识
- 可能值有：SYN, ACK, FIN, RST, URG, PSH。可以全用，也可以不用。

### –ICMP-TYPE 表示 ICMP TYPE (ICMP 类型，用于 -P ICMP)
- 当使用 icmp 协议 “-p icmp” 的时候，可以使用 “–icmp-type” 特别指定 ICMP 类型
- 例如，使用 “–icmp-type 0″ 表示 “Echo Reply”，“–icmp-type 8″ 表示 “Echo”。

## 5. 用于仅允许入站 SSH 连接的 iptables 示例

在了解了各种 iptables 参数以及参数选项之后，再来看看下面的实例。

在这个例子中，我们的目标是：仅允许 SSH 连接到服务器，而其它所有连接都被禁止（包括 ping）。

警告：如果设置不当，可能会使系统无法通过网络访问，所以在玩 VPS 的时候要特别注意。不过如果是 Linode VPS 的话，它提供了 LISH 接口，可以让你象访问本地系统一样的操作 VPS，倒也不用太担心，只是有些麻烦而已。如果是别的 VPS，我就不太了解了。总之，万事小心！最好在本地的虚拟机里面测试这些玩意儿，否则把自己挡在了 VPS 外面无法通过 SSH 来连接控制台，那可就不好玩了。

### 1. 删除已有的规则

如果已经有了某些规则，需要先备份一下，以后恢复起来也方便:

```
service iptables save 

iptables: Saving firewall rules to /etc/sysconfig/iptables:[ OK ]
```

默认保存为 `/etc/sysconfig/iptables`。我们可以将这个文件再备份一下，
`cp /etc/sysconfig/iptables /etc/sysconfig/iptables.x0`
然后清空所有规则，从头开始，
`iptables -F`
当我们完成下面的设置后，就只能通过 SSH 连接服务器，而无法 ping 通或者访问服务器上的别的服务（如网站）了。切记不要在生产环境中玩。

### 2. 仅允许 SSH

下面开始设置使得服务器只接受 SSH 连接（不限制发起连接的位置），假设 SSH 使用了默认的端口 22.

使用下面的规则，让服务器接受所有通过 22 号端口进入的连接。
`iptables -A INPUT -i eth0 -p tcp --dport 22 -j ACCEPT`
上面的指令中包含了 4 个部分：
- “-A INPUT” – 表示我们要在 INPUT 链的最后增加后面的规则。所以这条规则将会应用于入站的数据包。
- “-i eth0″ – 表示检查通过 eth0 接口进入的数据包，看看是否匹配此规则。
- “-p tcp –dport 22″ – 该规则针对的是 TCP 数据包，并且使用了 -p 参数的选项 “–dport 22″ 来指定该规则针对的是要发送到服务器 22 号端口（即 SSH 端口）的数据包。
- “-j ACCEPT” – 跳转到 ACCEPT，即接受满足这些匹配规则的数据包的传输。

用普通话来讲，上面的规则就是说：所有从 eth0 接口传入的 SSH 连接都接受。

### 3. 丢弃所有其它包

指明了系统可以接受的连接之后，一般要设置一条“丢弃”规则，将所有那些没有打算接受的数据包统统抛弃。

这样的规则应该放到 INPUT 链的最末位置。

要丢弃所有数据包（因为在刚才的那条接受 SSH 连接的规则的后面，所以不会影响到刚才那条规则），使用下面的命令添加规则，
`iptables -A INPUT -j DROP`
### 4. 查看 SSH 规则和测试

要查看刚才添加的规则，使用命令 `iptables -L`，下面同时给了该命令的输出结果。

```
# iptables -L
Chain INPUT (policy ACCEPT)
target     prot opt source               destination
ACCEPT     tcp  --  anywhere             anywhere            tcp dpt:ssh
DROP       all  --  anywhere             anywhere
```

从上面的结果可以看到，规则是按照我们输入的顺序排列的，
- 第一条接受所有传入的 SSH 连接；
- 第二条丢弃所有剩下（与第一条不匹配)的数据包。

如果不想一条条的从命令行输入这些规则命令，最好创建一个脚本文件（shell script）来包含所有规则，然后自动执行。例如，

```
# vi iptables.sh
iptables -A INPUT -i eth0 -p tcp --dport 22 -j ACCEPT
iptables -A INPUT -j DROP

# sh -x iptables.sh
+ iptables -A INPUT -i eth0 -p tcp --dport 22 -j ACCEPT
+ iptables -A INPUT -j DROP

# iptables -L INPUT
Chain INPUT (policy ACCEPT)
target     prot opt source               destination
ACCEPT     tcp  --  anywhere             anywhere            tcp dpt:ssh
DROP       all  --  anywhere             anywhere
```

看了上面的 iptables 命令及其参数/选项之后，应该就可以看懂[我的 VPS 安全策略](http://cnzhx.net/blog/secure-my-vps/)中的 iptables 设置了。

上面的介绍针对的是 iptables，也就是针对 IPv4 的防火墙。IPv6 的与之类似，不过 [IPv6 的防火墙](http://cnzhx.net/blog/centos-rhel-ipv6-ip6tables-firewall-configuration/)是 ip6tables。[©](http://cnzhx.net/blog/common-iptables-cli/)

**5.设置chain策略**

对于filter table，默认的chain策略为ACCEPT，我们可以通过以下命令修改chain的策略：

```bash
iptables -P INPUT DROP iptables -P FORWARD DROP iptables -P OUTPUT DROP
```

以上命令配置将接收、转发和发出包均丢弃，施行比较严格的包管理。由于接收和发包均被设置为丢弃，当进一步配置其他规则的时候，需要注意针对INPUT和OUTPUT分别配置。当然，如果信任本机器往外发包，以上第三条规则可不必配置。

**6.屏蔽指定ip**

有时候我们发现某个ip不停的往服务器发包，这时我们可以使用以下命令，将指定ip发来的包丢弃：

```bash
BLOCK_THIS_IP="x.x.x.x" iptables -A INPUT -i eth0 -p tcp -s "$BLOCK_THIS_IP" -j DROP
```

以上命令设置将由x.x.x.x ip发往eth0网口的tcp包丢弃。

**7.配置服务项**

利用iptables，我们可以对日常用到的服务项进行安全管理，比如设定只能通过指定网段、由指定网口通过SSH连接本机：

```bash
iptables -A INPUT -i eth0 -p tcp -s 192.168.100.0/24 --dport 22 -m state --state NEW,ESTABLESHED -j ACCEPT 

iptables -A OUTPUT -o eth0 -p tcp --sport 22 -m state --state ESTABLISHED -j ACCEPT
```

若要支持由本机通过SSH连接其他机器，由于在本机端口建立连接，因而还需要设置以下规则：

```bash
iptables -A INPUT -i eth0 -p tcp -s 192.168.100.0/24 --dport 22 -m state --state ESTABLESHED -j ACCEPT 

iptables -A OUTPUT -o eth0 -p tcp --sport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
```

类似的，对于HTTP/HTTPS(80/443)、pop3(110)、rsync(873)、MySQL(3306)等基于tcp连接的服务，也可以参照上述命令配置。

对于基于udp的dns服务，使用以下命令开启端口服务：

```bash
iptables -A OUTPUT -p udp -o eth0 --dport 53 -j ACCEPT 

iptables -A INPUT -p udp -i eth0 --sport 53 -j ACCEPT
```

**8.网口转发配置**

对于用作防火墙或网关的服务器，一个网口连接到公网，其他网口的包转发到该网口实现内网向公网通信，假设eth0连接内网，eth1连接公网，配置规则如下：

```bash
iptables -A FORWARD -i eth0 -o eth1 -j ACCEPT
```

**9.端口转发配置**

对于端口，我们也可以运用iptables完成转发配置：

```bash
iptables -t nat -A PREROUTING -p tcp -d 192.168.102.37 --dport 422 -j DNAT --to 192.168.102.37:22
```

以上命令将422端口的包转发到22端口，因而通过422端口也可进行SSH连接，当然对于422端口，我们也需要像以上“4.配置服务项”一节一样，配置其支持连接建立的规则。

**10.DoS攻击防范**

利用扩展模块limit，我们还可以配置iptables规则，实现DoS攻击防范：

```bash
iptables -A INPUT -p tcp --dport 80 -m limit --limit 25/minute --limit-burst 100 -j ACCEPT
```

--litmit 25/minute 指示每分钟限制最大连接数为25

--litmit-burst 100 指示当总连接数超过100时，启动 litmit/minute 限制

**11.配置web流量均衡**

我们可以将一台服务器作为前端服务器，利用iptables进行流量分发，配置方法如下：

```bash
iptables -A PREROUTING -i eth0 -p tcp --dport 80 -m state --state NEW -m nth --counter 0 --every 3 --packet 0 -j DNAT --to-destination 192.168.1.101:80 

iptables -A PREROUTING -i eth0 -p tcp --dport 80 -m state --state NEW -m nth --counter 0 --every 3 --packet 0 -j DNAT --to-destination 192.168.1.102:80 

iptables -A PREROUTING -i eth0 -p tcp --dport 80 -m state --state NEW -m nth --counter 0 --every 3 --packet 0 -j DNAT --to-destination 192.168.1.103:80
```

以上配置规则用到nth扩展模块，将80端口的流量均衡到三台服务器。

**12.将丢弃包情况记入日志**

使用LOG目标和syslog服务，我们可以记录某协议某端口下的收发包情况。拿记录丢包情况举例，可以通过以下方式实现。

首先自定义一个chain：

```bash
iptables -N LOGGING
```

其次将所有接收包导入LOGGING chain中：

```bash
iptables -A INPUT -j LOGGING
```

然后设置日志前缀、日志级别：

```bash
iptables -A LOGGING -m limit --limit 2/min -j LOG --log-prefix "IPTables Packet Dropped: " --log-level 7
```

最后将包倒向DROP，将包丢弃：

```bash
iptables -A LOGGING -j DROP
```

另可以配置syslog.conf文件，指定iptables的日志输出。



