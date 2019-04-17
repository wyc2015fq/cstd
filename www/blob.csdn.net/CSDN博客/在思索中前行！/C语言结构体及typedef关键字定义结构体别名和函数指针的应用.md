# C语言结构体及typedef关键字定义结构体别名和函数指针的应用 - 在思索中前行！ - CSDN博客





2014年11月16日 19:43:24[_Tham](https://me.csdn.net/txl16211)阅读数：3378








# 结构体（struct）的初始化



```cpp
struct   autonlist   {   
        char   *symbol;   
        struct   nlist   nl[2];   
        struct   autonlist   *left,   *right;   
  };   </span>
```


 struct   autonlist   *nlists   =   0;   
 symbol，*symbol，nl[0]，nl[1]，*left，left，*right，right会初始化成什么？



 先搞清这句话的意思：

  struct   autonlist   *nlists   =   0;   
 等价于：struct   autonlist   *nlists   =   NULL;   

 这句话定义了一个autonlist结构的指针nlists，然后初始化指针为NULL，以防止被后面的语句误用（主要就是防止未对该指针赋值造成的内存错误，野指针操作）；
既然这只是对指针的初始化，还使这个指针指向相应的结构变量，当然就不存在对结构的成员进行初始化的操作。

  //*symbol，nl[0]，nl[1]，*left，*right
     //这些要自己去显示的初始化，系统默认的都是危险的，一般在用到结构体时要自己去初始化他们

# typedef定义别名


```cpp
typedef struct   autonlist   {   
        char   *symbol;   
        struct   nlist   nl[2];   
        struct   autonlist   *left,   *right;   
  }auto;  </span>
```
在使用typedef对结构体autonlist进行定义别名后，定义结构体的时候直接使用auto就可以了。

例如：

       auto *nlist; 即定义了一个结构体指针能力nlist。




# typedef定义函数指针



关于C++中函数指针的使用(包含对typedef用法的讨论) 
（一）简单的函数指针的应用。
//形式1：返回类型(*函数名)(参数表)
char (*pFun)(int); 

char glFun(int a) { return ;} 

void main() 

{ 

    pFun = glFun; 

    (*pFun)(2); 

}

       第一行定义了一个指针变量pFun。首先我们根据前面提到的“形式1”认识到它是一个指向某种函数的指针，这种函数参数是一个int型，返回值是char类型。只有第一句我们还无法使用这个指针，因为我们还未对它进行赋值。 

       第二行定义了一个函数glFun()。该函数正好是一个以int为参数返回char的函数。我们要从指针的层次上理解函数——函数的函数名实际上就是一个指针，函数名指向该函数的代码在内存中的首地址。 

       然后就是可爱的main()函数了，它的第一句您应该看得懂了——它将函数glFun的地址赋值给变量pFun。main()函数的第二句中“*pFun”显然是取pFun所指向地址的内容，当然也就是取出了函数glFun()的内容，然后给定参数为2。 
（二）使用typedef更直观更方便。
//形式2：typedef 返回类型(*新类型)(参数表)
typedef char (*PTRFUN)(int); 

PTRFUN pFun; 

char glFun(int a){ return;} 

void main() 

{ 

    pFun = glFun; 

    (*pFun)(2); 

}

      typedef的功能是定义新的类型。第一句就是定义了一种PTRFUN的类型，并定义这种类型为指向某种函数的指针，这种函数以一个int为参数并返回char类型。后面就可以像使用int,char一样使用PTRFUN了。 

      第二行的代码便使用这个新类型定义了变量pFun，此时就可以像使用形式1一样使用这个变量了。

（三）在C++类中使用函数指针。
//形式3：typedef 返回类型(类名::*新类型)(参数表)
class CA 

{ 

 public: 

          char lcFun(int a){ return; } 

}; 

CA ca; 

typedef char (CA::*PTRFUN)(int); 

PTRFUN pFun; 

void main() 

{ 

    pFun = CA::lcFun; 

    ca.(*pFun)(2); 

}

     在这里，指针的定义与使用都加上了“类限制”或“对象”，用来指明指针指向的函数是那个类的这里的类对象也可以是使用new得到的。比如： 
CA *pca = new CA; 

pca->(*pFun)(2); 

delete pca;

     而且这个类对象指针可以是类内部成员变量，你甚至可以使用this指针。比如： 

     类CA有成员变量PTRFUN m_pfun; 
void CA::lcFun2() 

{  

   (this->*m_pFun)(2); 

}

     一句话，使用类成员函数指针必须有“->*”或“.*”的调用。 







