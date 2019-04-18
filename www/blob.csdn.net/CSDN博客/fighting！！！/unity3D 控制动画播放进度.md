# unity3D 控制动画播放进度 - fighting！！！ - CSDN博客
2017年06月23日 20:10:08[dujiahei](https://me.csdn.net/dujiahei)阅读数：10618
1.如何获得当前动画播放到了第几帧~~ 
如果你指的是模型动画的话
 是没办法取到具体的帧数的 因为那个动画是连续的曲线动画。只能知道现在在第几帧和第几帧之间。
通过nbalexis的方法计算：AnimationState.normalizedTime
 的值始终都是从0到1的，然后你用这个值乘以你动画的总帧数 然后取整就可以得出你当前在哪两个帧之间。(int)变量
 进行取整。
来自：http://www.ceeger.com/forum/read.php?tid=2051&fid=2&page=2
2.
  3D引擎不同于2D，它的帧只是一个概念，30帧/s只是说曲线的关键点最小间隔，在播放的时候引擎通过时间来取曲线上对应的插值，因为每个update对于每台性能不同的电脑来说是不稳定的，update更不会去适应动画帧频，可以说大多数画面帧都不会对应到每个关键帧上（比如就算是导入1秒10帧的动画，仍然能在引擎中以每秒120帧的速度播放并且每帧都有相对运动）。所以你只能换算帧所在的时间点，通过记录动画的播放时间来判断激活事件。另外动画面板时间线下方右击也有事件激发器，但是不常用。
normalizedTime就是动画时间的百分比，你可以用这个来控制动画的播放。
来自：http://tieba.baidu.com/p/2347740881
3.Unity
 - Scripting API: AnimatorStateInfo.normalizedTime
https://docs.unity3d.com/ScriptReference/AnimatorStateInfo-normalizedTime.html
4.（转）Unity3D
 - 动作动画忽略timeScale - ☆A希亿 - 博客园
http://www.cnblogs.com/wonderKK/p/4645575.html
5.AnimationState.normalizedTime
 规范化时间
http://www.ceeger.com/Script/AnimationState/AnimationState.normalizedTime.html
6.AnimationState.normalizedTime
 归一化时间 [圣典百科]
http://wiki.ceeger.com/script/unityengine/classes/animationstate/animationstate.normalizedtime
# AnimationState.normalizedTime 归一化时间
`JavaScript` ⇒ public var **normalizedTime**:
 float; 
`C#` ⇒ public float **normalizedTime**;
#### Description 描述
The normalized time of the animation.
动画的归一化时间。
A value of 1 is the end of the animation. A value of 0.5 is the middle of the animation.
值为1是动画的结尾。值为0.5是动画的中间。
JavaScript:
```java
no example for javascript;
```
C#:
```
using UnityEngine;
using System.Collections;
 
public class ExampleScript : MonoBehaviour
{
    public Animation anim;
 
    void Start()
    {
        // Fast forward to the middle of the animation
        anim["Walk"].normalizedTime = 0.5f;
    }
}
```

