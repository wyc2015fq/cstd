# python中sorted和.sorted  、reversed和reverse的注意点 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





L=[1,2,3,4]
l1=[123,123,23]
if l1.sort() == L.reverse():   #这个判断式是恒等的，因为两个函数的返回值都是None（其实是无返回值）
    print(1)

------------------------------------------------------------

a.sort()是对列表a进行原地修改，而且只能修改列表

而sorted（a）对字符串、列表、元组都能排序，该函数返回一个排序好的列表（都是列表！！）

------------------------------------------------------------

个人感觉reverse没什么diao用、当然原地修改还是要用reverse()的

因为对于字符串、列表、元组来说都可以用a[::-1]来取反

要注意a[::-1]返回的是一个逆转的列表、字符串、元组。并不是对a的原地修改

还有一种方法就是用.sort(reverse=1)来取反

-------------------------------------------------------------

.reverse()无返回值

使用reversed（）返回一个迭代器(迭代器的意思是只能通过循环来输出）
a = '321'
b = reversed(a)
print(b)

>>> <reversed object at 0x02E2B090>

显示的是返回了一个迭代器

迭代器用循环输出

for i in b:

　　print(i,end =' ')

>>> 1  2 3

为什么会和sorted不同呢

------------------------------------------------------------

用for i in reversed(a):

　　print(i)

发现数据输出竖着输出

改一个参数print(i,end=' ')将末尾的换行符\n换成‘ ’这样就不会自动换行了











