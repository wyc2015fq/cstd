# IOT语义交互性之交叉 - 我相信...... - CSDN博客





2018年02月22日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1595
所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)










这个系列文章描述了一个单一的语义数据模型来支持物联网和建筑、企业和消费者的数据转换。 这种模型必须简单可扩展, 以便能够在各行业领域之间实现插件化和互操作性。 对于一个目前从事智能硬件的老码农，觉得这些文字具有积极的参考意义。本节讨论业务和设备本体论的交叉点, 以及两者的元素如何能够提高可伸缩性。
`“Good design is good business.” – Thomas Watson Jr.`
## 发展成为以IOT为中心的业务

对于用户来说, 物联网的真正价值并不在于远程控制智能设备, 而是这些设备之间能够相互良好的协作。 任何门的传感器都能够触发开关而不依赖哪家设备制造商， 而且任何 HVAC 单元都应该能够触发服务请求或触发更换空气过滤器的命令。

这就扩大了智能设备和商业系统之间对语义互操作性的需求。 它还需要一个新的架构, 可以支持业务流程, 设备处理, 消除围绕系统集成中间件构建的自定义系统的成本和复杂性。

这种新的架构必须使物联网的模型和概念与商业应用平台的模型和概念保持一致。 Gartner 预测, 到2020年, 大多数新的平台服务(PaaS)应用将以IOT为中心。 这打乱了传统的做法, 因为 PaaS 平台将用于实现围绕事件驱动的架构和物联网数据，而不是以传统主数据构建的业务应用程序。 这些以IOT为中心的业务应用程序反过来将转变应用程序设计实践, 将重点放在实时、丰富的上下文决策、事件分析、轻量级工作流程以及对网络规模数据的广泛访问。

与此同时,"物联网标准"和"业务标准"联盟需要适当地调整他们的数据模型、概念和术语。 这些资产必须集中在一个可伸缩的语义互操作性中心点上, 并在应用程序(信息)层中以分布式来数据管理。

## 共同业务本体的跨行业领域需求

语义互操作性依赖于一个指定的本体来解释交换数据的意义(上下文) , 并将其应用到一个有价值的目标。 这可以跨越多个系统、环境和工业。

针对特定行业的商业本体论(如金融业的 FIBO)和数据模型(如零售业的 ARTS ODM)有重叠的概念, 可以形成一个共同的商业本体论。 这个公共业务本体中的概念应该与一个上层本体(在第3部分 [IOT语义互操作性之本体论](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658972626&idx=1&sn=00e48cde6fa6fb500ad1d471128d1eec&chksm=80d32ef4b7a4a7e2d6404845fd4df0ee98254e0fe45fb66f6353bfce4119a7c45a1313a25e1b&scene=21#wechat_redirect))相一致, 就像根对象类, 这样所有知识领域的所有对象都具有内在的互操作性。 这些共同的商业本体论为促进跨行业语义互操作性提供了基础。

抽象(忽略不必要的细节的处理的一个广义模型)和面向对象的分解(将一个大的系统分解成逐渐较小的类或对象)已经一次又一次地证明了它们在解决分布式系统不一致性方面的价值。 为了使一个公共业务本体与上层本体的类别适当地对齐, 通用业务概念需要通过追踪与业务相关的数据到其原始对象类别来进行抽象和分解, 这可能是特定于行业的本体论或其他知识领域的一部分。 例如, 为了与上层本体对齐, 业务上显示的数据元素不会被视为独立"联系人"类的属性。 相反, 它们可以被分解成相关的顶级类的属性(如系统、位置或聚会)。

如图25所示, 一个电话号码、街道号、互联网域和电子邮件地址都是作为一种路由系统中端点的唯一"地址"属性, 并由系统操作员(组织)管理。 电话号码不是为"Contact"类定义一个"电话号码"属性, 而是可以定义为一种系统的属性, 该属性可以分配给一个设备。 这个设备可以被分配给一个派对（party）, 这个派对可以与另一个派对建立关系。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP9k9p88OEwKID5Egic4C3eeAqCx6Ok4mjC31blKyKvHMcdqS8pz62PVw/640?wx_fmt=jpeg)

图25 将名片分解为与上层本体对齐

在这个例子中,"Bob Smith"表示一个 person 的实例(对象) , 这是一种类型的派对（party）。 "Design by Design"表示一个组织对象, 它也是一种类型的Party。 鲍勃被授予"系统工程师"的角色, 他被设计得很聪明，他还被指定为一个移动设备, 由一个组织指定一个系统地址, 该组织本身被指定为"电话系统操作员"。

虽然这种层次的抽象和分解可能看起来很复杂, 但它实际上简化了语义互操作性, 防止了碎片化。 反过来, 它消除了复杂的数据映射在独立构思的对象类之间的需求。

## 位置 位置 位置

一个顶级的"位置"类和层次结构(图26)可以支持地理位置的封闭领域(例如, Haystack 的地理位置) , 以及住宅或建筑工地内的空间细分(例如, 区域、楼层、套房或房间)。 一个位置可以分配给一个资产。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPtRQiaz0CAIeqibEJYichnASUqiaxHPZShicnuW3Vc76D6dYX0DI4icfYEhWA/640?wx_fmt=jpeg)

