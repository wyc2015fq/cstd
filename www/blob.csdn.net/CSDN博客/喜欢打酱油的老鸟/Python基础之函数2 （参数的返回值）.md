
# Python基础之函数2 （参数的返回值） - 喜欢打酱油的老鸟 - CSDN博客


2019年03月04日 08:42:59[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：40标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6663685808244916739/](https://www.toutiao.com/a6663685808244916739/)

2019-03-02 15:04:15
![Python基础之函数2  （参数的返回值）](http://p1.pstatp.com/large/dfic-imagehandler/ec2b0f8b-d4ad-469f-a3b7-d5b104f2d2b0)

# 函数参数的类型
不可变类型参数：整数、字符串、元组。
fun(a) 内部修改a的值，只是修改另一个复制的对象，不会影响a本身
可变类型：列表，字典。
如fun(la)，则是将la真正的传过去，修改后的fun外部的la也会受到影响
```python
不可变数据类型
a = 10000
print('a标量的id值--->',id(a))
def num(b):
 print(b) # a和b的地址相同，因此值也是相同的
 b = 200
 print('修改后的b的id值--->', id(b))
 print(b)
num(a)
'''
a标量的id值---> 7459696
10000
修改后的b的id值---> 490814384
200
'''
```
**可变数据类型**
```python
print('outter_list',outter_list,id(outter_list))
def func(inter_list):
 print('inter_list',inter_list,id(inter_list))
 inter_list.append(4)
 print(inter_list)
func(outter_list) #传递的是一个地址，所以才会变
print(outter_list)
"""
st [1, 2, 3] 12221320
inter_list [1, 2, 3] 12221320
[1, 2, 3, 4]
[1, 2, 3, 4]
"""
```
![Python基础之函数2  （参数的返回值）](http://p1.pstatp.com/large/dfic-imagehandler/c5b00cbf-6291-41c8-a020-c0d5b246cfc2)

# 函数的返回值
格式
def 函数名（参数1，参数2，..........）
return 值
注意 1 在函数体的最后一个使用，关键字return 要返回的值
2 如果一个函数没有返回值，默认返回None
3 return有两个作用，一个是返回值，另一个是结束函数，可以结束整个方法
```python
def sum_num(aa,bb):
 num3 = aa+bb
 print(id(num3))
ret = sum_num(1,2) #定义一个变量ret，来接受函数的返回值，赋值给变量
print(ret,id(ret))
```
**拿到返回值后可以进行逻辑操作**
![Python基础之函数2  （参数的返回值）](http://p1.pstatp.com/large/dfic-imagehandler/162324d5-d1b6-49c4-b849-4c8abf6b68dc)
例如：
```python
if ret < 10:
 pass
else:
 pass
def func(c):
 print('func...')
 a = 10
 b = c
 if b < a:
 return #可以结束整个方法
 print('zzz')
 print('xxxx')
func(11)
print('程序结束了')
结果：
func...
zzz
xxxx
程序结束了
```
# 返回None
返回None
```python
def func():
 print('func....')
ret = func()
print(ret)
结果 func....
 None
```
# 返回多个值
```python
格式
def fun():
 a = '10'
 b = '20'
 return a,b 
ret = fun()
print(ret) #多个值被组织成一个元组返回 
 因为可以用多个变量来接受元组中的内容 因此
num1,num2 = fun()
print(num1,num2,type(num1),type(num2))
结果：
('10', '20')
10 20 <class 'str'> <class 'str'>
```
**注意:**
1如果函数不写返回值，默认返回值None
2 return关键字有两个作用，一个是返回内容，另一个是结束此方法
3 return关键字后面不允许在写代码

