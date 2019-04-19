# C++之字符串 - LC900730的博客 - CSDN博客
2017年06月18日 22:17:55[lc900730](https://me.csdn.net/LC900730)阅读数：111标签：[c语言](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)
个人分类：[c++](https://blog.csdn.net/LC900730/article/category/6933816)
### 一、C++中字符串
C++中字符串有2种形式，string和char *，char *与C语言中用法一致，而string是一个类，既然是一个类，就有类方法。方便我们查找、替换、删除、遍历等操作。C++中的string封装了char *，管理这个字符串，是一个char *的容器。下面简单介绍string提供的一系列字符串操作函数，查找find，拷贝copy删除erase 替换replace 插入insert等
### 1.字符串的初始化
```cpp
int main() {
    string s1="aaa";
    string s2("bbbb");
    string s3 = s2;   //拷贝构造实现的，涉及到指针需要注意
    string s4(10,'a'); 
    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;
    cout << s4 << endl;
```
### 2.字符串的遍历
```
①数组方式
    string s1 = "bacdd";
        for (int i = 0; i < s1.length(); i++) {
        cout << s1[i] << endl;
    }
②迭代器方式
    string s2 = "asssss";
        for (string::iterator it = s2.begin(); it != s2.end(); it++) {
        cout << *it << endl;
    }
```
两者访问方式有区别，主要体现在异常方面。
```
at()与[]访问方式的区别
string s1 = "asssss";
    try {
        for (int i = 0; i < s1.length()+3; i++) {
            cout << s1.at(i) << endl;
        }
    }
    catch(...){
        cout << "发生异常" << endl;    //抛出异常
    }
    try {
        for (int i = 0; i < s1.length() + 3; i++) {
            cout << s1[i] << endl;     //不抛出异常，引起程序中断
        }
    }
    catch (...) {
        cout << "发生异常" << endl;
    }
```
### 3.字符指针与string的转换
返回内存首地址 
s1.c_str() 
把string拷贝到char *指向的内存空间的操作。 
s1.copy(buf,3,0);//将s1拷贝到buf数组中，从位置0(默认)开始，长度是3，不会再字符串末尾加’\0’。
### 4.字符串的拼接
①：s3=s1+s2; 
②：s2.append(s1); //将s1拼接到s2后面
### 5.查找与替换
```
find(char c,int pos=0);
find(const char *)
s1.replace(offindex,3,"WBM");查找到后删除然后替换
s2.insert(0,"AAA");
```
### 6.字符的删除
//删除pos开始的n个字符，并且返回删除之后的字符串 
string &erase(int pos=0,int n=npos); 
通过迭代器删除
```
string s1="hello hello2 hellol";
string::iterator it=find(s1.begin(),s1.end(),'l');
if(it!=s1.end()){
    s1.erase(it);//将第一个hello中的'l'删除，只删除了一个
}
//全部删除
s1.erase(s1.begin(),s1.end());
//区间删除和插入
string s="hello hello2 hello3";
```
## 7.数组元素
函数返回值当左值，必须是返回引用
```cpp
vector<int> v1;
    cout << v1.size() << endl;
    v1.push_back(1);
    v1.push_back(3);
    v1.push_back(5);
    cout << v1.size() << endl;
    cout << v1.back() << endl;
    v1.front()=11;   //front（）返回的是引用
    v1.pop_back();
    v1.front()
```
### 8.插入
```
//头插入法
string &insert(int pos,string &s);
string &insert(int pos,char *);
s1.insert(0,"AAA");
//尾部插入
s1.insert(s.length(),"CCC");
```
