
# 特例模式（Special Case Pattern）与空对象模式（Null Pointer Pattern）—— 返回特例对象而非 null - Zhang's Wikipedia - CSDN博客


2018年01月29日 22:53:37[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：467


返回 null 值，基本上是在给自己增加工作量，也是给调用者添乱。只有一处没有检查返回的是否为 null，程序就会抛 NullPointerException 异常。
如果你打算在方法中返回 null 值，不如：
抛出异常，或者返回特例对象。
如果你在调用某个第三方 api 中可能返回 null 值的方法，可以考虑用新方法中进一步打包（封装）这个方法，在更上层的代码中抛出异常或返回特例对象；
如下的一段代码：
```python
List
```
```python
<Employee> employees = getEmployees();
```
```python
if
```
```python
(employees !=
```
```python
null
```
```python
) {
```
```python
for
```
```python
(Employee employee : employees) {
        totalPay += employee.getPay();
    }
}
```
之所以这样处理，自然 getEmployees() 可能返回 null，不妨对 getEmployees 函数做进一步的封装：
```python
public
```
```python
List<Employee>
```
```python
getNoNullEmployees
```
```python
() {
    List<Employee> employees = getEmployees();
```
```python
if
```
```python
(employees ==
```
```python
null
```
```python
) {
```
```python
return
```
```python
Collections.emptyList();
    }
```
```python
return
```
```python
employees;
}
```
## 1. Special Case ： 特例模式
该模式的类图如下：

![这里写图片描述](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)特例类同样继承自普通基类，只是其封装的是异常处理。
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)我们将如下两种不友好的（返回 null 或特殊值的异常处理），改造为特例类的处理方式：
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
// 不要返回 null
```
```python
class
```
```python
MissingCustomer
```
```python
implements
```
```python
Customer
```
```python
{
```
```python
public
```
```python
Long getId() {
```
```python
return
```
```python
null
```
```python
;
    }
}
```
```python
// 不要返回特殊值
```
```python
class
```
```python
MissingCustomer
```
```python
implements
```
```python
Customer
```
```python
{
```
```python
public
```
```python
Long getId() {
```
```python
return
```
```python
0
```
```python
;
    }
}
```
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)而是采用如下的方式（抛出有意义的特殊异常）：
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
class
```
```python
MissingCustomer
```
```python
implements
```
```python
Customer
```
```python
{
```
```python
public
```
```python
Long getId() throws MissingCustomerException {
```
```python
throw
```
```python
new
```
```python
MissingCustomerException();
    }
}
```
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2. 特例类的默认方法
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)特例类封装了异常处理，有时当异常发生时，我们并不想让程序直接崩溃退出，而是继续运行下去。此时在特例类中，还需给出当异常情况发生时，特例实例的异常处理方式：
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
class
```
```python
MissingSprite
```
```python
implements
```
```python
Sprite
```
```python
{
```
```python
public
```
```python
void
```
```python
draw() {
```
```python
// draw nothing or a generic something.
```
```python
}
}
```
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## references
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[Awesome Special Case Patterns – AdhocMaster](https://adhocmaster.wordpress.com/2017/12/21/awesome-special-case-patterns/)
[](https://img-blog.csdn.net/20180129223935286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

