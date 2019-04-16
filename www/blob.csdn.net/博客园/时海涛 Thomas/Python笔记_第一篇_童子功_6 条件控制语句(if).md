# Python笔记_第一篇_童子功_6.条件控制语句(if) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Python笔记_第一篇_童子功_6.条件控制语句(if)](https://www.cnblogs.com/noah0532/p/8429899.html)





　　Python正如其他语言一样存在两种常用的逻辑判断体（也叫结构化程序设计）。所谓逻辑判断体是通过你想要完成的编程思路，通过在逻辑判断体中的相互判断和作用得到你想要的结果。逻辑判断体也叫控制语句，Python分为两种：第一种叫：条件控制语句；第二种叫：循环控制语句。另外，还会对这两种判断体之间的组合进行举例，也就是所谓语句的嵌套。这里要特别说明的，基本上来说，**任何语言对于语句的执行过程都是从上而下，从左至右的这么一个过程，这个逻辑必须要有！这部分的内容是最有意思，并且是Python语言的重中之重，他们之间的组合千变万化可以让你的思想或者想要实现的东西变为现实！**



**条件控制语句简介**

　　所谓条件控制语句时通过一条或者多条语句执行结果（True或者False）来决定执行的代码块。

　　共分两类三种：if条件控制语句和其他条件控制语句（try语句，包括raise、assert）。后面这一种叫做异常处理和断言判断。所谓条件控制就是如果...是什么，就会...怎么样。

**　这里要特别说明的是，Python和其他语言最大的不一样就是每句结尾是没有分号的(;)，而且语句体内也没有{  }括号进行包含。这是因为Python是非常注重语句的对齐，如果语句对不齐的话就会造成语句无法执行。也就是说Python是用语句对齐的方式来替代花括号，去表达那些是结构体内信息，那一句是属于上面的条件判断的，这就是Python语言的特点。**

**　　很多教材吧try语句和raise放到后面来说，其实这应该算是条件控制语句里面的，所以应该提到前面来。**



**第一部分：if条件控制语句**

**1.   详细解释　**

　　存在形式形式如下：

**　　1. if**

**　　2. if-elif-elif...**

**　　3. if-elif-else**

**　　4. if-else**

　　通过这四种形式可以发现，if、elif、else这三个关键字组成了if条件控制语句的形式。很多教材这里分别讲解，有时候小白比较乱，这里通过一个结构解析非常快速的掌握这方面的内容。

　　表示一个最完整的if条件控制语句的结构：

**　　if 判断语句0：**

**　　　　执行语句0**

**　　elif 判断语句1：**

**　　　　执行语句1**

**　　elif 判断语句2：**

**　　　　执行语句2**

**　　elif 判断语句n：**

**　　　　执行语句n**

**　　else：**

**　　　　执行语句q**

　　解释如下：

　　1. **基本的逻辑**：所谓判断语句，就是通过设定的条件，得出仅且只有的两种结果**真（True）**或者**假（False）**，如果条件为**真（True）**就执行自己的执行语句；如果条件为假（False）就不执行自己的执行语句。（这句话很好理解，就是判断**真（True）**或者是这么回事儿，就执行下面的执行语句，反之亦然。注意哦，这里是**执行自己的执行语句**）。

　　2. **短路原则**：所谓的短路原则就是指所有的判断语句（包括判断语句0，判断语句1，判断语句2，判断语句n），只要有一条判断语句为真（True），就跳出整个条件判断结构体，执行完毕；当然如果全部为假的话，判断完毕后将没有任何结果，向下顺延执行if条件判断结构体之外的信息。

　　3. 有了**基本的逻辑**和**短路原则**，后面对着四种存在的形式分别进行解释：

**(1) 上面这个最完整的存在形式(也就是形式3. if - elif - else)**

　　　　流程如下：

***第一步：***

**　　if 判断语句0：# 如果这部分为真**

