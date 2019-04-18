# Unity3D之Mecanim动画系统学习笔记（五）：Animator Controller - fighting！！！ - CSDN博客
2017年07月24日 21:44:19[dujiahei](https://me.csdn.net/dujiahei)阅读数：611
                
Unity3D之Mecanim动画系统学习笔记（五）：Animator Controller - 阿诚de窝 - 博客园
http://www.cnblogs.com/hammerc/p/4828774.html
# 简介
Animator Controller在Unity中是作为一种单独的配置文件存在的文件类型，其后缀为controller，Animator Controller包含了以下几种功能：
- 可以对多个动画进行整合；
- 使用状态机来实现动画的播放和切换；
- 可以实现动画融合和分层播放；
- 可以通过脚本来对动画播放进行深度控制；
下面我们通过一个图来直观的看看动画的组成结构：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922141756928-510797327.jpg)
Animator组件用来控制人物动画的播放，其中需要的两个最核心的内容就是控制动画播放逻辑的Animator Controller以及动画骨骼Avatar对象。
# Animator组件
我们需要播放动画的角色都需要添加Animator组件，该组件即为我们控制动画的接口，下面我们来看看Animator组件：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922144710147-2018778661.jpg)
- Controller：使用的Animator Controller文件。
- Avatar：使用的骨骼文件。
- Apply Root Motion：绑定该组件的GameObject的位置是否可以由动画进行改变（如果存在改变位移的动画）。
- Update Mode：更新模式：Normal表示使用Update进行更新，Animate Physics表示使用FixUpdate进行更新（一般用在和物体有交互的情况下），Unscale Time表示无视timeScale进行更新（一般用在UI动画中）。
- Culling Mode：剔除模式：Always Animate表示即使摄像机看不见也要进行动画播放的更新，Cull Update Transform表示摄像机看不见时停止动画播放但是位置会继续更新，Cull Completely表示摄像机看不见时停止动画的所有更新。
下方还会显示我们的动画的一些主要信息。
## Animator Override Controller
我们在右键创建资源时还会发现这样的一个资源，该资源的意思是基于某个Animator Controller进行每个状态的动画的修改，其他设置不变，可以方便的复用我们已经创建好的Animator Controller。
# 创建一个Animator Controller
我们在Project视图中右击菜单中可以创建Animator Controller，下面我们看看新创建的Animator Controller：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922152236381-1792947235.jpg)
首先，我们发现的是3个默认的状态，这些状态是Unity自动帮我们创建的同时也无法删除：
- Entry：表示当进入当前状态机时的入口，该状态连接的状态会成为进入状态机后的第一个状态；
- Any State：表示任意的状态，其作用是其指向的状态是在任意时刻都可以切换过去的状态；
- Exit：表示退出当前的状态机，如果有任意状态指向该出口，表示可以从指定状态退出当前的状态机；
## 创建新状态
1.我们可以通过右键菜单进行创建：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922153118756-1782925165.jpg)
2.或者我们可以通过把一个Animation Clip拖拽到状态机窗口中进行创建；
我们发现我们创建的第一个状态被设置为默认的第一个状态，第一个状态会被标记为黄色，如下：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922153424944-897321920.jpg)
我们可以通过Inspector窗口进行该状态的设置，上篇笔记有详细的解说。
我们可以使用这两种方法来创建多个状态，同时配置好每个状态，如下：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922154844522-618763279.jpg)
## 状态切换
我们的状态机已经有了状态，但是还没有指定每个状态之间的关系，下面我们来看看该如何指定状态之间的关系。
**在Mecanim中，动画之间的播放不再是通过调用诸如“Play”之类的方法进行切换了，而是通过判断参数的变换来进行状态即动画的切换。**
我们打开Parameters面板，这里被用来设置状态机使用到的各种参数，如下：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922160421115-411306153.jpg)
点击加号可以创建一个参数，在Unity中允许我们创建4种类型的参数：
Float：float类型的参数，多用于控制状态机内部的浮点型参数；
Int：int类型的参数，多用于控制状态机内部的整型参数；
Bool：bool类型参数，多用于状态切换；
Trigger：本质上也是一个bool类型的参数，但是其值默认为false，且设置为true后系统会自动将其还原为false；
下面我们创建两个简单的Bool类型的变量，如下：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922161920334-226562848.jpg)
我们希望，当Walk为true时人物走动，为false时人物站立，Run也是一致，只不过会过渡到跑动。
下面我们通过右击状态的菜单可以拉出一条状态转换的线条，表示当前状态转换到目标状态。
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922162203256-260789742.jpg)
我们点击线条可以在Inspector窗口设定转换的条件，如下：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922162355069-846623976.jpg)
我们在条件框设置Walk为true，表示当Walk被设置为true时就从站立跳转到走路动画，同样的，走路回到站立也需要一条转换的线条，不过Walk要设置为false：
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150922162638647-995691916.jpg)
Run的设定也一致。
## Has Exit Time
我们发现线条中有一个勾选项Has Exit Time，那么它是什么意思呢？
![](http://images2015.cnblogs.com/blog/223374/201509/223374-20150923180306397-1215298187.jpg)
如果我们勾选了该项，在动画转换时会等待当前动画播放完毕才会转换到下一个动画，如果当前动画是循环动画会等待本次播放完毕时转换，所以对于需要立即转换动画的情况时记得要取消勾选。
还有一种情况时，当我当前的动画播放完毕后就自动转换到箭头所指的下一个状态（没有其他跳转条件），此时必须勾选该选项，否则动画播放完毕后就会卡在最后一帧，如果是循环动画就会一直循环播放。
## 控制动画转换
我们在绑定了Animator组件的GameObject之上添加下面的新脚本组件，就可以实现通过按键来切换人物播放的动画了：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 using UnityEngine;
 2 using System.Collections;
 3 
 4 public class TestAnimChange : MonoBehaviour
 5 {
 6     private Animator _animator;
 7 
 8     void Start()
 9     {
10         _animator = this.GetComponent<Animator>();
11     }
12     
13     void Update()
14     {
15         if(Input.GetKeyDown(KeyCode.W))
16         {
17             _animator.SetBool("Walk", true);
18         }
19         if(Input.GetKeyUp(KeyCode.W))
20         {
21             _animator.SetBool("Walk", false);
22         }
23         if(Input.GetKeyDown(KeyCode.R))
24         {
25             _animator.SetBool("Run", true);
26         }
27         if(Input.GetKeyUp(KeyCode.R))
28         {
29             _animator.SetBool("Run", false);
30         }
31     }
32 }
```
