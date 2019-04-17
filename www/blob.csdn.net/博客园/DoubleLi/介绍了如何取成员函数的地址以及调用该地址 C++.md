# 介绍了如何取成员函数的地址以及调用该地址:C++ - DoubleLi - 博客园






摘要：介绍了如何取成员函数的地址以及调用该地址.

关键字：C++成员函数 this指针 调用约定

一、成员函数指针的用法

　　在C++中，成员函数的指针是个比较特殊的东西。对普通的函数指针来说，可以视为一个地址,在需要的时候可以任意转换并直接调用。但对成员函数来说，常规类型转换是通不过编译的，调用的时候也必须采用特殊的语法。C++专门为成员指针准备了三个运算符: "::*"用于指针的声明，而"->*"和".*"用来调用指针指向的函数。比如:
   class tt   {       public: void foo(int x){ printf("\n %d \n",x); }   };   typedef   void  ( tt::* FUNCTYPE)(int );    FUNCTYPE ptr = tt::foo;  //给一个成员函数指针赋值.    tt a;    (a.*ptr)(5);   //调用成员函数指针.    tt *b = new tt;    (b->*ptr)(6);  //调用成员函数指针.

　　注意调用函数指针时括号的用法，因为 .* 和 ->* 的优先级比较低，必须把它们和两边要结合的元素放到一个括号里面，否则通不过编译。不仅如此，更重要的是，无法为成员函数指针进行任何的类型转换，比如你想将一个成员函数的地址保存到一个整数中（就是取类成员函数的地址），按照一般的类型转换方法是办不到的.下面的代码:
    DWORD dwFooAddrPtr= 0;    dwFooAddrPtr = (DWORD) &tt::foo;  /* Error C2440 */    dwFooAddrPtr = reinterpret_cast (&tt::foo); /* Error C2440 */
　　你得到只是两个c2440错误而已。当然你也无法将成员函数类型转换为其它任何稍有不同的类型，简单的说，每个成员函数指针都是一个独有的类型，无法转换到任何其它类型。即使两个类的定义完全相同也不能在其对应成员函数指针之间做转换。这有点类似于结构体的类型，每个结构体都是唯一的类型，但不同的是，结构体指针的类型是可以强制转换的。有了这些特殊的用法和严格的限制之后，类成员函数的指针实际上是变得没什么用了。这就是我们平常基本看不到代码里有"::*", ".*" 和 "->*"的原因。

二、取成员函数的地址

　　当然，引用某位大师的话:"在windows中，我们总是有办法的"。同样，在C++中，我们也总是有办法的。这个问题，解决办法已经存在了多年，并且广为使用（在MFC中就使用了）。一般有两个方法，一是使用内嵌的汇编语言直接取函数地址，二是使用union类型来逃避C++的类型转换检测。两种方法都是利用了某种机制逃避C++的类型转换检测，为什么C++编译器干脆不直接放开这个限制，一切让程序员自己作主呢？当然是有原因的，因为类成员函数和普通函数还是有区别的，允许转换后，很容易出错，这个在后面会有详细的说明。现在先看看取类成员函数地址的两种方法：

第一种方法:
template void GetMemberFuncAddr_VC6(ToType& addr,FromType f){    union     {      FromType _f;      ToType   _t;    }ut;    ut._f = f;    addr = ut._t;}

这样使用:

DWORD dwAddrPtr;

GetMemberFuncAddr_VC6(dwAddrPtr,&tt::foo);

　　为什么使用模版? 呵呵，如果不使用模版，第二个参数该怎么些，写成函数指针且不说太繁琐，关键是没有通用性，每个成员函数都要单独写一个转换函数。

第二种方法:
#define GetMemberFuncAddr_VC8(FuncAddr,FuncType)\{                                               \    __asm                                       \    {                                           \        mov eax,offset FuncType                 \    };                                          \    __asm                                       \    {                                           \        mov FuncAddr, eax                       \    };                                          \}
这样使用:

DWORD dwAddrPtr;

GetMemberFuncAddr_VC8(dwAddrPtr,&tt::foo);

