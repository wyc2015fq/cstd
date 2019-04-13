
# android app开发，如何使用-引入第三方扩展jar - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月19日 18:22:26[Jlins](https://me.csdn.net/dyllove98)阅读数：1174标签：[android																](https://so.csdn.net/so/search/s.do?q=android&t=blog)个人分类：[android																](https://blog.csdn.net/dyllove98/article/category/1130032)


我目前开发的android app需要使用第三方的jar  hessian-4.0.1.jar,于是我把jar加入到了工程中，并通过Order and Export. 进行检查。
Order and Export 提示从第三方jar的所有类都没有加入到运行环境中。
如何才能正确快速的在android工程中使用第三方jar？ 我不太会使用ant 和 Maven

### 处理方法
下面是一个不错的方法，可以有效的在android工程里面添加第三方jar
在工程中创建一个libs的目录
把你的第三方jar加入到libsMul
然后通过  Build Path > Add to Build Path  把jar加入到classpath中
按照上面的步骤做完之后就不会找不到class了

原文地址：[http://www.itmmd.com/201410/89.html ](http://www.itmmd.com/201410/89.html)
该文章由[萌萌的IT人](http://www.itmmd.com/)整理发布,转载须标明出处。

