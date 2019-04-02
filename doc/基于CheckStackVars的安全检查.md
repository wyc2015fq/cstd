# 基于CheckStackVars的安全检查



在VS2008下，函数的栈空间里如果存在数组，就会自动加上CheckStackVars检查，顾名思义，就是用来检查局部数据是否访问越界。相对来说，这种检查只能起到一定的作用，并不会所有越界访问都能检查到，根据后面的原理介绍会了解到这点。既然是检查局部的，那么在函数内定义的static类型数组或者函数外部的全局数组并不会采用此检查，既然是检查数组，那么如果函数内没有局部数组时，此检查也不会存在。

 

首先来看一个简单的例子，验证这个检查的存在：

```cpp
void TestVars( void )
{
    int bf = 0xeeeeeeee;
    char array[10] = { 0 };
    int bk = 0xffffffff;
    
    strcpy( array, "masefee" );
}

int main( void )
{
    TestVars();
    return 0;
}
```

 

在这个例子中，存在一个数组array，这里刻意定义了另外两个变量，用于看这两个变量与数组array的内存分布情况。这样就能清晰的了解到CheckStackVars这个检查的原理。然后来看看Debug下，TestVars函数内部的3个局部变量的内存分布情况。断点打在strcpy这句上，分布如下：

ff ff ff ff cc cc cc cc **cc cc cc cc** 00 00 00 00 00 00 00 00 00 00 **cc cc cc cc cc cc** cc cc cc cc ee ee ee ee  

   bk                                                                   array                                                                            bf

 

上面的关系已经很明确了，我们发现，在C++的代码中看，bf、array、bk三者在内存分布上应该是连续的，紧挨着的。但是这里并不是这样的，看看bf和array之间居然像个10个字节之远。原因在于，在VS2008的debug版本下，局部变量之间并不是连续存放在栈内存里的，而是以4字节对齐的方式，前后都会有保护字节的。这里的保护字节占4个字节，值为0xcc，很明显这是汇编指令int 3中断的代码字节。因此这里bk和bf变量前后都会有4个字节的0xcc。上面绿色的部分就是，在数组array两端也有4字节的0xcc。上面黑色加粗的部分即是，array数组一共占10字节，要以4字节对齐，所以要补两字节，因此多了两个0xcc，因此导致bf和array之间相隔10字节。上面array后面紧挨着的本应该是两个0xcc，用于补充对齐。这里故意标识到后面去了。这里这样标识的意图是为了说明CheckStackVars这个检查的原理。

 

好了，清楚了内存分布情况，那么CheckStackVars在什么时间执行检查的呢，在C++代码上并不能显示的看到，于是来翻翻TestVars函数的反汇编代码：

 

TestVars:
004113B0  push        ebp  
004113B1  mov         ebp,esp 
004113B3  sub         esp,0F0h 
004113B9  push        ebx  
004113BA  push        esi  
004113BB  push        edi  
004113BC  lea         edi,[ebp-0F0h] 
004113C2  mov         ecx,3Ch 
004113C7  mov         eax,0CCCCCCCCh 
004113CC  rep stos    dword ptr es:[edi] 
**004113CE  mov         eax,dword ptr [___security_cookie (417004h)]** 
004113D3  xor         eax,ebp 
004113D5  mov         dword ptr [ebp-4],eax 
004113D8  mov         dword ptr [ebp-0Ch],0EEEEEEEEh 
004113DF  mov         byte ptr [ebp-20h],0 
004113E3  xor         eax,eax 
004113E5  mov         dword ptr [ebp-1Fh],eax 
004113E8  mov         dword ptr [ebp-1Bh],eax 
004113EB  mov         byte ptr [ebp-17h],al 
004113EE  mov         dword ptr [ebp-2Ch],0FFFFFFFFh 
004113F5  push        offset string "masefee" (415804h) 
004113FA  lea         eax,[ebp-20h] 
004113FD  push        eax  
004113FE  call        @ILT+160(_strcpy) (4110A5h) 
00411403  add         esp,8 
00411406  push        edx  
**00411407  mov         ecx,ebp** 
00411409  push        eax  
0041140A  lea         edx,[ (**411438h**)] 
00411410  call        @ILT+130(@_RTC_CheckStackVars@8) (411087h) 
00411415  pop         eax  
00411416  pop         edx  
00411417  pop         edi  
00411418  pop         esi  
00411419  pop         ebx  
0041141A  mov         ecx,dword ptr [ebp-4] 
0041141D  xor         ecx,ebp 
0041141F  call        @ILT+25(@__security_check_cookie@4) (41101Eh) 
00411424  add         esp,0F0h 
0041142A  cmp         ebp,esp 
0041142C  call        @ILT+320(__RTC_CheckEsp) (411145h) 
00411431  mov         esp,ebp 
00411433  pop         ebp  
00411434  ret              
00411435  lea         ecx,[ecx] 
**00411438**  db          01h  
00411439  db          00h  
0041143A  db          00h  
0041143B  db          00h  
0041143C  db          40h  
0041143D  db          14h  
0041143E  db          41h  
0041143F  db          00h  
00411440  db          e0h  
00411441  db          ffh  
00411442  db          ffh  
00411443  db          ffh  
00411444  db          0ah  
00411445  db          00h  
00411446  db          00h  
00411447  db          00h  
00411448  db          4ch  
00411449  db          14h  
0041144A  db          41h  
0041144B  db          00h  
0041144C  db          61h  
0041144D  db          72h  
0041144E  db          72h  
0041144F  db          61h  
00411450  db          79h  
00411451  db          00h 

 

 

