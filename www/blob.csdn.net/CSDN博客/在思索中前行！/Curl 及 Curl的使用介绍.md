# Curl 及 Curl的使用介绍 - 在思索中前行！ - CSDN博客





2014年10月21日 17:28:05[_Tham](https://me.csdn.net/txl16211)阅读数：4369








## Curl 简介

Curl是Linux下一个很强大的http命令行工具，其功能十分强大。

1) 二话不说，先从这里开始吧！

$ curl http://www.linuxidc.com

回车之后，www.linuxidc.com 的html就稀里哗啦地显示在屏幕上了    ~

2) 嗯，要想把读过来页面存下来，是不是要这样呢？

$ curl http://www.linuxidc.com > page.html

当然可以，但不用这么麻烦的！

用curl的内置option就好，存下http的结果，用这个option: -o

$ curl -o page.html http://www.linuxidc.com

这样，你就可以看到屏幕上出现一个下载页面进度指示。等进展到100%，自然就 OK咯

3) 什么什么？！访问不到？肯定是你的proxy没有设定了。

使用curl的时候，用这个option可以指定http访问所使用的proxy服务器及其端口： -x

$ curl -x 123.45.67.89:1080 -o page.html http://www.linuxidc.com

4) 访问有些网站的时候比较讨厌，他使用cookie来记录session信息。

像IE/NN这样的浏览器，当然可以轻易处理cookie信息，但我们的curl呢？.....

我们来学习这个option: -D <— 这个是把http的response里面的cookie信息存到一个特别的文件中去

$ curl -x 123.45.67.89:1080 -o page.html -D cookie0001.txt http://www.linuxidc.com

这样，当页面被存到page.html的同时，cookie信息也被存到了cookie0001.txt里面了

5）那么，下一次访问的时候，如何继续使用上次留下的cookie信息呢？要知道，很多网站都是靠监视你的cookie信息，来判断你是不是不按规矩访问他们的网站的。

这次我们使用这个option来把上次的cookie信息追加到http request里面去： -b

$ curl -x 123.45.67.89:1080 -o page1.html -D cookie0002.txt -b cookie0001.txt http://www.linuxidc.com

这样，我们就可以几乎模拟所有的IE操作，去访问网页了！

6）稍微等等    ~我好像忘记什么了    ~

对了！是浏览器信息

有些讨厌的网站总要我们使用某些特定的浏览器去访问他们，有时候更过分的是，还要使用某些特定的版本     NND，哪里有时间为了它去找这些怪异的浏览器呢！？

好在curl给我们提供了一个有用的option，可以让我们随意指定自己这次访问所宣称的自己的浏览器信息： -A

$ curl -A "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)" -x 123.45.67.89:1080 -o page.html -D cookie0001.txt http://www.linuxidc.com

这样，服务器端接到访问的要求，会认为你是一个运行在Windows 2000上的 IE6.0，嘿嘿嘿，其实也许你用的是苹果机呢！

而"Mozilla/4.73 [en] (X11; U; Linux 2.2; 15 i686"则可以告诉对方你是一台 PC上跑着的Linux，用的是Netscape 4.73，呵呵呵

7）另外一个服务器端常用的限制方法，就是检查http访问的referer。比如你先访问首页，再访问里面所指定的下载页，这第二次访问的 referer地址就是第一次访问成功后的页面地址。这样，服务器端只要发现对下载页面某次访问的referer地址不是首页的地址，就可以断定那是个盗连了    ~

讨厌讨厌 ~我就是要盗连    ~！！

幸好curl给我们提供了设定referer的option： -e

$ curl -A "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)" -x 123.45.67.89:1080 -e "mail.linuxidc.com" -o page.html -D cookie0001.txt http://www.linuxidc.com

这样，就可以骗对方的服务器，你是从mail.linuxidc.com点击某个链接过来的了，呵呵呵

8）写着写着发现漏掉什么重要的东西了！——- 利用curl 下载文件

刚才讲过了，下载页面到一个文件里，可以使用 -o ，下载文件也是一样。比如，

$ curl -o 1.jpg http://cgi2.tky.3web.ne.jp/~zzh/screen1.JPG

这里教大家一个新的option： -O 大写的O，这么用：

$ curl -O http://cgi2.tky.3web.ne.jp/~zzh/screen1.JPG

这样，就可以按照服务器上的文件名，自动存在本地了！

再来一个更好用的。

如果screen1.JPG以外还有screen2.JPG、screen3.JPG、....、screen10.JPG需要下载，难不成还要让我们写一个script来完成这些操作？

不干！

在curl里面，这么写就可以了：

$ curl -O http://cgi2.tky.3web.ne.jp/~zzh/screen[1-10].JPG

