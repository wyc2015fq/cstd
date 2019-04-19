# GDC2017: 《Lone Echo》中的VR动画 - 逍遥剑客 - CSDN博客
2017年06月21日 21:30:43[xoyojank](https://me.csdn.net/xoyojank)阅读数：4846
![这里写图片描述](https://img-blog.csdn.net/20170621142144658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
去年在Oculus Connect 3体验过《Lone Echo》的多人竞技玩法, 品质很高, 算是VR游戏中的一线大作了. 正好今年GDC上他们分享了一些关于VR中角色动画的一些经验, 还是比较有借鉴意义的, 毕竟Avatar做了全身的VR游戏少之又少. 
![这里写图片描述](https://img-blog.csdn.net/20170621142037584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这是一个太空失重背景下的游戏, 可以用手抓住环境物体或者推墙进行反向的漂浮. 一开始他们是通过手部射线了检测是不是抓住静态几何体, 通过让身体与双手保持与真人1:1的相对位置来驱动身体, 但是这样对于抓/推动态的环境物体效果不是很好. 原因是游戏逻辑的更新与物理引擎的更新会有次序上的问题. 
![这里写图片描述](https://img-blog.csdn.net/20170621143740741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
后来抓取的实现修改成了基于物理的Constraint来代替射线.  
![这里写图片描述](https://img-blog.csdn.net/20170621143516725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
有趣是他们在头跟手之前也加了个Constraint, 这样手抓到移动的物体上就可以带动玩家进行漂浮移动了. 
![这里写图片描述](https://img-blog.csdn.net/20170621145350809?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
手抓物体的动作在多数VR游戏中都是使用美术预先制作的动画, 但《Lone Echo》中要求可以抓住世界中的任意表面. 所以他们通过程序化的方式来计算手指抓住物体时的角度.  
![这里写图片描述](https://img-blog.csdn.net/20170621145638675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
针对单根手指来看这个问题, 其实就是求解三角形与圆盘的交点: 
1. A*搜索靠近手掌的最近三角形 
2. 根据相交边找到相邻的三角形 
3. 持续这个过程直到不在圆盘范围内 
4. 计算与相交点接触的手指角度, 取最大的那个来旋转手指 
5. 拇指使用另外的算法, 分享中没有提及 
![这里写图片描述](https://img-blog.csdn.net/20170621150527641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
为了避免手与表面穿插, 需要在手掌与表面之间做一些距离和角度的约束.  
![这里写图片描述](https://img-blog.csdn.net/20170621204351493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
虽然这样做减少了一些走样, 但是射线拾取到错误的表面会导致手掌吸咐到表面时的角度或位置偏转过大, 与真实手的位置不同步后一下就让人出戏了(影响VR体验). 所以需要选择一个同步代价最小的位置. 
![这里写图片描述](https://img-blog.csdn.net/20170621205121143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
双臂的动画是比较难模拟的, 这是因为我们只有三个点的位置(朝向): 头, 左手, 右手. 对于肩, 胸, 肘的信息是我们是不知道的, 只能通过IK算法进行估算. 
![这里写图片描述](https://img-blog.csdn.net/20170621205429416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在肩与手的位置确定的情况下, 我们只需要估算肘部的转动角度, 肘部的弯曲角度只与肩到手的距离有关系, 是确定可以计算出来的.  
![这里写图片描述](https://img-blog.csdn.net/20170621205913016?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
肩部位置的估算涉及到锁骨的伸展和朝向. 朝向指向手的方向, 伸展通过一条手肩距到伸展量的曲线映射进行估算.  
![这里写图片描述](https://img-blog.csdn.net/20170621210910582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
胸部的朝向通过混合三个方向的权重进行估算: 头的观看方向, 头到左手的方向, 头到右手的方向. 运行时也会调整这些权重的分配, 因为手离胸近的时候它的权重就小了, 如果手在身后, 那就完全忽略掉它的影响.  
![这里写图片描述](https://img-blog.csdn.net/20170621211327473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
手臂的长度是可以动态调整的, 因为虚拟手的位置必须与控制器的位置1:1进行同步.  
![这里写图片描述](https://img-blog.csdn.net/20170621211544937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
所以手臂的IK估算一共有5步. 
![这里写图片描述](https://img-blog.csdn.net/20170621212143610?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对于脊柱和双腿的运动模拟, 主要是通过角度约束从脖子一级级往下传递, 与双臂的IK是分开计算的. 由于《Lone Echo》是一个太空行走游戏, 所以不需要处理双腿与地面的走路/下蹲等, 只需要根据位置和速度进行模拟跟随躯干即可.  
对于待机/推开/观看等, 是通过叠加动画师制作的动画来实现的. 
总结: 《Lone Echo》这种太空失重背景的游戏比较少见, 所以其中的攀爬漂浮与双腿的处理不是很具备通用性, 不过手指的抓握模拟与双臂的IK解决思路是值得每个VR游戏学习的, 目前也有一些IK中间件专门针对VR下的三点驱动的FullbodyIK做出了一些尝试, 相信随着技术的成熟, 以后的VR游戏不再是只有一个头和两只手了. 
