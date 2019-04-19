# Activity Diagram - Jun5203 - CSDN博客
2018年10月04日 19:52:00[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：174
所属专栏：[UML](https://blog.csdn.net/column/details/26918.html)
### 前言
活动图是UML中描述系统动态行为的图之一，它用于展现参与行为的类的活动或动作。在UML里，活动图本质上就是流程图，它描述系统的活动、判定点和分支等。
### 芝士
- 活动图与流程图的区别
- 活动图支持并行活动 （主要）
- 流程图着重描述处理过程，它的主要控制结构是顺序、分支和循环，各个处理过程之间有严格的顺序和时间关系；而活动图 描述的是对象活动的顺序关系所遵循的规则，它着重表现的是系统的行为，而非系统的处理过程。
- 活动图能够表示并发活动的情形，而流程图不能
- 活动图是面向对象的，而流程图是面向过程的
- 活动图的基本要素
![在这里插入图片描述](https://img-blog.csdn.net/20181004192812196?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 活动状态Action states
- 活动状态之间的转移transitions
![在这里插入图片描述](https://img-blog.csdn.net/20181004191853204?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 判断decisions——一种表示判断决策的特殊活动
![在这里插入图片描述](https://img-blog.csdn.net/20181004191901494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 保证条件guard conditions——只有保证条件为真时转移才发生
- 同步条synchronization bar——一种表示活动之阿金的同步的特殊活动
![在这里插入图片描述](https://img-blog.csdn.net/20181004191922300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 起点和终点——起点有且只有一个，终点可有一个或多个
![在这里插入图片描述](https://img-blog.csdn.net/20181004191830665?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181004191837309?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 泳道——用于对活动中的活动进行分组，用于描述对象之间的合作关系![在这里插入图片描述](https://img-blog.csdn.net/20181004192434430?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所谓泳道技术，是将活动用线分成一些纵向区域，这些纵向区域称为泳道。每个区域代表一个特定类，或者人，或者部门的责任区。泳道技术是活动图中引入的一种面向对象机制。可为提取类及分析各个对象之间的交互提供方便。
- 活动图作用
- 描述一个操作的执行过程中所完成的工作或者动作
- 描述对象内部的工作
- 显示如何执行一组相关的动作，以及这些动作如何影响周围对象
- 描述用例的执行
- 处理多线程应用
- 以下场合不使用活动图（用序列图和协作图）
- 显示对象这些合作
- 显示对象在其生命周期内的运转情况
- 举例——图书馆管理系统图书管理员活动图
![在这里插入图片描述](https://img-blog.csdn.net/20181004194629574?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 附加——机房收费系统
在机房收费系统中，有明确活动的类包括一般用户、操作员、管理员，下面我以一般用户的部分活动为例，对上篇博客的学习情况进行检验，如有错误，还望路过的大佬们指出~
- 登录
![在这里插入图片描述](https://img-blog.csdn.net/20181004205838251?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 一般用户
![在这里插入图片描述](https://img-blog.csdn.net/20181004205855840?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 泳道
![在这里插入图片描述](https://img-blog.csdn.net/20181004205912298?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
站在巨人的肩膀上，确实是一种非常好的学习方法，今天我深有体会。刚开始的时候我对话活动图的思路不是很清晰，但是看了巨人的博客后，对整体也有了一个大概的了解。虽然各不相同，可各人有各人的特点，画的都很不错，以后继续，Come on Baby！
### 小结
昨天看完的视频，今天晚上才做总结，愧疚呀愧疚~脖子落枕“蓝瘦香菇”，有病赶紧治，真的是这样的，不仅自己遭罪，关键是耽误事儿呀！
