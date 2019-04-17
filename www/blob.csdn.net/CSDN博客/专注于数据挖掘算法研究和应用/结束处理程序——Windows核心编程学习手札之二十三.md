# 结束处理程序——Windows核心编程学习手札之二十三 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月13日 17:10:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2261








# 结束处理程序

## ——Windows核心编程学习手札之二十三

使用SEH可以只关注程序要完成任务，而运行中发生的错误，系统将会发现并通知。Windows引入SHE是为了便于操作系统的开发，使用SHE所造成的负担主要由编译程序来承担，而不是由操作系统承担。当异常块（exception block）出现时，编译程序要生成特殊的代码，产生一些表（table）来支持处理SHE的数据结构，提供回调（callback）函数，操作系统可以调用这些函数，保证异常块被处理。编译程序还负责准备栈结构和其他内部信息，供操作系统使用和参考。在编译程序中增加SHE支持，不同的编译商会以不同的方式实现SEH。SHE实际包含两个主要功能：结束处理（termination handling）和异常处理（exception handling）。

一个结束处理程序能够确保去调用和执行一个代码块（结束处理程序，termination handling），而不管另外一段代码（保护体，guarded body）是如何退出，结束处理程序的文法结构如下：

__try{

//guarded body

。。。

}

__finally{

//termination handler

。。。

}

__try和__finally关键字用来标出处理程序两段代码的轮廓，操作系统和编译程序共同确保结束处理程序中的__finally代码块能够被执行，不管保护体（try块）是如何退出的，不论保护体中使用return，还是goto，或者是longjump，结束处理程序（finally块）都将被调用。

尽管结束处理程序可以捕捉try块过早退出的大多数情况，但当线程或进程被结束时，却不能引起finally块中的代码执行。当调用ExitThread或ExitProcess时，将立即结束线程或进程，而不会执行finally块中的任何代码，另外，由于某个程序调用terminateThread或terminateProcess，线程或进程将死掉，finally块中的代码也不执行。某些C运行期函数（如abort）要调用ExitProcess，也使finally块中的代码不能执行，虽然没有办法阻止其他程序结束线程或进程，但可以避免过早调用ExitThread和ExitProcess，以执行finally块中代码。最好将return、continue、break、goto等语句从结束处理程序的try块和finally块中移出去，放在结束处理程序的外面，这样使编译程序产生较小的代码，因为不需要再捕捉try块中的过早退出，也使编译程序产生更快的代码（因为执行局部展开的指令也不少），另外代码更容易阅读和维护。

为避免在try块中使用return语句，microsoft在C/C++编译程序中增加了__leave关键字。在try块中使用__leave关键字会引起跳转到try块的结尾，即跳转到try块的右大括号，由于控制流自然地从try块中退出并进入finally块，不产生系统开销，不过需要增加BOOL型变量来指示函数的成功或失败，当然这个代价是很小的。按照这个方式利用结束处理程序来设计函数时，需要在进入try块之前，将所有资源句柄初始化为无效的值，然后在finally块中查看那些资源被成功的分配，就可以知道那些要释放。另外一种确定需要释放资源的办法是对成功分配的资源设置一个标志，然后在finally块中的代码检查标志的状态，来确定资源是否被释放。

区分强制执行finally块的情况：

1）从try块中进入finally块的正常控制流；

2）局部展开：从try块中过早退出（goto、longjump、contiune、break、return等）强制控制转移到finally块，由于系统开销比较大，尽量避免在try块中过早退出，可使用__leave代替return；

3）全局展开（global unwind），发生时没有明显标识，如引起一个内存访问违规（memory access violation），一个全局展就会在finally块执行。

为了确定是那种情况引起finally块执行，可调用内部函数（或内蕴函数，intrinsic function）Abnormal Termination：

BOOL AbnormalTermination（）；

这个内部函数只在finally块中调用，返回一个Boolean值，指出与finally块相结合的try块是否过早退出，如果控制流离开try块并自然进入finally块，则返回FALSE；如果控制流非正常退出try块（由于goto/return/break/contiune语句引起的局部展开），或由于内部访问违规或其他异常引起的全局展开，将返回TRUE，这里没发区别finally块的执行是由于局部展开还是全局展开。

内部函数是编译程序识别的一种特殊函数，编译程序为内部函数产生内联（inline）代码而不是生成调用函数的代码，例如，memcpy是一个内部函数（如果指定/Oi编译程序开关）当编译程序看到一个对memcpy的调用，直接将memcpy的代码插入调用memcpy的函数中，而不是生成一个对memcpy函数的调用，其作用是代码的长度增加了，但执行速度加快了。

利用结束处理程序的理由：

1）简单错误处理，所有清理工作在一个位置并且保证被执行；

2）提高程序的可读性；

3）使代码更加容易维护；

4）如果使用得当，具有最小的系统开销。





