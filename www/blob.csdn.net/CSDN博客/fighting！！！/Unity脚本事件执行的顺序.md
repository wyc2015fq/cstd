# Unity脚本事件执行的顺序 - fighting！！！ - CSDN博客
2017年06月25日 20:12:54[dujiahei](https://me.csdn.net/dujiahei)阅读数：353标签：[unity																[时间																[函数](https://so.csdn.net/so/search/s.do?q=函数&t=blog)](https://so.csdn.net/so/search/s.do?q=时间&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)
个人分类：[unity学习](https://blog.csdn.net/dujiahei/article/category/6975775)
来源：http://blog.csdn.net/u_9_5/article/details/50937198
有过c#开发经验的人都知道，程序的入口是一个类似于Main的函数，其他的代码都是以Main函数为起点。了解程序的执行循顺序对理解程序结构、判断在何处设置断点是极有帮助的。
在Unity脚本中，各事件函数的功能和顺序如下所述：
1、Awake 
Awake用于脚本唤醒。此方法为系统执行的第一个方法，用于脚本初始化，在脚本的生命周期中只执行一次。
2、Start 
Start方法在Awake之后执行，在脚本中只执行一次。 
Awake和Start函数的特性与c#中的构造函数类似，一般在Unity中都常用来初始化类的成员变量。
3、FixedUpdate 
FixedUpdate用于固定频率的更新。 
在Unity中依次单击菜单项Edit->Project Setting->Time，可以打开Time Manager面板，其中Fixed Timestep选项用于设置FixedUpdate的更新频率，默认为0.02秒，也就是50次/秒。
通常情况下，FixedUpdate()会比Update()更频繁地调用。
在Unity中，若设置Time Scale值为0，可以实现动力学特性的暂停，即所有的代码都不会被执行。可以用此特性实现游戏暂停的功能。
4、Update 
Update用于正常更新。用于帧更新后同步场景状态。 
每帧都会由系统自动调用一次。 
对于一些变量，如速度、移动距离等通常需要乘以Time.deltaTime来抵消帧率带来的影响，以此使物体状态的改变起来比较均匀、正常。 
而在FixedUpdate中，由于其更新频率固定，故不需要采用Time.deltaTime来修正状态改变频率。
5、LateUpdate 
LateUpdate用于延迟更新，此方法在Update()之后执行，每一帧调用一次。
6、OnGUI 
OnGUI用来绘制用于交互界面，在一帧中会调用多次。其中，与布局Layout和重绘Repaint相关的事件会被优先处理，然后是键盘和鼠标事件。
7、OnDestroy 
OnDestroy在当前脚本销毁时调用。若脚本中动态分配了内存，可以再OnDestroy()中进行释放。
```
using UnityEngine;
using System.Collections;
public class methoExceOrder : MonoBehaviour {
    void Awake()
    {
        print("Awake is called");
    }
    // Use this for initialization
    void Start () {
        print("Start is called");   
    }
    void OnGUI() {
        print("OnGUI is called");   
    }
    void LateUpdate() {
        print("LateUpdate is called");  
    }
    void FixedUpdate() {
        print("FixedUpdate is called");
    }
    void OnDestroy() {
        print("OnDestroy is called");
    }
    // Update is called once per frame
    void Update () {
        print("Update is called");
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
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
其他： 
在具体的项目开发中，一个项目会含有多个脚本，在一个游戏对象上也可以绑定若干个脚本，此时如果要控制脚本的执行顺序，可以通过单击菜单项 
Edit->Project Setting->Script Execution Order 打开MonoManager面板，通过单击+,-按钮和拖动操作来设置特定脚本的执行顺序。
