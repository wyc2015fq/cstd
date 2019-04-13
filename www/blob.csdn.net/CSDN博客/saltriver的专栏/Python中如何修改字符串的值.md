
# Python中如何修改字符串的值 - saltriver的专栏 - CSDN博客


2016年08月12日 20:07:46[saltriver](https://me.csdn.net/saltriver)阅读数：15656个人分类：[Python																](https://blog.csdn.net/saltriver/article/category/6363186)



Python中列表list中的值是可修改的，而元组和字符串的值是不可修改的。看下面的示例：
string='abcdafg'
string[4] ='e'
print(string)输出结果：
string[4] = 'e'
TypeError: 'str' object does not support item assignment

但在实际应用中，经常需要对字符串的值进行一些修改操作，解决方案有3种：
**（1）使用字符串的replace函数**
**string****=****'abcdafg'**
**newstr =****string****.replace(****'a'****,****'e'****)**
**print****(****string****,****newstr****,****sep****=****'****\n****'****)**
输出结果：
abcdafg
ebcdefg

可以看出，replace会生成一个新的字符串，原字符串仍然不变。replace会修改字符串中所有相关的子串。很多时候并不能满足我们的应用要求。
**（2）将字符串转换成列表后修改值，然后用join组成新字符串**
string='abcdafg'
newstr =list(string)
newstr[4] ='e'
print(string,''.join(newstr),sep='\n')输出结果：
abcdafg
abcdefg
**（3）使用序列切片方式**
字符串本身也是一种序列，可以使用序列切片的方式。
[http://saltriver.blog.163.com/blog/static/2260971542016525103755389/](http://saltriver.blog.163.com/blog/static/2260971542016525103755389/)
string='abcdafg'
newstr =string[:4] +'e'+string[5:]
print(string,newstr,sep='\n')输出结果：
abcdafg
abcdefg



