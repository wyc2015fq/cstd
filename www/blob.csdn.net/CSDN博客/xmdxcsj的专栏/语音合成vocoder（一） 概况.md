# 语音合成vocoder（一） 概况 - xmdxcsj的专栏 - CSDN博客





2017年05月17日 18:11:22[xmucas](https://me.csdn.net/xmdxcsj)阅读数：6690








> 
Question1: vocoder在合成中的角色？？？


## 合成概况

语音合成主要有波形拼接和参数合成两种方法[1]。

### 波形拼接方法

![这里写图片描述](https://img-blog.csdn.net/20170517180904295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

使用原始语音波形替代参数，合成的语音清晰自然，质量相比于参数合成方法要好。PSOLA（pitch synchronous overlap add）算法可以对拼接单元的韵律特征进行调整。
### 参数合成方法

提取参数->HMM建模->合成参数->波形重建 
![这里写图片描述](https://img-blog.csdn.net/20170517180933764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

合成中的[vocoder](https://zh.wikipedia.org/wiki/聲碼器)的作用主要是：提取语音信号的相关参数；根据相关参数合成出最终的语音。一些vocoder[2]如下： 

- STRAIGHT可以产生高质量的合成效果，但是速度慢 

- Real-time STRAIGHT在STRAIGHT基础上简化了算法，虽然速度变快，但是以损失性能为代价 

- TANDEM-STRAIGHT相比STRAIGHT性能差不多，但是做不到实时。 

- WORLD相比于TANDEM-STRAIGHT，它在性能不变的前提下，减少了计算复杂度，实现了实时的合成。
WORLD是基于vocoder的语音合成工具 

- F0 **DIO->Harvest**

- aperiodicity **PLANTIUM->D4C**

- spectral envelope **CheapTrick**
![这里写图片描述](https://img-blog.csdn.net/20170517180949025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
Question2: vocoder为什么可以用以上的三个参数来进行语音合成？？？


## 发声建模

![这里写图片描述](https://img-blog.csdn.net/20170521164047061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上图基于人发声机理的经典源-滤波器（source-filter）模型，其中**源激励部分**对应于肺部的气流和声带共同作用形成的激励，**声道谐振部分**对应于声道的调音运动。
### 声道模型

两种建模方法： 

- 将声道看成多个不同截面积的短管串联而成的系统，“声管模型” 

- 将声道看做一个谐振腔，“共振峰模型”
### 激励模型

声带激励分为两类，可以产生清音(/p/ /b/)或者浊音(/m/ /a/)。辅音有清有浊，而多数语言中元音均为浊音。 

- 浊音（voiced） 

气流通过紧绷的声带，对声带进行冲击而产生振动，使声门处形成准周期性的脉冲串，激励信号简化为周期性的脉冲激励。 

- 清音（unvoiced） 

声带处于松弛状态，不发生振动，气流通过声门直接进入声道，激励信号简化为随机白噪声。
上面的二元激励模型将复杂的产生激励过程简单的划为两部分，大大简化了声门激励的特征，但是合成语音的自然度较低。 

STRAIGHT引入了混合激励，浊音激励由分别通过高低通滤波器的随机噪声序列和脉冲序列相加，清音的激励源由噪声序列叠加一个位置随机的正脉冲并跟随一个负脉冲构成的脉冲对形成的爆破脉冲。[3]

### 对应关系
- F0对应于激励部分的$e(n)$中的周期脉冲序列
- spectral envelope对应于声道谐振部分的$h(n)$
- aperiodicity对应于混合激励部分的$e(n)$中的非周期序列

混合激励可以通过aperiodicity来控制浊音段中的周期激励和噪声成分的相对比重

## 声音指标
- 音高 

跟声波的基频F0有关，影响声音的声调（汉语的四种声调），跟声带震动的快慢有关
- 响度 

跟声波的振幅有关，声音的大小
- 泛音 

假设某个音的基频为f，则频率为2f的音称为第二泛音，频率为3f的音称为第三泛音
- 音色 

基音和不同泛音的能量比例关系是决定一个音的音色的核心因素。跟声带的震动频率、发音器官的送气方式、声道的形状尺寸有关。
- 共振峰 

共振峰是用来描述声源内部的共振，特别是对乐器而言，指的是共鸣箱内的共振。 

pitch跟声带有关（声带震动快慢），共振峰跟口腔的固有频率有关（不同的口型和舌位对应不同的共振峰）。
## 参考文献

[1].《语音信号处理》 韩纪庆 

[2].WORLD: a vocoder-based high-quality speech synthesis system for real-time applications 

[3].基于STRAIGHT 分析的高效语音合成算法研究










