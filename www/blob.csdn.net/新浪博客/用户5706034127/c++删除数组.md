# c++删除数组_用户5706034127_新浪博客
||分类：[cPlusPlus](http://blog.sina.com.cn/s/articlelist_5706034127_1_1.html)|
转载：http://blog.csdn.net/shandaliuyan/article/details/5930719
<<c++ primer>>练习 
14.11 
中提到： 
Account 
*parray=new 
Account[100]; 
delete 
parray; 
delete 
[] 
parray; 
方括号的存在会使编译器获取数组大小（size）然后析构函数再被依次应用在每个元素上，一共size次。否则，只有一个元素被析构。 
无论哪种情况，分配的全部空间被返还给自由存储区。 
我的问题是:为什么无论哪种情况，分配的全部空间被返还给自由存储区? 
对于delete
parray,为什么不是删除单个Account元素，而是删除了100个. 
编译器怎么知道parray这个指针实际指向的是数组还是单个元素，即便知道指向的是自由存储区的数组，这个数组的大小又怎么知道。 
难道是编译器辅助行为？ 
总结:空间释放(肯定有个log记录分配的大小)和调用析构函数(类型识别,不同的编译器实现不同)采用不同的机制. 
(1)一般在分配时分配器会自动写一个日志（一般在分配使用得内存之前又一个结构），用于记录分配的大小，分配内容的sizeof等等。 
直观得想想,delete和delete[]都是传入一个void*如果不保存日志就无法知道分配时到底是分配了一个还是多个单元. 
所以虽然delete和delete[]不同但是分配器在执行释放过程中都会读取这个日志，从而了解到底应该释放多少内存，但是从程序员的角度来说，既然分配了数组，就应该用delete[] 
(2)在VC下用汇编跟过delete[]的执行情况,发现这个 "日志
"就是一个4字节长的整数记录数组元素个数,紧挨在数组第一个元素之前. 
但是有个前提:对象类型(或其基类)有显式析构函数.换句话说,析构函数是非trivial的. 
否则的话,数组前面是没有这个日志的.其实对于析构函数是trivial的情况,delete[]时无需调用其析构函数,因此此时VC把delete[]当做delete同样处理. 
(3)因为**释放数组空间和为数组调用析构函数是两个独立的部分**，可以使用不同的机制来实现。 
**释放空间的机制是需要绝对保证的。**因此，即使你不写delete[]，它也会将所有空间释放，其机制可以是前置的长度信息，也可以不是（如后置的特征分割符等等）。 
**而调用析构函数可以一般采用前置长度信息的方式**（当然也可以有其他方式）。在没有[]提示时，编译器在调用析构就将它当一个元素,而不会使用数组方式来调用每一个析构函数了。 
LS：“但是有个前提:对象类型(或其基类)有显式析构函数.换句话说,析构函数是非trivial的.
否则的话,数组前面是没有这个日志的.” 
——这说明，LS使用的编译器在释放数组空间时，并没有用前置的长度信息的方式。由此可见，释放数组空间和为数组调用析构函数确实可以使用不同的机制 
(4)delete
parray，编译器得到类型信息是Account单个的指针，那么释放时，只调用一次析构函数。 
delete[]
parray，编译器得到的类型信息是Account[]类型，则按照Account数组来处理，依次调用每个元素的析构函数。 
注意，以上是在编译期间就确定下来的，**编译器识别到类型信息的不同会决定调用析构函数的情况有不同。**
但是**对于内存释放，delete操作则不是通过类型信息来确定分配的内存大小，那么内存大小的信息从什么地方得到呢？**
当我们使用operator
new为一个自定义类型对象分配内存时，实际上我们得到的内存要比实际对象的内存大一些，这些内存除了要存储对象数据外，还需要记录这片内存的大小，此方法称为 
cookie。这一点上的实现依据不同的编译器不同。（例如 
MFC 
选择在所分配内存的头部存储对象实际数据，而后面的部分存储边界标志和内存大小信息。g++ 
则采用在所分配内存的头4个自己存储相关信息，而后面的内存存储对象实际数据。）当我们使用 
delete 
operator 
进行内存释放操作时，delete 
operator 
就可以根据这些信息正确的释放指针所指向的内存块。 
对于parray指针，可以根据这样的cookie信息来得到指向内存空间的大小，delete parray和delete[]
parray都是一样的，同样一个指针，cookie信息是相同的，所以对应的内存都会被释放掉。但是由于编译器理解两种情况下的类型是不同的，所以调用析构函数会有不同。 
(5)难道是编译器辅助行为？ 
没错，就是。不同的编译器可能采用的具体方法有可能不一样，但不管采用什么方法，编译器必须记住那块大小。 
(6)转自<<effective c++>> 
条款5：对应的new和delete要采用相同的形式 
下面的语句有什么错？ 
string *stringarray = new string[100]; 
delete stringarray; 
一切好象都井然有序——一个new对应着一个delete——然而却隐藏着很大的错误：程序的运行情况将是不可预测的。至少，stringarray指向的100个string对象中的99个不会被正确地摧毁，因为他们的析构函数永远不会被调用。 
用new的时候会发生两件事。首先，内存被分配(通过operator new
函数，详见条款7-10和条款m8)，然后，为被分配的内存调用一个或多个构造函数。用delete的时候，也有两件事发生：首先，为将被释放的内存调用一个或多个析构函数，然后，释放内存(通过operator
delete 函数，详见条款8和m8)。对于
delete来说会有这样一个重要的问题：内存中有多少个对象要被删除？答案决定了将有多少个析构函数会被调用。 
这个问题简单来说就是：要被删除的指针指向的是单个对象呢，还是对象数组？这只有你来告诉delete。如果你在用delete时没用括号，delete就会认为指向的是单个对象，否则，它就会认为指向的是一个数组： 
string *stringptr1 = new string; 
string *stringptr2 = new string[100]; 
... 
delete stringptr1;// 删除一个对象 
delete [] stringptr2;// 删除对象数组 
如果你在stringptr1前加了"[]"会怎样呢？答案是：那将是不可预测的； 
如果你没在stringptr2前没加上"[]"又会怎样呢？答案也是：不可预测。 
int这样的固定类型来说，结果也是不可预测的，即使这样的类型没有析构函数。所以，解决这类问题的规则很简单：如果你调用new时用了[]，调用delete时也要用[]。如果调用new时没有用[]，那调用delete时也不要用[]。 
Cpp代码 
- 
#include 
- 
- 
usingnamespacestd; 
- 
- 
structfoo 
- 
{ 
- ~foo(){}; //去掉后,就不会记录个数了.
- 
}; 
- 
- 
- 
intmain(intargc,char*argv[]) 
- 
{ 
- 
- foo* f = newfoo; 
- deletef; 
- f=0; 
- deletef; 
- 
- foo* fa = newfoo[8]; 
- printf("%u/n", *((char*)fa - 4)); //输出8:辅助析构函数.
- 
- deletefa; //有的编译器这里会有异常.
- //用delete而不是delete[]释放用new[]分配的空间这种行为是undefined的，
- //也就是由编译器实现所决定的
- fa = 0; 
- deletefa; 
- 
- return0; 
- 
}  
![c++删除数组](http://static.blog.csdn.net/images/save_snippets.png)
[](http://blog.csdn.net/shandaliuyan/article/details/5930719#)[](http://blog.csdn.net/shandaliuyan/article/details/5930719#)[](http://blog.csdn.net/shandaliuyan/article/details/5930719#)[](http://blog.csdn.net/shandaliuyan/article/details/5930719#)[](http://blog.csdn.net/shandaliuyan/article/details/5930719#)[](http://blog.csdn.net/shandaliuyan/article/details/5930719#)
