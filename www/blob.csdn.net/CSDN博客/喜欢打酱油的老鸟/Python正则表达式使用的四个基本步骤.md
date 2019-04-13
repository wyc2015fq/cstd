
# Python正则表达式使用的四个基本步骤 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月14日 15:29:40[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：35标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)[正则表达式																](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Python&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[http://blog.itpub.net/31403259/viewspace-2157701/](http://blog.itpub.net/31403259/viewspace-2157701/)
Python正则表达式使用的四个基本步骤
1、用import re导入正则表达式模块
2、用re.complile()函数创建一个Regex对象
3、箱Regex对象的search()方法传入想查找的字符串，它返回一个Match对象
4、调用Match对象的group()方法，返回实际匹配的文本字符串
以下测试使用Python正则表达式的四个基本步骤来匹配查找“IT168”，如下：
>>> import re
>>> testData = "ITPUB,中国专业的IT技术社区_每天百余篇IT技术资讯,5万活跃技术爱好者,13万热帖话题讨论,主要为高端IT专业技术人员提供媒体信息传播和服务平台-IT168旗下网站"
>>> testRe = re.compile(r'IT\d\d\d')
>>> mo = testRe.search(testData)
>>> print('The Result is: ' + mo.group())
The Result is: IT168
>>>

