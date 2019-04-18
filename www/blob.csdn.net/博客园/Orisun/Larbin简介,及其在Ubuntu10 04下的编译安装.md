# Larbin简介,及其在Ubuntu10.04下的编译安装 - Orisun - 博客园







# [Larbin简介,及其在Ubuntu10.04下的编译安装](https://www.cnblogs.com/zhangchaoyang/articles/2031523.html)





Larbin是一种多功能的网络爬虫,一个法国人最初为

XYLEME project写的,当时只是为了获取网络上的XML文件.Larbin是非常灵活可订制的.

最新版本的Larbin在一台普通的PC上一天可爬到5,000,000个网页,当然这很大程序上依赖于你的网速.Larbin工作于linux上,并且依赖于一些标准库,比如adns.

下载一个larbin-2.6.3.tar.gz解压,进入目录,运行

./configure

make

进行编译安装.

一般会出很多问题,你需要提前完成以下工作:

1.
确认已经安装了build-essential程序包,最新的GCC编译器,已安装了内核程序包.

这一步一般都没问题,该装的都装了.

2.configure时会提示找不到makedepend命令,所以你需要安装一下


sudo apt-get install xutils-dev

3.编译的时候还会出一个问题,说从char*转换到byte*出错,这是由
/larbin-2.6.3/adns下internal.h中第568—571行代码引起的,直接把它注释掉即可.

当然也可以改一下

adns_status adns__parse_domain(adns_state ads, int serv, adns_query qu,

         vbuf *vb, parsedomain_flags flags,

          const byte *dgram, int dglen, int *cbyte_io, int max); 

修改为：

adns_status adns__parse_domain(adns_state ads, int serv, adns_query qu,

         vbuf *vb, adns_queryflags flags,

          const byte *dgram, int dglen, int *cbyte_io, int max);

改过之后还是会有警告,不过谢天谢地,已经不是error了.

4.make的时候会说找不到<iostream.h>,你需要把每个程序文件里的

<iostream.h>改为

<iostream>,然后加上using namespace std这句话.也可以有一种简单省力的方法:

到/usr/include/c++/下CP一份iostream文件到larbin的src目录下。并将其的名改为iostream.h.打开此文件添加一句using namespace std;

然后再./configure  make应该就没问题了.

运行　　./larbin

中止　　Ctrl+C　　　　　　　　

重起　　./larbin -scratch

注意停止larbin的时候不要用Ctrl+z,那样当你再次

./larbin或者

./larbin -scratch的时候,会端口已被占用,无法重新启动.

下面报一下我的两个配置文件里的内容

**larbin.conf**

httpPort 8099

inputPort 1976

pagesConnexions 100

dnsConnexions 5

waitDuration 60

startUrl http://www.baidu.com/

limitToDomain .com .cn .hk end

forbiddenExtensions.tar .gz .tgz .zip .Z .rpm .deb

.ps .dvi .pdf.png .jpg .jpeg .bmp .smi .tiff .gif

.mov .avi .mpeg .mpg .mp3 .qt .wav .ram .rm

.jar .java .class .diff

.doc .xls .ppt .mdb .rtf .exe .pps .so .psd

end

**options.h**

#ifndef LARBIN_CONFIG

#define LARBIN_CONFIG
#include "config.h"

#define SIMPLE_SAVE 

#define FOLLOW_LINKS

#define EXIT_AT_END

#define CGILEVEL 1

#define DEPTHBYSITE

#define RELOAD

#define CRASH

#endif

在/larbin-2.6.3/和

/larbin-2.6.3/src下都options.h文件,保持它们的内容是一致的.












