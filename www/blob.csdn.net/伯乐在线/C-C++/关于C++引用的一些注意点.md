# 关于C++引用的一些注意点 - 文章 - 伯乐在线
原文出处： [fusae的博客](http://www.cnblogs.com/fusae-blog/p/4471787.html)
C++的引用首先跟指针的最大区别就是引用不是一个对象，而指针是一个对象；其次引用在其定义时就要初始化，而指针可以不用。

C++
```
int    val = 42;
int    &rval = val;
```
此时rval就绑定了val，其实就是rval就是val的一个别名。你修改了两个其中的一个，其值都会改变。
因为引用在一开始就初始化了，所以一个引用只能引用一个变量。还有，引用只能引用对象，也就是有地址的，不能是一个常数或者表达式。而且类型要匹配。

C++
```
int    &rval = 10;    
//error: initializer must be an object
double    dval = 3.14;
int    &rdval = dval;   
 //error: initializer must be an int object
```
References to const　　常量引用
不同于非常量的引用，常量引用所引用的对象不能修改


```
const    int    ci = 1024;
const    int    &r1 = ci;
r1 = 42;    
//error: r1 is a reference to const
int    &r2 = ci;    
//error: nonconst reference to a const object
```
因为ci是一个常量，所以我们不能直接用一个引用来引用ci，因为我们不能修改ci。
上面提到引用要引用正确的自身的类型，但是常量引用可以引用一个非const的对象，一个数，或者表达式。

C++
```
int    i = 42;
const    int    &r1 = i;    　　 //ok
const    int    &r2 = 42;　　　　//ok　
const    int    &r3 = r1 * 2;　 //ok
int    &r4 = r * 2;    
//error: 非常量引用不能引用一个表达式
```
让我们想想这是为什么？


```
double    dval = 3014;
const    int    &ri = dval;    //ok
```
其实编译器帮我们多做了一步

C++
```
double    dval = 3014;
const    int    temp = dval;    
//创建一个暂时的常量对象来存放dval             
const    int    &ri = temp;      
//将引用到这个暂时的常量
```
正因为有这个无名的中转存量，所以常量引用才可以引用数，表达式，还有不同类型的对象。
那为什么非常量引用就不能这样呢？
想想，刚才说的编译器帮我们弄了一个中转对象，其实我们引用是引用它，修改也修改它， 但它是无名的，也就是找不到地址，也无法找着。修改了也没用，我们是要修改dval（在上面列子中）。
所以说，只有常量引用才可以引用数字，表达式，不同类型的对象。因为我们不打算修改它，所以那个中转变量真的只是负责传递值，让常量引用初始化而已。
最后一个例子

C++
```
int    i = 42;
int    &r1 = i;
const    int    &r2 = i;
r1 = 0;    //ok
r2 = 0;    //error
```
这样非常量引用和常量引用都引用一个值是可以的。非常量引用就不用说了，跟对象绑在一起，是别名，修改引用的同时也就修改了对象本身的内容。而常量引用，就是跟上面一样，编译器帮我们创建了一个无名的中转变量来储存，其实也就是赋值给常量对象初始化。你不能修改它就是了。