　　本来是想写成一个模版函数的，可惜虽然通过了编译，却不能正确运行。估计在汇编代码中使用模版参数不太管用，用offset取偏移量直接就得0。
　　上面的宏是可以正确运行的，并且还有一个额外的好处，就是可以直接取私有成员函数的地址（大概在asm括号中,编译器不再检查代码的可访问性）。不过缺点是它在vc6下是无法通过编译的，只能在VC8下使用。

三、调用成员函数地址

　　通过上面两个方法，我们可以取到成员函数的地址。不过，如果不能通过地址来调用成员函数的话，那也还是没有任何用处。当然，这是可行的。不过在这之前，需要了解关于成员函数的一些知识。
　　我们知道，成员函数和普通函数最大的区别就是成员函数包含一个隐藏的参数this指针，用来表明成员函数当前作用在那一个对象实例上。根据调用约定（Calling Convention）的不同，成员函数实现this指针的方式也不同。如果使用__thiscall调用约定，那么this指针保存在寄存器ECX中，VC编译器缺省情况下就是这样的。如果是__stdcall或__cdecl调用约定，this指针将通过栈进行传递，且this指针是最后一个被压入栈的参数，相当于编译器在函数的参数列表中最左边增加了一个this参数。
　　这里还有件事不得不提，虽然vc将__thiscall类型作为成员函数的默认类型，但是vc6却没有定义__thiscall关键字!如果你使用__thiscall来定义一个函数，编译器报错：'__thiscall' keyword reserved for future use。

知道这些就好办了，我们只要根据不同的调用约定，准备好this指针，然后象普通函数指针一样的使用成员函数地址就可以了。

