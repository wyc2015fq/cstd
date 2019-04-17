# nginx支持flv MP4 扩展nginx_mod_h264_streaming，nginx-rtmp-module-master，yamdi - DoubleLi - 博客园






![复制代码](https://common.cnblogs.com/images/copycode.gif)
./configure \
--prefix=/usr/local/nginx \
--sbin-path=/usr/local/nginx/sbin/nginx \
--conf-path=/usr/local/nginx/conf/nginx.conf \
--error-log-path=/usr/local/nginx/log/error.log \
--http-log-path=/usr/local/nginx/log/access.log \
--pid-path=/usr/local/nginx/var/nginx.pid \
--lock-path=/usr/local/nginx/var/nginx.lock \
--http-client-body-temp-path=/tmp/clientbody \
--http-proxy-temp-path=/tmp/proxy \
--http-fastcgi-temp-path=/tmp/fastcgi \
--http-uwsgi-temp-path=/tmp/uwsgi \
--http-scgi-temp-path=/tmp/scgi \
--user=www \
--group=www \
--with-file-aio \
--with-http_realip_module \
--with-http_ssl_module \
--with-openssl=/usr/local/src/openssl \
--with-http_gzip_static_module \
--with-zlib=/usr/local/src/zlib \
--with-http_stub_status_module \
--with-pcre=/usr/local/src/pcre \
--without-select_module \
--without-poll_module \
--without-http_ssi_module \
--without-http_userid_module \
--without-http_geo_module \
--without-http_empty_gif_module \
--without-http_map_module \
--without-mail_pop3_module \
--without-mail_imap_module \
--without-mail_smtp_module \
--with-http_flv_module \
--with-http_mp4_module \
--add-module=/usr/local/src/nginx_mod_h264_streaming-2.2.7 \ 
--add-module=/usr/local/src/nginx-rtmp-module-master  

注意第三方模块使用的是add-module指定的绝对路径
yamdi
  tar xzvf yamdi-1.4.tar.gz

                   cd yamdi-1.4

           make && make install

      使用方法：yamdi -i input.flv -o out.flv


/usr/local/src 所有的文件都在这个目录
 tar xzvf nginx_mod_h264_streaming-2.2.7

 tar xzvf nginx-rtmp-module-master 


即可编译的时候指定路径--with-http_flv_module \
--with-http_mp4_module \
--add-module=/usr/local/src/nginx_mod_h264_streaming-2.2.7 \ 
--add-module=/usr/local/src/nginx-rtmp-module-master  

make
会出错

vi nginx_mod_h264_streaming-2.2.7/src/ngx_http_streaming_module.c
:set nu
158行
if (r->zero_in_uri)
{
return NGX_DECLINED;
}
注释掉/*if (r->zero_in_uri)
{
return NGX_DECLINED;
}*/
保存
在使用配置./configure 上面的那一段↑
make
make install








