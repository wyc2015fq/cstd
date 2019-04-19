# GDC2011: Fast and Efficient Facial Rigging - 逍遥剑客 - CSDN博客
2016年07月20日 23:15:29[xoyojank](https://me.csdn.net/xoyojank)阅读数：3350标签：[GDC																[FaceRig																[Animation](https://so.csdn.net/so/search/s.do?q=Animation&t=blog)](https://so.csdn.net/so/search/s.do?q=FaceRig&t=blog)](https://so.csdn.net/so/search/s.do?q=GDC&t=blog)
个人分类：[图形技术](https://blog.csdn.net/xoyojank/article/category/337624)
讲的是Gears of War 3中的Face Rig 
![这里写图片描述](https://img-blog.csdn.net/20160720220949439)
GOW3中的角色很多, 过场动画也很多, 这就带来了一些挑战:
- 表情动画必须容易编辑
- 在外包制作的基础可以建立良好的工作流
- 动画数据可以共享给不同的模型
![这里写图片描述](https://img-blog.csdn.net/20160720221713443)
容易编辑体现在所有结点都是一条曲线, 这也保证了数据的简洁, 方便外包数据传输
![这里写图片描述](https://img-blog.csdn.net/20160720221956679)
灵感来自MGS4 XSI的文章
![这里写图片描述](https://img-blog.csdn.net/20160720222212719)
Rig分4层:
- Cage: 一个低面的几何体, 定义了面部的大部分变形区域. 因为面数低, 所以可以快速变形和进行迭代. 
- Locator: 一些固定在Cage表面的点, 跟面部动作捕捉时用的关键点类似
- Offset Rig: 跟随Locator移动, 所以调整一个Pose就是调整这些offset
- Joint: 也就是骨骼, 受Offset Rig约束
![这里写图片描述](https://img-blog.csdn.net/20160720223345388)
这里提到一个F.A.C.S(Facial Action Coding System), 非常有借鉴意义: 即面部可以使用32个动作单元组合出所有的面部运动. 一个动作单元可以定义为一个或多个面部肌肉的收缩或者放松. 
![这里写图片描述](https://img-blog.csdn.net/20160720224750192)
通过Cage的变形(Morph, DCC中常用动画制作方式的一种)带动Locator 
Locator的移动又带动了Offset Rig控制点, 同时offset也就可以计算出来了 
Joint是Offset Rig的子结点调节offset会带动Joint的移动, 从而影响蒙皮的顶点. 
因为每个角色的Face Rig结构都一样, 所以可以很方便地建立一套Pose库, 而且这些数据可以在不同角色之间进行复用 
![这里写图片描述](https://img-blog.csdn.net/20160720225402012)
![这里写图片描述](https://img-blog.csdn.net/20160720225413512)
![这里写图片描述](https://img-blog.csdn.net/20160720225519202)
转到另一张脸的原理: 把新脸当成旧脸的Morph Target, 得到的变形数据可以叠加到Pose库中的所有表情上, 这样就完成了所有Pose的转换. 
![这里写图片描述](https://img-blog.csdn.net/20160720230240602)
一些瑕疵可以通过叠加一个校正变形
![这里写图片描述](https://img-blog.csdn.net/20160720230408651)
使用wrinkle map可以增加一些皱纹效果
当然这个方案还有一些改进空间:
- 进行权重蒙皮是一个瓶颈
- 制作校正变形是一个瓶颈
解决方案就是使用一个通用的头部模型: 
![这里写图片描述](https://img-blog.csdn.net/20160720230918549)
如果需要多个不同的脸, 就制作多个不同的校正变形. 这样就不用再为每张脸进行蒙皮, 也不用再单独进行校正. 
![这里写图片描述](https://img-blog.csdn.net/20160720231308141)
感叹一句: Epic的技术美术太NB~