呵呵呵，厉害吧？！ ~

9）再来，我们继续讲解下载！

$ curl -O http://cgi2.tky.3web.ne.jp/~{zzh,nick}/[001-201].JPG

这样产生的下载，就是

~zzh/001.JPG

~zzh/002.JPG

...

~zzh/201.JPG

~nick/001.JPG

~nick/002.JPG

...

~nick/201.JPG

够方便的了吧？哈哈哈

咦？高兴得太早了。

由于zzh/nick下的文件名都是001，002...，201，下载下来的文件重名，后面的把前面的文件都给覆盖掉了 ~

没关系，我们还有更狠的！

$ curl -o #2_#1.jpg http://cgi2.tky.3web.ne.jp/~{zzh,nick}/[001-201].JPG

—这是.....自定义文件名的下载？ —对头，呵呵！

这样，自定义出来下载下来的文件名，就变成了这样：原来： ~zzh/001.JPG —-> 下载后： 001-zzh.JPG 原来： ~nick/001.JPG —-> 下载后： 001-nick.JPG

这样一来，就不怕文件重名啦，呵呵

## [Windows下curl使用](http://blog.csdn.net/ichliebephone/article/details/6602537)


下面简单的来说下在Windows下同样的使用curl命令的方式。


二、具体实现

Windows下默认没有curl命令，需要安装后才能使用。



