
# java中的值传递与引用传递 - bitcarmanlee的博客 - CSDN博客


2018年03月22日 14:45:11[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：186标签：[值传递																](https://so.csdn.net/so/search/s.do?q=值传递&t=blog)[引用传递																](https://so.csdn.net/so/search/s.do?q=引用传递&t=blog)[内存地址																](https://so.csdn.net/so/search/s.do?q=内存地址&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=引用传递&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=值传递&t=blog)个人分类：[java																](https://blog.csdn.net/bitcarmanlee/article/category/6139246)
[
																								](https://so.csdn.net/so/search/s.do?q=值传递&t=blog)


在java中，如果参数类型是原始类型，那么传过来的就是这个参数的一个副本，也就是这个原始参数的值，如果在函数中改变了副本的 值不会改变原始的值。
如果参数类型是引用类型，那么传过来的就是这个引用参数的副本，这个副本存放的是参数的地址。如果在函数中没有改变这个副本的地址，而是改变了地址中的 值，那么在函数内的改变会影响到传入的参数。如果在函数中改变了副本的地址，如new一个，那么副本就指向了一个新的地址，此时传入的参数还是指向原来的 地址，所以不会改变参数的值。
传递值的数据类型：八种基本数据类型和String(这样理解可以，但是事实上String也是传递的地址,只是string对象和其他对 象是不同的，string对象是不能被改变的，内容改变就会产生新对象。那么StringBuffer就可以了，但只是改变其内容。不能改变外部变量所指 向的内存地址)。
传递地址值的数据类型：除String以外的所有复合数据类型，包括数组、类和接口
为了对上面参数传递有更清楚的认识，可以参考如下的测试代码：
```python
public
```
```python
class
```
```python
ParamTransferTest {
```
```python
public
```
```python
static
```
```python
void
```
```python
swapnum
```
```python
(
```
```python
int
```
```python
a,
```
```python
int
```
```python
b) {
```
```python
int
```
```python
tmp = a;
        a = b;
        b = tmp;
    }
```
```python
public
```
```python
static
```
```python
class
```
```python
Person {
```
```python
public
```
```python
String name;
```
```python
public
```
```python
int
```
```python
age;
```
```python
public
```
```python
Person
```
```python
(String name,
```
```python
int
```
```python
age) {
```
```python
this
```
```python
.name = name;
```
```python
this
```
```python
.age = age;
        }
```
```python
public
```
```python
void
```
```python
changeattr
```
```python
(Person p, String name,
```
```python
int
```
```python
age) {
            p.name = name;
            p.age = age;
        }
```
```python
public
```
```python
void
```
```python
printp
```
```python
() {
            System.
```
```python
out
```
```python
.println(
```
```python
this
```
```python
.name +
```
```python
", "
```
```python
+
```
```python
this
```
```python
.age);
        }
    }
```
```python
public
```
```python
static
```
```python
void
```
```python
main
```
```python
(String[] args) {
```
```python
int
```
```python
a =
```
```python
1
```
```python
, b =
```
```python
2
```
```python
;
        System.
```
```python
out
```
```python
.println(
```
```python
"before swap, a is: "
```
```python
+ a +
```
```python
", b is: "
```
```python
+ b);
        swapnum(a, b);
        System.
```
```python
out
```
```python
.println(
```
```python
"after swap, a is: "
```
```python
+ a +
```
```python
", b is: "
```
```python
+ b);
        Person p =
```
```python
new
```
```python
Person(
```
```python
"zhangsan"
```
```python
,
```
```python
15
```
```python
);
        System.
```
```python
out
```
```python
.println(
```
```python
"\nbefore change, person is: "
```
```python
);
        p.printp();
        p.changeattr(p,
```
```python
"lisi"
```
```python
,
```
```python
16
```
```python
);
        System.
```
```python
out
```
```python
.println(
```
```python
"after change, person is: "
```
```python
);
        p.printp();
    }
}
```
将代码run起来以后，得到的结果为：
```python
before
```
```python
swap, a
```
```python
is
```
```python
:
```
```python
1
```
```python
, b
```
```python
is
```
```python
:
```
```python
2
```
```python
after
```
```python
swap, a
```
```python
is
```
```python
:
```
```python
1
```
```python
, b
```
```python
is
```
```python
:
```
```python
2
```
```python
before
```
```python
change, person
```
```python
is
```
```python
: 
zhangsan,
```
```python
15
```
```python
after
```
```python
change, person
```
```python
is
```
```python
: 
lisi,
```
```python
16
```
简单总结起来就是：
1.如果传入的参数是基本数据类型，方法里的操作不会对原始值做改变。
2.如果传入的参数是一个对象或者数组或者接口等复杂类型，方法里的操作可以改变这个对象里面的具体内容，但是不会改变对象的内存地址。(如果方法里没做相应操作的话)

