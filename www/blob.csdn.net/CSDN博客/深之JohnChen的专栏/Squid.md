# Squid - 深之JohnChen的专栏 - CSDN博客

2012年01月05日 09:22:06[byxdaz](https://me.csdn.net/byxdaz)阅读数：2217


Squid cache（简称为Squid）是一个流行的自由软件（GNU通用公共许可证）的代理服务器和Web缓存服务器。Squid有广泛的用途，从作为网页服务器的前置cache服务器缓存相关请求来提高Web服务器的速度，到为一组人共享网络资源而缓存万维网，域名系统和其他网络搜索，到通过过滤流量帮助网络安全，到局域网通过代理上网。Squid主要设计用于在Unix一类系统运行。

Squid的发展历史相当悠久，功能也相当完善。除了HTTP外，对于FTP与HTTPS的支援也相当好，在3.0 测试版中也支援了IPv6。 

一、代理服务器概述 

1.1什么是代理服务器  
在TCP/IP网络中，传统的通信过程是这样的：客户端向服务器请求数据，服务器响应该请求，将数据传送给客户端。在引入了代理服务器以后，这一过程变成了这样：客户端向服务器发起请求，该请求被送到代理服务器；代理服务器分析该请求，先查看自己缓存中是否有请求数据，如果有就直接传送给客户端，如果没有就代替客户端向该服务器发出请求。服务器响应以后，代理服务器将响应的数据传送给客户端，同时在自己的缓存中保留一份该数据的拷贝。这样，再有客户端请求相同的数据时，代理服务器就可以直接将数据传送给客户端，而不需要再向该服务器发起请求。 

1.2 代理服务器的功能  
一般说来，代理服务器具有以下的功能：  
1.通过缓存增加访问速度  
随着Internet的迅猛发展，网络带宽变得越来越珍贵。所以为了提高访问速度，好多ISP都提供代理服务器，通过代理服务器的缓存功能来加快网络的访问速度。一般说来，大多数的代理服务器都支持HTTP缓存，但是，有的代理服务器也支持FTP缓存。在选择代理服务器时，对于大多数的组织，只需要HTTP缓存功能就足够了。  
通常，缓存有主动缓存被动缓存之分。所谓被动缓存，指的是代理服务器只在客户端请求数据时才将服务器返回的数据进行缓存，如果数据过期了，又有客户端请求相同数据时，代理服务器又必须重新发起新的数据请求，在将响应数据传送给客户端时又进行新的缓存。所谓主动缓存，就是代理服务器不断地检查缓存中的数据，一旦有数据过期，则代理服务器主动发起新的数据请求来更新数据。这样，当有客户端请求该数据时就会大大缩短响应时间。还需要说明的是，对于数据中的认证信息，大多数的代理服务器都不会进行缓存的。  
2.提供用私有IP访问Internet的方法  
IP地址是不可再生的宝贵资源，假如你只有有限的IP地址，但是需要提供整个组织的Internet访问能力，那么，你可以通过使用代理服务器来实现这一点。  
3.提高网络的安全性  
这一点是很明显的，如果内部用户访问Internet都是通过代理服务器，那么，代理服务器就成为进入Internet的唯一通道；反过来说，代理服务器也是Internet访问内部网的唯一通道，如果你没有做反向代理，则对于Internet上的主机来说，你的整个内部网只有代理服务器是可见的，从而大大增强了网络的安全性。  

1.3 代理服务器的分类及特点  
通常的代理服务器分类方法，是从实现的机理分为线路层代理、应用层代理、智能线路层代理等等。在这里，我想从另外一个角度出发，把代理服务器分为传统代理服务器和透明代理服务器。  
我认为有必要好好搞清楚两者的区别，只有真正明白了内在地机理，才能在遇到问题时，有章可循，才不会一头雾水，不知从何解决问题。因此，下面我们就通过具体的实例来说明。本章的写作思路来源于Paul Russell所写的IPCHAINS-HOWTO。下面所举的例子也来源于该文章，我觉得我读该文的最大收获在于对内部网访问外部网以及外部网访问内部网的实现手段有了一个清晰的认识。当然，这里所谓的内部网是指使用私有IP的内部网络。  
我们的例子都基于以下假设：  
你的域名为sample.com,你的内部网(192.168.1.*)用户通过proxy.sample.com(外部接口 eth0:1.2.3.4;内部接口 eth1:192.168.1.1)的代理服务器访问Internet，换句话说，该代理服务器是唯一一台直接与Internet和内部网相连的机器。并假该设代理服务器上运行着某种代理服务器软件（如squid)。假设内部网中某一客户机为client.sample.com(192.168.1.100)。  
+-------------------+  
|内部网(192.168.1.*)| eth1+--------+eth0 DDN  
| +------------| proxy |<===============>;Internet  
|client198.168.1.100| +--------+  
+-------------------+  
eth0: 1.2.3.4  
eth1: 198.168.1.1  

1.3.1传统代理  
在以上基础上我们做以下工作：  
1.代理服务软件被绑定到代理服务器的8080端口。  
2.客户端浏览器被配置使用代理服务器的8080端口。  
3.客户端不需要配置DNS。  
4.代理服务器上需要配置代理服务器。  
5.客户端不需要配置缺省路由。  
当我们在客户端浏览器中打开一个web请求，比如“http://www.linuxaid.com.cn”，这时将陆续发生以下事件：  
1.客户端使用某一端口（比如1025)连接代理服务器8080端口，请求web页面“http://www.linuxaid.com.cn”  
2.代理服务器向DNS请求“www.linuxaid.com.cn”,得到相应的IP地址202.99.11.120。然后，代理服务器使用某一端口（比如1037)向该IP地址的80端口发起web连接请求，请求web页面。  
3.收到响应的web页面后，代理服务器把该数据传送给客户端。  
4.客户端浏览器显示该页面。  
从www.linuxaid.com.cn的角度看来，连接是在1.2.3.4地1037端口和202.99.11.120的80端口之间建立的。从client的角度看来，连接是在192.168.1.100的1025端口和1.2.3.4的8080端口之间建立的。 

