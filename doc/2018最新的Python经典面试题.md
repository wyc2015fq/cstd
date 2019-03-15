# 2018最新的Python经典面试题

本文福利：私信回复【pdf】可以获取Python电子一套

1：Python如何实现单例模式？

Python有两种体例可以实现单例模式，下面两个例子使用了不合的体例实现单例模式：

1.

![Python程序员面试必备的21条经典面试题-2.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133551&size=550x400&key=1d321bd6d9480864&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



\2. 

使用decorator来实现单例模式

![Python程序员面试必备的21条经典面试题-3.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133552&size=550x400&key=ead3971625d08537&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



2：什么是lambda函数？

Python允许你界说一种单行的小函数。

界说lambda函数的形式如下：labmda 参数：表达式lambda函数默认返回表达式的值。你也可以将其赋值给一个变量。

lambda函数可以接受任意个参数，包含可选参数，可是表达式只有一个：

![Python程序员面试必备的21条经典面试题-4.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133553&size=550x400&key=492ad5b2e5bc2e46&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



也能够直接使用lambda函数，不把它赋值给变量：

![Python程序员面试必备的21条经典面试题-5.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133554&size=550x400&key=f70fb3fa9eee121d&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



如果你的函数很是简单，只有一个表达式，不包含命令，可以考虑lambda函数。

不然，你还是界说函数才对，究竟结果函数没有这么多限制。

3：Python是如何进行类型转换的？

Python提供了将变量或值从一种类型转换成另一种类型的内置函数。

int函数能够将合适数学格式数字型字符串转换成整数。

不然，返回毛病信息。

![Python程序员面试必备的21条经典面试题-6.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133555&size=550x400&key=ca5310e4fe71dfa8&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



函数int也能够把浮点数转换成整数，但浮点数的小数部分被截去。

![Python程序员面试必备的21条经典面试题-7.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133556&size=550x400&key=17195fb07571895d&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



函数°oat将整数和字符串转换成浮点数：

![Python程序员面试必备的21条经典面试题-8.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133557&size=550x400&key=360395a6fc1d393f&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



函数str将数字转换成字符：

![Python程序员面试必备的21条经典面试题-9.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133558&size=550x400&key=3de28cd8777a281c&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



整数1和浮点数1.0在python中是不合的。

虽然它们的值相等的，但却属于不合的类型，这两个数在计算机的存储形式也是不一样。

4：Python如何界说一个函数

函数的界说形式如下：

![Python程序员面试必备的21条经典面试题-10.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133559&size=550x400&key=8e62578a7e153f1f&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



函数的名字也必须以字母开头，可以包含下划线" ",但不克不及把Python的关键字界说成函数的名字。

函数内的语句数量是任意的，每个语句至少有一个空格的缩进，以暗示此语句属于这个函数的。缩进结束的处所，函数自然结束。下面界说了一个两个数相加的函数：

![Python程序员面试必备的21条经典面试题-11.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133560&size=550x400&key=cee81303ca10efae&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



函数的目的是把一些复杂的操作隐藏，来简化程序的结构，使其容易阅读。

函数在挪用前，必须先界说，也可以在一个函数内部界说函数，内部函数只有在外部函数挪用时才能够被执行。

程序挪用函数时，转到函数内部执行函数内部的语句，函数执行完毕后，返回到它离开程序的处所，执行程序的下一条语句。

5：Python是如何进行内存管理的？

Python的内存管理是由Python得解释器负责的，开发人员可以从内存管理事务中解放出来，致力于应用程序的开发，这样就使得开发的程序毛病更少，程序更健壮，开发周期更短

6：如何反序的迭代一个序列？

how do I iterate over a sequence in reverse order

如果是一个list, 最快的解决方案是：

![Python程序员面试必备的21条经典面试题-12.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133561&size=550x400&key=bad2cf36f0913bbc&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



如果不是list, 最通用可是稍慢的解决方案是：

![Python程序员面试必备的21条经典面试题-13.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133562&size=550x400&key=21e3af66e11d3d6c&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



7：Python里面如何实现tuple和list的转换？

函数tuple(seq)可以把所有可迭代的(iterable)序列转换成一个tuple, 元素不变，排序也不变。

例如，tuple([1,2,3])返回(1,2,3), tuple('abc')返回('a'.'b','c').如果参数已经是一个tuple的话，函数不做任何拷贝而直接返回原来的对象，所以在不确定对象是不是tuple的时候来挪用tuple()函数也不是很耗费的。

函数list(seq)可以把所有的序列和可迭代的对象转换成一个list,元素不变，排序也不变。

例如 list([1,2,3])返回(1,2,3), list('abc')返回['a', 'b', 'c']。如果参数是一个list, 她会像set[:]一样做一个拷贝

8：Python面试题：请写出一段Python代码实现删除一个list里面的重复元素

可以先把list重新排序，然后从list的最后开始扫描，代码如下：

![Python程序员面试必备的21条经典面试题-14.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133563&size=550x400&key=1e77e5ded99e09c2&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



9：Python文件操作的面试题

\1. 如何用Python删除一个文件？

使用os.remove(filename)或者os.unlink(filename);

\2. Python如何copy一个文件？

shutil模块有一个copyfile函数可以实现文件拷贝

10：Python里面如何生成随机数？

标准库random实现了一个随机数生成器，实例代码如下：

![Python程序员面试必备的21条经典面试题-15.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133564&size=550x400&key=1ec4f80d848a35b3&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



它会返回一个随机的0和1之间的浮点数

11：如何用Python来发送邮件？

可以使用smtplib标准库。以下代码可以在支持SMTP监听器的办事器上执行。

![Python程序员面试必备的21条经典面试题-16.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133565&size=550x400&key=46faacde17502d60&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



12：Python里面如何拷贝一个对象？

一般来说可以使用copy.copy()体例或者copy.deepcopy()体例，几乎所有的对象都可以被拷贝

一些对象可以更容易的拷贝，Dictionaries有一个copy体例：

![Python程序员面试必备的21条经典面试题-17.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133566&size=550x400&key=c0484ea6cba0ab6c&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



13：有没有一个工具可以帮忙查找python的bug和进行静态的代码阐发？

有，PyChecker是一个python代码的静态阐发工具，它可以帮忙查找python代码的bug, 会对代码的复杂度和格式提出警告

Pylint是另外一个工具可以进行coding standard检查。

14：如何在一个function里面设置一个全局的变量？

解决体例是在function的开始插入一个global声明：

![Python程序员面试必备的21条经典面试题-18.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133567&size=550x400&key=1e5f2d4c2e062cec&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



15：有两个序列a,b，巨细都为n,序列元素的值任意整形数，无序；要求：通过交换a,b中的元素，使[序列a元素的和]与[序列b元素的和]之间的差最小。

将两序列合并为一个序列，并排序，为序列Source

拿出最大元素Big，次大的元素Small在余下的序列S[:-2]进行平分，获得序列max，min将Small加到max序列，将Big加大min序列，重新计算新序列和，和大的为max，小的为min。

Python代码

![Python程序员面试必备的21条经典面试题-19.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133568&size=550x400&key=b0c44c0060ff6b72&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



输出结果

![Python程序员面试必备的21条经典面试题-20.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133569&size=550x400&key=74eece6b73e54392&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



16：用Python匹配HTML tag的时候，<.>和<.?>有什么区别？

当重复匹配一个正则表达式时候， 例如<.*>, 当程序执行匹配的时候，会返回最大的匹配值

例如：

![Python程序员面试必备的21条经典面试题-21.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133570&size=550x400&key=b603f3d1dca3b821&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



会返回一个匹配而不是而

![Python程序员面试必备的21条经典面试题-22.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133571&size=550x400&key=9fa069c271f02c05&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



则会返回<.>这种匹配称作贪心匹配 <.?>称作非贪心匹配

17：Python里面search()和match()的区别？

match（）函数只检测RE是不是在string的开始位置匹配， search()会扫描整个string查找匹配, 也就是说match（）只有在0位置匹配成功的话才有返回，如果不是开始位置匹配成功的话，match()就返回none

例如：

> 
> print(re.match('super', 'superstition').span())会返回(0, 5)而print(re.match('super', 'insuperable'))则返回Nonesearch()会扫描整个字符串并返回第一个成功的匹配例如：print(re.search('super', 'superstition').span())返回(0, 5)print(re.search('super', 'insuperable').span())返回(2, 7)

18：如何用Python来进行查询和替换一个文本字符串？

可以使用sub()体例来进行查询和替换，sub体例的格式为：sub(replacement, string[, count=0])replacement是被替换成的文本string是需要被替换的文本count是一个可选参数，指最大被替换的数量例子：

![Python程序员面试必备的21条经典面试题-23.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133572&size=550x400&key=b99b3845182f39d8&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



输出：

![Python程序员面试必备的21条经典面试题-24.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133573&size=550x400&key=4c012f678f900f63&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



subn()体例执行的效果跟sub()一样，不过它会返回一个二维数组，包含替换后的新的字符串和总共替换的数量

例如：

![Python程序员面试必备的21条经典面试题-25.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133574&size=550x400&key=175646f3f5cae6bd&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



输出

![Python程序员面试必备的21条经典面试题-26.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133575&size=550x400&key=aea2295fe03c5409&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



19：介绍一下except的用法和作用？

Python的except用来捕获所有异常， 因为Python里面的每次毛病城市抛出 一个异常，所以每个程序的毛病都被看成一个运行时毛病。

以下是使用except的一个例子：

![Python程序员面试必备的21条经典面试题-27.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133576&size=550x400&key=5571656ae1d32dbe&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



因为这个毛病是由于open被拼写成opne而造成的，然后被except捕获，所以debug程序的时候很容易不知道出了什么问题。

下面这个例子更好点：

![Python程序员面试必备的21条经典面试题-28.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133577&size=550x400&key=a31a89f8b8e0ad2f&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



20：Python中pass语句的作用是什么？

pass语句什么也不做，一般作为占位符或者建立占位程序，pass语句不会执行任何操作，好比：

![Python程序员面试必备的21条经典面试题-29.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133578&size=550x400&key=b5270e4392623278&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



pass通经常使用来建立一个最简单的类：

![Python程序员面试必备的21条经典面试题-30.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133579&size=550x400&key=2ec41fb013a72c93&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



pass在软件设计阶段也经经常使用来作为TODO，提醒实现相应的实现，好比：

![Python程序员面试必备的21条经典面试题-31.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133580&size=550x400&key=fc25876978964010&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



21：介绍一下Python下range()函数的用法？

如果需要迭代一个数字序列的话，可以使用range()函数，range()函数可以生成等差级数。

如例：

![Python程序员面试必备的21条经典面试题-32.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133581&size=550x400&key=ba5584a2eb83c22a&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



这段代码将输出0, 1, 2, 3, 4五个数字range(10)会产生10个值， 也可以让range()从另外一个数字开始，或者界说一个不合的增量，甚至是负数增量range(5, 10)从5到9的五个数字range(0, 10, 3) 增量为三， 包含0,3,6,9四个数字range(-10, -100, -30) 增量为-30， 包含-10, -40, -70可以一起使用range()和len()来迭代一个索引序列例如：

![Python程序员面试必备的21条经典面试题-33.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133582&size=550x400&key=82348ac442de627c&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)



祝各位读者，都能找到自己心仪的工作！

<div class="pgc-img">

![Python程序员面试必备的21条经典面试题-34.jpg](http://www.vcchar.com/forum.php?mod=image&aid=133583&size=550x400&key=da1b1327651fd1c9&type=1) 
[登录/注册后可看大图](http://www.vcchar.com/member.php?mod=logging&action=login)