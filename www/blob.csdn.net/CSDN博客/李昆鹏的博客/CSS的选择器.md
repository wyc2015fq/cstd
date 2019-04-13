
# CSS的选择器 - 李昆鹏的博客 - CSDN博客


2018年05月11日 11:20:28[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：31


---------------------------------CSS的选择器------------------------------
## CSS的基本选择器：
Ø  html选择器（元素类型选择器），就是把html本身作为选择器。
Ø  class选择器，就是把html标签的class属性值作为选择器。为了和HTML进行区分，加.作为前缀。
Ø  id选择器，就是把html标签的id属性值作为选择器。为了和HTML进行区分，加\#作为前缀。
Ø**基本选择器的优先级：元素类型选择器<****类选择器< ID选择器**
代码示例：
|@CHARSET|"UTF-8"|;
|p|{
|text-align|:|center|;
|color|:|blue|;
|}
|.rlteach|{
|text-align|:|center|;
|color|:|gray
|}
|\#rteach|{
|text-align|:|center|;
|color|:|purple|;
|}
|
--------------------------------------
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<!--|项目中应用最多|-->
|<|link|href|=|"style2.css"|type|=|"text/css"|rel|=|"stylesheet"|>
|</|head|>
|<|body|>
|<|h1|>|选择器|</|h1|>
|<|hr|>
|<|p|>|www.likunpeng.com|</|p|>
|<|p|>|李昆鹏|</|P|>
|<|p|>|大家好，我们开始学习|CSS|</|P|>
|<|hr|>
|<|p|class|=|"rlteach"|>|www.likunpeng.com|</|p|>
|<|p|class|=|"rlteach"|>|李昆鹏|</|P|>
|<|p|class|=|"rlteach"|>|大家好，我们开始学习|CSS|</|P|>
|<|hr|>
|<|p|id|=|"rteach"|>|www.likunpeng.com|</|p|>
|<|p|id|=|"rteach"|>|李昆鹏|</|P|>
|<|p|id|=|"rteach"|>|大家好，我们开始学习|CSS|</|P|>
|</|body|>
|</|html|>
|
-------------------------------------------------------------
**关联选择器：**
**标签的嵌套使用。相同标签中某些内容有不同显示**
需求：写一个段话,我爱学习,把学习用粗体表示。
针对一段话中的粗体部分添加样式：红色,30px
pb：元素中间的空格代表p元素内部的b元素
|@CHARSET|"UTF-8"|;
|p|{
|text-align|:|center|;
|color|:|blue|;
|}
|p|b|{
|font-size|:|30px|;
|color|:|red|;
|}
|
--------------------------------------------------------
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<!--|项目中应用最多|-->
|<|link|href|=|"style3.css"|type|=|"text/css"|rel|=|"stylesheet"|>
|</|head|>
|<|body|>
|<|h1|>|选择器扩展|</|h1|>
|<|hr|>
|<|p|>|我爱|<|b|>|学习|</|b|></|p|>
|<|b|>|我不喜欢学习|</|b|>
|</|body|>
|</|html|>
|
-----------------------------------------------------------
**组合选择器：**
**不同的标签显示相同的样式。**
需求：我要把所有的p标签和b标签的内容都添加样式：蓝色,130px
b,p：逗号代表两侧的元素使用当前同一个样式
|@CHARSET|"UTF-8"|;
|p,b|{
|font-size|:|3em|;
|color|:|red|;
|}
|
------------------------------------
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<!--|项目中应用最多|-->
|<|link|href|=|"style4.css"|type|=|"text/css"|rel|=|"stylesheet"|>
|</|head|>
|<|body|>
|<|h1|>|选择器扩展|</|h1|>
|<|hr|>
|<|p|>|我爱学习|</|p|>
|<|b|>|我不喜欢学习|</|b|>
|</|body|>
|</|html|>
|
-------------------------------------------------------
**伪元素选择器：**
**其实就在html****中预先定义好的一些选择器。称为伪元素。是因为CSS的术语。**
格式：标签名：伪元素。类名。类名：伪元素。都可以。
a:link  超链接未点击状态。
a:visited被访问后的状态。
a:hover光标移到超链接上的状态（未点击）。
a:active点击超链接时的状态。
代码示例：
|@CHARSET|"UTF-8"|;
|a|:link|{
|color|:|green|;
|text-decoration|:|none|;
|}
|a|:visited|{
|color|:|red|;
|text-decoration|:|none|;
|}
|a|:hover|{
|color|:|gray|;
|text-decoration|:|underline|;
|}
|a|:active|{
|color|:|navy|;
|text-decoration|:|underline|;
|font-size|:|30px|;
|}
|
---------------------------------------
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<!--|项目中应用最多|-->
|<|link|href|=|"style5.css"|type|=|"text/css"|rel|=|"stylesheet"|>
|</|head|>
|<|body|>
|<|h1|>|伪元素选择器|</|h1|>
|<|hr|>
|<|a|href|=|"http://www.rlteach.com"|>|www.rlteach.com|</|a|><|br|/>
|<|a|href|=|"http://www.baidu.com"|>|百度|</|a|><|br|/>
|<|a|href|=|"http://www.sina.com"|>|新浪|</|a|><|br|/>
|</|body|>
|</|html|>
|


