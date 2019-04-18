# CSS3对老式浏览器适配以及HTML5适配 - weixin_33985507的博客 - CSDN博客
2016年03月22日 16:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：22
- CSS HACK
- 老式浏览器支持HTML5
- 老式浏览器支持CSS3
#### CSS HACK
![189984-65d107dcdd3e73bd.png](https://upload-images.jianshu.io/upload_images/189984-65d107dcdd3e73bd.png)
Paste_Image.png
#### Brower
![189984-ff9d29bebfe18ad6.png](https://upload-images.jianshu.io/upload_images/189984-ff9d29bebfe18ad6.png)
Paste_Image.png
#### 几款浏览器的市场份额
![189984-b317193ba078fab1.png](https://upload-images.jianshu.io/upload_images/189984-b317193ba078fab1.png)
Paste_Image.png
#### 分辨率使用范围
![189984-70bd0893b920521a.png](https://upload-images.jianshu.io/upload_images/189984-70bd0893b920521a.png)
Paste_Image.png
#### IE6适配 缝隙缝隙问题
![189984-eabe3fa380d1f6d9.png](https://upload-images.jianshu.io/upload_images/189984-eabe3fa380d1f6d9.png)
Paste_Image.png
#### IE8 padding-left不生效的问题
![189984-6b09ab85d338f228.png](https://upload-images.jianshu.io/upload_images/189984-6b09ab85d338f228.png)
Paste_Image.png
因为在IE8中，左边部分脱离文档流 导致骑在了上面 所以padding-left设置50不起作用
故要设置200px才可以
但是，如何实现IE56的兼容性呢？
![189984-1a5bcdb86dd81350.png](https://upload-images.jianshu.io/upload_images/189984-1a5bcdb86dd81350.png)
Paste_Image.png
添加*号即可只在IE67起作用
当然也有很多HACK，这里就不一一列举了。
- 这里有篇文章 骚年秘籍拿去
[http://www.duitang.com/static/csshack.html](https://link.jianshu.com?t=http://www.duitang.com/static/csshack.html)
[http://blog.csdn.net/freshlover/article/details/12132801](https://link.jianshu.com?t=http://blog.csdn.net/freshlover/article/details/12132801)
- CSS常见BUG
[http://kayosite.com/ie6-common-css-bug.html](https://link.jianshu.com?t=http://kayosite.com/ie6-common-css-bug.html)
![189984-9a332e983a69f940.png](https://upload-images.jianshu.io/upload_images/189984-9a332e983a69f940.png)
Paste_Image.png
当然，虽然有着CSS HACK来适配浏览器版本问题 但是呢 很多大公司的网站会写如上的一句话 来使得浏览器用高版本进行适配工作
### 除了以上的方法，我们这里也有一些框架是用来专门解决这个问题的
- （鄙视）IE 嘿嘿 这里就提供了兼容IE6的方案
[http://www.bootcss.com/p/bsie/](https://link.jianshu.com?t=http://www.bootcss.com/p/bsie/)
### 让老式IE浏览器支持HTML5
- 引入下面这段代码 即可  记住一定要引导到head部分 才能生效
```
<!--[if IE]>
<script src=”http://html5shiv.googlecode.com/svn/trunk/html5.js”></script>
< ![endif]-->```
![Paste_Image.png](http://upload-images.jianshu.io/upload_images/189984-49e9843f3afbd933.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
###让老式浏览器支持CSS3
- 加入下面这句话 即可
```
behavior:url(ie-css3.htc);```
![189984-11e58ab25b8b3ba6.png](https://upload-images.jianshu.io/upload_images/189984-11e58ab25b8b3ba6.png)
Paste_Image.png
当然我这里还有篇文章，用来解决CSS3不兼容问题
[IE中的CSS3不完全兼容方案](https://link.jianshu.com?t=http://www.cnblogs.com/platero/archive/2010/08/31/1870151.html)
其中的圆角 阴影 等等 处理方式 还是非常有用的
