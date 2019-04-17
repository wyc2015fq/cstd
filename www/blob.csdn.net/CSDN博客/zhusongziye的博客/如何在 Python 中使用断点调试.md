# 如何在 Python 中使用断点调试 - zhusongziye的博客 - CSDN博客





2018年10月18日 22:11:59[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1942








*实际上没人能一次就写出完美的代码，除了我。但是世界上只有一个我。*

*-- 林纳斯·托瓦兹（Linux 之父）*



既然不是神，写代码自然免不了要修改。修改代码的过程被称作调试，又叫 debug。



刚接触编程的人，往往对调试代码没有很深的认识，觉得把功能做出来就完事了。一旦程序运行出现问题，便无从下手。而实际开发中，调试代码通常要花费比新编写代码更多的时间。所谓“行百里者半于九十”，这句话放在软件开发上一点不为过。你以为完成了 90% 开发工作的时候，剩下的工作可能还要花费差不多的时间。



因此，选择合理的调试方法和工具，对于开发来说意义重大。



在写 Python 代码的时候，我习惯的调试方法很简单，就是在程序里增加输出，以便了解程序的运行路径和变量的值。



以下面这段代码为例：



> 
def twice(n):

    n *= 2

    return n



a = input("a:")

b = input("b:")

if a > 3:

    b += 4

    if b > 5:

        c = a + twice(b)

    else:

        c = twice(a) + b

else:

    b -= 2

    if b < 1:

        c = a - twice(b)

    else:

        c = twice(a) - b

print c




即使这样不长的一段代码，如果对于某些输入，最后输出的值和你的预期不一致，想要用肉眼从中看出到底哪里出了问题也并不是一目了然的事情。



于是我会添加一些辅助的代码：



> 
def twice(n):

    n *= 2

    return n



a = input("a:")

b = input("b:")

print "====a,b:", a, b 

if a > 3:

    b += 4

    print "====1 b:", b

    if b > 5:

        c = a + twice(b)

        print "====1 c:", c

    else:

        c = twice(a) + b

        print "====2 c:", c

else:

    b -= 2

    print "====2 b:", b

    if b < 1:

        c = a - twice(b)

        print "====3 c:", c

    else:

        c = twice(a) - b

        print "====4 c:", c

print c




运行后的效果：



> 
a:2

b:4

====a,b: 2 4

====2 b: 2

====4 c: 2

2




通过添加类似的输出内容，就能比较清晰地了解程序的运行状态。





不过，直接通过输出来调试，有时候会需要一点经验判断，决定在哪里输出什么数据。尤其当程序复杂之后，需要参考的数值会很多，全部都输出会很繁琐。



一种更便捷更直观的方式就是使用断点调试。



断点（break point）是指在代码中指定位置，当程序运行到此位置时变中断下来，并让开发者可查看此时各变量的值。因断点中断的程序并没有结束，可以选择继续执行。



实现断点调试需要 IDE 的支持。下面以 PyCharm 来演示一下，仍然是前面的代码：



![](https://img-blog.csdn.net/20181018221042387?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



在某一行代码的左侧栏点击，就可以在这行增加断点。现在在 3 个 if 判断的地方都加上断点。



选择 debug，程序开始运行。



![](https://img-blog.csdn.net/20181018221050894?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



输入 a、b 两个值之后，程序就在第一个 if 处中断，并且在工具栏中显示出当前程序中的各种数值。



![](https://img-blog.csdn.net/20181018221058857?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



选择“resume program”，程序就会继续执行，直到遇到下一个断点。



![](https://img-blog.csdn.net/20181018221110339?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



有了断点功能的帮助，调试的时候就方便许多，很容易观察程序运行时的状态。





在断点调试时，还有个经常一起使用的功能就是单步调试。



![](https://img-blog.csdn.net/20181018221122300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



在程序中断时，选择“Step Over”，会向下执行一行代码后继续中断。



当所处代码行中有函数时，“Step Into”会中断在函数内部，之后将在其中单步执行。对应的，“Step Out”将中断在执行完当前所在函数后的位置。



![](https://img-blog.csdn.net/20181018221132360?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



这些操作在工具栏有快捷按钮，也可通过快捷键操作。



![](https://img-blog.csdn.net/20181018221139812?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





“工欲善其事，必先利其器”。把上述调试工具熟悉之后，对你的开发效率将会有很大提升。



当然，比调试工具更重要的，还是调试的思路。遇到错误时，读懂报错信息，分析出错原因，并逐步定位问题所在，而不是盲目地修改代码、无意义地重复运行，才是解决问题的关键。



之后会继续跟大家分享调试代码的经验，这些经验都是一行行代码积累出来的。不过我的经验终究是我的经验，要把它变成你的经验，仍然需要经过一行行代码的磨练。



没有捷径。



