# URL重写与伪静态 - 左直拳的马桶_日用桶 - CSDN博客
2008年02月22日 19:37:00[左直拳](https://me.csdn.net/leftfist)阅读数：1438标签：[url																[iis																[正则表达式																[asp.net																[dll																[asp](https://so.csdn.net/so/search/s.do?q=asp&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=asp.net&t=blog)](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)](https://so.csdn.net/so/search/s.do?q=iis&t=blog)](https://so.csdn.net/so/search/s.do?q=url&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
URL重写与伪静态
左直拳
实现网页伪静态的方法有很多种罢？可是我都不会。
在百度、GOOGLE里找来找去，许久连个屁也没找到。
新年过后，春回大地，万象复苏，冻雨暴雪天气逐渐结束，听说湖南郴州城区也慢慢恢复了供电，而近日科索沃也悍然独立了，真是喜事连连，令人笑逐颜开。
在改革春风的吹拂下，我居然也得以利用URL重写实现了伪静态。
原理是在ASP。NET层面上将URL重写。用户访问的是类似
[http://www.fuck.com/1-345.aspx](http://www.fuck.com/1_345.aspx)这样的地址，实际上真实的页面是
[http://www.fuck.com/fuck.aspx?male=1&female=345](http://www.fuck.com/fuck.aspx?male=1&female=345)。
要实现这种效果，需要利用一个老外写的一个包(详细请见[http://msdn2.microsoft.com/zh-cn/library/ms972974.aspx](http://msdn2.microsoft.com/zh-cn/library/ms972974.aspx)
)。不过他提供了所有的源代码。与其他下载来历不明的DLL方法相比，这种方法更令我们放心。一切尽在掌握。
将这个包编译，然后引用产生的DLL。接着修改 Web.config。加入以下代码
<configSections>
<sectionname="RewriterConfig"type="URLRewriter.Config.RewriterConfigSerializerSectionHandler, URLRewriter"/>
</configSections>
……
<!-- url rewriter规则 开始-->
<RewriterConfig>
<Rules>
<RewriterRule>
<LookFor>~/(/d{1,})-(/d{1,})/.aspx</LookFor><!—正则表达式 -->
<SendTo>~/fuck.aspx?male=$1&female=$2</SendTo>
</RewriterRule>
<RewriterRule>
<LookFor>~/fuckyou.aspx</LookFor>
<SendTo>~/fuck.aspx?malename=me&femalename=you</SendTo>
</RewriterRule>
</RewriterConfig>
<!-- url rewriter规则 结束-->
……
<httpModules>
<addtype="URLRewriter.ModuleRewriter, URLRewriter"name="ModuleRewriter" />
</httpModules>
可以这还不是伪静态啊。我想达到的效果是输入
[http://www.fuck.com/1-345.html](http://www.fuck.com/1_345.aspx)，从而访问
[http://www.fuck.com/fuck.aspx?male=1&female=345](http://www.fuck.com/fuck.aspx?male=1&female=345)。
将MSDN这篇文章反反复复的看，都只能处理ASPX文件。怎么办？
主要是输入*.html，IIS会认为这是一个静态页面，根本就到不了ASP.NET这一层，那个引用的包也就无用武之地了。如果能让ASP。NET处理html类型的页面就好了。
结果配置一下IIS就搞定了。
在IIS里，点击指定网站的属性，主目录-配置-映射，添加，可执行文件选：
C:/WINDOWS/Microsoft.NET/Framework/v2.0.50727/aspnet_isapi.dll
扩展名：.html
勾上“全部动作”和“脚本引擎”
务必**取消“检查文件是否存在”**（不然的话IIS一看没这个1-345.html，立马返回一个页面不存在的错误）
确定即可。
相应的WEB.CONFIG里也要改改：
<RewriterRule>
<LookFor>~/(/d{1,})-(/d{1,})/.html</LookFor>
<SendTo>~/fuck.aspx?male=$1&female=$2</SendTo>
</RewriterRule>
啊！终于搞定了。我情不自禁地流出了激动的泪水。

