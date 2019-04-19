# ffmpeg.exe抓取视频流图片 - xcyl的口袋 - CSDN博客
2016年06月17日 17:24:02[xcyl](https://me.csdn.net/cai6811376)阅读数：3595
## ffmpeg.exe从rtsp地址抓取图片
`ffmpeg.exe -i rtsp://admin:admin@192.168.66.222/11 -rtsp_transport tcp -f image2 -an D:\\pic\\%10d.jpg`
![ffmpeg](https://img-blog.csdn.net/20160617165639390)
![pic](https://img-blog.csdn.net/20160617165855927)
## ffmpeg.exe从视频文件中抓取图片
`ffmpeg.exe -i D:\\test.mp4 -f image2 -an D:\\pic\\%10d.jpg`
![ffmpegfile](https://img-blog.csdn.net/20160617170409221)
![picfile](https://img-blog.csdn.net/20160617170449331)
## 参数
> 
输入的视频流源地址，可以是文件或者rtsp地址等； 
  -i D:\test.mp4
默认1s截取25张图片,可以通过-r设置每秒截取的图片数量 
  -r fps  
  设置帧率,也就是每秒截取图片的数量(默认25)
ffmpeg.exe -i D:\test.mp4 -f image2 -an -r 1 D:\pic\%10d.jpg 
  这样子每1s截取1张图片
还可以设置截取间隔,起止 
  -ss 设定时间位置,语法:hh:mm:ss[.xxx] 
  -t 时长:限制转码/捕获视频的时间,语法:hh:mm:ss[.xxx] 
  ffmpeg -i D:\test.mp4 -r 25 -ss 00:00:10 -t 00:00:05 D:\pic\%10d.jpg 
  在第10秒开始,以每秒截取25张图片的速度,截取5秒时长的图片
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
