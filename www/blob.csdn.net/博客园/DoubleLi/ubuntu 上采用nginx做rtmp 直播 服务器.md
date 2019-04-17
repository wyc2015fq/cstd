# ubuntu 上采用nginx做rtmp 直播 服务器 - DoubleLi - 博客园






首先安装必要的依赖库



sudo apt-get install autoconf automake

sudo apt-get install libpcre3 libpcre3-dev



安装 zlib库



sudo apt-get install openssl

sudo apt-get install libssl-dev



./configure --add-module=../nginx-rtmp-module-master --with-http_flv_module --with-http_ssl_module



make



sudo make install



cd /usr/local/nginx/sbin

sudo ./nginx -s reload         重启 nginx 服务





nginx.conf 中增加如下配置




rtmp {

   server {

        listen 1935;

        chunk_size 4000;

        application  live {

                   live on;

        }

    }

}

Nginx  不仅可以做直播 还可以对flv视频通过http协议进行点播



源码下载地址:  [http://download.csdn.net/detail/mtour/8021047](http://download.csdn.net/detail/mtour/8021047)










