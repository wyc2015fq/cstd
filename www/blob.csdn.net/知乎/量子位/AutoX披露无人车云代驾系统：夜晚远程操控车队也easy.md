# AutoX披露无人车云代驾系统：夜晚远程操控车队也easy - 知乎
# 



> 雷刚 发自 Sanya 
量子位 报道 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-7a225dbaa94513c5f3492665b1500aa2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='720'></svg>)



无人驾驶公司AutoX又披露了新进展。


这一次，是关于他们的远程操控系统。即在无人车运营中，如需要远程介入，这个系统就显得格外重要。

画面画风，看起来跟打游戏差不多。

就权且简称**云代驾**吧。

## **AutoX云代驾方案**

AutoX通过一段夜晚远程操控的驾驶视频来体现该系统。

主要方案是通过4个广角摄像头，覆盖360度环视画面。

通过该系统，远程开车的最高速度可达45英里/小时（72千米/小时）。

当然，因为夜晚光线条件对无人驾驶的感知挑战，所以AutoX专门选了夜晚场景，也是想“悄悄”传达目前的技术实力。

而且还需指出的是，AutoX最知名的就是整个自动驾驶方案，以摄像头为主。

AutoX官方称，这个远程监控系统在白天、晚上、各种天气状况下都可以正常运行。

此外，搭配远程操控系统，AutoX开发了全新的车队管理系统及代号为Xhybrid的智能监控系统，协助调度司机、车辆及辅助人类驾驶员。

那效果究竟如何呢？

## **看疗效**

不妨一起看这个认（wan）真（de）工（zhen）作（shuang）的无人车工程师。

过弯道：



![](https://pic4.zhimg.com/v2-b0d955afd52e177a37b2887fddf80ffb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='202'></svg>)



高速场景：




![](https://pic3.zhimg.com/v2-a9a675abb35e121849d8ef28594ed7f2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='203'></svg>)



左拐来一个：




![](https://pic3.zhimg.com/v2-2bb250ac32ac5b6977bcbc6a6b592c72_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='203'></svg>)



右拐也来一发：




![](https://pic3.zhimg.com/v2-741c21ef16d4274f7255bd3b74d4d652_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='202'></svg>)



遇到黄灯：




![](https://pic3.zhimg.com/v2-4e24747bd391b2998f0cce13df4f3786_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='203'></svg>)



Stop sign（美国的停车标识，需要停车3秒后继续前进）：




![](https://pic4.zhimg.com/v2-9326c6ece3ca73b1cd158c51c371c4db_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='203'></svg>)



停车：




![](https://pic3.zhimg.com/v2-3e120f69bfcf7b60cf3295aed3484bd6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='203'></svg>)



## **要重点关注的数据**

以上，开得还不错，不过数据维度，云代驾中需要重点关注的有哪些呢？

AutoX方面分享了三点：
- 1）画面延迟（glass to camera）从摄像头到远程操控中心的计算机，画面延迟在50ms左右，可以保证顺畅沟通。
- 2）LTE网络基于多频段，同时对运营区域信号强度和稳定性做系统的测试报告地图，避开这些区域LTE网络不够稳定的地方。
- 3）可覆盖区域：可以覆盖所有达到AutoX远程操控字体LTE网络要求的区域。

## **技术准备**

此外，AutoX也分享了该系统打造背后的技术准备，依然是三点：
- 1）采用了反拥塞算法和摄像机优先级选择来提高图像的实时性。会根据现在的网络情况调整数据量，并且设置了不同摄像机的优先级，优先保证主摄像机。
- 2）AutoX正在开发更加完善的车队管理系统。该系统可以实时记录测试车辆的位置、速度和驾驶状态等信息，录制测试车辆传回的图像，并且根据情况分配远程操作人员协助指定车辆。
- 3）还研发了一套智能监控系统，代号Xhybrid，系统开发已经完成。AutoX Xhybrid支持高级人机混合决策，能从多个层次融合来自无人驾驶系统、远程监控人员的决策和规划，并用无人驾驶系统辅助人类作出更精确判断。
![](https://pic2.zhimg.com/v2-b43a0fb7165cd4b73f46cbdcd7d55149_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='710'></svg>)



最后，AutoX创始人肖健雄也分享了对远程操控系统的看法，他说这是实现未来大规模无人驾驶部署的关键一步，也是安全冗余的手段之一。


学术上，Sheridan’s Levels of Autonomy可以划分成10级，1级是完全人工遥控，10级是完全自动。AutoX开发的这种混合系统，是介于第5-7级的混合操作，是处理corner cases，比如大规模修路等的最优办法。

目前，AutoX已在美国加州推出无人驾驶生鲜递送服务：AutoX Autonomous Delivery。在此项服务的初始试运营区域，从用户叫车到无人车到达，配送完成只需要十几分钟。

更多详情可移步：

[肖健雄的无人车公司AutoX，现在要在美国配送生鲜了](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247503105%26idx%3D4%26sn%3D234e8f426d2f60757d38612d6283be10%26chksm%3De8d07a73dfa7f365d89766c915eafabfccd9b8263db64e4892d8f001e79721bc19e131cddba1%26scene%3D21%23wechat_redirect)

[“让天下没有难开的店”，宣言来自无人车公司AutoX](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247502021%26idx%3D3%26sn%3D5367de887bb32346e2cdfbd211ab640d%26chksm%3De8d07fb7dfa7f6a14a8e3af9adb645db6fa3e2e6a8a891afeb58a027595d826fe89d1a02d32c%26scene%3D21%23wechat_redirect)

[这是云代驾，不是打游戏](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495624%26idx%3D1%26sn%3D5b523c757f5a81d6bd95d1dfba035a8c%26chksm%3De8d058badfa7d1ac0c7d3ba621badb7bbbac5f1110350fb8fc8cf7f79dbcb4030650904738a1%26scene%3D21%23wechat_redirect)

— **完** —
欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)
诚挚招聘
量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。
[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者
վ'ᴗ' ի 追踪AI技术和产品新动态








