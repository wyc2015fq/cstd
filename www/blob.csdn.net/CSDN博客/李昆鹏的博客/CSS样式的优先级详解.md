
# CSS样式的优先级详解 - 李昆鹏的博客 - CSDN博客


2018年05月11日 11:15:59[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：49


------------------------------------CSS样式的优先级详解-------------------------------------
**样式优先级：**
**由上到下，由外到内。优先级由低到高。**
代码示例：
创建style.css文件
|@CHARSET|"UTF-8"|;
|p|{
|text-align|:|center|;
|color|:|red|;
|}
|
-----------------------------
创建style1.css文件
|@CHARSET|"UTF-8"|;
|p|{
|text-align|:|center|;
|color|:|blue|;
|}
|div|p|{
|text-align|:|center|;
|color|:|green|;
|}
|
---------------------------------------
创建HTML文件
|<!|DOCTYPE|html>
|<|html|>
|<|head|>
|<|meta|charset|=|"UTF-8"|>
|<|title|>|Insert title here|</|title|>
|<!--|项目中应用最多|-->
|<|link|href|=|"style.css"|type|=|"text/css"|rel|=|"stylesheet"|>
|<|link|href|=|"style1.css"|type|=|"text/css"|rel|=|"stylesheet"|>
|</|head|>
|<|body|>
|<|p|>|www.likunpeng.com|</|p|>
|<|p|>|李昆鹏|</|P|>
|<|p|>|大家好，我们开始学习|CSS|</|P|>
|<|div|><|p|>|优先级由外到内增高|</|p|></|div|>
|</|body|>
|</|html|>
|
--------------------------------------------------------------------------------------------------------------------
www.likunpeng.com
李昆鹏
大家好，我们开始学习CSS
优先级由外到内增高