"Site"子类(类似于 Haystack 的位置实体)可以包含支持邮政地址的唯一属性(可以出现在商业名片上)。 Site可以被分配到一个当事人和一个交易(订单, 装运)。

图27所示位置的例子显示,"会议室"包含在一个城市内的一个场地内的一个"第四"楼层内的"会议室", 等等。位置类的"气温"属性可以从分配给位置实例(四楼)的温度传感器中调节值(77.4 华氏度)。

邮政地址的定位实例可以反映邮政系统运营商组织(USPS)的所有权(所有者派对)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPUZASoVgoicvToiaKQrKrjObmcDQmfRbWSYMopLSvSW9NsheibWcf5SDyw/640?wx_fmt=jpeg)

图27 位置显示控制和所有权的示例实例

## 对象所有权的一方

一个顶级的派对类和层次结构(图28)可以包含 Person 和 Organization 子类。 一个组织可以包括业务单元和团队子类。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPe1B921ickJbicxF3Fhj1ArPLd7VzrkhHsbzMc7BCKk7q3FaBWJwG0UNA/640?wx_fmt=jpeg)

图28 派对类的等级和关系

一方当事人能够拥有法律所有权, 可以被分配到一个事务, 并指定一个对象的所有者(对象类的"所有方"属性)。 通过这种方法, 每个对象都属于一个缔约方, 并且在创建时被分配给一个所有者派对。 所有者一方可以是创建对象的人, 也可以是与人或设备相关联的组织。 所有者一方被赋予了所有当局对该物体的所有权力。

当事人也可以承担一个或多个与一个或多个过程相关的角色。 "派对的角色"类(类似于 ARTS ODM)可以作为顶级关系类的子类来建模。

如图29所示, 一个缔约方角色的实例可以为另一方(所有者派对)指定一方的角色。 例如, 鲍勃 · 史密斯(一个人的例子, 是一种类型的聚会)可以被分配一个"系统工程师"的角色(出现在他的名片上)和一个"雇员"角色。 与鲍勃•史密斯(Bob Smith)(这种关系的所有者, 在他的简历上显示)相比,"雇主"的角色可以转换。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPjQq0AWvSvHv2C3rFr1ByJhydlADVtqp2wicjaLUkjia2PJeSS8CHQxPQ/640?wx_fmt=jpeg)

图29 派对角色反向关系的实例

当一个"供应商"角色被分配给一个当事方(Arrow Electronics)时, 就可以建立贸易关系, 而反向的"客户"角色则分配给另一方(通过设计来智能化)。

## 商业交易

