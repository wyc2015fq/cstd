# linux shell 脚本实现tcp/upd协议通讯（重定向应用） - xqhrs232的专栏 - CSDN博客
2017年04月24日 18:34:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1091
原文地址::[http://www.justwinit.cn/post/7985/](http://www.justwinit.cn/post/7985/)
相关文章
1、Linux下反弹shell方法----[https://www.waitalone.cn/linux-shell-rebound-under-way.html](https://www.waitalone.cn/linux-shell-rebound-under-way.html)
2、如何在Windows、Linux中安装后门？----[http://www.360doc.com/content/17/0310/20/40940225_635639207.shtml](http://www.360doc.com/content/17/0310/20/40940225_635639207.shtml)
背景：有时像硬件测试，比如SIM900a，如果你有一台公网ip服务器，想开一个tcp上的ip端口，在Windows下有tcp/ip的调试助手的界面软件，而linux下没有这样的玩意，怎么办呢？shell有这个功能，好像nc也有类似的模拟client去连接服务器的功能，咱就先找个shell的，Linux
 下有兄弟好像写了一个debian下的的deb包：http://blog.chinaunix.net/uid-21977056-id-4310527.html 如下。
bash socket 编程 ：
在bash环境中,你可以打开一个socket,并通过它发送数据. 并不需要安装curl或者lynx等程序.
通过两个特殊的设备文件, 可以打开网络socket
/dev/tcp/host/port
如果 host是一个有效的主机名或者IP地址, 端口号是一个整数或者常见的服务名(如dns, http), 那么bash将打开一个TCP连接 
/dev/udp/host/port
打开到host:port的UDP连接
(echo >/dev/tcp/localhost/80) &>/dev/null && echo “TCP port 80 open” || echo “TCP port 80 close”
/dev/[tcp|upd]/host/port 只要读取或者写入这个文件，相当于系统会尝试连接:host 这台机器，对应port端口。如果主机以及端口存在，就建立一个socket 连接。将在，/proc/self/fd目录下面，有对应的文件出现。
用来判断本机的80端口是否打开
另外, 一段crontab脚本
!/bin/bash
exec 8<>/dev/tcp/zhiwei.li/80
if [ $? -eq 0 ] ; then
    printf "GET /status HTTP/1.0rn" >&8
    printf "Host: zhiwei.lirn" >&8
    printf "User-Agent: Mozilla/5.0rn" >&8
    printf "rn" >&8
    while read -u 8 -r -t 2 block;
    do
            echo $block
            echo "$(date) - web server is running."
            while read -u 8 line ;
            do
                    echo $line
            done
            exec 8&-
            exit
    done
fi
echo “need reboot” 
…. 
来自：http://my.oschina.net/panzhc/blog/182843#OSC_h1_1
参考资料： 
http://xpt.sourceforge.net/techdocs/nix/shell/gsh05-ShBash/ar01s28.html 
http://www.cyberciti.biz/tips/spice-up-your-unix-linux-shell-scripts.html
————————————————————————————————————————————
一）如果出于效率的话还是选择工具的好shell不怎么合适做这个^_^，但作测试足够了：
直接在交互shell下
exec 3</dev/tcp/localhost/80
exec 3</dev/tcp/localhost/25
lsof -p $$
我开了httpd和sendmail服务，因此80和25端口有响应。
可以看到当前shell只打开了25这个端口。
二）如何查看这个shell是否真正打开了这个80端口？
一、安装nmap
yum install nmap #输入y安装
二、使用nmap
nmap localhost #查看主机当前开放的端口
nmap -p 1024-65535 localhost#查看主机端口（1024-65535）中开放的端口
nmap -PS 192.168.21.163 #探测目标主机开放的端口
nmap -PS22,80,3306 192.168.21.163 #探测所列出的目标主机端口
nmap -O 192.168.21.163 #探测目标主机操作系统类型
nmap -A 192.168.21.163 #探测目标主机操作系统类型
nmap --help #更多nmap参数请查询帮助信息
三、关闭或者打开主机端口
1、nmap localhost #查看主机当前开放端口
2、ntsysv #打开系统服务器管理器（需要先安装yum install ntsysv），选择要关闭或者打开的服务
摘自：http://www.dedecms.com/knowledge/servers/linux-bsd/2012/0819/8442.html
nmap localhost  
Starting Nmap 5.51 ( http://nmap.org ) at 2015-04-26 18:07 CST
Nmap scan report for localhost (127.0.0.1)
Host is up (0.000026s latency).
Other addresses for localhost (not scanned): 127.0.0.1
Not shown: 992 closed ports
PORT     STATE SERVICE
22/tcp   open  ssh
25/tcp   open  smtp
80/tcp   open  http
111/tcp  open  rpcbind
443/tcp  open  https
5901/tcp open  vnc-1
6001/tcp open  X11:1
8080/tcp open  http-proxy
三）测试：用telnet就行了。
——————————————————————————————————————————————————————————
http://www.csdn.net/article/2012-02-28/312483
时间 2014-03-17 22:04:04  向东博客
原文  http://justwinit.cn/post/7098/
前几天发了重定向以及管道相关使用方法，今天这里发些很有趣的例子。通过重定向实现基于tcp/udp协议的软件通讯。
linux 设备里面有个比较特殊的文件:
/dev/[tcp|upd]/host/port 只要读取或者写入这个文件，相当于系统会尝试连接:host 这台机器，对应port端口。如果主机以及端口存在，就建立一个socket 连接。将在，/proc/self/fd目录下面，有对应的文件出现。
一、测试下：/dev/tcp/host/post文件
[chengmo@centos5  shell]$ cat</dev/tcp/127.0.0.1/22
SSH-2.0-OpenSSH_5.1
#我的机器shell端口是：22
#实际:/dev/tcp根本没有这个目录，这是属于特殊设备
[chengmo@centos5  shell]$ cat</dev/tcp/127.0.0.1/223
-bash: connect: 拒绝连接
-bash: /dev/tcp/127.0.0.1/223: 拒绝连接
#223接口不存在,打开失败
[chengmo@centos5  shell]$ exec 8<>/dev/tcp/127.0.0.1/22
[chengmo@centos5  shell]$ ls -l /proc/self/fd/
总计 0
lrwx------ 1 chengmo chengmo 64 10-21 23:05 0 -> /dev/pts/0
lrwx------ 1 chengmo chengmo 64 10-21 23:05 1 -> /dev/pts/0
lrwx------ 1 chengmo chengmo 64 10-21 23:05 2 -> /dev/pts/0
lr-x------ 1 chengmo chengmo 64 10-21 23:05 3 -> /proc/22185/fd
lrwx------ 1 chengmo chengmo 64 10-21 23:05 8 -> socket:[15067661]
#文件描述符8，已经打开一个socket通讯通道，这个是一个可以读写socket通道,因为用："<>"打开
[chengmo@centos5  shell]$ exec 8>&-
#关闭通道
[chengmo@centos5  shell]$ ls -l /proc/self/fd/
总计 0
lrwx------ 1 chengmo chengmo 64 10-21 23:08 0 -> /dev/pts/0
lrwx------ 1 chengmo chengmo 64 10-21 23:08 1 -> /dev/pts/0
lrwx------ 1 chengmo chengmo 64 10-21 23:08 2 -> /dev/pts/0
lr-x------ 1 chengmo chengmo 64 10-21 23:08 3 -> /proc/22234/fd
从时间服务器读取时间：
[chengmo@centos5 html]$ cat</dev/tcp/time-b.nist.gov/13
55491 10-10-22 11:33:49 17 0 0 596.3 UTC(NIST) *
上面这条语句使用重定向输入语句就可以了。
二、通过重定向读取远程web服务器头信息
#!/bin/sh 
#testhttphead.sh
#实现通过主机名，端口读取web 服务器header信息
#copyright chengmo,qq:8292669
if(($#<2));then
    echo "usage:$0 host port";
    exit 1;
fi
#如果参数缺失，退出程序，返回状态1
exec 6<>/dev/tcp/$1/$2 2>/dev/null;
#打开host的port 可读写的socket连接，与文件描述符6连接
if(($?!=0));then
    echo "open $1 $2 error!";
    exit 1;
fi
#如果打开失败，$?返回不为0，终止程序
echo -e "HEAD / HTTP/1.1\n\n\n\n\n">&6; 
#将HEAD 信息，发送给socket连接
cat<&6; 
#从socket读取返回信息，显示为标准输出
exec 6<&-;
exec 6>&-; 
#关闭socket的输入，输出
exit 0;
脚本建立后：存为testhttphead.sh
运行结果：
[chengmo@centos5 ~/shell]$ sh testhttphead.sh www.baidu.com 80
HTTP/1.1 200 OK
Date: Thu, 21 Oct 2010 15:17:23 GMT
Server: BWS/1.0
Content-Length: 6218
Content-Type: text/html;charset=gb2312
Cache-Control: private
Expires: Thu, 21 Oct 2010 15:17:23 GMT
Set-Cookie: BAIDUID=1C40B2F8C676180FD887379A6E286DC1:FG=1; expires=Thu, 21-Oct-40 15:17:23 GMT; path=/; domain=.baidu.com
P3P: CP=" OTI DSP COR IVA OUR IND COM "
Connection: Keep-Alive
[chengmo@centos5 ~/shell]$ sh testhttphead.sh 127.0.0.1 8080
open 127.0.0.1 8080 error!
突然有个奇怪想法：
我们在windows时代就通过telnet 可以实现tcp/upd协议通讯，那么如果用传统方法怎么实现呢？
[chengmo@centos5 ~/shell]$ echo -e "HEAD / HTTP/1.1\n\n\n\n\n"|telnet www.baidu.com 80 
Trying 220.181.6.175...
Connected to www.baidu.com.
Escape character is '^]'.
Connection closed by foreign host.
#直接给发送，失败
[chengmo@centos5 ~/shell]$ (telnet www.baidu.com 80)<<EOF
HEAD / HTTP/1.1
EOF
Trying 220.181.6.175...
Connected to www.baidu.com.
Escape character is '^]'.
Connection closed by foreign host.
#重定向输入，还是失败？
找到正确方法：
[chengmo@centos5 shell]$ (echo -e "HEAD / HTTP/1.1\n\n\n\n\n";sleep 2)|telnet www.baidu.com 80 
Trying 220.181.6.175...
Connected to www.baidu.com.
Escape character is '^]'.
HTTP/1.1 200 OK
Date: Thu, 21 Oct 2010 15:51:58 GMT
Server: BWS/1.0
Content-Length: 6218
Content-Type: text/html;charset=gb2312
Cache-Control: private
Expires: Thu, 21 Oct 2010 15:51:58 GMT
Set-Cookie: BAIDUID=0B6A01ACECD5353E4247E088A8CB345A:FG=1; expires=Thu, 21-Oct-40 15:51:58 GMT; path=/; domain=.baidu.com
P3P: CP=" OTI DSP COR IVA OUR IND COM "
Connection: Keep-Alive
#成功了！加入sleep 居然可以了，sleep 改成1秒也可以
是不是由于sleep后，echo会推出2秒发给通道：telnet呢？推论可以从这2个方面推翻：
一个方面：通过()括的数据是一对命令，会作为一个子命令执行，一起执行完程序结束。每个命令echo语句，就直接发送到屏幕（也就是标准输出），只要有标准输出了，就会通过通道马上传个：telnet ，如果接下来命令还有输出，会注意传给telnet ，直到()内所有命令执行完，与通道连接就断开了。
再一个方面：如果说是起到推迟发送的话，什么时候有数据过来，发给telnet，什么时候telnet命令启动。跟你推迟一点还是早一点发送过来。没有关系。
这种类型命令，看出sleep，其实就是保持通道跟telnet 连接2秒钟。 通道连接着了，telnet终端输入也还在，因此可以保持从baidu服务器获得数据。
所以,延迟多久，还是跟服务器处理速度有关系。
如果通过echo 向telnet发送数据，保持通道联通，使用sleep是个很好方法。
通过重定向给telnet输入参数这种方法，我还想不到怎么样实现延迟输入。有知道朋友，可以指点指点.
区别：
telnet与echo 实现 http访问，与通过打开读写socket是不一样的，打开socket通道，是可以进行交换处理的。传入命令，活动结果，再传入命令，再获得结果。telnet通过echo 就不能这样处理了。
三、通过shell脚本重定向实现监控memcache状态
实例：
#!/bin/sh
#通过传入ip 以及端口，发送指令获得返回数据
#copyright chengmo qq:8292669
#函数往往放到最上面
function sendmsg()
{
    msg=$1;
    echo  "$1">&8;
    getout;
}
#向socket通道发送指令，并且调用获得返回参数
function getout()
{   
    #read 命令 -u 从打开文件描述符 8 读取数据，-d读取数据忽略掉:\r换行符 
    while read -u 8 -d $'\r' name; 
    do 
        if [ "${name}" == "END"  -o "${name}" == "ERROR" ];then
            break;
        fi;
        echo $name;
    done
}
#由于：memcached每次通讯完毕，会返回：END或者ERROR(出错），通过判断是否是"END"觉得读取是不是结束，否则循环不会停止
if [ $# -lt 2 ];then
    echo "usage:$0 host port [command]";
    exit 1;
fi;
[[ $# -gt 2 ]]&&command=$3;
#设置默认值 如果command为定义则为：stats
command="${command=stats}";
host="$1";
port="$2";
exec 8<>/dev/tcp/${host}/${port};
#打开通向通道是8
if [ "$?" != "0" ];then
    echo "open $host  $port fail!";
    exit 1;
fi
sendmsg "$command";
#发送指定命令
sendmsg "quit";
#发送退出通向命令
exec 8<&-;
exec 8>&-;
#关闭socket通道
exit 0;
这是通过重定向，实现socket通讯中，发送然后获取返回的例子。其实，上面代码看似一次只能发送一段。时间上。我们可以反复调用：sendmsg ，捕捉输出数据。实现连续的，读与写操作。
实例截图：
其它实现方法：
其实通过：telnet也可以实现的。
[chengmo@centos5 shell]$ (echo "stats";sleep 2)|telnet 127.0.0.1 11211
通过nc命令实现：
[chengmo@centos5 shell]$ (echo "stats")|nc 127.0.0.1 11211
不需要加延迟，直接打开通道
第二个程序里面，看到shell完全可以处理交互设计了。如果按照这样，登陆ftp,pop3,stmp都可以类似实现。这些，我们通过shell socket类似程序实现，应该不困难，只是捕捉如发送解析的问题了。
来自：http://www.cnblogs.com/chengmo/archive/2010/10/22/1858302.html
http://blog.chinaunix.net/uid-21454189-id-442258.html
**作者：[justwinit](http://www.justwinit.cn/user/1/)@[向东博客 专注WEB应用 构架之美 --- 构架之美，在于尽态极妍 | 应用之美，在于药到病除](http://www.justwinit.cn/)地址：[http://www.justwinit.cn/post/7985/](http://www.justwinit.cn/post/7985/)版权所有。转载时必须以链接形式注明作者和原始出处及本声明！**
