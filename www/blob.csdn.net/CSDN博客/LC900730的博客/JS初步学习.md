# JS初步学习 - LC900730的博客 - CSDN博客
2016年03月07日 22:07:41[lc900730](https://me.csdn.net/LC900730)阅读数：254
1.var str=“hello world”;
alert(str.length);
输出结果是：11
因为空格也占一个位置。
2.toFixed的使用：
四舍五入，输出指定位数的小数（面向对象）。
var str2=13.3689;
alert(str2.toFixed(3))；
输出结果：13.369
var str=3.33333；
alert(str2.toFixed(3))；
输出结果：13.333
3 未看视频情况下写自己的第一个脚本：判断2数能否整除
var a1=window.prompt("qingshuruyige");
var a2=window.prompt("qingshuruyige");
var a3=a1/a2;
var a4=""+a3;
if(a4.indexOf(".")==-1)
{
alert("能整除")
}
else{
alert("不能整除")
}
实验了几次，判断正确。
问题：代码正确，但是不了解取余运算符的使用。
var a1=*；
var a2=*；
if（a1%a2==0）{
alert（“整除”）
}
else
{
“不能整除”
}
4当使用var num1=prompt（）命令时候，即使是输入了一个正整数，用typeof命令查看，可以知道，num1被当做一个字符串来对待。
5 严格相等
var  num1=“123”；
var num2=123;
if(num1==num2);
{
alert(num1==num2);
}
if(num1===num2){num1===num2;}
**结果是num1==num2.问题来了，既然123与“123”明明是不同的数据类型，为什么想等？？？？？？用typeof查看明明一个是number，一个是string。**
