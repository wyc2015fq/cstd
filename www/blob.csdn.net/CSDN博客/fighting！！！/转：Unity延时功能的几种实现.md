# 转：Unity延时功能的几种实现 - fighting！！！ - CSDN博客
2017年06月21日 09:36:14[dujiahei](https://me.csdn.net/dujiahei)阅读数：5457标签：[unity																[动画																[脚本](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=动画&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)
个人分类：[unity学习](https://blog.csdn.net/dujiahei/article/category/6975775)
转自《[Unity3D项目实战笔记（5）：延时功能的几种实现](http://www.xifarm.com/invoke/)》
我所做过的系统，分单机版系统（2005年）、CS系统（2010年）、实时系统（2015年），各个系统均有“延时”功能：定时调度的； 本博客说的是实时系统中的延时功能（基于Unity3D游戏引擎）。
image
在Unity3D中，新建的脚本会默认继承MonoBehaviour，其中Update函数会被引擎自动调度，如在我的电脑上，Update函数被调用的周期为0.012s。
`那么Unity3D中，仅有通过Update函数来实现延时功能吗？  还有无其他的实现呢？`
怎么才能让一个界面只显示1秒之后 然后再执行其他的操作~
比如 游戏的胜利界面 只让他显示一秒 然后执行其他操作
答案是有的,下面就用过的几种延时功能(定时器)一一列出来：
|延时功能|是否继承MonoBehaviour|配合使用的函数|
|----|----|----|
|Update函数|是|Timer.timer|
|Invoke|是|CancelInvoke，InvokeRepeating，IsInvoking，|
|Coroutine|是|StartCoroutine ，StopCoroutine，StopAllCoroutines，|
|DOTween (HOTween v2)|否|商业授权|
|VisionTimer.VP_Timer|否|商业授权|
下面的几个例子，我们在ShowA函数中，延时5s调用ShowB函数，通过代码看看各个实现方法的异同。
为了方便，我仅截取部分关键代码，是否继承MonoBehaviour等自行添加即可。
1.- Update函数 实现定时器
```cpp
```cpp
float m_timer = 0;
    void Update()
    {
        m_timer += Time.time;
        if (m_timer >= 5)
        {
            ShowB();
            m_timer = 0;
        }
    }
    private void ShowB()
    {
    }
```
```
Update函数实现定时器，几乎是学习Unity3D必修的功课。 但是，在实战项目中需要维护m_timer这个变量，且从设计角度看，代码写得有些不整齐。
2.- Invoke函数 实现定时器
```
GameController.Get().CancelInvoke();
    GameController.Get().Invoke("ShowB", 5f);
    private void ShowB()
    {
        game.ShowCardInTurn();
    }
```
Invoke是MonoBehaviour提供的函数，可以延时调用函数，且可以通过CancelInvoke控制调度器的停止。 但是，Invoke的调用是在运行时生效的，对于ShowB函数，很容易导致在代码审查阶段被误杀—函数删除后，编译不会报错，你可能会误认为这个函数是废弃的函数。
3.- Coroutine 协程 实现定时器
```cpp
```cpp
private IEnumerator ShowA()
    {
        yield return new WaitForSeconds(5);
        ShowB();
    }
    private void ShowB()
    {
    }
```
```
使用协成的WaitForSeconds可以实现定时器，使用起来比较简单。
4.- DOTween 实现定时器
DOTween动画插件，是在Unity 4.6时代，为了弥补UGUI无动画脚本的先天不足而云起的商业插件，当然雨松MoMO的《UGUI研究院之界面中使用DoTween（七）》对普及DOTWeen功不可没。
```cpp
```cpp
private void ShowA()
    {
        GameObject go;
        go.transform.DOMoveZ(0.1f, 5f)
            .OnComplete(new TweenCallback(ShowB));
    }
    private void ShowB()
    {
    }
```
```
DOTween的主要作用不是为了做延时调度用的，但是其带有这个功能，且思想非常酷。
5.- VP_Timer 实现定时器
```
```
vp_Timer.Handle Timer = new vp_Timer.Handle();
    vp_Timer.In(5, new vp_Timer.Callback(() =>{Timer.Cancel();ShowB(); }), Timer);
```
```
VP_Timer是某天下午，在网上搜索中，无意找到了，其简洁的用法，着实让人眼前一亮。 目前项目中，涉及的地方超过10处，极大的方便了延时功能的实现，非常不错。 也为项目节约了大量的时间。
谢谢给我普及这个插件的博主啦： 《Unity时钟定时器插件——Vision Timer源码分析之一》。
上面的几种方法，在实战Unity3D项目中，均有所涉及，我本人用的比较多的是DOTWeen和VP_Timer，这2个不依赖继承MonoBehaviour，相对比较灵活，且其是商业项目，封装还是挺给力的。
[预览Tween的移动类型](http://robertpenner.com/easing/easing_demo.html)
