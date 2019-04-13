
# python-回调函数和递归函数 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月17日 08:24:20[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：20


[https://www.toutiao.com/a6642513312263176717/](https://www.toutiao.com/a6642513312263176717/)

# 回调函数
一个函数中的参数是另一个函数的名字。
```python
#回调函数
i=10;
j=20;
def myfunc(i,j,f):
 return f(i,j);
def sum(a,b):
 return '{}+{}={}'.format(a,b,a+b);
print(myfunc(i,j,sum));
注：f参数将来会是sum函数的名字，f被称之为回调参数、sum被称之为回调函数。
```
运行结果：
```python
10+20=30
[Finished in 0.1s]
```
# 递归函数
函数体内调用本函数。
a函数调用a函数
递归函数不能写成死循环，如果写成死循环则很容易引起内存溢出。
统计目录大小
一定要善于使用草图来完成程序的设计
```python
#递归函数
num=3;
def myfunc(n):
 print(n);
 if n>1:
  myfunc(n-1);
 print(n);
myfunc(num);
```
运行结果：
```python
3
2
1
1
2
3
[Finished in 0.1s]
注：结果为什么是3/2/1 1/2/3
```
为什么是3、2、1和1、2、3？
首先，程序的执行是自上而下、自左至右的。
下图是递归函数的草图，
第一次执行时，输出两个3，由于满足if条件，到myfunc时，继续执行myfunc2，所以第一次没有输出两个3。
第二次执行时，输出两个2，由于满足if条件，到myfunc时，继续执行myfunc1，所以第二次没有输出两个2。
第三次执行时，输出两个1，由于不满足if条件，到myfunc时，该函数结束执行，此时输出两个1。
综上所述，第一次输出两个3、第二次输出两个2、第三次输出两个1，但是程序的执行是自上而下、自左至右的。所以最后输出的结果是我们看到的3、2、1和1、2、3。
**不知道大家明白没有？**
```python
递归草图
myfunc(3){
 print(3);
 myfunc(2){
  print(2);
  myfunc(1){
   print(1);
   print(1);
  }
  print(2);
 }
 print(3);
}
```
# 递归函数的应用
使用递归函数完成数字累加之和。
```python
#递归函数
num=10;
def myfunc(n):
 if n>1:
  return n+myfunc(n-1);
 else:
  return 1;
print(myfunc(num));


# 递归草图
# myfunc(3):
#  return 3+myfunc(2){
#   return 2+myfunc(1){
#    return 1; 
#   }
  
#  }
 
# }
# 
# 
# def myfunc(n):
#   if n>1:
#    return n+myfunc(n-1);
#   else:
#    return 1;
```
运行结果：
```python
55
[Finished in 0.1s]
```
原因如下：
![python-回调函数和递归函数](http://p1.pstatp.com/large/dfic-imagehandler/4b9c18bc-8dd3-475c-a528-05e09254c8fd)
最后第一次返回的3+第二次返回的3+第三次不满足条件返回的1=6
# 结语
感谢阅读，欢迎在评论区中发表自己不同的观点，若有其他问题请在评论区留言，喜欢的朋友请多多关注转发支持一下。
头条号：ys0202
**----------------END----------------**


