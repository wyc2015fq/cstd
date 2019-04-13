
# 【随笔】如何向传统企业推荐极简的UI - 全栈空间 - CSDN博客

2019年04月03日 18:42:33[流浪的狗和一坨屎](https://me.csdn.net/github_38885296)阅读数：447标签：[毕设](https://so.csdn.net/so/search/s.do?q=毕设&t=blog)[分类](https://so.csdn.net/so/search/s.do?q=分类&t=blog)[抽象](https://so.csdn.net/so/search/s.do?q=抽象&t=blog)[假设](https://so.csdn.net/so/search/s.do?q=假设&t=blog)[cfg](https://so.csdn.net/so/search/s.do?q=cfg&t=blog)[](https://so.csdn.net/so/search/s.do?q=假设&t=blog)[](https://so.csdn.net/so/search/s.do?q=抽象&t=blog)个人分类：[经验思想/哲学概念](https://jimmy.blog.csdn.net/article/category/7212236)[设计模式](https://jimmy.blog.csdn.net/article/category/8232887)[全栈技术](https://jimmy.blog.csdn.net/article/category/8399683)[哲学](https://jimmy.blog.csdn.net/article/category/8791418)[](https://jimmy.blog.csdn.net/article/category/8399683)
[](https://jimmy.blog.csdn.net/article/category/8232887)
[](https://jimmy.blog.csdn.net/article/category/7212236)
[](https://jimmy.blog.csdn.net/article/category/7212236)
[](https://so.csdn.net/so/search/s.do?q=抽象&t=blog)
[](https://so.csdn.net/so/search/s.do?q=分类&t=blog)
> 这里不谈技术，只谈思想

## 关于我的毕业设计。。。
> 这篇文章目的很简单，就是想把之前零零碎碎的东西找个地方记下来，主要是做个记录。如果恰巧能帮到你那是我的荣幸。
经过若干小时的跪舔，导师终于同意将原来的毕设主题---“移动端自动化测试？？？？”’，改成做一个web app，正好符合我的技术栈，而且可喜可贺的是我可以将之前给公司做的管理系统[CompetenceX](https://github.com/JinHengyu)照搬过来，毕业基本没压力了，所以有时候变通很重要，毕竟你和你的导师都希望你的任务好做一些，而有的同学还卡在“验证码识别MNIST”上不知所措。。
给公司开发的CompetenceX是一个员工信息管理系统，但创新的是，在上世纪风格的窗口标签化的主题笼罩传统制造业企业使用的外包网站的阴霾下， 我决定给他们添上一点material的元素，也就是谷歌风，以及动画：老板们看到后确实眼睛一亮，大加赞赏。正如Steve Jobs说过，**“A lot of times, people don't know what they want until you show it to them.”.**
当一个充满恰当的动画，字体颜色鲜明，操作无需教，而且功能强大的全新网站摆在那些非技术leader面前，他们才知道原来有更优雅的解决方案，变革通常也是这么来的。
下面是一些课设的废话我直接拷贝过来了，可以忽略：
![](https://img-blog.csdnimg.cn/20190403154956140.png)开始。。。。
### 毕业论文题目：
基于web的项目资源分配系统
### 论文（设计）目标：
目前，很多中小型企业有着“上云”的趋势，很多公司将内部的内容管理系统的建设任务外包给软件公司，本次设计的目标是以产品经理和项目经理为目标用户, 搭建一个存储在云端的项目资源管理网站，提供高效的项目资源分配分析服务。
### 论文（设计）内容：
以产品经理和项目经理为目标用户，设计开发一款项目资源管理网站。要求给出合理的需求分析、详尽的总体设计方案以及详细设计说明，编程实现基于web的项目资源分配系统，具有云端数据存储 、在线编辑资源、监控项目资源、数据统计分析及图表绘制等功能，软件最终要完成测试。
### 指定参考文献：
> [1] 韩万江. 软件项目管理案例教程 第三版，北京:机械工业出版社，2015

> [2] HTTP设计模式RESTful API维基百科

> [3] 李洪海、石爽. 交互界面设计 化学工业出版社出版 2011
![](https://img-blog.csdnimg.cn/2019040315501493.jpg)完。。。

## 技术栈与库
毕业设计仍然是比较激进的全栈套餐——MEAN，MRM（自创的，代表MVVM，Restful，MVC），但是没用angular和mvvm，毕竟只是个100+用户的小系统。前端使用了[MDC](https://material.io/develop/web/)这个material框架，以及[Ag-grid](https://www.ag-grid.com/)这个重量级嘉宾，如果你不认识aggrid，可以想象成一个Excel over Web的框架：企业级自然离不开表格。
MongoDB数据库非常好用，因为他既包含了SQL的关系代数的查询技巧，也含有nosql的许多功能，也就是说所有数据库产品的优良特性基本都可以在这里找到，除此之外我还使用了“archiver”这个node库来让用户可以一键“打包备份”，除此之外，expressJS的session库也存储在Mongo中；前端的json-editor框架（一个JSON转编辑器的插件）也直接兼容mongo里的对象；可以说，整个网站架构都发散自mongodb。

## 如何设计全局数据。。以及如何避免bug
全局变量 对于程序员来说，是程序 “同步”的一种最方便的 方式，我相信大多数的程序员 刚开始都是使用全局变量来实现“同步”操作的，但是随着我们 编程理念的升华，我们会发现，原来全局变量不是我们想象的只有好用，而没有缺点。
比如全局变量存放在静态存储区，系统需要为其分配内存，一直到程序结束， 才会释放内存。过多的全局变量还会影响函数的封装性能，降低函数的移值性，降低代码的可读性。
我觉得正确做法就是，全局变量和配置文件统一！统一维护一个cfg.js文件，所有的数据最后挂在在global.cfg下，同时，还要将过滤后的cfg对象传给前端，由js根据cfg填满html中的占位符，由此可以省却html的模板引擎。。
所以说，一切数据来自数据库和配置文件。
不赞同“世上没有0bug的软件”一说，因为理论上软件是可以无bug的，hello world程序不就是吗？不过无bug理论是建立在忽略物理因素，以及足够多的开发时间之上，为了最大程度减少日后发现bug的次数，在一开始设计的时候不仅要考虑到所有情况，还要减少情况。“统一”的思想就是一种减少可能情况的思维模型，【所有数据来自后端的配置文件】就是一种“统一化”的措施，这样避免了日后各种数据杂乱无章的情况。

## 人类3大思考方式
**分类分析，抽象分析和假设分析**是人类分析世界和具体问题的3种方式，但是正真拥有智慧，并且能掌握这三种分析方式的人很少。生活中，为什么有时候和长辈讲道理那么难？
认知水平较落后的人，拒绝对问题进行分类分析，只想把这个世界分成便于他们通过站队而获益的不同类型，他们不会做假设、也不善于抽象分析或逻辑思考。你永远无法说服一个不会抽象思考的人，他可能是你的长辈，也可能是杠精和键盘侠们。抽象就是影藏细节，提取公共特征，越抽象就越哲学；分类则是将所有事物放到一棵树上；假设分析则是模拟可能存在的事物。
我们程序圈，能完美运用这3种分析方式的人也不多，现代编程语言都提供了面向对象模型，类型，逻辑控制等符合分类分析，抽象分析，假设分析的工具，但如何用这些工具来分析具体问题却止步于许多程序员的固化思维。
这是毕设一边做一边总结的，清明节继续更~
2019年4月3日18:39:22

[](https://so.csdn.net/so/search/s.do?q=分类&t=blog)
[](https://so.csdn.net/so/search/s.do?q=毕设&t=blog)