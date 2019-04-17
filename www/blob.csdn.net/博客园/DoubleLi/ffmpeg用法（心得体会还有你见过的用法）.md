# ffmpeg用法（心得体会还有你见过的用法） - DoubleLi - 博客园






ffmpeg的常用用法很多，我这里提供的用法有可能有许多地方是你没见过的。

一、ffmpeg合并视频

我经常需要切割再把一些零碎的视频给拼接起来，这样可以省许多磁盘空间。其实用mencoder挺不错的，至少合并的参数极简单。用ffmpeg可以用下面的方法合并，至少我觉得比用cat这么野蛮的合并文件的方法要好。



```
```bash
ffmpeg -i 1.avi -qscale:
```

```bash
v
```

```bash
1 1.mpg
```



```bash
ffmpeg -i 2.avi -qscale:
```

```bash
v
```

```bash
1 2.mpg
```



```bash
ffmpeg -i concat:
```

```bash
"1.mpg|2.mpg"
```

```bash
-c copy all.mpg
```



```bash
ffmpeg -i all.mpg -qscale:
```

```bash
v
```

```bash
2 output.avi
```
```





二、ffmpeg做水面倒影效果



```
```bash
ffmpeg -i input.avi -vf
```

```bash
"split[a][b];[a]pad=iw:ih*2[a];[b]vflip[b];[a][b]overlay=0:h"
```

```bash
output.avi
```
```





三、ffmpeg处理MKV文件也很方便

一般情况下用mkvtoolnix的，ffmpeg处理mkv也很灵活强大。

例如：把2.mkv和3.mkv的所有音频加到1.mkv里（在拼轨同时还可以分别指定对每个轨道进行copy还是重编码）：



```
```bash
ffmpeg -i 1.mkv -i 2.mkv -i 3.mkv -map 0:0 -map 0:a -map 1:a -map 2:a -map 3:a -c copy OK.mkv
```
```





四、添加图片水印



```
```bash
ffmpeg -i input.avi -vf
```

```bash
"movie=1.png[wm];[in][wm]overlay=W-w:H-h"
```

```bash
output.avi
```
```



这样会把图片放在原影片的最右下角位置



五、把图片和音频合为mp4



```
```bash
ffmpeg -f image2 -r 30
```

```bash
/216
```

```bash
-i %3d.jpg -i music.mp3 -c:
```

```bash
v
```

```bash
mpeg4 -c:a copy OK.mp4
```
```



这样就能把图片和mp3合成视频了，而且图片的张数的时间按指定的music.mp3的时间来平均分配，不会出现图片放完了，音乐还在播放的情况。但有一点要特别注意，如果图片特别少指定的帧率太低了播放可能会出问题，我在Windows下用smplayer播放会出现卡死情况。用mencoder做这些事也非常方便。



六、分割视频并给分割出的视频开头和结尾做淡入淡出效果



```
```bash
ffmpeg -ss 20 -i p.mp4 -vf
```

```bash
"fade=in:0:50,fade=out:450:50"
```

```bash
-t 20 Ok.mp4
```



```bash
ffmpeg -ss 20 -i p.mp4 -vf
```

```bash
"fade=in:0:d=1,fade=out:st=29:d=1"
```

```bash
-t 20 OK.mp4
```
```



上面第一个命令是从p.mp4的第20秒开始到第40秒结束的一段视频保存为OK.mp4，并给开头的前50帧做个渐入的效果，给结尾的50帧做个渐黑的效果。第二个命令是按时间来算的，意思是开头淡入1秒和结尾最后一秒淡出，这在指定结尾的淡出非常方便。









