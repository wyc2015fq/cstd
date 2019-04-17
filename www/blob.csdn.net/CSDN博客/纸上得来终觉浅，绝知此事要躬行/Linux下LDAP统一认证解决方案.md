# Linux下LDAP统一认证解决方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月04日 11:31:45[boonya](https://me.csdn.net/boonya)阅读数：2789标签：[linux																[ldap](https://so.csdn.net/so/search/s.do?q=ldap&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/boonya/article/category/1740759)









From：[http://www.cangfengzhe.com/wangluoanquan/3.html](http://www.cangfengzhe.com/wangluoanquan/3.html)


企业内部需要认证的服务很多，员工需要记住很多的密码， 即使对这些服务进行相同的密码设置，也存在很大的安全隐患。笔者目前工作的企业就是如此，每一个新员工的到来管理员都要初始化很多密码，而这些密码都被设置成了“888888”等弱密码，由于各种软件的认证机制之间没有使用一个统一的标准，员工无法一次性修改所有服务的密码，这导致很多即使是入职很久的员工都还在使用这个“众所周知”的密码。

另外—个比较严重的问题出现在公司增加内部服务的时候，例如领导要在公司内部提供邮件服务或把现有的Proftpd 换成更高效的Vsftpd，管理员需要重新为所有的员工初始化新的账户信息，对于一个有上千员工的企业来说这将是一个“灾难”。

如果可以为各种软件提供一个标准的认证机制，所有软件就可以不再用独有的用户管理方法， 而是通过这种统一的认证机制进行用户认证，这样就解决了目前很多企业遇到的问题。LDAP正是这样一种标准的协议，LDAP的历史可以追溯到1988年，之后诞生的很多软件基本上都支持这个协议。近年随着企业对LDAP需求的不断增加，绝大多数有认证机制的软件都会首先提供对LDAP的支持。本文将介绍通过LDAP统一身份认证的方法，以简化这种复杂的管理过程。

**LDAP概述**

LDAP是Light weight Directory Access Protocol（轻量级目录访问协议）的缩写，其前身是更为古老的DAP协议。它是基于X.500标准的，但是很简单，并且可以根据需要定制。与X.500不同，LDAP支持TCP/IP，这对访问Internet是必需的。LDAP的核心规范在RFC中都有定义， 大体上讲LDAP协议定义了和后台数据库通信的方法及客户端软件和LDAP协议之间的通信标准，如图1所示（更详细的说明可以参见相关的RFC文档）。

![客户端、LDAP Server的关系](http://www.cangfengzhe.com/uploads/allimg/110412/1AI33628-0.jpg)


图1  客户端、LDAP Server的关系

在图1中，LDAP Client指各种需要身份认证的软件，例如Apache、Proftpd和Samba等。LDAP Sever指的是实现LDAP协议的软件，例如OpenLDAP等。Datastorage指的是OpenLDAP的数据存储，如关系型数据库（MySQL）或查询效率更高的嵌入式数据库（BerkeleyDB），甚至是平面文本数据库（—个txt的文本文件）。可见，OpenLDAP软件只是LDAP协议的一种实现形式，并不包括后台数据库存储。但在很多时候管理员经常将LDAP Server和DataStorage放在同一台服务器，这样就产生了人们通常所说的“LDAP数据库”。虽然后台数据库（backend）可以是多种多样，但LDAP协议还规定了数据的存储方式。LDAP数据库是树状结构的，与DNS类似，如图2所示。

![跨国公司员工信息的树状逻辑结构](http://www.cangfengzhe.com/uploads/allimg/110412/1AI311Q-1.jpg)




图2 跨国公司员工信息的树状逻辑结构

在图中，以这种方式存储数据最大的一个好处就是查询速度快，LDAP数据库专门对读操作进行了优化， OpenLDAP配合Berkeley DB可使其读操作的效率得到很大提高。LDAP数据库的树状结构的另一个好处是便于分布式的管理，有关这方面的内容将在后面有所介绍。

**实现思路**

统一身份认证主要是改变原有的认证策略，使需要认证的软件都通过LDAP进行认证，如图3所示。在统一身份认证之后，用户的所有信息都存储在LDAP Server中。终端用户在需要使用公司内部服务的时候，都需要通过LDAP服务器的认证。每个员工只需要记住一个密码，在需要修改用户信息的时候可以通过管理员提供的Web界面直接修改LDAP Server中的信息。

![修改后的认证策略](http://www.cangfengzhe.com/uploads/allimg/110412/1AI3A34-2.jpg)




图3修改后的认证策略

目前大部分主流软件都对LDAP提供很好地支持，但由于各种软件对LDAP的支持程度不同，在做实施的时候也要区别对待。软件对LDPA的支持可以大概分为两大类：

一类是完全支持，也就是在软件的配置文件中加入和LDAP有关的选项就可以完成。这种方式的好处是不需要借助其他的工具或软件，由软件的开发团队直接完成对LDAP的支持。可能的缺陷也在此，由于各个软件开发团队的水平和开发者经验有所差异，虽然同样是支持了LDAP，但稍微复杂一些的高级功能就无法提供，有的时候甚至出现运行错误导致整个软件出现问题。笔者曾遇到过一个基于Web的CVS浏览软件，安装文档中说明了支持LDAP，但实际使用当中遇到了很多问题导致整个软件crash。一些比较主流的软件，例如Apache 2.X对LDAP的支持已经近乎完美。

另一类软件由于很多原因并不直接支持LDAP，而是通过PAM 做身份认证，由于PAM本身支持LDAP，这样也同样可以实现使用LDAP做身份认证。这种方式对LDAP的支持同样有其优势，软件开发人员不需要修改代码支持LDAP协议，减少了软件本身产生安全漏洞的可能。缺点是在一些不希望使用PAM 的系统中，这样的软件就无法通过LDAP进行用户认证，而且虽然PAM对LDAP的支持已经很完善，但一些高级、复杂的功能还是无法实现。正由于这些原因，这类软件为数不多，比较典型的就是Vsftpd和CVS。

**使用LDAP做身份认证**

介绍到这里可能很多读者会问为什么需要使用LDAP数据库？用传统的关系型数据库不可以吗？从上文可以看到，LDAP服务器就是起到了一个认证Server的作用，从技术本身而言，这个认证Server具体使用的是何种数据库并不重要，如果使用一个关系型数据库也可以达到统一身份认证的目的，但LDAP 自身的优势使得很多公司最终选择它。以下笔者列举了一些选择LDAP的重要原因：

◆LDAP是一个开放的标准协议，不同于SQL数据库，LDAP的客户端是跨平台的，并且对几乎所有的程序语言都有标准的API接口。即使是改变了LDAP数据库产品的提供厂商，开发人员也不用担心需要修改程序才能适应新的数据库产品。这个优势是使用SQL语言进行查询的关系型数据库难以达到的。

◆由于LDAP数据库的数据存储是树结构，整棵树的任何一个分支都可以单独放在一个服务器中进行分布式管理，不仅有利于做服务器的负载均衡，还方便了跨地域的服务器部署。这个优势在查询负载大或企业在不同地域都设有分公司的时候体现尤为明显。

◆LDAP支持强认证方式，可以达到很高的安全级别。在国际化方面，LDAP使用了UTF-8编码来存储各种语言的字符。

◆更灵活添加数据类型，LDAP是根据schema的内容定义各种属性之间的从属关系及匹配模式的。例如在关系型数据库中如果要为用户增加一个属性，就要在用户表中增加一个字段，在拥有庞大数量用户的情况下是十分困难的，需要改变表结构。但LDAP只需要在schema中加入新的属性，不会由于用户的属性增多而影响查询性能。

◆LDAP数据库是对读操作进行优化的一种数据库，在读写比例大于7比1的情况下，LDAP会体现出极高的性能。这个特性正适合了身份认证的需要。

目前，很多公司都把LDAP和自己的产品、技术结合在一起，增加LDAP在各个领域中的有效性，这一切都来源于LDAP是一个开放的协议，很容易和其他标准协议共存。

**LDAP软件的选择**

目前，几乎所有大IT厂商都有自己商用的LDAP产品，每个厂商的LDAP产品都有其特点，很多时候用户更多地选择开源的OPenLdap。

OpenLDAP与其他的商用软件相比有很多优势， 商用的软件版本更新很慢，对Bug的反应速度比开源软件差许多，OpenLDAP还包含了很多有创造性的新功能，能满足大多数使用者的要求。笔者曾使用过许多商用LDAP产品，OpenLDAP是其中最轻便且消耗系统资源最少的一个。OpenLDAP是开源软件，近年国内很多公司开发的LDAP产品都是基于OpenLDAP开发的。

OpenLDAP的源代码可以到官方网站（[http://www.openldap.org](http://www.openldap.org/)）下载，安装过程也很简单。由于openldap需要Berkeley DB来存放数据，所以需先安装Berkeley DB 4.4.20，可到它的网站下载，网址见上。

# tar -zxvf db-4.4.20.tar.gz

解完压后，会生成一个db-4.4.20目录，进入该目录下的build_unix目录。执行以下命令进行配置安装。


# ../dist/configure

# make

# make install

# tar -zxvf openldap-stable-20090411.tgz

解压完成后，会生成一个openldap目录。进入该目录，执行以下命令进行配置安装。

#env CPPFLAGS="-I/usr/local/BerkeleyDB.4.2/include" LDFLAGS="-L/usr/local/BerkeleyDB.4.4/lib" ./configure -prefix=/usr/local/openldap --enable-ldbm

注意以上配置语句，要设置资料库的include和lib路径，否则在配置到资料库相关内容时会提示Berkeley DB版本不兼容，并中断配置。如果没有--enable-ldbm选项，在make test时会提示ldbm找不到。为了减少出错，还是加上为好。

#make depens

#make

#make test

在make test阶段要花费较长时间进行测试，好像有16项吧。你可以放松一下，这个时间应该是最紧张的了。

#make install

通过配置命令可以看出，我们把openldap安装到/usr/local/openldap目录下。建议以源码安装的软件都放到独立的目录下，不要放到软件默认的目录。好处是方便管理和控制，所有文件在统一的目录下，卸载软件只要删除整个目录就可以了。在此过程中需要注意如下几个地方：

◆在执行configure之前要确定安装了Berkeley DB，虽然OpenLDAP可以使用很多数据库做back-end，但BerkeleyDB仍然是OpenLDAP开发团队强烈推荐的。

◆如果需要更安全的方式访问OpenLDAP，在配置执行configure之前要确定已经安装了cyrus-sasl。

◆有些用户在安装OpenLDAP的时候忽略了最后的make test。经验告诉我们，很多的错误都会在执行make test的过程中暴露出来，OpenLDAP的开发团队很注意维护软件的Test Case， 整个测试非常全面，很多在日常使用中很少用到的功能都会被测试到，很好的保证了软件在投入使用以后的稳定性。

默认情况下，OpenLDAP的配置文件保存在/usdlocal/etc/openIdap下的sIapd.conf中，配置文件中记录着OpenLDAP的管理员密码。默认情况下密码是用明文表示，可以用slappasswd命令产生密文替换配置文件中的明文。以图3为例，配置文件的相关项应做如下更改：

Suffix  “dc=ldap_abc,dc=org”

Rootdn  “cn=manager,dc=ldap abc,dc=org"

directory  /usr/Iocal/var/openldap-data

其中directory参数为数据文件在服务器上的位置，出于稳定性和性能方面的考虑，数据文件最好放在独立的分区或磁盘阵列上。配置文件的ACL部分也十分重要，很多读取或修改操作不能正常完成都是由于ACL的写法错误造成的。下面是默认情况下ACL部分的配置：

Access to attrs=userPassword

By self write

By * auth

Access to *

By * read

**轻松搞定LDAP账号管理**

在这里笔记告诉大家使用PHP语言开发基于Web的LDAP管理器，并着重分析该管理器中类属性与对象类的继承关系、修改过程的递归关系时所用到的相关技术及相关算法，使得我们在使用和管理LDAP账号等方面变得更为简便。

LDAP Account Manager（LAM）采用PHP 4/5编写，是基于Web的LDAP用户、用户组、主机和LDAP目录的管理系统，管理员可以通过SSUTLS加密方式进行操作，增强了安全性。LAM支持管理的账号类型有Samba 2/3、Unix、Kolab 2、地址簿接口和计算机管理需要的信息，包括NIS映射、Email假名、MAC地址等。

1）LDAP Account Manager的强大功能

◆使用LAM，可以通过Web接口较为直观的、简便的管理存储在LDAP 目录里的用户、用户组和计算机系统账户。

◆可以管理Unix的用户、用户组、主机、域名。

◆具备强大的过滤和排序功能。

◆具有账号属性管理。

◆多构造属性。

◆直观的树状察看模式。

◆计划查看模式。

◆开放式的编辑器。

◆通过文件上传创建账户。

◆所有账号可导出为PDF文件格式。

◆可以管理用户、用户组、配额和自动创建，删除用户的Home 目录。

◆支持LDAP+SSL加密模式。

◆多国语言支持，如Catalan、Chinese(Traditional)、Dutch、English、French、German、Hungarian、Italian、Japanese和Spanish等。

2）安装需求

◆PHP4/5语言环境和Perl语言环境

◆Openldap2.0或更高版本

◆支持CSS的网页浏览器

◆Apache webserver，建议安装SSL、PHP-Module（PHP-Module with ldap，gettext，XML，mcrypt+mhash）等模块。

3）安装和使用

首先从[http://lam.sourceforge.net/](http://lam.sourceforge.net/)下载LAM压缩包，解压缩并修改PHP的配置文件php.ini的如下内容：

* memory_limit=64M

接下来复制文件到Web服务器的html-file目录中，如apache/htdocs，并给下面的文件设置合适的读写权限：

◆-lam/sess：设置给apache用户写权限；

◆-lam/tmp：设置给apache用户写权限；

◆-lam/config：（包括子目录）设置给Apache用户写权限；

◆-lam/lib lamdaemon.pl：必须设置为可执行权限。

然后配置conflg.cfg文件，创建一个配置属性，设置密码，并复制config.cfg_到配置目录。最后，在浏览器中打开index.html，在Configuration Login处可以使用默认密码“lan”登录进行相关操作，如图4所示。

![ldap安装和使用](http://www.cangfengzhe.com/uploads/allimg/110412/1AI3B06-3.jpg)




图4

4）配置管理LAM

成功登录进入LAM 系统后，通过网页的形式对我们的LDAP进行管理就变得十分的直观和简单了，通过鼠标点击就可以轻松查看和管理我们的LDAP，还可以在线编辑和数据导出。在LAM 系统中，我们主要可以进行如下的操作：

◆LAM系统配置LAM的具体配置，如图5所示。

![LAM系统配置LAM的具体配置](http://www.cangfengzhe.com/uploads/allimg/110412/1AI32335-4.jpg)




图5

◆LAM模块选择和管理：让LAM 列出用户信息。

◆修改用户信息，在LAM 中查看和修改用户属性，如图6所示。

![LAM 中查看和修改用户属性](http://www.cangfengzhe.com/uploads/allimg/110412/1AI35439-5.jpg)




图6

不仅如此，LAM的管理功能十分全面， 还可以对用户进行很多操作，如列出用户组信息、列出所管理的服务器主机信息、通过文件上传方式创建用户、树状查看方式、SAMBASAM信息查看修改、开放式在线编辑模式和LDAP信息PDF文件格式输出等，对于系统管理员来说实在是太方便了。

**配置Apache支持LDAP**

虽然各种软件支持LDAP的方法有两种，但本质上是一样的，只是一些软件希望自主开发LDAP的支持，另一些软件使用PAM已经开发好的LDAP支持。任何一个软件要通过LDAP做用户认证，至少要在软件的配置文件中提供两个认证所需要的信息，一个是LDAP服务器的IP地址，另一个是需要认证的用户在LDAP数据库中的位置。前面提到过，LDAP数据库是和DNS类似的树状结构，每个用户作为叶子节点被放置在这棵树上，而这些用户的父节点就是认证所要提供的用户在LDAP数据库中的位置。如图3所示，如果要给亚洲地区市场部的所有员工提供FTP登录账号，需要提供的用户在LDAP数据库中的位置就是“ou=Sales，I=Asia，o=ldap_
 abc.org”。下面以Apache为例介绍一下软件的配置。

Apache和LDAP整合可以用来限制用户以HTTP方式访问文件的权限，Apache使用LDAP做用户认证有很多现实意义，首先可以限制用户对公司内部HTTP敏感内容的访问。此外，很多版本管理软件（如Subversion）都提供了LDAP支持，这样在用户需要通过Web方式访问文件的时候，可以通过Apache的设置来限制用户对文件的访问权限。Apache是从2.0.41以后才开始支持 LDAP的，如果希望使用LDAP做身份认证就要安装Apache 2.0.41以上的版本，企业版本的Red Hat AS 3/4/5默认安装的Apache2.0就已经把L
 DAP相关的模块进行了编译，用户只需正确修改Apache的配置文件就可以支持L DAP认证。

下面代码是对保护目录/var/www/htm/internal设置了一定权限。

<Directory /var/www/html/internal>

AuthName “WeIcome to Linux"

AuthLDAPEnabled on

AuthLDAPURL [ldap://192.168.1.2/ou=Sales,I=Asia,o=ldap_abc.org?uid]()

 Require valid-user

</Directory>

◆AuthName是可选项，用户在访问受保护目录的时候，浏览器会弹出—个提示窗口，要求输入用户名和密码，AuthName设置的内容会出现在弹出窗口的标题栏（此项内容支持中文）。

◆AuthLDAPEnabled是可选项，默认值为on。当已经把目录/var/www/htm/intemal设置为需要认证，但又需要将var/www/htm/internal/pub目录设置为公开时，就可以将此项设置成off。

◆AuthLDAPURL为必填项，192.168.1.2即为LDAP服务器的IP地址，ou=SaIes，I=Asia，o=ldap abc.org为用户在LDAP数据库中的位置，uid表示使用每个用户uid属性的值作为认证过程中使用的用户名。

◆require valid-user表示只有认证成功的用户才能访问指定的资源。LDAP树结构的设计下面用一个实际的案例简单介绍一下树结构的设计。某上百人的IT企业，总公司设在北京，且在上海、广州设有分部，公司的市场部、开发部、管理部及人力资源部在三个城市都有分部，公司要求所有员工每天更新自己在这一天内的工作成果，并为每个员工设有企业内部的FTP共享空间，程序开发人员使用CVS提交代码，公司内部有多台Linux服务器，只允许有权限的账号登录。

员工使用到上述服务的时候需要进行身份认证。在这样的情况下，既可以选择购买几台昂贵的数据库服务器，装上大型的商业数据库来解决这个问题；也可以选择更廉价且高效 的办法，使用开源软件的解决方案。首先要考虑的是数据的拓扑，根据企业的需要做数据库设计是关键。公司在北京、上海和广州设有分公司，如果数据库服务器只存放在一个城市，在网络流量高峰期会影响认证速度，降低员工的工作效率，因此数据库设计可以有以下两种方案：一种是把主LDAP服务器放在北京，同时在上海和广州同时放有LDAP服务器，类似于DNS的区域授权，总公司把上海和广州员工的管理下放，各地的员工通过本地的LDAP服务器进行认证，既提高了效率，又保证了服务器出现问题都不会影响到其他两个城市的认证服务。这种数据库设计并不是把公司的所有员工割裂成了没有联系的三个部分。

如图7所示，三台服务器上的LDAP分支是在同一棵树上通过类似于“引用”的特殊属性连接在一起的，三个城市都有自己的管理员来维护各自的分支。三台服务器连接在一起有很多好处，在需要搜索或查询公司所有员工信息的时候就会十分方便。如果某个城市的员工较多或负载过重还可以在这个城市增加LDAP服务器，增加的服务器就如同本地服务器的镜像，可以起到负载均衡的作用。需要注意的是，在这个设计方案里每个LDAP服务器都是可以读写的。

![树结构的设计](http://www.cangfengzhe.com/uploads/allimg/110412/1AI34321-6.jpg)




图7 树结构的设计

另一种方案是在北京存放一台主LDAP服务器，同时在北京、广州和上海三个城市分别放置一台辅LDAP服务器。主LDAP服务器只负责写入数据，辅LDAP服务器只读，任何数据库的修改都要先写入主LDAP服务器之后再同步到三个辅LDAP 服务器，这样的拓扑结构也比较适合认证服务器的需要，因为写操作相对于读操作更少，这种设计的优势在于能根据员工的多少或各分公司的负载情况灵活增加或减少服务器，任何一台辅LDAP服务器的瘫痪都不会影响到整个认证系统。在国外的一些案例中，辅LDAP服务器会多达几十甚至上百台。但这种设计也有缺陷，
 如果主LDAP出现问题，那么所有的写操作就会受到影响， 目前OpenLDAP还不支持多个主LDAP 服务器的情况，因为设置多个主LDAP服务器有可能会造成整个数据的不一致， 只有少数商用的LDAP 支持多主LDAP的功能。

**统一身份认证的未来**

在国外，LDAP应用于身份认证已经十分成熟，最近几年国内才开始逐渐流行起来。前面提到的认证方式还存在一些不足，例如用户在使用FTP服务后，如果再使用Samba服务就需要再次输入用户名和密码，目前微软的Active Directory通过管理域用户已经完美的实现了单点登录，Linux可以通过OpenLDAP和Samba实现大部分Active Directory能够实现的功能。相信在不久的将来，用LDAP做身份认证的技术还会渗透到更多领域，包括网络计算机、门禁系统，甚至智能IC卡的应用。





