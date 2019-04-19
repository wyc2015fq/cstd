# Python基础(九) type元类 - _天枢 - 博客园
## [Python基础(九) type元类](https://www.cnblogs.com/yhleng/p/7779112.html)
2017-11-03 17:06 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7779112)
**python元类：****type()   **
**元类是python高阶语法. 合理的使用可以减少大量重复性的代码.**
**元类实际上做了以下三方面的工作：**
- **干涉创建类的过程**
- **修改类**
- **返回修改之后的类**
### 为什么使用元类？
**为什么要使用元类这种模糊且容易出错的功能？**
**一般情况下，我们并不会使用元类，99%的开发者并不会用到元类，所以一般不用考虑这个问题。**
**元类主用用于创建API，一个典型的例子就是Django的ORM。**
**它让我们可以这样定义一个类：**
```
class Person(models.Model):
  name = models.CharField(max_length=30)
  age = models.IntegerField()
```
**运行下面的代码：**
```
guy = Person(name='bob', age='35')
print(guy.age)
```
**返回的结果是`int`类型而不是`IntegerField`对象。这是因为`models.Model`使用了元类，它会将Python中定义的字段转换成数据库中的字段。**
**通过使用元类，Django将复杂的接口转换成简单的接口。**
**`原型：type(类名,基类元组(可以为空，用于继承), 包含属性或函数的字典)`**
** 以下两种写法都可以:**
**type('Class',(object,),dict(hello=fun()))**
**type('Class',(object,),{"hello":fun()})**
**1、class 自定义的类名称**
**2、（object,）是继承类，的元组，如果只有一个就写这种形势(object,)；多个(object,xxxx,)**
**3、dict(hello=fun()) 或 {"hello":fun()} 第三个参数，是一个字典等号左是 自定义的方法名，右侧是已写好的方法名，这个要注意，有参数且没有默认值的情况下，要加括号；**
```
def fun():
    print('hello world!')
if __name__=="__main__":
    h = type('Hello',(object,),dict(hello=fun()))
    tc = h()
    tc.hello
```
**引用：**
**h 相当于接收Hello类；tc = h()实例化类；tc.hello方法，调用的其实是我们定义的fun方法。**
```
Hello = type('Hello',(object,),dict(hello=fun()))
    tc = Hello()
    tc.hello
```
 type()动态创建类后,还可以添加更多的方法和属性:
```
def mysql():
    conn = pymysql.connect(host='127.0.0.1',port=3306 ,user='root' ,passwd='q123456' ,db='amsql' )
    cur = conn.cursor()
    sql = "SELECT * FROM amt_case_interface_table"
    ret = cur.execute(sql)
    print(cur.fetchmany(3))
    #conn.commit()
    cur.close()
    conn.close()
```
```
Hello.mysql = mysql()
```
调用:
```
tc.mysql
```
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,
