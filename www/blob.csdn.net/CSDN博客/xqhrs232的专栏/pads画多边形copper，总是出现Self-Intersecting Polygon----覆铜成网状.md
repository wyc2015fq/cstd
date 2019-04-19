# pads画多边形copper，总是出现Self-Intersecting Polygon----覆铜成网状 - xqhrs232的专栏 - CSDN博客
2013年12月25日 23:52:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：8859
原文地址::[http://blog.sina.com.cn/s/blog_66b4d47e0100hp39.html](http://blog.sina.com.cn/s/blog_66b4d47e0100hp39.html)
相关文章
1、用多边形画copper等时总是出现Self-Intersecting Polygon错误提示，解决方法  ----[http://yuandi6.blog.163.com/blog/static/207265185201210308252597/](http://yuandi6.blog.163.com/blog/static/207265185201210308252597/)
2、**用多边形画copper等时为什么总是出现Self-Intersecting Polygon错误提示----[http://www.360doc.com/content/13/0709/15/6973384_298717595.shtml](http://www.360doc.com/content/13/0709/15/6973384_298717595.shtml)**
**3、**PADS铺铜怎样设置为网状----[http://zhidao.baidu.com/link?url=GdJwSzBKZn8zqienr0Uqn9BsAUNOkcXRgmeqW0jtj_0zXougjiZIAGH2eUpiKMZSnlLbyofWTZRhRFWH2xH5qK](http://zhidao.baidu.com/link?url=GdJwSzBKZn8zqienr0Uqn9BsAUNOkcXRgmeqW0jtj_0zXougjiZIAGH2eUpiKMZSnlLbyofWTZRhRFWH2xH5qK)
4、pads如何消除铺铜po----[http://www.360doc.com/content/13/0710/09/6973384_298865234.shtml](http://www.360doc.com/content/13/0710/09/6973384_298865234.shtml)
经过自己的实践经验，Self-Intersecting Polygon 的原因是线重叠在一起（猜的）。把线改小一点就可以解决？试了一下，画线时 w1 ，问题马上解决。
//--------------------------
缩小线宽会导致破铜成网状的。出现这种情况是因为倒角线太短，拉长一些即可。。。
//=========================================================================================================================
备注::
1》缩小线宽会导致覆铜成网状的----这个确实会这样，亲自实践过，确实如此！
2》还有就是覆铜的Outline线宽设置的太小，覆铜后覆铜区域会有正方形的通孔形状存在----开始我设置为8 Mils就会有正方形的通孔存在，当我把线宽设置为10 Mils就没了，覆铜区域就是一块完整的铜皮了，如果是这个线宽得大于系统设置的Copper Clearance这个值。因为当我设置为9 Mils的线宽时，覆铜区域的正方形通孔有小了一些！！！
3》主动去找端点进行闭合，而不要去进行交叉，这样就很容易闭合成功
4》Design Rules下Clearance/Copper就是覆铜的规则----如果你的Outline画的没达到对应的要求，但Flood操作会让你避开这样的错误
5》Pads如何消除铺铜----有两个指令可以试试，一个是”po” 另一个是”spo”，因为铺铜的方式有两种，所以解除铺铜也有两种
6》----各种铺铜
COPPER 这个铜.是死铜.不会避开别的网络. 
COPPER CUT OUT 这个这个铜就是上对上面的COPPER挖铜. 
COPPER POUR 这个是灌铜.它有自开别的网络.LAYOUT推荐用这个. 
COPPER POUR  CUTOUT 同上. 
7》原本10的线，可是你的转角只有小于10，他怎么显得出来画得出来----线宽太大了，转角太小了(2014--2--14)
8》自己手工一段段线去画肯定会出很多问题，还是应该借助于软件来做-----TOP与BOT层每层画一个可以包括整块板子的Copper Pour区域，
如何用Flood命令来通过软件来进行大面积灌铜( 带智能化，会计算间距并遵从规则)，然后在大面积灌铜的基础上做修善与调整就可以了！！！

