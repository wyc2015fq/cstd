# 契约式设计 Design by contract - weixin_33985507的博客 - CSDN博客
2011年09月01日 22:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
*在我职业之初，我想得最多的是：当调用一个方法时，传入的参数，我是进入该方法前检查呢？还是进入到该方法后检查？特别是在某个方法中，为了保证该方法能顺利执行，之前的条件检查、if…else…简直是噩梦。所以我会想：至于吗？有必要这样做吗？哇，这个问题让我纠结了好长时间。其实，问题本身并不难，只是需要抽象出一个理念，或是原则，然后一直这样做。后来，我为自己定个原则：在进入方法前，进行严格检查；而进入到方法后，只进行简单检查。也就是说，如果符合方法的需求，就执行，否则什么都不做，直接返回。之后，无意间看到“契约式设计”才意识到，我这样做原来有理论基础的，只是之前不知道而已。*
*因此，当我们不断前进时，的确需要时不时地看些理论，这样就会加深我们对问题的理解。*
### 本文内容
- 概述
- 历史
- 描述
- 与软件测试的关系
### 概述
契约式设计/契约式编码（Design by Contract(DbC)/Programming by Contract，以下简称 DbC ）是一种设计计算机软件的方法。这种方法描述了，软件设计者应该为软件组件定义正式的、准确的、可验证的接口规范，它扩展了抽象数据类型（abstract data type，ADT）对于先验条件（preconditions）、后验条件（postcondition）和不变性（invariants）的一般定义。这些规范称为“契约（contracts）”，它是一个比喻，类似于商业契约/合同的条件和职责。
（注：软件组件的概念比较广泛，不要想得过于狭隘）
因为，契约式设计（DbC）是美国的 Eiffel Software 的注册商标，所以，很多开发人员用 "Programming by Contract"，"Contract Programming" 或 "Contract-First development"来代替这个词。
### 历史
*The term was coined by Bertrand Meyer in connection with his design of the Eiffel programming language and first described in various articles starting in 1986 and the two successive editions (1988, 1997) of his book Object-Oriented Software Construction. Eiffel Software applied for trademark registration for Design by Contract in December 2003, and it was granted in December 2004. The current owner of this trademark is Eiffel Software.*
*Design by Contract has its roots in work on formal verification, formal specification and Hoare logic. The original contributions include:*
- *A clear metaphor to guide the design process*
- *The application to inheritance, in particular a formalism for redefinition and dynamic binding*
- *The application to exception handling*
- *The connection with automatic software documentation*
### 描述
DbC 的核心思想，用一个比喻就是：基于相互的“义务”与“权利”，一个软件系统的要素之间如何互相合作。这个比喻来自于商业活动，“客户”与“供应商”达成的“合约”而来。例如：
- 供应商必须提供某种产品（这是供应商的义务），并且有权期望客户付费（这是供应商的权利）。 
- 客户必须支付费用（这是客户的义务），并且有权得到产品（这是客户的权利）。 
- 双方必须满足应用于合约的某些义务，如法律和规定。 
同样地，如果在面向对象程序设计中的一个类的例程（方法）提供某些功能，那么，它要：
- 期望所有调用它的客户模块都保证某个进入的条件：这就是例程（方法）的先验条件—客户的义务和供应商的权利（方法本身），这样，它就不用去处理不满足先验条件的情况。 
- 保证退出时给出某个属性：这就是例程（方法）的后验条件——供应商的义务，也是客户的权利（调用方法的主要权利 
- 进入时假设，退出时保证，维护某个属性：类的不变性。 
契约（contract ）就是这种义务和权利的形式化。可以用“三个问题”来概括 DbC，这也是软件设计者必须反复问的：
- 期望什么？ 
- 保证什么？ 
- 维护什么？ 
很多语言都使用这样的断言。然而，DbC 认为契约对于软件的正确性至关重要，它们应当是设计过程的一部分。实际上，DbC  提倡首先写断言。
契约的概念可以扩展到方法/过程。每个方法的契约通常包含如下信息
- 可接受和不可接受的输入的值或类型，以及它们的含义 
- 返回的值或类型，以及它们的含义 
- 错误和异常的值或类型，以及它们的含义 
- 副作用 
- 先验条件 
- 后验条件 
- 不变性 
- （不太常见）性能保证，例如所需的时间和空间 
在继承结构中，允许派生类弱化先验条件（但不能加强），加强后验条件和不变性（但不能弱化）。这些规则很接进 Liskov 替换原则（behavioral subtyping 一种特殊形式）。
所有类的关系在“客户”类与“供应商”之间。在不违反“供应商”结果状态的情况下，一个“客户”类可以调用“供应商”的特性（功能）。接下来，“供应商”也有义务在不违反“客户”状态需求的情况下向“客户”提供一个返回状态和数据。例如，一个“供应商”的数据缓冲区要求，当调用删除功能时，缓冲区中要有数据；然后，“供应商”向“客户”保证，当调用删除功能，数据项必须从缓冲区中删除。其他的设计契约是“类不变性（Class Invariant）”。“不变性”保证（对本地类），在执行完每个功能后，类的状态被保持在一个规定范围内。
当使用契约时，“供应商”不用去验证是否满足契约条件。通常的看法是用安全网的契约校验，直接让代码“严重失败”。DbC 的 "严重失败" 的性质把调试的契约行为简化为清晰地规定每个过程的意图。这与防御性编程有明显的区别。在防御性编程中，当先验条件被破坏时，供应商去负责如何解决。更多的情况是，在 DbC 和防御性编程中，“供应商”抛出一个异常通知“客户”先验条件被破坏，客户必须解决如何响应这个信息。DbC 使得“供应商”的工作更简单了。
（注：比如，项目中，当我们认为某段代码可能会出错时，会加入 try{}catch{}finally{}，或是只要不符合先验条件，就让程序直接返回 null。）
DbC 也定义了一个软件模块的正确性标准：
- 当“客户”调用“供应商”之前，如果类不变性（class invariant ）和先验条件为真，那么服务完成后，类不变性和后验条件也为真。 
- 当“客户”调用“供应商”时，软件模块不应该违反“供应商”的先验条件。 
因为，在程序运行期间，不能违反契约的条件，所以，它们作为调试代码保留，或者出于性能原因，在产品版本中移除。
DbC 也可以用于代码重用，因为每段代码的契约都被完全地文档化。对于这个模块的行为，一个模块的契约可以被看作软件文档的一种形式。
### 与软件测试的关系
单元测试隔离地测试一个模块，来检查，如果它的子契约符合，那么它是否符合它的契约。集成测试检查各个模块是否可以很好的协同工作。
