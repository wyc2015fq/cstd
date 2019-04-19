# PADS差分信号如何画 - xqhrs232的专栏 - CSDN博客
2016年10月05日 22:01:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：655
原文地址::[http://blog.csdn.net/phenixyf/article/details/12506101](http://blog.csdn.net/phenixyf/article/details/12506101)
相关文章
1、PADS9.3如何走差分线----[http://forum.eepw.com.cn/thread/267564/1/](http://forum.eepw.com.cn/thread/267564/1/)
2、PADS PCB功能使用技巧系列 —— 如何走差分线----[http://wenku.baidu.com/link?url=5LQ9BSZ704XNyp9QNp1Bjy2Qr-37yC-cCMpOgvGb8lW7iLhny_HAqojdqgRXac_7RKmTsbRWa3zJct270lgcek9u3JT7kG0jp_DlOLxe2wK](http://wenku.baidu.com/link?url=5LQ9BSZ704XNyp9QNp1Bjy2Qr-37yC-cCMpOgvGb8lW7iLhny_HAqojdqgRXac_7RKmTsbRWa3zJct270lgcek9u3JT7kG0jp_DlOLxe2wK)
3、PADS Router高速走线指南差分走线----[http://jingyan.baidu.com/article/27fa7326c36eb546f8271fa9.html](http://jingyan.baidu.com/article/27fa7326c36eb546f8271fa9.html)

在高速设计中你可以使用到差分走线进行设计。要进行差分走线一定要了解相关的功能术语。
差分走线的目的是将差分信号的走线同时从源管脚（Source pins）走出，绕过障碍物并同时进行被控间距的走线。将两根信号走到一个相同点（称为集合点Gathering Point），即差分走线的起始点，从源管脚到集合点这部分走线我们称之为**开始区域（Start Zone）**。
在集合点之后我们必须保证差分线的走线是平行、等距的。差分走线最终在接近目标管脚位置分开（称为分离点Split Point），而最终的目标管脚我们称之为Destination pins，即差分走线的终点。从分离点到目标管脚这段区域我们称之为**结束区域（End Zone）**。
**Shoulder Length（侧翼长度）**
侧翼长度是指从起始管脚到集合点的距离或者从分离点到目标管脚的距离。
**障碍**
通常我们不希望差分线在布线的过程中绕过障碍物而使得差分线被分开，例如绕过过孔或者管脚等。但是例外的是可以在开始区域和结束区域。
提示：走线过程中点击鼠标右键选择分开命令Split Trace Segments 或Shift+X，那么在交互式走线的时候，差分线可以自动绕过走线过程中的障碍对象，然后再继续合并进行走线。
**定义差分线对 **
1、点击标准工具条上项目浏览器的图标按钮，在打开的项目浏览器Project Explorer 窗口中选择Object View 页面，点击展开窗口中的Net Objects。
![](https://img-blog.csdn.net/20140218170151812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGhlbml4eWY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140218170258296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGhlbml4eWY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、将里面的Nets 项展开。
3、选中网络1,该网络在工作区域高亮。
4、Ctrl+左键选中网络2。
5、使用Ctrl＋C 和Ctrl＋V 命令，将这两个网络复制到差分对Differential Pairs 分支中，这时你展开差分对分支可以已经添加了一对差分线对。差分线走线
以上步骤均在Router中完成
1、在未选择任何对象状态下鼠标右键选择Select Trace/Pins/Unroutes。
2、在标准工具栏中的层目录中选择Primary Component Side 为当前操作层。
3、在项目浏览器的差分对分支中选中
$7652，高亮此两个网络，在工作区域点击此两个网络中的任意一个管脚。
4、右键Interactive Route 或按F3 开始对该网络走线，可以看到两根差分线被自动地选择并进行并行走线。
5、不点击鼠标，移动光标，可以看到一对差分线的走线状态。
6、从源管脚拉出走线，在合适的位置点击鼠标左键确定第一段走线和拐角位置。
7、沿着设计的目标管脚方向继续走线。
8、当走线接近目标管脚的时候鼠标右键Complete。
在画差分信号时，如出现线宽间距报错，不能画的问题，需更改差分走线rule:
回到PADS模式，然后如下图所示设置
![](https://img-blog.csdn.net/20140508122013750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGhlbml4eWY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[http://padslayout.com/page/6/](http://padslayout.com/page/6/)
