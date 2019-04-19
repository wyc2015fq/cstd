# C++小知识点 - LC900730的博客 - CSDN博客
2017年07月02日 10:25:13[lc900730](https://me.csdn.net/LC900730)阅读数：183标签：[c语言](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)
个人分类：[c++](https://blog.csdn.net/LC900730/article/category/6933816)
```cpp
#include<iostream>
using namespace std;
class C1{
public:
    int i;
    int j;
    int k;
protected:
private:
};
class C2{
public:
    int i;
    int j;
    int k;
    static int m;
public:
    int getK(){
        return k;
    }
    void setK(int val){
        k=val;
    }
protected:
};
struct S1{
    int i;
    int j;
    int k;
};
struct S2{
    int i;
    int j;
    int k;
    static int m;
};
int main(){
    cout<<sizeof(C1)<<endl;   //12
    cout<<sizeof(C2)<<endl;   //12
    cout<<sizeof(S1)<<endl;   //12
    cout<<sizeof(S2)<<endl;   //12
   //cout<<Test::getC();
    getchar();
    return 0;
}
```
C++编译器是
类对象中成员变量和成员函数是分开存储的； 
成员变量： 
    普通成员变量：存储于对象中，与struct变量有相同的内存布局和对齐方式； 
    静态成员变量：存储于全局数据区； 
成员函数：存储于代码段中； 
问题：很多对象共用一块代码？代码是如何区分具体对象的？
this指针
如果类成员的形参和类的属性，名字相同，那么通过this指针来解决； 
类成员函数可以通过const修饰，请问const修饰的是谁？
const的位置； 
①void const Opvar(){} 
②const void Opvar(){} 
③void Opvar()const{}
void OpVar(int a,int b)const  //void OpVar(const Test *this,int a,int b) const 
const指的是这个this，这个const位置无论放在哪里都是一样的，保证这个this指向的内存空间不能被修改，只读属性 
“`C 
class Test{ 
public: 
    Test(int a,int b){ 
        this->a=a; 
        this->b=b; 
    } 
    void printf(){ 
        cout<<”a:”<
