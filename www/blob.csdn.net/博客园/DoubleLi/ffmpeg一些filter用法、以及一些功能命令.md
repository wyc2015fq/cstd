# ffmpeg一些filter用法、以及一些功能命令 - DoubleLi - 博客园






## 1、加字幕

         命令：ffmpeg -i <input> -filter_complex subtitles=filename=<SubtitleName>-y <output>

         说明：利用libass来为视频嵌入字幕，字幕是直接嵌入到视频里的硬字幕。

         参考资料：http://ffmpeg.org/ffmpeg-filters.html#subtitles-1

## 2、剪切

         命令：ffmpeg -i <input>-ss 0 -t 10 -y <output>

         说明：ss跟的是起始时间，t为持续时间，上面命令意思为从0秒开始截取10秒的时间。

         参考资料：http://ffmpeg.org/ffmpeg.html

## 3、缩放

         命令： ffmpeg -i<output> -filter_complex scale=320:240  -y <output>

         说明：scale参数为宽高。

## 4、剪裁

         命令：ffmpeg -i <input>-filter_complex crop=320:240:0:0 -y <output>

         说明：其中的 crop=320:240:0:0为裁剪参数，具体含义是 crop=width:height:x:y，其中 width 和 height 表示裁剪后的尺寸，x:y 表示裁剪区域的左上角坐标。

## 5、加水印

命令：ffmpeg -i src.avi -vf "movie=<LogoName>[logo];[in][logo]overlay=100:100[out]"-y <output>

         说明：LogoName为图片名，overlay=100:100意义为overlay=x:y，在(x,y)坐标处开始添加水印。

         左上角：overlay=10:10 

         右上角：overlay=main_w-overlay_w-10:10

         左下角：overlay=10:main_h-overlay_h-10 

         右下角：overlay=main_w-overlay_w-10:main_h-overlay_h-10

## 6、拼接视频

         第一种命令：

                   第一步：ffmpeg  -i INPUT -fmpeg  OUTPUT

                   第二步：copy /b INPUT+INPUT OUTPUT

                   第三步：ffmpeg -i INPUT -f FORMAT OUTPUT

         说明：第一步把输入文件转为相同格式，第二步利用copy命令把文件合并，第三步把合并文件转为最终结果视频。

         例：把名为test.avi、test1_2.mp4 两个视频合并为resu.avi。

         第一步：ffmpeg -itest1.avi test1.mpg

                            ffmpeg-i test1_2.mp4 test2.mpg

         第二步：copy /btest1.mpg+test2.mpg resu.mpge

         第三步：ffmpeg -iresu.mpge -y resu.avi

第二种命令：

ffmpeg -i 1.mov -i 2.wmv -filter_complex "[0:0] [0:1] [1:0] [1:1]  concat=n=2:v=1:a=1 [v] [a]" -map [v] -map [a] output.mp4

## 7、旋转

         命令： ffmpeg -i <input> -filter_complex transpose=X -y <output>

         说明：transpose=1为顺时针旋转90°，transpose=2逆时针旋转90°。

## 8、镜像

**上下镜像**

                   命令：

ffmpeg -i src.avi -vf "split[mian][tmp];[tmp]crop=iw:ih/2:0:0,vflip[flip];[mian][flip]overlay=0:H/2"-y GHO.avi

                  说明：从命令中可以看出crop和vflip在一条流水线上，他们的处理流程如下图所示：

![](http://img.blog.csdn.net/20150706172251206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



                   可以利用此filter来做上下颠倒，命令如下： ffmpeg-i src.avi -vf "split [main][tmp

]; [tmp] crop=iw:ih:0:0, vflip [flip];[main][flip] overlay=0:0" GHO2.avi处理效果和命令ffmpeg -isrc.avi -vf vflip GHO_v_1.avi一样，这样写只是为了更好的理解filter处理链。

**左右镜像**

                  命令： ffmpeg -i src.avi-vf "split [main][tmp]; [tmp] crop=iw/2:ih:0:0, hflip [flip]; [main][flip]overlay=W/2:0" GHO_H.avi

                   说明：流程和上下镜像一样，只是把坐标换了，并且指定用名为hfilp的filter。

                   可以利用此filter来做左右颠倒，命令如下： ffmpeg-i src.avi -vf "split [main][tmp

]; [tmp] crop=iw:ih:0:0, hflip [flip];[main][flip] overlay=W:0" GHO_H_1.avi和命令ffmpeg -i src.avi-vf hflip GHO_H_1.avi一样的效果，这样写只是为了更好的理解filter处理链。

**小结：**split过滤器把输入分裂为2路输出**，**crop过滤器为翻转选取图像范围，vflip和hflip过滤器把crop切下的图像翻转（垂直、水平），overlay过滤器指定坐标来贴经过翻转处理的图像。



## 9、加黑边

         命令： ffmpeg -isrc.avi -vf pad=1280:800:0:40:black -y test_pad.avi

         说明：pad=width:high:x:y:coler，这里的宽和高指的是结果视频尺寸（包含加黑边的尺寸），XY指的是源视频添加到结果视频所在位置，coler为填充颜色。

## 10、调音量

         命令：ffmpeg -i<input> -vol X <output>

         说明：不解释









