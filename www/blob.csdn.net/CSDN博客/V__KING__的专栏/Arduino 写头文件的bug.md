# Arduino 写头文件的bug - V__KING__的专栏 - CSDN博客





2014年01月02日 12:11:16[v__king__](https://me.csdn.net/V__KING__)阅读数：2780








**问题：**

     居然不能自己添加头文件，自己添加的时候会出现  'byte' does not name a type  的错误。如果你不用这个头文件而是在.ino中定义同样的变量，复制到.ino中，就不会出错。why？

##解释：

    letters.h 文件中少了一些头文件


![](https://img-blog.csdn.net/20140102120615296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVl9fS0lOR19f/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20140102120628656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVl9fS0lOR19f/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**测试——没问题的添加头文件：**![](https://img-blog.csdn.net/20140102121002218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVl9fS0lOR19f/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20140102121047921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVl9fS0lOR19f/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






