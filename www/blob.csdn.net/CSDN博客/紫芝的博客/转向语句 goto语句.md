# 转向语句 goto语句 - 紫芝的博客 - CSDN博客





2018年12月03日 09:46:28[紫芝](https://me.csdn.net/qq_40507857)阅读数：2035








> 
语句标号是一种用来标识语句的标识符，用冒号分割


```cpp
#include<iostream>
using namespace std;
//goto只能在一个函数体内进行转向
int main(){
    int i;
    for( i=0;i<100;i++)
        if(i==50)
            goto found;
        cout<<"Not found!"<<endl;
        goto end;
        found:cout<<i<<endl;//found语句
        end:;//end标志是一个空语句
}
```



