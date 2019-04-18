# Perl语言入门--3--perl的控制结构 - 默槑 - 博客园







# [Perl语言入门--3--perl的控制结构](https://www.cnblogs.com/modaidai/p/7363940.html)





表达式真假值总结: 表达式不一定是逻辑表达式，但一定要得出真假值

　　   假值：逻辑值为假

　　　　　　值为0

　　　　　　字符串为空

　　　　　　列表为空

　　　　　　undef

　　　其他情况为真



1、if {} elsif {} else {}

2、unless ：

　　与if条件含义相反。执行语句除非表达式为真。为真时退出，表达式是退出条件

3、while {表达式}{语句}

　　表达式为真时执行，是执行条件

　　认真设置条件为假的可能，否则无法退出

　　do{语句} while (表达式)

　　　　　　　　不管条件是什么，至少会执行一次

4、until {表达式}{语句}

　　与while正相反，执行语句，知道表达式为真停止，也有do语句

5、for(表达式1；表达式2；表达式3){语句}

6、foreach $w(列表或数组){语句}

　　for和foreach是通用的

7、循环控制

　　last :退出循环

　　next :执行下一循环。循环中next下的语句本次不执行。对于for要执行变量自增

　　redo: 重新执行本次循环，对于for不执行变量自增自增。在do语句中无效

　　goto: 跳转。不能跳转到循环或子程序内。现在大家都不用了

8、单行操作语句

　　print $_ if @a ==0

　　print $_ unless (@a ==0)

　　print $_ while( @a-- ==0)

　　print $_ until (@a --==0)　　

　　下面的与或非也算是单行条件语句

　　　　$a ==0 && print $a;      #这是and

　　　　open(F,'file.txt') || die "can't open ";       #这是or
















