# C++对象内存布局 - gauss的专栏 - CSDN博客
2013年01月08日 13:05:25[gauss](https://me.csdn.net/mathlmx)阅读数：237
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)

C++对象内存布局
1.1    最简单的类
写一小段简单代码：
class CTest
{
public:
     int var_a;
     int var_b;
     int var_c;
public:
     void fun1()
     {
         var_a = 10;
     }
     void fun2()
     {
         var_b = 20;
     }
};
CTest a, *p;
int _tmain(int argc, _TCHAR* argv[])
{
     a.var_a = 1;
     a.var_b = 2;
     a.var_c = 3;
     p = &a;
     p->fun1();
     p->fun2();
     return 0;
}
为了更直观地观察编译器的行为，把a和p做为全局变量。
1.1.1   赋值语句的行为
先观察赋值语句的行为：
     a.var_a = 1;
004113CE C7 05 C0 74 41 00 01 00 00 00 mov         dword ptr [a (4174C0h)],1 
     a.var_b = 2;
004113D8 C7 05 C4 74 41 00 02 00 00 00 mov         dword ptr [a+4 (4174C4h)],2
     a.var_c = 3;
004113E2 C7 05 C8 74 41 00 03 00 00 00 mov         dword ptr [a+8 (4174C8h)],3
再观察赋值结束后p指向的内存区域：
0x004174C0  01 00 00 00 02 00 00 00 03 00 00 00 00 00 00  ...............
显然，这个时候p指向CTest::var_a，也就是说vs2008并没有在类中插入多余的内容。CTest的大小为var_a, var_b, var_c的大小之和，即12个字节。
1.1.2   函数调用的行为
再观察一下函数调用的行为：
     p->fun1();
004113F6 8B 0D BC 74 41 00 mov         ecx,dword ptr [p (4174BCh)] 
004113FC E8 53 FD FF FF   call        CTest::fun1 (411154h) 
这段代码可以说明CTest::fun1的调用和其它的C代码并没有什么不同，一个函数而已。看看call把我们带到哪里：
00411118 E9 83 03 00 00   jmp         CTest::fun2 (4114A0h) 
0041111D E9 6E 15 00 00   jmp         __CxxUnhandledExceptionFilter (412690h) 
00411122 E9 E9 15 00 00   jmp         __CxxSetUnhandledExceptionFilter (412710h)
00411127 E9 64 24 00 00   jmp         QueryPerformanceCounter (413590h) 
0041112C E9 9F 16 00 00   jmp         _wsetargv (4127D0h) 
00411131 E9 0A 17 00 00   jmp         __p__commode (412840h) 
00411136 E9 89 22 00 00   jmp         _unlock (4133C4h) 
0041113B E9 62 24 00 00   jmp         GetCurrentProcessId (4135A2h) 
00411140 E9 9B 04 00 00   jmp         _RTC_CheckStackVars2 (4115E0h) 
00411145 E9 80 18 00 00   jmp         __set_app_type (4129CAh) 
0041114A E9 A1 03 00 00   jmp         _RTC_CheckEsp (4114F0h) 
0041114F E9 8C 16 00 00   jmp         _RTC_Initialize (4127E0h) 
00411154 E9 07 03 00 00   jmp         CTest::fun1 (411460h) 
一个神秘的地方，只是func1和func2为什么要离得这么远呢？莫非有什么特殊的考虑？暂且不管它，在jmp的带领下，看到了func1的代码：
     void fun1()
     {
00411460 55               push        ebp  
00411461 8B EC            mov         ebp,esp 
00411463 81 EC CC 00 00 00 sub         esp,0CCh 
00411469 53               push        ebx  
0041146A 56               push        esi  
0041146B 57               push        edi  
0041146C 51               push        ecx  
0041146D 8D BD 34 FF FF FF lea         edi,[ebp-0CCh] 
00411473 B9 33 00 00 00   mov         ecx,33h 
00411478 B8 CC CC CC CC   mov         eax,0CCCCCCCCh 
0041147D F3 AB            rep stos    dword ptr es:[edi] 
0041147F 59               pop         ecx  
00411480 89 4D F8         mov         dword ptr [ebp-8],ecx 
         var_a = 10;
00411483 8B 45 F8         mov         eax,dword ptr [this] 
00411486 C7 00 0A 00 00 00 mov         dword ptr [eax],0Ah 
     }
