# 菜鸟升级记——C#流程控制语句 - Jun5203 - CSDN博客
2018年11月19日 16:15:50[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：84
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
一个应用程序，通常包含了复杂的应用逻辑，要实现这些逻辑，就必须进行流程控制，下面把5种流程控制语句分享给大家。
- if 语句
if 语句用来判断条件，符合条件则进入if 语句的代码块，不符合则执行if代码块后面的内容。
![](https://img-blog.csdnimg.cn/20181119130658526.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
if 语句的语法
```
if （a>0）{ }
```
如果有两个判断条件可使用if…else 的组合语句
```
if （a>0）{ }
else { }
```
举例
```
int a=15；
if （a>10）
{
 a--；
}
else
{
a++；
}
```
如果有多个判断条件，可使用if…else if 的组合语句
```
if（a>0）{ }
else if （a=0）{ }
else if （a<0）{ }
```
- switch 语句
switch语句也是条件判断语句，主要用于两个以上条件的判断。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119131859886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
switch语句的语法
```
switch （）
{
    case "":
        break;
    case "":
        break;
    default "":
        break;                
}
```
上述代码中，switch后面括号中的内容表示要判断的值。语句中的case关键字表示一种判断。关键字break表示此次判断结束并返回。default关键字是指如果括号中的内容与以上的所有case都不相符时，默认的执行语句。
举例
```
string city="北京"；
string welcome；
switch （city）
{
    case "北京":
        welcome="奥运北京"；
        break;
    case "上海":
        welcome="上海世博"；    
        break;
    defaut:
        welcome="欢迎您来到中国"；    
        break;                
}
```
- for 语句
for语句表示一个循环。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119133021151.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
for 语句的语法
```
for （初始值；布尔表达式；更新值）
布尔表达式是允许执行循环的条件
```
举例
```
string str="";
for（int i=1；i<11；i++）
    {
    str=str+i.Tostring（）；
    }
```
- foreach语句
foreach也是一个循环，通常用其实现对集合的遍历。因为此循环的条件不是布尔表达式，而是获取集合中的所有对象。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119134916630.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
foreach 语句的语法
```
foreach（集合个例 变量名 in 集合）
```
举例
```
//定义一个字符串数组
string [ ] arrayStr=new string [ ] { "北京"，"上海"，"深圳" }；
string strLong=""；
foreach（string str in arrayStr）  //遍历数组中的每个字符串
{
    strLong=strLong+str；     //l连接字符串
}
```
- while 语句
while语句是一个标准循环。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119161010169.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
while 语句的语法
```
while（布尔表达式）
如果值为true，则执行循环语句的内容
```
举例
```
int a=10；
string str=""；
while（a>10）
{
    str=str+a.ToString（）；
    a--；
}
```
学无止境，基础很重要！
