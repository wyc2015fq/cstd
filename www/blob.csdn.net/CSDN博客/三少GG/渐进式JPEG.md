# 渐进式JPEG - 三少GG - CSDN博客
2015年05月06日 09:41:56[三少GG](https://me.csdn.net/scut1135)阅读数：1855
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
### [alswl](http://www.zhihu.com/people/alswl)，前端爱好者
[鲍鳗](http://www.zhihu.com/people/lenneko)、[leilei](http://www.zhihu.com/people/leilei-59)、[陈海雁](http://www.zhihu.com/people/chen-hai-yan-88)等人赞同
感谢@吴亚桐 回答给我找到线索，感谢提问者的好问题，我为这个问题曾经困惑了多年。这种渐进式载入方法容易和其他渐进式载入混淆。 
我这里小总结一下几种渐进式载入办法。 
- js延迟载入，当浏览器滚动条拉到下面时候才显示出图片，参考 [http://www.neoease.com/lazy-load-jquery-plugin-delay-load-image/](http://www.neoease.com/lazy-load-jquery-plugin-delay-load-image/)
- 使用两张一大一小照片，小的先显示，等大图片完全下载好之后再载入。参考 [http://blog.rexsong.com/?p=929](http://blog.rexsong.com/?p=929)
- 图片渐进式技术，也就是本问题。 
- 大图片切割成小图片，逐个载入，这是互联网早期方式，现在已经几乎看不到了。太浪费http连接了。
下面是答案 
----------------------------------- 
浏览器下载图片的时候渐进式载入，这样下载完一张图可以看到它的样子，只不过只是不清楚的图，这样可以减少你等待看它的时间。 
实际上有两种方式可以实现这种渐进式效果，一种是图像隔行扫描（**Interlace**），中文可以成为交错，另外一种叫做图像渐进式扫描（**Progressive**）。 
- 隔行扫描可以在gif/png中实现。隔行GIF是指图像文件是按照隔行的方式来显示的，比如先出奇数行，再出偶数行，造成图像是逐渐变清楚的。将图像文件保存成隔行GIF格式的方法是，在 Photoshop中进行保存时，选择“Interlaced”（不要选择“Normal”），在Paint Shop Pro中进行保存时，点击“Option…”按钮，选择“Interlaced”（不要选择“Non Interlaced”）。 
- 渐进式扫描在在jpg中实现。逐级JPG文件可以让图像先以比较模糊的形式显示，随着图像文件数据不断从网上传过来，图像会逐渐变清晰。这样做的好处是图像可以尽快地显示出来，虽然图像不很完美，但是却让浏览者看到了希望，并且图像在不断地变好。将图像文件保存成逐级JPG格式的方法是，在Photoshop中进行保存时，选择“Progressive”，在Paint Shop Pro中进行保存时，点击“Option…”按钮，选择“Progressive
 encoding”。
# [使用渐进式JPEG来提升用户体验](http://www.biaodianfu.com/progressive-jpeg.html)
[技术](http://www.biaodianfu.com/category/development)[标点符](http://www.biaodianfu.com/author/admin) 2年前
 (2013-07-17)  2007℃[0评论](http://www.biaodianfu.com/progressive-jpeg.html#comments)
今天才认识到原来JPEG文件有两种保存方式他们分别是Baseline JPEG（标准型）和Progressive JPEG（渐进式）。两种格式有相同尺寸以及图像数据，他们的扩展名也是相同的，唯一的区别是二者显示的方式不同。
**Baseline JPEG**
这种类型的JPEG文件存储方式是按从上到下的扫描方式，把每一行顺序的保存在JPEG文件中。打开这个文件显示它的内容时，数据将按照存储时的顺序从上到下一行一行的被显示出来，直到所有的数据都被读完，就完成了整张图片的显示。如果文件较大或者网络下载速度较慢，那么就会看到图片被一行行加载的效果，这种格式的JPEG没有什么优点，因此，一般都推荐使用Progressive JPEG。
![baseline](http://www.biaodianfu.com/wp-content/uploads/2013/07/baseline.gif)
**Progressive JPEG**
和Baseline一遍扫描不同，Progressive JPEG文件包含多次扫描，这些扫描顺寻的存储在JPEG文件中。打开文件过程中，会先显示整个图片的模糊轮廓，随着扫描次数的增加，图片变得越来越清晰。这种格式的主要优点是在网络较慢的情况下，可以看到图片的轮廓知道正在加载的图片大概是什么。在一些网站打开较大图片时，你就会注意到这种技术。
![progressive](http://www.biaodianfu.com/wp-content/uploads/2013/07/progressive.gif)
渐进式图片带来的好处是可以让用户在没有下载完图片就可以看到最终图像的大致轮廓，一定程度上可以提升用户体验。（瀑布留的网站建议还是使用标准型的）
![baseline_vs_progressive](http://www.biaodianfu.com/wp-content/uploads/2013/07/baseline_vs_progressive.jpg)
另外渐进式的图片的大小并不会和基本的图片大小相差很多，有时候可能会比基本图片更小。渐进式的图片的缺点就是吃用户的CPU和内存，不过对于现在的电脑来说这点图片的计算并不算什么。
说了这边多下面就改讲讲怎么讲图片保存为或者转化为Progressive JPEG了。
1、PhotoShop
在photoshop中有“存储为web所用格式”，打开后选择“连续”就是渐进式JPEG。
![photoshop](http://www.biaodianfu.com/wp-content/uploads/2013/07/photoshop.png)
2、Linux
检测是否为progressive jpeg ： identify -verbose filename.jpg | grep Interlace（如果输出 None 说明不是progressive jpeg；如果输出 Plane 说明是 progressive jpeg。）
将basic jpeg转换成progressive jpeg：> convert infile.jpg -interlace Plane outfile.jpg
3、PHP
使用[imageinterlace](http://php.net/manual/en/function.imageinterlace.php)和[imagejpeg](http://www.php.net/manual/en/function.imagejpeg.php)函数我们可以轻松解决转换问题。

```
```
|123456|&lt;?php$im=imagecreatefromjpeg(&#039;pic.jpg');imageinterlace($im,1);imagejpeg($im,&#039;./php_interlaced.jpg', 100);imagedestroy($im);?&gt;|
4、Python

```
```
|12345678910|importPILfromexceptionsimportIOErrorimg=PIL.Image.open(&quot;c:\\users\\biaodianfu\\pictures\\in.jpg&quot;)destination=&quot;c:\\users\\biaodianfu\\pictures\\test.jpeg&quot;try:img.save(destination,&quot;JPEG&quot;,quality=80,optimize=True,progressive=True)exceptIOError:PIL.ImageFile.MAXBLOCK=img.size[0]*img.size[1]img.save(destination,&quot;JPEG&quot;,quality=80,optimize=True,progressive=True)|
5、jpegtran
jpegtran -copy none -progressive <inputfile> <outputfile>
6、C#

```
```
|12345678|using(Imagesource=Image.FromFile(@&quot;D:\temp\test2.jpg&quot;)){ImageCodecInfocodec=ImageCodecInfo.GetImageEncoders().First(c=&gt;c.MimeType==&quot;image/jpeg&quot;);EncoderParametersparameters=newEncoderParameters(3);parameters.Param[0]=newEncoderParameter(System.Drawing.Imaging.Encoder.Quality,100L);parameters.Param[1]=newEncoderParameter(System.Drawing.Imaging.Encoder.ScanMethod,(int)EncoderValue.ScanMethodInterlaced);parameters.Param[2]=newEncoderParameter(System.Drawing.Imaging.Encoder.RenderMethod,(int)EncoderValue.RenderProgressive);source.Save(@&quot;D:\temp\saved.jpg&quot;,codec,parameters);}|
转载请注明：[标点符](http://www.biaodianfu.com/) » [使用渐进式JPEG来提升用户体验](http://www.biaodianfu.com/progressive-jpeg.html)
# WEB前端优化：使用“渐进”图片或“交错”图片
### 一、简介  
      JPEG、GIF和PNG这三种图像格式都提供了一种功能，让图像能够更快地显示。图像可以以一种特殊方式存储，显示时先大概显示图像的草图，当文件全部下载后再[填充](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=e3ffd657e7e1b29f&k=%CC%EE%B3%E4&k0=%CC%EE%B3%E4&kdi0=0&luki=2&n=10&p=baidu&q=06011078_cpr&rb=0&rs=1&seller_id=1&sid=9fb2e1e757d6ffe3&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1922429&u=http%3A%2F%2Fwww%2Eadmin10000%2Ecom%2Fdocument%2F2437%2Ehtml&urlid=0)细节（百度图片，QQ空间，点点网等显示大图时都使用的这种方式）。这起到一种很有意义的心理效果，因为这样使人们有东西可看，而不必坐着干等大型图像慢慢显示在屏幕上。但这种效果并不是所有浏览器都支持。
　　在测试各个浏览器（未说明都为最新版本）时发现：
　　1、Chrome对“渐进”和“交错”都支持良好
　　2、Safari(PC/MAC)对“渐进”不支持，“交错支持良好”
　　3、Fiefox对“渐进”和“交错”都支持良好
　　4、Opera对“渐进”和“交错”都支持良好
　　5、IE9对“渐进”和“交错”都不支持
　　测试地址：[Interlace_test](http://www.laoshu133.com/Interlace_test/)
　　图一：基本显示效果演示
![](http://www.admin10000.com/UploadFiles/Document/201307/18/20130718092124529345.GIF)
　　图二：渐进\交错显示效果[演示](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=e3ffd657e7e1b29f&k=%D1%DD%CA%BE&k0=%D1%DD%CA%BE&kdi0=0&luki=3&n=10&p=baidu&q=06011078_cpr&rb=0&rs=1&seller_id=1&sid=9fb2e1e757d6ffe3&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1922429&u=http%3A%2F%2Fwww%2Eadmin10000%2Ecom%2Fdocument%2F2437%2Ehtml&urlid=0)
![](http://www.admin10000.com/UploadFiles/Document/201307/18/20130718092124998347.GIF)
#### 　　上图一：
　　图一中我们可以发现图片是从上倒下一条一条逐渐加载的，显示效果如图一的这种JPG压缩模式叫做顺序式编码（Sequential Encoding），一次将图像由左到右、由上到下顺序处理。也是一种常见的JPG编码模式。
#### 　　上图二：
　　图二中我们可以发现同样是一样图片，因为图片较大图，要逐渐加载完我们才知道这张图片的大致轮廓，但是在图二上，由于使用了渐进式JPG格式，在图片加载的时候我们已经可以看到了图片的大致轮廓，这种就是渐进式JPG，使用了递增式编码（Progressive Encoding）。
### 　　所以？
　　所谓的渐进式JPG格式就是采用了递增式编码的JPG，你可以通过谷歌搜索[关键字](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=e3ffd657e7e1b29f&k=%B9%D8%BC%FC%D7%D6&k0=%B9%D8%BC%FC%D7%D6&kdi0=0&luki=1&n=10&p=baidu&q=06011078_cpr&rb=0&rs=1&seller_id=1&sid=9fb2e1e757d6ffe3&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1922429&u=http%3A%2F%2Fwww%2Eadmin10000%2Ecom%2Fdocument%2F2437%2Ehtml&urlid=0)JPG
 Progressive Encoding查的更多英文的资料，因为我发现国内这方面的介绍不是很多。
　　这种JPG格式是当[图像传输](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=e3ffd657e7e1b29f&k=%CD%BC%CF%F1%B4%AB%CA%E4&k0=%CD%BC%CF%F1%B4%AB%CA%E4&kdi0=0&luki=5&n=10&p=baidu&q=06011078_cpr&rb=0&rs=1&seller_id=1&sid=9fb2e1e757d6ffe3&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1922429&u=http%3A%2F%2Fwww%2Eadmin10000%2Ecom%2Fdocument%2F2437%2Ehtml&urlid=0)的时间较长时，可将图像分数次处理，以从模糊到清晰的方式来传送图像（效果类似GIF在网络上的传输）。
### 　　二、渐进式JPEG创建
　　1. php转换
　　据说是如下的代码：  
```
`<?php`
```
$image
 = imagecreatefromjpeg($this->attach['<span id="2_nwp" style="width: auto; height: auto; float: none;"><a id="2_nwl" href="[http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=e3ffd657e7e1b29f&k=target&k0=target&kdi0=0&luki=8&n=10&p=baidu&q=06011078_cpr&rb=0&rs=1&seller_id=1&sid=9fb2e1e757d6ffe3&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1922429&u=http%3A%2F%2Fwww%2Eadmin10000%2Ecom%2Fdocument%2F2437%2Ehtml&urlid=0](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=e3ffd657e7e1b29f&k=target&k0=target&kdi0=0&luki=8&n=10&p=baidu&q=06011078_cpr&rb=0&rs=1&seller_id=1&sid=9fb2e1e757d6ffe3&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1922429&u=http%3A%2F%2Fwww%2Eadmin10000%2Ecom%2Fdocument%2F2437%2Ehtml&urlid=0)"
 target="_blank" mpid="2" style="text-decoration: none;"><span style="color:#0000ff;font-size:14px;width:auto;height:auto;float:none;">target</span></a></span>']);
```
```
//imagefilter($image,
 IMG_FILTER_MEAN_REMOVAL);
```
```
imageinterlace($image,
 1);//php的GD库中自带的函数
```
`imagejpeg($image,$this->attach['target']);//生成相应图片`
`imagedestroy($image);`
`?>`
```
　　其他语言的转换自己去搜吧。
　　2. photoshop生成
　　大伙都知道photoshop中有个“[存储](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=e3ffd657e7e1b29f&k=%B4%E6%B4%A2&k0=%B4%E6%B4%A2&kdi0=0&luki=7&n=10&p=baidu&q=06011078_cpr&rb=0&rs=1&seller_id=1&sid=9fb2e1e757d6ffe3&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1922429&u=http%3A%2F%2Fwww%2Eadmin10000%2Ecom%2Fdocument%2F2437%2Ehtml&urlid=0)为web所用格式”，然后，其中（见下图），那个连续勾选就是渐进式JPEG图片了：
![](http://www.admin10000.com/UploadFiles/Document/201307/18/20130718092124791608.JPG)
　　据说，需要勾选那个转换为sRGB选项，在某些浏览器下，图像设置为CMYK会出现一些问题！
![](http://www.admin10000.com/UploadFiles/Document/201307/18/20130718092910350666.JPG)
### 　　总结
[Scott Gilbertson](http://www.webmonkey.com/author/luxagraf/)对渐进式图片有其他的补充：
　　1. 你永不知道基本式图片内容，除非他完全加载出来；
　　2. 渐进式图片一开始大小[框架](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=e3ffd657e7e1b29f&k=%BF%F2%BC%DC&k0=%BF%F2%BC%DC&kdi0=0&luki=6&n=10&p=baidu&q=06011078_cpr&rb=0&rs=1&seller_id=1&sid=9fb2e1e757d6ffe3&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1922429&u=http%3A%2F%2Fwww%2Eadmin10000%2Ecom%2Fdocument%2F2437%2Ehtml&urlid=0)就定好，不会像基本式图片一样，由于尺寸未设定而造成回流——提高的渲染性能；
　　3. 渐进式图片也有不足，就是吃CPU吃内存。
