
# C++使用sort排序导致的coredump（Strict Weak Ordering） - jiange_zh的博客 - CSDN博客


2017年10月15日 14:57:40[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：910


当我们需要自定义排序规则时，需要实现一个比较函数，该函数类似如下：
```python
bool
```
```python
cmp(
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
b)
{
```
```python
return
```
```python
a>b;
}
```
当cmp返回true时，a将会排在b前面，因此上面的函数将从大到小排序。
换句话说，cmp函数重新定义了“小”的概念（当a>b时，a“小于”b），整个序列将按照这个“小”的规则从“小”到“大”排序。
前几天同事遇到一个问题：
对一组数据从小到大排序，对于值相等的两个元素，经过排序之后，原本靠后的元素排在前面。于是他写出了如下的比较函数：
```python
bool
```
```python
cmp(
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
b)
{
```
```python
return
```
```python
a<=b;
}
```
以上写法有两个问题：
首先我们回顾下排序算法稳定性的概念：假定在待排序的记录序列中，存在多个具有相同的关键字的记录，若经过排序，这些记录的相对次序保持不变，即在原序列中，ri=rj，且ri在rj之前，而在排序后的序列中，ri仍在rj之前，则称这种排序算法是稳定的；否则称为不稳定的。
而sort内部使用的排序算法不一定是稳定的（当元素较多时，使用的快速排序是不稳定的），对于一个不稳定的算法，是无法保证值相同的两个元素的顺序的。
从使用上来说，传递给sort的cmp函数，就不应该使用等号。
从后果上来说，上述用法不仅不能解决问题，还可能导致程序coredump——没错，就在周五晚上，运营人员配置了一个新的元素之后，触发了sort函数的coredump，幸亏人还在公司，立马把触发coredump的资源下掉，并及时修复了该bug。
coredump原因分析：
对于std::sort()，当容器里面元素的个数大于_S_threshold 的枚举常量值时，会使用快速排序（stl的这个默认值是16）。
STL快速排序源码的关键代码：
![这里写图片描述](https://img-blog.csdn.net/20171015151906228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmdlX3po/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注意这两行：
```python
while
```
```python
(__comp(
```
```python
*__first
```
```python
,__pivot))
    ++__first;
```
这两行代码里面，__pivot是中间值，__first是迭代器，假设我们的__comp函数对于相等的值返回true，那么如果一个容器里面最后一段元素所有值都相等，那么__comp(*__first,__pivot)就恒为真。迭代器往前移的时候，终会移过最后一个元素，于是迭代器失效，程序core。
可以看到，上面快排的思路是，从左往右找一个比中间值“大”（即cmp函数返回false）的元素，从右往左找一个比中间值“小”元素，然后交换两个元素的位置，使得大的在右边，小的在左边。
当cmp函数return a < b时，如果所有元素已经是有序的了，只要遇到一个值跟中间元素相等的元素（包括中间元素自己），cmp便返回false，__first迭代器右移停止。
而当cmp函数return a <= b时，若中间元素前面的元素都比它小，而后面的元素都跟它相等或者比它小，那么cmp恒返回true，__first迭代器不断右移，程序越界导致coredump。
坑踩过了，才能涨知识。
其实类似的问题，在《Effective STL》 的条款21中就有讨论：**永远让比较函数对相同元素返回false**！
构建一个set，比较类型用的是less_equal，然后insert一个10：
```python
set
```
```python
<
```
```python
int
```
```python
, less_equal<
```
```python
int
```
```python
>
```
```python
> s;
```
```python
// s is sorted by “<=”
```
```python
s.insert(
```
```python
10
```
```python
);
```
```python
// insert the value 10
```
现在尝试再insert一次10：
```python
s.
```
```python
insert
```
```python
(
```
```python
10
```
```python
);
```
对于这一个insert的调用，set必须先要搞明白10是否已经位于其中。 我们知道它已经位于其中，但set可是木头木脑的，它必须执行检查。为了便于弄明白发生了什么，我们将已经在set中的10称为10A，而正试图insert的那个叫10B。
set遍历它的内部数据结构以查找加入10B的位置。 最终，它总要检查10B是否与10A相同。 关联容器对“相同”的定义是equivalence(见Item 19)（WQ注： equivalence应指“数学相等”，two elements are equal if neither is less than the other，见《The C++ Standard Library》中文版P82，英文版电子P77；equality指“逻辑等价”，使用operator==()，见《The Standard Template Library》英文电子版P30)。
因此set测试10B是否**数学等值**于10A。 当执行这个测试时，它自然是使用set的比较函数。在这一例子里，是operator<=，因为我们指定set的比较函数为less_equal，而less_equal就是operator<=。于是，set将计算这个表达式是否为真：
```python
!(
```
```python
10
```
```python
A
```
```python
<=
```
```python
10
```
```python
B ) && !(
```
```python
10
```
```python
B <=
```
```python
10
```
```python
A
```
```python
)       // test
```
```python
10
```
```python
A
```
```python
and
```
```python
10
```
```python
B for equivalence
```
10A和10 B都是10，因此，10A <= 10B 肯定为真。同样，10A <= 10B。上述的表达式简化为
```python
!(
```
```python
true
```
```python
) && !(
```
```python
true
```
```python
)
```
再简化就是
```python
false
```
```python
&&
```
```python
false
```
结果当然是false。 也就是说，set得出的结论是10A与10B不等值，因此不一样，于是它将10B加入容器。在技术上而言，这个行动导致**未定义的行为**，但是通常的结果是set终结于拥有了两个值为10的元素的拷贝，也就是说它不再是一个set了。通过使用less_equal作为我们的比较类型，我们破坏了容器！
此外，所有对相同的元素返回true的比较函数都会做相同的事情。根据定义，相同的元素，是不等值的！
因此我们需要确保用在关联容器上的比较函数总是对相同的元素返回false。
要避免掉入这个陷阱，你所要记住的就是比较函数的返回值指明的是在此函数定义的排序方式下，**一个元素是否应该位于另一个之前**。对于关联容器，相同的元素绝不该一个领先于另一个，所以比较函数总应该为相同的元素返回false。
以上的讨论是针对关联容器的。从技术上讲，用于关联容器的比较函数必须在它们所比较的对象上定义一个“strict weak ordering“。其实，传给sort 等泛型算法的比较函数也有同样的限制。
如果你对strict weak ordering的含义细节感兴趣，可在很多STL指导书籍中找到，比如Josuttis的《The C++ Standard Library》（WQ注：中文版P176，英文版电子P156），Austern的《Generic Programming and the STL 》，和SGI STL的网站。
任何定义了一个strict weak ordering 的函数都必须在传入相同元素的两个拷贝时返回false，具体要求如下：
如果一个comp函数要满足“Strict Weak Ordering”，意味着它应该满足如下特征（更多细节可以参见SGI版实现相关描述[http://www.sgi.com/tech/stl/StrictWeakOrdering.html](http://www.sgi.com/tech/stl/StrictWeakOrdering.html)）:
(a)  反自反性：也即comp(x, x)必须是false
(b)  非对称性：也即如果comp(x, y)和comp(y, x)的结果必然相反
(c)   可传递性：也即如果comp(x, y)为true，comp(y, z)为true，那么comp(x, z)必然为true
这么看到，示例代码的comp定义明显违反了(a)(b)两条，所以sort使用它时就可能工作不正常。解决办法也很简单，去掉那个“=”，再对照下”Strict Weak Ordering”的定义，就满足了。

