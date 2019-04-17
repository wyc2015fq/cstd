# nginx 点播mp4方法 - DoubleLi - 博客园






## 1.配置文件

配置文件中rtmp部分： 
application vod { 
play html; 
} 
配置文件中http部分： 
location ~ .mp4$ { 
root html; 
mp4; 
limit_conn addr 20; 
limit_rate 20000k;

## 2.视频存放路径##

将流化后的1.mp4存放到html路径下

## 3.点播地址

[http://127.0.0.1/1.mp4](http://127.0.0.1/1.mp4)
rtmp://127.0.0.1/vod/1.mp4









