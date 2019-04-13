
# java异常处理中的返回值 - bitcarmanlee的博客 - CSDN博客


2018年08月16日 17:51:31[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：780标签：[异常处理																](https://so.csdn.net/so/search/s.do?q=异常处理&t=blog)[try catch																](https://so.csdn.net/so/search/s.do?q=try catch&t=blog)[返回值																](https://so.csdn.net/so/search/s.do?q=返回值&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=try catch&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=异常处理&t=blog)个人分类：[java																](https://blog.csdn.net/bitcarmanlee/article/category/6139246)
[
																								](https://so.csdn.net/so/search/s.do?q=异常处理&t=blog)



## 1.try-catch中的返回值
java代码中，有各种各样的try-catch代码用来捕获各种异常。那么在try-catch代码中，返回值是如何处理的呢？先看看下面的代码
```python
public
```
```python
static
```
```python
int
```
```python
test_finally
```
```python
() {
```
```python
int
```
```python
x;
```
```python
try
```
```python
{
            x =
```
```python
1
```
```python
;
```
```python
//int y = 1/0; 注释1
```
```python
//String raw = "a"; 注释2
```
```python
//raw.substring(3); 注释2
```
```python
return
```
```python
x;
        }
```
```python
catch
```
```python
(ArithmeticException ex) {
            x =
```
```python
2
```
```python
;
```
```python
return
```
```python
x;
        }
```
```python
finally
```
```python
{
            x =
```
```python
3
```
```python
;
        }
    }
```
## 2.执行结果
1.如果代码正常执行，返回结果为1。
2.如果加入注释1的代码，返回结果为2。
3.如果加入注释2的代码，直接抛出异常无返回结果。
## 3.结果分析
第一种情况很好理解，代码走正常逻辑直接返回结果。
第三种情况也比较好理解，直接抛异常。
第二种情况有的同学可能会问，明明finally中给x=3，为什么会返回2？
对于一个java方法来说，退出的方式有两种：
1. 遇到一个返回的指令（return语句）
2. 遇到一个异常，并且没有搜索到异常处理器，不会给调用返回任何值。
所以当catch中有return时，程序遇到return时就被标记了，finally块中只是执行最后的清理工作，如数据库连接的释放等。但是并不会修改return这个语句。
如果我们把代码放在intellij里面，x = 3这一行会提示：
`The value 3 assigned to 'x' is never used less`
如果我们在intellij里面debug，当代码运行到x = 3这一行时，堆栈信息如下：
![这里写图片描述](https://img-blog.csdn.net/20180816174805600?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当程序执行到catch代码快中的return x 那一行时，首先是将x的一个副本保存在了方法栈帧的本地变量表中，执行return之前必须执行finally中的操作：x=3，此时将x设置为了3。但是return时是将本地变量表中保存的x的那个副本拿出来放到栈顶返回，所以出ArithmeticException异常或其子类异常时，返回值是2。

