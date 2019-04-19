# ffmpeg的使用 - =朝晖= - 博客园
# [ffmpeg的使用](https://www.cnblogs.com/dhcn/p/7125224.html)
ffmpeg参数中文详细解释：http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/leixiaohua1020/article/details/12751349
ffmpeg教程中文版:  http://download.csdn[.net](http://lib.csdn.net/base/dotnet)/detail/stevenq2012/8142329 
**How To Convert MP4 Video File into FLV Format Using FFMPEG**：http://stackoverflow.com/questions/8504923/how-to-convert-mp4-video-file-into-flv-format-using-ffmpeg
- ffmpeg -i source.mp4 -c:v libx264 -crf 19 destinationfile.flv  
- ffmpeg -i source.mp4 -c:v libx264 -ar 22050 -crf 25 -acodec aac -f flv destinationfile.flv  
//FLV does not support sample rate 48000, choose from (44100, 22050, 11025)
//Audio codec mp3 not compatible with flv
**用FFmpeg从视频截取任意一帧图片的解决办法~ **http://www.cnblogs.com/yao/archive/2006/01/17/318772.html
- ffmpeg -i test.asf -y -f  image2  -ss 00:01:00 -vframes 1  test1.jpg  
- 
- ffmpeg -i test.asf -y -f  image2  -ss 60 -vframes 1  test1.jpg  
给flv加流媒体播放相关meta信息：
- yamdi -i input.flv -o output.flv  
- 
