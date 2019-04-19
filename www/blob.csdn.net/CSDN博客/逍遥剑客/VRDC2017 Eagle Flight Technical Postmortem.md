# VRDC2017: Eagle Flight Technical Postmortem - 逍遥剑客 - CSDN博客
2017年03月19日 22:06:31[xoyojank](https://me.csdn.net/xoyojank)阅读数：3181
Eagle Flight是我认为目前品质达到”优秀”的VR游戏, 所以关于它的一些分享是非常有参考价值的 
![这里写图片描述](https://img-blog.csdn.net/20170319215621784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个游戏有几点挑战: 
* 怎么让高速的飞行尽可能的舒适? 因为VR中的移动非常难保证不会晕 
* 规模非常庞大的场景, 还有非常多的植被 
* PC上要达到90FPS, PS4上要达到60FPS, 性能优化是一个挑战 
![这里写图片描述](https://img-blog.csdn.net/20170319215706043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第一个关键点: 飞行的方向就是你面向的方向, 没有漂移, 这样可以极大地避免眩晕的问题. 不过这样带来的问题就是没办法边飞的时候边往四周看了.  
关于加速度的问题, 很多VR游戏的制作建议是不要有, 但是这里却说有时候加速度是有必要的, 出于真实的考虑.  
![这里写图片描述](https://img-blog.csdn.net/20170319215734355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第二个关键点: 不要让玩家倾斜太大, 25度是最大值了. 这也是避免眩晕的一个手段. 游戏里是通过歪头(不是转头)来进行转向的, 倾斜超过1度才开始进行线性的转向 
![这里写图片描述](https://img-blog.csdn.net/20170319215903475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
关于撞墙: 撞墙后会变成黑屏, 但是径向流动的白色的风特效在撞墙后并不会停止, 这样会让玩家有仍然向前惯性飞行的感觉, 不会感觉很突兀地停止, 因为撞在墙上的感觉是不舒服的.  
![这里写图片描述](https://img-blog.csdn.net/20170319215839834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
晕动症的来源是视觉感知到相对运动, 但是内耳没有感觉到, 所以舒适度的优化需要从这个问题着手 
![这里写图片描述](https://img-blog.csdn.net/20170319215920819?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
人们都知道视野的中心区域是对细节敏感的, 但是很少知道视野的外围区域对运动更敏感. 所以, 视野的外围视觉的相对运动才是我们应该重点关注的. 
![这里写图片描述](https://img-blog.csdn.net/20170319215932435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这种外围视觉相对运动在游戏中主要出现在两个情况下: 一是自身转向时, 二是高速近距离飞行时(包括钻洞) 
![这里写图片描述](https://img-blog.csdn.net/20170319215948507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
游戏中引入了一种”隧道视野”, 会把外围视觉的高速移动像素给遮住, 转向时会自动把侧面挡住, 避免外围视觉感知到过多的相对运动 
![这里写图片描述](https://img-blog.csdn.net/20170319220027139?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对于近距离的物体, 通过在4个方向绑定相应的触发器进行检测, 分别控制上下左右的抛物遮挡面, 为了避免跳变, 做了Temporal Smoothing的平滑过渡 
![这里写图片描述](https://img-blog.csdn.net/20170319220050259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
性能优化部分主要提到的有: LOD, 内存, 加载时间, Overdraw, 垃圾回收. 
整个巴黎场景主要是建筑和植被, 虽然场景很大, 但是一次性加载到内存的, 不会在运行时时行流式加载, 整个城市共享两张拼接合并过的4k纹理, 一张是不透明的, 一张是Alpha Test的.  
尝试过一些优化手段, 包括遮挡剔除, 批次合并, 实例化等, 效果不大或者起反作用.  
![这里写图片描述](https://img-blog.csdn.net/20170319220111916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
LOD是通过离线预生成每个Cell的层次, 通过层级结构组织到一起, 比Unity的LOD快了2.7ms 
![这里写图片描述](https://img-blog.csdn.net/20170319220128374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
层级的遍历可以通过简单的4N+i来在不同层级之间进行索引的换算, 相比使用一个树形结构快得多 
![这里写图片描述](https://img-blog.csdn.net/20170319220139901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过Mesh压缩减少了一半的内存占用, 虽然GPU上的解压缩会带来额外的计算量, 但是带宽的节省使得最终的速度提升了0.5ms 
![这里写图片描述](https://img-blog.csdn.net/20170319220154296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
很多不可见的在模型内部面也会被绘制出来, 比如植被在建筑内部或者地下, 特别是从远处看时, 这个靠人工来优化显然是不现实的 
![这里写图片描述](https://img-blog.csdn.net/20170319220206042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170319220228621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
开发了一个减面工具, 专门干掉那些永远看不到的三角形 
![这里写图片描述](https://img-blog.csdn.net/20170319220242621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第一种尝试过的方法: 通过把每个区块用一个半球罩起来, 从半球上的每个采样点生成快照, 统计最终每个三角形上的像素数, 把低于一定阈值的干掉 
![这里写图片描述](https://img-blog.csdn.net/20170319220315797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
不过在飞行时经常会穿过一些隧道, 这种方法会连隧道里的三角形也干掉了, 所以在LOD0这一级不进行这个优化, 只在LOD1, LOD2, LOD3优化效果就很好了, 大约降低了25%~40%的面数 
![这里写图片描述](https://img-blog.csdn.net/20170319220345079?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第二种尝试过的方法: 空间划分4m的小格子, 每个格子进行一次cubemap渲染, 通过统计背面和正面三角形的比率, 标记这个格式是内部的还是外部的. 这种方法保留了洞之类的面, 但是整个城市处理一遍要12个小时. 优化办法是只处理植被, 不处理建筑, 结合十个GPU分布式计算, 可以把时间缩短到40分钟.  
最终再结合第一种方法处理LOD1和LOD2, 减少了29%的三角形, 14%的顶点, GPU时间提升了2.8ms 
![这里写图片描述](https://img-blog.csdn.net/20170319220408998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
优化加载时间: 避免使用Awake(), 分帧处理物理和音效对象等 
![这里写图片描述](https://img-blog.csdn.net/20170319220425720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
运行时的优化, 继续挑战11ms 
![这里写图片描述](https://img-blog.csdn.net/20170319220443312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
类似VR渲染时使用模板缓冲优化的思路, 在鹰鼻, 眉毛, 隧道视野方面进行模板优化, 最多节省了1ms. (鹰鼻原来不只是防眩晕用的…) 
![这里写图片描述](https://img-blog.csdn.net/20170319220458236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
公告板全部改到GPU上, 避免了遍历层级进行绘制更新 
避免使用全屏特效, 避免使用依赖深度, 只是把隧道视野从基于深度像素检测的改成基于碰撞体的, 在PS4上就节省了4.5ms 
![这里写图片描述](https://img-blog.csdn.net/20170319220513391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
另外关于多平台移植的问题, 提到了Vive的水平FOV略大, 所以中心位置跟Oculus不一样
最后总结的一点: **舒适度, 操作, 性能**三者是紧密关联的, 一个改变了, 其它两个都要相应调整
很多时候, 我们只看到某个事情”不好做”或者其他所谓的”前辈”分享的”失败经验”不就不去尝试了, 规避掉各种坑之后, 做出的东西也只能是中规中矩的水平. 就像之前很多人说的, VR里不能快速移动, VR里不能有加减速, VR里不能做Roll旋转等, 如果被这些”经验”限制住了, 根本做不出Eagle Flight这样的产品. 所以, 作为一个工程师, 需要的是不断地挑战难题, 不断地去尝试的精神, 这样才能创造更多的可能性. 
