# arcgis 发布地图服务 - 左直拳的马桶_日用桶 - CSDN博客
2017年01月18日 16:18:50[左直拳](https://me.csdn.net/leftfist)阅读数：433标签：[arcgis																[gis																[地图](https://so.csdn.net/so/search/s.do?q=地图&t=blog)](https://so.csdn.net/so/search/s.do?q=gis&t=blog)](https://so.csdn.net/so/search/s.do?q=arcgis&t=blog)
个人分类：[gis](https://blog.csdn.net/leftfist/article/category/6685585)
arcgis中，地图文档需要发布，才能为WEB所用。
咋发布呢？
**1、在arcmap里面，点击 文件 - 共享为 - 服务**
**2、在弹出的对话框里选“发布服务**”，然后 
![这里写图片描述](https://img-blog.csdn.net/20170118112236783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3、这里面有点让人迷糊**
![这里写图片描述](https://img-blog.csdn.net/20170118113000927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
服务器的URL该填啥？其实这个URL，是指存放服务的GIS站点地址。所以应该填这个： 
![这里写图片描述](https://img-blog.csdn.net/20170118113324987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
得到地址：`http://localhost:6080/arcgis/manager/`
然后填上账号及密码。这个账号及密码是创建站点时设置的。如果还没有站点，则可以新建一个。
奇怪的是，当我们填好提交后，系统会自动切换成另一个地址返回给我们，不知何解。比如我填的是`http://localhost:6080/arcgis/manager/`，结果
其实，这个发布连接连接，也可以事先在ArcCatalog里维护。看来ArcCatalog就是一个集中的入口，或者类似目录之类的东西，要不咋叫catalog呢。 
![这里写图片描述](https://img-blog.csdn.net/20170118154633311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4、现在一切就绪，点分析，如果没啥错误（警告有些可以接受，有些还是会导致发布失败），就可以点发布了。
我就遇到了高级警告导致发布失败的，说数据源没有注册到站点，要打包数据发到服务上，死活发布不成功。将数据源注册到服务站点（见前面）后就可以了。注册方法如图： 
![这里写图片描述](https://img-blog.csdn.net/20170118161811975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
