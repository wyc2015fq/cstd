# Unity3D--Update和FixedUpdate的区别与共性 - fighting！！！ - CSDN博客
2017年06月25日 20:11:46[dujiahei](https://me.csdn.net/dujiahei)阅读数：150
来源：http://blog.csdn.net/mariohos/article/details/44256789
void Update ( ) 更新 
void FixedUpdate ( ) 固定更新
相同点：当MonoBehaviour启用时，其在每一帧被调用，都是用来更新的。 
异同点：第一点不同： 
Update()每一帧的时间不固定，即第一帧与第二帧的时间间隔t1和第三帧与第四帧的时间间隔t2不一定相同。比如：
代码片段：
```
using UnityEngine;
    using System.Collections;
    public class TestScript : MonoBehaviour
    {
         int i = 0;
         float time = 0f;
         void Update () 
         {
         i ++;
         time = Time.deltaTime;
         Debug.Log("这是第" + i + "帧")；
         Debug.Log("时间为:" + time);
         }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
![Update运行控制台](https://img-blog.csdn.net/20150314091218930)
这里t1 = 0.04 - 0.02 = 0.02；t2 = 0.55 - 0.35 = 0.2；大约相差了十倍。而如果使用FixedUpdate()会出现什么情况？
代码片段：
```
using UnityEngine;
    using System.Collections;
    public class TestScript : MonoBehaviour
    {
         int i = 0;
         float time = 0f;
         void FixedUpdate () 
         {
         i ++;
         time = Time.deltaTime;
         Debug.Log("这是第" + i + "帧")；
         Debug.Log("时间为:" + time);
         }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
![这里写图片描述](https://img-blog.csdn.net/20150314091338709)
我们很清楚地看到t1 = t2 = 0.02。也就是说，FixedUpdate()每帧与每帧之间相差的时间是固定的，而Update()不是固定的。 
第二点不同： 
Update受当前渲染的物体影响，这与当前场景中正在被渲染的物体有关（比如人物的面数，个数等），有时快有时慢，帧率会变化，Update被调用的时间间隔就会发生变化。但是FixedUpdate则不受帧率的变化影响，它是以固定的时间间隔来被调用。 
在官网文档中也提到说： 处理Rigidbody时，需要用FixedUpdate代替Update。例如:给刚体加一个作用力时，你必须应用作用力在FixedUpdate里的固定帧，而不是Update中的帧。(两者帧长不同)。正是因为FixedUpdate是以固定的时间间隔来被调用，所以人物运动起来比较平滑（运动起来不会有忽快忽慢的感觉，像是跳帧似的），这也就是为什么处理Rigidbody时要用FixedUpdate了（当然还有其他一些原因）。FixedUpdate是以固定的时间间隔来被调用的，而这个固定的时间是可以修改的。 
Edit->Project Setting->time 下的Fixed timestep。 
![这里写图片描述](https://img-blog.csdn.net/20150314091237444)
这里是0.02；与上面做的实验t1 = t2 = 0.02一致
[](http://blog.csdn.net/mariohos/article/details/44256789#)[](http://blog.csdn.net/mariohos/article/details/44256789#)[](http://blog.csdn.net/mariohos/article/details/44256789#)[](http://blog.csdn.net/mariohos/article/details/44256789#)[](http://blog.csdn.net/mariohos/article/details/44256789#)[](http://blog.csdn.net/mariohos/article/details/44256789#)
