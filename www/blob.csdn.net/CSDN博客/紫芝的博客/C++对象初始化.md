# C++对象初始化 - 紫芝的博客 - CSDN博客





2018年12月05日 16:03:34[紫芝](https://me.csdn.net/qq_40507857)阅读数：1225








```cpp
#ifndef TDATE_H_INCLUDED
#define TDATE_H_INCLUDED

#include<iostream>
class TDate{
public:
    TDate(int y,int m,int d);
    ~TDate();
    void Print();
private:
    int year,month,day;
};
//构造器
TDate::TDate(int y,int m,int d){
    year=y;
    month=m;
    day=d;
    std::cout<<"Constructer called.\n";
};
//析构器
TDate::~TDate(){
    std::cout<<"Destructor called.\n";
};
void TDate::Print(){
    std::cout<<year<<"."<<month<<"."<<day<<std::endl;
}

#endif // TDATE_H_INCLUDED
```

```cpp
#include<iostream>
#include"tdate.h"
int main(){
    TDate today(1998,4,9),tomorrow(1998,4,10);
    today.Print();
    tomorrow.Print();
    return 0;
}
```

**Result**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205160320111.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)



