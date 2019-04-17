# 利用linux的mtrace命令定位内存泄露(Memory Leak) - DoubleLi - 博客园






  一谈到内存泄露， 多数程序员都闻之色变。 没错， 内存泄露很容易引入， 但很难定位。  以你我的手机为例（假设不经常关机）， 如果每天泄露一些内存， 那么开始的一个星期， 你会发现手机好好的， 当内存泄露积累到一定程度，  那就是各种卡死了， 系统异常， 最后死机， 不得不重启。

        如果搞开发， 遇到内存泄露问题， 那就呵呵了。 你可能先得花好几天来复现问题（泄露积累）， 然后需要花好几天来定位问题和修改问题， 然后又要花好几天来验证问题， 而且， 很有可能没法一次改好， 上述流程又要循环了。 确实挺苦逼的。

        我个人认为， 在内存泄露问题上， 主动预防比被动定位要划算得多， 但无论你怎么预防， 总有掉链子的时候， 所以， 有时候不得不去被动定位内存泄露。



        在本文中， 暂不谈论手机内存泄露问题的定位， 仅仅介绍一个有用的linux小命令：mtrace(memory trace)， 它可以用来协助定位内存泄露。 搞开发的， 应该或多或少地听说过mtrace.

        下面， 我们来看看程序：



**[cpp]**[view plain](http://blog.csdn.net/stpeace/article/details/46642507)[copy](http://blog.csdn.net/stpeace/article/details/46642507)



- #include <stdio.h>  
- 
- int main()  
- {  
-         setenv("MALLOC_TRACE", "taoge.log", "1");  
-         mtrace();  
- 
- int *p = (int *)malloc(2 * sizeof(int));  
- 
- return 0;  
- }  


       有的朋友要说了， 一眼就能看出内存泄露啊。 但是， 当程序大了之后， 怎能仅仅依靠肉眼？ 好， mtrace该出场了。





        编译：gcc -g -DDEBUG test.c   (千万要注意， -g不可漏掉， 否则， 虽然最后能定位到内存泄露， 但却找不到在代码的第几行。由于我代码中没有Debug宏控制， 所以编译时， -DDEBUG是可以省略的， 因此， 直接写成gcc -g test.c即可)

        运行：./a.out

        定位：mtrace a.out taoge.log

        结果：

![](http://img.blog.csdn.net/20150625231638873?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3RwZWFjZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       可以看到， 有内存泄露，且正确定位到了代码的行数。



       我们想一下mtrace函数/命令的原理， 其实也很简单， 无非就是记录每一对malloc/free的调用情况， 从这个意义上来讲， mtrace替代了部分我们的眼睛， 紧紧地盯着malloc/free， 所以能看到泄露还是不泄露啊。





**说明一下， 我的linux上并没有安装mtrace命令， 所以无法调试， 在网友Jukay的帮助下， 我才接触到shiyanlou这个优秀的在线工具， 地址是：https://www.shiyanlou.com/ , 大家不需要注册， 直接用QQ登录即可， 上面的过程就是在shiyanlou中做的。 没有linux环境的朋友们， 以后就可以在这上面玩了， 不要再扯理由说没有linux环境啦。 再次感谢Jukay介绍这么优秀的在线工具。**



       OK,  本文先写到这里， 后续会继续介绍一些与linux有关的基本调试工具和方法。