**　　　　执行语句0  # 将执行这个语句且跳出整个结构体 / 如果上面的判断语句为假将执行第二步**

***第二步：***

**　　elif 判断语句1：**

**　　　　执行语句1**

**　　elif 判断语句2：**

**　　　　执行语句2**

**　　elif 判断语句n：**

**　　　　执行语句n**

**　　# 如果其中一条elif语句为真将执行自己的执行语句且跳出整个结构体 / 如果没有一条为真将执行第三步**

***第三步：***

**　　else：**

**　　　　执行语句q**

**　　# 如果上面的全部为假（换句话说考虑的判断语句条件之外，可能还存在其他的情况话）将执行这条语句，执行完毕跳出整个条件判断结构体。**

**　　# 很简单吧！**



**(2) 从上面这个完整形式可以看出else这个关键字在这里起到了所有条件我都考虑了，但是还有可能其他条件我还没有考虑在内的情况，用一个else全部包含了，去执行语句。因此我们这里把第四种存在形式说一下(4. if - else)**

**　　　　流程如下：**

***第一步：***

**　　if 判断语句0：**# 如果这部分为真****

**　　　　执行语句0 **# 将执行这个语句且跳出整个结构体 / 如果上面的判断语句为假将执行第二步****

*****第二步：*****

**　　else：**

**　　　　执行语句q**

**　　# 如果if判断条件为假，其他可能存在的情况下，执行执行语句q这一句，然后跳出整个条件判断结构体。**



**　　　　(3) 另外，我们再看一下存在的第二种形式(2.if - elif - elif ...)，从这第二种形式来说elif可以有n条。另外有人要问，都是去判断为什么不都单独用if这个最简单存在形式1呢？这里是处于一种计算机效率的考虑，有机会可以做一个这样的实验：我判断10种条件，第一种做法是if , if, if,...写10个；第二种做法是，if,elif,elif,...写9个，执行效率和所耗费语句的执行时间第二种做法肯定是优于第一种做法，这是因为if - elif这种形式是在一个结构体当中取执行这些语句，如果单独一个一个的用if去判断，计算机会针对每一个if都会开辟一块儿内存空间去执行这些语句，结果显而易见，废话少说，把这个流程也解释一下。**

　　　　流程如下：

***第一步：***

**　　if 判断语句0：****# 如果这部分为真******

**　　　　执行语句0****# 将执行这个语句且跳出整个结构体 / 如果上面的判断语句为假将执行第二步******

***第二步：***

**　　elif 判断语句1：**

**　　　　执行语句1**

**　　elif 判断语句2：**

**　　　　执行语句2**

**　　elif 判断语句n：**

**　　　　执行语句n**

**# 程序运行到第二步，其中如果有一条elif的判断语句为真，将执行自己的执行语句，当然如果都为假将跳出整个条件判断结构体，执行下面的代码。**



**　　　　(4) 为了完整，把第一种存在形式也说一下(1. if )，这个显而易见非常的简单了。**

　　　　流程如下：

***第一步：***

**　　if 判断语句0：****# 如果这部分为真******

**　　　　执行语句0****# 将执行这个语句且跳出整个结构体 / 如果上面的判断语句为假将跳出整个条件判断结构体，执行下面的代码。******



**2.   总结**

　　if条件控制语句结构体是最简单而且最容易理解的结构体形式，但是要注意这么几点：

　　(1) 语句的缩进一定要对齐，如果不对齐的话，语句将出现错误，因为正如开始说python的时候，python是非常注重语句规整的。

　　(2) 每一个判断语句后面要加上 "："号，这个表示判断完毕后要说的话。



**3.   举例**

