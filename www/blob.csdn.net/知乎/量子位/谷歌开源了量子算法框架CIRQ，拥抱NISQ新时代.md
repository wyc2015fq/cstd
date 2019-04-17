# 谷歌开源了量子算法框架CIRQ，拥抱NISQ新时代 - 知乎
# 



> 量子栗 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-b5508fbffdb2763d74a0a23c65599a0b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='660' height='469'></svg>)
**NISQ**，是嘈杂中型**量子** (Noisy Intermediate-Scale Quantum) 的简称。

今年年初，美国人**John Preskill**提出了这个概念。

拥有**50-100量子比特**、以及高保真量子门 (Quantum Gate) 的计算机，便可称为**NISQ计算机**。

他相信，在不远的将来，人类就能用这样的**量子计算机**，踏足**经典计算机**无力探索的新领地。

量子计算的NISQ新时代，已经到来。
![](https://pic4.zhimg.com/v2-8ebb9e42084660ae196534c0d2827317_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='860' height='460'></svg>)
今年3月，谷歌宣布拥有**72**量子比特的芯片，刷新了IBM的纪录。谷歌的计划是，**5年**实现量子技术的商业化。

当然，这个计划里不能只有**硬件**，**算法**也要跟上。

## **开源框架，为NISQ而生**

于是，谷歌开源了**Cirq**框架，这是专为**NISQ算法**打造的框架。

Cirq主要用来**短期** (Near-Term) 问题，希望帮助研究人员，了解NISQ计算机到底能不能解决，实际应用的中的计算问题。



![](https://pic2.zhimg.com/v2-cc34b6e573a052c908e35e8c059eeb39_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='64'></svg>)



这个框架，经Apache 2.0协议许可，可以修改，可以嵌入任何开源/付费的软件包。

框架安装好之后，开发者就可以给特定的量子处理器，编写它的量子算法了，据说很友好——

用户可以精确控制量子电路 (Quantum Circuits) ；为了编写和编译量子电路，数据结构是专门优化过的，让开发者能更加充分地利用NISQ架构。

另外，Cirq支持在**模拟器**上运行算法，如果将来有了量子计算机，或者更大的模拟器，也很容易通过**云**，把设备和算法**集成**起来。

## **还有应用示例**

与Cirq框架一同发布的，还有OpenFermion-Cirq，这是一个基于Cirq的应用示例。
![](https://pic1.zhimg.com/v2-8f3b967c2a5079795c1c0b12004e55c4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='67'></svg>)
介绍一下，OpenFermion是一个量子算法开发平台，专注解决化学问题。OpenFermion-Cirq则是一个开源库，把量子模拟算法编译成Cirq能用的样子。

比如，这个新库，可以用来搭建量子**变分算法** (Variational Algorithms) ，模拟**分子**或者**复杂材料**的性质。
![](https://pic4.zhimg.com/v2-963c69f231a5f6bd43931b208445fc37_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='768' height='512'></svg>)
说到这里，机智的人类可能感受到了，量子算法的开发，需要**跨学科/跨行业**的合作。

谷歌团队，在宣布Cirq开源的这篇博客里，还给出了许多额外的栗子：
![](https://pic1.zhimg.com/v2-62202d8a73e1fe480cc45d72c7710f84_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='479'></svg>)
想要探索量子世界的同学，可以前往本文底部的传送门。

## **软硬兼施**

宣布“72”这个数字的时候，谷歌团队曾经表示，他们已经很接近“量子霸权 (Quantum Supremacy) ”了。

也就是说，量子计算机的**计算速度**，超越经典计算机指日可待。

而在量子算法的路上，开源框架这一步，走得也算是很用力了。

Google AI博客传送门：
[https://ai.googleblog.com/2018/07/announcing-cirq-open-source-framework.html](https://link.zhihu.com/?target=https%3A//ai.googleblog.com/2018/07/announcing-cirq-open-source-framework.html)

GitHub传送门：
[https://github.com/quantumlib/cirq](https://link.zhihu.com/?target=https%3A//github.com/quantumlib/cirq)

OpenFermion-Cirq传送门：

[https://github.com/quantumlib/OpenFermion-Cirq](https://link.zhihu.com/?target=https%3A//github.com/quantumlib/OpenFermion-Cirq)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


