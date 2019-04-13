
# CSS在元素属性中引入 - 李昆鹏的博客 - CSDN博客


2018年05月11日 11:07:26[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：88标签：[CSS																](https://so.csdn.net/so/search/s.do?q=CSS&t=blog)个人分类：[CSS																](https://blog.csdn.net/weixin_41547486/article/category/7656511)



---------------------------CSS在元素属性中引入-------------------------
# CSS概述
CSS（Cascading Style Sheets）是层叠样式表用来定义网页的显示效果。可以解决html代码对样式定义的重复，提高了后期样式代码的可维护性，并增强了网页的显示效果功能。简单一句话：CSS将网页内容和显示样式进行分离，提高了显示功能。
单纯使用html的属性来控制页面样式不方便
|<p align="center"><font  color="\#00ff33">|[www.rlteach.com</font></p](/font%3E%3C/p)|>
|<p  align="center"><font color="\#00ff33">任亮</font></p>
|<p  align="center"><font color="\#00ff33">大家好，我们开始学习css</font></p>
|
----------------------------------------------------------------------------------------
**在HTML元素属性中引入**
在html标签中使用style属性：
属性值是键值对形式的集合，每一个键值对之间的用:连接，多个属性值之间用;隔开。
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|</|head|>
|<|body|>
|<!--
|通过|alt|+/|来提示|html|的属性和|css|样式属性和样式值
|text-align:|元素内的水平布局
|color:|内容的颜色
|-->
|<|p|style|="|text-align|:|center|;|color|:|red|;"|>|www.likunpeng.com|</|p|>
|<|p|style|="|text-align|:|center|;|color|:|red|;"|>|李昆鹏|</|P|>
|<|p|style|="|text-align|:|center|;|color|:|red|;"|>|大家好，我们开始学习|CSS|</|P|>
|</|body|>
|</|html|>
|


