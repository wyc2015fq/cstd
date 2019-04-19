# 深入理解C/C++函数指针 - maopig的专栏 - CSDN博客
2011年09月06日 18:56:21[maopig](https://me.csdn.net/maopig)阅读数：548标签：[function																[buffer																[fun																[null																[delete																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=fun&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)
个人分类：[c/C++](https://blog.csdn.net/maopig/article/category/847675)
#### 函数指针数组的妙用
         笔者在开发某软件过程中遇到这样一个问题，前级模块传给我二进制数据，输入参数为 char* buffer和 int length，buffer是数据的首地址，length表示这批数据的长度。数据的特点是：长度不定，类型不定，由第一个字节（buffer[0]）标识该数据的类型，共有256（28 ）种可能性。我的任务是必须对每一种可能出现的数据类型都要作处理，并且我的模块包含若干个函数，在每个函数里面都要作类似的处理。若按通常做法，会写出如下代码：
void MyFuntion( char* buffer, int length )
{
　　　　__int8 nStreamType = buffer[0];
　　　　switch( nStreamType )
　　　　{
　　　　　　　case 0:
　　　　　　　　　　　function1();
　　　　　　　　　　　break;
　　　　　　　case 1:
　　　　　　　......
　　　　　　　case 255:
　　　　　　　　　　　function255();
　　　　　　　　　　　break;
　　　　　}
}
如果按照这种方法写下去，那么在我的每一个函数里面，都必须作如此多的判断，写出的代码肯定很长，并且每一次处理，都要作许多次判断之后才找到正确的处理函数，代码的执行效率也不高。针对上述问题，我想到了用函数指针数组的方法解决这个问题。
　　函数指针的概念，在潭浩强先生的C语言程序设计这本经典的教程中提及过，在大多数情况下我们使用不到，也忽略了它的存在。函数名实际上也是一种指针，指向函数的入口地址，但它又不同于普通的如int*、double*指针，看下面的例子来理解函数指针的概念：
int funtion( int x, int y );
void main ( void )
{
 　　　int (*fun) ( int x, int y );
 　　　int a = 10, b = 20;
 　　　function( a, b );
 　　　fun = function;
 　　　（*fun）( a, b );
 　　　　……
}
　　语句1定义了一个函数function，其输入为两个整型数，返回也为一个整型数（输入参数和返回值可为其它任何数据类型）；语句3定义了一个函数指针，**与int*或double*定义指针不同的是，函数指针的定义必须同时指出输入参数，表明这是一个函数指针，并且*fun也必须用一对括号括起来**；语句6将函数指针赋值为funtion，前提条件是*fun和function的输入参数和返回值必须保持一致。语句5直接调用函数function（），语句7是调用函数指针，二者等效。
　　当然从上述例子看不出函数指针的优点，目的主要是想引出函数指针数组的概念。我们从上面例子可以得知，既然函数名可以通过函数指针加以保存，那们也一定能定义一个数组保存若干个函数名，这就是函数指针数组。正确使用函数指针数组的前提条件是，这若干个需要通过函数指针数组保存的函数必须有相同的输入、输出值。
这样，我工作中所面临的问题可以解决如下：
首先定义256个处理函数(及其实现)。
void funtion0( void );
……
void funtion255(void );
其次定义函数指针数组，并给数组赋值。
void (*fun[256])(void);
fun[0] = function0;
……
fun[255] = function();
最后，MyFunction()函数可以修改如下：
void MyFuntion( char* buffer, int length )
{
　　　　__int8 nStreamType = buffer[0];
　　　　（*fun[nStreamType]）();
}
　　只要2行代码，就完成了256条case语句要做的事，减少了编写代码时工作量，将nStreamType作为数组下标，直接调用函数指针，从代码执行效率上来说，也比case语句高。假如多个函数中均要作如此处理，函数指针数组更能体现出它的优势。
#### 函数指针与typedef
关于C++中函数指针的使用(包含对typedef用法的讨论)
**（一）简单的函数指针的应用。**
//形式1：返回类型(*函数名)(参数表)
**char (*pFun)(int);char glFun(int a){ return;}void main(){    pFun = glFun;    (*pFun)(2);}**
        第一行定义了一个指针变量pFun。首先我们根据前面提到的“形式1”认识到它是一个指向某种函数的指针，这种函数参数是一个int型，返回值是char类型。只有第一句我们还无法使用这个指针，因为我们还未对它进行赋值。
        第二行定义了一个函数glFun()。该函数正好是一个以int为参数返回char的函数。**我们要从指针的层次上理解函数——函数的函数名实际上就是一个指针，函数名指向该函数的代码在内存中的首地址。**        然后就是可爱的main()函数了，它的第一句您应该看得懂了——它将函数glFun的地址赋值给变量pFun。main()函数的第二句中“*pFun”显然是取pFun所指向地址的内容，当然也就是取出了函数glFun()的内容，然后给定参数为2。
**（二）使用typedef更直观更方便。**
//形式2：typedef 返回类型(*新类型)(参数表)
**typedef char (*PTRFUN)(int);PTRFUN pFun;char glFun(int a){ return;}void main(){    pFun = glFun;    (*pFun)(2);}**
        typedef的功能是定义新的类型。第一句就是定义了一种PTRFUN的类型，并定义这种类型为指向某种函数的指针，这种函数以一个int为参数并返回char类型。后面就可以像使用int,char一样使用PTRFUN了。
        第二行的代码便使用这个新类型定义了变量pFun，此时就可以像使用形式1一样使用这个变量了。
**（三）在C++类中使用函数指针。**
//形式3：typedef 返回类型(类名::*新类型)(参数表)
**class CA{ public:    char lcFun(int a){ return; }};CA ca;typedef char (CA::*PTRFUN)(int);PTRFUN pFun;void main(){    pFun = CA::lcFun;    ca.(*pFun)(2);}**
        在这里，指针的定义与使用都加上了“类限制”或“对象”，用来指明指针指向的函数是哪个类的,这里的类对象也可以是使用new得到的。比如：
**CA *pca = new CA;pca->(*pFun)(2);delete pca;**
        而且这个类对象指针可以是类内部成员变量，你甚至可以使用this指针。比如：
        类CA有成员变量PTRFUN m_pfun;
**void CA::lcFun2(){    (this->*m_pFun)(2);}**
        一句话，使用类成员函数指针必须有“->*”或“.*”的调用。
在调用动态库时，习惯用typedef重新定义动态库函数中的函数地址（函数指针），如在动态库（test.dll）中有如下函数：
      int   DoCase(int, long);
则，在调用动态库是有两种方法：
  1.  先声明一个与动态库中类型一致的指针函数变量：
        int (*DOCASE)(int ,long);//用于指向动态库中的DoCase函数地址
        HINSTANCE gLibMyDLL = NULL;
       gLibMyDLL = LoadLibrary("test.dll");
       if(gLibMyDLL != NULL)
         {
                   //得到函数地址
                     DOCASE = (int(*)(int,long))GetProcAddress(gLibMyDLL, "DoCase");
          }  
         //调用函数
         int s = DOCASE(1,1000);
   2.用typedef定义一个指针函数：typedef (*DOCASE)(int ,long);
         HINSTANCE gLibMyDLL = NULL;
        DOCASE _docase;
       gLibMyDLL = LoadLibrary("test.dll");
      if(gLibMyDLL != NULL)
          {
                _docase = (DOCASE)GetProcAddress(gLibMyDll, "DoCase");
         }
      //调用函数
      int s=_docase(1,1000);
