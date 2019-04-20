# 丧心病狂的Github骚操作 - 纯洁的微笑 - CSDN博客
2019年03月15日 09:09:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：3301
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnp57jAteHWzU4AnFqtfMUYXeef41rLpOibGMKdZO9DcY2puoXZLOia5CsB6zicOl6icib0eHWDfwzlicPdA/640?wx_fmt=jpeg)
Github,一个被业内朋友成为「全球最大的同性交友社区」的平台。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfWQtKJdGIOuxSDqkK0KdvfXRDeNnyZfEMUpeLK4Xr0VprqAZSwsuD1A/640?wx_fmt=png)
小时候遇到不会的字可以查新华字典。后来写作文我们可以通过作文书、或者文摘去找合适的素材。同样，写代码可以去Github上找适合自己的代码片段，甚至开源框架。不再重复造轮子，带来的好处不言而喻。
Github在年初宣布个人私有仓库免费，小伙伴们拍手叫好。除了日常的托管代码功能以外。还可以在上面搭建博客，写技术教程或者书籍，发布一些日常的碎碎念，当然你也可以follow一些大佬，关注他们的日常动态。
今天主要分享一些检索上的技巧，能够帮你更精确的找到需要的项目代码。
开始之前有必要说一下几个常用词的含义，肯定有很多人不知道
- 
**watch**：会持续收到该项目的动态
- 
**fork**，复制某个项目到自己的Github仓库中
- 
**star**，可以理解为点赞
- 
**clone**，将项目下载至本地
- 
**follow**，关注你感兴趣的作者，会收到他们的动态
**通过in关键词限制搜索范围**
**xxx in:name**  项目名包含xxx的 
**xxx in:description** 项目描述包含xxx的 
**xxx in:readme** 项目的readme文件中包含xxx的
当然也可以通过**xxx in:name,desciption**来组合使用
如下，我需要搜索项目名或者描述中包含「Hello」的项目，
通过**Hello in:name,decription** 完成
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfiaKU0YAM34yic9vkgQKmaAw6p4hSH7vPhxqfvUaz2NkBqbeAqAZtS6Iw/640?wx_fmt=png)
**通过 Star 或者Fork数 去查找项目**
通过通配符 > < = 即可，区间范围内可通过 num1..num2
如，要查找stars数不小于666的springboot项目
springboot  stars:>=666
forks 大于等于500
springboot forks:>500
查找fork在100到200之间 且stars数在80到100之间的springboot项目
**springboot forks:100..200 stars:80..100**
**awesome + 关键字**
搜索和关键字匹配的优秀项目
awesome springboot 搜索优秀的springboot相关的项目，包括框架、教程等
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfurP1NCBRoRP1JShQDTjXg1Du0C5vmbHiab9iaGlC7ud8ibic7WZbia22vog/640?wx_fmt=png)
**分享项目中某一行的代码**
只需要在具体的网址后面拼接#Lxx(xx为行数)
如
我需要分享这个类中的@SprintBootApplication注解，值需要在后面拼接上#L6 即可
`https://github.com/lxy-go/SpringBoot/.../JpaApplication.java#L6`
浏览器访问 发现高亮显示了
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfpZmiamwr40XuU9HoFLpUlDSQkmHjGJd8LgBstj3uupFF0mr8WkTewGw/640?wx_fmt=png)
当然也可以段落进行高亮显示
`https://github.com/lxy-go/SpringBoot/blob/master/SpringBoot/DataJpaApplication.java#L6-L10`
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfGibaiboUzj9oA7OdsX6Mj7UMmicrHMZd4Z6R5pRxcQTfDY0SqHj1qic5pA/640?wx_fmt=png)
**项目内搜索**
打开你想要搜索的项目，然后按一下‘T’键。会跳转至一个新的网页，
如下图：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfTs3BI8F3qComfGmtYWFnOgsmUWDAbxvEFQLrtOUcib6eEgjiaClLvCCQ/640?wx_fmt=png)
例如我想要搜索该项目的配置文件 application.yml
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfCBqC1DyUH9a1HrxrGSiaM5DJau9sia30VhKVTlNE3zZ2Jwgiau30Ad3gw/640?wx_fmt=png)
类似的页面内快捷键还有很多，常用的还有’L‘, 在文件内跳转到多少行。
更多快捷键可以查看官方文档
https://help.github.com/en/articles/using-keyboard-shortcuts
**搜索某个地区内的大佬**
可以通过location:地区 进行检索，在具体可以通过language:语言  缩小检索范围
如搜索地区在北京的Java方向的用户
location:beijing language:java
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfOfKSYr0XU2p5ciayahQLB88OOriaZfZEpB31ncODwHBjvzbahmoGNZIA/640?wx_fmt=png)
**推荐一些Github插件**
***1、O*ctotree**
平时浏览github代码时，每个文件都需要点击才能查看，用了这个插件可以将项目的目录结构以树形结构显示，点击之后会自动跳转到相应的目录。墙裂推荐！
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfrFngSU0HmMqpFXCdUIrliaJicNTxM0Yg28gXbHE6uMBBDtic9cSaf2vag/640?wx_fmt=png)
## *2、O*ctoLinker
浏览代码如果可以像ide那样可以链接跳转岂不是美滋滋，这个插件就可以帮助我们完成这些。只需要ctrl+点击变量名即可。
**3、Sourcegraph**
一个可以在github上浏览和搜索代码的工具，安装好插件之后会出现一个view Repository的按钮
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1Cfx2uel9LNyf1An5clPeVFs65oTUY5cLO8Ey8OuJ1oRQGicic8KMicMcY5A/640?wx_fmt=png)
点击进去之后，简直不要太爽。可以随意在项目中搜索，可以查看变量和方法，以及进行跳转等等。多说无益，推荐大家安装插件自己感受一下
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/6D5fS3V8mLwZ5jSFUhPF2IULhCSZt1CfBiagPSS2cTmcBXSUQ1V0uvkh9Kuxf04aDM7OJTxeCGjae4BlSicibhYlA/640?wx_fmt=png)
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
