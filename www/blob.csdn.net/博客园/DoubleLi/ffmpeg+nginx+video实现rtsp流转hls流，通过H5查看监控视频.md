# ffmpeg+nginx+video实现rtsp流转hls流，通过H5查看监控视频 - DoubleLi - 博客园






一、FFmpeg下载：http://ffmpeg.zeranoe.com/builds/ 
下载并解压FFmpeg文件夹，配置环境变量：在“Path”变量原有变量值内容上加上d:\ffmpeg\bin，验证：ffmpeg -version 出现版本号则成功。
二、官网下载windows  Stable version版Nginx安装nginx服务器，配置：config和mime.types。
1.在nginx\conf\nginx.conf中：
  http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    keepalive_timeout  65;
    access_log off;
    server {
修改：    
    listen       20000;
        server_name  localhost;
       修改：
        location / {
           root   html;
           index  index.html index.htm;
        }
为了能访问到hls流协议新增：
location /hls {  
types{  
application/vnd.apple.mpegurl m3u8;  
video/mp2t ts;  
}  
root html;  
add_header Cache-Control no-cache;
        add_header Access-Control-Allow-Origin *;
        }  
2.在nginx\conf\mime.types中
为了支持hls流协议新增：
application/x-mpegURL m3u8; 
application/vnd.apple.mpegurl m3u8;
video/mp2t   ts;
3.在命令行中输入即可转换：也可写成脚本的形式运行。

ffmpeg -i "rtsp://admin:ajb123456@192.168.10.36" -c copy -f hls -hls_time 2.0 -hls_list_size 0 -hls_wrap 15 C:/wjanzhuang/nginx/html/hls/test.m3u8

三、在H5中查看:

1.引进：

 <link href="videolive/css/video.css" rel="stylesheet">
  <script src="videolive/js/video.js"></script>
  <script src="videolive/js/videojs-live.js"></script>

  2.使用video则可以在pc机浏览器上查看视频：

  <video id="my_video_1" class="video-js vjs-default-skin" controls preload="auto" width="500" height="500" 
  data-setup='{}'>
    <source src="http://192.168.10.245:20000/hls/test.m3u8" type="application/x-mpegURL"> 
  </video> 

三、在JAVA代码中查看方法

List<String> commend = new ArrayList<String>();
        commend.add("ffmpeg");
        commend.add("-i");
        commend.add("\""+"rtsp://admin:ajb123456@192.168.10.36"+"\"");
        commend.add("-c");
        commend.add("copy");
        commend.add("-f");
        commend.add("hls");
        commend.add("-hls_time");
        commend.add("2.0");
        commend.add("-hls_list_size");
        commend.add("0");
        commend.add("-hls_wrap");
        commend.add("15");
        commend.add("C:/wjanzhuang/nginx/html/hls/hls1/test.m3u8");
        try {
            ProcessBuilder builder = new ProcessBuilder(); //创建系统进程
            builder.command(commend);
            builder.start();//启动进程
        } catch (Exception e) {
            e.printStackTrace();
        }









