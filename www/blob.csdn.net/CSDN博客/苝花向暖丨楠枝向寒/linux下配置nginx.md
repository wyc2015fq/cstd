# linux下配置nginx - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月02日 19:41:37[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：30标签：[nginx](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)
个人分类：[nginx](https://blog.csdn.net/weixin_40247263/article/category/7559627)



1 下载nginx --放到一个文件夹里（我用的Xshell5）

2 安装gcc  # yum -y install gcc-c++

3 安装 pcre  # yum -y install pcre pcre-devel

4 安装 zlib # yum -y install zlib zlib-devel

5 安装 openssl # yum -y install  openssl openssl-devel

安装和编译

1 将nginx放到linux下   我是放在了usr/nginx目录下

2 解压nginx

3 进入文件夹

4 进入文件夹后发现没有make   这样就无法编译  

5 创建一个nginx临时目录  mkdir -p /var/temp/nginx

6  进入  [root@0dce1173d7c0 soft]# cd nginx-1.8.1 文件夹

  执行 configure命令    （后面的参数别硬记,没必要）

#

./configure \

--prefix=/usr/local/nginx \

--pid-path=/var/run/nginx/nginx.pid \

--lock-path=/var/lock/nginx.lock \

--error-log-path=/var/log/nginx/error.log \

--http-log-path=/var/log/nginx/access.log \

--with-http_gzip_static_module \

--http-client-body-temp-path=/var/temp/nginx/client \

--http-proxy-temp-path=/var/temp/nginx/proxy \

--http-fastcgi-temp-path=/var/temp/nginx/fastcgi \

--http-uwsgi-temp-path=/var/temp/nginx/uwsgi \

--http-scgi-temp-path=/var/temp/nginx/scgi

注意 要手动创建临时目录  mkdir -p /var/temp/nginx

7 再次执行ls 发现多了一个Makefile   有了这个  执行make即可   # make

8 安装   # make install

9 /usr/local目录下有了nginx

  进入/usr/local/nginx/conf

   vi nginx.conf

 在 server 上面 配置 

10 启动nginx    在nginx/sbin目录下    #  ./nginx

