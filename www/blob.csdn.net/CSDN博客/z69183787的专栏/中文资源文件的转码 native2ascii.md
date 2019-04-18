# 中文资源文件的转码 native2ascii - z69183787的专栏 - CSDN博客
2012年11月06日 16:45:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1136
资源文件都必须是ISO-8859-1编码，因此，对于所有非西方语系的处理，都必须先将之转换为Java Unicode Escape格式。转换方法是通过JDK自带的工具native2ascii.
中文资源文件的转码 native2ascii
这个工具用法如下:
![](http://img1.51cto.com/attachment/200907/200907291248879241218.png)
如果觉得麻烦，可以直接将中文粘贴到里面，回车就可以看到转码后的结果了。
![](http://img1.51cto.com/attachment/200907/200907291248879324359.png)
