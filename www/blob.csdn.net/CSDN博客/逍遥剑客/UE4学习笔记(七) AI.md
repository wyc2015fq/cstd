# UE4学习笔记(七): AI - 逍遥剑客 - CSDN博客
2014年12月21日 16:49:01[xoyojank](https://me.csdn.net/xoyojank)阅读数：7991
NavMesh, 这个一个Demo看不出有什么优缺点, 编辑起来到是很方便
![](https://docs.unrealengine.com/latest/images/Engine/AI/BehaviorTrees/QuickStart/2/NMVis.jpg)
角色控制, 与UE3一样, 还是Character+Controller的方式, Character(Pawn)负责表现, Controller负责行为. 对于AI来说, 是派生AIController来实现自定义的AI行为
与UE3不同的地方, 一是Blueprint可以代替很多原来脚本所做的事情. 另外, UE3中更多的是依赖脚本中的内建状态机(state)进行AI行为控制, UE4则引入了BehaviourTree
![](https://docs.unrealengine.com/latest/images/Engine/AI/BehaviorTrees/QuickStart/12/fullTree.jpg)
基本的运作思路是: AIController里在Blackboard中记录一些数据状态什么的, 然后运行一个BehaviourTree, BT中的各个结点根据相关条件控制流程分支, 运行相应的Task
基中有几个概念:
- Blackboard: 保存了AIController实例及BT所需要的数据, 可以看做是一个在BP之间共享数据的地方, 可以通过Key-Value的方式进行查询/修改
- Selector: 按顺序运行每个子结点, 直到某个结果是success, 然后返回success. 如果全失败了, 则返回failure
- Sequencer: 按顺序运行每个子结点, 成功了则运行下一个. 子结点全部成功则返回success, 有一个失败就返回failure
- Simple Parallel: 可以在执行一个Task的过程中同时执行另一个分支, 可以在Task执行完后选择是取消另一个分支还是等待其执行完
- Task: 传统BT概念中的Leaf结点, 就是实际要做的事情
- Decorator: 可能叫"conditional"(条件)更好理解一些, 就是相当于if(xxx){}
- Service: 通常是跟Blackboard交互的, 运行到当前分支就会执行
总体用下就是可以在不写代码的基础上, 完整地实现一套AI. 由于BP的存在, 自定义的子结点也可以通过拖结点的方式实现, 降低了对程序员的依赖. Blackboard的应用是一个亮点, 数据交互和状态管理方便多了. 由于在BP中可以用事件的方式触发BT, 相对于传统的BT性能也会好一些. 最方便的就是调试了, 当前走到哪个分支都会在BT上显示出来:
![](https://img-blog.csdn.net/20141221164812453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
