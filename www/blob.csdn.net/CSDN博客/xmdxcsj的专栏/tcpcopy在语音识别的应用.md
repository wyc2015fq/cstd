# tcpcopy在语音识别的应用 - xmdxcsj的专栏 - CSDN博客





2015年10月09日 18:02:14[xmucas](https://me.csdn.net/xmdxcsj)阅读数：494








语音识别线上服务使用tcpcopy搭建一套测试环境，用于上线前的测试和评估，如下图所示：

![](https://img-blog.csdn.net/20151009180026876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中解码器1与线上的环境一致，解码器2对应的测试环境，对解码器任何的修改可以先到解码器2上面测试一段时间，评估与解码器1的差距。

其原理如下图所示：

![](https://img-blog.csdn.net/20151009180047534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