一个顶级的事务类和层次结构(图30)可以支持 B2B 事务和业务对消费者的交易, 包括订单、装运和付款对象。 作为一种替代的电子数据交换(EDI)方法, 这种面向对象的层次结构可以利用所有对象类共有的可互操作数据交换机制。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPYqbI5ypd97CogBKJq7sIPQb4DQpHBFcjdbLZJl2ZoG4XNX9PZXkXng/640?wx_fmt=jpeg)

图30 事务类层次结构和关系

交易实例可以定义一方之间与另一方(所有方)之间的商务交互(EDI 文档)定义为与其有贸易关系的另一方(所有方)。

事务项类可以作为顶级关系类的子类建模。 事务项的多个实例可以包含在事务实例中, 以反映所交易的产品(货物和服务)。

## 系统的系统

各种规模的企业都实施信息和自动化系统, 以有效和准确地控制其业务。 每个系统都包含一个本体或数据模型来管理、处理和存储信息对象。

会计信息系统(财务 / 订单管理系统)是一个核心的跨行业业务系统。 通过利用一个公共业务本体, 这个系统可以固有地与其他内部和外部系统相互操作, 这些系统也包含这个本体。

这种方法可以使企业有效地向多方数据共享过渡, 以满足不断变化的客户需求和行业遵守情况(例如供应链可追溯性)。

一个顶级系统类和层次结构(图31)可以支持系统的系统管理。 每个"组成"系统可以定义为"独立可操作性", 但可连接一段时间, 以实现某一较高的目标[11]。 例如, 将仓库管理系统与订单管理系统连接起来, 可以消除重复的数据输入, 并简化订单实现操作。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPM95L6SO7WtB7pMBW95xNwH2UkuuDyJb8FqO25iba4Sk6XCibFcJROFyA/640?wx_fmt=jpeg)

图31 系统类和关系

### 自动化系统的事件处理

每个系统实例都可以包含 Process 实例集合, 其中可以包含规则实例集合。 进程实例可以根据其规则使用事件实例, 并从其操作中生成事件实例。 例如, 更改订单的状态(事件实例)可以触发一个自动化过程, 生成一批货物或付款。

### 系统根系的互联网域

系统的一个互联网域子类(图32)可以将一个当事人(注册人)分配给一个互联网域(类似 IETF 的 EPP), 这可以为该方系统的系统提供一个根系。 例如, 互联网域名"Smart-by-Design.com"，"com"可以被分配到"设计智能"组织。 智能设计的所有子系统都可以直接或间接地连接到"Smart-by-Design.com"。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPbFPcXv4K8TT7Pu0IrWuZYVyZrAibtGZjj3BlcL08ppLRTomD6PicfjUw/640?wx_fmt=jpeg)

图32 域名系统为系统的根系指定一个当事人

系统属性类和系统连接类可以模拟为顶级关系类的子类。 两个类的多个实例都可以包含在系统实例中。

如图33所示, 系统属性实例可以在一个或多个代表系统进程的内部输入 / 输出，或与其他系统共享数据的一个或多个本体中的类属性。 例如, 一个订单管理系统可以引用事务类的"状态"属性和事务项类的"产品"属性, 这两个属性都包含在公共业务本体中。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPuX3WibqdX4MpkIJuoKjlib5E8Dy5jeZGq65KKrrMtKpu12l6XKOLUuBw/640?wx_fmt=jpeg)

图33 业务系统的系统属性示例实例

如图34所示, 系统连接实例可以反映基础系统(通过设计智能)之间的连接和一个子系统(订单管理)。 这个子系统反过来可以成为另一个系统连接实例中的基础系统。 镜像系统连接实例可以反映交叉对象系统之间的连接。 例如, 通过设计实现的智能订单管理系统可以连接到 “Arrow 电子”的订单管理系统, 以便能够共享其系统属性实例(如事务状态)中定义的共同属性交叉数据共享。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP9hvo0Zr48a8EwSHO440j9VFic5ViafFppy824SbtZT2eh2szEialKLOuA/640?wx_fmt=jpeg)

