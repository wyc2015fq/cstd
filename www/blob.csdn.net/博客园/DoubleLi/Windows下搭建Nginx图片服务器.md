# Windows下搭建Nginx图片服务器 - DoubleLi - 博客园







在项目最开始，上传图片的时候，服务器先保存原图再使用`ImageMagick`生成上传图片缩略图，这种方法有很多缺点，例如生成的缩略图的大小是固定的，不能动态请求指定大小的缩略图。

虽然有非常多的图片云存储服务，例如 七牛、UpYun、Aliyun OSS 等，但是因为一些其他的考虑（例如：价格因素），我们最后还是选择本地存储。

通过调研，发现`nginx`可以动态请求缩略图。但是网上的教程都是针对Linux系统下的，几经周折终于在Windows系统下编译`nginx`并成功搭建`nginx`图片服务器。

点击[下载](http://nginx.org/en/download.html)进入`nginx`下载页，Windows系统可以直接点击`nginx/Windows-a.b.c`下载编译好的程序，解压之后即可使用，Linux系统需要点击`nginx-a.b.c`下载源码并编译后才能使用。

​

## 编译Nginx

`ngx_http_image_filter_module`模块是一个转换JPEG、GIF、PNG、WebP格式图片的过滤器，如果需要使用`ngx_http_image_filter_module`等非默认编译的模块，则需要重新编译`nginx`源码。

​

### 编译需求

在Microsoft Win32平台上构建`nginx`你需要：
- 微软 Visual C 编译器。
- [MSYS](http://www.mingw.org/wiki/MSYS) 。
- `Perl`，如果你想构建OpenSSL，让nginx支持SSL。例如 [ActivePerl](http://www.activestate.com/activeperl) 或 [Strawberry Perl](http://strawberryperl.com/) 。
- [Mercurial](https://www.mercurial-scm.org/) 客户端。
- [PCRE](http://www.pcre.org/) 、[zlib](http://zlib.net/) 和 [OpenSSL](http://www.openssl.org/) 库源码，注意这里是`PCRE`不是`PCRE2` 。
- [libgd](http://libgd.org/) ，编译`ngx_http_image_filter_module`模块依赖项。

​

### 编译

在开始构建之前确保`Perl`、`Mercurial`和`MSYS`的bin目录添加到`PATH`环境变量中。
- 
从 Visual C 目录运行`vcvarsall.bat`脚本用以设置 Visual C 运行环境。以管理员身份运行命令行工具，运行`vcvarsall.bat`脚本（根据你的实际路径）：
`"D:\Program Files\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"`
- 如果需要编译ngx_http_image_filter_module模块则需要编译`libgd`，编译方法详见[Building on Windows with Visual Studio 2015](https://github.com/libgd/libgd/tree/master/windows) 。
- 运行`msys.bat` ，本人使用的是`MSYS2` 。
- 
从`hg.nginx.org`库签出`nginx`源代码（这里会用到上面提到的`Mercurial`客户端）：
`$ hg clone http://hg.nginx.org/nginx`
- 
在下载的`nginx`目录中，创建一个用户构建的`objs`目录和用于存放第三方库源码的`lib`目录，解压`zlib` 、`PCRE`和`OpenSSL`库源码到`lib`目录

```
$ mkdir objs
$ mkdir objs/lib
$ cd objs/lib
$ tar -xzf ../../pcre-8.40.tar.gz
$ tar -xzf ../../zlib-1.2.11.tar.gz
$ tar -xzf ../../openssl-1.0.2k.tar.gz
```

- 
执行`which perl`命令，确保使用的`Perl`是上面下载安装的而不是`MSYS`自带的，执行`which link`命令，确保使用的`link.exe`是Visual自带的。将`MSYS`环境下的`perl.exre`和`link.exre`分别重命名为`perl_UNUSED.exe`和`ink_UNUSED.exe` 。可以使用下面的命令查看、修改环境变量。

```
$ echo $PATH
$ export PATH=/c/Perl/bin:$PATH
$ export PATH="/d/Program Files/Microsoft Visual Studio 14.0/VC/bin":$PATH
```

- 
如果要编译`ngx_http_image_filter_module`模块则打开nginx源码目录下的`auto/feature`文件，将

```
# -x Linux系统下检查文件是否有执行权限，Windows系统下不适用
if [ -x $NGX_AUTOTEST ]; then
```
- 修改为
`if [ YES ]; then`
- 
如果要编译`ngx_http_image_filter_module`模块则打开nginx源码目录下的`auto/lib/libgd/conf`文件，在适当的位置添加下面几行代码

```
ngx_feature="GD library"
    ngx_feature_name=
    ngx_feature_run=no
    ngx_feature_incs="#include <gd.h>"
    ngx_feature_path=
    ngx_feature_libs="-lgd"
    ngx_feature_test="gdImagePtr img = gdImageCreateFromGifPtr(1, NULL);"
    . auto/feature
    
# 添加下面的代码片段
if [ $ngx_found = no ]; then

    ngx_feature="GD library in custom path"
    # 指定libgd头文件位置
    ngx_feature_path="/e/thirdparty_source/libgd-2.2.4/src"
    # 指定libgd库文件位置和库文件
    ngx_feature_libs="-L/e/thirdparty_source/gdbuild -lgd"
    . auto/feature
    
fi
```

- 
运行`configure`脚本：

```
$ auto/configure --with-cc=cl --builddir=objs --prefix= \
--conf-path=conf/nginx.conf --pid-path=logs/nginx.pid \
--http-log-path=logs/access.log --error-log-path=logs/error.log \
--sbin-path=nginx.exe --http-client-body-temp-path=temp/client_body_temp \
--http-proxy-temp-path=temp/proxy_temp --http-fastcgi-temp-path=temp/fastcgi_temp \
--with-cc-opt="-DFD_SETSIZE=1024 -Ie:/thirdparty_source/libgd-2.2.4/src" \
--with-pcre=objs/lib/pcre-8.40 --with-zlib=objs/lib/zlib-1.2.11 \
--with-openssl=objs/lib/openssl-1.1.0d --with-select_module --with-http_ssl_module \
--with-http_image_filter_module --with-ld-opt="E:/thirdparty_source/gdbuild/libgd.lib"
```

- 在Visual 2015命令行工具中执行`nmake -f objs/Makefile`命令完成编译，编译成功后生成`nginx.exe`文件。

如果编译`openssl`时出现` error A2070:invalid instruction operands`错误，则可能是因为是因为`openssl-1.1.0`之前版本（使用汇编）编译时需要`NASM` ，最简单的方法是使用`openssl-1.1.0`以上版本重新配置编译。

​

## Nginx的使用

可在命令行中通过命令操作nginx：

```
$ start nginx        # 运行nginx
$ nginx -s stop      # 停止nginx
$ nginx -s reload    # 重新加载配置文件(如修改配置文件后，可通过该命令重新加载)
$ nginx -s quit      # 退出nginx
$ nginx -v           # 查看nginx版本
```

更多帮助信息可以查看 [nginx for Windows](http://nginx.org/en/docs/windows.html) 和 [Beginner’s Guide](http://nginx.org/en/docs/beginners_guide.html) 。

​

## 配置图片服务器

配置图片服务器的步骤如下：
- 在本地建了一个文件夹，例如：`H:/www/imgs`，里面放了几张测试图片。
- 
在`nginx/html`下创建一个名为`imgs`文件夹（实际访问的不是这个路径，经测试此步在`Windows`下非必要）
`mkdir $(nginx-dir)/html/imgs`
- 
修改`$(nginx-dir)/conf/nginx.conf`在默认的`server`节点里再添加一个`location`节点并指定实际路径，修改后的配置文件部分内容如下：

```
http {
    ...
    server {
        listen       80;
        server_name  localhost;

        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        location / {
            root   html;
            index  index.html index.htm;
        }
        
        # 设置图片目录
        location /imgs/ {
            root       H:/www;   # 路径“H:/www”下必须含有一个“imgs”目录
            autoindex  on;       # 如果为on，那么就发送请求地址对应的服务端路径下的文件列表给客户端
        }
        
        # 图片剪裁 crop
        # 按比例缩放 resize
        # 访问链接 http://t.cn/imgs_thumb/20170214152753.jpg?type=crop&width=100&height=100
        location ~* /imgs_thumb/(.+)$ {
            set $filename $1;
            
            if ($filename = "") {
                break;    # 同return 404;
            }
        
            set $img_type  $arg_type;    
            set $img_w     $arg_width;
            set $img_h     $arg_height;
            
            if ($img_type = "") {
                return 400; 
            }
            
            if ($img_w = "") {
                return 400; 
            }
            
            if ($img_h = "") {
                return 400; 
            }
            
            rewrite ^ /imgs_$img_type;
        }
        
        # 缩放图片的处理
        location /imgs_resize/ {
            alias  H:/www/imgs/$filename;
            image_filter resize $img_w $img_h;
        }
        
        # 剪裁图片的处理
        location /imgs_crop/ {
            alias  H:/www/imgs/$filename;
            image_filter crop $img_w $img_h;
        }
        
        # 设置视频目录
        location /videos {
            root       H:/www;
            autoindex  on;
        }
    }
}
```

- 修改完后需要重新启动nginx或重新加载配置文件即可。

​

推荐教程：
[Nginx开发从入门到精通](http://tengine.taobao.org/book/)

参考教程：
[Nginx开发从入门到精通](http://tengine.taobao.org/book/)
[Module ngx_http_image_filter_module](http://nginx.org/en/docs/http/ngx_http_image_filter_module.html)
[Nginx图片剪裁模块探究](http://cwtea.blog.51cto.com/4500217/1333142)
[Building nginx on the Win32 platform with Visual C](http://nginx.org/en/docs/howto_build_on_win32.html)
[Compile nginx with visual studio](http://stackoverflow.com/questions/21486482/compile-nginx-with-visual-studio)
[nginx架构详解(50%)](http://tengine.taobao.org/book/chapter_09.html)
[Linux下nginx编译安装教程和编译参数详解](http://www.jb51.net/article/49479.htm)
[Nginx图片剪裁模块探究 http_image_filter_module](http://cwtea.blog.51cto.com/4500217/1333142)
[利用 Nginx 的 ngx_http_image_filter_module 做实时的图片缩略图](https://ruby-china.org/topics/31498)










