# x的n次幂 - guoziqing506的博客 - CSDN博客





2016年04月11日 22:11:44[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：825
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：实现一个函数，计算x的n次幂




样例：

Pow(2.1, 3) = 9.261

Pow(0, 1) = 0

Pow(1, 0) = 1




题目的意思很简单，就是实现一个简单的pow函数。既然出现在二分法的章节里，我们当然想到用二分法解决之。一个数的n次方可以看做是它的（n / 2）次方的平方，然后他的（n / 2）次方又是（n / 4）次方的平方。。。。以此类推，每个求幂运算都“二分”后平方。显然，不难发现这是一个二分法和递归思想（有关递归的基本思想详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51057987)）的综合运用。


当然，写程序时，需要注意的是对于负数次幂的处理，应该先转换成相应的正数次幂，再求倒数。



```python
class Solution:
    # @param {double} x the base number
    # @param {int} n the power number
    # @return {double} the result
    def myPow(self, x, n):
        if n < 0:
            return 1 / self.helper(x, -n)
        else:
            return self.helper(x, n)
        
    def helper(self, x, n):
        if n == 0:
            return 1
        mid = n // 2
        if n % 2 == 0:
            return self.myPow(x, mid) * self.myPow(x, mid)
        else:
            return self.myPow(x, mid) * self.myPow(x, mid) * x
        # Write your code here
```



方便起见，另外定义了一个辅助函数helper(x, n)来计算x的正数次幂，这样整个函数的结构会更清晰。函数myPow()解决的问题是处理正负次幂的转换，而helper()中利用了递归的思想，“触底”的条件是"n == 0"，然后根据n的奇偶性分两种情况讨论即可。

这里既然牵涉到了正数奇偶性的判断，就再多说一句，我们可以用下面的两个函数来判断整数的奇偶性：



```python
def iseven(n):
	if n % 2 == 0:
		return True
	else:
		return False

def isodd(n):
	if n % 2 != 0:
		return True
	else:
		return False
```
却不能用下面这个函数来判断整数是否为奇数：



```python
def isodd(n):
	if n % 2 == 1:
		return True
	else:
		return False
```


因为如果是负数，模2的结果就有可能是-1（这个因语言而异，在python里面是无所谓的，例如-3 % 2 = 1，但是，放到某些其他语言中，例如C++等，就必须用n % 2 != 0这种形式）