```
1 # (1) if形式
 2 if 10 < 20:
 3     print("你真聪明！")
 4 # 输出结果：你真聪明！
 5 
 6 # (2) if-elif-elif...形式
 7 num = 50
 8 if num < 30:
 9     print("num < 30,结果正确")
10 elif num < 20:
11     print("num < 20,结果正确")
12 elif num > 10:
13     print("num > 10,结果正确")  # 这条被执行
14 # 输出结果：num > 10,结果正确
15 
16 num = 50
17 if num < 30:
18     print("num < 30,结果正确")
19 elif num < 20:
20     print("num < 20,结果正确")
21 elif num < 10:
22     print("num > 10,结果正确")
23 # 输出结果：空 # 都没有结果，跳出条件判断结构体
24 
25 
26 # (3) if-elif-else形式
27 age = int(input("请输入年龄： "))
28 if age <= 0:
29     print("在娘胎里")
30 elif age > 0 and age <= 6:
31     print("婴儿")
32 elif age > 6 and age <=18:
33     print("青年")
34 else:
35     print("再议")
36 # 输出结果：
37 # 请输入年龄： 50
38 # 再议
39 
40 # (4) if-else形式
41 num = 30
42 if num < 10:  # 结果为假
43     print("你说对了")
44 else:   # 必定执行这条语句
45     print("你就是个大傻帽")
46 # 输出结果：你就是个大傻帽
```



**第二部分：其他条件控制语句（也叫异常处理和断言语句）**

 　　try语句在其他语言中也叫try...catch语句，起作用也是一样的。在python中try语句叫try...except/finally语句。

　　 python提供两个非常重要的功能来处理python程序在运行中出现的异常和错误，可以使用该功能来**调试python程序**。准确的来讲对于这个部分分两个篇章进行讲解。

**2.1 异常处理**

　　什么叫异常？

　　异常即是一个事件，该事件会在程序执行过程中发生，影响了程序的正常执行。

　　一般情况下，在python无法正常处理程序时就会发生一个异常。

　　异常是python的对象（python是把啥都看成对象，汗！），表示一个错误。

　　当python脚本发生异常时我们需要扑捉它，否则程序会终止执行。

　　常用的几个关键字：try、except、finally、else、raise，我们就用这几个关键字来组合这些条件来看看python怎么进行异常处理的（当然这几个关键字在一个条件体内可以叠加使用，这里分开进行论述）



**2.1.1   try...except ...else(完整形式)**

　　捕捉异常可以使用try...except语句来检测try语句块中的错误，从而让except语句铺货异常信息并处理。如果你不想在异常发生时结束你的程序，只需在try里捕获它。

**　　语法：**

**　　try：**

**　　<语句>　　# 运行别的代码**

**　　except <名字>：　　**

**　　<语句>　　# 如果在try部分引发了“名字”异常**

**　　except<名字> as <数据>：**

**　　<语句>　　# 如果发生"名字"异常，获得附加数据**

**　　else:**

**　　<语句>　　# 如果没有异常发生，执行这条语句**

　　整体来说，try至except这个部分好比一个容器，用于存放你要执行的程序，如果这部分程序正常执行，什么事儿也不会发生，如果有错误的话except就起到了作用。

　　解释：

　　1. 上面这个属于一个整体形式与前面的if语句一样，如果全部的异常处理检测通过将会执行else语句的内容，如果异常处理检测出问题将会执行except检测的内容，将会执行except下面的语句（一般都是print，还有保存到文件）。

　　2. 当然else语句也可以省略。

　　3. except的表达格式有三种：

　　　　(1) **except <python标准异常检测内容>： / **except (<python标准异常检测内容1, ****python标准异常检测内容2,****python标准异常检测内容n********>)****：

　　　　　 如果只检测一个异常处理，只写一个即可，如果后面要求检测多个异常检测内容，要用括号括起来；另外，后面有一个**附录python标准异常**内容都有哪些。

　　　　　　　　<语句>  # 这个语句时必须要有的

　　　　(2) **except**：啥都不加，自动检测

　　　　　　　　<语句>  # 这个语句时必须要有的

