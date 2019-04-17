# Linux上用nginx搭建RTMP服务器 - DoubleLi - 博客园






参考文章：[https://obsproject.com/forum/resources/how-to-set-up-your-own-private-rtmp-server-using-nginx.50/](https://obsproject.com/forum/resources/how-to-set-up-your-own-private-rtmp-server-using-nginx.50/)

我的环境ubuntu

1、 安装编译 nginx 所需要的库
` sudo apt-get install build-essential libpcre3 libpcre3-dev libssl-dev`- 1
- 1

2、 下载 nginx-1.9.15.tar.gz
`wget http://nginx.org/download/nginx-1.9.15.tar.gz`- 1
- 1

3、 下载 nginx-rtmp-module
`wget https://github.com/arut/nginx-rtmp-module/archive/master.zip`- 1
- 1

4、解压

```
tar -zxvf nginx-1.9.15.tar.gz
unzip master.zip
cd nginx-1.9.15
```
- 1
- 2
- 3
- 1
- 2
- 3

5、编译

```
./configure --with-http_ssl_module --add-module=../nginx-rtmp-module-master
make
sudo make install
```
- 1
- 2
- 3
- 1
- 2
- 3

6、启动nginx，检测nginx是否能成功运行

```bash
sudo /usr/local/nginx/sbin/nginx
```
- 1
- 1

7、配置nginx使用RTMP, /usr/local/nginx/conf/nginx.conf

```
rtmp {
        server {
                listen 1935;
                chunk_size 4096;

                application live {
                        live on;
                        record off;
                }
        }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11

8、重启nginx服务器

```bash
sudo /usr/local/nginx/sbin/nginx -s stop
sudo /usr/local/nginx/sbin/nginx
```
- 1
- 2
- 1
- 2

9、测试 
OBS 直播设置

```
Streaming Service: Custom
Server: rtmp://<your server ip>/live //例如：rtmp://192.168.18.116/live
Play Path/Stream Key: test
```
- 1
- 2
- 3
- 1
- 2
- 3

VLC 播放设置

```
Network Stream URL: rtmp://%your_server_ip%/live/test
例如：rtmp://192.168.18.116/live/test
```









