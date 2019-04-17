# python cookbook 1.2 python从任意长度的可迭代对象中分解元素(可变长参数) - 别说话写代码的博客 - CSDN博客





2019年01月07日 10:37:41[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：34
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```python
#1.2从任意长度的可迭代对象中分解元素
'''需要从某个可迭代对象中分解出N个元素，但是这个可迭代对象的长度可能超过N，这会导致出现“分解值过多”的异常'''
#   *表达式 用来解决这个问题
record = ('david','david@gmail.com','123456788','32543657')
name,email,*phonenumbers = record
print(name,email)
print(phonenumbers)
```

![](https://img-blog.csdnimg.cn/20190107103458371.png)

```python
#  *表达式 可以放最前面，可以放中间   可以放最后面。目的就是从任意长度的可迭代对象中分解元素
#用8个值表示8个季度的销售额，想实现最近一个季度的销售额与前7个季度的销售额的平均值作比较，可以这么做
sales_record = [45,50,46,47,48,49,55,54]
*head_qtr,current_qtr = sales_record
print(head_qtr)
head_avg = sum(head_qtr) / len(head_qtr)
print(1 if head_avg > current_qtr else -1)
```

![](https://img-blog.csdnimg.cn/20190107103524278.png)

```python
# *式语法在迭代一个变长的元组序列时尤其有用
records = [('foo',1,2),('bar','hello'),('foo',3,4)]
def do_foo(x,y):
    print('foo',x,y)
def do_bar(s):
    print('bar',s)

for tar,*args in records:
    if tar == 'foo':
        do_foo(*args)
    elif tar == 'bar':
        do_bar(*args)
```

![](https://img-blog.csdnimg.cn/20190107103551370.png)

```python
#  当和某些特定的字符串处理操作相结合时，比如做拆分操作，这种*式语法所支持的分解操作也非常有用
line ='nobody:8:-1:-1:unprivilaged user:/var/empty:/usr/bin/false'
uname,*fileds,homedir,sh = line.split(':')
print(uname,homedir,sh)
print(fileds)
```

![](https://img-blog.csdnimg.cn/20190107103617198.png)

```python
#  *分解操作和各种函数式语言的列表处理功能有相似性,  下面这样可以轻松将列表分为头部和尾部
items = [1,10,4,6,87,8]
head,*tail = items
print(head,tail)
```

![](https://img-blog.csdnimg.cn/20190107103642685.png)

```python
#  丢弃分解的值，不能使用一个单独的*，但可以使用几个常用来表示待丢弃的变量名
record = ('acm',50,60.0,(1,2,2019))
name,*_,(*_,year) = record
print(name,year)
```

![](https://img-blog.csdnimg.cn/20190107103705237.png)



