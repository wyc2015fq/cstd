# 剑指offer经典题（利用C++的特性） - youbingchen的博客 - CSDN博客





2016年05月24日 20:50:26[youbingchen](https://me.csdn.net/youbingchen)阅读数：409








1.题目：求1+2+…..+n,要求不能使用乘除法、for、while、if、else、switch、case等关键字 

解法一:利用构造函数

```cpp
class Temp
{
   public:
      Temp() {++N;Sum+=N}
      //static void Reset(){N=0;Sum=0;}
      static unsigned int GetSum(){ return Sum;}
   private:
      static unsigned int N;
      static unsigned int Sum;
};
unsigned int Temp::N=0;
unsigned int Temp::Sum=0;
unsigned int Sum_Solution1(unsigned int n)
{
  //Temp::Reset();
  Temp *a=new Temp[n];

  delete []a;
  a=NULL;
  return Temp::GetSum();

}
```

解法二：利用虚函数求解 

如何把一个数值变量n转化为布尔值。如果对n连续做两次反运算，即!!n,那么非零的n转换成true,0转换成false

```cpp
class A;
A* Array[2];
class A
{
   public:
      virtual unsigned int Sum(unsigned int n)
        {
             return 0;
        }
};
class B:public A
{
   public: 
    virltual unsigned int Sum(unsigned int n)
    {
        return Array[!!n]->Sum(n-1)+n;
    }
};
int Sum_Solution2(int n)
{
   A a;
   B b;
   Array[0]=&a;
   Array[1]=&b;

   int value=Array[1]->Sum(n);

   return value;
}
//这种思路就是用虚函数来实现函数的选择，当n不为零，调用B::Sum,当n等于0时，调用函数A::Sum
```

解法三：利用函数指针求解 

在C语言的编程环境中，不能使用虚函数，此时可以用函数来模拟

```cpp
typedef unsigned int (*fun) (unsigned int);
unsigned int Solution3_Teminator(unsigned int n)
{
     return 0;
}
unsigned int Sum_Solution3(unsigned int n)
{
    static fun f[2]={Solution3_Teminator,Sum_Solution3};
    return n+f[!!n](n-1);
}
```

解法四：利用模板类型求解

```cpp
template <unsigned int n> struct Summ_Solution4
{
  enum Value{N=Sum_Solution<n-1>::N+n};
};

template <>struct Sum_Solution4<1>
{
  enum Value{N=1};
};
```

2.题目:不用加减乘除做加法运算 
**异或和加法就差一个进位**

```cpp
int Add(int num1,int num2)
{
     int sum,carry;
     do{
         sum=num1^num2;
         carry=(num1&num2)<<1;

         num1=sum;
         num2=carry;
     }while(num2!=0);
     return num1;
}
```

3.题目：用C++设计一个不能被继承的类

解法一：把构造函数设为私有函数

```cpp
class SealedClass1
{
    public:
       static SealedClass1 *GetInstance()
       {
         return new SealedClass1();
       }
       static void DeleteInstance(SealedClass1* pInstance)
       {
          delete pInstance;
       }
   private:
    SealedClass1(){}
    ~SealedClass1(){}
};
```

解法二:利用虚拟继承:实现一个与一般的类型相比除了不能被继承之外其他用法一样的类型，

```cpp
template<typename T>class MakeSealed
{
   friend T;
   private:
    MakeSealed(){}
    ~MakeSealed(){}
}
class SealedClass2:virtual public MakeSealed<SealedClass2>
{
   public:
      SealedClass2(){}
      ~SealedClass2(){}
};
```

这个SealedClass2 使用起来和一般的类型没有区别，可以在栈上和队上创建实例，尽管类MakeSealed的构造函数和析构函数都是私有的，但由于类SealedClass2是它的友元类型，因此在SealedClass2中调用MakeSealed的构造函数和析构函数都不会引起编译错误 

  但是试图从SealClass2中继承一个类并创建它的实例时候，却不能通过编译。 

```
cpp 

 class Try:public SealedClass2 

 { 

     public: 

      Try(){} 

      ~Try(){} 

 }; 

 

 由于类SealedClass2是从类MakeSealed虚继承过来的，在调用Try的构造函数的时候，会跳过SealedClass2而直接调用MakeSealed构造函数，Try并不是MakeSealed的友元类型，因此不能调用它的私有构造函数。 

 **这种的可移植性不好，目前Gcc中还不支持模板参数类型作为友元类型**
```




