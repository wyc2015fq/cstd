# Unity中Update和FixedUpdate的区别 - fighting！！！ - CSDN博客
2017年06月25日 19:55:21[dujiahei](https://me.csdn.net/dujiahei)阅读数：515
来源：http://blog.csdn.net/linkhai/article/details/50068601
```
一直以来用的最多的是Update，但是当在Unity中处理一些物理有关属性的更新时，应该在FixedUpdate中处理，因为效果会更好。
看看unity官方的Script Life Flowchart图（地址：http://docs.unity3d.com/Manual/ExecutionOrder.html）
```
![Script Life Flowchart](https://img-blog.csdn.net/20151127115943807)
```
显然，FixedUpdate跟Update这两个函数处于不同的“帧环”（自己发明的词）中，FixedUpdate处于Physics帧环中，而Update显然不是。所以这两个函数的使用也有了不同。
1.Update的执行受场景GameObject的渲染的影响，三角形的数量越多，渲染所需要的时间也就越长。FixedUpate的执行则不受这些影响。
2.由于前面的1的原因，Update每个渲染帧之间的间隔是不相等的，而Fixedupdate在每个渲染帧之间的时间间隔是相等的。下面的代码很好的演示了这个不同
```
```
using UnityEngine;
using System.Collections;
public class UpdateAndFixedUpdateTest : MonoBehaviour
{
    void FixedUpdate ()
    {
        Debug.Log("FixedUpdate time :" + Time.deltaTime);
    }
    void Update ()
    {
        Debug.Log("Update time :" + Time.deltaTime);
    }
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
- 15
- 16
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
- 15
- 16
```
输出结果如下：
```
![输出结果](https://img-blog.csdn.net/20151127121306441)
```
可以看到，FixedUpdate每次渲染帧之间的时间间隔是相等的，而Update却不尽相同。
3.由于1，2的原因，所以一些物理属性的更新操作应该放在FxiedUpdate中操作，比如Force，Collider等。外设的操作也是，比如说键盘或者鼠标
```
`的输入输出Input，因为这样GameObject的物理表现的更平滑，更接近现实。`