　　　　(3) **except <python标准异常检测内容> as 变量名**：     ===>>后面的其他条件控制语句会对as进行详细讲解

　　　　　　　　<语句>  # 这个语句时必须要有的



**举例说明**：

　　(1)  去掉else：只保留except信息进行检测，只检测一个异常内容：

```
1 # 除去else：只保留except信息进行检测
 2 try:
 3     s = input('输入一个1000以内的数字：')
 4     a = int(s)
 5     if a < 10:
 6         print("你输入了一个一位数！")
 7     elif a >= 10 and a < 100:
 8         print("你输入了一个两位数！")
 9     elif a >= 100 and a < 1000:
10         print("你输入了一个三位数！")
11     else:
12         print("输入一个1000以内的数字！")
13 except ValueError:    # 发生异常A时执行下面的语句
14     print("请输入数字！")
15 # 输出结果：
16 # 输入一个1000以内的数字：大中国
17 # 请输入数字！  # 直接执行错误信息后的东西
```



　　(2)  不去掉else：只保留except信息进行检测，只检测一个异常内容，我们输入一个正常的数字，执行完毕try中内容，**直接跳到else去执行**，然后输入一个非数字，执行except的内容：

```
1 try:
 2     s = input('输入一个1000以内的数字：')
 3     a = int(s)
 4     if a < 10:
 5         print("你输入了一个一位数！")
 6     elif a >= 10 and a < 100:
 7         print("你输入了一个两位数！")
 8     elif a >= 100 and a < 1000:
 9         print("你输入了一个三位数！")
10     else:
11         print("输入一个1000以内的数字！")
12 except ValueError:    # 发生异常A时执行下面的语句
13     print("请输入数字！")
14 else:
15     print("没有发生异常")
16 # 第一种输入一个正常结果，跳过except执行else后的内容，其输出结果如下：
17 # 输入一个1000以内的数字：50
18 # 你输入了一个两位数！
19 # 没有发生异常
20 
21 # 第二种输入一个不正常结果，执行except检测内容，不会执行else后的内容，其输出结果如下：
22 # 输入一个1000以内的数字：大中国
23 # 请输入数字！
```



　　(3)  输入多个except表达式后的内容，用括号括起来：

```
1 try:
 2     s = input('输入一个1000以内的数字：')
 3     a = int(s)
 4     if a < 10:
 5         print("你输入了一个一位数！")
 6     elif a >= 10 and a < 100:
 7         print("你输入了一个两位数！")
 8     elif a >= 100 and a < 1000:
 9         print("你输入了一个三位数！")
10     else:
11         print("输入一个1000以内的数字！")
12 except (Exception, TypeError, ValueError):  # 发生异常A和B时执行下面的语句
13     print("请输入数字！")
14 else:
15     print("没有发生异常")
16 # 输出结果：
17 # 输入一个1000以内的数字：大中国
18 # 请输入数字！
```



　　(4) except后啥都不加，自动检测

```
1 try:
 2     s = input('输入一个1000以内的数字：')
 3     a = int(s)
 4     if a < 10:
 5         print("你输入了一个一位数！")
 6     elif a >= 10 and a < 100:
 7         print("你输入了一个两位数！")
 8     elif a >= 100 and a < 1000:
 9         print("你输入了一个三位数！")
10     else:
11         print("输入一个1000以内的数字！")
12 except:   # 也可以什么都不加，自己识别错误。
13     print("请输入数字！")
14 else:
15     print("没有发生异常")
16 # 输出结果：
17 # 输入一个1000以内的数字：大中国
18 # 请输入数字！
```



　　(5) 把错误信息导出来的做法，添加as

