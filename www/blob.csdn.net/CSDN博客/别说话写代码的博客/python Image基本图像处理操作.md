# python Image基本图像处理操作 - 别说话写代码的博客 - CSDN博客





2018年01月19日 09:15:33[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：400








本文转自：http://blog.csdn.net/chenriwei2/article/details/42071517




Python 里面最常用的图像操作库是

﻿﻿

Image library（PIL），功能上，虽然还不能跟Matlab比较，但是还是比较强大的，废话补多少，写点记录笔记。


|getbbox()|包含非零区域的最小bbox|
|----|----|
|histogram(mask=None)|统计直方图|
|----|----|
offset(dx,dy=None)
平移
|putpixel(xy, color)|改变单个像素点颜色|
|----|----|
thumbnail(size,filter=None)缩略图|transform(xs, ys, Image.AFFINE, (a,b,c,d,e,f))|affine变换|
|----|----|
transpose(method)翻转旋转



1. 首先需要导入需要的图像库：

     import Image

2. 读取一张图片：

     im=Image.open('/home/Picture/test.jpg')

3. 显示一张图片：

    im.show()

4. 保存图片：

    im.save("save.gif","GIF")      #保存图像为gif格式

5. 创建新图片：

    Image.new(mode, size)  

    Image.new(mode, size, color)  

    栗子：newImg = Image.new("RGBA",(640,480),(0,255,0))
               newImg.save("newImg.png","PNG")

6. 两张图片相加：

    Image.blend(img1, img2, alpha)    # 这里alpha表示img1和img2的比例参数

7. 点操作：

    im.point(function) #,这个function接受一个参数，且对图片中的每一个点执行这个函数
    比如：out = im.point(lambda i : i*1.5)#对每个点进行50%的加强

8. 查看图像信息：
    im.format, im.size, im.mode

9.  图片裁剪：
    box = (100,100,500,500)  #设置要裁剪的区域

    region = im.crop(box)     #此时，region是一个新的图像对象。

10. 图像黏贴（合并）

    im.paste(region, box)#粘贴box大小的region到原先的图片对象中。

11. 通道分离：
   r,g,b = im.split()#分割成三个通道  ，此时r,g,b分别为三个图像对象。

12. 通道合并：
   im = Image.merge("RGB", (b, g, r))#将b,r两个通道进行翻转。

13. 改变图像的大小：
   out = img.resize((128, 128))#resize成128*128像素大小

14. 旋转图像：
   out = img.rotate(45) #逆时针旋转45度
   有更方便的：
   region = region.transpose(Image.ROTATE_180）

15. 图像转换：
out = im.transpose(Image.FLIP_LEFT_RIGHT)#左右对换。

out= im.transpose(Image.FLIP_TOP_BOTTOM)#上下对换

16. 图像类型转换：
    im=im.convert("RGBA")

17. 获取某个像素位置的值：
   im.getpixel((4,4))

18.  写某个像素位置的值：
   img.putpixel((4,4),(255,0,0))



