# Morph动画的转移 - 逍遥剑客 - CSDN博客
2017年06月27日 20:29:35[xoyojank](https://me.csdn.net/xoyojank)阅读数：6891
以往做游戏换装什么的, 大多是使用骨骼来调整体型/脸型, 这算是比较成熟的技术了.  
最近的项目里尝试了使用MorphTarget(BlendShape)来做这个事情, 好处是能够做到逐顶点的调节, 可以做出比较细腻的效果, 但是与此同时带来了制作成本的上升. 不过目前AAA级的游戏中这种做法已经比较普遍了, 所以还有必要把制作流程理一下. 
常见的问题: 比如我们人的身体通过Morph做了胖瘦, 那么衣服也需要跟随身体做相应的变形. 这在使用骨骼控制时是没有问题的, 但是使用Morph后, 就需要衣服也做同样的Morph, 不然就会出现穿插的现象: 
![这里写图片描述](https://img-blog.csdn.net/20170627195023607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170627195036836?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
MorphTarget大量应用的另一个领域就是面部表情动画了, 因为表情如果使用骨骼来实现的话, 需要绑定大量的骨骼, 对于美术制作来说是一件很费时的体力劳动. 用Morph做表情动画的好处就是可以不用受骨骼的蒙皮限制, 可以做出非常夸张的表情(类似动漫里真人做不出来的效果). 不过从制作流程上来说, 一般也是需要先绑定骨骼的. 私下里跟3D美术尝试了不绑定骨骼的情况下, 使用Zbrush直接刷变形的脸, 再导回3dsmax生成Morph动画, 效果还可以, 至少在流程上来说可以节省很多工作. 表情使用Morph制作后, 有两个问题: 一是不同脸之间的Morph动画需要重做, 二是如果有胡子眉毛眼镜什么的, 也需要跟随脸部的形状变化. 
以上身体vs衣服, 脸1vs脸2, 脸vs胡子(眉毛等)之类的由Morph导致的制作成本问题, 总结下来就是需要**把现有模型上的所有Morph动画, 转移到新的模型上**. 当然靠堆人力肯定是可以解决的, 不过做为程序来说, 需要思考的就是怎么把机械劳动交给机器去做. 
以胡子为例: 
![这里写图片描述](https://img-blog.csdn.net/20170627201526671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一般游戏里, 比较省事的做法就是只做胡茬, 这样贴个图就搞定了. 也有一些游戏是通过绑定骨骼的方式来做突出脸部的长胡子. 使用了Morph后, 胡子也会面临同步Morph的问题: 
![这里写图片描述](https://img-blog.csdn.net/20170627201753856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
比如下图, 张大嘴时, 胡子没跟着动, 模型就穿插了: 
![这里写图片描述](https://img-blog.csdn.net/20170627201814347?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
给胡子加个SkinWarp修改器: 
![这里写图片描述](https://img-blog.csdn.net/20170627201913373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
把脸的顶点当成变形的参考顶点: 
![这里写图片描述](https://img-blog.csdn.net/20170627202011133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
默认的设计会有一些顶点受不到脸部顶点的影响 
![这里写图片描述](https://img-blog.csdn.net/20170627202052723?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这时就需要调节控制点的影响范围, 把所有胡子的顶点都包含进去: 
![这里写图片描述](https://img-blog.csdn.net/20170627202132246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后胡子就可以完美的跟着脸的Morph动画一起变形了: 
![这里写图片描述](https://img-blog.csdn.net/20170627202206397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是现在这样是无法导入引擎使用的, 必须烘焙成Morph动画. 
参考CryMorphTools, 把脸的Morph动画, 烘焙到胡子模型上, 这样会生成很多个Target模型: 
![这里写图片描述](https://img-blog.csdn.net/20170627202353843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后把这些Target加到胡子的Morpher修改器里生成MorphTarget: 
![这里写图片描述](https://img-blog.csdn.net/20170627202446506?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
删除SkinWrap和Target模型就完成了: 
![这里写图片描述](https://img-blog.csdn.net/20170627202527057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
由于脸与胡子的Morph动画一模一样, 所以胡子可以做出跟脸两样的变形, 也就不会再出现穿插的问题了: 
![这里写图片描述](https://img-blog.csdn.net/20170627202643608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170627202658620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170627202711672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170627202723434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
脸与脸之间的Morph表情转移需要多做一步原模型到目标模型的映射, 详细可以参考下面的Cry文档链接
# 参考资料
EASILY RETARGET BLENDSHAPES IN MAYA TO DIFFERENT SHAPED GEOMETRY 
[http://lesterbanks.com/2016/06/retarget-blendshapes-maya/](http://lesterbanks.com/2016/06/retarget-blendshapes-maya/)
Morph Targets for Additional Body Parts ? 
[https://forums.unrealengine.com/showthread.php?101816-Morph-Targets-for-Additional-Body-Parts](https://forums.unrealengine.com/showthread.php?101816-Morph-Targets-for-Additional-Body-Parts)
Adaptive Skin - Blend Shape Facial Rig Retargeting 
[https://forums.unrealengine.com/showthread.php?90681-Adaptive-Skin-Blend-Shape-Facial-Rig-Retargeting](https://forums.unrealengine.com/showthread.php?90681-Adaptive-Skin-Blend-Shape-Facial-Rig-Retargeting)
Morph Transfers Between Meshes 
[http://docs.cryengine.com/display/SDKDOC2/Morph+Transfers+Between+Meshes](http://docs.cryengine.com/display/SDKDOC2/Morph+Transfers+Between+Meshes)
