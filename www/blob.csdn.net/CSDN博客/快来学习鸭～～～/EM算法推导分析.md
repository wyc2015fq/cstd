# EM算法推导分析 - 快来学习鸭～～～ - CSDN博客





2017年09月27日 14:53:14[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：267








研一整理的纸质资料，整理一下。

EM算法推导：

![这里写图片描述](https://img-blog.csdn.net/20170927145251082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927145411101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927145442901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927145851193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927145906742?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927145916683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927145929714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927145954203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927150010573?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927150022256?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927150031254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927150041032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927150050099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170927150101591?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



