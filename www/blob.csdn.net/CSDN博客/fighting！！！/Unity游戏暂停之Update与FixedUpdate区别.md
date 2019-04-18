# Unity游戏暂停之Update与FixedUpdate区别 - fighting！！！ - CSDN博客
2017年06月25日 20:50:09[dujiahei](https://me.csdn.net/dujiahei)阅读数：321标签：[unity																[时间																[函数](https://so.csdn.net/so/search/s.do?q=函数&t=blog)](https://so.csdn.net/so/search/s.do?q=时间&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)
个人分类：[unity学习](https://blog.csdn.net/dujiahei/article/category/6975775)
来源：http://www.cnblogs.com/zhaoqingqing/p/3296086.html
## 游戏暂停
### 示例程序
下面这段代码演示游戏暂停
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
using UnityEngine;
using System.Collections;
public class GamePauseTest : MonoBehaviour
{
    public float moveSpeed = 2.0f;
    void Update ()
    {
        //move
        transform.Translate (new Vector3 (0, 0, moveSpeed * Time.deltaTime));
    }
 
    void OnGUI ()
    {
        if (GUI.Button (new Rect (140, 0, 100, 50), "暂停")) {
            Time.timeScale = 0;
        }
        if (GUI.Button (new Rect (280, 0, 100, 50), "继续")) {
            Time.timeScale = 1;
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
### 注意事项
示例中的 速度*Time.deltaTime ，目的是保证游戏运行在 帧数不同的情况下角色移动速度都一样，因为此时角色是按时间移动与帧数无关。
## Update和FixedUpdate区别
### FPS=2的情况
![](http://images.cnitblog.com/blog/363476/201309/02110205-19526fac8ca748d3922571bd33b1cd7d.jpg)
### FPS=60的情况
![](http://images.cnitblog.com/blog/363476/201309/02111134-05aa21438f6246fe9b7b503db2da9954.jpg)
### Update和FPS有关
从上面的两幅图可以看了，当FPS为2时，update打印的次数特别少，而当FPS改为60时，update打印的次数就明显提高了很多
### 总结
update跟当前平台的帧数有关，而FixedUpdate是真实时间，所以处理物理逻辑的时候要把代码放在FixedUpdate而不是Update.
### Update
Update是在每次渲染新的一帧的时候才会调用，也就是说，这个函数的更新频率和设备的性能有关以及被渲染的物体（可以认为是三角形的数量）。在性能好的机器上可能fps 30，差的可能小些。这会导致同一个游戏在不同的机器上效果不一致，有的快有的慢。因为Update的执行间隔不一样了。
### FixedUpdate
FixedUpdate是在固定的时间间隔执行，不受游戏帧率的影响。Tick：在处理Rigidbody的时候最好用FixedUpdate。
### 修改FixedUpdaet
FixedUpdate的时间间隔可以在项目设置中更改，点击 Edit - Project Setting - time  找到 Fixed timestep，就可以修改了。
