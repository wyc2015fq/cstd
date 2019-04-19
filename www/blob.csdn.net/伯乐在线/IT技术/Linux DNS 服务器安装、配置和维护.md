# Linux DNS 服务器安装、配置和维护 - 文章 - 伯乐在线
原文出处： [dzone](https://dzone.com/articles/linux-dns-server-installation-amp-configuration-am)   译文出处：[开源中国](https://www.oschina.net/translate/linux-dns-server-installation-amp-configuration-am?print)
每个 IP 地址都可以有一个主机名，主机名由一个或多个字符串组成，字符串之间用小数点隔开。有了主机名，就不要死记硬背每台 IP 设备的 IP 地址，只要记住相对直观有意义的主机名就行了。这就是 DNS 协议所要完成的功能。
今天我们将讨论DNS服务器，特别是Linux DNS服务器，及其如何安装、配置和维护它。
## /etc/hosts文件
在没有DNS服务器的情况下，每个系统在本地网络上保留其主机名和相应IP地址列表的副本是合理的——特别是在没有互联网连接的小型站点上。
在Linux系统中，这个列表就是/etc/hosts文件。 即使你没有DNS服务器或DNS服务器不可用，该文件也可以使用/etc/hosts文件将IP地址转换为名称。
也许你已经有DNS服务器了，但你也会因为其它原因而想保留这个文件。例如，系统可能需要在向外部查询之前在本地查找DNS服务器的IP地址；这意味着系统在查询DNS服务器之前先检索该文件，如果查找到对应的域则无须查询任何DNS服务器直接将其转换为IP地址。
试试编辑下/etc/hosts文件，并添加以下信息：*127.0.0.1 google.com*.
然后，返回你的浏览器，输入*google.com*，看看结果如何。如果你的系统上安装了Apache并且本地主机正在运行，浏览器会显示localhost的索引页，而不是Google页面。
![](http://jbcdn2.b0.upaiyun.com/2017/04/8fb10f47603b46da8bdf9d933cbdc291.png)
作为确认，你可以将google.com映射到任何网站的任何其他IP地址并查看结果。
因此这个文件所做的是将IP地址转换成名字，但这仅仅是在同一互相连接的网络下。 那么外部网络和众多系统的所有记录是如何维护的呢？
每个人都需要维护自己的/etc/hosts文件并自己更新吗？
更为稳健的域名服务是DNS服务器。
## 域名
当你访问网站时，你可以输入FQDN（Fully Qualified Domain Name，完全限定域名）或类似likegeeks.com或www.google.com的域名。在域名中从右到左的两个点之间的每个文本依次是顶级域组件、二级域组件和三级域组件。
所以，com是顶级域名组件; google是二级域组件; 而www是三级域名组件。
实际上，当你访问任何网站时，浏览器会默认在域的末尾添加一个不可见的点，因此该域将像www.google.com.一样。 该点被称为根域。
该点是由一大堆称为根域名服务器的特殊服务器管理的。截止这篇文章发表前，世界上有13个根域名服务器。 你可以把他们当成互联网的大脑 – 如果他们失效了，世界上就没有互联网了。
为什么是13呢？ 因为如果世界的某处地震可能会破坏一个根服务器，所以其他的服务器可以继续提供服务直到受影响的服务器重新上线。
这些根名称服务器按字母顺序命名，名称如a.root-server.net、b.root-server.net等。
## 顶级域名 (或称作一级域名 TLDs)
我们已经见过顶级域名的组成部分，如 com。可以认为，顶级域名为 DNS 命名空间提供分类组织。
顶级域名（TLD）根据地理或功能方面分为几类。
截止本文撰写时，网上有 800 多个顶级域名。
顶级域名类别有：
- 通用的顶级域名如：*org*, *.com*, *.net*, *.gov*, *.edu 等等*
- 国家代码顶级域名如*：.us*,* .ca**等，*分别对应美国和加拿大的国家代码
- 新的品牌顶级域名，允许组织创建最多 64 个字符的TLD，如：*.linux*, *.microsoft*, *.companyname 等*
- 基础架构顶级域名如： *.arpa*
## 子域名
当你访问一个类似 *mail.google.com *这样的网站, 这里的*mail* 就是 *google.com*的子域名.
只有*mail.google.com* 的名称服务器知道他下面存在的所有主机，所以Google会回复是否有一个叫*mail* 的子域名。根名称服务器对此并不知情。
## DNS服务器的类型
一共有三种DNS服务器。
### 主DNS服务器
这些服务器上存放了特定域名的配置文件，并且基于此权威地规定了特定域名的地址。主DNS服务器知道全部在它管辖范围的主机和子域名的地址。
### 辅助DNS服务器
这些服务器作为主DNS服务器的备份，也承担一定负载。主服务器知道辅助DNS服务器的存在，并且会向他们推送更新。
### 缓存DNS服务器
这些服务器上不存放特定域名的配置文件。当客户端请求缓存服务器来解析域名时，该服务器将首先检查其本地缓存。如果找不到匹配项便会询问主服务器。接着这条响应将被缓存起来。您也可以轻松地将自己的系统用作缓存服务器。
## 搭建 Linux DNS 服务器
Linux 下有很多实现了 DNS 功能的包，不过我们只关注 BIND DNS 服务器。它用于世界上大多数 DNS 服务器。
如果你在使用基于 Red Hat 发行版的 Linux，比如 CentOS，可以像这样安装：`$ dnf -y install bind`
如果你使用基于 Debian 的操作系统，比如 Ubuntu：`$ apt-get install bind9`
安装完成之后就可以启动它并让它在计算机启动的时候一并启动起来。


```
$ systemctl start named
$ systemctl enable named
```
## 配置 BIND
这个服务使用 `/etc/named.conf` 作为配置文件。
BIND 在那个文件中使用像下面这样的一些语句：
- **options**: 用于全局 BIND 配置。
- **logging**: 配置哪些需要记录，哪些需要忽略。我推荐你看看 [Linux syslog server](https://likegeeks.com/linux-syslog-server-log-management/)。
- **zone**: 定义 DNS 区域。
- **include**: 在 `named.conf` 中包含另一个文件。
在 options 语句中可以看到 BIND 的工作目录在 `/var/named`。
zone 语句可用于定义 DNS 区域，比如域名 *google.com*，它包含子域名 *mail.google.com* 和 *analytics.google.com*。
上述三个域名(主域名和子域名) 都有一个由 zone 语句定义的区域。
## 定义一个主域服务器
我们知道 DNS 服务器类型有主域名服务器、辅助域名服务器和缓存域名服务器。不同于缓存域名服务器，主域名服务器和辅助域名服务器在应答过程中是处于同等地位的。
在 `/etc/named.conf` 的配置文件中，你可以使用如下语法定义一个主域服务器：


```
zone    "likegeeks.com" {
type master;
file likegeeks.com.db
};
```
包含主要区域信息的文件存放在 `/var/named` 目录下，从 options 可知，这是一个工作目录。
注意：软件服务器或者托管面板会根据你的域名自动为你创建主域服务器信息的文件名，因此如果你的域名是 *example.org*，那么你主域服务器信息的文件就为 `/var/named/example.org.db`。
类型为 `master`，也就是说这是一个主域服务器。
# 定义一个辅助域服务器
同定义一个主域服务器一样，辅助域服务器的定义稍微有些变化：


```
zone    "likegeeks.com" {
type slave;
masters IP Address list; ;
file likegeeks.com.db
};
```
对于辅助域服务器来说，它的域名和主域服务器是一样的。上述语法里的的slave类型表示这是一个辅助域服务器，“masters IP Address list”表示辅助域服务器中区域文件内的信息都是通过主域服务器中区域文件内的信息复制过来的。
# 定义一个缓存服务器
即使你已经配置了主域或者辅助域服务器，你仍有必要（不是必须）定义一个缓存服务器，因为这样你可以减少DNS服务器的查询次数。
在定义缓存服务器之前，你需要先定义三个区域选择器，第一个：


```
zone      "." IN {
type hint;
file "root.hint";
};
```


```
zone      "." IN {
type hint;
file "root.hint";
};
```


```
zone      "." IN {
type hint;
file "root.hint";
};
```


```
zone      "localhost" IN {
type master;
file "localhost.db";
};
```
定义第三个区域是为了反向查找到本地主机。这种反向查找是把本地的IP地址执向本地主机。


```
zone      "0.0.127.in-addr.arpa" IN {
type master;
file "127.0.0.rev";
};
```
把这三个区域信息放到/etc/named.conf文件里，你的系统就可以以缓存服务器来工作了。但是如何引用类似*likegeeks.com.db*, *localhost.db*, 和 *127.0.0.rev*这些文件中的内容呢？
这些文件包含具有某些选项的每个区域的DNS记录类型。 那么，这些DNS记录类型是什么以及它们是如何写的？
**DNS记录类型**
数据库文件包含诸如SOA、NS、A、PTR、MX、CNAME和TXT在内的记录类型。
我们看看每一种类型都是如何记录的吧。
**SOA：起始授权机构记录**
SOA记录按如下形式开始描述一个站点的DNS条目：


```
example.com.      86400      IN      SOA      ns1.example.com.   mail.example.com.      (
2017012604 ;serial
86400 ;refresh, seconds
7200 ;retry, seconds
3600000 ;expire, seconds
86400 ;minimum, seconds
)
```
第一行以域名example.com开始，以句号结束——该语句和/etc/named.conf文件中的区域定义是一致的。我们要始终记得，DNS配置文件是极其挑剔的。
IN 告诉域名服务器：这是一条网络记录。
SOA 告诉域名服务器：这是一条起始授权机构记录。
ns1.example.com. 是该文件所在域的域名服务器的完全合格域名（FQDN: Fully Qualified Domain Name)。
mail.host.com. 是域管理员的邮箱地址。你会发现这个邮箱地址没有“@”标志，而是被句号所取代，并且末尾还有一个句号。
第2行是一个序列码，它被用来告诉域名服务器文件是什么时候升级的。因此，如果你对区域码做了变更，你必须对这个序列码进行递增。这个序列码的格式是 YYYYMMDDxx ，其中的 xx 是从 00 开始的。
第3行是每秒刷新率。这个值被用来告诉第二个域名服务器查询主服务器中的记录是否已经被更新的频率。
第4行是每秒重试的频率。如果第二个服务器多次尝试连接主域名服务器来进行更新检测，但无法连接上的时候，第二个服务器就会在每秒内重试指定的数值次数。
第5行是超时指示。其目的是为了第二个服务器能将区域数据缓存下来。这个值告诉这些服务器如果它们不能连接到主服务器来进行更新，那么它们就会在这个指定数值秒数之后抛弃这个值。
第6行告诉缓存服务器，如果它们不能连接到主域名服务器时，它们应该在超时前等待多久。
### NS: Name Server Records(名称服务器记录)
NS记录用于指定哪个名称服务器维护该域的记录。
你可以这样编写的NS记录：


```
IN           NS         ns1.example.com.
IN           NS         ns2.example.com.
```
并不需要有2个NS记录，但是通常偏好有备份名称服务器。
### A和AAAA: Address Records（地址记录）
A记录用于提供从主机名到IP地址的映射*support IN A 192.168.1.5*。
如果你在地址为192.168.1.5上的support.example.com上有一个主机，你可以像上面的例子那样输入。
请注意，我们所写的主机并没有句号。
### PTR: Pointer Records（指针记录）
PTR记录用于执行反向名称解析，允许某人指定IP地址然后找出对应的主机名。
这与A记录的功能相反：192.168.1.5 IN PTR *support.example.com*.
在这里，我们键入具有点号的完整主机名。
### MX: Mail Exchange Records（邮件交换记录）
MX记录告诉其他站点关于你所在域的邮件服务器地址：example.com. IN MX 10 mail.
当然这个域以句号结束。数字10是邮件服务器的重要性标志，如果你拥有多个邮件服务器，其中较小的数字不太重要。
### CNAME: Canonical Name Records（权威名称记录）
CNAME记录允许你为主机名创建别名。当你想提供一个易于记住的名称时，这很有用。
假设某个站点具有一个主机名为*whatever-bignameis.example.com*的Web服务器，并且由于系统是Web服务器，因此可以为主机创建一个名为www的CNAME记录或者别名。
你可以创建名为www.example.com的域名创建CNAME记录：


```
whatever-bignameis      IN           A                    192.168.1.5
www                     IN           CNAME                whatever-bignameis
```
第一行通知DNS服务器关于别名的位置。第二行创建一个指向www的别名。
## TXT记录
您可以将任何信息存储到TXT记录中，例如你的联系方式或者你希望人们在查询DNS服务器时可获得的任意其他信息。
你可以这样保存TXT记录：example.com. IN TXT ” YOUR INFO GOES HERE”.
此外，RP记录被创建为对host联系信息的显式容器：example.com. IN RP mail.example.com. example.com。
## DNS TTL值
在/etc/named.conf文件的顶部，这里有一个$TTL条目**。**
该条目告诉BIND每个单独记录的TTL值（time to live，生存时间值）。
它是以秒为单位的数值，比如14,400秒（4个小时），因此DNS服务器最多缓存你的域文件4个小时，之后就会向你的DNS服务器重新查询。
你可以降低这个值，但是默认值通常是合理的。除非你知道你正在做什么。
## 捕获配置错误
当您写入域文件时，也许您忘记了一个句号或空格或其他任意错误。
你可以从日志诊断Linux DNS服务器错误。BIND服务通过/var/log/messages上的错误，可以使用tail命令来查看实时错误日志，须使用-f选项：$ tail -f /var /log/messages。
因此，当你编写域文件或修改/etc/named.config并重新启动服务时，显示错误之后，你可以从日志中轻松识别错误类型。
## Host命令
在你成功添加或修改记录后，可以使用host命令查看主机是否正确解析。
host命令允许你将主机名解析为IP地址：$ host example.com。
此外，你可以执行反向查找：$ host 192.168.1.5。
你可以[this在此篇文章中查看更多关于host和dig命令](https://likegeeks.com/linux-network-commands/)的信息。
## Whois命令
whois命令用于确定域名的所有权及其拥有者的e-mail地址和联系电话：$ whois example.com.
## Rndc命令
rndc工具可用于安全地管理名称服务器，因为与服务器的所有通信均通过数字签名进行身份验证。
此工具用于控制名称服务器和调试问题。 你可以通过以下方式检查Linux DNS服务器的状态：$ rndc status。
此外，如果你更改任何域(zone)文件，您可以重新加载服务，而无须重启命名服务：$ rndc reload example.com。
在这里，我们重新加载example.com域文件。 你可以重新加载所有域：$ rndc reload。
或者你可以添加新的域或更改服务的配置。 你可以重新加载配置，如下所示：$ rndc reconfig。
## Linux DNS解析器
我们已经知道Linux DNS服务器的工作原理以及如何配置它。另一部分当然是与DNS服务器交互的（正在与DNS服务器通信以将主机名解析为IP地址的）客户端。
在Linux上，解析器位于DNS的客户端。要配置解析器，可以检查/etc/resolv.conf这个配置文件。
在基于Debian的发行版上，可以查看/etc/resolvconf/resolv.conf.d/目录。
/etc/resolv.conf文件中包含客户端用于获取其本地DNS服务器地址所需的信息。
第一个表示默认搜索域，第二个表示主机名称服务器(nameserver)的IP地址。
名称服务器行告诉解析器哪个名称服务器可使用。 只要你的BIND服务正在运行，你就可以使用自己的DNS服务器。
使用Linux DNS服务器非常简单。 我希望你发现这篇文章很有用，并且很容易理解。