1.3.2 透明代理  
透明代理的意思是客户端根本不需要知道有代理服务器的存在。  
在以上基础上我们做以下工作：  
1.配置透明代理服务器软件运行在代理服务器的8080端口。  
2.配置代理服务器将所有对80端口的连接重定向到8080端口。  
3.配置客户端浏览器直接连解到Internet。  
4.在客户端配置好DNS.  
5.配置客户端的缺省网关为192.168.1.1.  
当我们在客户端浏览器中打开一个web请求，比如“http://www.linuxaid.com.cn”，这时将陆续发生以下事件：  
1.客户端向DNS请求“www.linuxaid.com.cn”,得到相应的IP地址202.99.11.120。然后，客户端使用某一端口（比如1066)向该IP地址的80端口发起web连接请求，请求web页面。  
2.当该请求包通过透明代理服务器时，被重定向到代理服务器的绑定端口8080。于是，透明代理服务器用某一端口（比如1088)向202.99.11.120的80端口发起web连接请求，请求web页面。  
3.收到响应的web页面后，代理服务器把该数据传送给客户端。  
4.客户端浏览器显示该页面。  
从www.linuxaid.com.cn的角度看来，连接是在1.2.3.4地1088端口和202.99.11.120的80端口之间建立的。从client的角度看来，连接是在192.168.1.100的1066端口和202.99.11.120的80端口之间建立的。  
以上就是传统代理服务器和透明代理服务器的区别所在。  