图34 业务系统的系统连接示例实例

### 设备的产品和资产类别

顶级产品、资产类别和层次结构(图35)可以支持生产单位(实例) , 这些生产单位是生产过程的所用材料, 并成为交易、转化、使用或消费的资产。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPpTXEjdicVK5dULj6N1Ck0KeCEy1WQchvopmK8tha5TJ4vAX8qBbNdAQ/640?wx_fmt=jpeg)

图35 产品及资产类别等级及多重继承

大多数本体模型(例如 OMG 的 UML, Schema.org)允许类层次结构中的多重继承: 类可以是多个类的子类。 这使得一个生产单位, 如一个设备, 既是一种产品, 也是一种资产。 它可以从"产品"类继承"模型"属性, 并从资产类中继承"位置"属性。 生产单位通常从制造商的库存资产开始, 转向为客户的设备资产。

### 电子商务的共同产品本体

一个共同的产品本体(类似于 GS1的全球产品分类)可以根据产品的基本属性及其与其他产品的关系对产品进行分类。 这个本体可以使交易伙伴以相同的方式将产品分组, 以便在多个电子商务网站上进行比较。

产品本体可以包括一个设备子类和层次结构(图36) , 它定义了特定类别设备的属性。 例如,"Pin Count"属性可以包含在设备类中(适用于所有设备)和"最大输出电压"属性, 可以属于设备的传感器子类。 一个电子商务网站(Arrow.com)可以显示传感器实例的继承属性, 作为传感器模型之间的特征比较。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPNnmsicGHE44vpjz88RnLUWia2qPxCiayWicgx9p9jDEbpkkagdyu6qO8lw/640?wx_fmt=jpeg)

图36 电子商务的设备类属性

一个产品组件类(图37)可以作为顶级关系类的子类建模。 产品组件的实例可以定义一个形成"产品的产品"的材料，或者组件的形式。 例如, 实例可以定义恒温器(传感器和执行器)的组件, 它本身被定义为冰箱的一个组成部分。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPcV9JN5yHeWQsrDEXnvbpHzFllzHfxwVKSicxkXx10tOycHmlYRfELmA/640?wx_fmt=jpeg)

图37 将冰箱定义为"产品的产品"的产品组件实例

联合体概念可以与这个公共产品本体(图38)对齐, 它包含一个带有 Identifier、类和 Owner Party 属性的根对象类。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPABy0u4lHjjrLvF6icMoF0Gln4kP562xMwNEpSgQbqfgJgc7XybUmgRg/640?wx_fmt=jpeg)

图38 

### 替换标识符的属性

任何一方可以根据标准方法(ISO, IETF)生成的通用(全局)唯一标识符(UUID)可以识别一个对象, 而无需依赖于中央注册机构。 虽然 UUID 被复制的概率不是零, 它是如此接近零, 以至于可以忽略不计。 UUID 和 GUID 的应用非常普遍, 许多计算平台为生成它们提供了支持。

然而, 使用由注册机构(组织)控制的编号方案生成的标识符, 也可以唯一地识别相同的对象。 例如, GS1维护的"ID 密钥"编号，这些密钥能够独特地识别支持全球供应链分布式系统的产品、资产、位置和事务类的实例。 此外, 任何组织(例如产品制造商、零售商或业务单位)都可以维护自己的编号模式(例如产品模型) , 该模式在其内部系统中唯一地标识对象。

为了有效地管理这些"备用标识符", 属性实例可以定义一个唯一的类属性, 以支持由一方(所有方)控制的特定编号模式。 在图39中, 一个包含在产品类中并由 GS1拥有的属性实例, 用于标识产品实例的全局交易识别号(GTIN)的每个长度。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPAyMhsgIgDR8kbqno95hkOvAMHqQhPEZdrPQCiccY3SOcLXnrZNA4CCg/640?wx_fmt=jpeg)

图39 支持备用标识符的示例属性实例

