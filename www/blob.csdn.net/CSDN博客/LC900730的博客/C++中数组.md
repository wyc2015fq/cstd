# C++中数组 - LC900730的博客 - CSDN博客
2017年09月24日 20:16:00[lc900730](https://me.csdn.net/LC900730)阅读数：128
数组形参 
数组的2个性质：1不允许拷贝数组；2。使用数组时(通常)会将其转换成指针。 
因为不能拷贝数组，所以无法以值传递的方式使用数组参数。因为数组参数会被转换为指针，所以当我们为函数传递一个数组当时候，实际上传递的是指向数组首元素的指针。
```
//形式不同，但是print函数是等价的
void print(const int *);
void print(const int []);
void print(const int [10]);
```
尽管表现形式不同，但是上面的三个函数是等价的；每个函数唯一参数是const int *类型的。当编译器处理对printf函数的调用时候，会检查传入的参数是否是const int *类型；
```
int i=0，j[2]={0,1};
print(&i);  //正确，&i的类型是int *;
print(j); //正确，j转换成int *并且指向j[0];
```
C++中其实没有多维数组，所谓多维数组是数组的数组。
和所有数组一样，当将多维数组传递给函数时候，真正传递的是指向数组首元素的指针。 
因为我们处理的是数组 数组，所以首元素本身就是一个数组，指针就是一个指向数组的指针。
如：
```
void print(int matrix[][10],int rowSize){
//编译器会忽略第一个维度，所以最好不要把它包含在形参列表内。
//matrix看起来像是二维数组，实际上形参是指向含有10个整数的数组的指针。
}
```
二维数组中；
```
int i[2][3]={1,2,3,4,5};
//p是一个指向3个int的数组的指针
//初始化为二维数组i的第一行。
//这个3必须与i的列维数相等
int (*p)[3]=&i[0];
p++;
cout<<(*p)[0];   //4
```
### 返回引用
函数返回引用，那么仅仅是返回它所引对象的一个别名。
```
const string &shorterString(const string &s1,const string &s2){
    return s1.size()<=s2.size()?s1:s2;
}
```
返回的是引用，不是真正拷贝string对象。
### 返回局部对象的引用或者指针
//要想确保返回值安全，我们不妨提问：引用所引的是在函数之前已经存在的哪个对象
```
const string &mainp(){
    string ret;
    if(!ret.empty())
        //返回的是局部对象的引用
        return ret;
    else
    //"Empty"字符串字面量转换成一个临时string变量，该对象也是局部的。当函数结束时候，会释放
        return "Empty";   //"Empty"是一个局部临时量
    //因此都指向了不可用的内存空间
}
```
## 引用返回左值
调用一个返回引用的函数得到左值。
```cpp
char &get_val(string &str,string::size_type ix){
    return str[ix];
} 
int main(){
    string s("a value");
    cout<<s<<endl;
    get_val(s,0)='A';
    cout<<s<<endl;
    return 0;
}
```
### typedef误区
```
//以前一直以为是给int 取别名
typedef int arrT[10]:含义是arrT是一个类型别名，表示的类型是含有10个整数的数组。
```
## 返回数组指针
因为数组不能被拷贝，所以函数不能返回数组。不过函数可以返回数组的指针或引用。
```cpp
typedef int arrT[10];     <===>    using arrT=int [10];
arrT* func(int i);
arrT是含有10个整数的数组的别名，将返回类型定义成数组的指针。
```
## 声明一个返回数组指针的函数
```
int arr[10];
int *p1[10];
int (*p2)[10]=&arr;
//如果想定义一个返回数组指针的函数，则数组的维度必须跟在函数名字之后
。但是函数的形参列表页跟在函数名字之后且形参列表应该优先于数组维度。因此返回数组指针的函数形式如下
Type (*function(parameter_list)[dimension]);
```
### 解析一个函数
```
int (*func(int i))[10];
//逐层解析
//1.func(int i):func函数需要一个int类型的参数
//2.(*func(int i)):对函数调用的结果执行解引用
//3.(*func(int i))[10]:解引用func的调用将得到一个大小是10的数组。
//4.int (*func(int i))[10]表示数组中元素是int类型
```
### C++11的尾置返回类型
““CPP 
auto func(int i)->int (*)[10]; 
func接受一个int类型的实参，返回一个指针，该指针指向含10个int的数组
```
<h3>使用decltype</h3>
如果我们知道函数返回的指针指向哪个数组，就可以使用decltype关键字声明返回类型。
```CPP
int odd[]={1,3,5,7,9};
int even[]={0,2,4,6,8};
//arrPtr使用关键字decltype表示它的返回类型是个指针，指针所指的对象与odd类型一致。
//arrPtr返回一个指向5个int的数组的指针。
decltype(odd) * arrPtr(int i){
//但是decltype结果是个数组，要想表示arrPtr返回指针必须在函数声明前面加上*符号
    return (i%2)? &odd : &even;  //返回一个指向数组的指针
}
```
