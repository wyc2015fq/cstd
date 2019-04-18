# Horner法则 - Orisun - 博客园







# [Horner法则](https://www.cnblogs.com/zhangchaoyang/articles/2668233.html)





Horner Rule是一种对多项求值的高效方法。

![](http://www.forkosh.com/mathtex.cgi?\dpi{150}f(x)=a_0+a_1x+a_2x^2+......+a_nx^n)

直接计算的话，需要进行的乘法次数为：1+2+3+......+n，![](http://www.forkosh.com/mathtex.cgi?O(n^2))

Horner给出的算法：

![](http://www.forkosh.com/mathtex.cgi?\dpi{150}f(x)=((\cdots((a_nx+a_{n-1})x+a_{n-2})x+\cdots+a_2)x+a_1)x+a_0)

需要进行的乘法次数为：N次

```
#include<iostream>
#include<deque>

using namespace std;

double horner(deque<double> & coe,double x,double pre=0.0){
    if(coe.size()<=0)
        return pre;
    double rect=pre*x+coe.back();
    coe.pop_back();
    return horner(coe,x,rect);
}

int main(){
    deque<double> d;
    for(int i=1;i<6;++i)
        d.push_back(i);
    double x=2;
    cout<<horner(d,x)<<endl;
    return 0;
}
```












