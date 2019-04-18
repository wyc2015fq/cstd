# Shell编程中的“局部变量”和“导出变量” - nosmatch的专栏 - CSDN博客
2011年08月12日 18:51:18[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：1959
**情景一：建立一个shell脚本var1.sh，其内容为：**
[rocrocket@rocrocket SHELL]$ cat-n var1.sh
1#!/bin/bash2echo :$myvar:
然后在终端上执行变量赋值语句并用echo显示此变量：
[rocrocket@rocrocket SHELL]$ myvar=hello
[rocrocket@rocrocket SHELL]$ echo$myvar
hello
但是当执行var1.sh脚本，想显示myvar变量时，情况却并非你所愿：
[rocrocket@rocrocket SHELL]$ bash ./var1.sh
::
显然，var1.sh并没有看到用户在终端中定义的myvar变量。可见，在终端中设置的变量，在运行的shell脚本中是无法看到的。
**情景二：我们建立一个脚本var2.sh，内容为：**
[rocrocket@rocrocket SHELL]$ cat-n var2.sh
     1#!/bin/bash2myvar=503echo :$myvar:
此时，用户在终端设置myvar的值并显示此值，然后再运行var2.sh，看看结果吧：
[rocrocket@rocrocket SHELL]$ myvar=100[rocrocket@rocrocket SHELL]$ echo$myvar100[rocrocket@rocrocket SHELL]$ bash var2.sh
:50:
可见，终端中设置的值，在shell脚本中是无法改变的；而且，终端也无法改变shell脚本里面设置的变量的值。
**阶段总结：**
在Shell工作机制中，存在一个子shell的概念，上面的两个情景展现的就是子shell的形象。子shell是登录shell为了运行某个脚本程序而建立的一个全新的shell，这个全新的shell只会使用自己的局部变量，对父shell（登录shell）的局部变量不屑一顾。而且，子shell和父shell对对方的局部变量都不会有任何冒犯，井水不犯河水。
**引子：**
有些事情偏偏是需要子shell和父shell有所交互的，这就需要有一个变量是子shell和父shell都能看到和修改的。这时候，export出现了，它可以“导出变量”。
**情景三：建立一个新的shell脚本var3.sh，其内容如下：**
[rocrocket@rocrocket SHELL]$ cat-n var3.sh
     1#!/bin/bash2echomylocal=$mylocal3echomyglobal=$myglobal
在var3.sh脚本中，设置了两个变量，一个是mylocal，表示局部变量，一个是myglobal，表示全局变量。
在终端上给此两个变量赋值，然后执行脚本程序var3.sh，很明显，结果如你所愿，是空值：
[rocrocket@rocrocket SHELL]$ mylocal=1[rocrocket@rocrocket SHELL]$ myglobal=2[rocrocket@rocrocket SHELL]$ bash var3.sh
mylocal=
myglobal=
我们针对myglobal执行export来导出变量，再看看结果：
[rocrocket@rocrocket SHELL]$ export myglobal
[rocrocket@rocrocket SHELL]$ bash var3.sh
mylocal=
myglobal=2
看，我们的脚本程序已经看到了myglobal的值了，看来export将myglobal导出，真是起到作用了。
**阶段总结：**
他的内幕是这样的：当使用export来导出一个变量时，当前shell就会将此变量放到“导出变量列表”中，一旦在某个时刻需要建立子shell时，就会将这个导出变量列表拷贝一份给子shell，这样子shell也就有所应当的看到了这些被导出的变量了。
（为了能让10岁以前小朋友看懂，更形象的说：从前有个母亲叫shell，shell她手里攥着“导出变量列表”和“局部变量列表”两件宝贝，当shell生育了小孩（名叫子shell）的时候，她就把“导出变量列表”这件宝贝复制一份，交给她的孩子，而shell手里的另一件宝贝“局部变量列表”则不会交给小孩）
**引子：**
现在子shell如果想改变这份“导出变量列表”中的某些内容，那么父shell到底能不能察觉呢？父shell会不会随着子shell一起修改这些值呢？
**情景四：看看整个过程：**
[rocrocket@rocrocket SHELL]$ exportmyglobal=10[rocrocket@rocrocket SHELL]$ cat-n var4.sh
     1#!/bin/bash2myglobal=333echomyglobal=$myglobal[rocrocket@rocrocket SHELL]$ bash var4.sh
myglobal=33[rocrocket@rocrocket SHELL]$ echo$myglobal10
可见，在子shell是无法改变父shell中的“导出变量列表”中变量的值的。子shell只能修改自己手里的那份“导出变量列表”中变量的值。
**阶段总结：**
子shell是无法改变父shell中的“导出变量列表”中变量的值的。
变量一旦被导出，对所有后续执行的子shell，该变量都是导出变量。
**关于export -p：**
可以执行export -p来查看当前shell的导出变量列表。这其中一定是包括了从父shell继承来的导出变量列表。
**全文总结：**
1 没有导出的变量是局部变量，子shell是看不到的。
2 导出变量列表被复制给子shell，子shell可以修改和存取它，但是这种修改父shell看不到。
3 导出变量列表的上述特性对于直接产生的子shell生效，对于由子shell产生的后继子shell也是如此。
4 export可以在变量赋值之后用，也可以在变量赋值之前用。
