# C++ 获取类成员函数地址方法 浅析 - DoubleLi - 博客园






C语言中可以用函数地址直接调用函数:
- 



void print ()


- 



{


- 



printf ("function print");


- 



}


- 



typdef void (*fun)();


- 



fun f = print;


- 



f();



C++中类非静态成员函数必须通过实例去调用,C++中类成员函数调用:
- 



class test


- 



{


- 



public:


- 



void print ()


- 



{


- 



printf ("function print");


- 



}


- 



};



我们同样可以通过定义函数指针来调用如下:
- 



typedef void (test::*fun)();


- 



fun f = &test::print;


- 



test t;


- 



(t.*f)();



如果我们想取函数地址怎么办,直接强转会报错

```cpp
unsigned int a = reinterpret_cast<unsigned int>(f);//编译错误
```

http://bbs.pediy.com/showthread.php?t=47835

这里给出了答案.


- 



char buf [32] = {0};


- 



sprintf(buf , "%u", f);


- 



int a = atoi( buf);



其实我们知道类成员函数只是函数第一个参数是this指针,但是this指针不是通过普通函数参数压栈方式,而是放进ecx中。









