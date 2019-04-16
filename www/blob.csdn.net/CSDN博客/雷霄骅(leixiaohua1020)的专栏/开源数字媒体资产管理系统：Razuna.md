# 开源数字媒体资产管理系统：Razuna - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月26日 12:54:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：132
个人分类：[开源多媒体系统																[开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1652801)](https://blog.csdn.net/leixiaohua1020/article/category/1362938)











Razuna以一个使用Java语言编写的开源的数字媒体资产管理（Digital Asset Management）系统。功能很强大，可以用于管理各种格式的数字媒体资源。

![](https://img-blog.csdn.net/20131223231329687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


Razuna在本机的安装配置还是比较复杂的。主要是因为需要处理多种格式的视频，音频，图片等媒体格式。因此需要安装很多的视音频处理程序。

安装过程详见：[开源数字媒体资产管理系统：Razuna安装方法](http://blog.csdn.net/leixiaohua1020/article/details/16951923)

安装完成之后，启动服务器，在浏览器中输入：http://localhost:8080/razuna就可以访问系统了。

系统的登录页面如图所示：

![](https://img-blog.csdn.net/20131223230124109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

登陆之后的页面如图所示。这是一个系统管理的页面，并不是实际数字媒体资产的页面。在这个页面中可以对系统的各种参数进行配置。如果想进入实际的数字资产管理的页面，可以点击页面最上方的“Load tenant”登录进入数字资产管理页面。

注：之前因为ImageMagik里面自带的ffmpeg与我本机的ffmpeg同时加入到了环境变量中，引起了冲突。所以就把ImageMagik给卸载了。结果就是系统提示没有正确安装ImageMagik。上传图片后，无法生成图片的缩略图应该也和这个有关。

![](https://img-blog.csdn.net/20131223230130250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

参数设置页面位于系统的左边。下图显示了System Settings里面的内容。可以看出选项卡很多：Storage，Database Settings，File type等等。

![](https://img-blog.csdn.net/20131223230135703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图显示了Tenant Settings里面的内容。可以看出选项卡很多：System Settings，Meta Tags等等一系列选项。下图正在设置媒体资产的存储位置。



![](https://img-blog.csdn.net/20131223230140234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图显示了Users里面的内容。可以对用户进行管理。

![](https://img-blog.csdn.net/20131223230146921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



![](https://img-blog.csdn.net/20131223230150843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

点击页面最上方的“Load tenant”，可以登录进入数字资产管理页面。界面很像Windows的资源管理器。可以新建文件夹，上传媒体文件等等。

下图所示正准备上传两个媒体资源。一个是flv格式的视频，一个是JPEG格式的音频。

![](https://img-blog.csdn.net/20131223230201796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上传完成后，在页面中单击媒体，可以对媒体的信息进行编辑。Razuna中媒体的可编辑的信息还是相当的多的。

![](https://img-blog.csdn.net/20131223230205906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图所示是打开了一个图片文件夹，里面包含了我当时去重庆的时候照的一些照片~

![](https://img-blog.csdn.net/20131223230209703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Razuna界面通俗易懂，因此其他功能不再一一记录了。

Razuna官方网站：[http://www.razuna.org/](http://www.razuna.org/)





