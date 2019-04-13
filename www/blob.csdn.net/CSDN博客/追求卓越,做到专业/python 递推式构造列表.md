
# python 递推式构造列表 - 追求卓越,做到专业 - CSDN博客


2019年04月09日 23:23:24[Waldenz](https://me.csdn.net/enter89)阅读数：2标签：[python 递推式列表																](https://so.csdn.net/so/search/s.do?q=python 递推式列表&t=blog)[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[递推式列表																](https://so.csdn.net/so/search/s.do?q=递推式列表&t=blog)[递推式字典																](https://so.csdn.net/so/search/s.do?q=递推式字典&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=递推式列表&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[Python																](https://blog.csdn.net/enter89/article/category/7594775)
[
																								](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=python 递推式列表&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=python 递推式列表&t=blog)

递推式列表表达式
```python
new_list = [expression(i) for i in old_list if filter(i)]
```
举例：要创建一个列表，是从零到十的平方，普通写法如下
```python
squares = []
for x in range(11):
    squares.append(x**2)
```
递推式列表写法如下
```python
squares = [x**2 for x in range(11)]
```
`[0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100]`另外递推式列表支持嵌套写法，如找出30以内所有能构成直角三角形的数组：
```python
ret  = [(x,y,z) for x in range(1,30) for y in range(x,30) for z in range(y,30) if x**2 + y**2==z**2]
print(ret)
```
`[(3, 4, 5), (5, 12, 13), (6, 8, 10), (7, 24, 25), (8, 15, 17), (9, 12, 15), (10, 24, 26), (12, 16, 20), (15, 20, 25), (20, 21, 29)]`递推式字典和递推式列表类似，只是[]替换成了{}
```python
import string
print(string.ascii_lowercase)
# 递推式字典
dct = {c:ord(c) for c in string.ascii_lowercase}
print(dct)
# 递推式列表
lst = [ord(c) for c in string.ascii_lowercase]
print(lst)
```
![](https://img-blog.csdnimg.cn/20190409232022213.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
dct即为生成的字典，dict字典是无序的，lst为生成的列表，是有序的，为26个英文字母的ascii码值集合。
ord函数返回参数字符的ASCII数值