二、各种代理服务器的比较  
linux下的代理服务器软件很多，我从www.freshmeat.com(一个著名的linux软件站点）查看了一下，足有六十多个。但是被广泛应用的只有Apache、socks、squid等几个实践证明是高性能的代理软件。下面我们分别来比较一下这几个软件：  

2.1 Apache  
Apache是世界上用的最广泛的HTTP服务器，之所以用的最广泛，是因为它强大的功能、高效率、安全性和速度。从1.1.x版本开始，Apache开始包含了一个代理模块。用Apache作代理服务器的性能优势并不明显，不建议使用。  

2.2 Socks  
Socks是一种网络代理协议，该协议可以让客户机通过Socks服务器获得对Internet的完全访问能力。Scoks在服务器和客户端之间建立一个安全的代理数据通道，从客户的角度看来，Scoks是透明的；从服务器的角度看来，Socks就是客户端。客户端不需要具有对Internet的直接访问能力(也就是说，可以使用私有IP地址），因为Socks服务器能够把来自于客户端的连接请求重定向到Internet。此外，Socks服务器可以对用户连接请求进行认证，允许合法用户建立代理连接。同理，Socks也能防止非授权的Internet用户访问及的内部网络。所以常常把Socks当作防火墙来使用。  
常见的浏览器如netscape、IE等可以直接使用Socks, 并且我们也可以使用socsk5的所带的client来使那些不直接支持socks的internet软件使用Socks。  
更多的资料可以参考Socks官方站点http://www.socks.nec.com。  

2.3 Squid  
对于web用户来说，Squid是一个高性能的代理缓存服务器，Squid支持FTP、gopher和HTTP协议。和一般的代理缓存软件不同，Squid用一个单独的、非模块化的、I/O驱动的进程来处理所有的客户端请求。  
Squid将数据元缓存在内存中，同时也缓存DNS查询的结果，除此之外，它还支持非模块化的DNS查询，对失败的请求进行消极缓存。Squid支持SSL，支持访问控制。由于使用了ICP（轻量Internet缓存协议），Squid能够实现层叠的代理阵列，从而最大限度地节约带宽。  
Squid由一个主要的服务程序squid,一个DNS查询程序dnsserver，几个重写请求和执行认证的程序，以及几个管理工具组成。当Squid启动以后，它可以派生出预先指定数目的dnsserver进程，而每一个dnsserver进程都可以执行单独的DNS查询，这样一来就大大减少了服务器等待DNS查询的时间。  

2.4 选择  
从上面的比较可以看出，Apache主要功能是web服务器，代理功能只不过是其一个模块而已，Socks虽然强大，但有欠灵活，因此我们着重推荐你使用Squid。下面的章节我们就一起来学习Squid激动人心的特性及相关的安装与配置。 

三、安装Squid Proxy Server  

3.1获取软件  
你可以通过以下途径获取该软件：  
1.从Squid的官方站点http://www.squid-cache.org下载该软件；  
2.从你的linux发行版本中获取该软件；  
通常，Squid软件包有两种：一种是源代码，下载后需要自己重新编译；可执行文件，下载后只需解压就可以使用；另一种是就是RedHat所使用的rpm包。下面我们分别讲讲这两种软件包的安装方法。  

3.2安装软件  
我们以目前最新的稳定版本squid-2.3.STABLEX为例。  

3.2.1rpm包的安装  
1.进入/mnt/cdrom/RedHat/RPMS  
2.执行rpm -ivh squid-2.2.STABLE4-8.i386.rpm。  
当然，我们也可以在开始安装系统的过程中安装该软件。  

3.2.2 源代码包的安装  
1.从http://www.squid-cache.org下载squid-2.3.STABLE2-src.tar.gz。  
2.将该文件拷贝到/usr/local目录。  
3.解开该文件 tar xvzf squid-2.3.STABLE2-src.tar.gz。  
4.解开后，在/usr/local生成一个新的目录squid-2.3.STABLE2，为了方便用mv命令将 该目录重命名为squid mv squid-2.3.STABLE2 squid;  
5.进入squid cd squid  
6.执行./configure 可以用./confgure --prefix=/directory/you/want指定安装目录  
系统缺省安装目录为/usr/local/squid。  
7.执行 make all  
8.执行 make install  
9.安装结束后，squid的可执行文件在安装目录的bin子目录下，配置文件在etc子目录下。  

四、配置squid基础篇——让代理服务器跑起来  
由于RedHat各方面的优势（包括易用性，稳定性等等），全世界范围内使用该发行版的用户比较多，所以，我们下面的说明都是以RedHat6.1环境下squid-2.2.STABLE4-8版本为主。从我的使用经验看来，该版本的squid要比其他版本稳定的多，以前的1.1.22版本也比较稳定，但是在功能及灵活性方面有所欠缺。  
squid有一个主要的配置文件squid.conf,在RedHat环境下所有squid的配置文件位于/etc/squid子目录下。  

4.1常用的配置选项  
因为缺省的配置文件有问题，所以我们必须首先修改该配置文件的有关内容,以便让squid跑起来。  
下面我们来看一看squid.conf文件的结构以及一些常用的选项：  
squid.conf配置文件的可以分为十三个部分，这十三个部分分别是：  
1.NETWORK OPTIONS （有关的网络选项）  
2.OPTIONS WHICH AFFECT THE NEIGHBOR SELECTION ALGORITHM （作用于邻居选择算 法的有关选项）  
3.OPTIONS WHICH AFFECT THE CACHE SIZE （定义cache大小的有关选项）  
4.LOGFILE PATHNAMES AND CACHE DIRECTORIES (定义日志文件的路径及cache的目录）  
5.OPTIONS FOR EXTERNAL SUPPORT PROGRAMS （外部支持程序选项）  
6.OPTIONS FOR TUNING THE CACHE （调整cache的选项）  
7.TIMEOUTS （超时）  
8.ACCESS CONTROLS （访问控制）  
9.ADMINISTRATIVE PARAMETERS （管理参数）  
10.OPTIONS FOR THE CACHE REGISTRATION SERVICE （cache注册服务选项）  
11.HTTPD-ACCELERATOR OPTIONS （HTTPD加速选项）  
12.MISCELLANEOUS （杂项）  
13.DELAY POOL PARAMETERS （延时池参数）  
虽然squid的配置文件很庞大，但是如果你只是为一个中小型网络提供代理服务，并且只准备使用一台服务器，那么，你只需要修改配置文件中的几个选项。这些几个常用选项分别是：  
1.http_port  
说明：定义squid监听HTTP客户连接请求的端口。缺省是3128，如果使用HTTPD加速模式 则为80。你可以指定多个端口，但是所有指定的端口都必须在一条命令行上。  
2.cache_mem (bytes)  
说明：该选项用于指定squid可以使用的内存的理想值。这部分内存被用来存储以下对象 ：  
In-Transit objects （传入的对象）  
Hot Objects （热对象，即用户常访问的对象）  
Negative-Cached objects （消极存储的对象）  
需要注意的是，这并没有指明squid所使用的内存一定不能超过该值，其实，该选项只 定义了squid所使用的内存的一个方面，squid还在其他方面使用内存。所以squid实际 使用的内存可能超过该值。缺省值为8MB。  
3.cache_dir Directory-Name Mbytes Level-1 Level2  
说明：指定squid用来存储对象的交换空间的大小及其目录结构。可以用多个cache_dir命令来定义多个这样的交换空间，并且这些交换空间可以分布不同的磁盘分区。"directory "指明了该交换空间的顶级目录。如果你想用整个磁盘来作为交换空间，那么你可以将该目录作为装载点将整个磁盘mount上去。缺省值为/var/spool/squid。“Mbytes”定义了可用的空间总量。需要注意的是，squid进程必须拥有对该目录的读写权力。“Level-1”是可以在该顶级目录下建立的第一级子目录的数目，缺省值为16。同理，“Level-2”是可以建立的第二级子目录的数目，缺省值为256。为什么要定义这么多子目录呢？这是因为如果子目录太少，则存储在一个子目录下的文件数目将大大增加，这也会导致系统寻找某一个文件的时间大大增加，从而使系统的整体性能急剧降低。所以，为了减少每个目录下的文件数量，我们必须增加所使用的目录的数量。如果仅仅使用一级子目录则顶级目录下的子目录数目太大了，所以我们使用两级子目录结构。  
那么，怎么来确定你的系统所需要的子目录数目呢？我们可以用下面的公式来估算。  
已知量：  
DS = 可用交换空间总量（单位KB）/ 交换空间数目  
OS = 平均每个对象的大小= 20k  
NO = 平均每个二级子目录所存储的对象数目 = 256  
未知量：  
L1 = 一级子目录的数量  
L2 = 二级子目录的数量  
计算公式：  
L1 x L2 = DS / OS / NO  
注意这是个不定方程，可以有多个解。  
4.acl  
说明：定义访问控制列表。  
定义语法为：  
acl aclname acltype string1 ...  
acl aclname acltype "file" ...  
当使用文件时，该文件的格式为每行包含一个条目。  
acltype 可以是 src dst srcdomain dstdomain url_pattern urlpath_pattern time port proto method browser user 中的一种。  
分别说明如下：  
src 指明源地址。可以用以下的方法指定：  
acl aclname src ip-address/netmask ... (客户ip地址)  
acl aclname src addr1-addr2/netmask ... (地址范围)  
dst 指明目标地址。语法为：  
acl aclname dst ip-address/netmask ... (即客户请求的服务器的ip地址)  
srcdomain 指明客户所属的域。语法为：  
acl aclname srcdomain foo.com ... squid将根据客户ip反向查询DNS。  
dstdomain 指明请求服务器所属的域。语法为：  
acl aclname dstdomain foo.com ... 由客户请求的URL决定。  
注意，如果用户使用服务器ip而非完整的域名时，squid将进行反向的DNS解析来确 定其完整域名，如果失败就记录为“none”。  
time 指明访问时间。语法如下：  
acl aclname time [day-abbrevs] [h1:m1-h2][hh:mm-hh]  
day-abbrevs:  
S - Sunday  
M - Monday  
T - Tuesday  
W - Wednesday  
H - Thursday  
F - Friday  
A - Saturday  
h1:m1 必须小于 h2:m2，表达示为[hh:mm-hh]。  
port 指定访问端口。可以指定多个端口，比如：  
acl aclname port 80 70 21 ...  
acl aclname port 0-1024 ... （指定一个端口范围）  
proto 指定使用协议。可以指定多个协议：  
acl aclname proto HTTP FTP ...  
method 指定请求方法。比如：  
acl aclname method GET POST ...  
5.http_access  
说明：根据访问控制列表允许或禁止某一类用户访问。  
如果某个访问没有相符合的项目,则缺省为应用最后一条项目的“非”。比如最后一条为允许，则缺省就是禁止。所以，通常应该把最后的条目设为"deny all" 或 "allow all" 来避免安全性隐患。 
4.2 应用实例  
假想情景：某公司用squid作代理服务器，该代理服务器配置为PII450/256M/8.4G,公司所用ip段为1.2.3.0/24,并且想用8080作为代理端口。  
则相应的squid配置选项为：  
1.http_port  
http_port 8080  
2.cache_mem  
思路：由于该服务器只提供代理服务，所以该值可以尽量设得大一些。  
cache_mem 194M  
3.cache_dir Directory-Name Mbytes Level-1 Level2  
思路：硬盘为8.4G的，在安装系统时应该做好规划，为不同的文件系统划分可用空间。在本例中，我们可以这样来划分：  
/cache1 3.5G  
/cache2 3.5G  
/var 400M  
swap 127M  
/ 剩余部分  
并且，在安装时，我们尽量不安装不必要的包。这样在节约空间的同时可以提高系统的安全性和稳定性。下面我们来计算所需的第一级和第二级子目录数。  
已知量：  
DS = 可用交换空间总量（单位KB）/ 交换空间数目＝7G/2=3500000KB  
OS = 平均每个对象的大小= 20k  
NO = 平均每个二级子目录所存储的对象数目 = 256  
未知量：  
L1 = 一级子目录的数量  
L2 = 二级子目录的数量  
计算公式：  
L1 x L2 = DS / OS / NO＝3500000/20/256=684  
我们取  
L1=16  
L2=43  
所以，我们的cache_dir语句为：  
cache_dir /cache1 3500M 16 43  
cache_dir /cache2 3500M 16 43  
4.acl  
思路：通过src来定义acl.  
acl allow_ip src 1.2.3.4/255.255.255.0  
5.http_access  
http_access allow allow_ip  

4.3启动、停止squid。  
配置并保存好squid.conf后，可以用以下命令启动squid。  
squid  
或者，使用RedHat的启动脚本来启动squid.  
/etc/rc.d/init.d/squid start  
同样地，你也可以用下列脚本停止运行squid或重启动squid.  
/etc/rc.d/init.d/squid stop  
/etc/rc.d/init.d/squid restart  

五、根据需求配置你的squid——进阶篇  

5.1其它配置选项  
在进行squid的一些高级应用之前，我们有必要对其他有用的配置选项作一个全面的了解。下面我们分类来讲一讲这些选项，用于某些特殊应用的选项我们将放在讲该种应用时来讲。  

5.1.1网络选项  
1.tcp_incoming_address  
tcp_outgoing_address  
udp_incoming_address  
udp_outgoing_address  
说明：  
tcp_incoming_address指定监听来自客户或其他squid代理服务器的绑定ip地址；  
tcp_outgoing_address指定向远程服务器或其他squid代理服务器发起连接的ip地址  
udp_incoming_address为ICP套接字指定接收来自其他squid代理服务器的包的ip地址 udp_outgoing_address为ICP套接字指定向其他squid代理服务器发送包的ip地址；  
缺省为没有绑定任何ip地址。该绑定地址可以用ip指定，也可以用完整的域名指定。  

5.1.2交换空间设定选项  
1.cache_swap_low (percent, 0-100)  
cache_swap_high (percent, 0-100)  
说明：squid使用大量的交换空间来存储对象。那么，过了一定的时间以后，该交换空间就会用完，所以还必须定期的按照某种指标来将低于某个水平线的对象清除。squid使用所谓的“最近最少使用算法”（LRU）来做这一工作。当已使用的交换空间达到cache_swap_high时，squid就根据LRU所计算的得到每个对象的值将低于某个水平线的对象清除。这种清除工作一直进行直到已用空间达到cache_swap_low。这两个值用百分比表示，如果你所使用的交换空间很大的话，建议你减少这两个值得差距，因为这时一个百分点就可能是几百兆空间，这势必影响squid的性能。缺省为：  
cache_swap_low 90  
cache_swap_high 95  
2.maximum_object_size  
说明：大于该值得对象将不被存储。如果你想要提高访问速度，就请降低该值；如果你想最大限度地节约带宽，降低成本，请增加该值。单位为K，缺省值为：  
maximum_object_size 4096 KB  

5.1.3有关日志的选项  
1.cache_access_log  
说明：指定客户请求记录日志的完整路径（包括文件的名称及所在的目录），该请求可以是来自一般用户的HTTP请求或来自邻居的ICP请求。缺省值为：  
cache_access_log /var/log/squid/access.log  
如果你不需要该日志，可以用以下语句取消：cache_access_log none  
2.cache_store_log  
说明：指定对象存储记录日志的完整路径（包括文件的名称及所在的目录）。该记录表明哪些对象被写到交换空间，哪些对象被从交换空间清除。缺省路径为：  
cache_log /var/log/squid/cache.log  
如果你不需要该日志，可以用以下语句取消：cache_store_log none  
3.cache_log  
说明：指定squid一般信息日志的完整路径（包括文件的名称及所在的目录）。  
缺省路径为：cache_log /var/log/squid/cache.log  
4.cache_swap_log  
说明：该选项指明每个交换空间的“swap.log”日志的完整路径（包括文件的名称及所在的目录）。该日志文件包含了存储在交换空间里的对象的元数据（metadata）。通常，系统将该文件自动保存在第一个“cache_dir”说定义的顶级目录里，但是你也可以指定其他的路径。如果你定义了多个“cache_dir”，则相应的日志文件可能是这样的：  
cache_swap_log.00  
cache_swap_log.01  
cache_swap_log.02  
后面的数字扩展名与指定的多个“cache_dir”一一对应。  
需要注意的是，最好不要删除这类日志文件，否则squid将不能正常工作。  
5.pid_filename  
说明：指定记录squid进程号的日志的完整路径（包括文件的名称及所在的目录）。缺省路径为  
pid_filename /var/run/squid.pid  
如果你不需要该文件，可以用以下语句取消：pid_filename none  
6.debug_options  
说明：控制作日志时记录信息的多寡。可以从两个方面控制：section控制从几个方面作记录；level控制每个方面的记录的详细程度。推荐的方式（也是缺省方式）是：debug_options ALL,1  
即，对每个方面都作记录，但详细程度为1(最低)。  
7.log_fqdn on|off  
说明：控制在 access.log 中对用户地址的记录方式。打开该选项时，squid记录客户的完整域名，取消该选项时，squid记录客户的ip地址。注意，如果打开该选项会增加系统的负担，因为squid还得进行客户ip的DNS查询。缺省值为：log_fqdn off  

5.1.4有关外部支持程序的选项  
1.ftp_user  
说明：设置登录匿名ftp服务器时的提供的电子邮件地址，登录匿名ftp服务器时要求用你的电子邮件地址作为登录口令（更多的信息请参看本书的相关章节）。需要注意的是，有的匿名ftp服务器对这一点要求很苛刻，有的甚至会检查你的电子邮件的有效性。缺省值为：ftp_user Squid@  
2.ftp_list_width  
说明：设置ftp列表的宽度，如果设得太小将不能的浏览到长文件名。缺省值为： ftp_list_width 32  
3.cache_dns_program  
说明：指定DNS查询程序的完整路径（包括文件的名称及所在的目录）。缺省路径为：  
cache_dns_program /usr/lib/squid/dnsserver  
4.dns_children  
说明：设置DNS查询程序的进程数。对于大型的登录服务器系统，建议该值至少为10。最大值可以是32，缺省设置为5个。注意，如果你任意的降低该值，可能会使系统性能急剧降低，因为squid主进程要等待域名查询的结果。没有必要减少该值，因为DNS查询进程并不会消耗太多的系统的资源。  
5.dns_nameservers  
说明：指定一个DNS服务器列表，强制squid使用该列表中的DNS服务器而非使用/etc/resolv.conf文件中定义的DNS服务器。你可以这样指定多个DNS服务器：dns_nameservers 10.0.0.1 192.172.0.4  
缺省设置为：dns_nameservers none  
6.unlinkd_program  
说明：指定文件删除进程的完整路径。  
缺省设置为：  
unlinkd_program /usr/lib/squid/unlinkd  
7.pinger_program  
说明：指定ping进程的完整路径。该进程被squid利用来测量与其他邻居的路由距离。该选项只在你启用了该功能时有用。缺省为：  
pinger_program /usr/lib/squid/pinger  
8.authenticate_program  
说明：指定用来进行用户认证的外部程序的完整路径。squid的用户认证功能我们将在后面的章节讲述。缺省设置为不认证。  

5.1.5用户访问控制选项  
1.request_size (KB)  
说明：设置用户请求通讯量的最大允许值(单位为KB)。如果用户用POST方法请求时，应该设一个较大的值。缺省设置为：  
request_size 100 KB  
2.reference_age  
说明：squid根据对象的LRU（最近最少使用算法）来清除对象，squid依据使用磁盘空间的总量动态地计算对象的LRU年龄。我们用reference_age定义对象的最大LRU年龄。如果一个对象在指定的reference_age内没有被访问，squid将删除该对象。缺省值为一个月。你可以使用如下所示的时间表示方法。  
1 week  
3.5 days  
4 months  
2.2 hours  
3.quick_abort_min (KB)  
quick_abort_max (KB)  
quick_abort_pct (percent)  
说明：控制squid是否继续传输被用户中断的请求。当用户中断请求时，squid将检测  
quick_abort 的值。如果剩余部分小于“quick_abort_min”指定的值,squid 将继续完成剩余部分的传输；如果剩余部分大于“quick_abort_max”指定的值,squid 将终止剩余部分的传输；如果已完成“quick_abort_pct”指定的百分比，squid将继续完成剩余部分的传输。缺省的设置为：  
quick_abort_min 16 KB  
quick_abort_max 16 KB  
quick_abort_pct 95  

5.1.6各类超时设置选项  
1.negative_ttl time-units  
说明：设置消极存储对象的生存时间。所谓的消极存储对象，就是诸如“连接失败”及"404 Not Found"等一类错误信息。缺省设置为：negative_ttl 5 minutes  
2.positive_dns_ttl time-units  
说明：设置缓存成功的DNS查询结果的生存时间。缺省为6小时。  
positive_dns_ttl 6 hours  
3.negative_dns_ttl time-units  
说明：设置缓存失败的DNS查询结果的生存时间。缺省为5分钟。  
negative_dns_ttl 5 minutes  
4.connect_timeout time-units  
说明：设置squid等待连接完成的超时值。缺省值为2分钟。  
connect_timeout 120 seconds  
5.read_timeout time-units  
说明：如果在指定的时间内squid尚未从被请求的服务器读入任何数据，则squid将终止该客户请求。缺省值为15分钟。  
read_timeout 15 minutes  
6.request_timeout  
说明：设置在建立与客户的连接后，squid将花多长时间等待客户发出HTTP请求。缺省值为30秒。  
request_timeout 30 seconds  
7.client_lifetime time-units  
说明：设置客户在与squid建立连接后，可以将该连接保持多长时间。  
注意，因为客户建立的每个连接都会消耗一定的系统资源，所以如果你是为一个大型网络提供代理服务的话，一定要正确地修改该值。因为如果同一时间的连接数量太大的话，可能会消耗大量的系统资源，从而导致服务器宕机。缺省值为1天，该值太大了，建议根据你自己的情况适当减小该值。  
client_lifetime 1 day  
8.half_closed_clients on/off  
说明：有时候由于用户的不正常操作，可能会使与squid的TCP连接处于半关闭状态，  
这时候，该TCP连接的发送端已经关闭，而接收端正常工作。缺省地，squid将一直保持这种处于半关闭状态的TCP连接，直到返回套接字的读写错误才将其关闭。如果将该值设为off，则一旦从客户端返回“no more data to read”的信息，squid就立即关闭该连接。half_closed_clients on  
9.pconn_timeout  
说明：设置squid在与其他服务器和代理建立连接后，该连接闲置多长时间后被关闭。缺省值为120秒。  
pconn_timeout 120 seconds  
10.ident_timeout  
说明：设置squid等待用户认证请求的时间。缺省值为10秒。  
ident_timeout 10 seconds  
11.shutdown_lifetime time-units  
说明：当收到SIGTERM 或者 SIGHUP 信号后, squid将进入一种shutdown pending的模式，等待所有活动的套接字关闭。在过了shutdown_lifetime所定义的时间后，所有活动的用户都将收到一个超时信息。缺省值为30秒。  
shutdown_lifetime 30 seconds  

5.1.7管理参数选项  
1.cache_mgr  
说明:设置管理员邮件地址。缺省为：  
cache_mgr root  
2. cache_effective_user  
cache_effective_group  
说明：如果用root启动squid，squid将变成这两条语句指定的用户和用户组。缺省变为squid用户和squid用户组。注意这里指定的用户和用户组必须真是存在于/etc/passwd中。如果用非root帐号启动squid，则squid将保持改用户及用户组运行，这时候，你不能指定小于1024地http_port。  
cache_effective_user squid  
cache_effective_group squid  
3.visible_hostname  
说明：定义在返回给用户的出错信息中的主机名。  
如: visible_hostname www-cache.foo.org  
4.unique_hostname  
说明：如果你有一个代理服务器阵列，并且你为每个代理服务器指定了同样的“visible_hostname”，同时你必须为它们指定不同的“unique_hostname”来避免“forwarding loops ”（传输循环）发生。  

5.1.8其它杂项  
1. dns_testnames  
说明：设置进行DNS查询测试,如果第一个站点解析成功则立即结束DNS查询测试。如果你不愿意进行DNS查询测试，就不要去掉缺省的设置。  
#dns_testnames netscape.com internic.net nlanr.net microsoft.com  
2.logfile_rotate  
说明：通常，squid会定期的将日志文件更名并打包。比如正在使用的日志文件为access.log,squid会将其更名并打包为access.log.1.gz；过了一定时间后，squid又会将  
access.log.1.gz更名为access.log.2.gz并将当前的日志文件更名并打包为access.log.1.gz，以此循环。logfile_rotate所指定的数字即为打包并备份的文件的数量，当达到这一数目时，squid将删除最老的备份文件。缺省值为10。如果你想手动来进行这些操作，你可以用logfile_rotate 0来取消自动操作。  
3.err_html_text  
说明：用该语句定义一个字符串变量，可以用％L在返回给用户的错误信息文件中引用。错误信息文件通常在/etc/squid/errors目录中，这是一些用HTML写成的脚本文件，你可以自己修改它。  
4.deny_info  
说明：你可以定制自定义的拒绝访问信息文件，并且可以和不同的用户列表相关联。当用户被http_access相关规则拒绝时，squid可以向用户显示你自定义的相应的拒绝访问信息文件。语法为：  
Usage: deny_info err_page_name acl  
比如：  
deny_info ERR_CUSTOM_ACCESS_DENIED bad_guys  
5.memory_pools on|off  
说明:如果你将该项设为on，则squid将保留所有已经分配（但是未使用）的内存池以便在将来使用。缺省为on.  
memory_pools on  
6.log_icp_queries on|off  
说明：设置是否对ICP请求作日志。如果你的系统负载很大，你可以用off来取消该功能。缺省为:  
log_icp_queries on  
7.always_direct  
说明：该选项允许你指定某些用户类，squid将这些用户类的请求直接转发给被请求的服务器。语法为:  
always_direct allow|deny [!]aclname ...  
如：直接转发FTP请求可以这样设置：  
acl FTP proto FTP  
always_direct allow FTP  
8.never_direct  
说明：与always_direct相反。语法为：  
Usage: never_direct allow|deny [!]aclname ...  
比如，为了强制除了本地域的其他用户使用代理服务器，你可以这样设置：  
acl local-servers dstdomain foo.net  
acl all src 0.0.0.0/0.0.0.0  
never_direct deny local-servers  
never_direct allow all  
9.icon_directory  
说明：指明向用户传送错误信息时所用到的图标文件的目录。缺省路径为： icon_directory /usr/lib/squid/icons  
10.error_directory  
说明：指明向用户传送错误信息所用到的错误描述文件的目录。缺省路径为：  
error_directory /etc/squid/errors  

5.2 用户认证设置  
缺省的，squid本身不带任何认证程序，但是我们可以通过外部认证程序来实现用户认证。一般说来有以下的认证程序：  
1.LDAP认证：你可以访问以下资源来获取更多的有用信息。  
http://www.geocities.com/ResearchTriangle/Thinktank/5292/projects/ldap/  
http://home.iae.nl/users/devet/squid/proxy_auth/contrib/ldap_auth.tar.gz  
2.SMB认证：可以实现基于NT和samba的用户认证。更多的信息请访问以下资源。  
http://www.hacom.nl/~richard/software/smb_auth.html  
3.基于mysql的用户认证。  
http://home.iae.nl/users/devet/squid/proxy_auth/contrib/mysql_auth.c  
4.基于sock5密码用户认证。  
http://nucleo.freeservers.com/  
5.基于Radius 的用户认证。  
http://home.iae.nl/users/devet/squid/proxy_auth/contrib/auth.pl  
但是我们一般常用的是用ncsa实现的认证和用smb_auth实现的基于NT和samba的用户认证。下面我们就来讲这两种认证方法的具体实现。  

5.2.1 ncsa用户认证的实现  
ncsa是squid源代码包自带的认证程序之一，下面我们以squid-2.3.STABLE2版本为例讲述ncsa的安装和配置。  
1.从www.squid-cache.org下载squid源代码包squid-2.3.STABLE2-src.tar.gz并放到/tmp目录下。  
2.用tar解开：  
tar xvzf squid-2.3.STABLE2-src.tar.gz  
%make  
%make install  
3.然后，进入/tmp/squid-2.3.STABLE2/auth_modules/NCSA目录。  
% make  
% make install  
编译成功后，会生成ncsa_auth的可执行文件。  
4.拷贝生成的执行文件ncsa_auth到/usr/bin目录  
cp ncsa_auth /usr/bin/bin  
5.修改squid.conf中的相关选项如下所示：  
authenticate_program /usr/local/squid/bin/ncsa_auth /usr/bin/passwd  
6.定义相关的用户类  
acl auth_user proxy_auth REQUIRED  
注意，REQUIRED关键字指明了接收所有合法用户的访问。  
7.设置http_access  
http_access allow auth_user  
注意，如果你在改行中指定了多个允许访问的用户类的话，应该把要认证的用户类放在第一个。如下所示：  
错误的配置：http_access allow auth_user all manager  
正确的配置：http_access allow auth_user manager all  
8.利用apache携带的工具软件htpasswd在/usr/local/squid/etc下生成密码文件并添加相应的用户信息。一般说来，该密码文件每行包含一个用户的用户信息，即用户名和密码。  
用htpasswd生成密码文件passwd并添加用户bye。  
htpasswd -c /usr/local/squid/etc/passwd bye  
然后重新启动squid，密码认证已经生效。  

5.2.2 smb用户认证的实现  
国内介绍并使用ncsa实现用户认证的文章不多，而使用smb_auth和samba实现基于NT的用户认证我还没有看到过，下面我们就来看一看在squid中实现基于NT的用户认证。  
当前smb_auth的最高版本是smb_auth-0.05，你可以在以下地址下载。当然，squid的源代码包中也包含smb_auth,但是是0.02版的。  
http://www.hacom.nl/~richard/software/smb_auth-0.05.tar.gz  
smb_auth的主页地址是http://www.hacom.nl/~richard/software/smb_auth.html。  
1.系统需求：  
squid2.0以上版本。  
安装samba2.0.4以上版本。你并不需要运行samba服务，因为smb_auth只用到了 samba的客户端软件。  
2.下载smb_auth-0.05.tar.gz并复制到/tmp.  
3.tar xvzf smb_auth-0.05.tar.gz  
4.根据你的要求修改Makefile中的SAMBAPREFIX和INSTALLBIN参数。SAMBAPREFIX指定了你的samba安装路径，INSTALLBIN指明了smb_auth的安装路径。我们指定：  
SAMBAPREFIX=/usr,INSTALLBIN=/usr/bin.  
5.make  
6.make install,成功后会在INSTALLBIN指定路径中生成可执行文件smb_auth.  
7.按下列步骤设置你要用于认证的主域控制器：  
首先在NETLOG共享目录中建立一个“proxy”文件，该文件只包含一个“allow”的字符串，一般说来，该NETLOG目录位于winntsystem32Replimportscripts目录中；然后，设置所有你想让其访问squid的用户和用户组拥有对该文件的读的权力。  
8.修改squid.conf中的相关选项如下所示：  
authenticate_program /usr/local/squid/bin/smb_auth your_domain_name  
9.定义相关的用户类  
acl auth_user proxy_auth REQUIRED  
注意，REQUIRED关键字指明了接收所有合法用户的访问。  
10.设置http_access  
http_access allow auth_user  
注意，如果你在改行中指定了多个允许访问的用户类的话，应该把要认证的用户类放在第一个。如下所示：  
错误的配置：http_access allow auth_user all manager  
正确的配置：http_access allow auth_user manager all  
如果一切正确的话，然后重新启动squid，密码认证已经生效。  
说明：smb_auth的调用方法：  
1.smb_auth -W your_domain_name  
用your_domain_name指定你的域名。smb_auth将进行广播寻找该主域控制器。  
2.smb_auth -W your_domain_name -B  
如果你有多个网络接口，可以用-B 指定用于广播的网络接口的ip地址。  
3.smb_auth -W your_domain_name -U  
也可以用-U直接指定该主域控制器的ip地址。  
4.smb_auth -W your_domain_name -S share  
可以用-S指定一个不同于NETLOG的共享目录。  

5.2.3 squid.conf中关于认证的其他设置  
1.authenticate_children  
说明：设置认证子进程的数目。缺省为5个。如果你处于一个繁忙的网络环境中，你可以适当增大该值。  
2.authenticate_ttl  
说明：设置一次认证的有效期，缺省是3600秒。  
3.proxy_auth_realm  
说明：设置用户登录认证时向用户显示的域名。  

5.3透明代理的设置  
关于透明代理的概念我们已经在第一节将过了，下面我们看一下怎么样在squid中实现透明代理。  
透明代理的实现需要在Linux 2.0.29以上，但是Linux 2.0.30并不支持该功能，好在我们现在使用的通常是2.2.X以上的版本，所以不必担心这个问题。下面我们就用ipchains+squid来实现透明代理。在开始之前需要说明的是,目前我们只能实现支持HTTP的透明代理，但是也不必太担心，因为我们之所以使用代理，目的是利用squid的缓存来提高Web的访问速度，至于提供内部非法ip地址的访问及提高网络安全性，我们可以用ipchains来解决。  
实现环境：RedHat6.x+squid2.2.x+ipchains  

5.3.1 linux的相关配置  
确定你的内核已经配置了以下特性：  
[*] Network firewalls  
[ ] Socket Filtering  
[*] Unix domain sockets  
[*] TCP/IP networking  
[ ] IP: multicasting  
[ ] IP: advanced router  
[ ] IP: kernel level autoconfiguration  
[*] IP: firewalling  
[ ] IP: firewall packet netlink device  
[*] IP: always defragment (required for masquerading)  
[*] IP: transparent proxy support  
如果没有，请你重新编译内核。一般在RedHat6.x以上，系统已经缺省配置了这些特性。  

5.3.2squid的相关配置选项  
设置squid.conf中的相关选项，如下所示：  
http_port 3218  
httpd_accel_host virtual  
httpd_accel_port 80  
httpd_accel_with_proxy on  
httpd_accel_uses_host_header on  
说明：  
1.http_port 3128  
在本例中，我们假设squid的HTTP监听端口为3128,即squid缺省设置值。然后，把所有来自于客户端web请求的包（即目标端口为80)重定向到3128端口。  
2.httpd_accel_host virtual  
httpd_accel_port 80  
这两个选项本来是用来定义squid加速模式的。在这里我们用virtual来指定为虚拟主机模式。80端口为要加速的请求端口。采用这种模式时，squid就取消了缓存及ICP功能，假如你需要这些功能，这必须设置httpd_accel_with_proxy选项。  
3.httpd_accel_with_proxy on  
该选项在透明代理模式下是必须设置成on的。在该模式下，squid既是web请求的加速器，又是缓存代理服务器。  
4.httpd_accel_uses_host_header on  
在透明代理模式下，如果你想让你代理服务器的缓存功能正确工作的话，你必须将该选项设为on。设为on时，squid会把存储的对象加上主机名而不是ip地址作为索引。这一点在你想建立代理服务器阵列时显得尤为重要。  

5.3.3 ipchains的相关配置  
ipchains在这里所起的作用是端口重定向。我们可以使用下列语句实现将目标端口为80端口的TCP包重定向到3128端口。  
#接收所有的回送包  
/sbin/ipchains -A input -j ACCEPT -i lo  
#将目标端口为80端口的TCP包重定向到3128端口  
/sbin/ipchains -A input -p tcp -d 0.0.0.0/0 80 -j REDIRECT 80  
当然在这以前，我们必须用下面的语句打开包转发功能。  
echo 1 >; /proc/sys/net/ipv4/ip_forward  

小节  
开始，我们讨论了代理服务器的概念，代理服务器的分类；然后，我们把注意力集中在squid，讲述了如何安装和配置squid；最后我们讲了一些squid配置中的高级话题，即实现用户认证的两种方法，透明代理的实现等。当然，还有一些高级话题本章没有讲到，如代理阵列的实现，加速模式的运用等等。但是，我们不可能把所有东西都讲完讲全，希望读者能举一反三，自己去摸索，去尝试。

备注：

1、源代码：[http://www.oschina.net/p/squid](http://www.oschina.net/p/squid)

2、《Squid中文权威指南》在线版：[http://blog.s135.com/book/squid/index.html](http://blog.s135.com/book/squid/index.html)

