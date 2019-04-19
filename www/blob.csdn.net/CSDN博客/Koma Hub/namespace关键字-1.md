# namespace关键字-1 - Koma Hub - CSDN博客
2019年02月11日 20:05:51[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：62
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**目录**
[例1：命名空间中的类](#%E4%BE%8B1%EF%BC%9A%E5%91%BD%E5%90%8D%E7%A9%BA%E9%97%B4%E4%B8%AD%E7%9A%84%E7%B1%BB)
[例2：命名空间中的变量](#%E4%BE%8B2%EF%BC%9A%E5%91%BD%E5%90%8D%E7%A9%BA%E9%97%B4%E4%B8%AD%E7%9A%84%E5%8F%98%E9%87%8F)
[例3：命名空间里的函数](#%E4%BE%8B3%EF%BC%9A%E5%91%BD%E5%90%8D%E7%A9%BA%E9%97%B4%E9%87%8C%E7%9A%84%E5%87%BD%E6%95%B0)
[例4：命名空间里的函数](#%E4%BE%8B4%EF%BC%9A%E5%91%BD%E5%90%8D%E7%A9%BA%E9%97%B4%E9%87%8C%E7%9A%84%E5%87%BD%E6%95%B0)
[例5：匿名namespace](#%E4%BE%8B5%EF%BC%9A%E5%8C%BF%E5%90%8Dnamespace)
[例6：using](#%E4%BE%8B6%EF%BC%9Ausing)
### 例1：命名空间中的类
```cpp
#include<iostream>
#include<string>
#include<cstring>
#include<cmath>
#include<complex>
#include<algorithm>
#include<vector>
#include<utility>
#include<cstddef> /*size_t*/
#include<bitset>
#include<assert.h>
#include<cstdlib>//abort()
using namespace std;
namespace rong{
    template<class elemType>
    class list_item{
    public:
        list_item(elemType value, list_item *item = 0)
            :_value(value){
                if(!item)
                    _next = 0;
                else{
                    _next = item->_next;
                    item->_next = this;
                }
            }
        elemType value(){return _value;}
        list_item *next(){return _next;}
        void next(list_item *link){_next = link;}
        void value(elemType new_value){_value = new_value;}
        void display(){cout<<_value<<endl;}
    private:
        elemType _value;
        list_item *_next;
    };
}
//using namespace rong;
namespace name1{int a=1,b=2,c=3,d=4;}
namespace name2{int a=5,b=6,c=7,d=8;}
int main(int argc, char*argv[])
/* Rong Tao 2018.03.12 */
{
    using namespace rong;
    list_item<int> *l = new list_item<int>(3);
    using namespace name1;
    using namespace name2;
    
    cout<<name1::a<<endl;
    cout<<name2::a<<endl;
    
    return 0;
}
```
### 例2：命名空间中的变量
```cpp
#include<iostream>
#include<string>
using namespace std;
namespace rt{
    
    int r = 100;
    int t = 200;
};
using namespace rt;
int main(int argc, char *argv[])
{
    rt::r = 98;
    int r = 99;
    cout<<rt::r<<r<<endl;
    
    return 1;
}
```
### 例3：命名空间里的函数
```cpp
#include<iostream>
#include<string>
using namespace std;
namespace rt{
    int r = 100;
    int t = 200;
    template <typename elemType>
    void print(elemType r, elemType t)
    {
        cout<<r<<t<<endl;
    }
};
using namespace rt;
int main(int argc, char *argv[])
{
    rt::r = 98;
    int r = 99;
    cout<<rt::r<<r<<endl;
    rt::print<int>(rt::r, r);
    rt::print<string>(string("rong"), string("tao"));
    return 1;
}
```
### 例4：命名空间里的函数
```cpp
#include<iostream>
#include<string>
using namespace std;
namespace rt{
    int r = 100;
    int t = 200;
    template <typename elemType>
    void print(elemType r, elemType t)
    {
        cout<<r<<t<<endl;
    }
    //嵌套namespace
    namespace R{
      
        int r = 10;
        int t = 20;
        void print()
        {
            cout<<"rt::r::"<<endl;
        }
        void print(string str);
    };
};
void rt::R::print(string str)
{
    cout<<"printtt: "<<str<<endl;
}
using namespace rt;
int main(int argc, char *argv[])
{
    rt::r = 98;
    int r = 99;
    cout<<rt::r<<r<<endl;
    rt::print<int>(rt::r, r);
    rt::print<string>(string("rong"), string("tao"));
    
    rt::R::print();
    rt::R::print("rongtaoshinidaye");
    return 1;
}
```
### 例5：匿名namespace
```cpp
#include<iostream>
#include<string>
using namespace std;
//函数swap()只在此文件中可见
namespace {void swap(){/*...*/};}
int main(int argc, char *argv[])
{
    return 1;
}
```
### 例6：using
```cpp
#include<iostream>
#include<string>
using namespace std;
namespace China_University_of_Petrolume{int a = 999;}
int main(int argc, char *argv[])
{
    namespace upc = China_University_of_Petrolume;
    
    cout<<upc::a<<endl;
    
    using China_University_of_Petrolume::a;
    
    cout<<a<<endl;
    return 1;
}
```
