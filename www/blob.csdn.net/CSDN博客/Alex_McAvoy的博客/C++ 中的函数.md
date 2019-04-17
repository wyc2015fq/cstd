# C++ 中的函数 - Alex_McAvoy的博客 - CSDN博客





2018年02月22日 00:23:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：229









|函数名|格式|功能|例子|
|----|----|----|----|
|绝对值函数|abs(x)|求x绝对值|abs(-5)=5|
|指数函数|pow(x,y)|求x^y，结果为双精度实数|pow(2,3)=8|
|平方根函数|sqrt(x)|求√x|sqrt(25)=5|
|自然数指数函数|exp(x)|求实数x的自然指数e^x|exp(1)=2.718|
|自然对数函数|log(x)|求x的自然数对数|log(1)=0|
|向下取整函数|floor(x)|求不大于实数x的最大整数|floor(3.14)=3|
|向上取整函数|ceil(x)|求不大于实数x的最小整数|ceil(3.14)=4|

注：

①当实数x＞0时，floor函数与强制转换int(x)作用相同

**[cpp]**[view plain](http://blog.csdn.net/u011815404/article/details/79344853#)[copy](http://blog.csdn.net/u011815404/article/details/79344853#)
- **double** a;  
- cin>>a;//输入3.14
- cout<<floor(a)<<endl;//输出3
- cout<<ceil(a)<<endl;//输出4
- cout<<**int**(a)<<endl;//输出3

②当实数x<0时，ceil函数与强制转换int(x)相同。

**[cpp]**[view plain](http://blog.csdn.net/u011815404/article/details/79344853#)[copy](http://blog.csdn.net/u011815404/article/details/79344853#)
- **double** a;  
- cin>>a;//输入-3.14
- cout<<floor(a)<<endl;//输出-4
- cout<<ceil(a)<<endl;//输出-3
- cout<<**int**(a)<<endl;//输出-3



