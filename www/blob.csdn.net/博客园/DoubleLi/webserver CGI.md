# webserver/CGI - DoubleLi - 博客园






来自：http://blog.sina.com.cn/s/blog_466c6640010000nj.html



1. TUX
2. lighttpd,thttpd,shttpd 
3. 几种web server表现的对比
4. 我的总结



TUX  
[http://www.chinadesign.com.cn/NewsContents1.asp?id=3386](http://www.chinadesign.com.cn/NewsContents1.asp?id=3386)

tux是一种有GPL（GNU General Public License ）许可的基于内核的Web服务器。

　　目前它还只能提供静态的网页服务，只有和内核区模块，用户区模块，以及一般的用户区Web服务器后台一起应用的时候才提供动态的内容服务。一般用户区的Web服务器不需要为了适应TUX而改变很多地方。然而，用户区数据段代码必须要有一个通向TUX的系统调用的接口。

　　虽然动态的内容服务现在增长的很迅速，但是也必须要提供静态的内容服务。如，所有的图片文件就都是静态的。TUX能给在Linux的内核里提供非常高效率的静态内容服务，就象在内核里运行的NFS网络文件系统服务一样。

　　TUX也有处理动态内容的功能。TUX模块用网页缓冲存储动态内容的对象。当一个动态的请求到达的时候，一个TUX的模块就发送会一个联合 的动态请求并缓冲这个请求。这就是TUX的零拷贝缓存校验的结构。（建议TUX模块不许运行在用户区，虽然他们也可以运行在内核区。）

　　这个新的动态内容的处理需要一个新的API接口。当前的CGI的API接口不能充分的用在TUX的API上。因此，当前的CGI程序为了使 用TUX就要修改当前的程序。如果CGI程序不需要增加TUX的速度的话，TUX也可以把它安一般的CGI来处理。要实现这个功能就要用TUX的CGI模 块。TUX也可以处理复杂的CGI或是其他的请求，这要通过它把请求重定向到另外一个web服务器后台程序，如Apache。也就是说，静态的内 容，TUX模块，老的CGI程序，以及为TUX Web服务器编写的特定的程序都可以在TUX这个主服务器里运行。

　　一句话，TUX和其他的Web服务器的区别和TUX的好处包括：

TUX是作为Linux的内核2.4.x或更高的一部分来运行的，另外一部分可以做为用户区来运行。

TUX缓存部分的TCP校验并用它们来加快网络数据传输速度。

用一个特定的网络卡，TUX可以从页面缓存定向分散的DMA直接到网络，这样就避免了数据的拷贝。

当TUX不知道如何去处理一个请求或是接受到一个请求不能去运行的时候，它一般会把这个请求传送到用户区的Web服务器后台去处理它。一个典型的用户区Web服务器就是Apache。

2．TUX 2.0的新功能

　　TUX 2.0是从TUX 1.0升级成的，仍然保守着与用户区模块的兼容性。

　　增加的功能包括：

     真正的零磁盘读拷贝，TUX1.0是放到一个临时的缓存里的。
     生成网络写的零拷贝。
     零拷贝解析。

 　其他新功能：

     提高用户去功能模块的支持。
     虚拟主机的支持，它不限制虚拟主机的数量。
     CGI可以绑定到特定的进程
     一些BUG的修复。

3．系统的基本需要

     TUX需要基于2.4.x内核或是更高版本的系统。
     基于X86的服务器。
     交替的Web服务器如Apache来处理不知道的请求
　当前的限制：

     内核模块只支持服务器端。
     TUX只在x86的Linux平台上测试过。
     TUX能请求其他的Web服务器如Apache来处理请求。在当前的版本，它对回滚不是处理的很好。


lighttpd,thttpd,shttpd
国内绝大部分的web server不是IIS就是Apache，而论市场占有率，我认为Apache是大赢家了，至少是占据了半壁江山。
但 除了IIS/Apache外，其实我们有很多选择，对于高负载/大并发的网站而言，高性能、轻量级的web server是一剂良药。最近手头一台Server 的负载太高，居然将swap吃光导致机器非常缓慢，后来一查，原来是Apache耗掉了几乎所有资源，当时apache进程已有9XX个了。
于是用轻量级的web server替换掉apache就进入了日程表。这里顺带介绍一下这些可选的对象：


lighttpd
官方主页：[www.lighttpd.net](http://www.lighttpd.net/)
Lighttpd是一个德国人领导的开源软件，其根本的目的是提供一个专门针对高性能网站，安全、快速、兼容性好并且灵活的web server环境。具有非常低的内存开销，cpu占用率低，效能好，以及丰富的模块等特点。
lighttpd 是众多OpenSource轻量级的web server中较为优秀的一个。支持FastCGI, CGI, Auth, 输出压缩(output compress), URL重写, Alias等重要功能，而Apache之所以流行，很大程度也是因为功能丰富，在lighttpd上很多功能都有相应的实现了，这点对于apache的用 户是非常重要的，因为迁移到lighttpd就必须面对这些问题。
实用起来lighttpd确实非常不错，上文提到的apache overload的问题，用lighttpd就完全解决了。apache主要的问题是密集并发下，不断的fork()和切换，以及较高（相对于 lighttpd而言）的内存占用，使系统的资源几尽枯竭。而lighttpd采用了Multiplex技术，代码经过优化，体积非常小，资源占用很低， 而且反应速度相当快。
利用apache的rewrite技术，将繁重的cgi/fastcgi任务交给lighttpd来完成，充分利用两者的优点，现在那台服务器的负载下降了一个数量级，而且反应速度也提高了一个甚至是2个数量级！
下面是他们官方站点上的一段话：
"lightTPD is the BEST webserver out there, much faster than apache and much much much more flexible/configurable/secure than tux, and its fcgi php support is simply unbeatable."

thttpd
官方网站：[http://www.acme.com/software/thttpd/](http://www.acme.com/software/thttpd/)
thttpd 是一个非常小巧的轻量级web server，它非常非常简单，仅仅提供了HTTP/1.1和简单的CGI支持，在其官方网站上有一个与其他web server（如Apache, Zeus等）的对比图+Benchmark，可以参考参考。此外，thttpd 也类似于lighttpd，对于并发请求不使用fork()来派生子进程处理，而是采用多路复用(Multiplex)技术来实现。因此效能很好。
Thttpd 支持多种平台，如FreeBSD, SunOS, Solaris, BSD, Linux, OSF等。对于小型web server而言，速度快似乎是一个代名词，通过官方站提供的Benchmark，可以这样认为：thttpd至少和主流的web server一样快，在高负载下更快，因为其资源占用小的缘故。
Thttpd还有一个较为引人注目的特点：基于URL的文件流量限制，这对于下载的流量控制而言是非常方便的。象Apache就必须使用插件实现，效率较thttpd低。

shttpd
官方网站：[http://shttpd.sourceforge.net/](http://shttpd.sourceforge.net/) Shttpd是另一个轻量级的web server，具有比thttpd更丰富的功能特性，支持CGI, SSL, cookie, MD5认证, 还能嵌入(embedded)到现有的软件里。最有意思的是不需要配置文件！ 
由于shttpd可以嵌入其他软件，因此可以非常容易的开发嵌入式系统的web server，官方网站上称shttpd如果使用uclibc/dielibc(libc的简化子集)则开销将非常非常低。以下是其特点：

Stand-alone server, or embeddable into existing C/C++ program 
GET, POST, PUT, DELETE methods 
CGI 
SSL 
Digest (MD5) authorization 
Multiple (and user defineable) index files 
Directory listing 
Standard logging 
Cookies 
inetd mode 
User-defineable mime types 
No configuration files 
No external dependencies

由于shttpd可以轻松嵌入其他程序里，因此shttpd是较为理想的web server开发原形，开发人员可以基于shttpd开发出自己的webserver！








下面是我翻译的LiteSpeed Web Server官方的一篇文章。

图片请参见......

[http://www.litespeedtech.com/benchmark.html](http://www.litespeedtech.com/benchmark.html)
几种web server表现的对比：
被测试的web server 软件
测试环境
测试结果
 以小的静态文件为测试基准
 以CGI为基准
 以FAST CGI为基准
 以PHP脚本为基准：mod_php vs. FastCGI PHP
 以Perl脚本为基准：mod_perl vs. lsperld
结论。

被测试的web server 软件
下面几种web server都包含在这次测试中。
Apache 1.3.33 
Apache 2.0.52 (prefork mpm) 
IIS 6.0 
LiteSpeed 2.0 Standard 
LiteSpeed 2.0 Professional 
thttpd 2.25b 
TUX 3.2 
boa 0.94.14rc20 
lighttpd 1.3.12 
Aolserver 4.0.7 
Mathopd 1.5p3 
说明一下LSWS 是LiteSpeed Web Server的缩写。
本来也很想把zeus和 sunone也放到这次基准测试中来，但是那些软件的声明不允许对对它们的测试结果进行公布。

测试环境
Network Switch: D-Link Gigabit Eight Ports Switch DGS-1008D
Server Hardware: Dell PowerEdge SC1600
CPU: Single Intel Xeon 2.4GHz/533FSB/512KB L2 Cache 
memory: 256MB ECC PC2700 
Hard Drive: 36GB 10K RPM SCSI drive Seagate ST336607LW 
NIC: on board Intel PRO/1000 Gigabit Adapter 
Server Software: 
Fedora Core 3 with stock kernel 2.6.10-1.760 
IIS 6.0 on Windows 2003 Enterprise 
Web Server Configurations: 
Apache 1.3.33 
Apache 2.0.52 
IIS 6.0 - default 
LiteSpeed 2.0 Standard - default 
LiteSpeed 2.0 Professional - default 
thttpd 
TUX 3.2 - default 
boa 
lighttpd 
Aolserver 
Mathopd 
如果想知道他们当时的具体配置可以去原文中查看，Web Server Configurations: [http://www.litespeedtech.com/benchmark.html](http://www.litespeedtech.com/benchmark.html)  。

注意：LiteSpeed 和 lighttpd都支持在2.6内核中新的i/o 事件发送器sys_epoll()。但是有意思的是。我们发现这两种server基准测试的结果还没以前的标准时间发送器poll(),表现的好。
至少当我们通过网络测试并发等级<=1000的时候，我们仅仅用poll()来做我们所有的测试。
结果可能会和在本地机上通过回路测试完全不同。

Client Hardware: MSI K7D 
CPU: Dual Athlon MP 2000+/266FSB/256KB L2 Cache 
memory: 1GB PC2100 
Hard Drive: 36GB 1000RPM SCSI drive 
NIC: Intel PRO/1000 Gigabit Adapater 
Client Software: 
OS: RedHat 8.0 with kernel 2.4.22smp 
Test Tool: ApacheBench (ab) 1.3.33 
测试结果：
每种情况运行3次，最好的结果被采纳。服务器被重新启动，当每次测试完一种web server软件。
以小的静态文件为测试基准
小的静态页面大小只有100 bytes。用小文件测试的原因是为了避免网络带宽瓶颈。我们只为了展示web server的表现，而不是内核。
Non-KeepAlive test: ab -n 100000 -c <concurrent level> [http://<ip>:<port>/100.html](http://%3Cip%3E/100.html)



这儿必然有某种Anti-DoS特性在IIS 6.0中，当并发等级大于10（小于100）的时候它丢弃了所有的连接。

KeepAlive test: ab -n 100000 -c <concurrent level> -k [http://<ip>:<port>/100.html](http://%3Cip%3E/100.html)

CGI 基准测试
一个简单的c cgi脚本(echo)被用来在这里测试。它是C CGI/FCGI在fcgi 2.4包中 的例子。用来打印出所有的cgi环境变量。
CGI Test: ab -n 3000 -c <concurrent level> [http://<ip>:<port>/cgi-bin/echo](http://%3Cip%3E/cgi-bin/echo)


不知道为什么什么原因使thttpd's CGI在这表现如此差。
Fast CGI 基准测试
一个简单的c++ cgi脚本(echocpp) 被用来这里测试。它是一个fcgi 2.4包中修改版本的c++ FCGI例子。它被修改成只产生一个恒定的响应。这个响应是162 byte在响应体中（response body）。从请求到请求发生改变的部分被去掉了，因为ab（ApacheBench）不喜欢变长响应体的响应。
Fast CGI Test: ab -n 10000 -c <concurrent level> [http://<ip>:<port>/cgi-bin/echo](http://%3Cip%3E/cgi-bin/echo)


PHP脚本基准测试: mod_php vs. FastCGI PHP
两个简单的php脚本被用来测试。一个是hello.php只 输出hello world。另一个是phpinfo.php，只调用phpinfo()。这里都没有配置操作码加速器（opcode accelerator）。因为他们对这两个简单的php脚本没有太大的影响。
hello.php test: ab -n 10000 -c <concurrent level> [http://<ip>:<port>/hello.php](http://%3Cip%3E/hello.php)

phpinfo.php test: ab -n 10000 -c <concurrent level> [http://<ip>:<port>/phpinfo.php](http://%3Cip%3E/phpinfo.php)


Perl 脚本基准测试: mod_perl vs. lsperld
一个Hello World perl脚本，产生一个77 bytes的响应体。被用来测试。
Apache 2.0没有测。因为在同一个服务器上既为 1.3又为2.0安装mod_perl太麻烦了。
Apache 1.3 被配置成用 Apache::Registry去处理perl 脚本.
Lsperld 是一个Perl Fast CGI 脚本它可以运行CGI perl 脚本在一个 persistent Perl interpreter中, 就像 Apache mod_perl's Apache::Registry, 编译perl 脚本，cached 到内存中。

hello.pl test: AB -n 10000 -c <concurrent level> [http://<ip>:<port>/hello.pl](http://%3Cip%3E/hello.pl)




总结：
在以上的基准测试结果中我们得到了以下的结论。
Apache 1.3和 2.0的表现非常相近。2.0没有什么优势。我们没有测试Apache 2.0的worker MPM。因为prefork MPM更有可能被用于Unix/Linux系统的应用。跟Apache比起来, LiteSpeed Web Server能快3-7倍当它处理静态内容的时候。当处理动态内容的是时候能快50-100%，包括CGI, FastCGI, PHP and Perl.方式。
我们早期的测试表明，Apache的性能下降非常大，当distributed configuration override (.htaccess) 被设置为enabled。然而LiteSpeed web server几乎不受影响。所以我们将能看到更大的差距在一个共享的主机环境中。

跟这些纯粹的用户区（land） 内容加速器像thttpd, boa 和mathopd比较起来, LiteSpeed Web Server 在处理静态内容上有更好的表现。跟kernel内核内容加速器比较起来，TUX,由于在内核中的快捷和简单的特性设置。LiteSpeed Web Server Pro在non-keep-alive测试中表现有点逊色。
这全部的LiteSpeed web server的表现，包括Standard 和 Professional版本，比其他所有被测试的web server软件都好。


LiteSpeed Web Server 是一个多元（one-stop） 解决方案在高性能web服务环境中. 
如果你真的想你的网站停止遭受各种各样性能表现上的问题和在你现有的硬件上表现出最好的web服务的产出。你应该认真的考虑切换到LiteSpeed.。它将省去你所有在优化你的web各部分的时候所有头疼的事情。因为我们已经为你做完了所有艰难的工作。




我的总结：
如果只有静态页面和图片的话，可以用Zeus或者TUX，效率比apche高很多，尤其是TUX因为是和内核心结合的有较高的效率我认为是静态内容server的最佳选择，但他的限制是必须使用linux2.4以上的内核。我们的freebsd系统可能无法使用。

至于apache1.3和apache2.0对于理静态内容的比较，和worker MPM 与 prefork MPM性能方面的比较，我还没有一个权威的资料来说明。对于负载不是很高的站点，需要较高的稳定性的话，apache还是很好的选择。

LiteSpeed Web Server的介绍我在这里没有列出，从比较来看，他的综合性能是不错的。我可以给出一段他的描述：[http://www.litespeedtech.com/](http://www.litespeedtech.com/)
LiteSpeed web server is an Apache interchangeable, full-featured high performance, secure HTTP server specifically engineered from the ground up with security and scalability in mind.

lighttpd,thttpd,shttpd的稳定性还没有一个肯定的说法。
以后再对这个文档进行补充。翻译的不正确的地方欢迎指出，可以参看原文。