从TestVars的反汇编代码可以清楚的看到，黑色加粗的部分就是前一篇博文介绍的，在本篇注意看在strcpy调用之后，又调用了_RTC_CheckStackVars函数，这是一个什么样的函数？先来看看他的原型：

```cpp
 void   __fastcall _RTC_CheckStackVars( void *_Esp, _RTC_framedesc *_Fd );
```

这是一个fastcall函数，因此两个参数都是通过寄存器进行传递的。第二个参数是一个结构体类型，再来看看这个结构体的定义：

```cpp
typedef struct _RTC_framedesc
{
    int varCount;                            //  要检查的数组的个数
    _RTC_vardesc *variables;        //  要检查的数组的相关信息

} _RTC_framedesc;

//这个结构体定义在rtcapi.h头文件中的，_RTC_vardesc 也是一个结构体类型，看看定义：

typedef struct _RTC_vardesc
{
    int addr;                                   //  数组的首地址相对于EBP的偏移量
    int size;                                    //  数组的大小字节数
    char *name;                             //  数组的名字
} _RTC_vardesc;

//以上面的例子来填充这个结构体之后，结构体的数据就是：

_RTC_framedesc.varCount = 1;
_RTC_vardesc->addr          = array - EBP;  // 这里array在低地址，所以addr最终为负
_RTC_vardesc->size           = 10;
_RTC_vardesc->name         = "array";
```

 好了，这下清楚了信息的存储，再回到上面的反汇编代码，在调用_RTC_CheckStackVars函数之前，注意红色粗体的一句指令，将ebp赋值给了ecx寄存器，再将**411438h**这个地址值赋值给了edx，由于_RTC_CheckStackVars函数是fastcall，因此通过这两个寄存器进行传递参数，而不是push操作。ecx就是保存的TestVars函数的栈帧，edx这个地址有点奇怪，本来是应该传递_RTC_framedesc结构指针的，难道这个**411438h**地址值就是_RTC_framedesc结构体变量所在的内存地址？从上面的反汇编代码可以看到，下面从**411438h**地址开始，多了一段奇怪的数据，本应该函数下面不会有这么一段数据的，在Debug下大多数情况都是0xcc填充的。咱们仔细观察下这段数据，或者直接将**411438h**这个地址值copy到内存窗口里看：

0x00411438  **01 00 00 00** 40 14 41 00 **e0 ff ff ff** 0a 00 00 00 **4c 14 41 00** 61 72 72 61 79 00

 

看看上面的数据，是不是就是_RTC_framedesc结构应该有的数据？答案是肯定的，红色的部分就是_RTC_framedesc.variables指针的值，指向的位置就是紧跟其后，这是编译器故意这么处理的。当然可以是其它地方。这是编译器直接把这些信息记录在代码段的，并且紧跟在所记录的函数代码之后。因此不要误认为这些信息是在程序执行期间才写进去或填充的_RTC_framedesc结构。

 

