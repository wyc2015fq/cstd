# unix-002-chmod使用 - 小灰笔记 - CSDN博客





2018年01月25日 22:36:59[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：84








unix-002-chmod使用

       一直以来，我使用chmod的习惯都是chmod 777，也都懒得去做什么精细的配置。我的目的一般就是执行，而我也不在乎是否被人知道或者修改。本来我自己的环境就相对封闭。不过，涉及到公司的话这个问题就来了，相应的权限管控还是要做的好一点。尤其是多人共用的文件，若是个人自己用的倒也没有太大的需要。

       先看看简单的执行属性修改，编写一个Python脚本和一个调用它运行的shell脚本，尝试运行如下：

![](https://img-blog.csdn.net/20180125223440250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       修改属性并执行：

![](https://img-blog.csdn.net/20180125223459964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       执行成功。其实，如果仅仅是为了执行，大可没必要直接给出那么open的设置。如果在写一个shell脚本内容相同，属性如下：

![](https://img-blog.csdn.net/20180125223516356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       如果要让新创建的脚本文件具有可执行的属性，直接进行如下操作：

![](https://img-blog.csdn.net/20180125223530556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       由此，简单的具有执行属性的修改还是比较简单的。只需要增加一个整型属性，而这个属性的增加会面向拥有者、用户组以及其他人。如果是设置为更为详细的方式，那么需要了解一下权限设置的三段信息。从上面截图中就能够看得到三段信息，也就是-开头的rwx的组合。其中，r代表读、w代表写、x代表执行。对应的三种操作，有三个数字表示方式，读4、写2、执行1。实践中，个人觉得熟悉一下数字是最好用的，看着比较清爽。

       如此，再创建一个新文件看一下属性信息：

![](https://img-blog.csdn.net/20180125223550868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       刚创建，属性一般只有读写，而写权限只有拥有者具有。如果改成所有的都可执行，通过三段式的方式如下：

![](https://img-blog.csdn.net/20180125223603562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       从上面的属性信息也能够看得出属性修改成功。再次改回全都只读，然后尝试数字式的修改。为了保证演示好看一些，我在脚本文件中输入了一点命令信息。具体如下：

![](https://img-blog.csdn.net/20180125223621757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       可以看得出，可以读但是不能够执行。但是，解析器是可以解析的。至于写权限，就不去测试了。接下来，把这个属性修改成拥有者全部权限，用户组读、执行，其他用户执行。熟悉了以后，很自然知道数字对应着751。对应的操作如下：

![](https://img-blog.csdn.net/20180125223634211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       比较顺利，看得出相应的权限设置已经成功。不过，对我自己的需求来说，目前的需要基本上还是777。



