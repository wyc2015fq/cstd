# ubuntu17下用nginx搭建HLS直播服务器 - 枫林港的博客 - CSDN博客





2018年07月26日 22:48:24[枫林港](https://me.csdn.net/u013601132)阅读数：405








# ubuntu下用nginx搭建HLS直播服务器



- [ubuntu下用nginx搭建HLS直播服务器](#ubuntu下用nginx搭建hls直播服务器)- [编译nginx](#编译nginx)
- [nginx启动/退出](#nginx启动退出)- [1. nginx退出命令](#1-nginx退出命令)
- [2. nginx启动命令](#2-nginx启动命令)
- [3. nginx平滑重启](#3-nginx平滑重启)
- [4. 启动报错](#4-启动报错)


- [nginx配置](#nginx配置)- [1. 常用配置](#1-常用配置)- [2. 浏览目录下的文件](#2-浏览目录下的文件)
- [3. HLS直播](#3-hls直播)- [1>、application myapp](#1application-myapp)
- [2>、application hls](#2application-hls)
- [3>、location /test1](#3location-test1)


- [2. 转码](#2-转码)
- [3. HLS多码率](#3-hls多码率)
- [4. ffmpeg单独转码_重编码](#4-ffmpeg单独转码重编码)
- [5. ffmpeg单独转码_非重编码](#5-ffmpeg单独转码非重编码)





## 编译nginx

在ubuntu 17.4下编译nginx。

需要预先安装的软件：

```bash
sudo apt-get install build-essential  libtool libpcre3 libpcre3-dev libpcrecpp0 libssl-dev zlib1g-dev

sudo apt-get install libxml2 libxml2-dev libxml2-utils  libxslt1-dbg libxslt1-dev  libxslt1.1
```

解决编译错误：configure: error: the HTTP image filter module requires the GD library.

```bash
sudo apt-get install libgd2-xpm libgd2-xpm-dev
```

解决编译错误：configure: error: the HTTP rewrite module requires the PCRE library.

```bash
sudo apt-get install libgeoip-dev
```

解决编译错误：configure: error: the GeoIP module requires the GeoIP library.

安装ffmpeg(或者可以自己编译):

```bash
sudo apt-get install software-properties-common

sudo add-apt-repository ppa:kirillshkrogalev/ffmpeg-next

sudo apt-get update

sudo apt-get install ffmpeg
```

下载nginx-rtmp-module-1.1.11、openssl-OpenSSL_1_0_1e、pcre-8.40、zlib-1.2.8，都放在nginx-1.13.0/lib_src目录下。 

从[https://nginx.org/en/download.html](https://nginx.org/en/download.html)下载1.13.0（20170510），在nginx-1.13.0目录下执行configure和make，configure脚本如下：

```bash
#!/bin/sh

./configure \

--with-cc-opt='-g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security -D_FORTIFY_SOURCE=2'   \

--with-ld-opt='-Wl,-Bsymbolic-functions -Wl,-z,relro'  \

--prefix=$(pwd)/output --conf-path=/etc/nginx/nginx.conf \

--http-log-path=/var/log/nginx/access.log --error-log-path=/var/log/nginx/error.log \

--lock-path=/var/lock/nginx.lock --pid-path=/run/nginx.pid \

--http-client-body-temp-path=/var/lib/nginx/body \

--http-fastcgi-temp-path=/var/lib/nginx/fastcgi \

--http-proxy-temp-path=/var/lib/nginx/proxy \

--http-scgi-temp-path=/var/lib/nginx/scgi \

--http-uwsgi-temp-path=/var/lib/nginx/uwsgi \

--with-debug --with-pcre-jit \

--with-http_ssl_module --with-http_stub_status_module \

--with-http_realip_module --with-http_addition_module \

--with-http_dav_module --with-http_geoip_module \

--with-http_gzip_static_module --with-http_image_filter_module \

--with-http_sub_module \

--with-http_xslt_module --with-mail --with-mail_ssl_module \

--with-pcre=$(pwd)/lib_src/pcre-8.40 \

--with-zlib=$(pwd)/lib_src/zlib-1.2.8 \

--with-openssl=$(pwd)/lib_src/openssl-OpenSSL_1_0_1e \

--add-module=$(pwd)/lib_src/nginx-rtmp-module-1.1.11 \

--with-debug
```

编译命令：

```bash
make; sudo make install
```

执行make的时候，会去编译lib_src下面的目录。make install后二进制文件会存放在nginx-1.13.0 /output目录下。

## nginx启动/退出

### 1. nginx退出命令

退出前先要停止ffmpeg推流，然后执行下面的命令就退出了。

```bash
sudo /home/AA/nginx/nginx-1.13.0/output/sbin/nginx -s quit
```

### 2. nginx启动命令

```bash
sudo /home/AA/nginx/nginx-1.13.0/output/sbin/nginx
```

nginx启动后，就可以用ffmpeg推流了。 

成功启动后可以用`ps -ef`看到nginx的进程：

```
root     17558     1  0 11:25 ?        00:00:00 nginx: master process /home/AA/nginx/nginx-1.13.0/output/sb

AA 17559 17558  1 11:25 ?        00:00:00 nginx: worker process

AA 17560 17558  1 11:25 ?        00:00:00 nginx: worker process

AA 17561 17558  1 11:25 ?        00:00:00 nginx: worker process

AA 17562 17558  1 11:25 ?        00:00:00 nginx: worker process
```

### 3. nginx平滑重启

如果更改了配置后要重启Nginx，可以用平滑重启： 

先确认配置没有问题：

> 
AA@ubuntu17:/etc/nginx$`sudo /home/AA/nginx/nginx-1.13.0/output/sbin/nginx -t`

  nginx: the configuration file /etc/nginx/nginx.conf syntax is ok 

  nginx: configuration file /etc/nginx/nginx.conf test is successful 

  AA@ubuntu17:/etc/nginx$ 
/etc/nginx/nginx.conf是默认配置文件，可以用-c参数指定配置文件：

```bash
sudo nginx -t -c /usr/nginx/conf/nginx.conf
```

查询nginx主进程号

```bash
ps -ef | grep nginx
```

在进程列表里 面找master进程，它的编号就是主进程号了，如下所示的17558就是主进程号，即master进程了。

> 
root     17558     1  0 11:25 ?        00:00:00 nginx: master process /home/AA/nginx/nginx-1.13.0/output/sb


平滑重启：

```bash
sudo kill -HUP 主进程号
```

或者使用：

```bash
sudo /home/AA/nginx/nginx-1.13.0/output/sbin/nginx -s reload
```

### 4. 启动报错

> 
nginx: [emerg] mkdir() “/var/lib/nginx/body” failed (2: No such file or directory)


解决办法：

```bash
sudo mkdir -p /var/lib/nginx/body; sudo mkdir -p /var/lib/nginx/proxy; sudo mkdir -p /var/lib/nginx/fastcgi; sudo mkdir -p /var/lib/nginx/uwsgi; sudo mkdir -p /var/lib/nginx/sc
```

# nginx配置

`下面描述中的nginx_server_IP，是指nginx服务器的IP地址，实际应用的时候要换成具体的IP地址。`

## 1. 常用配置

/etc/nginx/nginx.conf里面将user配置改成自己的用户名，这样nginx生成的目录和文件自己都可以查看，不然很不方便。

`hls`：值on｜off  切换hls  
`hls_path`:  设置播放列表(m3u8)和媒体块的位置，经过nginx-rtmp-module-1.1.11分割编码的hls ts流文件会放在这个目录下。 
`hls_fragment`: 后面接时间，用来设置每一个块的大小，即每个TS分片文件的时间长度。默认是5秒，只能为整数。设置太小如2s，vlc播放可能会失败，看nginx打印，vlc还没有开始读分片文件，分片文件被删除了。 
`hls_playlist_length`:  设置播放列表的长度，即所有分片的时间长度总和，单位是秒。 
`hls_sync`:  音视频的同步时间 
`hls_continuous`:  on|off 设置连续模式，是从停止播放的点开始还是直接跳过 
`hls_nested`:  on｜off 默认是off。打开后的作用是每条流自己有一个文件夹 
`hls_base_url`: 设置基准URL，对于m3u8中使用相对URL有效 
`hls_cleanup`:  on｜off 默认是开着的，是否删除列表中没有列出的TS分片文件。即最先出现的TS分片文件会逐步删除。 
`hls_fragment_naming`: sequential（使用增长式的整数命名） | timestamp（使用媒体块的时间戳） | system（使用系统时间戳）命名方式  
`hls_fragment_naming_granularity`: 如果使用时间戳命名时时间戳的精度 
`hls_fragment_slicing`:  plain（媒体块达到需要的duration就换）｜aligned（当到达的数据库块时几个duration） 

启动命令：sudo rm -rf /var/log/nginx/*  ; sudo ./nginx/nginx-1.13.0/output/sbin/nginx 

没有将其加入系统服务。每次启动都删除log文件，方便查看log。
### 2. 浏览目录下的文件

配置如下：

```
location / {   
  root /data/www/file                  //指定实际目录绝对路径 
  autoindex on;                             //开启目录浏览功能 
  autoindex_exact_size off;        //关闭详细文件大小统计，让文件大小显示MB，GB单位，默认为b 
  autoindex_localtime on;         //开启以服务器本地时区显示文件修改日期
}
```

### 3. HLS直播

搭建hls直播环境，环境如下图所示：

在码流服务器上用OBS或FFMPEG循环给nginx发送rtmp码流，必须是rtmp的，因为nginx-rtmp-module模块只能处理rtmp码流。码流服务器和nginx可以在同一个物理设备上。nginx收到码流后进行分片，最后由客户端用vlc播放，也可以是其他设备如移动终端。

```
rtmp {
        server {
            listen 1935;

            application myapp {
                live on;
            }
            application hls {
                live on;
                hls on;
                hls_path /home/AA/hls/test1;
                hls_nested on;
                hls_fragment 5s;
                hls_playlist_length 15s;
                hls_variant _hi  BANDWIDTH=640000;
            }
        }
    }
    http {
        sendfile on;
        tcp_nopush on;
        tcp_nodelay on;
        keepalive_timeout 65;
        types_hash_max_size 2048;
        # server_tokens off;

        include /etc/nginx/mime.types;
        default_type application/octet-stream;

        access_log /var/log/nginx/access.log;
        error_log /var/log/nginx/error.log debug;

        gzip on;
        gzip_disable "msie6";

    server {  
        listen       80;  
        server_name  localhost;  
        location / {  
            root   html;  
            index  index.html index.htm;  
        }  
        error_page   500 502 503 504  /50x.html;  
        location = /50x.html {  
            root   html;  
        }  
        #hls 
        location /test1 {
            types {
                application/vnd.apple.mpegusr m3u8;
                video/mp2t ts;
            }
            root /home/AA/hls;
            add_header Cache-Control no-cache;
        }
     }
```

实现这个功能，需要在原始的nginx.conf上家两个配置，一个是rtmp段，如前面所示：

#### 1>、application myapp

配置rtmp直播，ffmpeg在发码流的时候，命令如下：

```bash
ffmpeg -re -fflags +genpts -stream_loop -1 -i "Jurassic Park_1min.mp4" -vcodec libx264 -vprofile main -acodec aac -ar 44100 -strict -2 -ac 1 -f flv -s 640x480 -q 10 rtmp://nginx_server_IP:1935/myapp
```

最关键的是最后的rtmp直播地址，1935是配置里面的listen 1935端口号，myapp对应于application后面的myapp。

```
application myapp {
            live on;
        }
```

**ffmpeg参数说明**：

`-re`：以原始片源Jurassic Park_1min.mp4的码率来读码流数据，以避免ffmpeg编码过快。

`-fflags +genpts`：重新产生PTS，在片源循环编码的时候需要设置。不然循环编码出来的码流在原始片源首尾相接的地方，PTS会出问题。 
`-stream_loop -1`：针对这个文件循环编码。

ffmpeg在编码前，`nginx必须先启动`，否则ffmpeg无法绑定socket。 

vlc播放这个`rtmp直播`的链接为：

> 
rtmp://nginx_server_IP:1935/myapp


遇到vlc播放rtmp直播经常失败的问题，主要是双网卡影响的，禁掉另外一个网段的网卡再播放就成功了。

#### 2>、application hls

配置`hls直播`，ffmpeg在发码流的时候，命令如下：

```bash
ffmpeg -re -fflags +genpts -stream_loop -1 -i "./Jurassic Park_1min.mp4" -vcodec libx264 -vprofile main -acodec aac -ar 44100 -strict -2 -ac 1 -f flv -s 640x480 -q 10 rtmp://nginx_server_IP:1935/ hls
```

也是用rtmp发流，ffmpeg命令最后的流地址里面，hls对应于application hls。

```
application hls {
                live on;
                hls on;
                hls_path /home/AA/hls/test1;
                hls_nested on;
                hls_fragment 5s;
                hls_playlist_length 15s;
                hls_variant _hi  BANDWIDTH=640000;
            }
```

配置里面hls_path，是nginx-rtmp-module模块用来存放hls分片文件的目录。

注意：用sudo ./sbin/nginx -s quit命令停止nginx的时候，先要停止ffmpeg发rtmp流，否则nginx进程不会退出，而且其会继续产生hls分片。

#### 3>、location /test1

`location /test1`：用于配置用户访问的hls服务。 
`root /home/AA/hls;` ：hls直播的根目录。和前面rtmp配置里面的`hls_path`对应。 

那么vlc播放这个`hls直播`的链接为：[http://nginx_server_IP/test1/index.m3u8](http://nginx_server_IP/test1/index.m3u8)，此index.m3u8位于服务器/home/AA/hls/test1目录下。 

播放链接里面test1对应于location /test1。
## 2. 转码

nginx-rtmp-module模块可以调用ffmpeg来做转码。配置如下：

```
rtmp {    
        server {    
            listen 1935;    

            application myapp {    
                live on; 
                exec /usr/bin/ffmpeg -i rtmp://localhost:1935/myapp -c:a copy  -c:v libx264 -b:v 312K -g 30 -f flv -s 1280x720 rtmp://localhost:1935/hls >/home/AA/ffmpeg.log ;   
            }    
            application hls {    
                live on;    
                hls on;    
                hls_path /home/AA/hls/test1;  
                hls_nested on;
                hls_fragment 5s;
                hls_playlist_length 15s;
                hls_variant _hi  BANDWIDTH=640000;  
            }    
        }    
    }

http {
        sendfile on;
        tcp_nopush on;
        tcp_nodelay on;
        keepalive_timeout 65;
        types_hash_max_size 2048;

        include /etc/nginx/mime.types;
        default_type application/octet-stream;
        access_log /var/log/nginx/access.log;
        error_log /var/log/nginx/error.log debug;
        gzip on;
        gzip_disable "msie6";

    server {  
        listen       80;  
        server_name  localhost;  

        location / {  
            root   html;  
            index  index.html index.htm;  
        }  
        error_page   500 502 503 504  /50x.html;  
        location = /50x.html {  
            root   html;  
        }  
        #hls 
        location /test1 {
            types {
                application/vnd.apple.mpegusr m3u8;
                video/mp2t ts;
            }
            root /home/AA/hls;
            add_header Cache-Control no-cache;
        }
     } 
        include /etc/nginx/conf.d/*.conf;
        include /etc/nginx/sites-enabled/*;
}
```

最主要的改动是application myapp这节，增加了ffmpeg命令：

```bash
exec /usr/bin/ffmpeg -i rtmp://localhost:1935/myapp -c:a copy  -c:v libx264 -b:v 312K -g 30 -f flv -s 1280x720 rtmp://localhost:1935/hls ;
```

这个命令以myapp为输入码流，转码后输出到rtmp://localhost:1935/hls，再由nginx-rtmp-module模块切片成hls。转码成功的话，在/home/AA/hls/test1下面会产生分片文件。

码流服务器推rtmp直播流(不需要推流到rtmp://nginx_server_IP:1935/ hls上面)：

```bash
ffmpeg -re -fflags +genpts -stream_loop -1 -i "Jurassic Park_1min.mp4" -vcodec libx264 -vprofile main -acodec aac -ar 44100 -strict -2 -ac 1 -f flv -s 640x480 -q 10 rtmp://nginx_server_IP:1935/myapp
```

vlc播放这个直播的链接为：

> 
[http://nginx_server_IP/test1/index.m3u8](http://nginx_server_IP/test1/index.m3u8)


刚开始调的时候发现经常没有发布成功，主要是exec /usr/bin/ffmpeg这个进程收不到码流，也没有看到错误打印，/var/log/nginx/error.log里面也没有提示。后来发现先用ffmpeg推流到myapp，nginx_server收到码流后就会启动/usr/bin/ffmpeg，用ps -ef可以看到。10s后停止推流，然后再启动推流，这样就能规避这个问题。

后面描述的HLS多码率，中间直接用ffmpeg产生多码率hls分片时，也是先启动ffmpeg转码，10s后再启动ffmpeg推流。

## 3. HLS多码率

用hls_variant参数来实现。 

rtmp exec命令+ffmpeg转码 

在rtmp配置里面加上：
```
application rtmp2 {
               live on;
               exec /usr/bin/ffmpeg -i rtmp://localhost/rtmp2/$name
                 -c:a aac -strict -2 -b:a 32k  -c:v libx264 -b:v 128K -f flv rtmp://localhost/hls2/$name_low
                 -c:a aac -strict -2 -b:a 64k  -c:v libx264 -b:v 256k -f flv rtmp://localhost/hls2/$name_mid
                 -c:a aac -strict -2 -b:a 128k -c:v libx264 -b:v 512K -f flv rtmp://localhost/hls2/$name_hi;
           }
           application hls2 {
               live on;
               hls on;
               hls_path /home/AA/hls/test2;
               hls_nested on;
               hls_variant _low BANDWIDTH=160000;
               hls_variant _mid BANDWIDTH=320000;
               hls_variant _hi  BANDWIDTH=640000;
           }
```

在http里面加配置：

```
#hls2
        location /test2 {
            types {
                application/vnd.apple.mpegusr m3u8;
                video/mp2t ts;
            }
            root /home/AA/hls;
            add_header Cache-Control no-cache;
        }
```

ffmpeg推流到rtmp2上：

```bash
ffmpeg -re -fflags +genpts -stream_loop -1 -i "CH100.ts" -vcodec copy -acodec aac -ar 44100 -strict -2 -ac 1 -f flv rtmp://nginx_server_IP/rtmp2/CH100
```

这里推流的地址是rtmp://nginx_server_IP:1935/rtmp2/CH100，那么配置里面的`$name`就是CH100。这个对应关系要搞清楚，不然会失败。

和前面描述的一样，用这个命令推流，10s后停止推流，然后再启动推流。成功转码的话会产生下列的文件：

> 
AA@ubuntu17:~$ sudo ls -l /home/AA/hls/test2/    drwxr–r– 2 www-data www-data 4096  51 16:58 CH100_hi    drwxr–r– 2 www-data www-data 4096  51 16:58 CH100_low    -rw-r–r– 1 www-data www-data  231  51 16:58 CH100.m3u8    drwxr–r– 2 www-data www-data 4096  51 16:58 CH100_mid    AA@ubuntu17:~$ sudo ls -l /home/AA/hls/test2/CH100_low 

  -rw-r–r– 1 www-data www-data 181984  51 17:01 17.ts 

  -rw-r–r– 1 www-data www-data 260944  51 17:01 18.ts 

  -rw-r–r– 1 www-data www-data 136864  51 17:01 19.ts 

  -rw-r–r– 1 www-data www-data 260380  51 17:01 20.ts 

  -rw-r–r– 1 www-data www-data 187624  51 17:02 21.ts 

  -rw-r–r– 1 www-data www-data  76704  51 17:02 22.ts 

  -rw-r–r– 1 www-data www-data    203  51 17:02 index.m3u8
此情况下vlc播放这个直播的链接为：

> 
[http://nginx_server_IP/test2/CH100.m3u8](http://nginx_server_IP/test2/CH100.m3u8)


也可以直接播放某个目录下的：

> 
[http://nginx_server_IP/test2/CH100_low/index.m3u8](http://nginx_server_IP/test2/CH100_low/index.m3u8)


## 4. ffmpeg单独转码_重编码

前面描述的是rtmp用exec启动ffmpeg来实现转码和多码率。其实也可以直接用ffmpeg来做转码。配置如下：

```
application rtmp2 {
               live on;
           }
           application hls2 {
               live on;
               hls on;
               hls_path /home/AA/hls/test2;
               hls_nested on;
               hls_variant _low BANDWIDTH=160000;
               hls_variant _mid BANDWIDTH=320000;
               hls_variant _hi  BANDWIDTH=640000;
           }
```

即不需要exec命令，直接用ffmpeg命令来做转码：

```bash
ffmpeg -i rtmp://nginx_server_IP/rtmp2/CH100 \
   -c:a aac -strict -2 -b:a 32k  -c:v libx264 -b:v 128K -f flv rtmp://nginx_server_IP/hls2/CH100_low \
   -c:a aac -strict -2 -b:a 64k  -c:v libx264 -b:v 256k -f flv rtmp://nginx_server_IP/hls2/CH100_mid \
   -c:a aac -strict -2 -b:a 128k -c:v libx264 -b:v 512K -f flv rtmp://nginx_server_IP/hls2/CH100_hi
```

即此ffmpeg命令接收rtmp://nginx_server_IP/rtmp2/CH100作为输入码流，转码后输出到rtmp://nginx_server_IP/hls2，nginx_server收到后由nginx-rtmp-module模块进行分片并产生.m3u8文件。

和前面描述的一样，用这个命令推流，10s后停止推流，然后再启动推流。此ffmpeg命令既可以在nginx_server上执行，也可以是另外的独立设备。

此情况下vlc播放这个直播的链接为：

> 
[http://nginx_server_IP/test2/CH100.m3u8](http://nginx_server_IP/test2/CH100.m3u8)


也可以直接播放某个目录下的：

> 
[http://nginx_server_IP/test2/CH100_low/index.m3u8](http://nginx_server_IP/test2/CH100_low/index.m3u8)


## 5. ffmpeg单独转码_非重编码

上面重编码的方法很耗cpu，intel i5-6500 3.2GHz cpu上，此命令占100% cpu，画面看上去还流畅。

平常可以用文件来做直播，可以先将文件预先编码成对应码率的文件，然后用ffmpeg循环发送此文件到rtmp服务器上，然后由nginx-rtmp-module模块进行分片并产生.m3u8文件。ffmpeg发送码流cpu很低。

配置和前面类似，在rtmp配置里面加上：

```
application rtmp2 {
               live on;
           }
           application hls2 {
               live on;
               hls on;
               hls_path /home/AA/hls/test2;
               hls_nested on;
               hls_variant _low BANDWIDTH=160000;
               hls_variant _mid BANDWIDTH=320000;
               hls_variant _hi  BANDWIDTH=640000;
           }
```

在http里面加配置：

```
#hls2
        location /test2 {
            types {
                application/vnd.apple.mpegusr m3u8;
                video/mp2t ts;
            }
            root /home/AA/hls;
            add_header Cache-Control no-cache;
        }
```

ffmpeg推流到hls2上，需要推3路：

```bash
ffmpeg -re -fflags +genpts -stream_loop -1 -i "CH100 _160K.ts" -vcodec copy -c:a aac -strict -2 -b:a 32k  -f flv rtmp://localhost/hls2/CH100_low

ffmpeg -re -fflags +genpts -stream_loop -1 -i "CH100 _320K.ts" -vcodec copy -c:a aac -strict -2 -b:a 64k  -f flv rtmp://localhost/hls2/CH100_mid

ffmpeg -re -fflags +genpts -stream_loop -1 -i "CH100_ 640K.ts" -vcodec copy -c:a aac -strict -2 -b:a 128k  -f flv rtmp://localhost/hls2/CH100_hi
```

命令`不能`是下面这种形式，否则会没有声音，可能是AAC音频没有ADTS封装：

```bash
ffmpeg -re -fflags +genpts -stream_loop -1 -i "CH100 _160K.ts" -vcodec copy -bsf:a aac_adtstoasc  -f flv rtmp://localhost/hls2/CH100_low
```

此情况下vlc播放这个直播的链接为：

> 
[http://nginx_server_IP/test2/CH100.m3u8](http://nginx_server_IP/test2/CH100.m3u8)


也可以直接播放某个目录下的：

> 
[http://nginx_server_IP/test2/CH100_low/index.m3u8](http://nginx_server_IP/test2/CH100_low/index.m3u8)











