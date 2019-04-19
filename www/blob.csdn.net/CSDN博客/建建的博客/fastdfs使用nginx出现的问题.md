# fastdfs使用nginx出现的问题 - 建建的博客 - CSDN博客
2018年02月06日 11:43:08[纪建](https://me.csdn.net/u013898698)阅读数：858
        3.解决报错
        make的时候会报一下错误，
        root/fastdfs-nginx-module/src//common.c:21:25: fatal error: fdfs_define.h: No such file or directory
 #include "fdfs_define.h"
                        ^
compilation terminated.
make[1]: *** [objs/addon/src/ngx_http_fastdfs_module.o] Error 1
解决办法：
  vim /root/fastdfs-nginx-module/src/config
 CORE_INCS="$CORE_INCS /usr/local/include/fastdfs /usr/local/include/fastcommon/"
 CORE_LIBS="$CORE_LIBS -L/usr/lib -lfastcommon -lfdfsclient"
 重新编译，安装即可
-------------------------------
```bash
[root@fastdfs-storage ~]
```
```bash
# vim /application/nginx/conf/nginx.conf
```
```bash
```
```bash
user  root;
```
```bash
```
```bash
worker_processes  1;
```
```bash
```
```bash
events {
```
```bash
```
```bash
worker_connections  1024;
```
```bash
```
```bash
}
```
```bash
```
```bash
http {
```
```bash
```
```bash
include       mime.types;
```
```bash
```
```bash
default_type  application
```
```bash
/octet-stream
```
```bash
;
```
```bash
```
```bash
sendfile        on;
```
```bash
```
```bash
keepalive_timeout  65;
```
```bash
```
```bash
server {
```
```bash
```
```bash
listen       8888;
```
```bash
```
```bash
server_name  localhost;
```
```bash
```
```bash
location ~
```
```bash
/group
```
```bash
[0-9]/ {
```
```bash
```
```bash
ngx_fastdfs_module;
```
```bash
```
```bash
}
```
```bash
```
```bash
error_page   500 502 503 504
```
```bash
/50x
```
```bash
.html;
```
```bash
```
```bash
location =
```
```bash
/50x
```
```bash
.html {
```
```bash
```
```bash
root   html;
```
```bash
```
```bash
}
```
```bash
```
```bash
}
```
```bash
```
```bash
}
```
说明： 
a、"user root"是解决下载操作时报404的问题
b、8888端口号与/etc/fdfs/storage.conf中的http.server_port=8888相对应
c、storage对应有多个group的情况下，访问路径带group名称，例如：/group1/M00/00/00/**，对应nginx配置：
    location ~/group[0-9]/ {
           ngx_fastdfs_module;
     }
