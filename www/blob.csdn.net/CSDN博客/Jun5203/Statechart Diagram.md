# Statechart Diagram - Jun5203 - CSDN博客
2018年10月05日 09:28:56[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：125
所属专栏：[UML](https://blog.csdn.net/column/details/26918.html)
### 前言
状态图指的是状态的变迁，展现从状态到状态的控制流，介绍系统行为特征。例如：电灯由关到开，改变了它的状态。
### 芝士
- 什么是状态图？
状态图描述一个特定对象的所有可能状态以及由于各种事件的发生而引起的状态之间的转移。
- 什么时候需要画状态图？
一个软件系统存在着复杂的状态变换，需要对这些状态进行软件的模拟和监控，为了保持这些状态转换的一致性不会出现其他异常非法状态。例如：10086的呼叫中心系统，语音提示后不同的按键进入不同的状态。
- 状态图的图符
- 状态
- 转移
- 起点
- 终点
![在这里插入图片描述](https://img-blog.csdn.net/20181005084254356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 什么是状态机？
它描述了一个对象或一个交互在生命期内响应事件所经历的状态序列，如描述单个类或一组类之间协作的行为
- 状态机的组成
- 状态
它描述了状态机所在对象状态行为的执行所产生的结果
- 名称name
- 入口/出口动作entry/exit action
- 内部转换Internal Transition
- 延迟事件Deferred Event
- 子状态Substate
![在这里插入图片描述](https://img-blog.csdn.net/20181005091309464?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 转换（从一个状态到另一个状态的流）
- 当某实体在第一个状态中执行一定的动作，并在某个特定事件发生而某个特定的条件满足时进入下一个状态。
- 源状态 Source State
- 目标状态Target State
- 触发事件Trigger Event
- 监护条件Guard Condition
- 动作Action
![在这里插入图片描述](https://img-blog.csdn.net/20181005091943523?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 事件（触发转换的元素）
- 状态图建模步骤
- 识别一个要对其生命周期进行描述的参与行为的类
- 对状态建模，即确定对象可能存在的状态
- 对事件建模，即确定对象可能存在的事件
- 对动作建模，即确定当转变被激活时，相应被执行的动作
- 对建模结果进行精化和细化
- 举例——图书馆管理系统
![在这里插入图片描述](https://img-blog.csdn.net/20181005092003249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 活动图和状态图的区别、
- 状态图侧重从行为的结果来描述（状态）
- 活动图侧重从行为的动作来描述（活动）
### 小结
这个世界并不是掌握在那些嘲笑者的手中，而恰恰掌握在能够经受得住嘲笑与批评忍不断往前走的人手中。