　　对__thiscall类型的成员函数(注意，这个是VC的默认类型)，我们在调用之前加一句: mov ecx, this; 然后就可以调用成员函数指针。例如:
class tt { public:    void foo(int x,char c,char *s)//没有指定类型,默认是__thiscall.    {        printf("\n m_a=%d, %d,%c,%s\n",m_a,x,c,s);    }    int m_a;};typedef  void (__stdcall *FUNCTYPE)(int x,char c,char *s);//定义对应的非成员函数指针类型,注意指定__stdcall.    tt abc;    abc.m_a = 123;    DWORD ptr;    DWORD This = (DWORD)&abc;    GetMemberFuncAddr_VC6(ptr,tt::foo); //取成员函数地址.    FUNCTYPE fnFooPtr  = (FUNCTYPE) ptr;//将函数地址转化为普通函数的指针.     __asm //准备this指针.    {        mov ecx, This;    }    fnFooPtr(5,'a',"7xyz"); //象普通函数一样调用成员函数的地址.

　　对其它类型的成员函数,我们只要申明一个与原成员函数定义完全类似的普通函数指针,但在参数中最左边加一个void * 参数。代码如下:
class tt {public:    void __stdcall foo(int x,char c,char *s)//成员函数指定了__stdcall调用约定.    {        printf("\n m_a=%d, %d,%c,%s\n",m_a,x,c,s);    }    int m_a;};typedef  void (__stdcall *FUNCTYPE)(void *This,int x,char c,char *s);//注意多了一个void *参数.    tt abc;    abc.m_a = 123;    DWORD ptr;    GetMemberFuncAddr_VC6(ptr,tt::foo); //取成员函数地址.    FUNCTYPE fnFooPtr = (FUNCTYPE) ptr;//将函数地址转化为普通函数的指针.     fnFooPtr(&abc,5,'a',"7xyz"); //象普通函数一样调用成员函数的地址,注意第一个参数是this指针.
　　每次都定义一个函数类型并且进行一次强制转化，这个事是比较烦的，能不能将这些操作写成一个函数，然后每次调用是指定函数地址和参数就可以了呢？当然是可以的，并且我已经写了一个这样的函数。
//调用类成员函数//callflag:成员函数调用约定类型,0--thiscall,非0--其它类型.//funcaddr:成员函数地址.//This:类对象的地址.//count:成员函数参数个数.//...:成员函数的参数列表.DWORD CallMemberFunc(int callflag,DWORD funcaddr,void *This,int count,...){      DWORD re;      if(count>0)//有参数,将参数压入栈.      {           __asm           {                 mov  ecx,count;//参数个数,ecx,循环计数器.                 mov  edx,ecx;                 shl  edx,2;                     add  edx,0x14;  edx = count*4+0x14;        next:    push  dword ptr[ebp+edx];                 sub   edx,0x4;                 dec   ecx                   jnz   next;           }      }      //处理this指针.      if(callflag==0) //__thiscall,vc默认的成员函数调用类型.      {           __asm mov ecx,This;      }      else//__stdcall      {           __asm push This;      }      __asm//调用函数      {           call funcaddr;           mov  re,eax;      }      return re;}
使用这个函数，则上面的两个调用可以这样写：

CallMemberFunc(0,ptr1,&abc,3,5,'a',"7xyz");//第一个参数0,表示采用__thiscall调用.

CallMemberFunc(1,ptr2,&abc,3,5,'a',"7xyz");//第一个参数1,表示采用非__thiscall调用. 

　　需要说明的是，CallMemberFunc是有很多限制的，它并不能对所有的情况都产生正确的调用序列。原因之一是它假定每个参数都使用了4个字节的栈空间。这在大多数情况下是正确的，比如参数为指针,char,short,int,long以及对应的无符号类型，这些参数确实都是每一个参数使用了4字节的栈空间。但是还有很多情况下，参数不使用4字栈空间，比如double,自定义的结构或类.float虽然是占了4字节，但编译器还产生了一些浮点指令,而这些无法在CallMemberFunc被模拟出来,因此对float参数也是不行的。
　　总结一下，如果成员函数的参数都是整型兼容类型，则可以使用CallMemberFunc调用函数地址。如果不是，那就只有按前面的方法，先定义对应的普通函数类型，强制转化，准备this指针，然后调用普通函数指针。

四、进一步的讨论

　　到目前为止，已经讨论了如何取成员函数的地址，然后如何使用这个地址。但是还有些重要的情况没有讨论，我们知道成员函数可分为三种：普通成员函数，静态，虚拟。另外更重要的是，在继承甚至多继承下情况如何。

首先看看最简单的单继承，非虚拟函数的情况。

class tt1{public:      void foo1(){ printf("\n hi, i am in tt1::foo1\n"); }};class tt2 : public tt1{public:      void foo2(){ printf("\n hi, i am in tt2::foo2\n"); }};
注意，tt2中没有定义函数foo1，它的foo1函数是从tt1中继承过来的。这种情况下，我们直接取tt2::foo1的地址行会发生什么?
DWORD tt2_foo1;tt1 x;GetMemberFuncAddr_VC6(tt2_foo1,&tt2::foo1);CallMemberFunc(0,tt2_foo1,&x,0); // tt2::foo1 = tt1::foo1
　　运行结果表明，一切正常！当我们写下tt2::foo1的时候，编译器知道那实际上是tt1::foo1，因此它会暗中作替换。编译器(VC6)产生的代码如下:
GetMemberFuncAddr_VC6(tt2_foo1,&tt2::foo1); //源代码.//VC6编译器产生的汇编代码:push offset @ILT+235(tt1::foo1) (004010f0) //直接用tt1::foo1 替换 tt2::foo1....
再看看稍微复杂些的情况，继承情况下的虚拟函数。
class tt1{public:      void foo1(){ printf("\n hi, i am in tt1::foo1\n"); }      virtual void foo3(){ printf("\n hi, i am in tt1::foo3\n"); }};class tt2 : public tt1{public:      void foo2(){ printf("\n hi, i am in tt2::foo2\n"); }      virtual void foo3(){ printf("\n hi, i am in tt2::foo3\n"); }};
现在tt1和tt2都定义了虚函数foo3，按C++语法，如果通过指针调用foo3，应该发生多态行为。下面的代码：
DWORD tt1_foo3,tt2_foo3;GetMemberFuncAddr_VC6(tt1_foo3,&tt1::foo3);GetMemberFuncAddr_VC6(tt2_foo3,&tt2::foo3);tt1 x;tt2 y;CallMemberFunc(0,tt1_foo3,&x,0); // tt1::foo3CallMemberFunc(0,tt2_foo3,&x,0); // tt2::foo3CallMemberFunc(0,tt1_foo3,&y,0); // tt1::foo3CallMemberFunc(0,tt2_foo3,&y,0); // tt2::foo3
输出如下:
hi, i am in tt1::foo3hi, i am in tt1::foo3hi, i am in tt2::foo3hi, i am in tt2::foo3
　　请注意第二行输出，tt2_foo3取的是&tt2::foo3，但由于传递的this指针产生是&x，所以实际上调用了tt1::foo3。同样，第三行输出，取的是基类的函数地址，但由于实际对象是派生类，最后调用了派生类的函数。这说明取得的成员函数地址在虚拟函数的情况下仍然保持了正确的行为。
　　你若真的理解了上面所说的，一定会觉得奇怪。取函数地址的时候就得到了一个整数(成员函数地址)，为何调用的时候却进了不同的函数? 只要看看汇编代码就都清楚了，"源码之前，了无秘密"。源代码: GetMemberFuncAddr_VC6(tt1_foo3,&tt1::foo3); 产生的汇编代码如下:
push offset @ILT+90(`vcall') (0040105f)...
　　原来取tt1::foo3地址的时候，并不是真的就将tt1::foo3的地址传给了函数，而是传了一个vcall函数的地址。顾名思义，vcall当然是虚拟调用的意思。我们找到地址0040105f，看看这个函数到底干了些什么。
@ILT+90(??_9@$BA@AE):0040105F jmp `vcall' (00401380)
该地址只是ILT的一个项,直接跳转到真正的vcall函数,00401380。找到00401380，就可以看到vcall的代码。
`vcall':00401380 mov eax,dword ptr [ecx] ;//将this指针视为dword类型,并将指向的内容(对象的首个dword)放入eax.00401382 jmp dword ptr [eax] ;//跳转到eax所指向的地址.
　　代码执行的时候,ecx就是this指针,具体说就是上面对象x或y的地址。而eax就是对象x或y的第一个dword的值。我们知道，对于有虚拟函数的类对象，其对象的首地址处总是一个指针，该指针指向一个虚函数的地址表。上面的对象由于只有一个虚函数，所以虚函数表也只有一项。因此，直接跳转到eax指向的地址就好。如果有多个虚函数，则eax还要加上一个偏移量，以定位到不同的虚函数。比如，如果有两个虚函数，则会有两个vcall代码，分别对应不同的虚函数，其代码大概是下面的样子:
`vcall':00401BE0 mov eax,dword ptr [ecx]00401BE2 jmp dword ptr [eax]`vcall':00401190 mov eax,dword ptr [ecx]00401192 jmp dword ptr [eax+4]
编译器根据取的是哪个虚函数的地址，则相应的用对应的vcall地址代替。

　　总结一下：用前面方法取得的成员函数地址在虚拟函数的情况下仍然保持正确的行为，是因为编译器实际上传递了对应的vcall地址。而vcall代码会根据上下文this指针定位到对应的虚函数表，进而调用正确的虚函数。
　　最后，我们看一下多继承情况。很明显，现在情况要复杂得多。如果实际试一下，会碰到很多困难。首先，指定成员函数的时候可能会碰到冲突。其次，给定this指针的时候需要经过调整。另外，对虚拟继承可能还要特别处理。解决所有这些问题已经超出了这篇文章的范围，并且我想要的成员函数指针是一个真正的指针，而在多继承的情况下，很多时候成员函数指针已经变成了一个结构体(见参考文献)，这时要正确调用该指针就变得格外困难。因此结论是，上面讨论的方法并不适用于多继承的情况，要想在多继承的情况下直接调用成员函数地址，必须手工处理各种调整，没有简单的统一方法。









