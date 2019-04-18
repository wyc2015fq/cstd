# 验证码/OCR图像识别预处理（去噪） - wsp_1138886114的博客 - CSDN博客





2018年10月07日 10:24:00[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1281








去噪

##### 获取图像红色通道（字体一般是黑色的，去除印章等其它因素）

##### 二值化

##### 邻域像素算法:

对于像素值>245的邻域像素，判别为属于背景色，如果一个像素上下左右4各像素值有超过2个像素属于背景色，那么该像素就是噪声。

```
def depoint(img):   #input: gray image
    pixdata = img.load()
    w,h = img.size
    for y in range(1,h-1):
        for x in range(1,w-1):
            count = 0
            if pixdata[x,y-1] > 245:
                count = count + 1
            if pixdata[x,y+1] > 245:
                count = count + 1
            if pixdata[x-1,y] > 245:
                count = count + 1
            if pixdata[x+1,y] > 245:
                count = count + 1
            if count > 2:
                pixdata[x,y] = 255
    return img
```