0041148C 5F               pop         edi  
0041148D 5E               pop         esi  
0041148E 5B               pop         ebx  
0041148F 8B E5            mov         esp,ebp 
00411491 5D               pop         ebp  
00411492 C3               ret              
传说中的对象应该有一个this指针的，怎么传递过来的？
在调用fun1之前，将p的值赋给了ecx，在fun1里面，其它寄存器都是函数开始就入栈，函数退出时再出栈，唯独ecx是个例外，想来vs就是通过ECX来传递this指针的值。
1.1.3   当p指向自定义的缓冲区
由于在此种简单情况下，vs2008并没有添加额外的空间，想来把p指向自己定义的数组再调用其成员函数应该也可以工作得很好，试试下面的代码：
     int buf[3] = {4, 5, 6};
     p = (CTest*)buf;
     p->fun1();
经过此调用，buf[0]的值果然变成了10。
 这次我们添加一个子类，父类和子类均不带虚函数：
class CParent
{
public:
     int parent_a;
     int parent_b;
public:
     void parent_f1()
     {
         parent_a = 0x10;
     }
     void parent_f2()
     {
         parent_b = 0x20;
     }
};
class CChild : public CParent
{
public:
     int child_a;
     int child_b;
public:
     void child_f1()
     {
         child_a = 0x30;
     }
     void child_f2()
     {
         child_b = 0x40;
     }
};
1.1.1   内存布局
先给这些成员变量赋几个值：
     child.parent_a = 1;
0041138E C7 05 50 71 41 00 01 00 00 00 mov         dword ptr [child (417150h)],1
     child.parent_b = 2;
00411398 C7 05 54 71 41 00 02 00 00 00 mov         dword ptr [child+4 (417154h)],2
     child.child_a = 3;
004113A2 C7 05 58 71 41 00 03 00 00 00 mov         dword ptr [child+8 (417158h)],3
     child.child_b = 4;
004113AC C7 05 5C 71 41 00 04 00 00 00 mov         dword ptr [child+0Ch (41715Ch)],4
观察&child所指的内存区内容：
0x00417150  01 00 00 00 02 00 00 00 03 00 00 00 04 00 00 00  ................
很明显，VS将父类和子类的数据成员合并到同一块内存区域，但是同样没有往里面加额外的东西，&child指针将指向第一个成员parent_a。
1.1.2   函数调用
观察函数调用：
     child.parent_f1();
00411840 B9 50 71 41 00   mov         ecx,offset child (417150h) 
00411845 E8 78 F9 FF FF   call        CParent::parent_f1 (4111C2h) 
     child.child_f1();
