# Waymo官方暴力突入指南：如何制服一辆完全自动驾驶故障车 - 知乎
# 



> 鲸喜栗 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-57b138444d43403e021c1f9eb2b4efda_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='330' height='214'></svg>)
交警是个伟大的职业，在和广大司机的嘴炮里奉献自己的青春。


不过，就算是醉驾神志不清，人类也勉强能回应警察的问话。

但自动驾驶汽车就不一样了，一旦发生问题，嘴炮的机会都没有，直接上手吧。

当然，不是我撺掇他们动手的。
![](https://pic4.zhimg.com/v2-3154d9722bb105f34ade480024e8e21f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='422' height='237'></svg>)
作为自动驾驶的一枚先驱，**Waymo**希望无论是警方还是消防部门，都可以用**不那么温柔**的方式，为自动驾驶汽车的上路安全保驾护航。

为此，Waymo官方还为可能存在的第一响应者，提供了一份针对完全自动驾驶汽车 (真**·**没司机) 故障的强行突入指南。无微不至，情节紧凑，**环环相扣**。

## **温柔的窥视**

**我认识你**

第一步，发现故障车。那是一辆克莱斯勒**Pacifica Hybrid**，上面有醒目的Waymo标志，以及自动驾驶车特有的车顶组件。
![](https://pic2.zhimg.com/v2-b793985653d3fe5a1350ca7d9fd5e1d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='879' height='485'></svg>)
如果还不够的话，前叶子板上方、车头、车尾突出的传感器也是它的特征。

重要的是，每台Waymo车有自己的**编号**，在挡风玻璃和后窗上都能看到。

**有事打电话**

热线已经开通 (号码暂时被打了码) ，遇到紧急情况不知怎样处理的时候，可以和Waymo经过专业训练的工作人员联系。
![](https://pic4.zhimg.com/v2-116f84cff5f6cbabb36c9127fb9e7783_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='798' height='378'></svg>)△ 看我的编号文身
不要忘了向客服提供问题车辆的编号、车牌号，还有位置信息。

**暗中观察**

从驾驶侧的车窗观察，仪表盘上**电子手刹** (EPB) 的红灯有没有亮。

亮了就表示汽车处在“**驻车** (Park) ”状态，这时自动驾驶系统 (SDS) 已经启用电子手刹，应该不会再企图运动了。
![](https://pic2.zhimg.com/v2-3dea8a15ced10c9318bd26ec20a32c31_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='586' height='339'></svg>)
没亮的话，车辆就还在运转，你可能需要一些**物理方法**。

## **强行沟通**

**暴力突入**

打开一扇门，不让车动起来。

门锁了的话，就**敲开一扇窗**。
![](https://pic3.zhimg.com/v2-cb20d89c6ec9c3ae9d1774c08fed72ce_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='429' height='203'></svg>)△ 意外领取隔山打牛成就
实在不行给Waymo打电话，让工作人员远程开门。

如此一来，便可以获得车内操作的机会。

**E-Stop禁用自动驾驶**

终于来到车里。
![](https://pic2.zhimg.com/v2-43ebfc159bd7e4a4531ab9194f85b5a1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='754' height='590'></svg>)
一部分Waymo测试车，是有紧急停止按钮的。E-Stop按扭在前排座位之间的杯架里，按**下去**就能停用自动驾驶模式。

**要是没有E-Stop**

就只能联系Waymo。

不过，除了打电话之外，还可以按一下后排座头顶前方的“实时助攻 (**Live Help**) ”按钮。
![](https://pic4.zhimg.com/v2-9fd003077c75065f6e11dfdda4a480ef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='353' height='274'></svg>)
向工作人员申请，把车辆**调成手动驾驶模式**。获得授权之后，工作人员就会告知手动切换的方法，还是要按一堆按钮。

**关掉引擎**

把点火按钮按成关闭。按钮上面的指示灯，会告诉你关了没有。

关不上的话，就要手动剪线了。

## **忍一下可能会痛**

**低压电池剪线技巧**

打开前盖，把12V电池的线剪断，那上面有第一响应者剪线标签。这样就可以关闭引擎，停用高压电池。
![](https://pic2.zhimg.com/v2-5eab1823e98894c8bb86a0833d56519d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='403'></svg>)
熄火或者剪线成功后，前排中控会发出一阵微弱的嗡声，大概是长舒一口气吧。

官方在此友情提示，没有调到“驻车”模式的话，**不要在车前停留太久**。

**前盖打不开的话**

比如，车翻了的时候，在驾驶侧靠近后备箱的地方，可以把电池盖打开。

**高压电池禁用方式**

后备箱里有一组高压 (HV) 部件，还有给自动驾驶系统用的另外一枚12V电池。高压电池可按以下复杂步骤禁用——
![](https://pic1.zhimg.com/v2-f698dc160fa9a6d9d04782c036d1a440_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='692'></svg>)
不过，如果是处理被水淹没的车辆，可能有**触电**风险。注意远离后备箱的高压部件，以保平安。

## **打开后备箱**

先**禁用自动驾驶** (步骤见前文) ，再开后备箱。

此处要注意的是，后备箱门是电动的，**没有12V电池** (车身自带的，不是自动驾驶用的那一枚) 打不开。
![](https://pic2.zhimg.com/v2-6725590a6d58d35f79bfb53318970e6d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='992' height='560'></svg>)△ 葵花宝典正确示范
这个故事告诉我们，说明书要看全。

## **其他高能**
![](https://pic4.zhimg.com/v2-3dfe704d2bee864b6ee078b9117fc387_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)△ 十万伏特
**！**不要伤害高压设备，危险。

**！**尽量避免切断冷却管线，冷却液在高温下，易燃。

**！**拖车的时候千万不要伤了激光雷达，很贵。
![](https://pic2.zhimg.com/v2-f708417c7873b2e05c9a54d734b00fd1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='762' height='162'></svg>)△ 等引擎休息了再关门
看到这里，会不会觉得，**打开车门**才是最厉害的制动大法？




完整指南传送门：

[https://assets.documentcloud.org/documents/4457998/Waymo-Emergency-Response-Supplement.pdf](https://link.zhihu.com/?target=https%3A//assets.documentcloud.org/documents/4457998/Waymo-Emergency-Response-Supplement.pdf)


— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


