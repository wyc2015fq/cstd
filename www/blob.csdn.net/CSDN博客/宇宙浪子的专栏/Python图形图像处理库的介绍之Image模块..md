# Python图形图像处理库的介绍之Image模块. - 宇宙浪子的专栏 - CSDN博客
2015年03月12日 11:51:48[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：7478
**Image模块的介绍**
**创建一个新的图片**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- Image.new(mode, size)   
- Image.new(mode, size, color)  
```python
Image.new(mode, size)
Image.new(mode, size, color)
```
**层叠图片**
层叠两个图片，img2和img2,alpha是一个介于[0,1]的浮点数，如果为0，效果为img1，如果为1.0，效果为img2。当然img1和img2的尺寸和模式必须相同。这个函数可以做出很漂亮的效果来，而图形的算术加减后边会说到。
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- Image.blend(img1, img2, alpha)  
```python
Image.blend(img1, img2, alpha)
```
![](http://www.iteye.com/upload/attachment/68202/cc554d89-fbaf-3af1-82d2-385eb57c4816.jpg)
composite可以使用另外一个图片作为蒙板(mask)，所有的这三张图片必须具备相同的尺寸，mask图片的模式可以为“1”，“L”，“RGBA”(关于模式请参看前一篇)
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- Image.composite(img1, img2, mask)  
```python
Image.composite(img1, img2, mask)
```
![](http://www.iteye.com/upload/attachment/68207/128b27c4-2e76-3d01-92a3-6fac9115beb2.jpg)
**转换图形模式**
下面看一个比较牛的方法convert，这个方法可以将图片在不同的模式间进行转换，在将灰度图转换成二值图时，所有的非零值被设置为255(白色)。灰度图的转换方式采用的是这个算法：
写道
L = R*299/1000 + G*587/1000 + B*114/1000
(此为ITU-R 610-2 亮度转换方程)
![](http://www.iteye.com/upload/attachment/68205/771df7ec-9729-33d6-b23c-3e92dbbad199.png)
![](http://www.iteye.com/upload/attachment/68224/12566e0f-1d7f-3873-91e1-80557b65337b.jpg)
![](http://www.iteye.com/upload/attachment/68223/7547bf50-0b71-3b14-945d-c463687da1c8.jpg)
![](http://www.iteye.com/upload/attachment/68227/aa838b50-5e6e-3e47-8f40-3c249d5cec6f.png)
**点操作**
img.point(function),这个function接受一个参数，且对图片中的每一个点执行这个函数，这个函数是一个匿名函数，在python之类的函数式编程语言中，可以使用
lambda表达式来完成，如
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- out = img.point(lambda i : i*1.2)#对每个点进行20%的加强
```python
out = img.point(lambda i : i*1.2)#对每个点进行20%的加强
```
如果图片是“I”或者“F”模式，那么这个lambda必须使用这样的形式
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- argument * scale + offset   
- 
- e.g   
- out = img.point(lambda i: i*1.2 + 10)  
```python
argument * scale + offset
e.g
out = img.point(lambda i: i*1.2 + 10)
```
**透明通道的使用**
putalpha(alpha)
这个方法是一个神奇的方法，你可以将一个图片(与原图尺寸相同)写入到原图片的透明通道中，而不影响原图片的正常显示，可以用于信息隐藏哦。当然，前提是原
始图片有透明通道。不过就算不是也没有多大关系，因为有PIL提供的convert功能，可以把一个图片先转换成RGBA模式，然后把要隐藏的信息文件转成“L”或者“1”模
式，最后使用这个putalpha将其叠加。而在图片的使用方，只需要简单的抽取其中的透明通道就可以看到隐藏信息了，哈哈。
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- def hideInfoInImage(img, info):   
- if img.mode != "RGBA":   
-         img = img.convert("RGBA")   
- if info.mode != "L"and info.mode != "1":   
-         info = info.convert("L")   
-     img.putalpha(info)   
- 
- return img  
```python
def hideInfoInImage(img, info):
    if img.mode != "RGBA":
        img = img.convert("RGBA")
    if info.mode != "L" and info.mode != "1":
        info = info.convert("L")
    img.putalpha(info)
    return img
```
测试之
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- if __name__ == "__main__":   
-     img = Image.open("green.png")   
-     band = Image.open("antelope_inhalf.jpg")   
- 
-     img = hideInfoInImage(img, band)   
-     img.show()#可以看到，原图片没有显式变化 
-     img.split()[3].show()#抽取出透明通道中的图片并显示
```python
if __name__ == "__main__":
    img = Image.open("green.png")
    band = Image.open("antelope_inhalf.jpg")
    img = hideInfoInImage(img, band)
    img.show()#可以看到，原图片没有显式变化
    img.split()[3].show()#抽取出透明通道中的图片并显示
```
![](http://www.iteye.com/upload/attachment/68205/771df7ec-9729-33d6-b23c-3e92dbbad199.png)
![](http://www.iteye.com/upload/attachment/68213/f64dde5e-82a0-30f6-828b-8331218fc2b3.jpg)
在windows系统中，默认的图片浏览器会把透明通道附加，可能会影响效果，可以用别的图片浏览器查看。
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- def randomPalette(length, min, max):   
- return [ randint(min, max) for x in xrange(length)]  
```python
def randomPalette(length, min, max):
    return [ randint(min, max) for x in xrange(length)]
```
**调色板的使用**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- putpalette(palette)   
-     img = Image.open("green.png").convert("L")   
- #img.show() 
-     l = randomPalette(768, 0, 255)   
-     img.putpalette(l)   
-     img.show()  
```python
putpalette(palette)
    img = Image.open("green.png").convert("L")
    #img.show()
    l = randomPalette(768, 0, 255)
    img.putpalette(l)
    img.show()
```
![](http://www.iteye.com/upload/attachment/68220/12302c92-9720-3709-99a6-202b37c78a96.png)
现将green.png转成灰度图，然后自定义一个随机调色板着色，可以看到这个效果。可能有人在想这个图形处理的作用，其实作用还是比较大的，做模式识别，人脸检
测等等操作事，真彩的图片往往很难处理，一个解决方案就是处理成灰度图或者二值图，一来可以去掉部分干扰，二来可以减少需要处理的数据量(提高效率)。
**图片转换(扭曲，映射)**
transform()方法的使用
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- transform(size, method, data)  
```python
transform(size, method, data)
```
method参数:
|EXTENT|剪一个矩形出来(用以剪切，拉伸，压缩等操作)|
|----|----|
|AFFINE|几何防射转换|
|QUAD|将一个四边形映射到一个矩形|
|MESH|将多个四边形映射到一个操作|
在EXTENT参数的情况下，data是一个四元组(x0,y0,x1,y1)，表示输入图片中需要处理的区域，size为新图片的尺寸，是一个二元组(width, height)
用途：剪贴，拉伸，压缩等操作
![](http://www.iteye.com/upload/attachment/68219/ee23ffc1-0611-34af-b6bc-7242db09e582.png)
在AFFINE参数的情况下，data是一个六元组(a,b,c, d,e,f),其中包含affine转置矩阵的前两行。对于输出图片的每个像素(x,y)，会被新值(ax+by+c, dx+ey+f)替换。
用途：反转，旋转，剪切等
![](http://www.iteye.com/upload/attachment/68199/32c675f7-ca1f-34a0-987b-399c36762cc7.png)
在QUAD参数的情况下，data是一个八元组(x0,y0,x1,y1,x2,y2,x3,y3)表示源四边形的四个定点的坐标值.MESH方式跟QUAD的效果相当，不过是对多个四边形进行映射操作.
![](http://www.iteye.com/upload/attachment/68217/e470eb69-92cb-33a3-9c95-ef604e76984f.png)
**transpose**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- im.transpose(method)  
```python
im.transpose(method)
```
method参数:
|FLIP_LEFT_RIGHT|左右倒置|
|----|----|
|FLIP_TOP_BOTTOM|上下倒置|
|ROTATE_90|旋转90度(逆时针)|
|ROTATE_180|旋转180度(逆时针)|
|ROTATE_270|旋转270度(逆时针)|
(360度?这个就不用转了吧，呵呵)
关于Image模块就介绍到这里，还有一部分属性，函数和方法没有在此做分析，这部分比较简单，如果想了解更多，可以参看pil的handbook。
