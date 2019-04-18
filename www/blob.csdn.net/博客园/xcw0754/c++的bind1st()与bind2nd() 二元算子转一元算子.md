# c++的bind1st()与bind2nd() 二元算子转一元算子 - xcw0754 - 博客园
# [c++的bind1st()与bind2nd() 二元算子转一元算子](https://www.cnblogs.com/xcw0754/p/8311030.html)
### bind1st()和bind2nd()是两个函数，用于将二元算子转成一元算子。
### 何谓二元算子？
比如`< > =`等等这些就是二元算子，即需要两个操作数的运算符。
### 何谓一元算子？
比如`++ --`等等这些就是一元算子，即只需要一个操作数的运算符。
### bind2st()的使用
其实这个函数并没有强大到可以真的实现二元算子转一元算子，只是在特定的场合用起来会很有用而已。看下面的例子
```
int main()
{
    int a[] = {1, 2, 100, 200};
    // 用数组a来初始化arr
    std::vector< int> arr(a, a + 4);  
    // 移除arr中小于100的所有元素
    arr.erase( 
        std::remove_if( arr.begin(),  arr.end(),  std::bind2nd( std::less< int>(), 100) ), 
        arr.end()
    );
    for(int i=0; i<arr.size(); i++) std::cout<<arr[i]<<std::endl;  // 输出: 100 200
    return 0;
}
```
对`remove_if`不熟悉的话请看下面的接口定义
```
template <class ForwardIterator, class UnaryPredicate>
ForwardIterator remove_if(ForwardIterator first, 
        ForwardIterator last,
        UnaryPredicate pred);
```
功能就是剔除从`[first, last)`中所有符合表达式`pred`的元素，返回一个`ForwardIterator`迭代器指向处理过的数组的`end`端。
比如有数组`a[] = {1, 2, 100, 200}`，剔除掉小于100的元素，处理过后变成`a[] = {100, 200, 100, 200}`;
比如有数组`a[] = {1, 100, 2, 200}`，剔除掉小于100的元素，处理过后变成`a[] = {100, 200, 2, 200}`;
以上两个例子的返回值均是指向`a[2]`，可以看出，其实就是将不符合表达式`pred`的元素全部移到数组`a`的前部而已，而后面的部分完全是未处理之前的一模一样，应该将a[2~3]删除掉才能达到我们的目的，故需要使用到`erase()`这个函数，处理过程已在上面的代码中。
### bind1st()的使用
```
// 移除所有大于100的元素，与上例相反
    arr.erase( 
        std::remove_if( arr.begin(),  arr.end(),  std::bind1nd( std::less< int>(), 100) ), 
        arr.end()
    );
```
这里的表达式`std::bind1nd( std::less< int>(), 100)`相当于`100 < arr.value`，而上个例子中`std::bind2nd( std::less< int>(), 100)`相当于`arr.value < 100`。聪明的人想到了一种用`bind2nd`也能实现移除所有大于100的元素的效果，那就是换个运算符，像这样
```
arr.erase( 
        std::remove_if( arr.begin(),  arr.end(),  std::bind2nd( std::greater< int>(), 100) ), 
        arr.end()
    );
```
这样也是OK的。
### 要实现小于等于怎么办？
STD提供了`not1()`函数，相当于逻辑运算符`!`。如果要实现`a <= b`，可以换种写法，像这样`!( a > b )`，那么`not1()`就可以派上用场了。
```
std::remove_if(
        arr.begin(),  
        arr.end(),  
        std::not1( std::bind2nd( std::greater<int>(), 100))   // 相当于  !( arr.value > 100 ) 
    )
```
参考文章 [点我直达](http://blog.csdn.net/simahao/article/details/405455)