了解到这里，发现整个规则都是有理有据的，并且设计都是很良好的。也能又一次感受MS的伟大。呵呵，废话了！

 

上面既然将两个参数都给了_RTC_CheckStackVars函数，再来看看此函数内部是怎么检测的，看看此函数的反汇编： 

_RTC_CheckStackVars:
00411500  mov         edi,edi 
00411502  push        ebp  
00411503  mov         ebp,esp 
00411505  push        ecx  
00411506  push        ebx  
00411507  push        esi  
00411508  push        edi  
00411509  xor         edi,edi                       // 清零
0041150B  mov         esi,edx                     // 将_RTC_framedesc结构指针赋值给esi
0041150D  cmp         dword ptr [esi],edi   // 比较varCount是否为0，if( _Fd->varCount != 0 )
0041150F  mov         ebx,ecx                     // 将TestVars的栈帧赋值给ebx
00411511  mov         dword ptr [i],edi       // 这里的i应该是循环变量，将数组的个数赋值给i，i = _Fd->varCount ;
00411514  jle         _RTC_CheckStackVars+58h (411558h) 
00411516  mov         eax,dword ptr [esi+4]       //  +4之后就是_RTC_framedesc.variables指针
00411519  mov         ecx,dword ptr [eax+edi]   //  _RTC_vardesc->addr了，就是数组的首地址相对于TestVars的EBP的偏移量
0041151C  add         eax,edi                              // 将eax定位到_RTC_vardesc结构首地址
0041151E  cmp         dword ptr [ecx+ebx-4],0CCCCCCCCh  //  [ecx+ebx-4]等价于ebp-addr-4，也就是array的前面4个保护字节
00411526  jne         _RTC_CheckStackVars+36h (411536h) //  如果不等于0xcccccccc就报错_RTC_StackFailure 
00411528  mov         edx,dword ptr [eax+4]     // eax+4就是_RTC_vardesc->size，表示数组的大小
0041152B  add         edx,ecx                             // ecx当前是偏移量，加上size后就是array数组尾部相对于ebp的偏移量
0041152D  cmp         dword ptr [edx+ebx],0CCCCCCCCh  // edx+ebx即是数组array尾部的后4个保护字节，然后比较
00411534  je          _RTC_CheckStackVars+4Ah (41154Ah) 
00411536  mov         eax,dword ptr [esi+4]      // esi+4为_RTC_framedesc.variables指针
00411539  mov         ecx,dword ptr [eax+edi+8] // eax+edi+8即是_RTC_vardesc->name，用于报错提示
0041153D  mov         edx,dword ptr [ebp+4] 
00411540  push        ecx    // 传入越界的数组名
00411541  push        edx   // 传入EBP+4的地址，此地址正是_RTC_CheckStackVars的返回地址，用于定位
00411542  call        _RTC_StackFailure (4110CDh) // 调用此函数后，弹出异常MessageBox，提示哪个数组越界
00411547  add         esp,8 
0041154A  mov         eax,dword ptr [i]   // 存在多个数组需要检查时有用
0041154D  inc         eax  
0041154E  add         edi,0Ch                 // 定位到下一个_RTC_vardesc结构
00411551  cmp         eax,dword ptr [esi] 
00411553  mov         dword ptr [i],eax 
00411556  jl          _RTC_CheckStackVars+16h (411516h)  // 循环
00411558  pop         edi  
00411559  pop         esi  
0041155A  pop         ebx  
0041155B  mov         esp,ebp 
0041155D  pop         ebp  
0041155E  ret

 

以上过程稍微解析得有点复杂，其主要原理就是读取_RTC_vardesc结构，挨个对每个数组进行前后边界检查，如果发生更改，则调用_RTC_StackFailure函数，最后弹出错误信息框，信息如：

Run-Time Check Failure #2 - Stack around the variable 'array' was corrupted.

 

这里需要说明一点，如果存在多个数组需要检查时，每个数组的name是紧挨着的，同时紧接着跟在多个_RTC_vardesc结构之后，内存分布如下：

 

[数组个数, _RTC_vardesc地址] [ 多个_RTC_vardesc结构（数组）][ 每个数组的name]

 

