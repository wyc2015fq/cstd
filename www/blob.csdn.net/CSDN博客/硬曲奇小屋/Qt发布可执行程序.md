# Qt发布可执行程序 - 硬曲奇小屋 - CSDN博客





2018年01月10日 10:56:51[HardCookies](https://me.csdn.net/james_616)阅读数：139








# Qt发布可执行程序

之前的高光谱的项目需要做成一个带有可视化的版本，原本就打算用MFC，可是MFC和OpenCV的结合有些麻烦。所以我还是选择打算用Qt，因为本科毕设的时候也用过，并且能够跨平台。 

下面来简单介绍对于QWigets程序如何发布一个可执行程序，即拷贝到他人的计算机上也可以使用。假设采用的项目名称为Notepad。 

1. 将程序改为Release版本并编译运行。 
![这里写图片描述](https://img-blog.csdn.net/20180110103915500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. 将release文件夹内的**.exe**文件拷贝到一个新建文件夹中。 
![这里写图片描述](https://img-blog.csdn.net/20180110104005879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3. 在开始菜单下打开Qt的控制台程序。 
![这里写图片描述](https://img-blog.csdn.net/20180110104047487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4. 定位到刚刚新建的文件夹的地址，键入以下代码 
`windeployqt Notepad.exe`

Qt的内置程序会自动添加需要的组建。 
![这里写图片描述](https://img-blog.csdn.net/20180110104109201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

5. 添加后的文件夹如下图所示。 
![这里写图片描述](https://img-blog.csdn.net/20180110104123941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6. 运行其中的**Notepoad.exe**，程序正常运行。 
![这里写图片描述](https://img-blog.csdn.net/20180110104148491?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

好啦！这样我们就可以把我们的小程序给更多的小伙伴分享啦！



