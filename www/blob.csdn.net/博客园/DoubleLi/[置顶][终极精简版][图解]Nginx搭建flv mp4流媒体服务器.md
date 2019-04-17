# [置顶][终极精简版][图解]Nginx搭建flv mp4流媒体服务器 - DoubleLi - 博客园






花了我接近3周，历经了重重问题，今日终于把流媒体服务器搞定，赶紧的写个博文以免忘记。。。

　　起初是跟着网上的一些教程来的，但是说的很不全面，一些东西也过时不用了（比如jwplayer老版本）。我这次是用的最新版**jwplayer6.8**，在配置上有很多不同的地方，也很坑，值得注意一下！在配置方面，我精简了很多，没有了那么多繁琐的配置项需要修改。

　　注意：本人是在虚拟机centos6.2系统下搭建的流媒体服务器，在win7主机上做[测试](http://lib.csdn.net/base/softwaretest)。

另，文章最后有下载地址，可下载搭建过程中所有用到的包和其他文件。

　　废话不说，从搭建服务器的准备工作开始：



一、准备工作，安装依赖包，缺一不可！（**推荐先用命令查看自己是否已经安装一下软件，确认没有再安装**）

-------------------------------------------------------------------------------------------------------------------------------------------------------

1.安装zlib

```
tar xzvf zlib-1.2.3.tar.gz　　#解压
      cd zlib-1.2.3　　#进入解压目录
      ./configure　　#配置
      make && make install　　#编译并安装
```



2.安装gcc-c++

```
yum -y install gcc-c++　　#会自动安装的，但需联网
```



4.安装pcre

```
tar zxvf pcre-7.9.tar.gz
　　 cd pcre-7.9
　　./configure --prefix=/usr/local/pcre　　#配置安装路径为/usr/local/pcre
　　 make && make install
```



5.安装 openssl openssl-devel

```
yum -y install openssl openssl-devel
```



二、安装yamdi，给flv添加关键帧用的。（老是想到“亚麻碟”，哈哈。。。）

-------------------------------------------------------------------------------------------------------------------------------------------------------

```
#先cd到某个目录下，用wget命令下载包

　　wget http://sourceforge.net/projects/yamdi/files/yamdi/1.4/yamdi-1.4.tar.gz/download

　　#安装yadmi
　　tar xzvf yamdi-1.4.tar.gz
　　cd yamdi-1.4
　　make && make install
```



三、安装Nginx服务器，并配置

-------------------------------------------------------------------------------------------------------------------------------------------------------

1.安装

```
groupadd www    #添加用户组www
    useradd -g www www    #向组添加用户www
    tar xzvf nginx-0.8.34.tar.gz    #解压包
    cd nginx-0.8.34    
    #此步骤重要,--with-http_flv_module和--with-http_ssl_module不可变动，其他自定义吧。
    ./configure --with-http_ssl_module --with-pcre=/opt/nginx/pcre-7.9 --with-zlib=/opt/nginx/zlib-1.2.3 --user=www --group=www --prefix=/opt/nginx --with-http_flv_module
    make && make install
```

2.配置

```
vim /opt/nginx/conf/nginx.conf    #编辑nginx配置文件
#找到server节点，参照一下代码简单变动一下，so easy!
server {
    listen       80;
    server_name  192.168.1.105;
    root    /opt/nginx/html/;
    limit_rate_after 5m;    #在flv视频文件下载了5M以后开始限速
    limit_rate 100k;         #速度限制为100K
    index   index.html;
    charset utf-8;

    #将.flv文件指向flv模块                
    location ~ \.flv {
        flv;
    }
                    
    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
        root   html;
    }
}
```

四、使用与测试

-------------------------------------------------------------------------------------------------------------------------------------------------------

1.为准备的flv文件添加关键帧

```
#参数 -i:input文件  -o:out文件
yamdi -i test1.flv -o test2.flv    #目录下会生成test2.flv，此时该文件已经生成了关键帧
```



2.拷贝test2.flv到服务器目录下，我的服务器目录是“/opt/nginx/html/”。

3.拷贝jwplayer.flash.swf到服务器目录下。

4.编辑网页，TestStreaming项目，（我是把网页部署到到win7主机apache服务器下的，用来模拟远程访问流媒体服务器，想在本机弄的，自己倒腾）

```
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
    <title></title>
    <script type="text/javascript" src="jwplayer.js"></script>
</head>
<body>
    <div id="myElement">Loading the player...</div>

    <script type="text/javascript">
        jwplayer("myElement").setup({
　　　　　　　//192.168.164.132是我虚拟机的ip，加载播放器文件
            flashplayer: "http://192.168.164.132/player6.swf",
　　　　　　  //加载服务器下，/video/test2.flv带关键帧的flv文件
            file: "http://192.168.164.132/video/test2.flv",
            image: "test.jpg",
            width: 800,
            height: 500,
            screencolor: "#BBBBBB",
            autostart: true,
            provider: "http",
            streamer: "start",
            stretching: "fill",
　　　　　　  //上面三个参数都不重要，下面两个参数才是最重要的！新版本奇葩之处！
            startparam: "start",
            primary: "flash"
        });
    </script>
</body>
</html>
```

5.测试，win7下打开浏览器，输入:localhost/TestStreaming/index.html



yeah！视频成功播放，并且可随意拖动seek！

若不能成功，请仔细检查jwplayer的配置参数！

附上下载链接：http://download.csdn[.NET](http://lib.csdn.net/base/dotnet)/detail/cyh970473/7443127

from:http://blog.csdn.net/cyh970473/article/details/50593227









