# C++ 精进篇（一）-数据类型 - fanyun的博客 - CSDN博客
2016年03月25日 13:48:15[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1664
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
       C++精进篇（一）之―数据类型：         
（1）类型修饰符signed和unsigned用于修饰字符型和整形。
（2）类型修饰符short和long用于修饰字符型和整形。
（3）当用signed和unsigned、short和long修饰int整形时，int可省略。当值不可能为负时，使用unsigned类型；
（4）其中bool和wchar_t是C++特有的。 当将非bool的值赋给bool，不为0则返回true，为0则返回false；当将boot的值赋给非bool，为true则返回1，为false则返回0；
（5）除上表以外，C/C++都可以自定义枚举enum、联合union和struct结构体类型。
（6）以上sizeof通过Windows XP 32位平台测试，其中某些类型数据的字节数和数值范围由操作系统和编译平台决定。比如16位机上，sizeof(int) = 2，而32位机上sizeof(int) = 4；32位机上sizeof(long) = 4，而64位机上sizeof(long) = 8。除此之外，注意64位机上的pointer占8byte。
（7）void的字面意思是“无类型”，不能用来定义变量。void真正发挥的作用在于：<1> 对函数返回和函数参数的限定，例如自定义既不带参数也无返回值的函数void MyFunc(void);<2>定义无类型通用指针void *，指向任何类型的数据。
（8）标准C++库及STL还提供了通用数据结构：字符串类string；向量类模板vector；双端队列类模板deque；链表类模板list；容器适配器堆栈类stack（实现先进后出的操作）；容器适配器队列类queue（实现先进先出的操作）；集合类set；多重集合类multiset；映射类map；多重映射类multimap；位集合bitset；迭代器iterator (类似指针的功能,对容器的内容进行访问)。
（9）在标准c++中，int的定义长度要依靠你的机器的字长，也就是说，如果你的机器是32位的，int的长度为32位，如果你的机器是64位的，那么int的标准长度就是64位，而vc中__int64是为在32机位机器长实现64位长度的整形数。
（10）关于32位平台下的int和long
long从字面上看，应该是64位才更合理，把long当成32位实在是一个历史的包袱。像C#那样新起炉灶的程序语言，由于没有需要支持老代码的问题，就把long当作64位来处理了。
在32位平台下，long是相对short而言，long（short）类型是long（short） int类型的简称，sizeof(long) = sizeof(int) = 4。int和long的范围虽然一样,但输入输出格式不同,printf int的格式为%d，而printf long的格式为%ld。
考虑到程序的可移植性，还是要将他们区分开来。但当要求的数值范围为4byte时，建议使用int类型，因为第一版的C语言只有一种类型，那就是int。不要使用char或bool做运算，仅仅使用它们存放字符或真值（char的符号根据架构的不同而不同）；
（11）在Win32 API及MFC中为了使类型名称在语意上更明了，对以上基本类型进行了大量的typedef。例如WINDEF.H中的BYTE,WORD,DWORD。
（12）将float赋给int，值会被截断，仅仅保留小数点之前的整数；如果将值赋给小于它位数的无符号类型，返回取模的结果，例如将258赋给unsighed char，则返回2；如果将值赋给小于它位数的有符号类型，返回结果不确定。不要混合signed和unsigned来运算。
（13）使用double做浮点数，一般来说，float的精度不够，且它们的运算量差别不大，甚至在有的架构上，double的运算速度还快于float，long double的开销很大，能不用就别用了。
（14）空指针请使用nullptr，不要使用NULL。
标识符的命名惯例：
 （1）名称最好能够指示它的实际意义；
 （2）变量名称一般小写；
（3）类名称一般首字母大写；
 （4）使用下划线或者大写来隔开字母。