```
1 try:
 2     s = input('输入一个1000以内的数字：')
 3     a = int(s)
 4     if a < 10:
 5         print("你输入了一个一位数！")
 6     elif a >= 10 and a < 100:
 7         print("你输入了一个两位数！")
 8     elif a >= 100 and a < 1000:
 9         print("你输入了一个三位数！")
10     else:
11         print("输入一个1000以内的数字！")
12 except (Exception, TypeError, ValueError) as Msg:  # 添加as语句把错误信息打印出来
13 # except Exception as Msg:  # 如果识别异常处理信息就一条可以这样添加
14     print(Msg)
15 else:
16     print("没有发生异常")
17 # 输出结果：
18 #** invalid literal for int() with base 10:** '大中国'（非法的字母对于10进制的整型数据而言)
```



**2.2.2   try...except...finally(完整形式，异常处理**语句的嵌套使用**)**

　　try...except...finally也是另外一种异常信息检测的语句，其实和try...except差不了多少，唯一不同的是：如果try内的程序发生了异常，python就会回来执行finally的句子，然后把异常教给上层try，控制流不会通过整个try语句。finally都是放到最后的。前面记得有一个else语句，就是说如果执行通过将执行else语句，**finally有一个特点：不管有没有异常，都执行这块儿代码**！**唯一不同就这点儿**。一般try...finally作为前套内使用比较多，后面2.2.3会将异常处理这类语句的嵌套。

**举例说明**：

　　(1) 通过添加finally和else来说明，如果执行异常检测处理else不会被执行，而finally不管有没有异常，都执行这块儿代码

```
1 try:
 2     s = input('输入一个1000以内的数字：')
 3     a = int(s)
 4     if a < 10:
 5         print("你输入了一个一位数！")
 6     elif a >= 10 and a < 100:
 7         print("你输入了一个两位数！")
 8     elif a >= 100 and a < 1000:
 9         print("你输入了一个三位数！")
10     else:
11         print("输入一个1000以内的数字！")
12 except Exception:  # Exception基本上都是万能的，所有异常都继承了Exception
13     print("请输入数字！")
14 else:
15     print("没有发生异常")    # 这里看到了没有，加入finally后，虽然else不再执行，但是finally后面的内容还是会执行。
16 finally:
17     print("finally都会最终执行的！")
18 # 输出结果：
19 # 输入一个1000以内的数字：大中国
20 # 请输入数字！
21 # finally都会最终执行的！
```



2.2.3 try...finally形式

    这种形式参照很容易解释，这里不再说明了。



**2.2.4  raise语句**

**　　raise语句叫做：自定义错误信息**

**　　格式：raise 异常处理种类/自定义异常处理种类 ("自定义错误输出信息")，语句执行到这个位置，如果检测出异常错误，直接显示内容，后面的内容将被跳过。**

**　　注意：(1) raise函数一般嵌入到try...except语句中用，如果检测出异常会直接打印内容。**

