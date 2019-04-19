# python图形处理库PIL(Python Image Library)的介绍 - 宇宙浪子的专栏 - CSDN博客
2015年03月12日 11:48:13[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：5888
### [python图形处理库PIL(Python Image Library)的介绍](http://abruzzi.iteye.com/blog/314790)
**关于PIL库的一些概念**
**pil能处理的图片类型**
pil可以处理光栅图片(像素数据组成的的块)。
**通道**
一个图片可以包含一到多个数据通道，如果这些通道具有相同的维数和深度，Pil允许将这些通道进行叠加
**模式**
|1|1位像素，黑和白，存成8位的像素|
|----|----|
|L|8位像素，黑白|
|P|8位像素，使用调色板映射到任何其他模式|
|RGB|3×8位像素，真彩|
|RGBA|4×8位像素，真彩+透明通道|
|CMYK|4×8位像素，颜色隔离|
|YCbCr|3×8位像素，彩色视频格式|
|I|32位整型像素|
|F|32位浮点型像素|
|||
**尺寸**
通过图片对象的size属性可以得到图片的尺寸，结果这是一个二元组，包含水平和垂直方向上的像素数。
**坐标**
Pil采取左上角为(0,0)的坐标系统
**调色板**
mode("P")为每个像素定义具体的颜色值
**图片信息**
可以通过info属性读取一张图片的附加信息，这个与图片的格式有关。
**滤镜**
在对图片的几何操作中可能会将多个输入像素映射到单个的输出像素，pil提供4种不同的采样滤镜(在目前的版本中，后续的版本可能支持更多)
|NEAREST|最近|
|----|----|
|BILINEAR|双线型|
|BICUBIC|双三次插值|
|ANTIALIAS|平滑|
在RGB模式下，每个图片由三个通道叠加而成，每个模式下为一个灰度图，当有一个调色板来调色的时候，这三张灰度图的叠加即可合成3*8位(每个像素)的一个真彩图片。pil库中，图片之间的模式(mode)可以转化。下面给出一些简单的例子，例子中的所有图片均来自于国家地理的官网，为了使得文档比较短小，每个图片均使用Pil缩放成1/2大小，如有侵权嫌疑，请尽快联系，我会删除这些图片。
所有的图片操作必须有一个操作对象，Pil提供*open(filename)*进行这个过程
**打开图片**
1.导入pil的Image模块
2.使用open(filename)打开文件，返回一个image对象
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- im = Image.open('filename')  
```python
im = Image.open('filename')
```
此后，一切关于图片的操作均基于这个对象。
![](http://abruzzi.iteye.com/upload/attachment/67907/9d59e028-a64d-38b5-86a9-17f1c458d4e5.jpg)
打开后，我们可以查看一些图片信息，如im.format, im.size, im.mode等。调用im.show()会在图片查看工具中显示当前操作的image对象，这个跟个人的系统有关系，我系统中默认是用Windows Picture and Fax Viewer打开的。这个方法用来查看临时的图片效果。
**读写图片**
pil中转换图片格式非常简单(转换图片模式是另一个概念，不要混淆)，只需要调用img.save(filename)即可比如有一个bmp(位图)图片，使用img = Image.open('file.bmp')打开后，只需要img.save('file.jpg')即可转换。不过一般情况下，save(filename)是不用做这个用途的，通常，save用以保存一个临时的image对象到硬盘。而转换工作由一个功能更为强大的convert()方法来完成。
**拷贝，粘贴，合并**
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- box = (100,100,500,500)#设置要拷贝的区域
- 
- #将im表示的图片对象拷贝到region中，大小为(400*400)像素。这个region可以用来后续的操作(region其实就是一个Image对象)，box变量是一个四元组(左，上，右，下)。
- region = im.crop(box)   
- 
- region = region.transpose(Image.ROTATE_180)#从字面上就可以看出，先把region中的Image反转180度，然后再放回到region中。
- im.paste(region, box)#粘贴box大小的region到原先的图片对象中。
```python
box = (100,100,500,500)#设置要拷贝的区域
#将im表示的图片对象拷贝到region中，大小为(400*400)像素。这个region可以用来后续的操作(region其实就是一个Image对象)，box变量是一个四元组(左，上，右，下)。
region = im.crop(box)
region = region.transpose(Image.ROTATE_180)#从字面上就可以看出，先把region中的Image反转180度，然后再放回到region中。
im.paste(region, box)#粘贴box大小的region到原先的图片对象中。
```
![](http://abruzzi.iteye.com/upload/attachment/67929/23e23659-c9d6-391a-a5c0-642a0da0da3b.jpg)
前面说过，每一个RGB都是由三个通道的灰度图叠加的，所以pil提供了将这三个通道分离的方法
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- r,g,b = im.split()#分割成三个通道 
- r.show()   
- g.show()   
- b.show()   
- im = Image.merge("RGB", (b, g, r))#将b,r两个通道进行翻转。
```python
r,g,b = im.split()#分割成三个通道
r.show()
g.show()
b.show()
im = Image.merge("RGB", (b, g, r))#将b,r两个通道进行翻转。
```
![](http://abruzzi.iteye.com/upload/attachment/67917/aded3d14-6b02-3c4e-8b6b-721e6c0513ac.jpg)
红色通道的灰度图
![](http://abruzzi.iteye.com/upload/attachment/67915/ab65be22-fd70-3a83-876e-7540d50b0516.jpg)
 绿色通道的灰度图
![](http://abruzzi.iteye.com/upload/attachment/67913/78d2e95c-de7e-3435-aff2-3a553fd9199d.jpg)
 蓝色通道的灰度图
![](http://abruzzi.iteye.com/upload/attachment/67921/228d018c-3a4c-382d-911f-e363616f7d2c.jpg)
 互换红蓝通道后的合成图
**几何转变**
几何转变提供resize,rotate等方法，用以重定义图片大小，对图片进行旋转等操作，在实际应用中比较广泛。
如 
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- out = img.resize((128, 128))#resize成128*128像素大小。
- out = img.rotate(45)#逆时针旋转45度
```python
out = img.resize((128, 128))#resize成128*128像素大小。
   out = img.rotate(45)#逆时针旋转45度
```
![](http://abruzzi.iteye.com/upload/attachment/67931/d78bd43c-edff-34f1-8e24-ff4864ad7ebf.jpg)
逆时针45度
![](http://abruzzi.iteye.com/upload/attachment/67923/153f70f2-ef4a-305b-8f19-1c5c50afed03.jpg)
 镜面效果，左右翻转
transpose()方法预定义了一些旋转方式，如
左右反转，上下翻转，逆时针旋转(90,180,270)度等，非常方便，rotate()和transpose()方法在表现上没有任何不同。
**图片加强**
*滤镜*
ImageFilter模块提供了很多预定义的图片加强滤镜。
比如一个常用的滤镜,细节(detail滤镜)
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- import ImageFilter   
- out = im.filter(ImageFilter.DETAIL)  
```python
import ImageFilter
out = im.filter(ImageFilter.DETAIL)
```
![](http://abruzzi.iteye.com/upload/attachment/67927/15d69cfb-4f77-35f2-a71f-30b2943d70da.jpg)
*直接操作像素点*
不但可以对每个像素点进行操作，而且，每一个通道都可以独立的进行操作。比如，将每个像素点的亮度(不知道有没有更专业的词)增大20%
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- out = img.point(lambda i : i * 1.2)#注意这里用到一个匿名函数(那个可以把i的1.2倍返回的函数)
```python
out = img.point(lambda i : i * 1.2)#注意这里用到一个匿名函数(那个可以把i的1.2倍返回的函数)
```
![](http://abruzzi.iteye.com/upload/attachment/67919/ef666066-02b8-3fb1-b8d7-ec0062f57c44.jpg)
对每个点都做20%的增强
如上边的那个例子，我们可以将一个RGB模式的图分离成三个通道的层
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- r,g,b = img.split()#神奇而又强大的python语法
```python
r,g,b = img.split()#神奇而又强大的python语法
```
然后对一个通道进行加强或减弱操作，完成后我们又可以使用Merge将通道合并，从而改变图片的色调(冷暖色调的互换)等。
更高级的图片加强，可以使用ImageEnhance模块，其中包含了大量的预定义的图片加强方式。
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- import ImageEnhance   
- enh = ImageEnhance.Contrast(im)   
- enh.ehhance(1.5).show("50% more contrast")  
```python
import ImageEnhance
enh = ImageEnhance.Contrast(im)
enh.ehhance(1.5).show("50% more contrast")
```
![](http://abruzzi.iteye.com/upload/attachment/67925/32acbe28-f089-31a6-990f-3d0e4a790486.jpg)
*读写图片的更多方式*
通常，我们使用open方法进行图片的打开操作。但是这不是唯一的方式。完全可以跟python的IO整合起来。如
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- fp = open("file.jpg", "rb")   
- im = Image.open(fp)  
```python
fp = open("file.jpg", "rb")
im = Image.open(fp)
```
甚至，你可以从一个字符串中读出图片数据来(python真是神奇啊)。
Python代码 ![复制代码](http://abruzzi.iteye.com/images/icon_copy.gif)![收藏代码](http://abruzzi.iteye.com/images/icon_star.png)![](http://abruzzi.iteye.com/images/spinner.gif)
- import StringIO   
- img = Image.open(StringIO.StringIO(buffer))  
```python
import StringIO
img = Image.open(StringIO.StringIO(buffer))
```
(文中很多内容来自pil的handbook，但是原手册比较学院派，没有一张图片来进行说明，因此不免枯燥，我给每个小例子都加上一个真实的图片作为说明，希望看起来不那没抽象。)
当然，PIL的功能远不止这些，由于篇幅原因，我这次先写这么多吧，后面会给出PIL中各个模块中函数的详细使用说明，同时尽量配以实图来说明。
