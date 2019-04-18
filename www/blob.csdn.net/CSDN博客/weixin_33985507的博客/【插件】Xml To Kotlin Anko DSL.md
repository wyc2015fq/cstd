# 【插件】Xml To Kotlin Anko DSL - weixin_33985507的博客 - CSDN博客
2017年11月06日 20:04:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
> 
在Kotlin和Anko库出来之后，我就很少用xml来写布局了，一般都是直接用[Anko](https://link.jianshu.com?t=https://github.com/Kotlin/anko)写。和写xml一样，不但省去了解析xml的开销，而且在布局时还能进行逻辑判断，可以说非常好用了。
> 
但是问题来了，直接用代码写不能预览布局是一个很头疼的事情。我之前写的时候一边看着UI一边码布局代码，虽然说只要多写界面心里自然有那么一把称来衡量控件的属性，但还是不够直观。特别是一些复杂的布局，一分神就不知道写哪儿了，还是比较消耗精神力的。
> 
接着我就想能不能写个插件把写好的xml转成代码。用xml来预览布局，然后用插件一转换就直接变成代码。然后我就入坑了，IDEA的插件开发真的找不到什么技术博客，全是点个菜单弹个消息，有毛用？？？（摔盐！！！）
> 
我是不是有点话痨，嗯，应该还好吧。好了不废话，下面进入正题。
![2113387-fa20d3459e03f08f.png](https://upload-images.jianshu.io/upload_images/2113387-fa20d3459e03f08f.png)
plugin1.png
![2113387-b04b3bfccbe14970.png](https://upload-images.jianshu.io/upload_images/2113387-b04b3bfccbe14970.png)
plugin2.png
> 
安装完插件以后，重启生效。然后选中一个layout下面的xml文件。
![2113387-7ee73e2469926a14.png](https://upload-images.jianshu.io/upload_images/2113387-7ee73e2469926a14.png)
plugin3.png
> 
然后选择"Code"菜单下面的"Convert Layout Xml to Kotlin Anko"
![2113387-b2f4de32652e203f.png](https://upload-images.jianshu.io/upload_images/2113387-b2f4de32652e203f.png)
plugin4.png
> 
这样就会在java目录下生成anko文件夹，anko文件夹下会生成一个Activity。你就可以把里面的布局代码直接复制到你自己的Activity里面使用。
> 
最后是一些小提示：
1.该插件基于Android Studio(IDEA),kotlin和anko库。
2.生成kt文件可能打不开，把文件夹收起来再展开可能是IDE没有刷新本地文件成功，如果有大神碰到这种情况知道怎么解决请留言或发邮箱，谢谢。
3.该插件并不支持所有的属性，只支持一些常用的属性，各位也可以发属性到我邮箱，会尽量尽快添加更新插件。
4.对于不支持的属性会自动注释并提示"not support attribute"。
> 
最后的最后，[GitHub](https://link.jianshu.com?t=https://github.com/Linyuzai/LayoutXmlConverter)
