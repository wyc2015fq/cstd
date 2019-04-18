# Unity脚本在层级面板中的执行顺序测试2 - weixin_33985507的博客 - CSDN博客
2015年12月09日 22:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
上一篇测试了生成顺序对执行顺序的影响，链接:[LINK](http://www.cnblogs.com/hont/p/4298110.html)
执行顺序测试3: [LINK](http://www.cnblogs.com/hont/p/5179427.html)
这篇主要测试一下Awake,OnEnable,Start三个常用消息的循环顺序
## 1.测试消息循环顺序
先上一个最简单的打印测试
![](https://images2015.cnblogs.com/blog/519009/201512/519009-20151209215656777-1140836010.png)
测试结果为Awake和OnEnable在第一次循环中，Start在第二次循环中
关于调用时的帧数
![](https://images2015.cnblogs.com/blog/519009/201512/519009-20151211161329668-1314886946.png)
第0帧是不做渲染的，第一帧才做渲染，并且Start延后一帧
## 2.测试动态创建时的顺序
### 2.1 - Component和GameObject都为打开状态
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
public TMP template;
void Start()
{
    var go = Instantiate(template.gameObject);
    print("--Creater Modify--");
    go.SetActive(true);
}
Dynamic Create
```
![](https://images2015.cnblogs.com/blog/519009/201512/519009-20151209215628465-668301352.png)
Awake和OnEnable优先被调用，连在此之前调用初始化的机会都没有。
如果将Prefab保存为非激活状态，并在初始化后激活。即可调用初始化代码：
![](https://images2015.cnblogs.com/blog/519009/201512/519009-20151209220628074-1750976820.png)
![](https://images2015.cnblogs.com/blog/519009/201512/519009-20151209215646121-1993905416.png)
### 2.2 - 测试GameObject为打开状态，Component非打开状态**(后手动打开)**
![](https://images2015.cnblogs.com/blog/519009/201512/519009-20151211161737433-1261477906.png)
Awake是针对GameObject，而OnEnable是针对组件Component。**所以组件不打开依然会调用Awake**
当手动打开组件之后，测试结果与之前一致。
**动态创建测试：**
![](https://images2015.cnblogs.com/blog/519009/201512/519009-20151211161711074-1320810935.png)
也和上面的测试结果吻合,不能在设置enable之后调用初始化代码。因为OnEnable是立即调用的
**Tips:**虽然Start延后一帧，但Awake/OnEnable和Start之间并不会穿插进Update/FixedUpdate的调用，
但IEnumerator Start()造成的异步执行，是会有Update/FixedUpdate调用的。
**最后归纳一下：**
1.Awake和OnEnable在第一次循环中，Start在第二次循环中。并且Start延后一帧。
2.如果是静置物体，Awake/OnEnable在第0帧调用，Start在第1帧调用。
3.OnEnable,Start消息属于组件本身，Awake属于GameObject。所以即使组件不激活，Awake也会被调用
4.在Awake/OnEnable和Start之间虽然延后一帧，但并不会穿插进Update/FixedUpdate的调用
