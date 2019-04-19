# C++ 类型转换详解 -- const_cast - 文章 - 伯乐在线
原文出处： [lwbeyond](http://blog.csdn.net/lwbeyond/article/details/6213382)
**一. 函数描述：**
** const_cast ( expression )**
主要是用来去掉const属性，当然也可以加上const属性。主要是用前者，后者很少用。
去掉const属性：const_case (&num)，常用，因为不能把一个const变量直接赋给一个非const变量，必须要转换。
加上const属性：const int* k = const_case(j)，一般很少用，因为可以把一个非const变量直接赋给一个const变量，比如：const int* k = j;
**二. 使用范围：**
**1. 常量指针被转化成非常量指针，转换后指针指向原来的变量(即转换后的指针地址不变)。**


```
class A    
{    
public:    
     A()    
     {    
      m_iNum = 0;    
     }    
        
public:    
     int m_iNum;    
};    
        
void foo()    
{    
    //1. 指针指向类    
    const A *pca1 = new A;    
    A *pa2 = const_cast<A*>(pca1);  //常量对象转换为非常量对象    
    pa2->m_iNum = 200;    //fine    
       
    //转换后指针指向原来的对象    
    cout<< pca1->m_iNum <<pa2->m_iNum<<endl; //200 200    
        
    //2. 指针指向基本类型    
    const int ica = 100;    
    int * ia = const_cast<int *>(&ica);    
    *ia = 200;    
    cout<< *ia <<ica<<endl;   //200 100    
}
```
**2. 常量引用转为非常量引用。**


```
class A  
{  
public:  
　　A()  
　　{  
    m_iNum = 1;  
　　}  
  
public:  
　　int m_iNum;  
};  
  
void foo()    
{    
   
　　A a0;  
　　const A &a1 = a0;  
　　A a2 = const_cast<A&>(a1);　//常量引用转为非常量引用  
      
　　a2.m_iNum = 200;    //fine    
  
　　cout<< a0.m_iNum << a1.m_iNum << a2.m_iNum << endl; //１　1 200    
}
```
**2. 常量对象(或基本类型)不可以被转换成非常量对象(或基本类型)。**


```
void foo()  
{  
 //常量对象被转换成非常量对象时出错  
 const A ca;  
 A a = const_cast<A>(ca);  //不允许  
  
 const int i = 100;  
 int j = const_cast<int>(i);  //不允许  
  
}
```
记住这种转换只是开了一个接口，并不是实质上的转换。(其实也算是实质上的转换了，只不过表达上不允许这样写)
**3. 添加const属性**


```
int main(int argc, char ** argv_)   
{  
 int i = 100;  
 int *j = &i;  
 const int *k = const_cast<const int*>(j);  
 //const int *m = j;   感觉和这样写差不多  
  
 //指的地址都一样  
 cout <<i<<","<<&i<<endl; //100, 0012FF78  
 cout <<*j<<","<<j<<endl; //100, 0012FF78  
 cout <<*k<<","<<k<<endl; //100, 0012FF78  
  
 *j = 200;  
 //*k = 200;   //error  
  
 return 0;  
}
```
**三. 总结：**1. 使用const_cast去掉const属性，其实并不是真的改变原类类型(或基本类型)的const属性，它只是又提供了一个接口(指针或引用)，使你可以通过这个接口来改变类型的值。也许这也是const_case只能转换指针或引用的一个原因吧。
2. 使用const_case添加const属性，也是提供了一个接口，来不让修改其值，不过这个添加const的操作没有什么实际的用途(也许是我认识太浅了)。