0041184A B9 50 71 41 00   mov         ecx,offset child (417150h) 
0041184F E8 64 F9 FF FF   call        CChild::child_f1 (4111B8h) 
和简单父类的函数调用没什么两样。用ECX传递this指针。
1.1.3   变量覆盖
在子类里添加两个和父类同名的成员变量：
class CChild : public CParent
{
public:
     int child_a;
     int child_b;
     int parent_a;
     int parent_b;
………
再执行前面的赋值语句：
     child.parent_a = 1;
0041180E C7 05 04 75 41 00 01 00 00 00 mov         dword ptr [child+10h (417504h)],1
     child.parent_b = 2;
00411818 C7 05 08 75 41 00 02 00 00 00 mov         dword ptr [child+14h (417508h)],2
     child.child_a = 3;
00411822 C7 05 FC 74 41 00 03 00 00 00 mov         dword ptr [child+8 (4174FCh)],3
     child.child_b = 4;
0041182C C7 05 00 75 41 00 04 00 00 00 mov         dword ptr [child+0Ch (417500h)],4
可以发现赋值时使用的偏移量发生了变化，观察&child所在的内存区：
0x004174F4  00 00 00 00 00 00 00 00 03 00 00 00 04 00 00 00  ................
0x00417504  01 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00  ................
可以看到合并后的内存有6个整数，且对child.parent_a的赋值其实是对子类的成员进行赋值。
而当我们用下面的赋值语句时：
     pparent = &child;
00412E44 C7 05 E4 74 41 00 F4 74 41 00 mov         dword ptr [pparent (4174E4h)],offset child (4174F4h)
     pparent->parent_a = 1;
00412E4E A1 E4 74 41 00   mov         eax,dword ptr [pparent (4174E4h)] 
00412E53 C7 00 01 00 00 00 mov         dword ptr [eax],1 
     pparent->parent_b = 2;
00412E59 A1 E4 74 41 00   mov         eax,dword ptr [pparent (4174E4h)] 
00412E5E C7 40 04 02 00 00 00 mov         dword ptr [eax+4],2 
从偏移量就可以看出它是对父类的成员进行赋值。
1.1.4   函数覆盖
在子类里面添加一个和父类同名的函数，再进行函数调用：
     pchild = &child;
00412E26 C7 05 F0 74 41 00 F4 74 41 00 mov         dword ptr [pchild (4174F0h)],offset child (4174F4h)
     pchild->parent_f1();
00412E30 8B 0D F0 74 41 00 mov         ecx,dword ptr [pchild (4174F0h)] 
00412E36 E8 9B E3 FF FF   call        CChild::parent_f1 (4111D6h) 
     pparent = &child;
00412E3B C7 05 E4 74 41 00 F4 74 41 00 mov         dword ptr [pparent (4174E4h)],offset child (4174F4h)
     pparent->parent_f1();
00412E45 8B 0D E4 74 41 00 mov         ecx,dword ptr [pparent (4174E4h)] 
00412E4B E8 72 E3 FF FF   call        CParent::parent_f1 (4111C2h) 
从反汇编的结果可以明显看出使用父类指针和使用子类指针调用同名函数时的差异。
这次我们为父类加上虚函数：
class CParent
{
public:
     int parent_a;
     int parent_b;
public:
     virtual void parent_f1()
     {
         parent_a = 0x10;
     }
     virtual void parent_f2()
     {
         parent_b = 0x20;
     }
};
class CChild : public CParent
{
public:
     int child_a;
     int child_b;
public:
     virtual void parent_f1()
     {
         child_a = 0x30;
     }
     virtual void parent_f2()
     {
         child_b = 0x40;
     }
};
CChild child, *pchild;
CParent parent, *pparent;
1.1.1   内存布局
同样，我们先对其进行简单赋值再观察内存变化。
先对父类进行赋值：
     parent.parent_a = 1;
004139FE C7 05 B8 81 41 00 01 00 00 00 mov         dword ptr [parent+4 (4181B8h)],1
     parent.parent_b = 2;
00413A08 C7 05 BC 81 41 00 02 00 00 00 mov         dword ptr [parent+8 (4181BCh)],2
看内存：
0x004181B4  5c 68 41 00 01 00 00 00 02 00 00 00 00 00 00 00  /hA.............
很显然，在数据之前插入了四个字节，猜想这个应该就是所谓的vtbl的指针，如果这样，那么所有同类对象都应该有相同的指针，定义两个变量：
CParent v1, v2;
先看v1的内存：
0x0012FF58  5c 68 41 00 cc cc cc cc cc cc cc cc cc cc cc cc  PhA.............
再看v2的内存：
0x0012FF44  5c 68 41 00 cc cc cc cc cc cc cc cc cc cc cc cc  PhA.............
果然是一样的，嘿。
再看子类的赋值：
          child.child_a = 3;
00413A12 C7 05 AC 81 41 00 03 00 00 00 mov         dword ptr [child+0Ch (4181ACh)],3
     child.child_b = 4;
00413A1C C7 05 B0 81 41 00 04 00 00 00 mov         dword ptr [child+10h (4181B0h)],4
看&child的内存：
0x004181A0  50 68 41 00 00 00 00 00 00 00 00 00 03 00 00 00  PhA.............
0x004181B0  04 00 00 00 5c 68 41 00 01 00 00 00 02 00 00 00  ..../hA.........
很明显，父类和子类指向的vtbl是不一样的。
1.1.2   父类虚函数调用
先看父类虚函数的调用：
     pparent = &parent;
00413A26 C7 05 98 81 41 00 B4 81 41 00 mov         dword ptr [pparent (418198h)],offset parent (4181B4h)
     pparent->parent_f1();
00413A30 A1 98 81 41 00   mov         eax,dword ptr [pparent (418198h)] 
00413A35 8B 10            mov         edx,dword ptr [eax] 
00413A37 8B F4            mov         esi,esp 
00413A39 8B 0D 98 81 41 00 mov         ecx,dword ptr [pparent (418198h)] 
00413A3F 8B 02            mov         eax,dword ptr [edx] 
00413A41 FF D0            call        eax  
00413A43 3B F4            cmp         esi,esp 
00413A45 E8 1E D7 FF FF   call        @ILT+355(__RTC_CheckEsp) (411168h) 
     pparent->parent_f2();
00413A4A A1 98 81 41 00   mov         eax,dword ptr [pparent (418198h)] 
00413A4F 8B 10            mov         edx,dword ptr [eax] 
00413A51 8B F4            mov         esi,esp 
00413A53 8B 0D 98 81 41 00 mov         ecx,dword ptr [pparent (418198h)] 
00413A59 8B 42 04         mov         eax,dword ptr [edx+4] 
00413A5C FF D0            call        eax  
00413A5E 3B F4            cmp         esi,esp 
00413A60 E8 03 D7 FF FF   call        @ILT+355(__RTC_CheckEsp) (411168h) 
这回的函数调用就不是简单地使用名称进行调用了，而是先取出vtbl的首地址（pparent指向的头四个字节），然后加上偏移量，再取出这个新地址中存储的指针，最后用call进行函数调用。这个偏移量是由编译器自动排列得到的。
我们看看vtbl的内容：
0x0041685C  b4 10 41 00 08 12 41 00 44 76 41 00 dc 10 41 00  ..A...A.DvA...A.
这里有两个指针：0x004110b4和0x00411208，看看它们指向的位置：
004110B4 E9 57 0C 00 00   jmp         CParent::parent_f1 (411D10h) 
……..
00411208 E9 43 0B 00 00   jmp         CParent::parent_f2 (411D50h) 
果然指向的是父类的成员函数。
1.1.3   子类虚函数的调用
现在我们用子类的指针调用子类的虚函数：
     pchild = &child;
00413A65 C7 05 9C 81 41 00 A0 81 41 00 mov         dword ptr [pchild (41819Ch)],offset child (4181A0h)
     pchild->parent_f1();
00413A6F A1 9C 81 41 00   mov         eax,dword ptr [pchild (41819Ch)] 
00413A74 8B 10            mov         edx,dword ptr [eax] 
00413A76 8B F4            mov         esi,esp 
00413A78 8B 0D 9C 81 41 00 mov         ecx,dword ptr [pchild (41819Ch)] 
00413A7E 8B 02            mov         eax,dword ptr [edx] 
00413A80 FF D0            call        eax  
00413A82 3B F4            cmp         esi,esp 
00413A84 E8 DF D6 FF FF   call        @ILT+355(__RTC_CheckEsp) (411168h) 
     pchild->parent_f2();
00413A89 A1 9C 81 41 00   mov         eax,dword ptr [pchild (41819Ch)] 
00413A8E 8B 10            mov         edx,dword ptr [eax] 
00413A90 8B F4            mov         esi,esp 
00413A92 8B 0D 9C 81 41 00 mov         ecx,dword ptr [pchild (41819Ch)] 
00413A98 8B 42 04         mov         eax,dword ptr [edx+4] 
00413A9B FF D0            call        eax  
00413A9D 3B F4            cmp         esi,esp 
00413A9F E8 C4 D6 FF FF   call        @ILT+355(__RTC_CheckEsp) (411168h) 
它的调用方式和父类完全一样，都是先取vtbl的首地址，再加上偏移量后进行call。看一下子类的vtbl：
0x00416850  a9 11 41 00 0d 12 41 00 20 76 41 00 b4 10 41 00  ..A...A. vA...A.
这里有两个指针：0x004111a9和0x0041120d，看看它们指向的位置：
004111A9 E9 82 03 00 00   jmp         CChild::parent_f1 (411530h) 
……..
0041120D E9 6E 03 00 00   jmp         CChild::parent_f2 (411580h) 
果然指向的是子类的成员函数。
1.1.4   父类指针调用子类的虚函数
这回我们用一个父类指针来调用虚函数：
     pparent = &child;
00413AA4 C7 05 98 81 41 00 A0 81 41 00 mov         dword ptr [pparent (418198h)],offset child (4181A0h)
     pparent->parent_f1();
00413AAE A1 98 81 41 00   mov         eax,dword ptr [pparent (418198h)] 
00413AB3 8B 10            mov         edx,dword ptr [eax] 
00413AB5 8B F4            mov         esi,esp 
00413AB7 8B 0D 98 81 41 00 mov         ecx,dword ptr [pparent (418198h)] 
00413ABD 8B 02            mov         eax,dword ptr [edx] 
00413ABF FF D0            call        eax  
00413AC1 3B F4            cmp         esi,esp 
00413AC3 E8 A0 D6 FF FF   call        @ILT+355(__RTC_CheckEsp) (411168h) 
可以看到，这个时候使用的是子函数的VTBL，因此调用的自然是子类的虚函数。
这回我们考虑多重继承的情况：
class CParentA
{
public:
     int parenta_a;
     int parenta_b;
public:
     virtual void parenta_f1()
     {
         parenta_a = 0x10;
     }
     virtual void parenta_f2()
     {
         parenta_b = 0x20;
     }
};
class CParentB
{
public:
     int parentb_a;
     int parentb_b;
public:
     virtual void parentb_f1()
     {
         parentb_a = 0x30;
     }
     virtual void parentb_f2()
     {
         parentb_b = 0x40;
     }
};
class CChild : public CParentA, public CParentB
{
public:
     int child_a;
     int child_b;
public:
     virtual void parenta_f1()
     {
         child_a = 0x50;
     }
     virtual void parenta_f2()
     {
         child_b = 0x60;
     }
     virtual void parentb_f1()
     {
         child_a = 0x70;
     }
     virtual void parentb_f2()
     {
         child_b = 0x80;
     }
};
CChild child, *pchild;
这个子类拥有两个父类。
1.1.1   内存布局
我们先对子类成员进行赋值，然后观察其内存变化：
     child.parenta_a = 1;
00413B9E C7 05 50 85 41 00 01 00 00 00 mov         dword ptr [child+4 (418550h)],1
     child.parenta_b = 2;
00413BA8 C7 05 54 85 41 00 02 00 00 00 mov         dword ptr [child+8 (418554h)],2
     child.parentb_a = 3;
00413BB2 C7 05 5C 85 41 00 03 00 00 00 mov         dword ptr [child+10h (41855Ch)],3
     child.parentb_b = 4;
00413BBC C7 05 60 85 41 00 04 00 00 00 mov         dword ptr [child+14h (418560h)],4
     child.child_a = 5;
00413BC6 C7 05 64 85 41 00 05 00 00 00 mov         dword ptr [child+18h (418564h)],5
     child.child_b = 6;
00413BD0 C7 05 68 85 41 00 06 00 00 00 mov         dword ptr [child+1Ch (418568h)],6
再看&child的内容：
0x0041854C  60 68 41 00 01 00 00 00 02 00 00 00 50 68 41 00  `hA.........PhA.
0x0041855C  03 00 00 00 04 00 00 00 05 00 00 00 06 00 00 00  ................
很容易可以发现，这个子类空间中包含有两个vtbl的指针。再有就是父类和子类的数据成员。当我们把继承的顺序改为：
class CChild : public CParentB, public CParentA
再看内存：
0x0041854C  60 68 41 00 03 00 00 00 04 00 00 00 50 68 41 00  `hA.........PhA.
0x0041855C  01 00 00 00 02 00 00 00 05 00 00 00 06 00 00 00  ................
可以发现，vs对类成员的排列是按照继承的顺序而来的。
1.1.2   虚函数调用
观察子类对虚函数的调用：
     pchild = &child;
00413BDA C7 05 48 85 41 00 4C 85 41 00 mov         dword ptr [pchild (418548h)],offset child (41854Ch)
     pchild->parenta_f1();
00413BE4 A1 48 85 41 00   mov         eax,dword ptr [pchild (418548h)] 
00413BE9 8B 10            mov         edx,dword ptr [eax] 
00413BEB 8B F4            mov         esi,esp 
00413BED 8B 0D 48 85 41 00 mov         ecx,dword ptr [pchild (418548h)] 
00413BF3 8B 02            mov         eax,dword ptr [edx] 
00413BF5 FF D0            call        eax  
00413BF7 3B F4            cmp         esi,esp 
00413BF9 E8 74 D5 FF FF   call        @ILT+365(__RTC_CheckEsp) (411172h) 
     pchild->parenta_f2();
00413BFE A1 48 85 41 00   mov         eax,dword ptr [pchild (418548h)] 
00413C03 8B 10            mov         edx,dword ptr [eax] 
00413C05 8B F4            mov         esi,esp 
00413C07 8B 0D 48 85 41 00 mov         ecx,dword ptr [pchild (418548h)] 
00413C0D 8B 42 04         mov         eax,dword ptr [edx+4] 
00413C10 FF D0            call        eax  
00413C12 3B F4            cmp         esi,esp 
00413C14 E8 59 D5 FF FF   call        @ILT+365(__RTC_CheckEsp) (411172h) 
     pchild->parentb_f1();
00413C19 8B 0D 48 85 41 00 mov         ecx,dword ptr [pchild (418548h)] 
00413C1F 83 C1 0C         add         ecx,0Ch 
00413C22 A1 48 85 41 00   mov         eax,dword ptr [pchild (418548h)] 
00413C27 8B 50 0C         mov         edx,dword ptr [eax+0Ch] 
00413C2A 8B F4            mov         esi,esp 
00413C2C 8B 02            mov         eax,dword ptr [edx] 
00413C2E FF D0            call        eax  
00413C30 3B F4            cmp         esi,esp 
00413C32 E8 3B D5 FF FF   call        @ILT+365(__RTC_CheckEsp) (411172h) 
     pchild->parentb_f2();
00413C37 8B 0D 48 85 41 00 mov         ecx,dword ptr [pchild (418548h)] 
00413C3D 83 C1 0C         add         ecx,0Ch 
00413C40 A1 48 85 41 00   mov         eax,dword ptr [pchild (418548h)] 
00413C45 8B 50 0C         mov         edx,dword ptr [eax+0Ch] 
00413C48 8B F4            mov         esi,esp 
00413C4A 8B 42 04         mov         eax,dword ptr [edx+4] 
00413C4D FF D0            call        eax  
00413C4F 3B F4            cmp         esi,esp 
00413C51 E8 1C D5 FF FF   call        @ILT+365(__RTC_CheckEsp) (411172h) 
从上面的代码可以发现，当调用父类A的虚函数时，使用的是第一个vtbl的指针，而在调用父类B的虚函数时，使用的则是第二个vtbl的指针。
还有很重要的一点，vs是通过ecx这个寄存器来传递this指针的，上面的汇编代码表明，当调用父类A的虚函数时，this指针将指向子类的首地址，当调用父类B的虚函数时，this指针将指向存放第二个vtbl的地址，也就是父类A成员结束的位置。
跟踪可以看到，调用parenta_a时的this指针值为0x0041854c，而调用parentb_a时的this指针值为0x00418558。从前面&child的内容可以得到印证。
1.1.3   当两个父类有同名的虚函数
一个很有意思的问题，既然调用父类A的虚函数时传递的this指针和调用父类B的虚函数时传递的this指针不一样，那么假如父类A和父类B拥有同名的虚函数，vs又该如何处理呢？
试试将CParentB::parentb_f1的名称改为CParentB::parenta_f1，编译：
     pchild = &child;
00413BDA C7 05 48 85 41 00 4C 85 41 00 mov         dword ptr [pchild (418548h)],offset child (41854Ch)
     pchild->parenta_f1();
00413BE4 A1 48 85 41 00   mov         eax,dword ptr [pchild (418548h)] 
00413BE9 8B 10            mov         edx,dword ptr [eax] 
00413BEB 8B F4            mov         esi,esp 
00413BED 8B 0D 48 85 41 00 mov         ecx,dword ptr [pchild (418548h)] 
00413BF3 8B 02            mov         eax,dword ptr [edx] 
00413BF5 FF D0            call        eax  
00413BF7 3B F4            cmp         esi,esp 
00413BF9 E8 74 D5 FF FF   call        @ILT+365(__RTC_CheckEsp) (411172h) 
显然，这个时候传递的是child的首地址。
再看看vtbl的其它内容，先写段代码：
class CParent
{
public:
     int parent_a;
     int parent_b;
public:  
     virtual void parent_f1() {}
     virtual void parent_f2() {}
public:  
     void parent_f3() {}
     void parent_f4() {}
};
class CChild : public CParent
{
public:
     int child_a;
     int child_b;
public:       // 子类的函数
     virtual void child_f1() {}
     virtual void child_f2() {}
public:  // 不可重载的函数
     void child_f3() {}
     void child_f4() {}
public:       // 重载父类的函数
     virtual void parent_f2() {}
     virtual void parent_f1() {}
};
CChild child, *pchild;
CParent parent, *pparent;
这是一个单继承的关系。
1.1.1   Vtbl的属性
Vtbl应该是在链接时确定，之后就不再改变，那么它应该具有只读的属性，先对此做个验证：
     MEMORY_BASIC_INFORMATION mi;
     ::VirtualQueryEx(::GetCurrentProcess(), (void*)(*(unsigned int*)&parent), &mi, sizeof(mi));
这段代码将查询vtbl所在内存块的属性，返回的mi值如下：
              BaseAddress   0x00416000 ___xc_a     void *
              AllocationBase 0x00400000    void *
              AllocationProtect    0x00000080    unsigned long
              RegionSize      0x00002000    unsigned long
              State       0x00001000    unsigned long
              Protect    0x00000002    unsigned long
              Type       0x01000000    unsigned long
Protect这个成员的值表明vtbl所在的内存块具有PAGE_READONLY的属性，MSDN这样解释这个属性的意义：
Enables read access to the committed region of pages. An attempt to write to the committed region results in an access violation. If the system differentiates between read-only access and execute access, an attempt to execute code in the committed region
 results in an access violation.
因而vtbl的内容将不可更改。
1.1.2   单继承时vtbl的内容
那么一个vtbl究竟应该有多大？先看&parent的vtbl指针，它的值为0x00416868，看这块内存的内容：
0x00416868  c8 10 41 00 1e 10 41 00 00 00 00 00 54 76 41 00  ..A...A.....TvA.
它的两个指针0x004110c8和0x0041101e分别指向下面的函数：
CParent::parent_f1:
004110C8 E9 33 08 00 00   jmp         CParent::parent_f1 (411900h) 
CParent::parent_f2:
0041101E E9 1D 09 00 00   jmp         CParent::parent_f2 (411940h) 
指向父类的两个虚函数。
再看&child的vtbl指针值为0x00416850，看它的内容：
0x00416850  d1 11 41 00 82 10 41 00 b3 11 41 00 0a 10 41 00  ..A...A...A...A.
0x00416860  00 00 00 00 3c 76 41 00 c8 10 41 00 1e 10 41 00  ....<vA...A...A.
这里有四个指针0x004111d1，0x00411082，0x004111b3和0x0041100a，看它们的内容：
CChild::parent_f1:
004111D1 E9 9A 06 00 00   jmp         CChild::parent_f1 (411870h) 
CChild::parent_f2:
00411082 E9 A9 07 00 00   jmp         CChild::parent_f2 (411830h) 
CChild::child_f1:
004111B3 E9 F8 05 00 00   jmp         CChild::child_f1 (4117B0h) 
CChild::child_f2:
0041100A E9 E1 07 00 00   jmp         CChild::child_f2 (4117F0h) 
这说明子类的VTBL前面的部分是和父类的VTBL一致的，只是它们指向的内容不一样而已。想想也是，如果我们把子类的指针赋给父类的指针再调用虚函数，此时编译器将按照父类中虚函数的偏移量来进行函数调用，如果子类的VTBL中的函数指针意义和父类不一致，那么调用自然失败。
因而可以得出结论，一个类的VTBL的大小等于父类的VTBL大小加上这个类中定义的虚函数个数。而非虚函数则与VTBL没有任何关系。
1.1.3   多重继承时VTBL的内容
将上面的代码改为多重继承：
class CParentA
{
public:
     int parenta_a;
     int parenta_b;
public:  
     virtual void parenta_f1() {}
     virtual void parenta_f2() {}
public:  
     void parenta_f3() {}
     void parenta_f4() {}
};
class CParentB
{
public:
     int parentb_a;
     int parentb_b;
public:  
     virtual void parentb_f1() {}
     virtual void parentb_f2() {}
public:  
     void parentb_f3() {}
     void parentb_f4() {}
};
class CChild : public CParentA, public CParentB
{
public:
     int child_a;
     int child_b;
public:       // 子类的函数
     virtual void child_f1() {}
     virtual void child_f2() {}
public:  // 不可重载的函数
     void child_f3() {}
     void child_f4() {}
public:       // 重载父类A的函数
     virtual void parenta_f2() {}
     virtual void parenta_f1() {}
public:       // 重载父类B的函数
     virtual void parentb_f2() {}
     virtual void parentb_f1() {}
};
CChild child, *pchild;
我们只看子类的VTBL，从前面的文章可以知道，子类将有两个VTBL，其第一个VTBL指针值为0x0041685c，看它的内容：
0x0041685C  49 12 41 00 44 12 41 00 b3 11 41 00 0a 10 41 00  I.A.D.A...A...A.
0x0041686C  00 00 00 00 00 00 00 00 54 76 41 00 f0 10 41 00  ........TvA...A.
它有四个指针：
00411249 E9 B2 06 00 00   jmp         CChild::parenta_f1 (411900h) 
00411244 E9 67 06 00 00   jmp         CChild::parenta_f2 (4118B0h) 
004111B3 E9 78 06 00 00   jmp         CChild::child_f1 (411830h) 
0041100A E9 61 08 00 00   jmp         CChild::child_f2 (411870h) 
和单继承时一样。
再看第二个VTBL的内容，其指针为0x00416850：
0x00416850  3a 12 41 00 35 12 41 00 a0 75 41 00 49 12 41 00  :.A.5.A..uA.I.A.
它有两个指针：
0041123A E9 41 05 00 00   jmp         CChild::parentb_f1 (411780h) 
00411235 E9 06 07 00 00   jmp         CChild::parentb_f2 (411940h) 
紧接着这两个指针之后就是第一个VTBL的内容。在这二者之间还有一个指针0x004175a0，猜想和type info有关，因为将编译选项里的“生成运行时类型信息”关闭后这个指针将为NULL。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/lights_joy/archive/2009/09/11/4542512.aspx](http://blog.csdn.net/lights_joy/archive/2009/09/11/4542512.aspx)
