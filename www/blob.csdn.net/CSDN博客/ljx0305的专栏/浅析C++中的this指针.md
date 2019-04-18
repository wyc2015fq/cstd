# 浅析C++中的this指针 - ljx0305的专栏 - CSDN博客
2008年11月09日 21:22:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：480标签：[c++																[编译器																[汇编																[null																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
                 有下面的一个简单的类： 
class CNullPointCall
{
public:
staticvoid Test1();
void Test2();
void Test3(int iTest);
void Test4();
private:
staticint m_iStatic;
int m_iTest;
};
int CNullPointCall::m_iStatic =0;
void CNullPointCall::Test1()
{
    cout << m_iStatic << endl;
}
void CNullPointCall::Test2()
{
    cout <<"Very Cool!"<< endl; 
}
void CNullPointCall::Test3(int iTest)
{
    cout << iTest << endl; 
}
void CNullPointCall::Test4()
{
    cout << m_iTest << endl; 
}
    那么下面的代码都正确吗？都会输出什么？
CNullPointCall *pNull = NULL; // 没错，就是给指针赋值为空
pNull->Test1(); // call 1
pNull->Test2(); // call 2
pNull->Test3(13); // call 3
pNull->Test4(); // call 4
    你肯定会很奇怪我为什么这么问。一个值为NULL的指针怎么可以用来调用类的成员函数呢？！可是实事却很让人吃惊：除了call 4那行代码以外，其余3个类成员函数的调用都是成功的，都能正确的输出结果，而且包含这3行代码的程序能非常好的运行。
    经过细心的比较就可以发现，call 4那行代码跟其他3行代码的本质区别：类CNullPointCall的成员函数中用到了this指针。
    对于类成员函数而言，并不是一个对象对应一个单独的成员函数体，而是此类的所有对象共用这个成员函数体。 当程序被编译之后，此成员函数地址即已确定。而成员函数之所以能把属于此类的各个对象的数据区别开, 就是靠这个this指针。函数体内所有对类数据成员的访问， 都会被转化为this->数据成员的方式。
    而一个对象的this指针并不是对象本身的一部分，不会影响sizeof（“对象”）的结果。this作用域是在类内部，当在类的非静态成员函数中访问类的非静态成员的时候，编译器会自动将对象本身的地址作为一个隐含参数传递给函数。也就是说，即使你没有写上this指针，编译器在编译的时候也是加上this的，它作为非静态成员函数的隐含形参，对各成员的访问均通过this进行。
    对于上面的例子来说，this的值也就是pNull的值。也就是说this的值为NULL。而Test1()是静态函数，编译器不会给它传递this指针，所以call 1那行代码可以正确调用（这里相当于CNullPointCall::Test1()）；对于Test2()和Test3()两个成员函数，虽然编译器会给这两个函数传递this指针，但是它们并没有通过this指针来访问类的成员变量，因此call 2和call 3两行代码可以正确调用；而对于成员函数Test4()要访问类的成员变量，因此要使用this指针，这个时候发现this指针的值为NULL，就会造成程序的崩溃。    
    其实，我们可以想象编译器把Test4()转换成如下的形式：
void CNullPointCall::Test4(CNullPointCall *this)
{
    cout <<this->m_iTest << endl; 
}
    而把call 4那行代码转换成了下面的形式：
CNullPointCall::Test4(pNull);
    所以会在通过this指针访问m_iTest的时候造成程序的崩溃。
    下面通过查看上面代码用VC 2005编译后的汇编代码来详细解释一下神奇的this指针。
    上面的C++代码编译生成的汇编代码是下面的形式：
    CNullPointCall *pNull = NULL;
0041171E  mov         dword ptr [pNull],0
    pNull->Test1();
00411725  call        CNullPointCall::Test1 (411069h) 
    pNull->Test2();
0041172A  mov         ecx,dword ptr [pNull] 
0041172D  call        CNullPointCall::Test2 (4111E0h) 
    pNull->Test3(13);
00411732  push        0Dh  
00411734  mov         ecx,dword ptr [pNull] 
00411737  call        CNullPointCall::Test3 (41105Ah) 
    pNull->Test4();
0041173C  mov         ecx,dword ptr [pNull] 
0041173F  call        CNullPointCall::Test4 (411032h) 
    通过比较静态函数Test1()和其他3个非静态函数调用所生成的的汇编代码可以看出：非静态函数调用之前都会把指向对象的指针pNull（也就是this指针）放到ecx寄存器中（mov ecx,dword ptr [pNull]）。这就是this指针的特殊之处。看call 3那行C++代码的汇编代码就可以看到this指针跟一般的函数参数的区别：一般的函数参数是直接压入栈中（push 0Dh），而this指针却被放到了ecx寄存器中。在类的非成员函数中如果要用到类的成员变量，就可以通过访问ecx寄存器来得到指向对象的this指针，然后再通过this指针加上成员变量的偏移量来找到相应的成员变量。
    下面再通过另外一个例子来说明this指针是怎样被传递到成员函数中和如何使用this来访问成员变量的。
    依然是一个很简单的类：
class CTest
{
public:
void SetValue();
private:
int m_iValue1;
int m_iValue2;
};
void CTest::SetValue()
{
    m_iValue1 =13;
    m_iValue2 =13;
}
    用如下的代码调用成员函数：
CTest test;
test.SetValue();
    上面的C++代码的汇编代码为：
    CTest test;
    test.SetValue();
004117DC  lea         ecx,[test] 
004117DF  call        CTest::SetValue (4111CCh) 
    同样的，首先把指向对象的指针放到ecx寄存器中；然后调用类CTest的成员函数SetValue()。地址4111CCh那里存放的其实就是一个转跳指令，转跳到成员函数SetValue()内部。
004111CC  jmp         CTest::SetValue (411750h)
    而411750h才是类CTest的成员函数SetValue()的地址。
void CTest::SetValue()
{
00411750  push        ebp  
00411751  mov         ebp,esp 
00411753  sub         esp,0CCh 
00411759  push        ebx  
0041175A  push        esi  
0041175B  push        edi  
0041175C  push        ecx // 1   
0041175D  lea         edi,[ebp-0CCh] 
00411763  mov         ecx,33h 
00411768  mov         eax,0CCCCCCCCh 
0041176D  rep stos    dword ptr es:[edi] 
0041176F  pop         ecx // 2 
00411770  mov         dword ptr [ebp-8],ecx // 3
    m_iValue1 =13;
00411773  mov         eax,dword ptr [this] // 4
00411776  mov         dword ptr [eax],0Dh // 5
    m_iValue2 =13;
0041177C  mov         eax,dword ptr [this] // 6
0041177F  mov         dword ptr [eax+4],0Dh // 7
}
00411786  pop         edi  
00411787  pop         esi  
00411788  pop         ebx  
00411789  mov         esp,ebp 
0041178B  pop         ebp  
0041178C  ret 
    下面对上面的汇编代码中的重点行进行分析：
    1、将ecx寄存器中的值压栈，也就是把this指针压栈。
    2、ecx寄存器出栈，也就是this指针出栈。
    3、将ecx的值放到指定的地方，也就是this指针放到[ebp-8]内。
    4、取this指针的值放入eax寄存器内。此时，this指针指向test对象，test对象只有两个int型的成员变量，在test对象内存中连续存放，也就是说this指针目前指向m_iValue1。
    5、给寄存器eax指向的地址赋值0Dh（十六进制的13）。其实就是给成员变量m_iValue1赋值13。
    6、同4。
    7、给寄存器eax指向的地址加4的地址赋值。在4中已经说明，eax寄存器内存放的是this指针，而this指针指向连续存放的int型的成员变量m_iValue1。this指针加4（sizeof(int)）也就是成员变量m_iValue2的地址。因此这一行就是给成员变量m_iValue2赋值。
    通过上面的分析，我们可以从底层了解了C++中this指针的实现方法。虽然不同的编译器会使用不同的处理方法，但是C++编译器必须遵守C++标准，因此对于this指针的实现应该都是差不多的。
引用：[http://blog.csdn.net/starlee/archive/2008/01/24/2062586.aspx](http://blog.csdn.net/starlee/archive/2008/01/24/2062586.aspx)
