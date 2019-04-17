# python通过ffmgep从视频中抽帧 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月08日 13:42:31[QuJack](https://me.csdn.net/A784586)阅读数：5554








**python通过ffmgep从视频中抽帧**



ffmpeg中文文档：[http://linux.51yip.com/search/ffmpeg](http://linux.51yip.com/search/ffmpeg)

ffmpeg -i
test_baofeng.wmv -y -f
image2 -ss 00:00:03
-vframes 1 myframe.jpg

**ffmpeg -i test.mp4 -y -f mjpeg -ss 3 -t 1 test1.jpg**

-f fmt 强迫采用格式fmt

-I filename 输入文件

-y 覆盖输出文件

-t duration 设置纪录时间 hh:mm:ss[.xxx]格式的记录时间也支持

-ss position 搜索到指定的时间 [-]hh:mm:ss[.xxx]的格式也支持

python使用ffmgep,通常用：subprocess ffmpeg/libav

--------------------python通过ffmgep抽帧---------------------


```python
import os, sys
from PIL import Image
#open a pipe from a command 
a, b, c = os.popen3("ffmpeg -i test.avi")
out = c.read()
dp = out.index("Duration: ")
duration = out[dp+10:dp+out[dp:].index(",")]
hh, mm, ss = map(float, duration.split(":"))
#total time ss
total = (hh*60 + mm)*60 + ss
for i in xrange(9):
    t = int((i + 1) * total / 10)
	# ffmpeg -i test.mp4 -y -f mjpeg -ss 3 -t 1  test1.jpg 
    os.system("ffmpeg -i test.avi -y -f mjpeg -ss %s -t 1 frame%i.jpg" % (t, i))

"""
num=int(total-3)
i=0
for t in xrange(0,num,3):
    i = i+1
	# ffmpeg -i test.mp4 -y -f mjpeg -ss 3 -t 1  test1.jpg 
    os.system("ffmpeg -i test.avi -y -f mjpeg -ss %s -t 1 %sframe%i.jpg" % (t,t, i))
"""
```








