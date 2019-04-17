# RTMP与HLS压力测试工具安装与配置 - DoubleLi - 博客园






在CentOS 6.5环境中安装依赖软件包，使用git下载最新版本st-load源码包
 [root@localhost ~]# yum install git unzip patch gcc gcc-c++ make
 [root@localhost ~]# git clone https://github.com/winlinvip/st-load.git
执行配置和编译
 [root@localhost st-load]# ./configure
 [root@localhost st-load]# make
查看生成的文件和目录
 [root@localhost st-load]# ls objs/
 http-parser-2.1 src st_hls_load st_rtmp_load st_rtmp_publish
 Makefile st-1.9 st_http_load st_rtmp_load_fast
 [root@localhost st-load]#

模拟RTMP用户
./st_rtmp_load -c 1 -r rtmp://127.0.0.1:1935/live/livestream

模拟HLS直播用户
./st_hls_load -c 1 -r http://127.0.0.1:3080/hls/hls.m3u8

模拟HSL点播用户
./st_hls_load -c 10000 -o -r http://127.0.0.1:3080/hls/hls.m3u8

模拟RTMP推流用户
./st_rtmp_publish -i doc/source.200kbps.768x320.flv -c 1 -r rtmp://127.0.0.1:1935/live/livestream

模拟RTMP多路推流用户
./st_rtmp_publish -i doc/source.200kbps.768x320.flv -c 1000 -r rtmp://127.0.0.1:1935/live/livestream_{i}
支持RTMP流播放测试，一个进程支持5k并发
支持RTMP流推流测试，一个进程支持500个并发。


相关下载：
(1) [SRS Bench](https://github.com/ossrs/srs-bench) （原st-load，最后更新：20151231）

原文地址：[http://www.linuxcache.com/archives/3328](http://www.linuxcache.com/archives/3328)









