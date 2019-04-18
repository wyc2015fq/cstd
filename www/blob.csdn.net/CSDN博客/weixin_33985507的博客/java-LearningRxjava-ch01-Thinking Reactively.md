# java-LearningRxjava-ch01-Thinking Reactively - weixin_33985507的博客 - CSDN博客
2019年02月11日 10:55:49[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
前言
公司用到rxjava，学习Learning-Rxjava笔记
本书源码位置：[https://github.com/PacktPubli...](https://github.com/PacktPublishing/Learning-RxJava)
## 1.A brief history of ReactiveX and RxJava
作为程序员，我们的工作是Modeling our world with code，object-oriented programming 很好地抽象我们的世界，但是，As 2010 approached, it became clear that object-oriented programming only solved part of the problem.
Classes and objects do a great job of representing an entity with properties and methods,但是各个entity 之间的复杂关系的抽象，使得面向对象编程有很多模板化的代码，十分臃肿，此时，functional programming卷土重来，作为object-oriented的一个补充。Reactive
programming, a functional event-driven programming approach, began to receive special attention.微软的Erik Meijer 为.net写了一个 Reactive Extensions的框架，后来其1.0Rx和2.0ReactiveX逐渐扩展到其它语言。
Netflix 公司的Ben Christensen 和 David Karnok  作为主要人员创作了rx-java.其中
 RxJava 1.0 在2014年11月发布 , RxJava 2.0在2016年11月发布。
reactiveX的核心理念是：
The fundamental idea of ReactiveX is that events are data and data are events.
## 2.Thinking reactively
**对象都是运动和变化的**
比如天气。书本上的内容是静止不变的，但是，当读者看书时，书上的内容被读者的眼睛一点点消费，那么其内容也是变化的。也是
**对象都是相互关联的**
比如天气和人的行为。
**对象的变化都是在同时（并行）的**
比如马路上，机动车道上的汽车和非机动车道路上的人都是同时运行的。彼此还要相互观察。
所以可以使用 multiple concurrent streams of events or data happening at the same time 来modeling world