**　　　　　(2) raise函数如果单独使用的话，会提示哪个位置出错的红色信息(这个过程类似于debug处理过程）**

 　  这个函数可以直接嵌入到要识别错误信息的位置，直接打印出来。其实raise函数等于except查找错误+print打印错误信息的一个组合。

　　举例1：try...except中使用：

```
1 # 在try...except体中运用
 2 a = int(input("请输入一个整数:"))
 3 try:
 4     if a < 10:
 5        # print("!!!!")
 6         raise ValueError("您输入的值小于10")
 7         print("!!!!")   # 后面的信息不再打印
 8     elif a >= 10:
 9         print("您输入的数字大于等于10")
10 except Exception as e:
11         print(e)
# 第一：如果一个小于10的数：8
# 请输入一个整数:8
# 您输入的值小于10  # raise信息检测出异常将被执行

# 第二：如果输入一个大于10的数：20
# 请输入一个整数:20
# 您输入的数字大于等于10
```

　　举例2：单独使用raise：

```
1 import math
 2 plz = int(input("请输入一个整数进行开平方处理"))
 3 if plz < 0:
 4     raise ValueError("cannot compute sqrt of % d" % plz)
 5     # raise ValueError("cannot compute sqrt of")
 6 elif plz >= 0:
 7     print(math.sqrt(plz))
 8 # 第一：输入一个大于零的整数：8
 9 # 输出结果：
10 # 请输入一个整数进行开平方处理8
11 # 2.8284271247461903
12 
13 # 第二：输入一个小于0的整数：-6
14 # 输出结果：
15 # Traceback (most recent call last):
16 #   File "D:/PycharmProjects/untitled/demo12.py", line 25, in <module>
17 #     raise ValueError("cannot compute sqrt of % d" % plz)
18 # ValueError: cannot compute sqrt of -6
```

　　总结：

　　(1) raise语句看起来其实在try..except当中就是等于一个print作用。

　　(2) raise语句其实最重要的是自定义错误信息。比如我们定义一种程序运行到这里可能出现什么样的问题，就执行raise语句。

　　举例如下：

```
1 class Networkerror(RuntimeError):
2     def __init__(self, arg):
3         self.args = arg
4 
5 try:
6     raise Networkerror("Bad hostname")
7 except Networkerror as e:
8     print (e.args)
```

　　这个raise作为自定义的话，在函数、模块和类里面有应用，后面会详细说明。



**2.2.4   附录：python标准异常内容（其中常用的在后面进行备注）**

　　异常名称 　　　　　　　　   　　　　     描述  　　　　　　　　    　　　　　　　　 　　　　　　　　   　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　
**BaseException**	　　　　　　　　　　 所有异常的基类                                         
SystemExit	　　　　　　　　　　　　   解释器请求退出
KeyboardInterrupt	　　　　　　　　     用户中断执行(通常是输入^C)
**Exception	**　　　　　　　　　　　　  常规错误的基类（**这个基本上是万能的！哈哈）**
StopIteration	　　　　　　　　　　      迭代器没有更多的值
GeneratorExit	　　　　　　　　　　    生成器(generator)发生异常来通知退出
StandardError	　　　　　　　　　　   所有的内建标准异常的基类
ArithmeticError	　　　　　　　　　     所有数值计算错误的基类
FloatingPointError	　　　　　　　　   浮点计算错误
OverflowError	　　　　　　　　　　   数值运算超出最大限制
ZeroDivisionError　　　　　　            除(或取模)零 (所有数据类型)
AssertionError	　　　　　　　　　     断言语句失败
AttributeError	　　　　　　　　　　   对象没有这个属性
EOFError	　　　　　　　　　　　　  没有内建输入,到达EOF 标记
EnvironmentError	　　　　　　　　    操作系统错误的基类
IOError	　　　　　　　　　　　　      输入/输出操作失败
OSError	　　　　　　　　　　　　    操作系统错误
WindowsError	　　　　　　　　         系统调用失败
ImportError	　　　　　　　　　　      导入模块/对象失败
LookupError	　　　　　　　　　　    无效数据查询的基类
IndexError	　　　　　　　　　　　    序列中没有此索引(index)
**KeyError　**　　　　　　　                  映射中没有这个键
MemoryError	　　　　　　　　　　   内存溢出错误(对于Python 解释器不是致命的)
NameError	　　　　　　　　　　　　未声明/初始化对象 (没有属性)
UnboundLocalError	　　　　　　　　访问未初始化的本地变量
ReferenceError	　　　　　　　　　   弱引用(Weak reference)试图访问已经垃圾回收了的对象
RuntimeError	　　　　　　　　　　　一般的运行时错误
NotImplementedError	　　　　　　　  尚未实现的方法
SyntaxError	Python 　　　　　　　　 语法错误
IndentationError	　　　　　　　　　   缩进错误
TabError	Tab 　　　　　　　　　　　和空格混用
SystemError　　　　　　　　　　      一般的解释器系统错误
**TypeError**	　　　　　　　　　　　　对类型无效的操作
**ValueError	**　　　　　　　　　　　  传入无效的参数
UnicodeError	Unicode 　　　　　　　相关的错误
UnicodeDecodeError	Unicode 　　　  解码时的错误
UnicodeEncodeError	Unicode　　　　编码时错误
UnicodeTranslateError	Unicode 　　　转换时错误
Warning	　　　　　　　　　　　　　 警告的基类
DeprecationWarning	　　　　　　　   关于被弃用的特征的警告
FutureWarning	　　　　　　　　　　 关于构造将来语义会有改变的警告
OverflowWarning	　　　　　　　　　 旧的关于自动提升为长整型(long)的警告
PendingDeprecationWarning	　　　　关于特性将会被废弃的警告
RuntimeWarning	　　　　　　　　　  可疑的运行时行为(runtime behavior)的警告
SyntaxWarning	　　　　　　　　　　 可疑的语法的警告
UserWarning	　　　　　　　　　　　 用户代码生成的警告



**2.2   断言(assert)**

 　　使用assert断言是学习python一个非常好的习惯，python assert断言语句格式和用法很简单。在没有完善一个程序之前，我们不知道程序哪里会出错，与其让它在崩溃运行，不如在出现错误条件时就崩溃，这是后就需要assert断言的帮助。

　　python assert断言是声明其布尔值必须为真的判断，如果发生异常就说明表达式为假。可以理解assert断言语句为raise -- if -- not，用来测试表示式，其返回值为假，就会触发议程。

**表达式：assert expression [, message]**

　　注意：assert后面跟随的是表达式和将表达式判断未假的情况打印信息，后面可以省略；因此不能加括号，跟if...的表达逻辑类似。

```
1 # 形式1（不带meg）
 2 assert 1 + 1 == 2   # 返回结果为真
 3 assert isinstance('Hello', str)  # 返回结果为真
 4 assert isinstance('Hello', int)  # 返回结果为假
 5 
 6 # 形式2（带meg）
 7 s = "nothin is impossible."
 8 key = "nothing"
 9 assert key in s, "Key: '{}' is not in Target: '{}'".format(key, s)
10 # 输出结果：
11 # Traceback (most recent call last):
12 #   File "C:/Users/Administrator/Desktop/Python-1704/day04/try语句.py", line 196, in <module>
13 #     assert key in s, "Key: '{}' is not in Target: '{}'".format(key, s)
14 # AssertionError: Key: 'nothing' is not in Target: 'nothin is impossible.'
```

**注意事项**：

　　assert与print是不同的。print可以正常工作，但assert是错误的。这是因为print是python内建方法而assert不是。看如下的例子：

```
1 assert (2 + 2 ==5, "Oops somethin went wrong.")
 2 # 这种表达方式是错误的：
 3 # 输出结果：
 4 # C:/Users/Administrator/Desktop/Python-1704/day04/try语句.py:174: SyntaxWarning: assertion is always true, perhaps remove parentheses?
 5 #   assert (2 + 2 ==5, "Oops somethin went wrong.")
 6 
 7 # 所以要更改为表达式的方式：
 8 assert 2+2 == 5, "Oops somethin went wrong."  # 打印了输出结果：AssertionError: Oops somethin went wrong.
 9 # 输出结果：
10 # Traceback (most recent call last):
11 #   File "C:/Users/Administrator/Desktop/Python-1704/day04/try语句.py", line 181, in <module>
12 #     assert 2+2 == 5, "Oops somethin went wrong."
13 # AssertionError: Oops somethin went wrong.
```

　　其中第一条代码错误的原因是：根据assert的意义，程序会检测assert后面的表达式的值，也就是bol((2 +2 == 5, "Oops something went wrong."))。而python中的一个非空元组的布尔值是True。

　　再比如assert(False)。当()出现在assert后面时，python编译器会认为assert后面的输入的是一个元组，而一个非空元组的布尔值总是为True。因此assert()不能正常的工作。

　　另外，再详细应用在相关篇章会详细说明。














