# 在google map上显示Instagram的照片 - 小平子的专栏 - CSDN博客





2017年01月01日 23:33:53[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：2225








之前Instagram有一个在地图上显示照片的功能，可惜后来这个功能去掉了。INS的官方说法是因为这个功能的使用率很低，因此最后舍弃了这个功能。其实我觉得这个功能还蛮好的，看到自己的照片遍布全世界很有成就感啊！实验室的同学也觉得这个去掉这个功能很可惜，大家都看不到自己去过哪里了。于是同学做了一个网页用来显示Instagram的照片。大家感兴趣可以戳一下这里（需翻墙）：[Instagram Map](http://140.114.77.11/~yenhao0218/insta_map/). 代码戳这里：[Instagram Map Code](https://github.com/yenhao/Instagram-Map).

进入网页后需要登录自己的Instagram帐号，之后网页会自动读取账户里有打卡记录的照片显示在读图上。下面是我的Instagram帐号的显示效果。

![Instagram照片的显示效果](https://img-blog.csdn.net/20170101232737020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlucGluZ19zaGk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

点击照片后会显示照片的详细信息，包括拍摄日期，打卡地点以及图片的说明。效果如下。图中『奇莱南峰-3358m』即为打卡地点。这个地点是用户上传图片时从INS提供的几个候选地点中选的。 
![INS照片详细信息](https://img-blog.csdn.net/20170101232902149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlucGluZ19zaGk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是仅仅是简单地根据打卡地点将照片显示在地图上会有问题。因为很多照片的打卡地点是一样的，这样显示的照片是重叠的。好在google提供了解决方法：[MarkerCluster](https://github.com/googlemaps/js-marker-clusterer)，可以将同一地点的照片分开；同时也实现了缩放地图是照片聚集分散的效果。

其实只显示Instagram的照片对我来说不太够。我手机有5000多张照片（我是多么爱拍照~），都会同步到Google Drive（Google Drive与院校合作开放了无限量的存储空间）；但是我时不时会『精选』一些照片修下图（我是多么闷骚），然后同步到OneDrive（因为Google Drive在国内不好用，原因懂的……但是好像OneDrive现在在国内用起来也不是很方便？），现在修过的照片也累计到300多张了，是时候展现一下了……

在同学代码的基础上，我直接连接了OneDrive账户然后获取照片列表记录，之后显示在地图上。OneDrive的照片没有打卡地点，只有经纬度坐标，因此还要利用Google Map的API将经纬度转换为字符串的地址（逆向地理编码，Reverse Geocoding），因此显示的地点都怪怪的。同样地，OneDrive里的照片都没有照片描述，因此显示出来的照片就没有描述信息了，不过之后想着用Google Vision API或是Microsoft的API自动生成照片描述。当然这种机器生成的描述信息也是怪怪的，都是关键字的堆叠。

OneDrive的照片显示效果如下，只显示有坐标的照片。 
![OneDrive照片显示效果](https://img-blog.csdn.net/20170101233014447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlucGluZ19zaGk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
宝岛台湾的照片显示效果如下。分布跟Instagram差不多，只是数量有一些差别。 
![OneDrive Taiwan](https://img-blog.csdn.net/20170101233108677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlucGluZ19zaGk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击图片，显示的地点是经纬度转换而成的详细地址。 
![OneDrive照片详细信息](https://img-blog.csdn.net/20170101233150903?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlucGluZ19zaGk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
目前显示的照片有点问题，缩小地图查看时有些照片会有重叠，如下图所示，标有数字3的照片显示有问题。但其它单独显示的两种照片没有问题。放大地图显示照片时正常。还没有找到原因。估计是OneDrive获取的缩略图尺寸大小的问题或是缩小查看时多张照片叠加显示的问题。 
![照片显示重叠](https://img-blog.csdn.net/20170101234047541?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlucGluZ19zaGk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 连接OneDrive获取照片

连接OneDrive费了我一番功夫。OneDrive提供了一个简单的例子：[OneDrive Explorer JS](https://github.com/OneDrive/onedrive-explorer-js). 我的code便是在这个例子上修改而成。从OneDrive中获取照片并显示在Google Map上的代码如下：[Foto-Map](https://github.com/SJinping/Foto-Map). 目前是第一个版本，可能会有一些bug，也不会有多好看。








