# MVC框架中的模型-视图分离问题（三） —— 分离原动力 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年02月15日 09:03:00[慢游](https://me.csdn.net/eengel)阅读数：2130








采用模板引擎最主要的原因就是同时从意图和机制的角度将逻辑和数据计算从它们的显示中分离出来。在网站开发中，概略地说，这意味着同时满足**HTML中无代码**和**代码中无HTML**。下面列举了为什么程序员和美工需要这种分离的八大理由：



**1. 封装性（encapsulation）：**所有与站点的表现相关的都包含在模板中，所有业务逻辑则被放置在数据模型中。每种都是一个完整的实体。
**2. 明确性（clarity）：**一个模板不是一个产生HTML页面的程序，而是一个HTML文件，能够直接被美工或程序员阅览。
**3. 分工合作（division of labor）：**在程序员写代码的同时，美工可以构造站点的模板。这样可以从两方面降低程序员的负担：一方面可以让美工做模板（通常这样会更便宜）；另一方面可以减少沟通成本；美工可以不和程序员说就直接调整HTML。
**4. 组件重用（component reuse）：**就像程序员为了明确性和可重用性，会经常把大的方法分解成小一点的方法，美工也可以将模板分解成不同类型的子模板如边栏、导航栏、搜索框、数据列等等。混淆了代码的模板就很难进行分解，被其他数据源重用。
**5. 单点变换（single-point-of-change）：**只要能够分解模板，美工就可以将小元素如链接及用户档案视图这样的大元素都抽象出来。将来，如果想变换站点上的每个用户列表，美工也只要简单地更换一个模板文件就行了。这也避免了在多个地方修改一个行为时可能引入的错误。对于模型来说，只在一个地方修改一个行为也是非常重要的。
**6. 可维护性（maintenance）：**改变一个站点的外观只需要改变一个模板而不需要改变程序。改变程序总是比改变模板更危险一些。此外，修改模板不需要重启运行中的服务器程序，而修改代码一般都需要重新编译和重新启动。
**7. 可互换的视图（interchangeable views）：**如果数据模型和显示混淆在一起，美工很难像换“皮肤”一样不费力地为站点更换新的外观。在jGuru.com，每个站点外观是一组被称作“群组（group）”的模板集合。有些群组长得几乎完全不同（对比只改变点颜色和字体大小）。当显示页面的时候，只需要用一个简单的指示器告诉页面控制器使用哪个模板群组。
**8. 安全性（security）：**模板使得博客作者可以对页面进行个性化定制，但就像宏（macros）在Microsoft Word中一样，不受限制的模板会带来严重的安全风险。在激活了类加载的博客作者经受了大量攻击后，squarespace.com放弃了Velocity[1]而改用StringTemplate[2]。很容易就可以想象出一种采用死循环的简单而有效的攻击方式。就如《Enforcing Strict Model-View Separation in Template Engines》[3]所推崇的，严格分离模型和视图并禁止控制结构就等于安全。



    程序员经常把严格的分离当作费时又麻烦的事情。在一些小的地方确实如此，比如在视图中增加一条新数据。Parr[3]的经验是如果采用严格的分离，长期来看进度是会更快的，而且由于上面列举的理由，这样会得到更灵活、稳定的代码。
    有了以上对模型-视图分离问题的分析，模型-视图分离的理论基础是有了。关于模板及分离的理论定义，《Enforcing Strict Model-View Separation in Template Engines》[3]中有非常详细的叙述，此处略去二千字。。。下节的内容会直接切换到最关键的步骤：到底如何进行模板-视图的分离？



参考文献
[1] Velocity. [http://jakarta.apache.org/velocity/index.html](http://jakarta.apache.org/velocity/index.html)
[2] T. Parr. StringTemplate documentation. September 2003. [http://www.antlr.org/stringtemplate/index.tml](http://www.antlr.org/stringtemplate/index.tml)
[3] Parr, T.J., Enforcing Strict Model-view Separation in Template Engines. WWW ’04: Proceedings of the 13th international conference onWorldWideWeb, New York, NY, USA, ACM Press (2004) 224–233, [http://www.cs.usfca.edu/~parrt/papers/mvc.templates.pdf](http://www.cs.usfca.edu/~parrt/papers/mvc.templates.pdf)



