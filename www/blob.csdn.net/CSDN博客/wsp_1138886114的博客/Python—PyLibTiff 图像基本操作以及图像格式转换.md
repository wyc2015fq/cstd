# Python—PyLibTiff 图像基本操作以及图像格式转换 - wsp_1138886114的博客 - CSDN博客





2018年12月27日 15:20:27[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：335











### 文章目录
- [一、图片 读、写、显示、属性查看](#__6)
- [libtiff 包装器](#libtiff__7)
- [Python 模块](#Python__19)
- [opencv 模块](#opencv__31)
- [PIL 模块](#PIL__45)
- [直接修改图片格式](#_61)
- [大(分辨率大)图片缩小](#_78)
- [与上面代码同效](#_123)



关于图像处理的模块常用的有 PIL，openCV等，不过应为要处理 tif 格式的图片，故特来写下这篇博客。

关于安装模块 **libtiff**

直接`pip install libtiff` 安装模块，发现无法导入，显示“No module named libtiff” ，打开anaconda prompt 执行`conda list`显示模块确实已经安装。尝试了把libtiff移除再重装还是没解决。


直接下载模块安装：[https://www.lfd.uci.edu/~gohlke/pythonlibs/](https://www.lfd.uci.edu/~gohlke/pythonlibs/) 注意下载适配的python版本，与win系统

### 一、图片 读、写、显示、属性查看

##### libtiff 包装器

```python
from libtiff import TIFF

tif = TIFF.open('filename.tif', mode='r')    #打开tiff文件进行读取
image = tif.read_image()                     #读取图像并作为numpy数组返回

for image in tif.iter_images()               #读取TIFF文件中的所有图像

tif = TIFF.open('filename.tif', mode='w')    #打开tiff文件进行写入
tif.write_image(image)                       #将图像写入tiff文件
```

##### Python 模块

```python
from libtiff import TIFFfile, TIFFimage

tif = TIFFfile('filename.tif')              #读取图片
samples, sample_names = tiff.get_samples()
tiff = TIFFimage(data, description='')

tiff.write_file('filename.tif', compression='none') # or 'lzw'
del tiff                                            # 刷新（释放缓存）
```

##### opencv 模块

```python
import cv2
cv2.imread("filename",flags)

=====================其中：flags四种选择如下：==================
IMREAD_UNCHANGED = -1 #不进行转化，比如保存为了16位的图片，读取出来仍然为16位。
IMREAD_GRAYSCALE = 0 #转化为灰度图，比如保存为了16位的图片，读取出来为8位，类型为CV_8UC1。
IMREAD_COLOR = 1     #进行转化为RGB三通道图像，图像深度转为8位
IMREAD_ANYDEPTH = 2  #保持图像深度不变，进行转化为灰度图。
IMREAD_ANYCOLOR = 4  #若通道数小于等于3，则保持不变；若通道数大于3则只取取前三个通道。图像深度转为8位

对于多通道TIFF图像，若要保证图像数据的正常读取，显然要选择IMREAD_UNCHANGED
```

##### PIL 模块

```python
from PIL import Image
img0 = Image.open("D:/python_script/ffff/11lalala.jpg")
img1 = Image.open("D:/python_script/ffff/42608122.tif")
img2 = Image.open("D:/python_script/ffff/42608122_1.jpg")  #这张图片是直接修改上张图的后缀名

print ("图片格式:{0}，图片大小:{1}，图片模式:{2}".format(img0.format,img0.size,img0.mode))
print ("图片格式:{0}，图片大小:{1}，图片模式:{2}".format(img1.format,img1.size,img1.mode))
print ("图片格式:{0}，图片大小:{1}，图片模式:{2}".format(img2.format,img2.size,img2.mode))

输出：#说明直接修改图片后缀名，图片的编码格式并没有改变
图片格式:JPEG，图片大小:(245, 213)，图片模式:RGB
图片格式:TIFF，图片大小:(2480, 3508)，图片模式:YCbCr
图片格式:TIFF，图片大小:(2480, 3508)，图片模式:YCbCr
```

##### 直接修改图片格式

```python
import PIL.Image
import os

def convert(input_dir,output_dir):
    for filename in os.listdir(input_dir):
        path = input_dir+"/"+filename
        print("doing... ",path)
        PIL.Image.open(path).save(output_dir+"/"+filename[:-4]+".jpg")
        print ("%s has been changed!"%filename)

if __name__ == '__main__':
   input_dir = "D:/classifier_data20181225/img1" 
   output_dir = "D:/classifier_data20181225/img2"
   convert(input_dir,output_dir)
```

##### 大(分辨率大)图片缩小

遇到分辨率大，图片文件大小并不大的文件，opencv打不开，此时用到了以下代码用来缩小图片。
- 若文件宽大于1200，（高度小于1800）以此宽度等比缩放
- 若文件高大于1800，（宽度小于1200）以此高度等比缩放

```python
import os
from PIL import Image
import shutil

def get_img(input_dir):
    img_path_list = []
    for (root_path,dirname,filenames) in os.walk(input_dir):
        for filename in filenames:
            img_path = root_path+"/"+filename
            img_path_list.append(img_path)
    print("img_path_list",img_path_list)
    return  img_path_list

def process_image(filename,output_dir, mwidth=1200, mheight=1800):
    image = Image.open(filename)
    w, h = image.size
    if w <= mwidth and h <= mheight:
        print(filename, 'is OK.')
        shutil.move(filename, output_dir+filename[-15:])
        return
    if (1.0 * w / mwidth) > (1.0 * h / mheight):
        scale = 1.0 * w / mwidth
        new_im = image.resize((int(w / scale), int(h / scale)), Image.ANTIALIAS)

    else:
        scale = 1.0 * h / mheight
        new_im = image.resize((int(w / scale), int(h / scale)), Image.ANTIALIAS)
    new_im.save(output_dir+filename[-15:])
    new_im.close()


if __name__ == '__main__':
    input_dir = "D:/classifier_data20181212/lipei_resize_1"
    output_dir = "D:/classifier_data20181212/lipei_resize/"
    img_path_list = get_img(input_dir)
    for filename in img_path_list:
        print("filename",filename)
        process_image(filename,output_dir)
```

##### 与上面代码同效

```python
# -*- coding: utf-8 -*-
import os
from PIL import Image


class image_aspect():

    def __init__(self, image_file, aspect_width, aspect_height):
        self.img = Image.open(image_file)
        self.aspect_width = aspect_width
        self.aspect_height = aspect_height
        self.result_image = None

    def change_aspect_rate(self):
        img_width = self.img.size[0]
        img_height = self.img.size[1]

        if (img_width / img_height) > (self.aspect_width / self.aspect_height):
            rate = self.aspect_width / img_width
        else:
            rate = self.aspect_height / img_height

        rate = round(rate, 1)
        print(rate)
        self.img = self.img.resize((int(img_width * rate), int(img_height * rate)))
        return self

    def past_background(self):
        self.result_image = Image.new("RGB", [self.aspect_width, self.aspect_height], (0, 0, 0, 255))
        self.result_image.paste(self.img, (
        int((self.aspect_width - self.img.size[0]) / 2), int((self.aspect_height - self.img.size[1]) / 2)))
        return self

    def save_result(self, file_name):
        self.result_image.save(file_name)

def get_img(input_dir):
    img_path_list = []
    for (root_path,dirname,filenames) in os.walk(input_dir):
        for filename in filenames:
            img_path = root_path+"/"+filename
            img_path_list.append(img_path)
    print("img_path_list",img_path_list)
    return  img_path_list


if __name__ == '__main__':
    input_dir = "D:/classifier_data20181212/img"
    output_dir = "D:/classifier_data20181212/img2/"
    img_path_list = get_img(input_dir)
    for filename in img_path_list:
        print("filename",filename)
        image_aspect(filename, 1200, 1600)\
            .change_aspect_rate()\
            .past_background()\
            .save_result(output_dir+filename[-13:])
```



