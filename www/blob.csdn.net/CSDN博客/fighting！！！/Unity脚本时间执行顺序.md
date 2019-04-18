# Unity脚本时间执行顺序 - fighting！！！ - CSDN博客
2017年06月25日 20:10:10[dujiahei](https://me.csdn.net/dujiahei)阅读数：397
来源：http://www.cnblogs.com/dawenhao/p/6135227.html
1.Awake
Awake用于脚本唤醒。此方法为系统执行的第一个方法，用于脚本的初始化，在脚本的生命周期中只执行一次。
2.Start
Start方法在Awake之后执行，在脚本生命周期中只执行一次。
由于Awake和Start函数的特性与C#中的构造函数类似（一般情况下，在整个脚本生命周期中只执行一次），所以在Unity中也常用来初始化类的成员变量。
3.FixedUpdate
FixedUpdate用于固定频率更新
在Unity中依次单击菜单项Edit→Project Settings→Time，可以打开Time Manager面板，其中Fixed Timestep选项用于设置FixedUpdate更新频率，默认为0.02秒，即每秒50次。
通常情况下，FixedUpdate()会比Update()更频繁的调用。当帧率较低时，在某一帧的时间间隔内FixedUpdate可能会被调用多久；而当帧率很高时，在某一帧的时间间隔内FixedUpdate可能根本不会被调用。
还有一点，在Unity中，若设置Time Scale值为0，可以实现动力学特性的暂停，即所有在FixedUpdate函数中的代码都不会被执行。（可以用这个特性来实现游戏暂停的功能）
4.Update
Update用于正常更新，即用于帧更新后同步场景状态。此方法每帧都会由系统自动调用一次。
在使用Update()时，对于一些变量，如速度、移动距离等通常需要乘以Time.deltaTime来抵消帧率带来的影响，使物体状态的改变看起来比较均匀、正常。而在FixedUpdate中，由于其更新帧率固定，所以不需要采用Time.deltaTime来修正状态改变帧率。
5.LateUpdate
LateUpdate用于延迟更新，此方法在Update()之后执行，每一帧调用一次。
6.OnGUI
OnGUI用来绘制用户交互界面，在一帧中会调用多次。其中，与布局（Layout）和重绘（Repaint）相关的事件会被优先处理，然后是键盘和鼠标事件。
7.OnDestroy
OnDestroy在当前脚本销毁时调用。若脚本中动态分配了内存，可以在OnDestroy()中进行释放。
下面脚本可以测试事件函数的顺序。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 public class MyScript : MonoBehaviour
 2 {
 3     void Awake()
 4     {
 5         print("Awake is called");
 6     }
 7 
 8     void Start()
 9     {
10         print("Start is called");
11     }
12 
13     void OnGUI()
14     {
15         print("OnGUI is called");
16     }
17 
18     void Update()
19     {
20         print("Update is called");
21     }
22 
23     void LateUpdate()
24     {
25         print("LateUpdate is called");
26     }
27 
28     void FixedUpdate()
29     {
30         print("FixedUpdate is called");
31     }
32 
33     void OnDestroy()
34     {
35         print("OnDestroy is called");
36     }
37 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images2015.cnblogs.com/blog/823252/201612/823252-20161205200415882-289892454.png)
Unity官方提供的事件顺序图。
