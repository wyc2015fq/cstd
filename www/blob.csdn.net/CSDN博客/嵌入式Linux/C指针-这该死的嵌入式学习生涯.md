
# C指针-这该死的嵌入式学习生涯 - 嵌入式Linux - CSDN博客

2018年08月25日 09:44:25[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：470所属专栏：[微信公众号:嵌入式linux](https://blog.csdn.net/column/details/26885.html)



# 01 前言
最近在公众号里面收到好几个同学关于嵌入式方面的咨询，再加上在知乎里面陆续推送了好几个嵌入式学习入门的问题，这次想统一整理一下，说说我这些年是如何被嵌入式按在地上摩擦的。
![](https://img-blog.csdn.net/20180903114715925?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**1、那一年夏天**
2008年的夏天，我记得很清楚，我和XH坐在电脑前两眼迷茫填报我们的高考志愿，我们当时没有当多的选择，说实话，我不知道以后想做什么，想从事哪方面的工作，刚好XH的舅舅在华为上班，当然，我们那时候也不知道华为是个怎么样的企业，我们就想着报考电子信息工程，于是我们选择了两个学校，桂林电子科技大学和广西师范大学，因为这两所大学都有电子信息工程的专业，据说学了这个专业以后可以去广东打工，一个月可以拿4000块钱的工资。我们报考广西师范大学作为第一志愿，这其中主要有两个原因
1、广西师范大学是一本学校，如果录取了至少在面子上会很那个，会让别人觉得你牛逼哄哄可以飞起来的那种。
2、广西师范大学的电子信息工程是师范兼非师范专业，可以这么说，我以后毕业了，可以做老师，也可以不做老师，这无形中就有了两个选择，不过最后证明这个是假的。
**2、嵌入式开发需要哪些基础?**
刚进学校后，同宿舍的葛子告诉我们，我们学院有一个电子创新基地，但是进基地学习需要经过一个非常牛逼的基地老大同意，而且一定自己有电脑。我们宿舍几个很疯狂，赶紧一起去买了电脑，然后经过了一件不愉快的沟通后非常不顺利的进入了创新基地。我那时候也不知道创新基地是搞什么的，听说是学院的大神都是基地的，基地都是大神的，这不是一个因果关系，也不是一个并列短语，这是一个必然关系。
**下面是正式文件，注意看好**
我们在大一在大二的课程，可以说是作为一个优秀电子开发的基础，其中包括
1、C和C++ 编程
2、汇编入门编程，这个主要是配合51单片机来学习
3、数字电路基础
4、模拟电路基础
5、一些常用的工具软件、protel、keil
上面说的都是基础，如果你要问我，这些基础有什么用？可能我会说，我也不知道有什么用，就像我们学习汉字一样，a 、o、 e 就是基础，这其中还包括动手能力，你说你不自己去稀释几个电路板，不自己焊接个电阻电容，不搞个电容爆炸，你都不好意思在同班的妹子面前说你是学电子信息专业的。
但是一定要具备这些才能搞嵌入式吗？答案肯定不是，我在毕业的第一年，在TCL上班，我们部门有一个嵌入式工程师，以前是厨师，这确实没有打错字，确实是厨师，但是他做了一个很不错的产品，那个产品在中南海的都有展示用到。
学完上面后，后面会有PLC，FPGA，微机原理，信号与系统，高数、等等，能学好这些的同学肯定不是普通的大牛，如果以后想往算法方面发展，这些是很重要的东西。当然PLC不是，这个是工控的东西，一个偏冷门的东西，像电梯控制系统就是用的PLC。
跑单片机只是一个基础，单片机是单核工作，是单线程工作，什么是单线程？可以这么理解，一个人在一个时间点只能做一件事，比如你吃饭的时候不能玩手机，你看手机的时候不能吃饭，这是一样一样的，从STC89C51开始，转到AVR再转到STM32，这些学习，我都不是游刃有余的，说实话，都是半桶水，反正我总是会在我的工位上跑各种流水灯吸引路过的同学让他们觉得我是一个非常牛逼的大神。老师不在的时候玩CS，double kill的声音在创新基地的上空回绕。
RTOS和嵌入式Linux这个才是我们要重视的，单片机开发的难度和薪水都比不上做嵌入式系统开发的，可以这样比喻，开拖拉机的工资肯定没有开火车的工资高啊，当然那时候我也不懂，我都是听一个叫KT的大神告诉我的，同学们都在学，我也不想自己被拉下，所以我也拼命去学。
说个题外话，我学嵌入式的第一个代码并不是“Hello,World”,我们是流水灯，哈哈，点亮一个LED灯才是我们的第一门课程。
总结说一下：
1、学习不要图快，先尝试理解，再实际动手操作
2、做笔记是很重要的，以后遇到问题也可以追溯
3、学习资料用一个就好，比如你学单片机，买个资料足够的开发板来学习就好，如果你学习Linux，你也买个带资料的开发板来学习就好，不要看到网上这个资料那个资料疯狂拷贝填充自己的电脑到头来都没时间看。
**我这里给一些基础的资料分享给大家**
C和C++
链接：https://pan.baidu.com/s/1EAgGYRfutnMeRUFTbRtLKQ 密码：whvr
《Altium Designer6.9 PCB设计教程(郭天祥)》
链接：https://pan.baidu.com/s/1B5j0y04FRahRzMi0thYoXA 密码：f61a
十天学会dan片机C语言
链接：https://pan.baidu.com/s/1pIxEjjIHwLq1nI7NNZOAig 密码：a8nw
Linux开发视频学习
链接：https://pan.baidu.com/s/1PJGrlV5Bx2jhcAjeJoEJpw 密码：nppy
**3、嵌入式是做什么的？**
在大一大二的时候，我根本不知道以后工作搞什么东西，我还是那个目标，以后要是能有个4000块钱的工资，我就烧高香了，心里也暗笑，以后每个月可以给家里寄个500块钱了。每次这样笑的时候，嘴角总是流了很多口水，醒了才知道，又做了一个美梦。嵌入式可以简单分为嵌入式软件和嵌入式硬件，当然可能还有其他，可能我还没有涉及。
**嵌入式硬件**
1、电源工程师，就是专门搞电源的，别小看这个东西非常值钱
2、基带工程师，手机里面的硬件工程师，评估原理，方案设计，芯片选型的。
3、射频工程师，专门搞天线，搞射频之类的，书里面说的载波信号之类的他们都懂。
4、layout工程师，专门画pcb板的，走线之类的非常需要注意，不是每个人都可以搞定的。
5、打杂工程师，这个自行体会
当然他们的工作可能会重合
**嵌入式软件**
嵌入式软件工程师，我其实就想说主要写C代码的都可以叫嵌入式软件工程师。但是不写C代码也有可能是嵌入式软件工程师，比如C++,汇编
你说手机是不是嵌入式，手机软硬件是不是可以裁剪？如果是，为什么不算嵌入式？那手机还有APP，后台，framework,hal,驱动，所以很多同学问，嵌入式软件以后做什么，所以我现在可以这么建议
去看看你想做的产品，不管是硬件还是软件，我们最终展示的肯定是一个产品形态给大家，你去查下这个产品好吧。查这个产品是哪个公司搞的，比如小米手机。
到他们官网上看手机的招聘信息，然后再点开看里面的岗位要求，就可以大概明白以后这个岗位的基本要求
![](https://img-blog.csdn.net/20180825093740402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)我只截图了一部分，做好一个产品需要的人员很多，细分也很多，当然有很多小公司可能一个人就能把他们上门的都干了，每个人的能力都有差异，大公司和小公司的体系也不一样。
**4、遇到问题了怎么办？**
![](https://img-blog.csdn.net/20180825093740508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
作为一个初级程序员，我当年在我们的基地，每天都能遇到一个或者几个需要几个师兄都可能解决不了的问题，不是问题能多难，但是就是会花费很多时间。
举个栗子：
我：师兄，我的代码怎么编译不过了？
师兄：你是什么代码啊？
我：STC89C51代码啊，就几十行的代码？
师兄：你CLEAN掉重新试试
我：（很焦急）还是不行呐，怎么这样的
师兄：我来看一下
然后师兄XXOO帮我解决了，但是我从中并没有学会如何排查问题的能力，当我以后遇到问题的时候，我潜意识还是找师兄，如果师兄不在，那就悲剧了。
**所以我总结一下，出现问题怎么办呢？纯粹是个人观点。**
1、www.baidu.com www.google.com www.bing.com 学会用搜索引擎来找自己的答案，比如我在我的QQ群里面，很多编译错误的问题，这种问题算是小白了吧，正常的编译错误在百度都能查到。
2、 做开发之前，你有没有做备份的习惯，比如用GIT，SVN，来搭建自己的本地服务器，有备份代码的习惯，这样出现问题可以回退回去找原因，有时候做开发没必要花费时间在这些事情上面。
3、如果是调试某个芯片出现问题，你有没有尝试查询手册资料？有没有查询FAQ资料？有没有自己检查，通过实验来验证自己的想法？
**如果你上面都做了，你应该更好的提问题呢？**
1、描述好当时使用的软件环境，SDK版本，哪里搞来的代码给大神说清楚
2、硬件是什么搭建的，如果是有开发板最好直接用开发板验证，没有开发板贴上关键的原理图。
3、你尝试过什么方法去排除问题，你这样说了，大神就知道你做了一些工作，可能你的这些排除方法也可能是他会让你去做的，他知道你做了，他就好安心去给你分析了。
4、附带LOG，出现问题的路径，好让大神去给你复现。
5、如果你有网络账号，在网络上描述清楚，直接发个链接过去，这样不仅你问的大神可以看到，很多网络大神也可以看到。
**5、做项目写代码**
你以为搞完上面就可以做项目万事大吉了，NO,NO,NO。
做好一个好的产品需要多方面的因素，写好代码也需要很多方面的学习，我大概总结了一下
1、变量命名，函数命名，如果你使用别人的sdk,代码风格要沿用原来的风格
2、要有一定的代码注释，不管是大神还是新手，没有注释的代码就是不规范的。
3、加上必要的日志，要在出问题的时候打印关键信息。
4、代码结构，代码结构非常关键，些代码有对象的思维就会很好。
分享一个业界大家都看的文档
华为编程规范
链接：https://pan.baidu.com/s/1jISE6qjkQ5y0_0tGRiJZlg 密码：xruv
华为布线规范-这个没研究过
链接：https://pan.baidu.com/s/1bnoz5dd1fUyDOSLEVEZh_g 密码：2127
**后语：以上内容只是个人观点，上面的内容可能还不能满足你求知的欲望，可以关注公众号我们找几个大神来一起探讨。**

# 02 正文
写完上面的内容，我还要写点跟技术相关的，这次把我以前对指针翻译放上来，希望对大家有帮助。
The first things to do with pointers are to declare a pointer variable, set it to point somewhere, and finally manipulate the value that it points to. A simple pointer declaration looks like this:
对指针的第一步操作就是声明一个指针变量，让它指向某个地方，最后操作指针指向的值（一个指针就是一个地址，内存的一个地址代表的就是一个指针，这个地址保存的是什么鬼东西，就是这个指针指向的地方），一个简单的指针声明如下：
int *ip;
This declaration looks like our earlier declarations, with one obvious difference: that asterisk. The asterisk means that ip, the variable we're declaring, is not of typeint, but rather of type pointer-to-int. (Another way of looking at it is that *ip, which as we'll see is the value pointed to by ip, will be an int.)
这个声明看起来非常像我们之前的变量声明，但是有一个明显的不同之处，就是那个星号（*），帅帅的星号跟着ip，这个我们声明的ip变量，不是int类型，它是一个指向整型的指针。（用另一种方法来看这个表达式是，先看*p做一个整体，*p的类型是一个int类型，然后*p分开，把*看到是取地址符号，那ip就是一个指针，也即一个地址，*ip取这个地址的值，这个地址的值是一个int类型）
We may think of setting a pointer variable to point to another variable as a two-step process: first we generate a pointer to that other variable, then we assign this new pointer to the pointer variable. We can say (but we have to be careful when we're saying it) that a pointer variable has a value, and that its value is ``pointer to that other variable''. This will make more sense when we see how to generate pointer values.
我们会想设置一个指针指向另外一个变量要用两个步骤，首先，我们声明一个存放在其他地方的变量，然后我们把这个变量的地址赋值给这个指针。我们可以这样说（但必须注意我们的描述）指针变量保存有一个值，并且它的值是指向其他变量的。这会使得我们对如何生成一个指针变量更加直观。
Pointers (that is, pointer values) are generated with the ``address-of'' operator &, which we can also think of as the ``pointer-to'' operator. We demonstrate this by declaring (and initializing) an int variable i, and then setting ip to point to it:
Pointers这个指指针的值，通常我们会用到取地址符号（&），当然我们也可以把它认为是“pointer-to”操作。我们声明一个int变量i,然后把ip指向这个变量i.
int i = 5;
ip = &i;
The assignment expression ip = &i; contains both parts of the ``two-step process'': &i generates a pointer to i, and the assignment operator assigns the new pointer to (that is, places it ``in'') the variable ip. Now ip ``points to'' i, which we can illustrate with this picture:
ip=&i;这个表达包含两个步骤，&i是一个指向i的指针，然后把新声明的指针ip指向这个地方，也即这个指针，这个地址。好了。现在ip就指向了i,我们可以用如下图示来表示：
i is a variable of type int, so the value in its box is a number, 5. ip is a variable of type pointer-to-int, so the ``value'' in its box is an arrow pointing at another box. Referring once again back to the ``two-step process'' for setting a pointer variable: the & operator draws us the arrowhead pointing at i's box, and the assignment operator =, with the pointer variable ip on its left, anchors the other end of the arrow in ip's box.
i是一个整型变量，所以在框框里的是一个数字5，ip是一个指向整型变量的指针，所以ip保存的值是一个地址，并且这个地址保存的是一个整型变量。重新回来看一下两步操作法：为了设置一个指针变量，&操作取得指向框框里的地址，然后是“=”操作符，把指针变量ip放在左边，最后重点是i的地址保存在了ip的框框里。
We discover the value pointed to by a pointer using the ``contents-of'' operator, *. Placed in front of a pointer, the * operator accesses the value pointed to by that pointer. In other words, if ip is a pointer, then the expression *ip gives us whatever it is that's in the variable or location pointed to by ip. For example, we could write something like
我们发现要取得指针指向的值用（*）操作符，放在指针的前面，（*）操作取得指针指向地址的值，换句话说，如果ip是一个指针，*ip表达式告诉我们ip这个地址的值保存的值是多少，例如，我们可以这样写一行代码：
printf("%d\n", *ip);
which would print 5, since ip points to i, and i is (at the moment) 5.
当然这会打印5，由于ip指向i,i的值（此时）是5.
(You may wonder how the asterisk * can be the pointer contents-of operator when it is also the multiplication operator. There is no ambiguity here: it is the multiplication operator when it sits between two variables, and it is the contents-of operator when it sits in front of a single variable. The situation is analogous to the minus sign: between two variables or expressions it's the subtraction operator, but in front of a single operator or expression it's the negation operator. Technical terms you may hear for these distinct roles are unary and binary: a binary operator applies to two operands, usually on either side of it, while a unary operator applies to a single operand.)
你一定想知道（*）取地址值的操作，与此同时，他也是乘法操作符。在这里是没有一点模糊的，乘法操作要具有两个变量，当它作为取地址值操作符时，它放在变量的前面，它就像减号（-）一样，当它在两个变量或者堂之间时，它是减号的功能，当它在一个常量前面时，它被当作负号。这就像一个操作符具有两种功能，简单叫它作二次元吧。哈哈！
The contents-of operator * does not merely fetch values through pointers; it can also set values through pointers. We can write something like
取地址值符（*）不仅仅是取得地址的值，也可以通过这个方法来设置一个地址的值。像下面一样：
*ip = 7;
which means ``set whatever ip points to to 7.'' Again, the * tells us to go to the location pointed to by ip, but this time, the location isn't the one to fetch from--we're on the left-hand sign of an assignment operator, so *ip tells us the location to store to. (The situation is no different from array subscripting expressions such as a[3] which we've already seen appearing on both sides of assignments.)
这是把ip指向的值设置为7.再次，（*）符号告诉我们它是取得（ip）地址的值的。但是这次，这个地址不是一个取得的值，左边的表达式（*ip）是取得ip指向的位置。（这就像数组里面的a[3]取得数组里面第2个元素的值一样）
The result of the assignment *ip = 7 is that i's value is changed to 7, and the picture changes to:
*ip=7这个表达式的结果是把ip这个地址的值变为7.上面的图改成下面：
![](https://img-blog.csdn.net/20180825093740508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
If we called printf("%d\n", *ip) again, it would now print 7.
我们当然可以用打印来看到*ip的值。它会打印7.
At this point, you may be wondering why we're going through this rigamarole--if we wanted to set i to 7, why didn't we do it directly? We'll begin to explore that next, but first let's notice the difference between changing a pointer (that is, changing what variable it points to) and changing the value at the location it points to. When we wrote *ip = 7, we changed the value pointed to by ip, but if we declare another variable j:
int j = 3;
and write
ip = &j;
we've changed ip itself. The picture now looks like this:
指针，我们可能想知道为啥我们写这么多这么啰嗦。如果我们想把它设置成7.为什么我们不直接操作，我们下面会探讨这个，但是首先让我们注意改变指针的值和改变指向指向位置的值的不同之处。当我们写*ip=7我们是改变指针指向地址的值的。但是如果我们声明另一个变量j.
int j=3;
然后写
ip=&j;
我们改变了ip的值，如下图：
我自己试了一下。要这样写才会正确：如果不要int i=8;int *ip=&i;只写成int*ip;可能会造成ip指向的地方不合法，会出错。

```python
#include 
#include 
#include 
int main(void)
{    
    int i=8;
    int *ip=&i;
    *ip=7;
    printf("%d\n",*ip);
    return 0;
}
```
We have to be careful when we say that a pointer assignment changes ``what the pointer points to.'' Our earlier assignment
我们一定要注意，当我们说一个指针指向改变的时候，“指针指向什么？”我们先前的分配是这样的。
*ip = 7;
changed the value pointed to by ip, but this more recent assignment
改变ip指向地址的值，但是我们大多数是这样声明的。
ip = &j;
has changed what variable ip points to. It's true that ``what ip points to'' has changed, but this time, it has changed for a different reason. Neither i (which is still 7) nor j (which is still 3) has changed. (What has changed is ip's value.) If we again call
ip=&j通过改变ip来达到改变ip指向的值。它是让ip指向的值发生了改变，但是这时，它因为另一个原因发生了改变。不是i的值让ip发生了变化，我们可以再来一次打印。
printf("%d\n", *ip);
this time it will print 3.
这时候它会打印出3来。
We can also assign pointer values to other pointer variables. If we declare a second pointer variable:
我们也可以把指针赋值给另一个指针
int *ip2;
then we can say
这时我们可以这样写
ip2 = ip;
Now ip2 points where ip does; we've essentially made a ``copy'' of the arrow:
这时ip2指向的地方也是ip指向的地址，它本质上只是一个复制操作
Now, if we set ip to point back to i again:
现在我们把ip又改回i的地址
ip = &i;
the two arrows point to different places:
这时两个指针指向了不同的地方
We can now see that the two assignments
这时我们可以看到下面两个表达式
ip2 = ip;
and
*ip2 = *ip;
do two very different things. The first would make ip2 again point to where ip points (in other words, back to i again). The second would store, at the location pointed to by ip2, a copy of the value pointed to by ip; in other words (if ip and ip2 still point to i and j respectively) it would set j to i's value, or 7.
是两个不同的东西。第一个是把ip和ip2是同一个地址。第二种是保存的值，*ip2是ip2地址的值，*ip是ip地址的值，换言之(如果ip 和ip2仍然指向i和j),它会把i的值赋给j.
It's important to keep very clear in your mind the distinction between a pointer and what it points to. The two are like apples and oranges (or perhaps oil and water); you can't mix them. You can't ``set ip to 5'' by writing something like
在里心里，指针和指针指向的值你一定要有一个明确的区分，他们两个就像苹果和橘子一样。你一定不能混肴他。你不能把指针赋值5.像下面一样，是错误的。
ip = 5; /* WRONG */
5 is an integer, but ip is a pointer. You probably wanted to ``set the value pointed to by ip to 5,'' which you express by writing
5是一个整型，但是ip是一个指针，你大概像把ip指向的值设置成5.你可以这样写。
*ip = 5;
Similarly, you can't ``see what ip is'' by writing
你也不能通过这样来查看ip的值。
printf("%d\n", ip); /* WRONG */
Again, ip is a pointer-to-int, but %d expects an int. To print what ip points to, use
再说一次，ip是一个指向整型的指针，但是%d期望是整型的，打针ip指向的值，通过下面的打印。
printf("%d\n", *ip);
Finally, a few more notes about pointer declarations. The * in a pointer declaration is related to, but different from, the contents-of operator *. After we declare a pointer variable
最后，一小点笔记关于指针声明，（*）在声明指针中，但是有区别于操作指针，在我们声明指针后
int *ip;
the expression 下面的表达式
ip = &i
sets what ip points to (that is, which location it points to), while the expression
指导ip指向的值设置为5
*ip = 5
sets the value of the location pointed to by ip. On the other hand, if we declare a pointer variable and include an initializer:
设置ip指向的值，另一方面，我们可以在声明的时候包含初始化指针的值。
int *ip3 = &i;
we're setting the initial value for ip3, which is where ip3 will point, so that initial value is a pointer. (In other words, the * in the declaration int *ip3 = &i; is not the contents-of operator, it's the indicator that ip3 is a pointer.)
在这里（*）不是说取得ip3指向的值，而是告诉编译器，ip3是一个指针，然后把这个指针初始化为i的地址
If you have a pointer declaration containing an initialization, and you ever have occasion to break it up into a simple declaration and a conventional assignment, do it like this:
你有一个指针声明如下，并用第二步来对他进行初始化
int *ip3;
ip3 = &i;
Don't write
但是不可以这样写
int *ip3;
*ip3 = &i;
or you'll be trying to mix oil and water again.
否则，你又把油和水弄乱了
Also, when we write
当然我们也可以这样写。
int *ip;
although the asterisk affects ip's type, it goes with the identifier name ip, not with the type int on the left. To declare two pointers at once, the declaration looks like
*告诉编译器，后面的变量是一个指针，我们可以像下面一样来声明两个指针
int *ip1, *ip2;
Some people write pointer declarations like this:
一些人这样来声明一个指针。
int* ip;
This works for one pointer, because C essentially ignores whitespace. But if you ever write
它在声明一个指针是正确的，但是C本质上忽视空格，但是你绝不能这样写
int* ip1, ip2; /* PROBABLY WRONG */
it will declare one pointer-to-int ip1 and one plain int ip2, which is probably not what you meant.
他会声明一个指向整型的指针，和一个整型变量ip2.当然不是你想要的意思
一个整型变量ip2.当然不是你想要的意思
### 喜欢可以关注微信公众号：嵌入式Linux