这些位置分布都是编译器直接写在代码里的。

 

这样就能实现简单的边界检查了，前面提到了，这种检查只是会检查前后边界，如果在程序中越界访问，但是没有修改或者写的值就是边界检查的值0xcccccccc，那也不会检测出代码已经有越界隐患。因此最主要的还是要小心谨慎。编译器总不能为我们做所有的事情。以上过程会在栈内存里加上边界检查值，所以在Debug版本下是比较实用的。在Release下不会这么浪费空间，因此越界就显得更加危险了。

 

从上面的分析过程来看，可以写出_RTC_CheckStackVars函数的伪代码，如下：

 

```cpp
typedef struct _RTC_vardesc   
{  
    int addr;  
    int size;  
    char* name;  
} _RTC_vardesc;

typedef struct _RTC_framedesc   
{  
    int varCount;  
    _RTC_vardesc* variables;  
} _RTC_framedesc;  

void __fastcall _RTC_CheckStackVars( void* _Esp, _RTC_framedesc* _Fd )  
{
    if ( _Fd->varCount == 0 )
        return;
  
    int _RetAddr = 0;
    __asm
    {  
        mov eax, ebp  
        add eax, 4  
        mov _RetAddr, eax   // 保存返回地址  
    }  
  
    int i = 0;  
    while ( i < _Fd->varCount )  
    {  
        char* pAddr = ( char* )_Esp + _Fd->variables[i].addr - 4;  
        if ( *( int* )pAddr != 0xcccccccc )  
            __asm int 3  // _RTC_StackFailure( _RetAddr, _Fd->variables[i].name );  
  
        int ofs = _Fd->variables[i].addr + _Fd->variables[i].size;  
        pAddr = ( char* )_Esp + ofs;  
  
        if ( *( int* )pAddr != 0xcccccccc )  
            __asm int 3  // _RTC_StackFailure( _RetAddr, _Fd->variables[i].name );
        ++i;  
    }  
}
```

 

这段代码可以直接通过编译，并起到相应的检查功能，上面检查失败我这里暂时使用的__asm int 3进行中断，后面的注释是真正的_RTC_CheckStackVars函数调用的错误函数，_RTC_StackFailure用于弹出错误信息和定位调试器的光标到这个返回地址。

 

以下代码是用于[测试](http://lib.csdn.net/base/softwaretest)这段伪代码的功能：

```cpp
void TestVars( void )  
{  
    int _EBP = 0;  
    __asm mov _EBP, ebp   
  
    int b = 0xeeeeeeee;  
    char array1[10] = { 0 };  
    char array2[10] = { 0 };  
    int c = 0xffffffff;  
  
    //array1[ 10 ] = 0;  
    //array2[ 10 ] = 0;  
    strcpy( array, "masefee" );  
  
    char* name[ 2 ] = { "array1", "array2" }; // 存放名字  
  
    _RTC_vardesc   vdesc[2];  
    vdesc[0].addr = ( int )array1 - _EBP;   // 求得与EBP之间的偏移, 注意:array1处于低地址, addr的值为负  
    vdesc[0].name = name[0];               // 名字的地址  
    vdesc[0].size = 10;                    // 数组大小  
  
    vdesc[1].addr = ( int )array2 - _EBP;  
    vdesc[1].name = name[1];  
    vdesc[1].size = 10;  
  
    _RTC_framedesc fdesc;  
    fdesc.varCount  = 2;                   // 数组个数  
    fdesc.variables = vdesc;               // 各个数组的检测信息  
  
    _RTC_CheckStackVars( ( void* )_EBP, &fdesc );  
}  
  
int main( void )  
{  
    TestVars();  
    return 0;  
}
```



 

上面的代码是合法的，调用了检查函数之后没有任何的越界访问，如果要测试失败的情况，则将：

```cpp
//array1[ 10 ] = 0;
//array2[ 10 ] = 0;
```

这两句的注释取消，就会在第二个__asm int 3出中断。

 

以上就是CheckStackVars的所有原理，基于这种检查机制还能发散出很多的东西，并且也可以自己实现一套规则，在一些关键的代码处设置这道检测关卡，也是非常有用的。本文到此结束，希望大家多提意见，欢迎拍砖！

