
# CSS样式在HTML中引入 - 李昆鹏的博客 - CSDN博客


2018年05月11日 11:11:52[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：72标签：[CSS																](https://so.csdn.net/so/search/s.do?q=CSS&t=blog)个人分类：[CSS																](https://blog.csdn.net/weixin_41547486/article/category/7656511)


-------------------------------------CSS样式在HTML中引入---------------------------------------------

**第一种方式在HTML的头信息中定义CSS**
|<style  type="text/css">
|p  {
|color:\#0000ff;
|text-align:center;
|}
|</style>
|
代码示例：
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<!--
|通过|alt|+/|来提示|html|的属性和|css|样式属性和样式值
|text-align:|元素内的水平布局
|color:|内容的颜色
|-->
|<|style|type|=|"text/css"|>
|/*|我是|CSS|注释|*/
|p|{
|text-align|:|center|;
|color|:|red|;
|}
|</|style|>
|</|head|>
|<|body|>
|<|p|>|www.likunpeng.com|</|p|>
|<|p|>|李昆鹏|</|P|>
|<|p|>|大家好，我们开始学习|CSS|</|P|>
|</|body|>
|</|html|>

|
**第二种引入方式**
通过head标签中link标签来实现，前提也是先要有一个已定好的CSS文件。
**注：可以通过多个link****标签链接进来多个CSS文件。重复样式以最后链接进来的CSS样式为准。**
---------------------------------------
代码示例：
首先创建CSS文件
|@CHARSET|"UTF-8"|;
|p|{
|text-align|:|center|;
|color|:|red|;
|}
|
--------------------------------------
再将CSS文件引入到HTML头中
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<!--|项目中应用最多|-->
|<|link|href|=|"style.css"|type|=|"text/css"|rel|=|"stylesheet"|>
|</|head|>
|<|body|>
|<|p|>|www.likunpeng.com|</|p|>
|<|p|>|李昆鹏|</|P|>
|<|p|>|大家好，我们开始学习|CSS|</|P|>
|</|body|>
|</|html|>
|
---------------------------------------------------
还有一种方式将CSS文件引入到HTML头中
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<|style|type|=|"text/css"|>
|@IMPORT|url("style.css")|;
|</|style|>
|</|head|>
|<|body|>
|<|p|>|www.likunpeng.com|</|p|>
|<|p|>|李昆鹏|</|P|>
|<|p|>|大家好，我们开始学习|CSS|</|P|>
|</|body|>
|</|html|>
|