如图40所示, 一个产品实例可以通过其继承的对象类中 Identifier 属性以及在产品类(Model, GTIN-8, GTIN-12, GTIN-13等)中的其他标识符属性。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPjxrlkiby5hziaKaNflDuuPI39ctwgVlj7OibHwr67KRNbCKKibTCdgte0w/640?wx_fmt=jpeg)

图40 具有代表备用标识符属性的产品实例

### 向物联网扩展一个公共业务本体

产品向智能连接设备的发展——越来越多地嵌入到更广泛的系统中——正在从根本上塑造公司和竞争[12]。

智能连接产品需要重新思考设计。 在最基本的层面上, 产品开发正在从主要的机械工程发展到真正的跨学科系统工程, 可以支持一个系统的系统, 如图41[13]所示。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPpCbavia2bJRPiaicLWXZYfnovHw6KrjUPS0cz9rOWMbMKsQk3DbhnsFBQ/640?wx_fmt=jpeg)

图41 将物联网转移到系统系统中

与系统的系统一样, 物联网组件是异构的, 自主的, 能够通信, 分布式, 操作和管理上独立。 这两个领域都在动态的情况下进化和运行, 从而导致新的行为。 在这种情况下, 物联网可以被看作是一个设备系统[14]。

随着业务系统和平台被重新设计成以IOT为中心, 智能设备可以被设计成以系统为中心, 并利用一个共同的业务本体提供业务和设备系统之间的内在互操作性。

控制器是一种设备(典型的微处理器或计算机) , 它监视和改变连接其控制系统的元器件(传感器、执行器)的运行状态(例如温度或速度属性)。

这些设备的控制属性可以在共同设备本体的类中定义(图42)。 在设备控制器子类中的"系统"属性可以使系统分配给 Controller 实例。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPyClcH1PG6mCicosHA5N0vtL7CW7ydlhic90UntJzNdL3XiaPibzK7wwsIg/640?wx_fmt=jpeg)

图42 设备本体中包含设备系统属性

系统属性类可以包括支持控制器设备系统的实例(图43)。 这些实例可以引用共同设备本体中的类属性, 该属性表示系统进程的内部输入 / 输出，或与其他系统的共享数据。 例如, 一个气流控制系统可以引用传感器子类的"温度"属性、控制器类的"时钟"属性以及执行器子类的"速度"属性。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPLZKLarrNy5y2pGxNwHkM3kM0WHRMq8FE1XL3ia6PvDH8Y4oXqwFUuQw/640?wx_fmt=jpeg)

图43 设备系统的系统属性实例

系统连接类可包括与业务系统相连的支持设备系统的实例(图44)。 例如, 实例可以反映空气流量控制系统和 HVAC 系统之间的连接, 这两个系统连接到智能建筑系统，该系统连接到由智能设计实现的设施管理系统。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP4YcnaIWkm4rVfpfgLebfdeSldARGAn32pO5VRCzRb4S75DHwFChEjw/640?wx_fmt=jpeg)

图44 设备和业务系统的系统连接实例实例

智能建筑系统可以包含一个进程, 该过程可以改变由分配给控制装置，连接空气流量是控制系统所控制的风扇电机(一个组件设备)的"速度"属性。 这个过程可以产生一个事件实例, 当触发事件实例发生时(如气温变化)时, 该事件实例可以改变风扇速度, 这种情况可以通过流程规则进行监控。 举例来说, 当智能建筑管理「第四」楼层位置的「气温」属性超过74华氏度的设定值, 时间为上午7时30分至下午6时30分(过程规则)时 , 把风扇转速改为30每分钟转速。

## 将组织数据模型与常见的本体结合起来

组织联盟可以通过抽象和分解不同的数据模型来加速采用共同的本体论, 使其以系统为中心, 并调整其概念和术语。 例如, Zigbee集群，开放连接基金会(OCF)的"资源"和蓝牙的"配置文件"可以转向一个共同的"系统"概念 / 术语, 包括"属性"和"过程"。
- 
可以对联合体概念进行对齐, 形成一个共同系统本体(图45)


