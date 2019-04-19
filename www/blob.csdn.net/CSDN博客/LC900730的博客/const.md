# const - LC900730的博客 - CSDN博客
2017年05月30日 10:33:54[lc900730](https://me.csdn.net/LC900730)阅读数：188标签：[c语言](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)
个人分类：[c++](https://blog.csdn.net/LC900730/article/category/6933816)
## C语言中的const
C++中的const可以直接初始化数组
```
const int num=5;
int *p=(int *)#   
*p=4;//编译通过,但是不会执行
```
## const_cast
## 对数组的引用
```
int a[10]={};
int(&ra)[10]=a;
int b[2][5]={X,X,X,X,X,X,X,X,X,X,X};
int (&rb)[2][5]=b;
std::cout<<a<<ra<<std::endl;   //输出相同的值
```
## 对函数指针的引用
```cpp
int(*p)(int a, int b)(jia);
    std::cout << p(3, 1) << std::endl;
    int(*&rp)(int a, int b)(p);//引用函数指针
    //p是(int *)()，那么对p的引用就是(int *&rp)()
    rp = jian;    //<=====>p=jian
    std::cout << p(1, 2);
```
## 改变函数指针经常用于劫持
```cpp
void change(int (*&rp)(int,int)){
    rp=jian;
}
int main(){
    int(*p)(int a,int b)(jia);
    change(p);
    std::cout<<p(1,2);//-1
}
```
## 返回函数指针的引用
```
//返回类型是函数指针的引用。
int (*& changep(int (* &rp)(int,int)))(int ,int) {  //函数指针的引用
    rp = jian;
    return rp;
}
int (*p)(inta,int b)=jia;
int (*&ra)(int ,int)=p;
ra=changep(p);
```
在C++中结构体中函数是跳过的，即结构体中的函数放在代码区，sizeof(struct XX)不计入大小
## sizeof(class)
```cpp
class MyClass {
    int a;
    double h;
    int b;
};
class MyClass2{
    int a;
    int b;
    double h;
};
int main(){
    std::cout<<sizeof(MyClass);   //24
    std::cout<<sizeof(MyClass1);  //16
}
```
直接sizeof(引用)，就是求引用的数据大小； 
引用变量占4个字节； 
int a=10; 
sizeof(&a);    //4(x86)
## 左值与右值
右值是为了节约内存
## 操作符重载
### 宽字符
C++中wcout输出宽字符： 
std::locale loc(“”); 
std::wcout.imbue(loc);
## inline与define
```
#define GETX3(N) N*N*N
std<<cout<<GETX3(1+2)<<std::endl;  //7    1+2*1+2*1+2
std::cout<<GETX3((1+2))<<std::endl;     3*3*3;
std::cout<<GETX3((1.1+2))<<std::endl;     3*3*3;
```
inline int getX3(int x);   //保证类型安全
inline只是对编译器的建议
- inline不能有递归；
- 不能保护静态数据；
- 不能包含循环；
- 不能包含switch和goto语句；
- 不能包含数组
否则加了inline编译器也会将其当做普通数组
