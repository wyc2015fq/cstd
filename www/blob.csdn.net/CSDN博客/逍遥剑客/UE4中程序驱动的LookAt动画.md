# UE4中程序驱动的LookAt动画 - 逍遥剑客 - CSDN博客
2017年05月16日 00:29:23[xoyojank](https://me.csdn.net/xoyojank)阅读数：4864
很多游戏中都有, 就是角色转头+眼镜转动盯着一个目标看, 通常是盯着镜头看用的比较多 
UE4里实现这个功能很容易, 直接在AnimGraph里使用LookAt结点就可以了 
![这里写图片描述](https://img-blog.csdn.net/20170516000233076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4个LookAt分别控制脖子, 头, 左眼, 右眼的朝向, 脖子和头的角度比眼睛小, 转动速度也比眼睛慢. 
实际项目中用的时候, 有个需求是NPC播放一段动画师做的全身动画后, 再盯住玩家, 然后再播, 再盯, 如此循环. 这种情况下, UE4内置的LookAt功能就出问题了.  
在播放完动画突然开启LookAt后, NPC会先看向上一次LookAt结束的位置, 然后再转向目标点, 而不是从当前动画的骨骼朝向转过去. 这样就造成了动画的抖动, 很不和谐.  
经过分析FAnimNode_LookAt发现, 这个节点是需要一直开着的, 如果出现运行时的开关(Alpha=0), 会导致节点停止Update. 所以这个节点是没考虑当前骨骼的状态的, 每次都是从上一次的LookAt位置开始进行插值.  
本着不直接修改引擎代码的考虑, 怎么解决这个问题呢? 
1. 首先, 把FAnimNode_LookAt和相应的UAnimGraphNode_LookAt拷贝一份到工程里, 改个名, 这样就可以对其进行修改了 
![这里写图片描述](https://img-blog.csdn.net/20170516002801092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2. 如果需要从当前骨骼的Transform开始进行插值, 那么就需要在Alpha=0(EvaluateBoneTransform停止执行)时对PreviousTargetLocation进行更新, 而EvaluateBoneTransform是不能override的, 已经被父类声明成了final.  
![这里写图片描述](https://img-blog.csdn.net/20170516002147755?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3. 在PreUpdate里取出当前动画的Bone Transform, 计算出一个TargetLocation, 这样在刚开始更新时开始进行插值的初始位置就是骨骼朝向的位置了, 也就没有了看向不相干位置的问题 
4. 刚切换时还缺个FadeIn/FadeOut的插值动画, 这个我打log看明明是计算了的, 还需要跟进一下看看看为什么骨骼朝向会直接跳到目标点
