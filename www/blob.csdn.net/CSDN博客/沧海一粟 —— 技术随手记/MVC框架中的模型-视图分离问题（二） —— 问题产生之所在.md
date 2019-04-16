# MVC框架中的模型-视图分离问题（二） —— 问题产生之所在 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年02月14日 11:28:00[慢游](https://me.csdn.net/eengel)阅读数：1302








    模板引擎[1]（template engine）是web模板系统[2]（web template system）的核心部件，主要功能是将web模板[3]（web templates）和内容信息整合到一起产生web文档。示意图如下：
![](http://hi.csdn.net/attachment/201102/14/0_1297653866p7PL.gif)
       图1: web模板系统示意[1]
    下面以基于Java的引擎为例说明模板引擎的历史演变。当然涉及的概念同样可以被应用到其他语言的引擎上，比如Perl, VisualBasic。主要内容翻译自《Enforcing Strict Model-View Separation in Template Engines[7]》。（见蓝色字体。以后若无另外说明，蓝色字体的内容均表示翻译的内容。）
**开端：Servlets[4]时代**    Java开始支持服务器端开发采用的是Servlets。它可以创建响应HTTP GET/POST命令的方法，在这些方法中可以通过print语句生成HTML。比如，下面是一个简单servlet的核心代码，它可以产生一个网页对某个URL参数传过来的name说“hello”。
out.println("<html>");
out.println("<body>");
out.println("<h1>Servlet test</h1>");
String name = request.getParameter("name");
out.println("Hello, "+name+".");
out.println("</body>");
out.println("</html>");
    问题是，用Java构建HTML不仅仅乏味而容易出错，而且没法让美工做这些事。一种改进的方法是，程序员可以将公共的HTML元素提取出来并包装成Java的表现对象（Java rendering object），例如Table和BulletList。不过这样还是不能避免servlet中包含HTML。
**巨大的进步：JSP[5]时代**    下一阶段的进化带来了Java Server Pages（JSP），乍看起来是一次巨大的进步。JSP文件本质上就是servlet，是嵌入了Java代码的HTML文件。JSP会被服务器自动转换为servlet。刚才的那个代码示例用JSP表示如下：
<html>
<body>
<h1>JSP test</h1>
Hello, <%=request.getParameter("name")%>.
</body>
</html>
    JSP开头也许只是包含了简单数据调用的HTML文件，就像上面的这个例子。但它们很快就退化到HTML和Java代码又纠缠在一起的情况了，就和当初的Servlets一样。实际上，美工还是无法修改JSP文件。更重要的是，JSP鼓励拙劣的面向对象设计。比如，一个include文件绝对不是类继承的一个好替代。JSP文件也不能创建子类，因此程序员就无法抽取出公共的代码和HTML。Hunter[6]还总结了一些JSP的其他问题，包括JSP针对列表显示的粗糙的循环机制。
**横向扩张：Template时代**    当JSP也不是问题的答案时，程序员们开始考虑模板（template）的主意。一个模板就是一个包含若干“洞（hole）”的HTML文档，这些洞里面可以放数据或简单操作的结果。不幸的是，几乎每个模板引擎都重复着JSP的错误；它们每个都提供了一种图灵完备的（Turing-complete）、针对特定工具的编程语言来嵌入到HTML中。就如JSP一样，设计师们被迫去想象程序产生的行为，而不是着眼在页面模板（page exemplar）上。
    有些模板引擎修复了JSP一些比较恶心的做法，但它们大多都没有去探寻过那个最初的原因：为什么JSP对于大型系统来说是一场灾难？一个模板应该只需要考虑如何呈现一个数据集的视图，而完全与底下要产生这些待显示的结果的数据计算分离。如果一种模板语言太强大，模板设计者就会承担混淆模板与业务逻辑的风险。

    从上面可以看到，“模型-视图分离”问题一直纠缠着模板引擎的实现。为什么这样的混淆应该被避免？敬请下回分解。



参考文献
[1] [http://en.wikipedia.org/wiki/Template_engine_(web](http://en.wikipedia.org/wiki/Template_engine_(web))
[2] [http://en.wikipedia.org/wiki/Web_template_system](http://en.wikipedia.org/wiki/Web_template_system)
[3] [http://en.wikipedia.org/wiki/Web_template](http://en.wikipedia.org/wiki/Web_template)
[4] Servlets. [http://java.sun.com/products/servlet/](http://java.sun.com/products/servlet/)
[5] JSP. [http://java.sun.com/products/jsp](http://java.sun.com/products/jsp)
[6] J. Hunter. The problems with JSP. [http://www.servlets.com/soapbox/problems-jsp.html](http://www.servlets.com/soapbox/problems-jsp.html)
[7] Parr, T.J., Enforcing Strict Model-view Separation in Template Engines. WWW ’04: Proceedings of the 13th international conference onWorldWideWeb, New York, NY, USA, ACM Press (2004) 224–233, [http://www.cs.usfca.edu/~parrt/papers/mvc.templates.pdf](http://www.cs.usfca.edu/~parrt/papers/mvc.templates.pdf)



