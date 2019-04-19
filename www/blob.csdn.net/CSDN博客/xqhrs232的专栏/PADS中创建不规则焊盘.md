# PADS中创建不规则焊盘 - xqhrs232的专栏 - CSDN博客
2016年12月05日 23:42:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2514
原文地址::[http://blog.csdn.net/yinshunjun123/article/details/26216179](http://blog.csdn.net/yinshunjun123/article/details/26216179)
相关文章
1、PADS不规则PCB封装编辑方法----[http://wenku.baidu.com/link?url=0ciBg61IxVhWqZTLuTtgeEem8ypgZ5fgPC8BhME8CZIy0-QG5o5_RGr0FLyNUcOFWCfYxxZ0PioZuXAGMRa0U1-p5HzTkRw9QpvTJ5BJyqq](http://wenku.baidu.com/link?url=0ciBg61IxVhWqZTLuTtgeEem8ypgZ5fgPC8BhME8CZIy0-QG5o5_RGr0FLyNUcOFWCfYxxZ0PioZuXAGMRa0U1-p5HzTkRw9QpvTJ5BJyqq)
2、PADS中不规则焊盘和通孔怎么画啊！画个焊盘比AD难10倍----[https://zhidao.baidu.com/question/1988615096891097387.html](https://zhidao.baidu.com/question/1988615096891097387.html)
1.首先创建一个含有三个焊盘的封装，如下:
![](https://img-blog.csdn.net/20140519093750171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWluc2h1bmp1bjEyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.接着在点击Drafting Toolbar中的Copper，开始绘制不规则的焊盘：
![](https://img-blog.csdn.net/20140519093836687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWluc2h1bmp1bjEyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3.绘制的不规则焊盘如下：
![](https://img-blog.csdn.net/20140519093900062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWluc2h1bmp1bjEyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.还没有完，还要将copper和焊盘关联起来，选中焊盘，右键点击“关联”，点击刚刚创建的copper即可将它们关联起来:
![](https://img-blog.csdn.net/20140519093926093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWluc2h1bmp1bjEyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

至此，我们完成了一个不规则焊盘的创建！
//====================================================================================================
备注::
- 
先放一个标准的[焊盘](https://www.baidu.com/s?wd=%E7%84%8A%E7%9B%98&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3njP-Pym4mHmYuWuhnWTv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHb3rjmkPH04PW64nH04P1n3Ps)
- 
用Copper指令画你想要的不规则[焊盘](https://www.baidu.com/s?wd=%E7%84%8A%E7%9B%98&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3njP-Pym4mHmYuWuhnWTv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHb3rjmkPH04PW64nH04P1n3Ps)的图形
- 
调整标准[焊盘](https://www.baidu.com/s?wd=%E7%84%8A%E7%9B%98&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3njP-Pym4mHmYuWuhnWTv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHb3rjmkPH04PW64nH04P1n3Ps)和copper所画的不择焊盘的[相对位置](https://www.baidu.com/s?wd=%E7%9B%B8%E5%AF%B9%E4%BD%8D%E7%BD%AE&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3njP-Pym4mHmYuWuhnWTv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHb3rjmkPH04PW64nH04P1n3Ps)
- 
右键点击焊盘，选择associate，点击copper
- 
当铜皮和焊盘都处于高亮状态了就标志他们是一个整体了
- 
一个异形焊盘制作成功
