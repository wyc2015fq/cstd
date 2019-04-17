# 用VLC做流媒体服务器 - DoubleLi - 博客园






 VLC确切来说只是个播放器，是videolan的开源产品，videolan原来还有一个VLM，是服务器端，专门用来做流媒体服务器的，但是现在VLM的功能已经都集成进VLC了，所以也就可以用VLC来做流媒体服务器了。VLC同时提供了[Linux](http://lib.csdn.net/base/linux)版和windows版等多种版本，是流媒体服务器的一个不错的选择。



        首先打开VLC播放器，选择“媒体”菜单中的“流”

![](http://img.blog.csdn.net/20151020100436929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        选择之后弹出如下界面：

![](http://img.blog.csdn.net/20151020100542443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这上面有4个选项，文件就是选择一个或多个文件进行直播；光盘是选DVD光盘之类的视频光盘；网络可以将一个实时的网络流进行直播，比如网络摄像头、编码器等；捕获设备可以是当前电脑的屏幕、usb摄像头、麦克风等设备。添加一个输入源后选择串流，出现如下界面：

![](http://img.blog.csdn.net/20151020100941983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这一步显示了刚刚选择的输入源，注意这个文本框中的路径，如果做流媒体服务器用代码来调用的话，就得这样传。选择下一步：

![](http://img.blog.csdn.net/20151020101138383?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这里选择以何种方式输出，可以选择各种协议输出，我这边选择的是RTSP协议，选完了之后不是点下一步，是点旁边的添加：

![](http://img.blog.csdn.net/20151020101323987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这里选择RTSP协议的端口和路径，点击下一步：

![](http://img.blog.csdn.net/20151020101426431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这一步选择编码，如果不勾选激活转码，则源输入是什么编码，输出就是什么编码格式。勾选激活转码后，可以选择VLC自带的配置文件格式，也可以自定义一种编码格式，可指定编码格式、分辨率、码率等参数。配置完后点击下一步。

![](http://img.blog.csdn.net/20151020101718450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这是最后一步，点击“流”后，刚刚选择的文件即以RTSP协议进行直播了，再用一个VLC即可观看直播了。这里文本框中的字符串是用代码调用时传入的参数，很重要哦。



        我这里介绍的是用界面操作的方式，实际使用中VLC是提供了API的，界面操作方式最后产生的参数就是代码调用时传入的参数。









