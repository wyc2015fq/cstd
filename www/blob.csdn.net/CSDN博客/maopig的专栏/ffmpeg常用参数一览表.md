# ffmpeg常用参数一览表 - maopig的专栏 - CSDN博客
2011年07月16日 10:39:04[maopig](https://me.csdn.net/maopig)阅读数：47709标签：[codec																[stream																[string																[flv																[image																[output](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=flv&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/maopig/article/category/847674)
-tvstd standard设置电视标准 NTSC PAL(SECAM)
**        要得到一个高画质音质低容量的MP4的话，首先画面最好不要用固定比特率，而用VBR参数让程序自己去判断，而音质参数可以在原来的基础上提升一点，听起来要舒服很多，也不会太大（看情况调整 ） **
**转换为flv:**
    ffmpeg -i test.mp3 -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 test.flv 
    ffmpeg -i test.wmv -ab 56 -ar 22050 -b 500 -r 15 -s 320x240 test.flv 
**转换文件格式的同时抓缩微图： **
    ffmpeg -i "test.avi" -y -f image2 -ss 8 -t 0.001 -s 350x240 'test.jpg' 
**对已有flv抓图： **
    ffmpeg -i "test.flv" -y -f image2 -ss 8 -t 0.001 -s 350x240 'test.jpg' 
**转换为3gp:**
    ffmpeg -y -i test.mpeg -bitexact -vcodec h263 -b 128 -r 15 -s 176x144 -acodec aac -ac 2 -ar 22500 -ab 24 -f 3gp test.3gp 
    ffmpeg -y -i test.mpeg -ac 1 -acodec amr_nb -ar 8000 -s 176x144 -b 128 -r 15 test.3gp 
**        例：ffmpeg -y -i "test.avi" -title "Test" -vcodec xvid -s 368x208 -r 29.97 - b 1500 -acodec aac -ac 2 -ar 24000 -ab 128 -vol 200 -f psp -muxvb 768 "test.***"     # 参数解释：**
    -y（覆盖输出文件，即如果1.***文件已经存在的话，不经提示就覆盖掉了） 
    -i "1.avi"（输入文件是和ffmpeg在同一目录下的1.avi文件，可以自己加路径，改名字） 
    -title "Test"（在PSP中显示的影片的标题） 
    -vcodec xvid（使用XVID编码压缩视频，不能改的） 
    -s 368x208（输出的分辨率为368x208，注意片源一定要是16:9的不然会变形） 
    -r 29.97（帧数，一般就用这个吧） 
    -b 1500（视频数据流量，用-b xxxx的指令则使用固定码率，数字随便改，1500以上没效果；还可以用动态码率如：-qscale 4和-qscale 6，4的质量比6高） 
    -acodec aac（音频编码用AAC） 
    -ac 2（声道数1或2） 
    -ar 24000（声音的采样频率，好像PSP只能支持24000Hz） 
    -ab 128（音频数据流量，一般选择32、64、96、128） 
    -vol 200（200%的音量，自己改） 
    -f psp（输出psp专用格式） 
    -muxvb 768（好像是给PSP机器识别的码率，一般选择384、512和768，我改成1500，PSP就说文件损坏了） 
    "test.***"（输出文件名，也可以加路径改文件名）            
