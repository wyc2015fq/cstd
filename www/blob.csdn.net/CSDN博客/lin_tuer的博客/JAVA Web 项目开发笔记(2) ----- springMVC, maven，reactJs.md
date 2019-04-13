
# JAVA Web 项目开发笔记(2) ----- springMVC, maven，reactJs - lin_tuer的博客 - CSDN博客


2017年07月14日 20:59:41[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1255


———接上篇————————–
在上一篇记录中，通过各种找资料，终于用maven把项目给搭建起来了，后端结构已经完整了，现在，我们来看一下，怎么把
前端配置为ReactJs框架开发（说起来都是泪，我在这里停留了有一个星期的样子，唉，到处找资料，就是对原理理解不够清楚，感谢大神们的博客指引，终于还是搭建起来啦）
# 1. webpack与react
webpack是个好东西，可以让我们更方便滴使用react， 看这里[webpack+react项目环境搭建](http://www.jianshu.com/p/05c1b9e3ef49)
按照例程走下去，先对它们都有个了解~
# 2. maven spring MVC项目，前端加入react
感谢[这篇文章](http://blog.csdn.net/linfujian1999/article/details/72639194)
我就不重复它的操作了，说一下，其间遇到的问题和我的解决方案
npm run-script watch 在项目下运行这个命令可以件事 app.js以及其他用到的模块文件的改变，自动重新生成 bundle.js，还是很方便的~
找不到js文件
这个问题，是由于springMVC配置引起的，在我们的WEB-INF->lib->spring-servlet.xml中有一个关于访问静态资源文件的配置
`<!-- 访问静态文件（jpg,js,css）的方法 -->``<``mvc:resources``location``=``"/files/"``mapping``=``"/files/**"``/>``<``mvc:resources``location``=``"/scripts/"``mapping``=``"/scripts/**"``/>``<``mvc:resources``location``=``"/styles/"``mapping``=``"/styles/**"``/>``<``mvc:resources``location``=``"/Views/"``mapping``=``"/Views/**"``/>``<``mvc:resources``location``=``"/static/"``mapping``=``"/static/**"``/>`原来忘记给static文件夹加上访问映射了，所以找不到，加上最后一句就好啦
每次重新打开项目 Intellij一直处于 scanning files to index…状态
这是因为，，npm install之后，有了一个文件夹 node_modules，实在是很庞大，把它设置为Exclusion文件夹，每次不去添加它就好啦
文件夹上面右击->Mark Directory As->Exclusion
ok啦
其他操作，按照前面提到那篇文章来做就可以啦
—————–此篇完结–后节待续———————————–

