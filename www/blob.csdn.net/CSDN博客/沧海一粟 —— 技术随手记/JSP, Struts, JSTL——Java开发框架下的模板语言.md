# JSP, Struts, JSTL——Java开发框架下的模板语言 - 沧海一粟 —— 技术随手记 - CSDN博客





2012年02月02日 21:53:46[慢游](https://me.csdn.net/eengel)阅读数：2116








前文 [几句话理解Struts, JSP & Servlet](http://blog.csdn.net/eengel/article/details/7225904) 回顾了Java开发框架的几种基本技术，基本涵盖了前后端的实现。这儿就重点记录一下这几种技术都是怎么支持模板语言的。




之前提到了在Servlet中，为了产生动态Web页面，需要将HTML代码嵌入到Java代码中。这种方法随后就因效率太低，太难维护而被抛弃了。取而代之的就是JSP倡导的将Java代码嵌入到HTML代码中。而这种方法大致也分成了两类实现形式：

**1. 在HTML代码中直接嵌入Java代码;**

**2. 在HTML代码中嵌入规定的模板标记，代替裸Java代码实现页面的动态性。**

相比较而言，后者比前者提供了更好的可重用性,也更易被页面设计者使用，因此后来的Struts和JSTL (JSP Standard Tag Library)又出台了大量的模板标记规范，用以完善JSP的标记系统。


总而言之，JSP提供了基本指令和脚本元素，以及少量的标记；而Struts和JSTL提供的都是标记。

鉴于这几种技术都处于活跃状态，可能很多人都被搞糊涂了：出了这么多规范，到底具体写一个jsp页面的时候用什么指令、什么标记最好？说实话，本人到目前为止也没有完全弄清楚所有这些标记之间的关系，往往都是想在HTML页面中实现某个动态效果了，随手抓一个比较熟悉的标记填上去。赖以指导的原则也就是：

**1. 能用标记实现所需功能，就不在页面中写裸Java代码；**

**2. 能用一种规范实现大多数功能，就尽量不用另一种。**


如果要做到优化，最理想的当然是将JSP, Struts, JSTL提供的标记按照上述原则都研究一遍，并结合具体的功能需求，找出最适合的搭配。

另外一个选择就是采用开源的Java模板引擎，如Velocity，FreeMarker等，有望用一种标记规范就实现模板的所有动态功能。




总而言之，由于前端页面需求的多样性和变化性，导致应运而生的技术规范也让人眼花缭乱，很难用一句话涵盖所有的解决方案。但好在这些技术万变不离其宗，遵循的使用原则也和上面提到的大同小异。最好的办法还是实际用一下，找到最适合自己项目的实现方案。




作为参考，将本文提到的一些规范的链接列举如下：

1. JSP基本语法，包含基本指令、脚本元素，和少量以“jsp”开头的标记：[http://java.sun.com/products/jsp/syntax/2.0/syntaxref20.html](http://java.sun.com/products/jsp/syntax/2.0/syntaxref20.html)

2. Struts 标记：[http://struts.apache.org/1.x/struts-taglib/tagreference.html#struts-html.tld](http://struts.apache.org/1.x/struts-taglib/tagreference.html#struts-html.tld)

3. JSTL 标记：[http://docs.oracle.com/javaee/5/jstl/1.1/docs/tlddocs/](http://docs.oracle.com/javaee/5/jstl/1.1/docs/tlddocs/)

4. 开源Java模板引擎：[http://www.open-open.com/21.htm](http://www.open-open.com/21.htm)




