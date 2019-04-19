# rejected execution (queue capacity 50) - 三少GG - CSDN博客
2015年09月08日 21:36:56[三少GG](https://me.csdn.net/scut1135)阅读数：1872
测试过程中，会出现错误：
[?](http://my.oschina.net/qiangzigege/blog/220224#)
```
```java
[
```
```java
7
```
```java
]:
 index [yyyy], type [rrrr], id [
```
```java
1964986
```
```java
],
 message [RemoteTransportException[[2sdfsdf][inet[/xxxxxxxxxx:
```
```java
9300
```
```java
]][bulk/shard]];
 nested: EsRejectedExecutionException[rejected execution (queue capacity
```
```java
50
```
```java
)
 on org.elasticsearch.action.support.replication.TransportShardReplicationOperationAction$AsyncShardOperationAction$
```
```java
1
```
```java
@3e82ee89
```
```java
];
 ]
```
```
修改配置文件，在最后增加：
[?](http://my.oschina.net/qiangzigege/blog/220224#)
```
```java
threadpool:
```
```java
```
```java
bulk:
```
```java
```
```java
type:
 fixed
```
```java
```
```java
size:
```
```java
60
```
```java
```
```java
queue_size:
```
```java
1000
```
```
至于这几个参数是什么意思，还请读者自己去弄明白。
参考：
[http://stackoverflow.com/questions/20683440/elasticsearch-gives-error-about-queue-size](http://stackoverflow.com/questions/20683440/elasticsearch-gives-error-about-queue-size)
[http://www.elasticsearch.org/guide/en/elasticsearch/reference/current/modules-threadpool.html](http://www.elasticsearch.org/guide/en/elasticsearch/reference/current/modules-threadpool.html)

# ubuntu 14.04上全局修改ulimit
![刘君涛](http://img4.douban.com/icon/u1347326-9.jpg)[刘君涛](http://www.douban.com/people/jinuljt/)2015-06-18
 01:15:14
远程登录服务器，在shell里面ulimit -n 65535 这样修改文件描述符数量限制的话实际上只在当前shell以及从当前shell启动的程序才会生效。那么那些随机启动的程序怎么办？当然可以程序启动的时候设置这个limit了。但是总觉得有点不怎么爽利。
下面这个通过修改系统配置文件来全局生效的方法，该文件是： /etc/security/limits.conf。
例如想要把所有用户的文件描述符数量限制修改为 65535，可以这样配置：
* soft nofile 65535
* hard nofile 65535
root soft nofile 65535
root hard nofile 65535
* 是通配所有用户。
but！root用户必须单独指定，因为通配符不允许匹配root。

### [linux最大文件句柄数量总结](http://jameswxx.iteye.com/blog/2096461)
原创文章，转载请注明出处：http://jameswxx.iteye.com/blog/2096461
写这个文章是为了以正视听，网上的文章人云亦云到简直令人发指。到底最大文件数被什么限制了？too many open files错误到底可以通过什么参数控制？网上的很多文章说的大致步骤是没有错的，大致如下：
shell级限制 
通过ulimit -n修改，如执行命令ulimit -n 1000，则表示将当前shell的当前用户所有进程能打开的最大文件数量设置为1000.
用户级限制  
ulimit -n是设置当前shell的当前用户所有进程能打开的最大文件数量，但是一个用户可能会同时通过多个shell连接到系统，所以还有一个针对用户的限制，通过修改 /etc/security/limits.conf实现，例如，往limits.conf输入以下内容：
*root soft nofile 1000 *
*root hard nofile 1200*
soft nofile表示软限制，hard nofile表示硬限制，软限制要小于等于硬限制。上面两行语句表示，root用户的软限制为1000，硬限制为1200，即表示root用户能打开的最大文件数量为1000，不管它开启多少个shell。
系统级限制
修改cat /proc/sys/fs/file-max
但是呢，有很多很重要的细节，有很多错误的描述，一塌糊涂，因此特的在这里做一个说明。
## **一  ulimit -n**
     网上很多人说，ulimit -n限制用户单个进程的问价打开最大数量。严格来说，这个说法其实是错误的。看看ulimit官方描述：
***Provides control over the resources available to the shell and to processes started by  it,  on  systems that allow such control.**The -H and -S options specify that the hard or soft limit is set for the given resource. A hard limit cannot be increased once it is set; a soft limit may  be  increased  up  to  the value of the hard limit. If neither -H nor -S is specified, both the soft and hard limits are set. The value of limit can be a number in the unit specified for the resource or one of the special values hard, soft,  or  unlimited,  which  stand  for  the  current hard limit, the current soft limit, and no limit,  respectively.*
*If limit is omitted, the current value of the soft limit  of  the  resource  is  printed,  unless  the  -H  option is given.  When more than one resource is specified, the limit name and unit are  printed before the value.*
人家从来就没说过是限制用户的单个进程的最大文件打开数量，看看红色部分，是限制当前shell以及该shell启动的进程打开的文件数量。为什么会给人限制单个线程的最大文件数量的错觉，因为很多情况下，在一个shell环境里，虽然可能会有多个进程，但是非常耗费文件句柄的进程不会很多，只是其中某个进程非常耗费文件句柄，比如服务器上运行着一个tomcat，那么就是java进程要占用大多数文件句柄。此时ulimit设置的最大文件数和java进程耗费的最大文件数基本是对应的，所以会给人这样的一个错觉。 
还有，很多文章称ulimit -n 只允许设置得越来越小，比如先执行了ulimit -n 1000，在执行ulimit -n 1001，就会报"cannot modify limit: Operation not permitted"错误。这个其实也是不准确的说法。首先要搞清楚，任何用户都可以执行ulimit，但root用户和非root用户是非常不一样的。
**非root用户只能越设置越小，不能越设置越大**
我在机器上以非root先执行：
*[wxx@br162 etc]$ ulimit -n 900[wxx@br162 etc]$*
执行成功，再增大：
*[wxx@br162 etc]$ ulimit -n 901-bash: ulimit: open files: cannot modify limit: Operation not permitted*
*[wxx@br162 etc]$*
增加失败，如果减少则是OK的：
*[wxx@br162 etc]$ ulimit -n 899*
*[wxx@br162 etc]$*
如果再增加到900是不行的：
*[wxx@br162 etc]$ ulimit -n 900-bash: ulimit: open files: cannot modify limit: Operation not permitted*
*[wxx@br162 etc]$ *
**root用户不受限制**
首先切换到root：
*[wxx@br162 etc]$ sudo su -[root@br162 ~]#*
查看下当前限制：
*[root@br162 ~]# ulimit -n1000000*
*[root@br162 ~]#*
增大：
* [root@br162 ~]# ulimit -n 1000001*
*[root@br162 ~]#*
可以成功增大，再减小：
*[root@br162 ~]# ulimit -n 999999*
*[root@br162 ~]#*
减小也是成功的，再增大：
* [root@br162 ~]# ulimit -n 1000002*
*[root@br162 ~]#*
也是ok的，可见root是不受限制的。 
**ulimit里的最大文件打开数量的默认值**
如果在limits.conf里没有设置，则默认值是1024，如果limits.con有设置，则默认值以limits.conf为准。例如我换了一台机器，登录进去，ulimit -n显示如下：
*[root@zk203 ~]# ulimit -n2000*
这是因为我的limits.conf里的文件打开数是2000，如下：
*[root@zk203 ~]# cat /etc/security/limits.conf*
*root soft nofile 2000*
*root hard nofile 2001*
如果limits.conf里不做任何限制，则重新登录进来后，ulimit -n显示为1024。
* [root@zk203 ~]# ulimit -n*
*1024*
**ulimit修改后生效周期**
修改后立即生效，重新登录进来后失效，因为被重置为limits.conf里的设定值
## **二  /etc/security/limits.conf**
**网上还有缪传，ulimit -n设定的值不能超过limits.conf里设定的文件打开数（即soft nofile）**
好吧，其实这要分两种情况，root用户是可以超过的，比如当前limits.conf设定如下：
*root soft nofile 2000*
*root hard nofile 2001*
但是我用root将最大文件数设定到5000是成功的：
*[root@zk203 ~]# ulimit -n 5000[root@zk203 ~]# ulimit -n 5000[root@zk203 ~]#*
但是非root用户是不能超出limits.conf的设定，我切换到wxx，执行命令如下：
*[wxx@zk203 ~]# ulimit -n 5000*
*-bash: ulimit: open files: cannot modify limit: Operation not permitted*
*[wxx@zk203 etc]$ *
所以网上的说法是错误的，即使非root用户的最大文件数设置不能超过limits.conf的设置，这也只是一个表象，实际上是因为，每个用户登录进来，ulimit -n的默认值是limits.conf的 soft nofile指定的，但是对于非root用户，ulimit -n只能越来越小，不能越来越大，其实这个才是真正的原因，但是结果是一样的。
**修改了limits.conf需要重启系统？**
这个说法非常搞笑，修改了limits.conf，**重新登录shell**进来就可以生效了。在机器上试试就知道了，好多人真的很懒，宁愿到处问也不愿意花一分钟时间实际操作一下。
## **三  /proc/sys/fs/file-max**
网上说，ulimit -n 和limits.conf里最大文件数设定不能超过/proc/sys/fs/file-max的值，这也是搞笑了，/proc/sys/fs/file-max是系统给出的建议值，系统会计算资源给出一个和合理值，一般跟内存有关系，内存越大，改值越大，但是仅仅是一个建议值，limits.conf的设定完全可以超过/proc/sys/fs/file-max。
*[root@zk203 ~]# cat /proc/sys/fs/file-max1610495*
我将limits.conf里文件最大数量设定为1610496，保存后，打印出来：
*[root@zk203 ~]# cat /etc/security/limits.conf*
*root soft nofile1610496*
*root hard nofile1610496*
## **四  总结一下**
- */proc/sys/fs/file-max限制不了/etc/security/limits.conf*
- *只有root用户才有权限修改/etc/security/limits.conf*
- *对于非root用户， /etc/security/limits.conf会限制ulimit -n，但是限制不了root用户*
- *对于非root用户，ulimit -n只能越设置越小，root用户则无限制*
- *任何用户对ulimit -n的修改只在当前环境有效，退出后失效，重新登录新来后，ulimit -n由limits.conf决定*
- *如果limits.conf没有做设定，则默认值是1024*
- *当前环境的用户所有进程能打开的最大文件数量由ulimit -n决定*
- 

