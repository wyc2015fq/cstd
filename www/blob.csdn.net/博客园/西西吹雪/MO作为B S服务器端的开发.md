
# MO作为B-S服务器端的开发 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[MO作为B/S服务器端的开发](https://www.cnblogs.com/watsonyin/archive/2005/12/21/301897.html)
本人从事MO开发有近两年，现在已经转向ArcGis Engine和ArcGis Server的开发。MO作为引导我进入GIS殿堂这样的一个GIS组件，我花在它上面的心血确实太多了。回忆起以前用MO实现符号化，实现自定义出图，实现投影，实现空间分析等功能所经历过酸甜苦辣，真是爱恨交加，感慨万千。现在我要告别MO了，于是我陆陆续续地写了一些小文在讲述我所做过的一些东西，以便我对这一段开发的历史还有所记忆。本文会是我写的最后一篇关于MO的文章了。
前面写了一篇MO作为B/S客户器端的开发的小文，其实MO还可以作为B/S服务器端的GIS支持平台。要实现初级的功能如获取地图是比较简单的，但要实现复杂功能的话，那就要花太多精力了，最后可能会得不偿失，建议采用ArcIMS或ArcGis Server之类的平台。好了，废话少说，现在我们来看一看怎么样用MO实现一个简单的地图获取功能。
原理：在服务器端运行一个服务程序，该程序要有一个地图控件，并且设置好加载的图层以及符号化效果，然后打开一个端口并监听，如果收到请求，则在服务器的一个临时目录下生成一个临时图片文件。（如果各位有兴趣，我想可以做成一个WebService，这样适应能力更强，扩展性也好）。网页是能过Asp.net生成的，网页上有一个Image组件，现在程序要做的是只是获取这个图片的URL，然后将其赋给Image组件的ImageUrl属性就可以了。
第一步：服务程序的开发。
开发工具：VB。
步骤：首先在Form上放在一个Map组件和一个Winsock组件，然后在Form_Load中写下代码：
With WinSock1
.RemoteHost = "127.0.0.1"
.RemotePort = 4001
.Bind 4002
End With
并在Form_Load中进行地图的初始化：加载图层，以及符号化等。
然后在Winsock1的Data_Arrival事件中，接收客户端的请求（实际是经过Asp.net处理过的，也就是说WinSock收到的数据是在服务器端发送的）。Data_Arrival的代码大致是这样的：
Dim strData() As Byte
Dim strRequest As String
Winsock1.GetData strData
strRequest = CStr(strData)
/////下面根据strRequest对地图进行处理，并生成图片
....................
....................
Dim strFile as String
//根据需求按一定规则生成一个文件名
..............
Map1.ExportJpeg(strFile);
第二步：在网页中，请求生成图片，并显示出来。
开发工具：Asp.net，C\#
首先，发送请求，请求的内容为一个字符串：
ShowLoading();
UdpClient udpClient = new UdpClient();
Byte[] sendBytes = Encoding.ASCII.GetBytes(strRequest);
try{
udpClient.Send(sendBytes, sendBytes.Length, HostIP, 4002);
}
服务器收到请求，会成一个图片文件，假设文件的URL地址为strUrl, 现在将这个文件显示在浏览器中。
Image1.ImageUrl = strUrl;
HideLoading();
经过这几步之后，浏览器就可以看到服务器端MO生成的图片了。
上面是一个简单的例子，还可以扩展出比较强的功能，但工作量会比较大。如果要进行扩展，我认为还要做好以下几个工作：
1、定义一个服务器与客户端交互的规范，就像ArcIMS中通过规格定式的ArcXML来交互一样；
2、确定服务器端的采用的技术，是DCOM，还是WebService等，确定服务该如何提供。上面例子中方式应该来说还是很不完善的；
3、定义一种地图的工程文件，像ArcGis Desktop的MXD文件，或是ArcIMS中的axl文件。总之这种文件能够定义地图的图层及显示效果等，你的程序必须能够加载种文件。
主要就是这几条吧。我也没有偿试去做，因为我现在不需要用它了～呵呵。





posted on2005-12-21 18:04[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=301897)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
