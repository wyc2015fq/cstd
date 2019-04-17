# python 图片旋转 上下 左右 180度 旋转 - 别说话写代码的博客 - CSDN博客





2018年01月19日 09:42:18[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：3676








```python
#首先建好一个数据_ud文件夹
import PIL.Image as img
import os
path_old = "C:/Users/49691/Desktop/数据/"
path_new = "C:/Users/49691/Desktop/数据_ud/"
filelist = os.listdir(path_old)
total_num = len(filelist)
print(total_num)
for i in range(total_num):
    im = img.open(path_old + str(i) + ".jpg")
    #ng = im.transpose(img.ROTATE_180) #旋转 180 度角。
    #ng = im.transpose(img.FLIP_LEFT_RIGHT) #左右对换。
    ng = im.transpose(img.FLIP_TOP_BOTTOM)  # 上下对换。
    ng.save(path_new + str(i) +'.jpg')
    if i%20 == 0:
        print(i)
print(i)

#ng = im.rotate(180) #逆时针旋转 45 度角。
#im.transpose(img.FLIP_LEFT_RIGHT) #左右对换。
#im.transpose(img.FLIP_TOP_BOTTOM) #上下对换。
#im.transpose(Image.ROTATE_90) #旋转 90 度角。

#im.transpose(Image.ROTATE_270) #旋转 270 度角。
#im.show()
#ng.show()
```




