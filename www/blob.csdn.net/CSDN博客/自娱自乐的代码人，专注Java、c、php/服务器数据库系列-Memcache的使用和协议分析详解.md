# 服务器数据库系列 - Memcache的使用和协议分析详解 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月11日 19:19:58[initphp](https://me.csdn.net/initphp)阅读数：1623







*作者：heiyeluren博客：*[*http://blog.csdn.net/heiyeshuwu*](http://blog.csdn.net/heiyeshuwu)
*时间：2006-11-12关键字：PHP Memcache Linux 缓存*


Memcache是danga.com的一个项目，最早是为 LiveJournal 服务的，目前全世界不少人使用这个缓存项目来构建自己大负载的网站，来分担数据库的压力。（关于Memcache的更多信息请Google）

Memcache官方网站：[*http://www.danga.com/memcached*](http://www.danga.com/memcached)


**【安装Memcache服务器端】**

我目前的平台，服务器是Fedora Core 1（内核：2.4.22），客户端是Windows XP SP2，需要安装的就是服务器的Memcached的守护进程和客户端的PHP扩展php_memcache两个东西。现在我分别来讲。

服务器端主要是安装memcache服务器端，目前的最新版本是 memcached-1.2.0 。

下载：[*http://www.danga.com/memcached/dist/memcached-1.2.0.tar.gz*](http://www.danga.com/memcached/dist/memcached-1.2.0.tar.gz)

另外，Memcache用到了libevent这个库用于Socket的处理，所以还需要安装libevent，libevent的最新版本是libevent-1.2。（如果你的系统已经安装了libevent，可以不用安装）

官网：[*http://www.monkey.org/~provos/libevent/*](http://www.monkey.org/~provos/libevent/)

下载：[*http://www.monkey.org/~provos/libevent-1.2.tar.gz*](http://www.monkey.org/~provos/libevent-1.2.tar.gz)

我分别把两个东东下载回来，放到 /tmp 目录下：
# cd /tmp

# wget [http://www.danga.com/memcached/dist/memcached-1.2.0.tar.gz](http://www.danga.com/memcached/dist/memcached-1.2.0.tar.gz)
# wget [http://www.monkey.org/~provos/libevent-1.2.tar.gz](http://www.monkey.org/~provos/libevent-1.2.tar.gz)

先安装libevent：
# tar zxvf libevent-1.2.tar.gz

# cd libevent-1.2

# ./configure --prefix=/usr

# make

# make install

然后看看我们的libevent是否安装成功：
# ls -al /usr/lib | grep libevent
lrwxrwxrwx    1 root     root          21 11?? 12 17:38 libevent-1.2.so.1 -> libevent-1.2.so.1.0.3

-rwxr-xr-x       1 root     root          263546 11?? 12 17:38 libevent-1.2.so.1.0.3

-rw-r--r--        1 root     root          454156 11?? 12 17:38 libevent.a

-rwxr-xr-x       1 root     root          811 11?? 12 17:38 libevent.la

lrwxrwxrwx    1 root     root          21 11?? 12 17:38 libevent.so -> libevent-1.2.so.1.0.3

还不错，都安装上了，再来安装memcache，同时需要安装中指定libevent的安装位置：

# cd /tmp

# tar zxvf memcached-1.2.0.tar.gz

# cd memcached-1.2.0

# ./configure --with-libevent=/usr

# make

# make install

如果中间出现报错，请仔细检查错误信息，按照错误信息来配置或者增加相应的库或者路径。

安装完成后会把memcached放到 /usr/local/bin/memcached ，我们看以下是否安装了：

# ls -al /usr/local/bin/mem*
-rwxr-xr-x    1 root     root       137986 11?? 12 17:39 /usr/local/bin/memcached

-rwxr-xr-x    1 root     root       140179 11?? 12 17:39 /usr/local/bin/memcached-debug

恩，安装完成了，现在我们看以下memcache的帮助：

# /usr/local/bin/memecached -h
memcached 1.2.0

-p <num>            port number to listen on

-s <file>               unix socket path to listen on (disables network support)

-l <ip_addr>        interface to listen on, default is INDRR_ANY

-d                          run as a daemon

-r                           maximize core file limit

-u <username> assume identity of <username> (only when run as root)

-m <num>          max memory to use for items in megabytes, default is 64 MB

-M                         return error on memory exhausted (rather than removing items)

-c <num>            max simultaneous connections, default is 1024

-k                          lock down all paged memory

-v                          verbose (print errors/warnings while in event loop)

-vv                        very verbose (also print client commands/reponses)

-h                         print this help and exit

-i                          print memcached and libevent license

-b                         run a managed instanced (mnemonic: buckets)

-P <file>             save PID in <file>, only used with -d option

-f <factor>          chunk size growth factor, default 1.25

-n <bytes>         minimum space allocated for key+value+flags, default 48

参数不算多，我们来启动一个Memcache的服务器端：
# /usr/local/bin/memcached -d -m 10  -u root -l 192.168.0.200 -p 12000 -c 256 -P /tmp/memcached.pid

-d选项是启动一个守护进程，-m是分配给Memcache使用的内存数量，单位是MB，我这里是10MB，-u是运行Memcache的用户，我这里是root，-l是监听的服务器IP地址，如果有多个地址的话，我这里指定了服务器的IP地址192.168.0.200，-p是设置Memcache监听的端口，我这里设置了12000，最好是1024以上的端口，-c选项是最大运行的并发连接数，默认是1024，我这里设置了256，按照你服务器的负载量来设定，-P是设置保存Memcache的pid文件，我这里是保存在 /tmp/memcached.pid，如果要结束Memcache进程，执行：
# kill `cat /tmp/memcached.pid`


也可以启动多个守护进程，不过端口不能重复。


**【安装Memcache的PHP扩展】**

Memcache就是在服务器监听端口，通过一定的协议交互来写入数据到服务器内存中，或者获取一些值。如果你了解Memcache的交互协议，完全可以自己构建Memcache的客户端，目前网上也有很多构建好的Memcache客户端的PHP Class，可以直接用，不过我这里为了效率，还是决定使用PECL中Memcache的专用扩展，因为毕竟是用C写的，效率比较高，而且安装部署比较方便。

下载PECL中的Memcache，因为我的客户端是Windows XP，所以需要下载dll版，我的PHP版本是PHP 5.1.4，必须下载PHP 5.1专用的扩展。

PECL官网：[*http://pecl.php.net*](http://pecl.php.net)* （For Linux）*
[*http://pecl4win.php.net*](http://pecl4win.php.net)*（For Windows）*

扩展下载： [*http://pecl4win.php.net/download.php/ext/5_1/5.1.2/php_memcache.dll*](http://pecl4win.php.net/download.php/ext/5_1/5.1.2/php_memcache.dll)

如果你的PHP是其他版本，请到 [*http://pecl4win.php.net/ext.php/php_memcache.dll*](http://pecl4win.php.net/ext.php/php_memcache.dll) 选择你相应的版本，如果是Linux下的PHP，请到[*http://pecl.php.net/package/memcache*](http://pecl.php.net/package/memcache) 选择相应想要下载的版本。

下载完了以后，我把php_memcache.dll 拷贝到 c:/php5/ext 目录下，如果你的扩展目录是在是缺省路径，（就是没有修改过php.ini中的扩展路径） 请拷贝到c:/windows/ 目录下，如果是Linux平台，请自己编译安装，可以在程序中使用dl()函数加载，或者在编译php的时候加载进去。最后重启Web服务器，IIS/Apache。

我的网站目录是在：d:/mysite 目录下，现在建立一个 
phpinfo.php 文件在网站根目录下，代码是：
<?phpinfo()?>

看有没有成功加载 php_memcache.dll 扩展。如果显示了 Memcache 选项和相应的版本信息，则说明成功加载了，否则请仔细检查上面的步骤。

如果一切正确无误，那么说明安装成功。



**【Memcache初试】**

**[ 接口介绍 ]**

服务器端和客户端都安装配置好了，现在我们就来测试以下我们的成果。Memcache客户端包含两组接口，一组是面向过程的接口，一组是面向对象的接口，具体可以参考PHP手册 “LXXV. Memcache Functions” 这章。我们为了简单方便，就使用面向对象的方式，也便于维护和编写代码。Memcache面向对象的常用接口包括：

Memcache::connect -- 打开一个到Memcache的连接

Memcache::pconnect -- 打开一个到Memcache的长连接

Memcache::close -- 关闭一个Memcache的连接

Memcache::set -- 保存数据到Memcache服务器上

Memcache::get -- 提取一个保存在Memcache服务器上的数据

Memcache::replace -- 替换一个已经存在Memcache服务器上的项目（功能类似Memcache::set）

Memcache::delete -- 从Memcache服务器上删除一个保存的项目

Memcache::flush -- 刷新所有Memcache服务器上保存的项目（类似于删除所有的保存的项目）

Memcache::getStats -- 获取当前Memcache服务器运行的状态

**[ 测试代码 ]**

现在我们开始一段测试代码：



<?php
//连接
$mem=new Memcache;
$mem->connect("192.168.0.200",12000);

//保存数据
$mem->set('key1','This is first value',0,60);
$val=$mem->get('key1');
echo"Get key1 value: ".$val."<br>";

//替换数据
$mem->replace('key1','This is replace value',0,60);
$val=$mem->get('key1');
echo"Get key1 value: ".$val."<br>";

//保存数组
$arr=array('aaa','bbb','ccc','ddd');
$mem->set('key2',$arr,0,60);
$val2=$mem->get('key2');
echo"Get key2 value: ";
print_r($val2);
echo"<br>";

//删除数据
$mem->delete('key1');
$val=$mem->get('key1');
echo"Get key1 value: ".$val."<br>";

//清除所有数据
$mem->flush();
$val2=$mem->get('key2');
echo"Get key2 value: ";
print_r($val2);
echo"<br>";

//关闭连接
$mem->close();
?>






如果正常的话，浏览器将输出：
Get key1 value: This is first value

Get key1 value: This is replace value

Get key2 value: Array ( [0] => aaa [1] => bbb [2] => ccc [3] => ddd ) 

Get key1 value: 

Get key2 value:



基本说明我们的Memcache安装成功，我们再来分析以下上面的这段程序。


**[ 程序分析 ]**

初始化一个Memcache的对象：
$mem = new 
Memcache;

连接到我们的Memcache服务器端，第一个参数是服务器的IP地址，也可以是主机名，第二个参数是Memcache的开放的端口：
$mem->connect("192.168.0.200",12000);

保存一个数据到Memcache服务器上，第一个参数是数据的key，用来定位一个数据，第二个参数是需要保存的数据内容，这里是一个字符串，第三个参数是一个标记，一般设置为0或者MEMCACHE_COMPRESSED就行了，第四个参数是数据的有效期，就是说数据在这个时间内是有效的，如果过去这个时间，那么会被Memcache服务器端清除掉这个数据，单位是秒，如果设置为0，则是永远有效，我们这里设置了60，就是一分钟有效时间：
$mem->set('key1','This is first value',
0, 60);

从Memcache服务器端获取一条数据，它只有一个参数，就是需要获取数据的key，我们这里是上一步设置的key1，现在获取这个数据后输出输出：
$val = 
$mem->get('key1');

echo "Get key1 value: " . $val;

现在是使用replace方法来替换掉上面key1的值，replace方法的参数跟set是一样的，不过第一个参数key1是必须是要替换数据内容的key，最后输出了：
$mem->replace('key1','This is replace value',
0, 60);
$val = $mem->get('key1');

echo "Get key1 value: " . 
$val;

同样的，Memcache也是可以保存数组的，下面是在Memcache上面保存了一个数组，然后获取回来并输出
$arr = array('aaa','bbb','ccc','ddd');
$mem->set('key2',$arr,
0, 60);
$val2 = $mem->get('key2');

print_r($val2);

现在删除一个数据，使用delte接口，参数就是一个key，然后就能够把Memcache服务器这个key的数据删除，最后输出的时候没有结果
$mem->delete('key1');

$val = $mem->get('key1');

echo "Get key1 value: " . $val ."<br>";

最后我们把所有的保存在Memcache服务器上的数据都清除，会发现数据都没有了，最后输出key2的数据为空，最后关闭连接
$mem->flush();

$val2 = $mem->get('key2');

echo "Get key2 value: ";

print_r($val2);

echo "<br>";




**【Memcache协议分析】**

如果你不喜欢 php_memcache.dll 扩展或者服务器器目前不支持这个扩展，那么就可以考虑自己构建，需要构建Memcahe的客户端，要先了解Memcache协议的交互，这样才能开发自己的客户端，我这里就简单的分析以下Memcache的协议。

（更详细的协议内容请在Memcache服务器端的源码的 doc/protocol.txt 文件中，本文基本来源于此）

Memcache既支持TCP协议，也支持UDP协议，不过我们这里是以TCP协议的协议作为主要考虑对象，想了解UDP协议的过程，请参考 doc/protocol.txt 文件。

**[ 错误指令]**

Memcache的协议的错误部分主要是三个错误提示之提示指令：

普通错误信息，比如指令错误之类的
ERROR/r/n

客户端错误
CLIENT_ERROR<错误信息>/r/n

服务器端错误
SERVER_ERROR<错误信息>/r/n

**[ 数据保存指令]**

数据保存是基本的功能，就是客户端通过命令把数据返回过来，服务器端接收后进行处理。

指令格式：
<命令> <键> <标记> <有效期> <数据长度>/r/n

**<命令>** - command name 
主要是三个储存数据的三个命令， set, add, replace
set 命令是保存一个叫做key的数据到服务器上
add 命令是添加一个数据到服务器，但是服务器必须这个key是不存在的，能够保证数据不会被覆盖
replace 命令是替换一个已经存在的数据，如果数据不存在，就是类似set功能

**<键>** - key

就是保存在服务器上唯一的一个表示符，必须是跟其他的key不冲突，否则会覆盖掉原来的数据，这个key是为了能够准确的存取一个数据项目

**<标记>** - flag

标记是一个16位的无符号整形数据，用来设置服务器端跟客户端一些交互的操作

**<有效期>** - expiration time

是数据在服务器上的有效期限，如果是0，则数据永远有效，单位是秒，Memcache服务器端会把一个数据的有效期设置为当前Unix时间+设置的有效时间

**<数据长度>** - bytes

数据的长度，block data 块数据的长度，一般在这个个长度结束以后下一行跟着block data数据内容，发送完数据以后，客户端一般等待服务器端的返回，服务器端的返回：

数据保存成功
STORED/r/n

数据保存失败，一般是因为服务器端这个数据key已经存在了
NOT_STORED/r/n


**[ 数据提取命令]**

从服务器端提取数据主要是使用get指令，格式是：
get<键>*/r/n

**<键>*** - key
key是是一个不为空的字符串组合，发送这个指令以后，等待服务器的返回。如果服务器端没有任何数据，则是返回：
END/r/n

证明没有不存在这个key，没有任何数据，如果存在数据，则返回指定格式：
VALUE <键> <标记> <数据长度>/r/n
**<数据块>**/r/n

返回的数据是以VALUE开始的，后面跟着key和flags，以及数据长度，第二行跟着数据块。

**<键>** -key 

是发送过来指令的key内容

**<标记>** - flags

是调用set指令保存数据时候的flags标记

**<数据长度>** - bytes

是保存数据时候定位的长度

**<数据块>** - data block

数据长度下一行就是提取的数据块内容



**[ 数据删除指令]**

数据删除指令也是比较简单的，使用get指令，格式是：

delete <键> <超时时间>/r/n

**<键>** - key

key是你希望在服务器上删除数据的key键

**<超时时间>** - timeout

按照秒为单位，这个是个可选项，如果你没有指定这个值，那么服务器上key数据将马上被删除，如果设置了这个值，那么数据将在超时时间后把数据清除，该项缺省值是0，就是马上被删除

删除数据后，服务器端会返回：
DELETED/r/n

删除数据成功
NOT_FOUND/r/n

这个key没有在服务器上找到

如果要删除所有服务器上的数据，可以使用flash_all指令，格式：
flush_all/r/n


这个指令执行后，服务器上所有缓存的数据都被删除，并且返回：
OK/r/n

这个指令一般不要轻易使，除非你却是想把所有数据都干掉，删除完以后可以无法恢复的。


**[其他指令]**

如果想了解当前Memcache服务器的状态和版本等信息，可以使用状态查询指令和版本查询指令。

如果想了解当前所有Memcache服务器运行的状态信息，可以使用stats指令，格式
stats/r/n

服务器将返回每行按照 STAT 开始的状态信息，包括20行，20项左右的信息，包括守护进程的pid、版本、保存的项目数量、内存占用、最大内存限制等等信息。


如果只是想获取部分项目的信息，可以指定参数，格式：
stats <参数>/r/n

这个指令将只返回指定参数的项目状态信息。

如果只是想单独了解当前版本信息，可以使用version指令，格式：
version/r/n

将返回以 VERSION 开头的版本信息

如果想结束当前连接，使用quit指令，格式：
quit/r/n

将断开当前连接

另外还有其他指令，包括incr, 
decr 等，我也不太了解作用，就不做介绍了，如果感兴趣，可以自己去研究。



**【Memcache在中型网站的使用】**

使用Memcache的网站一般流量都是比较大的，为了缓解数据库的压力，让Memcache作为一个缓存区域，把部分信息保存在内存中，在前端能够迅速的进行存取。那么一般的焦点就是集中在如何分担数据库压力和进行分布式，毕竟单台Memcache的内存容量的有限的。我这里简单提出我的个人看法，未经实践，权当参考。

**[ 分布式应用]**

Memcache本来支持分布式，我们客户端稍加改造，更好的支持。我们的key可以适当进行有规律的封装，比如以user为主的网站来说，每个用户都有User ID，那么可以按照固定的ID来进行提取和存取，比如1开头的用户保存在第一台Memcache服务器上，以2开头的用户的数据保存在第二胎Mecache服务器上，存取数据都先按照User ID来进行相应的转换和存取。

但是这个有缺点，就是需要对User ID进行判断，如果业务不一致，或者其他类型的应用，可能不是那么合适，那么可以根据自己的实际业务来进行考虑，或者去想更合适的方法。

**[ 减少数据库压力]**

这个算是比较重要的，所有的数据基本上都是保存在数据库当中的，每次频繁的存取数据库，导致数据库性能极具下降，无法同时服务更多的用户，比如MySQL，特别频繁的锁表，那么让Memcache来分担数据库的压力吧。我们需要一种改动比较小，并且能够不会大规模改变前端的方式来进行改变目前的架构。

我考虑的一种简单方法：

后端的数据库操作模块，把所有的Select操作提取出来（update/delete/insert不管），然后把对应的SQL进行相应的hash算法计算得出一个hash数据key（比如MD5或者SHA），然后把这个key去Memcache中查找数据，如果这个数据不存在，说明还没写入到缓存中，那么从数据库把数据提取出来，一个是数组类格式，然后把数据在set到Memcache中，key就是这个SQL的hash值，然后相应的设置一个失效时间，比如一个小时，那么一个小时中的数据都是从缓存中提取的，有效减少数据库的压力。

缺点是数据不实时，当数据做了修改以后，无法实时到前端显示，并且还有可能对内存占用比较大，毕竟每次select出来的数据数量可能比较巨大，这个是需要考虑的因素。

上面只是我两点没有经过深思熟虑的简单想法，也许有用，那就最好了。



**【Memcache的安全】**

我们上面的Memcache服务器端都是直接通过客户端连接后直接操作，没有任何的验证过程，这样如果服务器是直接暴露在互联网上的话是比较危险，轻则数据泄露被其他无关人员查看，重则服务器被入侵，因为Mecache是以root权限运行的，况且里面可能存在一些我们未知的bug或者是缓冲区溢出的情况，这些都是我们未知的，所以危险性是可以预见的。

为了安全起见，我做两点建议，能够稍微的防止黑客的入侵或者数据的泄露。

**[ 内网访问]**

最好把两台服务器之间的访问是内网形态的，一般是Web服务器跟Memcache服务器之间。普遍的服务器都是有两块网卡，一块指向互联网，一块指向内网，那么就让Web服务器通过内网的网卡来访问Memcache服务器，我们Memcache的服务器上启动的时候就监听内网的IP地址和端口，内网间的访问能够有效阻止其他非法的访问。

# memcached -d -m 1024  -u root -l 192.168.0.200 -p 11211 -c 1024 -P /tmp/memcached.pid

Memcache服务器端设置监听通过内网的192.168.0.200的ip的11211端口，占用1024MB内存，并且允许最大1024个并发连接


**[ 设置防火墙]**

防火墙是简单有效的方式，如果却是两台服务器都是挂在网的，并且需要通过外网IP来访问Memcache的话，那么可以考虑使用防火墙或者代理程序来过滤非法访问。

一般我们在Linux下可以使用iptables或者FreeBSD下的ipfw来指定一些规则防止一些非法的访问，比如我们可以设置只允许我们的Web服务器来访问我们Memcache服务器，同时阻止其他的访问。

# iptables -F

# iptables -P INPUT DROP

# iptables -A INPUT -p tcp -s 192.168.0.2 --dport 11211 -j ACCEPT

# iptables -A INPUT -p udp -s 192.168.0.2 --dport 11211 -j ACCEPT

上面的iptables规则就是只允许192.168.0.2这台Web服务器对Memcache服务器的访问，能够有效的阻止一些非法访问，相应的也可以增加一些其他的规则来加强安全性，这个可以根据自己的需要来做。



**【Memcache的扩展性】**

Memcache算是比较简洁高效的程序，Memcache 1.2.0 的源代码大小才139K，在Windows平台上是不可想象的，但是在开源世界来说，这是比较正常合理的。

Memcache目前都只是比较简单的功能，简单的数据存取功能，我个人希望如果有识之士，能够在下面两方面进行扩展。

**1. 日志功能**

目前Memcache没有日志功能，只有一些命令在服务器端进行回显，这样是很不利于对一个服务器的稳定性和负载等等进行监控的，最好能够相应的加上日志的等功能，便于监控。

**2. 存储结构**

目前的数据形式就是: key => data 的形式，特别单一，只能够存储单一的一维数据，如果能够扩展的话，变成类似数据库的格式，能够存储二维数据，那样会让可以用性更强，使用面更广，当然相应的可能代码效率和存取效率更差一些。

**3. 同步功能**

数据同步是个比较重要的技术，因为谁都不能保证一台服务器是持久正常的运行的，如果能够具有类似MySQL的 Master/Slave 的功能，那么将使得Memcache的数据更加稳定，那么相应的就可以考虑存储持久一点的数据，并且不用害怕Memcache的down掉，因为有同步的备份服务器，这个问题就不是问题了。

以上三点只是个人拙见，有识之士和技术高手可以考虑。



**【结束语】**

我上面的内容都只是自己安装和使用的一些想法，不能保证绝对正确，只是给需要的人一个参考，一个推广Memcache的文章，希望更多的人能够认识和了解这个技术，并且为自己所用。

我花费了整整一个晚上的时间洋洋洒洒的写了这么长，无非是对于这项开源技术的热爱，我想开源世界能够繁荣起来，就是源于大家的热爱并且愿意做出贡献，开源世界才这么精彩。

希望本文能够给需要的人一些帮助，希望不会误导他们，呵呵。


****

**附加：（我操作Memcache相应对应上面文章内容的图片） **

**[ 启动Memcache]![Memcache启动](https://p-blog.csdn.net/images/p_blog_csdn_net/heiyeshuwu/mem_start1.JPG)**

**[ Memcache的PHP测试代码]**

![代码](https://p-blog.csdn.net/images/p_blog_csdn_net/heiyeshuwu/mem_code.JPG)



**[测试代码执行效果]**

![浏览器效果](https://p-blog.csdn.net/images/p_blog_csdn_net/heiyeshuwu/mem_browser.JPG)



**[ 通过Telnet连接到Memcache ]**

![连接Memcache](https://p-blog.csdn.net/images/p_blog_csdn_net/heiyeshuwu/mem_cmd.JPG)

**[ 基本的Memcache的数据存取协议交互]**

![存取Memcache指令](https://p-blog.csdn.net/images/p_blog_csdn_net/heiyeshuwu/mem_cmd1.JPG)



**[ Memcache状态信息协议交互]**

![获取Memcache信息](https://p-blog.csdn.net/images/p_blog_csdn_net/heiyeshuwu/mem_cmd2.JPG)



