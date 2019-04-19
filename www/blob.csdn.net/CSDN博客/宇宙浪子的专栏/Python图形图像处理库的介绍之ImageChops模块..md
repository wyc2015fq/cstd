# Python图形图像处理库的介绍之ImageChops模块. - 宇宙浪子的专栏 - CSDN博客
2015年03月12日 11:58:20[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1644
**ImageChops 模块**
这个模块主要包括对图片的算术运算，叫做通道运算(*channel operations*)。这个模块可以用于多种途径，包括一些特效制作，图片整合，算数绘图等等方面。但是到目前为止，通道运算只支持8位图片("L"和"RGB"模式，可参看前文)。
![](http://www.iteye.com/upload/attachment/80754/bbe3acaa-8804-377b-8c83-68ac599df324.jpg)
![](http://www.iteye.com/upload/attachment/80756/5d310194-d10d-3a21-9e9b-f0c72e889d84.jpg)
这次的图片主要来源于网络上的一些关于黑客帝国的壁纸:***Matrix Revolution***,同样，如有侵权行为情快速联系我。图片本身没有什么意义，主要取其对比度比较明显之处。(本来打算使用以前几篇文章中的图片，不过工作平台移植到了linux, 图片丢失了，只好重新找了几张图片，大家可以自己找图片做一些有趣的实验)
在对图片进行算术操作前，需要导入Image和ImageChops模块，打开图片，然后作处理，这些前两篇文章有过介绍，就不多说了。
**Invert:**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- ImageChops.invert(image)   
```python
ImageChops.invert(image)
```
图片反色，类似于集合操作中的求补集，最大值为Max，每个像素做减法，取出反色。
公式
out = MAX - image
![](http://www.iteye.com/upload/attachment/80758/2bc47335-f4fd-339e-a45b-e6be5468c5c3.jpg)
**lighter:**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- ImageChops.lighter(image1, image2)  
```python
ImageChops.lighter(image1, image2)
```
比较两个图片(逐像素的比较)，返回一个新的图片，这个新的图片是将两张图片中的较淡的部分的叠加。也即使说，在某一点上，两张图中，哪个的值小则取之。
公式
out = max(img1, img2)
![](http://www.iteye.com/upload/attachment/80747/76ece233-c439-387e-99ba-47775b6ec5e2.jpg)
**darker:**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- ImageChops.darker(image1, image2)  
```python
ImageChops.darker(image1, image2)
```
与lighter正好相反。
公式
out = min(img1, img2)
![](http://www.iteye.com/upload/attachment/80752/2f174ae8-3f4c-36b1-a715-7b279d464d1d.jpg)
**difference:**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- ImageChops.difference(image1, image2)  
```python
ImageChops.difference(image1, image2)
```
求出两张图片的绝对值，逐像素的做减法
公式
out = abs(img1, img2)
![](http://www.iteye.com/upload/attachment/80762/84d752d2-d605-3524-a578-c7738d340a28.jpg)
**multiply: **
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- ImageChops.multiply(image1, image2)  
```python
ImageChops.multiply(image1, image2)
```
将两张图片互相叠加，如果用纯黑色与某图片进行叠加操作，会得到一个纯黑色的图片。如果用纯白色与图片作叠加，图片不受影响。
计算的公式如下，
公式
out = img1 * img2 / MAX
(可以看到，如果时白色，MAX和MAX会约去，返回原始图片)
![](http://www.iteye.com/upload/attachment/80746/77d287eb-e641-3a26-a5af-8bd409ae93fb.jpg)
可能是图片选择的原因，大家可以下来试一下自己做一下两个图片的multipy操作。
**screen:**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- ImageChops.screen(image1, image2)  
```python
ImageChops.screen(image1, image2)
```
先反色，后叠加。
公式 
out = MAX - ((MAX - image1) * (MAX - image2) / MAX)
![](http://www.iteye.com/upload/attachment/80764/572215ab-07fb-38b2-acda-58c5a98c38f3.jpg)
**add:**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- ImageChops.add(img1, img2, scale, offset)  
```python
ImageChops.add(img1, img2, scale, offset)
```
对两张图片进行算术加法，按照一下公式进行计算
公式 
out = (img1+img2) / scale + offset
![](http://www.iteye.com/upload/attachment/80766/ff5e8621-773f-324b-bf37-1be29b9a842c.jpg)
如果尺度和偏移被忽略的化，scale=1.0, offset=0.0
即
out = img1 + img2
**subtract:**
Python代码 ![复制代码](http://www.iteye.com/images/icon_copy.gif)![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- ImageChops.subtract(img1, img2, scale, offset)  
```python
ImageChops.subtract(img1, img2, scale, offset)
```
对两张图片进行算术减法：
公式 
out = (img1-img2) / scale + offset
![](http://www.iteye.com/upload/attachment/80768/a498de1e-b30f-3af2-8543-e497ab3ceffd.jpg)
这个模块的方法比较少，也比较直观，后面会介绍一些更高级的pil模块，包括图片的加强和pil自带的小型绘图库，由于这段时间工作上比较忙，所以更新受到了一定的影响，但是我会受时间将这些模块仔细研究，并给出demo来。
越学习越觉得这个库的丰富和强大，如果可以与前段时间说的opencv for python合并的话，就可以构造出更复杂，更精巧的图形处理工具了，呵呵
