# 在vs中建立web项目和web网站的区别 - Big Smile - CSDN博客
2016年06月23日 21:04:26[王啸tr1912](https://me.csdn.net/tr1912)阅读数：6641标签：[web																[visual studio](https://so.csdn.net/so/search/s.do?q=visual studio&t=blog)](https://so.csdn.net/so/search/s.do?q=web&t=blog)
个人分类：[B/S																[总结																[思考](https://blog.csdn.net/tr1912/article/category/6109423)](https://blog.csdn.net/tr1912/article/category/6109422)](https://blog.csdn.net/tr1912/article/category/6184179)
        在给同学调试牛腩的例子的时候偶然间发现了这个问题，值得去探讨一下。
# 一、建立B/S项目的方式
首先我们来看B/S建立项目的方式，建立网站和建立项目如图：
![](https://img-blog.csdn.net/20160623203606671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        从这两种情况来看生成的项目是不一样的，建立的是项目的话生成的就是带着解决方案，并且sln文件放在项目目录下面，然而建立的是网站的话，sln文件是放在我的文档的vs目录的project下面，而且项目生成的文件比网站生成的项目文件要多（例如*.aspx.designer.cs），且都含有命名空间。这都是很明显的区别。
# 二、根本区别
        从根本的角度看，首先就是生成的文件不同，由于从项目中建立的网站有应用程序的性质，所以它生成的dll文件是可以被别的程序所引用的，而网站生成的都是随机的dll所以也不可能被引用。这里体现出了程序和网站的编译特点，网站是预编译的，所以有的时候不用发布也可以运行，dll是随着编译的时间不断在变化的，而网站应用程序是发布的时候编译的，所以更新之后需要再编译之后才能进行发布。
# 三、维护角度
        由于应用程序的固定性，所以在网站应用程序编译完成之后就需要尽可能少的去改动它，一般设计的时候用户量是固定的，而且对于稳定性要求较高，维护较麻烦。
        网站则属于随时可以修改的地位，因为不用总是编译，所以修改起来很是方便，所以适合便捷的开发，和预实现效果使用，由于没有什么复用性所以比较适合小型的项目，我们以后做项目是不会用它的。
