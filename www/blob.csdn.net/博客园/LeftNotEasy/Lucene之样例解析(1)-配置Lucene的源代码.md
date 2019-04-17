# Lucene之样例解析(1)-配置Lucene的源代码 - LeftNotEasy - 博客园







# [Lucene之样例解析(1)-配置Lucene的源代码](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/08/1642182.html)





    从lucene.apache.org上面下载最新版本的Lucene源代码(目前是3.0.0), IDE我选择的Eclipse, 我也不太懂java, 不过非常想能够看看Lucene的底层的运作机制和一些技巧. 下载的包需要是后缀有src的, 我主要想看看源代码的内容, 所以就没有下载编译好的二进制包

    从Eclipse的File->New->Java 选择  Create Project From exsiting Source, 然后选择源代码中的src目录, 自己取一个喜欢的名字, 选择确定后就创建好项目了.

    另外我这边还出现了一点问题, 就是test包下面有非常多的编译错误, 我就懒得管了, 把test先排除在项目之外.就可以开始我们的Lucene之旅了.












