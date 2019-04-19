# Python中List变量reverse后，结果为None - Popeye_the_Sailor - CSDN博客
2018年06月19日 22:31:24[_Sailor_](https://me.csdn.net/lz0499)阅读数：2119
Python中List变量reverse后，结果为None
假设有列表a=[1,2,3,4,5,6],调用a = a.reverse()后，结果不是期望的，颠倒过来的list，却是None：
print( a)
值为：
a= None
【解决过程】
1.找了下，结果只找到这个官方解释：
> 
##### 5.6.4. Mutable Sequence Types
s.reverse() reverses the items of *s* in place
Note:The sort() and reverse() methods modify the list in place for economy of space when sorting or reversing a large list. To remind you that they operate by side effect, they don’t return the sorted or reversed list.
但是什么是side effect，还是不懂。
2.后来找到：[a=[1,2,3,4].reverse() – why "a" is None?](http://www.gossamer-threads.com/lists/python/dev/779891)，看到其讨论说到：
> 
b = [1,2,3,4] 
a = b.**reverse**() 
would change the value of b.
才想起来，原来这个reverse函数，针对列表的操作，其结果是直接改变列表本身（为了节省空间），所以，直接就把原先的list改为你所想要的reversed后的结果了，而返回值，是空的，不返回任何值。
所以，本身直接使用：
a.reverse(); # -> is OK, the self is reversed !!!
即可实现把a翻转。
【总结】
对于List等Sequence等类型的变量，比如此处的List变量，其内置函数reverse，是直接操作变量本身，调用reverse后，变量本身的值就是reverse后的值了，所以不能出现：
[?](https://www.crifan.com/python_list_after_reverse_become_to_none/#)
```
`reversedList ``=``orignalList.reverse();`
```
的用法，而应该是：
[?](https://www.crifan.com/python_list_after_reverse_become_to_none/#)
```
`orignalList.reverse();`
`reversedList ``=``orignalList;`
```
此原则，同样适用于sort函数。
Python列表`reverse()`方法用于将列表中的对象反转位置。
**语法**
以下是`reverse()`方法的语法 -
```python
```python
list.reverse()
```
```
Python
**参数**
- *NA*
**返回值**
- **此方法不返回任何值**，但从列表中反转给定对象。
## 示例
以下示例显示了`reverse()`方法的用法。
```python
```python
#!/usr/bin/python3
list1 = ['physics', 'Biology', 'chemistry', 'maths']
list1.reverse()
print ("list now : ", list1)
```
```
Python
当运行上面的程序，它产生以下结果 -
`List``: [``'hema'``,``'xyz'``, ``'abc'``, ``'zara'``, ``'xyz'``, ``126``]`
在函数中如果我们输入：
```python
return list.reverse（）
```
则会出现错误。因为正如前所述，列表的reverse函数不返回任何值，而return是需要返回值的。如果想要返回反转之后的列表值，只需要：
```python
list.reverse
return list
```
转载于：[Python中List变量reverse后，结果为None](https://www.crifan.com/python_list_after_reverse_become_to_none/)
