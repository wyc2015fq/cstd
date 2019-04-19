# PHP生命周期及运行模式 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2018年03月06日 13:56:32[baofenny](https://me.csdn.net/jiaminbao)阅读数：358
# [PHP](https://www.2cto.com/kf/web/php/)运行模式
1）CGI（通用网关接口 / Common Gateway Interface）
2）FastCGI（常驻型CGI / Long-Live CGI）
3）CLI（命令行运行 / Command Line Interface）
4）Web模块模式（Apache等Web服务器运行的模式）
~~5）ISAPI（Internet Server Application Program Interface）~~
备注：在PHP5.3以后，PHP不再有ISAPI模式
CGI是个协议，跟进程什么的没关系。那fastcgi又是什么呢？Fastcgi是用来提高CGI程序性能的。
# PHP中的CGI实现
PHP的CGI实现本质是是以socket[编程](https://www.2cto.com/kf)实现一个TCP或UDP协议的服务器，当启动时，创建TCP/UDP协议的服务器的socket监听， 并接收相关请求进行处理。这只是请求的处理，在此基础上添加模块初始化，sapi初始化，模块关闭，sapi关闭等就构成了整个CGI的生命周期。
![](https://img-blog.csdn.net/20180306135515154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbWluYmFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## CGI
> 
CGI全称是“通用网关接口”(Common Gateway Interface)， 它可以让一个客户端，从网页[浏览器](https://www.2cto.com/os/liulanqi/)向执行在Web服务器上的程序请求数据。
CGI描述了客户端和这个程序之间传输数据的一种标准。
CGI的一个目的是要独立于任何语言的，所以CGI可以用任何一种语言编写，只要这种语言具有标准输入、输出和环境变量。如php,perl,tcl等
CGI已经是比较老的模式了，这几年都很少用了。
每有一个用户请求，都会先要创建CGI的子进程，然后处理请求，处理完后结束这个子进程，这就是Fork-And-Execute模式。 当用户请求数量非常多时，会大量挤占[系统](https://www.2cto.com/os/)的资源如内存，CPU时间等，造成效能低下。
 所以用CGI方式的服务器有多少连接请求就会有多少CGI子进程，子进程反复加载是CGI性能低下的主要原因。
当web server收到/index.php这个请求后，会启动对应的CGI程序，这里就是PHP的解析器。接下来PHP解析器会解析php.ini文件，初始化执行环境，然后处理请求，再以规定CGI规定的格式返回处理后的结果，退出进程。web server再把结果返回给浏览器。
## FastCGI
> 
fast-cgi 是cgi的升级版本，FastCGI 像是一个常驻 (long-live) 型的 CGI，它可以一直执行着，只要激活后，不会每次都要花费时间去 fork 一次 (这是 CGI 最为人诟病的 fork-and-execute 模式)。
### FastCGI工作原理
Web Server启动时载入FastCGI进程管理器【PHP的FastCGI进程管理器是PHP-FPM(php-FastCGI Process Manager)】（IIS ISAPI或Apache Module) FastCGI进程管理器自身初始化，启动多个CGI解释器进程 (可见多个php-cgi.exe或php-cig)并等待来自Web
 Server的连接； 当客户端请求到达Web Server时，FastCGI进程管理器选择并连接到一个CGI解释器。Web server将CGI环境变量和标准输入发送到FastCGI子进程php-cgi FastCGI子进程完成处理后将标准输出和错误信息从同一连接返回Web Server。当FastCGI子进程关闭连接时，请求便告处理完成。FastCGI子进程接着等待并处理来自FastCGI进程管理器（运行在 WebServer中）的下一个连接。 在正常的CGI模式中，php-cgi.exe在此便退出了。在CGI模式中，你可以想象
 CGI通常有多慢。每一个Web请求PHP都必须重新解析php.ini、重新载入全部dll扩展并重初始化全部数据结构。使用FastCGI，所有这些都只在进程启动时发生一次。一个额外的好处是，持续[数据库](https://www.2cto.com/database/)连接（Persistent
 database connection）可以工作。
备注：PHP的FastCGI进程管理器是PHP-FPM（PHP-FastCGI Process Manager）
### 优点
从稳定性上看，FastCGI是以独立的进程池来运行CGI，单独一个进程死掉，系统可以很轻易的丢弃，然后重新分配新的进程来运行逻辑； 从安全性上看，FastCGI支持分布式运算。FastCGI和宿主的Server完全独立，FastCGI怎么down也不会把Server搞垮； 从性能上看，FastCGI把动态逻辑的处理从Server中分离出来，大负荷的IO处理还是留给宿主Server，这样宿主Server可以一心一意作IO，对于一个普通的动态网页来说,
 逻辑处理可能只有一小部分，大量的是图片等静态。
### 不足
因为是多进程，所以比CGI多线程消耗更多的服务器内存，PHP-CGI解释器每进程消耗7至25兆内存，将这个数字乘以50或100就是很大的内存数。
> 
Nginx 0.8.46+PHP 5.2.14(FastCGI)服务器在3万并发连接下，开启的10个Nginx进程消耗150M内存（15M*10=150M），开启的64个php-cgi进程消耗1280M内存（20M*64=1280M），加上系统自身消耗的内存，总共消耗不到2GB内存。如果服务器内存较小，完全可以只开启25个php-cgi进程，这样php-cgi消耗的总内存数才500M。
上面的数据摘自Nginx 0.8.x + PHP 5.2.13(FastCGI)搭建胜过Apache十倍的Web服务器(第6版)
## CLI
> 
PHP-CLI是PHP Command Line Interface的简称，就是PHP在命令行运行的接口，区别于在Web服务器上运行的PHP环境（PHP-CGI，ISAPI等）。
也就是说，PHP不单可以写前台网页，它还可以用来写后台的程序。 PHP的CLI Shell脚本适用于所有的PHP优势，使创建要么支持脚本或系统甚至与GUI应用程序的服务端，在Windows和[Linux](https://www.2cto.com/os/linux/)下都是支持PHP-CLI模式的。
我们在Linux下经常使用”php –m”查找PHP安装了那些扩展就是PHP命令行运行模式；
# PHP开始和结束阶段
PHP开始执行以后会经过两个主要的阶段：处理请求之前的开始阶段和请求之后的结束阶段。
![](https://img-blog.csdn.net/20180306140211983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbWluYmFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 开始阶段
### 模块初始化阶段MINIT
> 
在整个SAPI生命周期内(例如Apache启动以后的整个生命周期内或者命令行程序整个执行过程中)， 该过程只进行一次。
启动Apache后，PHP解释程序也随之启动；
PHP调用各个扩展（模块）的MINIT方法，从而使这些扩展切换到可用状态。
[?](https://www.2cto.com/kf/201601/456544.html#)
```
```java
<code
```
```java
class
```
```java
=
```
```java
"language-c
 hljs "
```
```java
>PHP_MINIT_FUNCTION(myphpextension)
```
```java
{
```
```java
```
```java
//
 注册常量或者类等初始化操作
```
```java
```
```java
return
```
```java
SUCCESS;
```
```java
}</code>
```
```
### 模块激活阶段RINIT
> 
该过程发生在请求阶段， 例如通过url请求某个页面，则在每次请求之前都会进行模块激活（RINIT请求开始）。
请求到达之后，SAPI层将控制权交给PHP层，PHP初始化本次请求执行脚本所需的环境变量
例如是Session模块的RINIT，如果在php.ini中启用了Session 模块，那在调用该模块的RINIT时就会初始化$_SESSION变量，并将相关内容读入； 然后PHP会调用所有模块RINIT函数,即“请求初始化”。
在这个阶段各个模块也可以执行一些相关的操作, 模块的RINIT函数和MINIT函数类似 ，RINIT方法可以看作是一个准备过程，在程序执行之前就会自动启动。
## 结束阶段
请求处理完后就进入了结束阶段, 一般脚本执行到末尾或者通过调用exit()或者die()函数,PHP都将进入结束阶段. 和开始阶段对应,结束阶段也分为两个环节,一个在请求结束后(RSHUWDOWN),一个在SAPI生命周期结束时(MSHUTDOWN).
### 请求结束后(RSHUWDOWN)
> 
请求处理完后就进入了结束阶段，PHP就会启动清理程序。
它会按顺序调用各个模块的RSHUTDOWN方法。
RSHUTDOWN用以清除程序运行时产生的符号表，也就是对每个变量调用unset函数。
### SAPI生命周期结束时(MSHUTDOWN)
> 
最后，所有的请求都已处理完毕
SAPI也准备关闭了
PHP调用每个扩展的MSHUTDOWN方法
这时各个模块最后一次释放内存的机会。
（这个是对于CGI和CLI等SAPI，没有“下一个请求”，所以SAPI立刻开始关闭。）
整个PHP生命周期就结束了。要注意的是，只有在服务器没有请求的情况下才会执行“启动第一步”和“关闭第二步”。
## SAPI运行PHP都经过的几个阶段
模块初始化阶段(Module init)
[?](https://www.2cto.com/kf/201601/456544.html#)
```
```java
<code>   
 即调用每个拓展源码中的的PHP_MINIT_FUNCTION中的方法初始化模块,进行一些模块所需变量的申请,内存分配等。
```
```java
</code>
```
```
请求初始化阶段(Request init)
[?](https://www.2cto.com/kf/201601/456544.html#)
```
```java
<code>
 即接受到客户端的请求后调用每个拓展的PHP_RINIT_FUNCTION中的方法,初始化PHP脚本的执行环境。
```
```java
</code>
```
```
执行PHP脚本
请求结束(Request Shutdown)
[?](https://www.2cto.com/kf/201601/456544.html#)
```
```java
<code>这时候调用每个拓展的PHP_RSHUTDOWN_FUNCTION方法清理请求现场,并且ZE开始回收变量和内存
```
```java
</code>
```
```
关闭模块(Module shutdown)
[?](https://www.2cto.com/kf/201601/456544.html#)
```
```java
<code>Web服务器退出或者命令行脚本执行完毕退出会调用拓展源码中的PHP_MSHUTDOWN_FUNCTION
 方法
```
```java
</code>
```
```
# 单进程SAPI生命周期
[?](https://www.2cto.com/kf/201601/456544.html#)
```
```java
<code>CLI/CGI模式的PHP属于单进程的SAPI模式。这类的请求在处理一次请求后就关闭。
```
```java
也就是只会经过如下几个环节：
 开始 - 请求开始 - 请求关闭 - 结束 SAPI接口实现就完成了其生命周期。
```
```java
</code>
```
```
![](https://img-blog.csdn.net/2018030613555673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbWluYmFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 多进程SAPI生命周期
通常PHP是编译为apache的一个模块来处理PHP请求。 Apache一般会采用多进程模式， Apache启动后会fork出多个子进程，每个进程的内存空间独立，每个子进程都会经过开始和结束环节 每个进程的开始阶段只在进程fork出来以来后进行，在整个进程的生命周期内可能会处理多个请求。 只有在Apache关闭或者进程被结束之后才会进行关闭阶段，在这两个阶段之间会随着每个请求重复请求开始-请求关闭的环节。
![这里写图片描述](https://www.2cto.com/uploadfile/Collfiles/20160106/20160106092139241.gif)
# 多线程的SAPI生命周期
[?](https://www.2cto.com/kf/201601/456544.html#)
```
```java
<code>多线程模式和多进程中的某个进程类似，不同的是在整个进程的生命周期内会并行的重复着
 请求开始-请求关闭的环节.
```
```java
</code>
```
```
在这种模式下，只有一个服务器进程在运行着，但会同时运行很多线程，这样可以减少一些资源开销，向Module init和Module shutdown就只需要运行一遍就行了，一些全局变量也只需要初始化一次，因为线程独具的特质，使得各个请求之间方便的共享一些数据成为可能。
![这里写图片描述](https://www.2cto.com/uploadfile/Collfiles/20160106/20160106092143242.gif)
