# 030_AUTOSAR软件组件学习笔记 - 小灰笔记 - CSDN博客





2018年12月11日 22:49:15[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：132








**软件组件类型从描述看给我一种面向对象的设计的感觉。软件组件类型SwComponentTypes****中封装了应用软件的功能，并且提供了提供给外部的连接点PortPrototypes****。**

         PortPrototypes的特征在于PortInterfaces，PortInterfaces支持以下通信范例：
- 基于数据（发件者-接收者）
- 基于操作（客户端-服务器）
- 基于模式（在各层次创建与模式相关软件）
- 基于触发器（允许激活远程原件组件上的功能）

**以上内容简单摘录于书籍资料，看完后也有一点简单的小疑问：**
- **基于操作不会与基于数据有交叉关系？**

PortPrototypes进一步设计，PortPrototypes可以表示为：

         RPortPrototypes：用于请求服务或数据

         PPortPrototypes：提供服务或数据

PRPortPrototypes：结合两种功能，既提供又请求服务或数据

         SwComponentTypes的风格分类：AtomicSwComponentTypes、CompositionSwComponentTypes。前者是最小可能的粒度，而后者可以满足在概念层面创建任意复杂度的SwComponentTypes。

         汽车应用软件的实际实现是通过AtomicSwComponentTypes的定义实现的。

**以上内容为摘录。**

**疑问：ASW****使用AtomicSwComponentTypes****实现，不需要使用CompositionSwComponentTypes****吗？复杂度主要体现在什么地方，是策略的复杂度还是实现的复杂度呢？看上去似乎更像是后者。**

**关于软件的实现，还是有一点疑惑。说的还是偏抽象一点，如果纯C****的话该定义什么样的数据结构出来呢？**



