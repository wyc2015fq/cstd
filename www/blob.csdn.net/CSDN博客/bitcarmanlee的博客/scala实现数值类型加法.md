
# scala实现数值类型加法 - bitcarmanlee的博客 - CSDN博客


2017年12月06日 19:04:22[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1100标签：[scala																](https://so.csdn.net/so/search/s.do?q=scala&t=blog)[泛型																](https://so.csdn.net/so/search/s.do?q=泛型&t=blog)[数值加法																](https://so.csdn.net/so/search/s.do?q=数值加法&t=blog)[implicit																](https://so.csdn.net/so/search/s.do?q=implicit&t=blog)[隐式转化																](https://so.csdn.net/so/search/s.do?q=隐式转化&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=implicit&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=数值加法&t=blog)个人分类：[scala																](https://blog.csdn.net/bitcarmanlee/article/category/6211331)
[
																								](https://so.csdn.net/so/search/s.do?q=数值加法&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=泛型&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=泛型&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=scala&t=blog)

在前面的文章里，我们讲了在java中如何利用泛型实现数值类型加法。具体可以参考博文[http://blog.csdn.net/bitcarmanlee/article/details/78733637](http://blog.csdn.net/bitcarmanlee/article/details/78733637)。
那么在scala中，我们怎么实现上面的需求呢？
## 1.用 <: 模拟extends关键字行不通
如果按照在java中的处理思路，我们可以这么尝试一下：
```python
def
```
```python
numberAdd[T <: Number](t1 : T, t2 : T) = {
        t1.doubleValue() + t2.doubleValue()
    }
```
```python
@Test
```
```python
def
```
```python
numberaddtest() = {
```
```python
val
```
```python
(t1, t2) = (
```
```python
1
```
```python
,
```
```python
2
```
```python
)
        numberAdd(t1, t2)
    }
```
在scala中，`<:`表示类型的上限，与java中的extends含义一样。我们试图将上面的test方法run起来，发现IDE中报错：
```python
Error:(
```
```python
26
```
```python
,
```
```python
19
```
```python
)
```
```python
type
```
```python
mismatch;
 found   : Int
 required: T
        numberAdd(t1, t2)
Error:(
```
```python
26
```
```python
,
```
```python
9
```
```python
) inferred
```
```python
type
```
```python
arguments [Int]
```
```python
do
```
```python
not
```
```python
conform
```
```python
to
```
```python
method
```
```python
numberAdd
```
```python
'
```
```python
s
```
```python
type
```
```python
parameter
```
```python
bounds
```
```python
[
```
```python
T
```
```python
<:
```
```python
Number]
        numberAdd(t1, t2)
Error:(
```
```python
26
```
```python
,
```
```python
23
```
```python
)
```
```python
type
```
```python
mismatch;
 found   : Int
 required: T
        numberAdd(t1, t2)
```
很明显，上面的方法是行不通的。
## 2.使用Numeric[T]
为什么上面的方法行不通呢？因为Scala的数字类型并不都共享一个超类，所以我们不能使用T <: Number。相反，要使之能工作，Scala的math库对适当的类型T 定义了一个隐含的Numeric[T]，我们可以使用他来完成类似的功能。
首先上可以运行的代码：
```python
def
```
```python
add
```
```python
[T](x: T, y: T)(implicit
```
```python
num
```
```python
: Numeric[T]) = {
        val
```
```python
result
```
```python
=
```
```python
num
```
```python
.plus(x, y)
```
```python
result
```
```python
}
    @Test
    def testAdd() = {
        val
```
```python
int1
```
```python
=
```
```python
1
```
```python
val
```
```python
int2
```
```python
=
```
```python
2
```
```python
println(
```
```python
"int sum is: "
```
```python
+
```
```python
add
```
```python
(
```
```python
int1
```
```python
,
```
```python
int2
```
```python
))
        val long1 =
```
```python
100
```
```python
L
        val long2 =
```
```python
200
```
```python
L
        println(
```
```python
"long sum is: "
```
```python
+
```
```python
add
```
```python
(long1, long2))
        val f1 =
```
```python
1.0
```
```python
f
        val f2 =
```
```python
2.0
```
```python
f
        println(
```
```python
"float sum is: "
```
```python
+
```
```python
add
```
```python
(f1, f2))
        val d1 =
```
```python
1.0
```
```python
val d2 =
```
```python
2.0
```
```python
println(
```
```python
"double sum is: "
```
```python
+
```
```python
add
```
```python
(d1, d2))
    }
```
将上面的test方法run起来，可以得到如下输出：
```python
int
```
```python
sum
```
```python
is
```
```python
:
```
```python
3
```
```python
long
```
```python
sum
```
```python
is
```
```python
:
```
```python
300
```
```python
float
```
```python
sum
```
```python
is
```
```python
:
```
```python
3.0
```
```python
double
```
```python
sum
```
```python
is
```
```python
:
```
```python
3.0
```
## 3.Numeric[T]的用法
Numeric[T]在scala中源码如下：
```python
type Numeric[T] = scala
```
```python
.math
```
```python
.Numeric
```
```python
[T]
  val Numeric = scala
```
```python
.math
```
```python
.Numeric
```
当然，我们也可以通过implicitly方法，用context bound(上下文绑定)的方式让上面的代码更简单：
```python
def
```
```python
add2
```
```python
[
```
```python
T
```
```python
:
```
```python
Numeric](x: T, y: T) = {
        implicitly[Numeric[T]].plus(x, y)
    }
```
其中，implicitly方法在scala中的定义如下：
```python
@inline def implicitly[
```
```python
T
```
```python
](
```
```python
implicit e: T
```
```python
) = e    // for summoning implicit values from the nether world
```
implicitly 主要是在当前作用域查找指定类型，例如以下的例子：
```python
@Test
```
```python
def
```
```python
testimplicit() = {
        implicit
```
```python
val
```
```python
x =
```
```python
1
```
```python
implicit
```
```python
val
```
```python
x1 =
```
```python
2.0
```
```python
val
```
```python
y = implicitly[Int]
```
```python
val
```
```python
z = implicitly[Double]
        println(y +
```
```python
"\t"
```
```python
+ z)
    }
```
将test方法run起来以后，输出如下：
```python
1   2.0
```
## 4.数值集合求和
搞定了单个的数值求和，那么数值集合的求和自然就变得容易了：
```python
def add4[T: Numeric](x :
```
```python
Array
```
```python
[T]) = {
        var sum =
```
```python
0.0
```
```python
for
```
```python
(
```
```python
each
```
```python
<- x) {
            sum += implicitly[Numeric[T]].toDouble(
```
```python
each
```
```python
)
        }
        println(sum)
    }
    @Test
    def test() = {
        val
```
```python
array
```
```python
=
```
```python
Array
```
```python
(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
)
        add4(
```
```python
array
```
```python
)
    }
```
代码输出：
```python
6.0
```
参考内容：
1.[https://stackoverflow.com/questions/4373070/how-do-i-get-an-instance-of-the-type-class-associated-with-a-context-bound](https://stackoverflow.com/questions/4373070/how-do-i-get-an-instance-of-the-type-class-associated-with-a-context-bound)
2.[https://stackoverflow.com/questions/2982276/what-is-a-context-bound-in-scala](https://stackoverflow.com/questions/2982276/what-is-a-context-bound-in-scala)
3.[http://www.jianshu.com/p/1d119c937015](http://www.jianshu.com/p/1d119c937015)Scala中的Implicit详解
4.[https://vimsky.com/article/1562.html](https://vimsky.com/article/1562.html)scala常见问题整理
5.[https://fangjian0423.github.io/2015/06/07/scala-generic/](https://fangjian0423.github.io/2015/06/07/scala-generic/)scala泛型

