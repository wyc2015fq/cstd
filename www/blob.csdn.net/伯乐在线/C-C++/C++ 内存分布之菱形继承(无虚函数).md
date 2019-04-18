# C++ 内存分布之菱形继承(无虚函数) - 文章 - 伯乐在线
原文出处： [armwind](http://blog.csdn.net/armwind/article/details/51863604)
菱形继承的定义是：两个子类继承同一父类，而又有子类同时继承这两个子类。例如a,b两个类同时继承c，但是又有一个d类同时继承a,b类。探究的过程还是很有趣的。 菱形继承的内存布局探究花了我几天时间，探究起来还是有点难度的。博文中如果有错误的地方，欢迎大家指正，大家共同进步。
**一、继承关系图**
![](http://jbcdn2.b0.upaiyun.com/2016/12/0666f3bcb18eda0287cb2b4fb4ab27f9.png)
图1.菱形类图
**二、源代码**


```
#include<iostream>  
using namespace std;  
  
class ancestor{  
    public:  
        int c;  
        ancestor():c(88){}  
        int* get_c(void){return &c;}  
};  
  
class father:virtual public ancestor{  
    public:  
        int cc;  
        father():cc(99){}  
        int* fa_get_anc_c(void){return get_c();}  
        int* get_cc(void){return &cc;}  
};  
class mother:virtual public ancestor{  
    public:  
        int ccc;  
        mother():ccc(100){}  
        int* ma_get_anc_c(void){return get_c();}  
        int* get_ccc(void){return &ccc;}  
};  
  
class son :public father,public mother{  
    public:  
        int d;  
        int e;  
        son():d(2),e(5){}  
};  
  
int main(){  
    ancestor *anc= new ancestor();  
    father *ba = new father();  
    mother *ma = new mother();  
    son *er = new son();  
    ancestor &ancanc =*er;  
    father &baba = *er;  
    mother &mama = *er;  
  
    int a =baba.c;  
    cout <<"anc_c:"<<ancanc.get_c()<<endl;  
    cout <<"\n"<<endl;  
  
    cout <<"baba_c:"<<baba.fa_get_anc_c()<<endl;  
    cout <<"baba_cc:"<<baba.get_cc()<<endl;  
    cout <<"\n"<<endl;  
  
    cout <<"mama_c:"<<mama.ma_get_anc_c()<<endl;  
    cout <<"mama_ccc:"<<mama.get_ccc()<<endl;  
    cout <<"\n"<<endl;  
      
    cout <<"&er :"<<er<<endl;  
    cout <<"er.c:"<<er->get_c()<<endl;  
    cout <<"er.fa_get_anc_c:"<<er->fa_get_anc_c()<<endl;  
    cout <<"er.get_cc:"<<er->get_cc()<<endl;  
    cout <<"er.ma_get_anc_c:"<<er->ma_get_anc_c()<<endl;  
    cout <<"er.get_ccc:"<<er->get_ccc()<<endl;  
    cout <<"er.d:"<<&er->d<<endl;  
    cout <<"er.e:"<<&er->e<<endl;  
  
    cout <<"\n"<<endl;  
    cout << "sizeof(ancestor):"<<sizeof(*anc)<<endl;  
    cout << "sizeof(father):"<<sizeof(*ba)<<endl;  
    cout << "sizeof(mother):"<<sizeof(*ma)<<endl;  
    cout << "sizeof( -son-):"<<sizeof(*er)<<endl;  
}
```
在使用g++编译的时候，如果father类和mother不是用虚继承的话，编译会报错的。有网友爆料说window平台是可以编译过的，这里我们就不关心了。下面的情况就会报错
class father: public ancestor //缺少virtual关键字
class mother:public ancestor//缺少virtual关键字
错误log：下面明显说有些成员和方法是模棱两可的。


```
father_jing.cpp: In function ‘int main()’:
father_jing.cpp:47: error: ‘ancestor’ is an ambiguous base of ‘son’
father_jing.cpp:64: error: request for member ‘get_c’ is ambiguous
father_jing.cpp:15: error: candidates are: int* ancestor::get_c()
father_jing.cpp:15: error:                 int* ancestor::get_c()
```
打印结果：下面打印的数据都是子类er对象的


```
anc_c:0xbdd094  
baba_c:0xbdd094
baba_cc:0xbdd078
mama_c:0xbdd094
mama_ccc:0xbdd088
&er :0xbdd070
er.c:0xbdd094
er.fa_get_anc_c:0xbdd094
er.get_cc:0xbdd078
er.ma_get_anc_c:0xbdd094
er.get_ccc:0xbdd088
er.d:0xbdd08c
er.e:0xbdd090
sizeof(ancestor):4
sizeof(father):16
sizeof(mother):16
sizeof( -son-):40
```
上面可以看到发现下面几个规律。
1.不管是直接打印子类er对象中c数据的地址，还是将子类对象er转换成father和mother指针，分别打印c数据域地址都是一样的。
2.除了ancestor对象的数据域（菱形的顶端类）在最顶端外，其它数据域仍然是按着继承关系排列的。但是这里共有祖先ancestor的数据域却放到了最后面。
打印的地址没有明显规律，而且和上一篇我们探究的多继承情况很不一样。
这里我们抛出下面几个疑问：
1.为什么ancestor的c数据域，没有放在子类对象首地址。
2.为什么子类对象son数据大小不是father和mother对象的之和。
3.为什么将son对象强制转化成father和mother对象他们的c数据域地址是同一个地址，难道他们没有继承ancestor吗？
……..
带着种种问题，我们开启探究之旅。
**三、探究分析**
**1)调试手段**
在使用g++编译C++程序一个强有力的编译选项，下面是它的注释：
> 
-fdump-class-hierarchy-options (C++ only)
Dump a representation of each class’s hierarchy and virtual function table layout to a file.  The file name is made by appending .class
to the source file name, and the file is created in the same directory as the output file.  If the -options form is used, options
controls the details of the dump as described for the -fdump-tree options.
意思就是：带上这个编译选项，可以在生成一个.class后缀文件，里面包含该文件的所有虚表。
这里使用下面的编译命令，来生成我们测试代码的虚表class，在生成虚表class当中，会夹杂许多其它无关紧要的虚表函数，如果你想看看这些无关紧要的虚表，还是自己把例子跑一遍吧，也许会有奇迹发现。这里我们只关心和我们类相关的虚表。有人可能会问，这里根本没有虚函数，怎么就有续表信息了。也许这就是因为我们是虚继承吧。看到下面那条短小精悍的命令了吗，赶紧执行它试试吧！
**g++-fdump-class-hierarchy father_jing.cpp**
虚表信息：


```
Vtable for father   //字面意思太明显，还要我翻译吗^_^  
father::_ZTV6father: 3u entries //3个入口函数，其实无效  
0     12u  
8     (int (*)(...))0  
16    (int (*)(...))(& _ZTI6father) //这个保存的是虚表开始地址。  
  
Class father  
   size=16 align=8 //father类大小是16字节，对齐8个字节  
   base size=12 base align=8  //基类大小12字节，对齐8字节，这个有点不准确，我们打印结果是基类4字节。  
father (0x7f98f88e6e00) 0  
    vptridx=0u vptr=((& father::_ZTV6father) + 24u) //虚表首地址加偏移量  
  ancestor (0x7f98f88e6e70) 12 virtual  
      vbaseoffset=-0x00000000000000018  
  
Vtable for mother  
mother::_ZTV6mother: 3u entries  
0     12u  
8     (int (*)(...))0  
16    (int (*)(...))(& _ZTI6mother)  
  
Class mother  
   size=16 align=8  
   base size=12 base align=8  
mother (0x7f98f89183f0) 0  
    vptridx=0u vptr=((& mother::_ZTV6mother) + 24u)//虚表首地址加偏移量  
ancestor (0x7f98f8918460) 12 virtual  
      vbaseoffset=-0x00000000000000018  
  
Vtable for son  
son::_ZTV3son: 6u entries  
0     36u    //这个值就是在将子类对象赋给father指针时，用father指针取c时，根据这个偏移量计算出c的地址，这一系列动作由编译器在编译阶段完成  
8     (int (*)(...))0  
16    (int (*)(...))(& _ZTI3son)  
24    20u   //如上同理  
32    (int (*)(...))-0x00000000000000010  
40    (int (*)(...))(& _ZTI3son)  
  
Construction vtable for father (0x7f98f8921a80 instance) in son //子类对象er中创建father对象虚表.  
son::_ZTC3son0_6father: 3u entries  
0     36u  
8     (int (*)(...))0  
16    (int (*)(...))(& _ZTI6father) //还是指向父类对象的虚表指针  
  
Construction vtable for mother (0x7f98f8921b60 instance) in son //子类对象er中创建mother对象虚表.  
son::_ZTC3son16_6mother: 3u entries  
0     20u  
8     (int (*)(...))0  
16    (int (*)(...))(& _ZTI6mother)//同理  
  
Class son  
   size=40 align=8  
   base size=36 base align=8  //这里base大小是36字节，father+mother=36.  
son (0x7f98f8920b00) 0  
    vptridx=0u vptr=((& son::_ZTV3son) + 24u) //子类对象中，包含的father数据的虚指针，至于那个vptridx是干什么用的，我也没搞多明白，对照子类虚表  
                                         //也许会有什么发现。  
  
father (0x7f98f8921a80) 0  
      primary-for son (0x7f98f8920b00)  
      subvttidx=8u  
    ancestor (0x7f98f8921af0) 36 virtual  
        vbaseoffset=-0x00000000000000018  
  mother (0x7f98f8921b60) 16  
      subvttidx=16u vptridx=24u vptr=((& son::_ZTV3son) + 48u) //子类对象中，包含的mother数据的虚指针  
    ancestor (0x7f98f8921af0) alternative-path
```
当C++中出现了虚函数，编译器都会为每一个类生成一个虚表，这个虚表具有可读属性，在ubuntu上它驻留在.rodata段，而且该类所有对象共有这个虚表。在后面会有打印信息，来证明这点。在每一个实例内存空间的最前面会安排一个vptr来指向这个虚表。在后面调试的时候，我会用gdb打出每一个实例的vptr。针对我们这个例子，上面是一张无效的虚表，因为我们类中根本就没有虚函数。原因就是我们采用了虚继承，g++还是按着有虚表的方式来编译。这样的话g++就会把一个虚指针安放在er对象中的father和mother数据开始处。
**2)GDB顺藤摸瓜**
gdb的调试大家应该很熟悉了吧。不熟悉的请看陈皓大神的[GDB调试程序](http://blog.csdn.net/haoel/article/details/2879)


```
Breakpoint 1, main () at father_jing.cpp:43  
43      ancestor *anc= new ancestor();  
(gdb) n  
44      father *ba = new father();  
(gdb) n  
45      mother *ma = new mother();  
(gdb) n  
46      son *er = new son();  
(gdb) n  
47      ancestor &ancanc =*er;  
(gdb) n  
48      father &baba = *er;  
(gdb) n  
49      mother &mama = *er;  
(gdb) n  
51      cout <<"anc_c:"<<ancanc.get_c()<<endl;  
(gdb) p ((int*)(er)+0)  
$1 = (int *) 0x603070  
(gdb) p *((int*)(er)+0)  
$2 = 4199096  
(gdb) p ((int*)(er)+1)  
$3 = (int *) 0x603074  
(gdb) p *((int*)(er)+1)  
$4 = 0  
(gdb) p ((int*)(er)+2)  
$5 = (int *) 0x603078  
(gdb) p *((int*)(er)+2)  //father数据域cc  
$6 = 99  
(gdb) p ((int*)(er)+3)  
$7 = (int *) 0x60307c  
(gdb) p *((int*)(er)+3)  
$8 = 0   //这个地方就是ancestor祖先的数据域，但是这里居然是0，不是88,why  
(gdb) p ((int*)(er)+4)  
$9 = (int *) 0x603080  
(gdb) p *((int*)(er)+4)  
$10 = 4199120  
(gdb) p ((int*)(er)+5)  
$11 = (int *) 0x603084  
(gdb) p *((int*)(er)+5)  
$12 = 0  
(gdb) p ((int*)(er)+6)  
$13 = (int *) 0x603088  
(gdb) p *((int*)(er)+6) //mother数据域ccc,那我们的祖先的ancestor数据域c呢。  
$14 = 100  
(gdb) p ((int*)(er)+7) //这个地方是ancestor祖先的数据域，但是这里居然是2,已经是子类的数据域了ancestor根本没有,why  
$15 = (int *) 0x60308c  
(gdb) p *((int*)(er)+7)  
$16 = 2  
(gdb) p ((int*)(er)+8)  
$17 = (int *) 0x603090  
(gdb) p *((int*)(er)+8)  
$18 = 5  
(gdb) p ((int*)(er)+9)  
$19 = (int *) 0x603094  
(gdb) p *((int*)(er)+9)  //最后我们打出son中基类ancestor数据域c放到了最后，地址为证。  
$20 = 88  
(gdb) p ((int*)(er)+10)  
$21 = (int *) 0x603098  
(gdb) p *((int*)(er)+10) //到这里已经获取到的是一个无效的值了，和我们类实例没什么关系了。由son大小是40字节，也可以知道。  
$22 = 135025    
(gdb) n
```
上面打印的地址和我们在之前的打印结果不一样的，这很正常。每次运行系统分配的地址都是不一样的。上面的打印结果可以看到下面几个现象：
1.子类对象中对应father类的数据域中，没有father继承ancestor的c数据域。
2.子类对象中对应father类的数据起始位置放的是虚表存放地址。
3.子类对象中对应mother类的数据域中，没有mother继承ancestor的c数据域。
4.子类对象中对应mother类的数据起始位置放的是虚表存放地址。
5.祖先ancestor放到了子类对象的最后，这也是最大的亮点。
由上面的打印结果，我们可以得到下面这张映射表：
![](http://jbcdn2.b0.upaiyun.com/2016/12/47c3adadd52cf7458ef4093646985c22.png)
图2 er对象的内存分布
**3)子类对象地址赋给父类指针会发生什么**
为了探究这个问题，我在代码中添加下面一行代码，就是为了验证子类对象转换成父类指针时，打印father中的c地址到底是它原有的的（0x60307c），还是打印er数据中最下面的c（0x603094）。


```
int a =baba.c;
```
首先，用objdump命令将可执行文件反汇编成汇编.main函数对应的汇编代码。
**objdump -S a.out > tttttt.txt**
使用这个命令会生成带有C语言的汇编的代码，前提是我们在编译可执行文件时，添加了-g选项。为了各位同学能自己计算下面注释中的一些值，打出了在进入main函数之前寄存器列表信息：


```
(gdb) info registers all  
rax            0x7ffff75b8548    140737343358280  
rbx            0x0                     0  
rcx            0x60                    96  
rdx            0x7fffffffdf58    140737488346968  
rsi            0x7fffffffdf48    140737488346952  
rdi            0x1                     1  
rbp            0x7fffffffde60    0x7fffffffde60  
rsp            0x7fffffffde10    0x7fffffffde10
```
下面只是主要的main函数反汇编代码：


```
int main(){  
  400a34:    55                       push   %rbp       "由1可得，此时rbp=0x7fffffffde60，将rbp寄存器入栈，关于这里的意思大家可以看看Linux中的局部变量和栈  
  400a35:    48 89 e5                 mov    %rsp,%rbp  "此时rsp = 0x7fffffffde10  
  400a38:    53                       push   %rbx  
  400a39:    48 83 ec 48              sub    $0x48,%rsp  
    ancestor *anc= new ancestor();  
  400a3d:    bf 04 00 00 00           mov    $0x4,%edi  
  400a42:    e8 f9 fe ff ff           callq  400940 <_Znwm@plt>  
  400a47:    48 89 c3                 mov    %rax,%rbx  "rax返回的就是anc对象实例的地址  
  400a4a:    48 89 d8                 mov    %rbx,%rax  
  400a4d:    48 89 c7                 mov    %rax,%rdi  
  400a50:    e8 43 05 00 00           callq  400f98 <_ZN8ancestorC1Ev> "调用构造函数  
  400a55:    48 89 5d e0              mov    %rbx,-0x20(%rbp)          "将anc实例如栈  
    father *ba = new father();  
  400a59:    bf 10 00 00 00           mov    $0x10,%edi  
  400a5e:    e8 dd fe ff ff           callq  400940 <_Znwm@plt>  
  400a63:    48 89 c3                 mov    %rax,%rbx                 "rax返回的就是ba对象实例的地址  
  400a66:    48 89 d8                 mov    %rbx,%rax  
  400a69:    48 89 c7                 mov    %rax,%rdi  
  400a6c:    e8 73 05 00 00           callq  400fe4 <_ZN6fatherC1Ev> "调用构造函数  
  400a71:    48 89 5d d8              mov    %rbx,-0x28(%rbp)        "将ba实例如栈  
    mother *ma = new mother();  
  400a75:    bf 10 00 00 00           mov    $0x10,%edi  
  400a7a:    e8 c1 fe ff ff           callq  400940 <_Znwm@plt>  
  400a7f:    48 89 c3                 mov    %rax,%rbx  "rax返回的就是ma对象实例的地址  
  400a82:    48 89 d8                 mov    %rbx,%rax  
  400a85:    48 89 c7                 mov    %rax,%rdi  
  400a88:    e8 f5 05 00 00           callq  401082 <_ZN6motherC1Ev> "调用构造函数  
  400a8d:    48 89 5d d0              mov    %rbx,-0x30(%rbp)        "将ma实例如栈  
    son *er = new son();  
  400a91:    bf 28 00 00 00           mov    $0x28,%edi  
  400a96:    e8 a5 fe ff ff           callq  400940 <_Znwm@plt>  
  400a9b:    48 89 c3                 mov    %rax,%rbx  "rax返回的就是er对象实例的地址  
  400a9e:    48 89 d8                 mov    %rbx,%rax  
  400aa1:    48 89 c7                 mov    %rax,%rdi  
  400aa4:    e8 4d 06 00 00           callq  4010f6 <_ZN3sonC1Ev>  "调用构造函数  
  400aa9:    48 89 5d c8              mov    %rbx,-0x38(%rbp)      "将er实例如栈  
    ancestor &ancanc =*er;  
  400aad:    48 83 7d c8 00           cmpq   $0x0,-0x38(%rbp)   "这里先比较er对象实例是否为空  
  400ab2:    74 18                    je     400acc <main+0x98>  
  400ab4:    48 8b 55 c8              mov    -0x38(%rbp),%rdx  
  400ab8:    48 8b 45 c8              mov    -0x38(%rbp),%rax   "取出里面保存er对象实例的地址  
  400abc:    48 8b 00                 mov    (%rax),%rax    
  400abf:    48 83 e8 18              sub    $0x18,%rax  
  400ac3:    48 8b 00                 mov    (%rax),%rax  
  400ac6:    48 8d 04 02              lea    (%rdx,%rax,1),%rax  
  400aca:    eb 05                    jmp    400ad1 <main+0x9d>  
  400acc:    b8 00 00 00 00           mov    $0x0,%eax  
  400ad1:    48 89 45 c0              mov    %rax,-0x40(%rbp)  "这里将er中保存的c域地址放到bp-40偏移处  
    father &baba = *er;  
  400ad5:    48 8b 45 c8              mov    -0x38(%rbp),%rax  "直接将er对象的地址放到rbp-0x48偏移处  
  400ad9:    48 89 45 b8              mov    %rax,-0x48(%rbp)  
    mother &mama = *er;  
  400add:    48 83 7d c8 00           cmpq   $0x0,-0x38(%rbp)  
  400ae2:    74 0a                    je     400aee <main+0xba>  
  400ae4:    48 8b 45 c8              mov    -0x38(%rbp),%rax  
  400ae8:    48 83 c0 10              add    $0x10,%rax  
  400aec:    eb 05                    jmp    400af3 <main+0xbf>  
  400aee:    b8 00 00 00 00           mov    $0x0,%eax  
  400af3:    48 89 45 b0              mov    %rax,-0x50(%rbp) "这里会将mama对象放到rbp-0x50处，具体怎么算的，请看下面获取c的过程  
  
    int a =baba.c;  
  400af7:    48 8b 55 b8              mov    -0x48(%rbp),%rdx "这里代码留到下面分析  
  400afb:    48 8b 45 b8              mov    -0x48(%rbp),%rax  
  400aff:    48 8b 00                 mov    (%rax),%rax  
  400b02:    48 83 e8 18              sub    $0x18,%rax  
  400b06:    48 8b 00                 mov    (%rax),%rax  
  400b09:    48 8d 04 02              lea    (%rdx,%rax,1),%rax  
  400b0d:    8b 00                    mov    (%rax),%eax  
  400b0f:    89 45 ec                 mov    %eax,-0x14(%rbp)  
  
.......  
  
}
```
我们知道每一个局部变量一般都会保存到栈中，如果想深入了解的话，同学们可以查看Linux中的局部变量和栈。这里在main函数中生成的都是局部变量，所以为此我们可以根据汇编代码，列出对象的分布图，如下所示。
![](http://jbcdn2.b0.upaiyun.com/2016/12/a105c97b921b746900a89872e3667cc1.png)
图 3 main函数中临时变量分布
当执行到int a = baba.c处，到底调用的是子类实例son中的父类数据域的c，还是共有的数据c。为了更清楚方便大家推算，我打出了执行这行代码前后的寄存器列表信息。


```
在执行int a = baba.c之前的寄存器状态  
(gdb) info registers all  
rax            0x603080    6303872  
rbx            0x603070    6303856  
rcx            0x10    16  
rdx            0x603070    6303856  
rsi            0x4013b0    4199344  
rdi            0x603080    6303872  
rbp            0x7fffffffde60    0x7fffffffde60  
rsp            0x7fffffffde10    0x7fffffffde10  
..............................  
    int a =baba.c;  
400af7:    48 8b 55 b8          mov    -0x48(%rbp),%rdx         "rdx=0x603070   
  
400afb:    48 8b 45 b8          mov    -0x48(%rbp),%rax         "rdx=0x603070  
400aff:    48 8b 00             mov    (%rax),%rax              "取出rax中的内容rax=0x401358  --------<1>  
400b02:    48 83 e8 18          sub    $0x18,%rax               "将寄存器rax-0x18,即rax=0x401340  
400b06:    48 8b 00             mov    (%rax),%rax              "取出0x401340的内容，rax=36  -------<2>  
400b09:    48 8d 04 02          lea    (%rdx,%rax,1),%rax       "rax=0x603094  ---------<3>  
400b0d:    8b 00                mov    (%rax),%eax              "0x603094就是c的地址  
400b0f:    89 45 ec             mov    %eax,-0x14(%rbp)         "将c的值放到rbp-0x14偏移处。  
...........................  
  
执行int a = baba.c之后的寄存器值列表  
(gdb) info registers all  
rax            0x58    88     "返回值就是88了  
rbx            0x603070    6303856  
rcx            0x10    16  
rdx            0x603070    6303856  
rsi            0x4013b0    4199344  
rdi            0x603080    6303872  
rbp            0x7fffffffde60    0x7fffffffde60  
rsp            0x7fffffffde10    0x7fffffffde10
```
扩展：
<1>(gdb) p *0x603070
$1 = 4199256 = 0x401358
(gdb) p *er
$2 = {<father> = {<ancestor> = {c = 88}, _vptr.father = 0x401358 <vtable for son+24>,
cc = 99}, <mother> = {_vptr.mother = 0x401370,
ccc = 100}, d = 2, e = 5}
<2>(gdb) p *0x401340
$3 = 36
<3>lea (%rdx,%rax,1),%rax 这句话的意思就是(rdx)+(rax)*1 即
=0x603070+36×1 = 0x603094 ，这里就得出了c的地址。
针对上面的36我们再来看看son的vtable
Vtable for son
son::_ZTV3son: 6u entries
0 36u //这个就是,编译器在虚表中记下了ancestor数据域的偏移
8 (int (*)(…))0
16 (int (*)(…))(& _ZTI3son)
24 20u
32 (int (*)(…))-0x00000000000000010
40 (int (*)(…))(& _ZTI3son)
总结：到现在我们就知道上面一开始抛出的几个问题了吧。
1.为什么ancestor的c数据域，没有放在子类对象首地址。
答:子类对象的首地址存放的是子类的虚表指针。由上面的调试和log打印我们已经发现，在菱形继承中，公共父类的数据域都是公用一份的，并且都是放在子类最下面的数据区。如果我们把子类对象地址赋给父类指针，例如：father *fa;fa=&er;.这里我们可以在上面的调试中发现，编译器会自动将对应father类的那部分数据区的首地址赋给fa。最后访问c域时，即fa->c,也是访问子类0x603094处的共有c数据。
2.为什么子类对象son数据大小不是father和mother对象的之和加son自己的数据域（sizeof(son)=sizeof(father)+sizeof(mother)+son_data）。
答：由于子类对象数据区多出了mother虚表指针，所以大小不一样。
3.为什么将son对象强制转化成father和mother对象他们的c数据域地址是同一个地址，难道他们没有继承ancestor吗？
答：上面一开始我看到子类对象中的father和mother数据域是8字节对齐的，我以为只是续表指针是8字节对齐，所以我将子类对象中对应father数据按4字节地址打印，结果本应该是c数据域的地方打印的是0，所以子类属于father的数据区中没有c数据域
