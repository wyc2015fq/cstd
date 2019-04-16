# 对js的一些总结 - ggabcda的博客 - CSDN博客





2017年08月31日 14:41:40[ggabcda](https://me.csdn.net/ggabcda)阅读数：158








---------**最近工作老是在做前端页面，不可避免的使用了js。下面针对工作中使用到的js进行整理总结。**



** 类型**

总所周知，js是个弱类型语言，可以使用var声明一切类型变量；js和其他的语言具有的数据类型差不多，除了Number数值类型需要注意。在对变量进行具体的赋值后；变量会与值的类型同步；而通过DOM操作获取的值类型往往是字符串类型，所以类型转换一般为字符串转其他。记录一下js常用的类型转化方法。



string 转换为其他类型  

方法：parseInt() 、parseFloat() 、Number() 其他类似；这些都是强制转换的方法 ；查看变量类型使用typeof()；都是原生级的方法，直接用！

**坑：NaN ----非数值类型，表明不是数字；往往在类型转化后或者数值运算后**出现的**结果。出现NaN说明类型转换**有问题**或者运算的类型不对称,这时候检查转换的值是否出现了不该出现的东西，比如中文、特殊字符、非数字等。**

类型转换完当然要计算的啦，加、减、乘、除、来一套，顺便带上浮点运算。



加法：**坑：小心变成字符串拼接 出现1+1 =11的情况，**还是类型转换问题，除了转换好类型，也可以这样操作 a*1+b*1;或者使用eval()；

减法：暂时没有被坑~~~；

乘法：同上；

除法：同上；

浮点运算：js的浮点运算，让以上四大法全有了坑；**坑：运算结果出现5.8999999999这种情况 或者6.00000000014；**

解决方法：使用以下代码  转载至：http://www.cnblogs.com/jiahaipeng/archive/2008/10/29/1321821.html

```
1 //除法函数，用来得到精确的除法结果
 2 //说明：javascript的除法结果会有误差，在两个浮点数相除的时候会比较明显。这个函数返回较为精确的除法结果。
 3 //调用：accDiv(arg1,arg2)
 4 //返回值：arg1除以arg2的精确结果
 5 function accDiv(arg1,arg2){
 6     var t1=0,t2=0,r1,r2;
 7     try{t1=arg1.toString().split(".")[1].length}catch(e){}
 8     try{t2=arg2.toString().split(".")[1].length}catch(e){}
 9     with(Math){
10         r1=Number(arg1.toString().replace(".",""));
11         r2=Number(arg2.toString().replace(".",""));
12         return (r1/r2)*pow(10,t2-t1);
13     }
14 }
15 //给Number类型增加一个div方法，调用起来更加方便。
16 Number.prototype.div = function (arg){
17     return accDiv(this, arg);
18 };
19 //乘法函数，用来得到精确的乘法结果
20 //说明：javascript的乘法结果会有误差，在两个浮点数相乘的时候会比较明显。这个函数返回较为精确的乘法结果。
21 //调用：accMul(arg1,arg2)
22 //返回值：arg1乘以arg2的精确结果
23 function accMul(arg1,arg2)
24 {
25     var m=0,s1=arg1.toString(),s2=arg2.toString();
26     try{m+=s1.split(".")[1].length}catch(e){}
27     try{m+=s2.split(".")[1].length}catch(e){}
28     return Number(s1.replace(".",""))*Number(s2.replace(".",""))/Math.pow(10,m);
29 }
30 //给Number类型增加一个mul方法，调用起来更加方便。
31 Number.prototype.mul = function (arg){
32     return accMul(arg, this);
33 };
34 //加法函数，用来得到精确的加法结果
35 //说明：javascript的加法结果会有误差，在两个浮点数相加的时候会比较明显。这个函数返回较为精确的加法结果。
36 //调用：accAdd(arg1,arg2)
37 //返回值：arg1加上arg2的精确结果
38 function accAdd(arg1,arg2){
39     var r1,r2,m;
40     try{r1=arg1.toString().split(".")[1].length}catch(e){r1=0}
41     try{r2=arg2.toString().split(".")[1].length}catch(e){r2=0}
42     m=Math.pow(10,Math.max(r1,r2));
43     return (arg1*m+arg2*m)/m;
44 }
45 //给Number类型增加一个add方法，调用起来更加方便。
46 Number.prototype.add = function (arg){
47     return accAdd(arg,this);
48 }
49 //减法函数
50 function accSub(arg1,arg2){
51      var r1,r2,m,n;
52      try{r1=arg1.toString().split(".")[1].length}catch(e){r1=0}
53      try{r2=arg2.toString().split(".")[1].length}catch(e){r2=0}
54      m=Math.pow(10,Math.max(r1,r2));
55      //last modify by deeka
56      //动态控制精度长度
57      n=(r1>=r2)?r1:r2;
58      return ((arg2*m-arg1*m)/m).toFixed(n);
59 }
60 ///给number类增加一个sub方法，调用起来更加方便
61 Number.prototype.sub = function (arg){
62     return accSub(arg,this);
63 }
```

调用：数字.mul(数字)；也可以自己进行浮点的”精度”进行设置，使用toFixed(2);保留两位小数(四舍五入)，然后再参与运算！

**DOM操作 (重中之重)**

**先写总结吧：1、在未定死网页组成结构之前，不要写根据上下结构而获取到的元素的代码，如parent().parent()..... children().children()...这种代码；因为网页结构很可能跟着变化的需求来修改；牵一发而动全身**

js技术的重中之重，分别写一下原生javascript和jquery操作dom的方法

原生 document.getElement



** JS测试调试**

alert() 、console.log()  最后+Chrome F12 其他不写了 （懒



未完待续！