1.      到这里下载[http://curl.haxx.se/download/](http://curl.haxx.se/download/)文件[curl-7.17.1-win32-ssl.zip](http://curl.haxx.se/download/curl-7.17.1-win32-ssl.zip)，如果不需要使用ssl的话，可以直接下载[curl-7.17.1-win32-nossl.zip](http://curl.haxx.se/download/curl-7.17.1-win32-nossl.zip)，并且直接就可以使用。不过因为我们请求的谷歌网址是https格式的，所以我们要使用带ssl功能的。

2.      在Windows下为了使用带ssl的curl命令，我们得安装OpenSSL，到[http://www.slproweb.com/products/Win32OpenSSL.html](http://www.slproweb.com/products/Win32OpenSSL.html)这里下载[Win32OpenSSL
 v1.0.0d Light](http://www.slproweb.com/download/Win32OpenSSL_Light-1_0_0d.exe)，安装即可。也可以不用安装，直接把[libeay32.dll, ssleay32.dll, libssl32.dll](http://download.csdn.net/source/3438836)这三个dll文件拷到你的curl目录下即可。

3.      接下来打开cmd，切换到curl目录下，就可以直接使用curl命令了。




## [CURL常用命令](http://www.cnblogs.com/gbyukg/p/3326825.html)

**下载单个文件，默认将输出打印到标准输出中(STDOUT)中**

```
curl http://www.centos.org
```

**通过-o/-O选项保存下载的文件到指定的文件中：**

-o：将文件保存为命令行中指定的文件名的文件中

-O：使用URL中默认的文件名保存文件到本地

```
1 # 将文件下载到本地并命名为mygettext.html
2 curl -o mygettext.html http://www.gnu.org/software/gettext/manual/gettext.html
3 
4 # 将文件保存到本地并命名为gettext.html
5 curl -O http://www.gnu.org/software/gettext/manual/gettext.html
```

同样可以使用转向字符">"对输出进行转向输出

**同时获取多个文件**

```
1 curl -O URL1 -O URL2
```

若同时从同一站点下载多个文件时，curl会尝试重用链接(connection)。

通过-L选项进行重定向

默认情况下CURL不会发送HTTP Location headers(重定向).当一个被请求页面移动到另一个站点时，会发送一个HTTP Loaction header作为请求，然后将请求重定向到新的地址上。

例如：访问google.com时，会自动将地址重定向到google.com.hk上。

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 curl http://www.google.com
 2 <HTML>
 3 <HEAD>
 4     <meta http-equiv="content-type" content="text/html;charset=utf-8">
 5     <TITLE>302 Moved</TITLE>
 6 </HEAD>
 7 <BODY>
 8     <H1>302 Moved</H1>
 9     The document has moved
10     <A HREF="http://www.google.com.hk/url?sa=p&hl=zh-CN&pref=hkredirect&pval=yes&q=http://www.google.com.hk/&ust=1379402837567135amp;usg=AFQjCNF3o7umf3jyJpNDPuF7KTibavE4aA">here</A>.
11 </BODY>
12 </HTML>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

上述输出说明所请求的档案被转移到了http://www.google.com.hk。

这是可以通过使用-L选项进行强制重定向

```
1 # 让curl使用地址重定向，此时会查询google.com.hk站点
2 curl -L http://www.google.com
```

**断点续传**

通过使用-C选项可对大文件使用断点续传功能，如：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 # 当文件在下载完成之前结束该进程
2 $ curl -O http://www.gnu.org/software/gettext/manual/gettext.html
3 ##############             20.1%
4 
5 # 通过添加-C选项继续对该文件进行下载，已经下载过的文件不会被重新下载
6 curl -C - -O http://www.gnu.org/software/gettext/manual/gettext.html
7 ###############            21.1%

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

**对CURL使用网络限速**

通过--limit-rate选项对CURL的最大网络使用进行限制

```
1 # 下载速度最大不会超过1000B/second
2 
3 curl --limit-rate 1000B -O http://www.gnu.org/software/gettext/manual/gettext.html
```

下载指定时间内修改过的文件

当下载一个文件时，可对该文件的最后修改日期进行判断，如果该文件在指定日期内修改过，就进行下载，否则不下载。

该功能可通过使用-z选项来实现：

```
1 # 若yy.html文件在2011/12/21之后有过更新才会进行下载
2 curl -z 21-Dec-11 http://www.example.com/yy.html
```

**CURL授权**

在访问需要授权的页面时，可通过-u选项提供用户名和密码进行授权

```
1 curl -u username:password URL
2 
3 # 通常的做法是在命令行只输入用户名，之后会提示输入密码，这样可以保证在查看历史记录时不会将密码泄露
4 curl -u username URL
```

**从FTP服务器下载文件**

CURL同样支持FTP下载，若在url中指定的是某个文件路径而非具体的某个要下载的文件名，CURL则会列出该目录下的所有文件名而并非下载该目录下的所有文件

```
1 # 列出public_html下的所有文件夹和文件
2 curl -u ftpuser:ftppass -O ftp://ftp_server/public_html/
3 
4 # 下载xss.php文件
5 curl -u ftpuser:ftppass -O ftp://ftp_server/public_html/xss.php
```

**上传文件到FTP服务器**

通过 -T 选项可将指定的本地文件上传到FTP服务器上

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

# 将myfile.txt文件上传到服务器
curl -u ftpuser:ftppass -T myfile.txt ftp://ftp.testserver.com

# 同时上传多个文件
curl -u ftpuser:ftppass -T "{file1,file2}" ftp://ftp.testserver.com

# 从标准输入获取内容保存到服务器指定的文件中
curl -u ftpuser:ftppass -T - ftp://ftp.testserver.com/myfile_1.txt

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

**获取更多信息**

通过使用 -v 和 -trace获取更多的链接信息

**通过字典查询单词**

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 # 查询bash单词的含义
2 curl dict://dict.org/d:bash
3 
4 # 列出所有可用词典
5 curl dict://dict.org/show:db
6 
7 # 在foldoc词典中查询bash单词的含义
8 curl dict://dict.org/d:bash:foldoc

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

**为CURL设置代理**

-x 选项可以为CURL添加代理功能

```
1 # 指定代理主机和端口
2 curl -x proxysever.test.com:3128 http://google.co.in
```



**其他网站整理**

**保存与使用网站cookie信息**

```
1 # 将网站的cookies信息保存到sugarcookies文件中
2 curl -D sugarcookies http://localhost/sugarcrm/index.php
3 
4 # 使用上次保存的cookie信息
5 curl -b sugarcookies http://localhost/sugarcrm/index.php
```

**传递请求数据**

默认curl使用GET方式请求数据，这种方式下直接通过URL传递数据

可以通过 --data/-d 方式指定使用POST方式传递数据

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 # GET
2 curl -u username https://api.github.com/user?access_token=XXXXXXXXXX
3 
4 # POST
5 curl -u username --data "param1=value1¶m2=value" https://api.github.com
6 
7 # 也可以指定一个文件，将该文件中的内容当作数据传递给服务器端
8 curl --data @filename https://github.api.com/authorizations

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

注：默认情况下，通过POST方式传递过去的数据中若有特殊字符，首先需要将特殊字符转义在传递给服务器端，如value值中包含有空格，则需要先将空格转换成%20，如：

```
1 curl -d "value%201" http://hostname.com
```

在新版本的CURL中，提供了新的选项 --data-urlencode，通过该选项提供的参数会自动转义特殊字符。

```
1 curl --data-urlencode "value 1" http://hostname.com
```

除了使用GET和POST协议外，还可以通过 -X 选项指定其它协议，如：

```
1 curl -I -X DELETE https://api.github.cim
```

上传文件

```
1 curl --form "fileupload=@filename.txt" http://hostname/resource
```



[http://curl.haxx.se/docs/httpscripting.html](http://curl.haxx.se/docs/httpscripting.html)












