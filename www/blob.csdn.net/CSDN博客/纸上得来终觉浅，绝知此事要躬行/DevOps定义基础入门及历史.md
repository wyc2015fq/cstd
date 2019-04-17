# DevOps定义基础入门及历史 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月17日 13:28:03[boonya](https://me.csdn.net/boonya)阅读数：1093








原文地址：[https://theagileadmin.com/what-is-devops/](https://theagileadmin.com/what-is-devops/)本文采用谷歌翻译，相应的文章链接可以参考原文。

---------------------------DevOps是什么？-----------------------------



DevOps是一组概念的术语，虽然不是全新的概念，已经催化成一个运动，并在整个技术社区迅速蔓延。像任何新的和流行的术语一样，人们对于它是什么有点混淆，有时是矛盾的印象。这是我对DevOps如何有效定义的看法？我将此定义提出为一个标准框架，以便更清楚地讨论DevOps涵盖的各种问题。像“质量”或“敏捷”一样，DevOps是一个足够大的概念，需要一些细节才能完全理解。

*[**2016年12月更新：**我们非常高兴地宣布了敏捷管理员的新资源。**我们[为lynda.com制作了一个全面的DevOps基础课程](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://www.lynda.com/Operating-Systems-tutorials/DevOps-Fundamentals/508618-2.html&usg=ALkJrhiMvxryppUlIQ_ob8eQnt4uiuKmGg)。**该课程涵盖从DevOps历史到DevOps功能区域的所有内容。**我们希望你喜欢。]*

## DevOps的定义

DevOps是两个主要相关趋势相冲突的新词。第一个也被称为“敏捷系统管理”或“敏捷操作”;它采用了较新的敏捷和精益方法来运作。第二个是，在创建和运营服务的过程中，开发和运营人员在开发生命周期的各个阶段的协作价值得到了更广泛的了解，以及运营在日益增长的以服务为导向的世界中的重要性（参见“[操作：新秘密酱](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://radar.oreilly.com/2006/07/operations-the-new-secret-sauc.html&usg=ALkJrhh8HY9DCIZp_PUHsenT-jhjN_Onjg)）。

Jez Humble向我解释的一个定义是，DevOps是“致力于建设，发展和运行快速变化的弹性系统的跨学科实践社区”。

这很好，很好吃，但它对于互联网启动类型可能有点太深奥和具体。我相信你可以更精确地定义DevOps

> 
**DevOps是运营和开发工程师参与整个服务生命周期的实践，从设计到开发过程到生产支持。**


对此的一个主要推论是，以前的方法的实践中的重大变化的一部分是

> 
**DevOps的特点还包括操作人员使用许多与开发人员相同的技术来进行系统的工作。**


这些技术可以从使用源代码控制到测试到参与敏捷开发过程。

为此，“DevOps”不区分不同的系统管理员子学科 - “操作”是系统工程师，系统管理员，操作人员，发布工程师，数据库管理员，网络工程师，安全专业人士和其他各种学科的综合术语和职称。“开发”特别用作开发人员的缩写，但实际上它更广泛，是指所有参与开发产品的人，包括产品，质量保证和其他类型的学科。

DevOps与敏捷和精益方法有很强的联系。旧的观点认为“开发者”一方是“制造者”，“行动纲领”一方是“出生后处理创造的人” - 实现行业中所造成的伤害这两个被视为沉默的问题是DevOps背后的核心驱动力。通过这种方式，DevOps可以被解释为敏捷软件开发的长处，规定了客户，产品管理，开发人员和（有时）QA的密切协作，填补了差距，并迅速地向更好的产品迭代 - DevOps说：“是的，但服务提供以及应用程序和系统如何交互是客户价值主张的基本组成部分，因此产品团队需要将这些关注作为顶级项目。“从这个角度来看，DevOps只是扩展超越“代码”边界的敏捷原则，整个交付的服务。

## 深度定义

DevOps对于不同的人来说意味着很多不同的东西，因为它周围的讨论涵盖了很多的理由。人们谈论DevOps是“开发商和运营协作”，或者将“将代码视为基础设施”，或者是“使用自动化”或“使用看板”或“工具链方法”或“文化”或多样化看似松散相关的物品。深入定义它的最好方法是使用并行方法来定义类似复杂的术语敏捷开发。根据[维基百科](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://en.wikipedia.org/wiki/Agile_software_development&usg=ALkJrhhKdaIn8Qdj4a81SXbYqbEV6aY35A)和[敏捷宣言](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://agilemanifesto.org/&usg=ALkJrhghUoutwmSMDv9E_PgfQ7EPMz-A9g)，敏捷开发由四个不同的“层次”组成。我添加了五分之一的工具级别 - 谈论敏捷和devops可以让工作太痴迷，但假装不存在也是没有帮助的。
- **敏捷价值观**- 顶级哲学，通常同意在“[敏捷宣言”中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://agilemanifesto.org/&usg=ALkJrhghUoutwmSMDv9E_PgfQ7EPMz-A9g)体现。这些是敏捷信息的核心价值观。
- **敏捷原则**- 一般同意支持这些价值观的战略方法。敏捷宣言引用[了十几个更具体的原则](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://agilemanifesto.org/principles.html&usg=ALkJrhjqgYIp0YAZsBVRETsnU73VxFy7Dw)。你不必买入他们所有的敏捷，但如果你不认可他们中的许多，你可能在做别的事情。
- **敏捷方法**- 更具体的流程实现原理。XP，Scrum，您自己的自制程序 - 这是哲学让位于“我们打算如何在现实生活中”的操作剧本的地方。它们都不是强制性的，只是可能的实现。
- **敏捷实践**- 高度具体的战术技术，倾向于与敏捷实现结合使用。没有必要敏捷，但许多敏捷实现已经看到了采用它们的价值。Standups，计划扑克，积压，CI，开发人员用于执行工作的所有特定工件。
- **敏捷工具**- 团队使用的这些做法的具体技术实现，以便于根据这些方法进行工作。JIRA敏捷（又名Greenhopper），planningpoker.com等。

理想情况下，较高的层次可以告知较低的层级 - 人员或组织在不了解基本原理的情况下接受具体的工具和实践可能会或可能没有看到效益，但是这种“货物崇拜”方法通常被认为具有次优的结果。我相信人们谈论的DevOps的不同部分直接映射到同一个级别。
- **DevOps价值观**- 我相信DevOps的基本价值观被有效地捕捉在敏捷宣言中 - 或许有一点轻微的注意力集中在全面交付给客户的整体服务或软件上，而不是简单的“工作软件”。以前对DevOps的一些定义，如Alex Honor的“[人们超越工具的过程](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://dev2ops.org/2010/02/people-over-process-over-tools/&usg=ALkJrhiXaMqSCbRr0XIIfoYjKs0OYnfbVw)”，回应了敏捷宣言的基本声明，并敦促开关+操作系统的协作。
- **DevOps原则**- 没有一个同意的列表，但有几个广泛接受的尝试 -[这里是约翰·威利斯创造“CAMS”](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.getchef.com/blog/2010/07/16/what-devops-means-to-me/&usg=ALkJrhiPbd83TWqVaC0UzW3OY73nHg6QIA)，[这里的詹姆斯·特恩布尔](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.kartar.net/2010/02/what-devops-means-to-me/&usg=ALkJrhijpMut8RxYvEFA71DCeRhiMEQ-Gw)在这个层面[给出自己的定义](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.kartar.net/2010/02/what-devops-means-to-me/&usg=ALkJrhijpMut8RxYvEFA71DCeRhiMEQ-Gw)。“基础设施作为代码”是通常引用的DevOps原则。我已经[削减了“DevOps'ing”现有的敏捷宣言和原则](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://theagileadmin.com/2010/10/15/a-devops-manifesto/&usg=ALkJrhiavWpomxKVHmq04Ahmv0Ttf0DifQ)。我个人认为，DevOps在概念层面主要只是敏捷的原则的扩大，包括系统和操作，而不是停止对代码检查的关注。
- **DevOps方法**- 这里的一些方法是一样的;您可以使用Scrum与操作，看板与操作等（虽然通常更侧重于集成op与开发人员，质量检查和产品在产品团队）。有一些更明显的方法，例如[可见Ops类型](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://itpi.3dcartstores.com/BookseBooks_c_11.html&usg=ALkJrhiwuWltRTbvi1xZOnbCKBqfk4cVDA)更改控制，并[使用事件命令系统进行事件响应](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://velocityconf.com/velocity2008/public/schedule/detail/1525&usg=ALkJrhhYDb0RsCUc5ydSQCAm84bmoxWO8Q)。这些方法的集合正在增长;例如，更周到的监测方法是常见方法尚未明确定义的领域。
- **DevOps实践 -**作为实现上述概念和过程的一部分使用的特定技术。持续集成和持续部署，“使您的开发人员得到呼叫并呼叫”，使用配置管理，指标和监控方案，工具链工具方法...甚至使用虚拟化和云计算是一种常见的做法，用于加速现代基础设施世界。
- **DevOps工具**- 您将用于实施这些原则的工具。在DevOps世界中，发布（jenkins，travis，teamcity），配置管理（puppet，chef，ansible，cfengine），编排（zookeeper，noah，mesos），监控，虚拟化和容器化（AWS，OpenStack ，流浪汉，码头）等等。而与敏捷一样，说一个工具就是“DevOps工具”，这个意义上，它将会神奇地带给你DevOps，这是不正确的，当然，正在开发具体的工具，其目的在于促进上述原则，方法和实践，并且对DevOps的整体理解应该包含这一层。

最终，DevOps有点棘手的定义，就像它的哥哥敏捷一样。但这是值得的。当离开纯粹的哲学层面时，两者似乎都像空的妈妈和苹果派的陈述，受到批评：“你只是告诉我”更好地做我的工作，“但是，相反，只是没有更高层次的指导变成货物崇拜。“我做这个Scrum的书，所以我做敏捷”是错误的，因为我正在使用厨师，所以我是DevOps对吗？成为一个成功的敏捷或DevOps从业者是要了解所有的层它和给定的DevOps实现可能包含或不包含。最终，DevOps希望为敏捷带来什么是理解和实践，直到软件成功交付给用户并满足他们对可用性，性能和变化速度的期望为止。

## DevOps的历史

DevOps的起源源于技术工作在系统方面越来越需要创新。DevOps运动继承了敏捷系统管理运动和企业系统管理（ESM）运动。

ESM在2000年代中期出现，提供了“嘿，我们的运行系统的方法似乎仍然处于一个相当原始的状态，尽管多年的努力的原动力。让我们开始谈论做得更好。“来自Zenoss的Whellley和Mark Hinkle的John Willis参与了此工作，并[围绕这个概念](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://barcamp.org/BarCampESM&usg=ALkJrhi9IUhppr6BoiMHy65LmftkzulAcA)发起了一个[BarCamp](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://barcamp.org/BarCampESM&usg=ALkJrhi9IUhppr6BoiMHy65LmftkzulAcA)。我认为在这个阶段，[ITIL](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://en.wikipedia.org/wiki/ITIL&usg=ALkJrhg_s0XBpGydYzJ6yt-HDMdcx62l_Q)作为治理架构的初步魅力在很大程度上被推翻为“ITIL Lite”[可视化操作](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.itpi.org/the-visible-ops-handbook-review.html&usg=ALkJrhjnCKp3J3Y2RNcEpLxZ31aRLft8NQ)方法，以及从“大厂商”转向专注于过去的企业框架，如惠普， IBM和CA是端到端系统管理的唯一有意义的解决方案，但更多的开放源代码和更小的供应商的产品正在出现，包括Spiceworks，Hyperic，Zenoss等。

另外在2008年，第一届[Velocity会议](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://en.oreilly.com/velocity2010&usg=ALkJrhgbvmglbj7-1YTQSsv1xxQzlIYWQA)由O'Reilly举办，专注于网络性能和运营，为业务最佳实践提供了信息共享的场所。在2009年，有一些关于大型商店（最着名的是[Flickr](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.slideshare.net/jallspaw/10-deploys-per-day-dev-and-ops-cooperation-at-flickr&usg=ALkJrhjhDF0ZltBCMAybQCa5VDF2MlVGlw)）的开发商/运营协作的重要演讲，以及如何促进Web环境的安全，快速变化。诸如木偶和厨师的配置工具在那里有很好的表现。更多的人开始思考这些新概念，并想知道如何实现它们。

随着敏捷开发在发展空间的增长，其发展速度达到最高点，从利基向普通做法转变为思考“敏捷系统管理”，特别是在欧洲。英国的戈登旗（Gordon Banner）早在[这个演讲中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.agilesysadmin.org/agile%2520sysadmin.pdf&usg=ALkJrhhNJJjZcsl6U1cm2xD8VyJCds5VVQ)就提到过。这个运动的很多重点是进程和从看板和精益制造过程到IT系统管理的类比。然后在2009年，比利时的Patrick Debois和来自美国的Andrew“Clay”Shafer会面并[开始谈话（并创造了这个术语）DevOps](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.jedi.be/blog/2009/12/22/charting-out-devops-ideas/&usg=ALkJrhigUgRcZENdIE62uDg8AOGvmEZS0g)，然后Patrick在根特举办了第一次DevOpsDays活动，点燃了保险丝。这个概念，现在有一个名字，开始在其他地方（我在[奥斯卡奥斯汀](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://theagileadmin.com/2010/02/05/opscamp-debrief/&usg=ALkJrhgr6AQiItJGTDLJmfNrTVdR3o_T4w)发现了），包括Velocity和DevOpsDays在美国，并迅速传播更多的话。

[在帕特里克·德博伊斯（Patrick Debois）的观点中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.jedi.be/blog/2010/02/12/what-is-this-devops-thing-anyway/&usg=ALkJrhj1JoEVT6Y7V38Vn1n0NQyDDzDyqg)，DevOps是对现有做法产生的孤岛和不灵活性的反应，这听起来很熟悉。[这是](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://itrevolution.com/the-convergence-of-devops/&usg=ALkJrhgR26s6x4N1U_tckPuk6rUOqKqImQ)John Willis关于DevOps运动历史的[好作品](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://itrevolution.com/the-convergence-of-devops/&usg=ALkJrhgR26s6x4N1U_tckPuk6rUOqKqImQ)，它解构了组合起来创建它的线程。

DevOps从这些事情的“完美风暴”中脱颖而出。越来越多的自动化和工具链方法由更好的监控和配置工具提供，敏捷过程和开发/操作协作的需要以及ITSM / ITIL大/重实施的失败 - 它们相互碰撞并无意识地将所有三层的您需要敏捷运动（原则，过程和实践）并引发火灾。此后，它进一步发展，最显着的是许多思想领袖纳入了精益原则。

## 什么是DevOps？

### 这不是NoOps

不是“他们正在占据我们的工作”。有些人认为DevOps意味着开发人员正在接管运营并且自己做。其中的一部分是真实的，其中一部分不是。

DevOps是从房屋的开发方面来消除操作的一个误解，DevOps及其在敏捷操作中的前提是通常由运营团队发起的。这是因为操作人员（我也在这里说话）已经意识到，我们现有的原则，流程和实践没有跟上成功所需要的步伐。随着企业和开发团队在商业环境变得越来越快的时候需要更多的敏捷性，当我们尝试以更加僵硬的方式解决我们的问题时，我们往往越来越少，我们需要一个基本的重新定位，以便能够在一个有效的方式。

现在，由于我们意识到操作的一些部分需要自动化，这意味着我们操作人们做一些自动化开发，或者开发人员正在编写“操作”代码，或者两者兼而有之。这对一些可怕，但是是整体协作方法价值的一部分。我使用这种方法运行的所有成功团队都拥有深度开发技能*和*深度操作技能的人员共同创造出更好的整体产品。而且我还没有看到有人在高科技领域自动完成工作 - 由于较低级别的关注变得更加自动化，技术熟练的员工开始将高价值问题解决一个层次。

### 不是（只）工具

DevOps也不是简单地实现一套工具。我认为需要一个更普遍接受的DevOps定义的一个原因是，具有各种混乱和结构差的定义增加了人们通过“理论”的风险，并且在没有原则的情况下实现DevOps的过程或工具，这绝对是一个反模式。自动化只是权力的运行，智能自动化可以带来很大的损害，因为明智的自动化可以带来好处。

同样，敏捷从业者会告诉你，在开始有意义的协作之前，开始反复工作或采取其他具体做法，可能无法真正实现。我在公司工作的一些团队采用了敏捷的一些方法和/或工具，但不是其原则，结果不太理想。当然，一个工具可以在敏捷（或DevOps）中使用，但是如果你不知道如何使用它，那就像向未经培训的人员提供攻击武器。

但是最后，对“工具不应该被称为DevOps”的错位是错误的。在这样的意义上，扑克策划“敏捷”是否会让您敏捷地获得敏捷？不，但它是各种敏捷方法中常用的工具，所以称之为“敏捷工具”是适当的。同样，只是因为DevOps不仅仅是工具的总和并不意味着专门设计为按照DevOps思维运行系统的工具并不重要。（肯定有一堆工具，我已经使用，似乎专门设计来防止它！）

### 不是（只）文化

许多人坚持认为DevOps“只是文化”，你不能将这个词应用于给定的原则或实践，但我觉得这是夸张和不正确的。敏捷并没有帮助成千上万的开发商，因为它的工作停止在“文化”，劝告同事们，并且确定最佳做法的主要从业者只是宣布它是不言而喻的，拒绝任何更多的规定。（虽然有一些）。DevOps由上面列出的所有级别的项目组成，并且在没有实际出现的实体的情况下，很大程度上是无用的。您可以自己确定所有这些最佳做法，因为文化方向和大量时间进行实验，但分享信息是为什么我们有互联网（以及印刷机）。

### 不是（只）开发和操作

最后，这不排斥。有人抱怨“安全人怎么样！和网络管理员！为什么要离开我们！“关键是，创建产品或系统的所有参与者应该从一开始就合作 - 各种条纹的业务人员，各种条纹的开发人员和各种条纹的操作人员，所有这些都包括安全性，网络，无论谁。还有很多不同类型的业务和开发者利益相关者;只是因为每个人都没有得到特定的呼叫（“不要忘记图标设计师！”）并不意味着它们不包括在内。原来的敏捷开发人员大多在思考“biz + dev”协作，DevOps正在指出关于“dev + ops”协作的问题和解决方案，但所有这一切的成熟结果是“每个人都在合作”。在这个意义上，DevOps只是一个学科参与敏捷协作的整体文化的一个重要步骤，这应该涉及组织中的所有学科。所以谁参与交付软件或服务是DevOps的一部分。

### 不是（只）一个职位

简单地采取一个现有的运营团队，并称之为“DevOps团队”实际上并不帮助任何事情。如果您不采用上述的价值观和原则，这不仅仅是在一个整体系统层面上进行改变，而且不仅仅是在一个给定的团队中，所以不会获得所有的好处。

然而，我不在营地，你不能拥有DevOps的职位。“它通常用于一个职位，作为区分”新风格DevOps思维，自动化优先，开发合作伙伴，CI运行等等。系统管理员“来自”grouchy后院的人，他们积极地不在乎你的公司为什么生活。“有些人觉得有价值，其他人没有，没关系。作为招聘经理本人，当我将系统工程师的职位发布给我时，我看到申请人的适应性有明显的差异，这给我鼓励我继续这样做

### 这不是一切

有时候，DevOps的人们会被遗弃，并且做出宏大的声明，DevOps是关于“无处不在的一切！”由于DevOps插入了大量精益和敏捷思维的整体结构，并且在整个组织中也有机会进行这种协作很高兴看到所有的相似之处，但是去重新设计您的业务流程并不是DevOps本身。它是整体，希望合作和敏捷的企业文化的一部分，但DevOps具体涉及操作如何插入。有些人超越，最终将DevOps变成精益，敏捷的超级浇水版本，或者只是爱每个人。这在视觉层面上是伟大的，但是随着您逐渐进行粒度层次化，您最终将主要处理运营整合 - 其他工作正在担心其他部分（您当然也可以）。但是，在提供软件和维护服务方面仍然存在许多未解决的问题，并使其快速，可靠，安全等。- 如果有人想要使用他们从DevOps学到的东西去做一个更大范围的公司顾问，那么DevOps中的大多数人都是技术从业者，他们正在寻找更好的方式来完成*他们的*工作，而不是别人。在敏捷中有“敏捷软件开发”，那么敏捷组织的工作就越大。我认为DevOps最好被定义为“敏捷软件交付和运营”，它应该与其他从事较大组织举措的人员一起工作，但不会忽视该组织的主要价值主张。

## DevOps入门

DevOps没有一条路径 - 您的组织中只有一条路可行。非常成功的DevOps计划源自开发团队，从上到下，从上到下，从公司内部和顾问，广泛的教育和臭鼬飞行员。因此，很难给出一个通用的手册，以便如何实现它。我认为可以自由的说，它开始于您自己学习DevOps的价值观，原则，方法和实践，并尝试通过任何渠道最有效地传播 - 告诉其他技术人员，获得管理层，开始实施事情以自己的DevOps方式，让自己成功，人们会尝试告诉你事情如何在你的组织中成功，但这个建议通常比现实更多的政策和想法。观察组织中其他流行的东西如何产生并获得货币，并尝试这些相同的渠道。并继续学习。

### 课程 - DevOps基础

事实上，在最近的消息（2016年11月）中，詹姆斯和我已经和lynda.com录制了DevOps基础课程 - 这是一个三小时的视频，旨在将初学者定位到DevOps的整个范围。[在这里阅读更多](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://theagileadmin.com/2016/11/29/a-new-way-to-learn-devops/&usg=ALkJrhjmRn4oX4CZ-yjs7bUAAf7TYR3Y8A)

### DevOps阅读列表

DevOps仍然是新的，所以一个未定义的一批博客每月更改一次，随后Twitter上的人通常是最新信息的最佳来源。是的，这很讨厌。然而，有几本书和其他可靠的信息来源可以使用，然后与他人分享。
- 首选 - 由Gene Kim，Patrick Debois，John Willis，John Allspaw和Jez Humble开发[的DevOps手册](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://www.amazon.com/DevOps-Handbook-World-Class-Reliability-Organizations/dp/1942788002&usg=ALkJrhjn2WW6D31PKORabIS-L3ne_opYcA)刚刚于2016年底推出，终于成为DevOps的最终来源。如果你只是得到一本书，得到这本书。
- [凤凰城项目](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://itrevolution.com/books/phoenix-project-devops-book/&usg=ALkJrhg7FpCLI8KOCOHToSjB8tWx908LQg)，基金，乔治·斯帕福德，凯文·贝尔 - 灵感来自精神领域精益工作的小说“目标”，这是一个困扰软件公司DevOps实施的叙述。
- [Web操作](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://shop.oreilly.com/product/0636920000136.do&usg=ALkJrhgBk_PQpCjhr4Suiy7qpjmlUrzy-Q)，各种 - 一本O'Reilly的书，收集了一系列关于Web操作的散文，这些都是很多DevOps开创者的真正想法。
- [连续交付](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://continuousdelivery.com/&usg=ALkJrhj6ct_ccIfDqvK4XqevdEJ0cO3tuA)，Jez Humble和David Farley - 虽然CI / CD不是像有些人一样拥有的DevOps总和，但它肯定是一个主要的创新领域，这是最终的工作。
- [大规模敏捷开发的实际方法](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://itrevolution.com/the-amazing-devops-transformation-of-the-hp-laserjet-firmware-team-gary-gruver/&usg=ALkJrhhFdf4mMlETM4juqsFTYPszr8o2BA)，Gary Gruver - 对于那些认为DevOps只是为创业公司或仅用于Web软件的人而言，这是HP LaserJet固件部门如何转变为敏捷/ CI / DevOps结构的故事。
- [云系统管理实践](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://the-cloud-book.com/&usg=ALkJrhjMAo4RY7SeW8Noj8Cc-OL3SYqVQw)Tom Limoncelli，Strata Chalup，Christina Hogan - 运营方面的教科书风格指南，大量新型系统指导和大量显式DevOps内容。
- [释放它！](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://pragprog.com/book/mnee/release-it&usg=ALkJrhi12kv5wdlmogOTDMo3KdgpxoVfOw)Michael Nygard - 需要有更多这样的书籍，它解释了常见的系统故障模式和成功模式 - 我认为它是“四人帮派”系列书籍。
- [精益软件开发](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.amazon.com/Lean-Software-Development-Agile-Toolkit/dp/0321150783&usg=ALkJrhjNE0UiI0-lmpLFv9bEMu3FCnrO1A)，玛丽和汤姆Poppendieck - 精益在DevOps社区越来越多地采用，但从戴明和TPS开始有点恐慌。这本书是关于精益软件的开创性工作。
- 并与Gareth Rushgrove的[DevOps Weekly电子邮件通讯一起完成](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.devopsweekly.com/&usg=ALkJrhgPt3bi7SsUwA3w_cJPr8q95PUcfw)。

-Ernest Mueller，2010年8月2日 - 最后修订于2016年12月7日