![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPXjibBe9KAgtMo7Cv9cS0VlPMJdU6gfePsrzjAn3fYyObZltqjIQXEnQ/640?wx_fmt=jpeg)

图45 共同系统本体的类对齐
- 
可以对联合体对象进行对齐, 以形成共同系统本体的实例(图46)


![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPCAE9r4icOibOtm6E0rxQvn4h5xq53aneFpAq8AUic5lYiaNzxzgKDePkvA/640?wx_fmt=jpeg)

图46 一个共同系统本体的对象分解和对齐
- 
可以对联合体概念进行对齐, 形成一个通用设备本体(图47)




- 
![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPydt8ibb0QRzMR1jDB6AIobl6uMGza0e15VuLOrfOn7InsUAMEUpk5kg/640?wx_fmt=jpeg)


图47 共用设备本体的类和属性对齐

## 一张物联网数据语义注释的地图

数据映射可以在两个不同的数据模型或本体之间联系数据元素。 例如, 需要与贸易伙伴发送和接收业务交易的企业可以从其业务系统的数据模型创建数据映射到 EDI 标准的数据模型。

数据地图也可以被一个控制器设备使用, 它接收来自一个资源稀缺的传感器的数据, 该传感器需要"语义注释", 以便在标准时间序列内提供完整的上下文。

数据映射类(图48)可以作为顶级关系类的子类建模。 数据地图实例可以将传感器对象(zn3-wwfl4)和属性(温度)联系到位置对象(四楼)和属性(气温)。 通过引用这个数据映射实例, 控制器可以用包含在数据映射实例中的语义来注释从传感器收到的语义, 以填充时间序列的元数据。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPa0HeDOBmdSWiavtaX8PV5Kic9bu7altW6RxyHUzEsKbfNZC6PXmSSNog/640?wx_fmt=jpeg)

图48 支持语义注释的对象属性数据映射

## 设备和业务事件的通用时间序列

为了创造价值, 物联网设备生成的数据越来越规范化为时间序列数据(标有时间戳的数据) , 并以定期间隔(基于间隔)或者基于状态(或值)的更改(基于事件)。

时间序列类(图49)可以作为顶级事件类的子类建模。 时间序列实例可以被有效地索引、共享、存储、查询和分析。 随着业务应用程序(系统)变得越来越以 IOT为中心, 并围绕事件驱动的架构构建, 来自业务事件的数据也可以归一化为时间序列数据。 一个通用时间序列类可以支持反映对象状态(属性值)变化的设备和业务事件。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPA1IibCJzssFqnlYic6S3ztias8JchibtZRNt46O2zV5uPLjqSn3rsEicGTQ/640?wx_fmt=png)

图49 设备和业务事件的公共时间序列示例

例如, 时间序列实例可以从传感器值中反映位置的气温变化, 从而产生反映冷却风扇速度变化的实例, 从而产生一个反映风机发动机故障的实例, 从而产生反映制造商更换零件订单和服务工作顺序的实例。 时间序列实例还可以反映与货币单位有关的货币单位的换算系数(货币兑换率)的价值变化, 这种变化会影响到替换部分的价格。

总的来说, 本文中讨论的概念(类)可以形成共同的跨行业本体论, 支持业务和设备的语义和过程。 这可以消除自定义系统集成的成本和复杂性。

## References

10 Lheureux, Benoit, Gartner Research, March 2016

11 Jamshidi, M., Systems of Systems Engineering: principle and applications, CRC Press, 2009.

12 Porter, Michael E. and Heppelmann, James E., "How Smart, Connected Products are Transforming Companies", Harvard Business Review, October 2015

13 Bleakley, Graham and Hause, Matthew, "Systems of Systems Engineering and the Internet of Things", OMG webinar Nov, 8 2016

14 Alkhabbas, F., Spalazzese, R., and Davidsson, P., IoT-based Systems of Systems, Malmo University, Sweden







