# C++多态在编译和运行期的差别 - youfangyuan - CSDN博客
2012年09月28日 14:50:18[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：887标签：[c++																[汇编																[编译器																[api																[语言																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/youfangyuan/article/category/891239)
多态是什么？简单来说，就是某段程序调用了一个API接口，但是这个API有许多种实现，根据上下文的不同，调用这段API的程序，会调用该API的不同实现。今天我们只关注继承关系下的多态。
还是得通过一个例子来看看C++是怎样在编译期和运行期来实现多态的。很简单，定义了一个Father类，它有一个testVFu[nc](http://www.linuxso.com/command/nc.html)虚函数哟。再定义了一个继承Father的Child类，它重新实现了testVFunc函数，当然，它也学习Father定义了普通的成员函数testFunc。大家猜猜程序的输出是什么？
> #include <ios[tr](http://www.linuxso.com/command/tr.html)eam>
using namespace std;
class Father
{
public:
	int m_fMember;
	vo[id](http://www.linuxso.com/command/id.html) testFunc(){
		cout<<"Father testFunc "<<m_fMember<<endl;
	}
	virtual void testVFunc(){
		cout<<"Father testVFunc "<<m_fMember<<endl;
	}
	Father(){m_fMember=1;}
};
class Child : public Father{
public:
	int m_cMember;
	Child(){m_cMember=2;}
	
	virtual void testVFunc(){cout<<"Child testVFunc "<<m_cMember<<":"<<m_fMember<<endl;}
	void testFunc(){cout<<"Child testFunc "<<m_cMember<<":"<<m_fMember<<endl;}
	void testNFunc(){cout<<"Child testNFunc "<<m_cMember<<":"<<m_fMember<<endl;}
};
int main()
{
	Father* pRealFather = new Father();
	Child* pFa[ls](http://www.linuxso.com/command/ls.html)eChild = (Child*)pRealFather;
	Father* pFalseFather = new Child();
	
	pFalseFather->testFunc();
	pFalseFather->testVFunc();
	pFalseChild->testFunc();
	pFalseChild->testVFunc();	
	pFalseChild->testNFunc();	
	return 0;
}
同样调用了testFunc和testVfunc，输出截然不同，这就是多态了。它的g++编译器输出结果是：
> Father testFunc 1
Child testVFunc 2:1
Child testFunc 0:1
Father testVFunc 1
Child testNFunc 0:1
看看main函数里调用的五个test*Func方法吧，这里有静态的多态，也有动态的多态。编译是静态的，运行是动态的。以下解释C++编译器是怎么形成上述结果的。
首先让我们用gcc -S来生成汇编代码，看看main函数里是怎么调用这五个test*Func方法的。
>         movl    $16, %edi
        call    _Znwm 
        movq    %rax, %rbx
        movq    %rbx, %rdi
        call    _ZN6FatherC1Ev
        movq    %rbx, -32(%rbp)
        movq    -32(%rbp), %rax
        movq    %rax, -24(%rbp)
        movl    $16, %edi
        call    _Znwm 
        movq    %rax, %rbx
        movq    %rbx, %rdi
        call    _ZN5ChildC1Ev
        movq    %rbx, -16(%rbp)
        movq    -16(%rbp), %rdi
        call    _ZN6Father8testFuncEv    本行对应pFalseFather->testFunc();
        movq    -16(%rbp), %rax
        movq    (%rax), %rax
        movq    (%rax), %rax
        movq    -16(%rbp), %rdi
        call    *%rax										本行对应pFalseFather->testVFunc();
        movq    -24(%rbp), %rdi
        call    _ZN5Child8testFuncEv		本行对应pFalseChild->testFunc();
        movq    -24(%rbp), %rax
        movq    (%rax), %rax
        movq    (%rax), %rax
        movq    -24(%rbp), %rdi
        call    *%rax										本行对应pFalseChild->testVFunc();	
        movq    -24(%rbp), %rdi
        call    _ZN5Child9testNFuncEv		本行对应pFalseChild->testNFunc();	
        movl    $0, %eax
        a[dd](http://www.linuxso.com/command/dd.html)q    $40, %rsp
        popq    %rbx
        leave
红色的代码，就是在依次调用上面5个test*Func。可以看到，第1、3次testFunc调用，其结果已经在编译出来的汇编语言中定死了，C++代码都是调用某个对象指针指向的testFunc()函数，输出结果却不同，第1次是：Father testFunc 1，第3次是：Child testFunc 0:1，原因何在？在编译出的汇编语言很明显，第一次调用的是_ZN6Father8testFuncEv代码段，第三次调用的是_ZN5Child8testFuncEv代码段，两个不同的代码段！编译完就已经决定出同一个API用哪种实现，这就是编译期的多态。
第2、4次testVFunc调用则不然，编译完以后也不知道以后究竟是调用Father还是Child的testVFunc实现，直到运行时，拿到CPU寄存器里的指针了，才知道这个指针究竟指向Father还是Child的testVFunc实现。这就是运行期的多态了。
现在我们看看，C++的对象模型是怎么实现这一点的，以及为什么最后打印的是如此结果。还以上面的代码做例子，生成的pFalseFather指向的对象是一个Child对象，它的内存布局是：
![](http://www.linuxso.com/uploads/allimg/111231/1622214611-0.gif)
再来看看调用代码：
> 	Father* pFalseFather = new Child();
	pFalseFather->testFunc();
	pFalseFather->testVFunc();
当我们调用pFaseFather->testFunc()代码时，这不是个virtual函数，所以，汇编代码里直接调用了Father::testFunc()实现，这是C++的规则。C++中，如果不是virtual字段的成员函数，调用它的程序将在编译时就直接调用到函数实现。所以，这行代码将执行以下C++代码：
> 	void testFunc(){
		cout<<"Father testFunc "<<m_fMember<<endl;
	}
注意到，pFaseFather指向的是个Child对象，所以Child对象在生成时同时执行了自己和Father父类的构造函数，所以，m_fMember被初始化为1，打印的结果就是Father testFunc 1。
而pFalseFather->testVFunc();调用了vptl指向的函数，上面说了，pFaseFather指向的是个Child对象，而Child对象实现了自己的testVFunc方法，在你new一个Child对象时，编译器会将vptl指向它自己的testVFunc的。所以，将会执行下面的C++代码：
> virtual void testVFunc(){cout<<"Child testVFunc "<<m_cMember<<":"<<m_fMember<<endl;}
m_cMemeber被Child的构造函数初始化为2，m_fMember被Father的构造函数初始化为1，所以打印出的结果是：Child testVFunc 2:1。
下面我们看看最后三个调用：
> 	pFalseChild->testFunc();
	pFalseChild->testVFunc();	
	pFalseChild->testNFunc();	
我们生成了一个pRealFather指向Father对象，它的内存空间是这样的：
![](http://www.linuxso.com/uploads/allimg/111231/162221E24-1.gif)
而后我们通过：
> Child* pFalseChild = (Child*)pRealFather;
指针pFalseChild是个Child类型，但它实际指向的是个Father对象。首先它调用testFunc函数，到底执行Father还是Child的实现呢？上面说过，非virtual函数一律编译期根据类型决定，所以，它调用的是Child实现：
> void testFunc(){cout<<"Child testFunc "<<m_cMember<<":"<<m_fMember<<endl;}
这里，m_fMember被Father的构造函数初始化为1，而m_cMember已经内存越界了！没错，在32位机器上，Father对象只有8个字节，而Child对象有12个字节，访问的m_cMember就是第9-12个字节转换成的int类型。通常情况，这段内存都是全0的，所以，m_cMember是0。看看结果：Child testFunc 0:1。
然后它调用testVFunc了，这次执行父类还是子类的？是父类的，因为这个对象是Father对象，在new出来的时候，Father的构造函数会把vptl指针指向自己的testVFunc实现哟。所以将会执行C++代码：
> 	virtual void testVFunc(){
		cout<<"Father testVFunc "<<m_fMember<<endl;
	}
执行结果自然是：Father testVFunc 1。
最后一个调用testNFunc，真实的Father对象对应的Father类中可没有这个函数，但是实际编译执行都没问题，why？同上理，在main函数中，因为指针pFalseChild是个Child类型，编译完的汇编语言在pFalseChild->testNFunc();这里就直接调用Child的testNFunc实现了，虽然m_cMember越界了，可是并不影响程序的执行哦。
