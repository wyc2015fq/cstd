# 流媒体压力测试rtmp&hls（含推流和拉流） - DoubleLi - 博客园






[http://blog.csdn.net/sinat_34194127/article/details/50816045](http://blog.csdn.net/sinat_34194127/article/details/50816045)








[root@localhost ~]# yum install git unzip patch gcc gcc-c++ make
[root@localhost ~]# git clone https://github.com/winlinvip/st-load.git

[root@localhost st-load]# ./configure
[root@localhost st-load]# make

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
./st_rtmp_publish -i doc/source.200kbps.768×320.flv -c 1 -r rtmp://127.0.0.1:1935/live/livestream
模拟RTMP多路推流用户
./st_rtmp_publish -i doc/source.200kbps.768×320.flv -c 1000 -r rtmp://127.0.0.1:1935/live/livestream_{i}

支持RTMP流播放测试，一个进程支持5k并发
支持RTMP流推流测试，一个进程支持500个并发。



build ok, you can:
     ./objs/sb_http_load
     ./objs/sb_hls_load
     ./objs/sb_rtmp_load
     ./objs/sb_rtmp_load_fast
     ./objs/sb_rtmp_publish



Examples:
1. start a client
   ./objs/sb_rtmp_load_fast -c 1 -r rtmp://127.0.0.1:1935/live/livestream
2. start 1000 clients
   ./objs/sb_rtmp_load_fast -c 1000 -r rtmp://127.0.0.1:1935/live/livestream
3. start 10000 clients
   ./objs/sb_rtmp_load_fast -c 10000 -r rtmp://127.0.0.1:1935/live/livestream
4. start 100000 clients
   ./objs/sb_rtmp_load_fast -c 100000 -r rtmp://127.0.0.1:1935/live/livestream



实例： 

./objs/sb_rtmp_load -c 500 -r rtmp://172.17.14.88:1935/live/BHIP91_172.17.17.185_1



./objs/sb_hls_load -c 1 -r http://172.17.14.88:10000/hls/BHIP91_172.17.17.183_1_sd.m3u8




Examples:
1. start a client
   ./objs/sb_hls_load -c 1 -r http://127.0.0.1:3080/hls/hls.m3u8
2. start 1000 clients
   ./objs/sb_hls_load -c 1000 -r http://127.0.0.1:3080/hls/hls.m3u8
3. start 10000 clients
   ./objs/sb_hls_load -c 10000 -r http://127.0.0.1:3080/hls/hls.m3u8
4. start 100000 clients
   ./objs/sb_hls_load -c 100000 -r http://127.0.0.1:3080/hls/hls.m3u8
5. start 10000 vod clients
   ./objs/sb_hls_load -c 10000 -o -r http://127.0.0.1:3080/hls/hls.m3u8










