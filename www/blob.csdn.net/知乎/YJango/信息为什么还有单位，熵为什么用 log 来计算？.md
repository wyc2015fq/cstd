# 信息为什么还有单位，熵为什么用 log 来计算？ - 知乎
# 

## **前言**

学习观10 的部分疑惑可以在 11 中解除了。

信息和熵之所以有单位，是因为它们都是客观的物理量。

## **导图**
![](https://pic2.zhimg.com/v2-7b291e9a9ea3e03711416ce8ad43793d_b.jpg)
## **信息的单位**

不知道答案是ABDC哪个选项的小明，需要从 4 种等概率情况里确定实际情况，需要的信息为： ![log_k m](https://www.zhihu.com/equation?tex=log_k+m) ，
- ![m](https://www.zhihu.com/equation?tex=m) 是被测事件的等概率情况个数，上面的例子中 ![m=4](https://www.zhihu.com/equation?tex=m%3D4) 。
- ![k](https://www.zhihu.com/equation?tex=k) 是参照事件的等概率情况个数。

- 当选择的参照事件有 ![2](https://www.zhihu.com/equation?tex=2) 种等概率情况（像抛硬币）时， ![k=2](https://www.zhihu.com/equation?tex=k%3D2) ，测得的信息量单位被称为比特 ![bit](https://www.zhihu.com/equation?tex=bit) 。小明需要 ![log_2 4](https://www.zhihu.com/equation?tex=log_2+4)![bit](https://www.zhihu.com/equation?tex=bit) 信息。
- 当选择的参照事件有  ![e](https://www.zhihu.com/equation?tex=e)  (自然底数，约 ![2.71](https://www.zhihu.com/equation?tex=2.71) ) 种等概率情况时，![k=e](https://www.zhihu.com/equation?tex=k%3De) ，测得的信息量单位被称为纳特 ![nat](https://www.zhihu.com/equation?tex=nat) 。小明需要 ![log_e 4=ln4](https://www.zhihu.com/equation?tex=log_e+4%3Dln4)![nat](https://www.zhihu.com/equation?tex=nat) 信息。
- 当选择的参照事件有  ![10](https://www.zhihu.com/equation?tex=10)  种等概率情况时，![k=10](https://www.zhihu.com/equation?tex=k%3D10) ，测得的信息量单位被称为  ![ban](https://www.zhihu.com/equation?tex=ban) 。小明需要 ![log_{10}4](https://www.zhihu.com/equation?tex=log_%7B10%7D4)![ban](https://www.zhihu.com/equation?tex=ban) 信息。

把骰子的  ![6](https://www.zhihu.com/equation?tex=6)  种等概率情况作为参照事件或者  ![4](https://www.zhihu.com/equation?tex=4)  面体作为参照事件的话也可以，只是还没有常用到被赋予统一的名字。

## **信息是客观物理量？**

**引用一句话**

> The universe is conventionally described in terms of physical quantities such as mass and velocity, but a quantity at least as important as these is information [1]

**举例说明**
- 视觉信号：小红将“**写有 C 的 传纸条**”递给小明，提供 2 bits。
- 听觉信号：小红告诉小明“**答案是 C**”，提供 2 bits。
- 触觉信号：小红“**蹬小明椅子 3 次**”，提供 2 bits。 

之所以称信息是客观物理量是**因为它不随信息形式而改变**，同一个观察者，对同一件事情接受到的信息与用于传递信息的信号形式无关。 

**信息也不会消失：**[https://youtu.be/zUDqI9PJpc8](https://link.zhihu.com/?target=https%3A//youtu.be/zUDqI9PJpc8)，4:01 开始

[1]: Stone, James V. Information Theory: A Tutorial Introduction (p-17)

## **视频**
![](https://pic1.zhimg.com/v2-8a7c746057144ea28ef02b30fe51a6e4.png)信息熵的公式是怎么来的https://www.zhihu.com/video/1071774417134321664
不明白信息和熵之间的关系的请先观看 
- 四格版：[学习观10四格版：什么是信息（定性）](https://zhuanlan.zhihu.com/p/55533624)
- 原版：[老师，我没有传纸条作弊，我在学习信息论](https://zhuanlan.zhihu.com/p/55459472)

## **预告（订阅号 超智能体 提供视频下载）**

《学习观》08：[既然人类无法感知真实，又为什么要努力学习科学](https://zhuanlan.zhihu.com/p/52992693)

《学习观》09：[以为只有你会用食物诱骗小猫，网上这些招数一直在把你当小猫诱骗和洗脑](https://zhuanlan.zhihu.com/p/53722157)

《学习观》10：[老师，我没有传纸条作弊，我在学习信息论](https://zhuanlan.zhihu.com/p/55459472)

