
# [Java] 学习笔记一(String,for循环,多线程,正则表达式,map用法) - Multiangle's Notepad - CSDN博客


2016年03月20日 19:14:03[multiangle](https://me.csdn.net/u014595019)阅读数：1218标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[java																](https://blog.csdn.net/u014595019/article/category/2911533)[零零碎碎																](https://blog.csdn.net/u014595019/article/category/2176549)[
							](https://blog.csdn.net/u014595019/article/category/2911533)



## String
**String.equals (String)**
判断两个字符串对象的内容是否相同。
值得注意的是，String.equals与’==’操作不同。前者比较的是两者的内容是否相同，而后者比较的是两者的地址是否相同。举个栗子，
```python
public
```
```python
class
```
```python
stringtest {
```
```python
public
```
```python
static
```
```python
void
```
```python
main
```
```python
(String[] args){
        String S1=
```
```python
new
```
```python
String(
```
```python
"abc"
```
```python
);
        String S2=
```
```python
"abc"
```
```python
;
        System.
```
```python
out
```
```python
.println(S1==S2);
        System.
```
```python
out
```
```python
.println(S1.equals(S2));
    }
}
```
```python
// 得到的输出结果是：
```
```python
// false
```
```python
// true
```
**String.concat (String)**
进行字符串的连接，将两个字符串连接以后形成一个新的字符串
**String.indexOf (int)**
查找特定字符或字符串在当前字符串中的**起始**位置
当然，也可以从特定位置以后查找对应的字符，例如：int index = s.indexOf(‘d’,4);
**String.length (void)**
返回字符串的长度
**String.charAt (int)**
按照索引值获得字符串中的指定字符
**String.getBytes**
将字符串转换为对应的byte数组，以便于数据的存储和传输
```python
String s = “计算机”;
```
```python
byte
```
```python
[] b = s.getBytes();
```
```python
//使用本机默认的字符串转换为byte数组
```
```python
byte
```
```python
[] b = s.getBytes(“gb2312”);
```
```python
//使用gb2312字符集转换为byte数组
```
**String.replace (char,char)**
**String.replaceAll (String,String)**
**String.replaceFirst (String, String)**
第一个替换字符，第二个替换字符串，第三个替换第一个字符串
**String.split (String)**
以特定的字符串作为间隔，将当前字符串拆分为字符串数组
**String.substring (int a,int b)**
取字符串中的“子串”。如果只有一个参数，则从该参数取至字符串结尾。如果有两个，则取[a,b)范围内的子串
**String.valueOf (x)**
将其它类型的数据转换为字符串类型。比如将int转化为String格式
**String.format (Pattern, Obj….args)**
这个跟上面的不同。这边的String不是指的变量名，而是类名。举个栗子，
```python
public
```
```python
class
```
```python
stringtest {
```
```python
public
```
```python
static
```
```python
void
```
```python
main
```
```python
(String[] args){
        String x=String.format(
```
```python
"%s alksdjf"
```
```python
,
```
```python
"fwlekrj"
```
```python
) ;
        System.
```
```python
out
```
```python
.println(x);
    }
}
```
```python
//这样即可得到输出结果  fwlekrj alksdjf
```
具体的转换符类型，即%s,%d这些代表了什么，可以参照[这里](http://blog.csdn.net/lonely_fireworks/article/details/7962171)
**Others**
String.compareTo
String.endsWith
String.startsWith
String.toCharArray
String.toLowerCase
String.toUpperCase
---两种for循环

---第一种

---，最古老的办法，

---int

---[]

---a=

---{

---1

---,

---2

---,

---3

---,

---4

---,

---5

---}

---;

---for

---(int

---i

---=

---0

---;

---i

---<

---a.

---length

---;

---i

---++)

---System.

---out.

---println(a

---[i]

---);

---对应python，是

---a

---=[

---1

---,

---2

---,

---3

---,

---4

---,

---5

---]

---for

---i

---in

---range(

---a

---.__len__()):
    print(

---a

---[i])

---第二种

---，比较简洁

---int

---[] a={

---1

---,

---2

---,

---3

---,

---4

---,

---5

---} ;

---for

---(

---int

---i:a) System.

---out

---.println(i);

---对应Python，是

---a

---=[

---1

---,

---2

---,

---3

---,

---4

---,

---5

---]

---for

---i

---in

---a

---:
    print(i)

---这两种方法各有优劣

---，第一种适合于要用到当前数据在数组中位置的时候，比如同步更改两个数组；

---而第二个方法，遍历更方便，可以直接操作数据，而不必再用a[i]这样的操作

---多线程

---public

---class

---threadTest

---extends

---Thread

---{

---private

---int

---id ;

---public

---threadTest

---(

---int

---id){

---this

---.id=id ;
    }

---public

---void

---run

---(){
        System.out.println(

---"threadTest is started!"

---);

---while

---(

---true

---){
            String temp=String.format(

---"%dth process is started"

---,

---this

---.id) ;
            System.out.println(temp);

---try

---{
                Thread.sleep(

---1000

---);
                System.out.printf(

---"%dth process is reporting\n"

---,

---this

---.id);
            }

---catch

---(InterruptedException e){
            }
        }
    }
}

---注意，上面类继承自Thread类。其中public void run()里面放置该线程开始运行之后的操作代码。

---该类的构造函数可以不写，但是如果需要传入参数的话，还是要写的。

---与python不同

---，python需要手动初始化父类，会有例如

---threading.Thread.__init__(self)

---这样的代码，调用子类的第一件事就是去找被继承的父类，所以不需要用户自己初始化父类~

---上面的写好之后，再新建一个文件，里面写

---public

---class

---test {

---public

---static

---void

---main

---(String[] args){
        threadTest thread1  =

---new

---threadTest(

---1

---) ;
        threadTest thread2  =

---new

---threadTest(

---2

---) ;
        thread1.start();
        thread2.start();
    }
}

---其中

---threadTest thread1  =   new threadTest(1) ;

---是初始化，这里会调用构造函数

---而

---thread1.start();

---则是开启进程，这里会调用上面的

---public void run()

---函数。当run函数中的代码执行完毕时，该线程也就结束了。所以如果想要让这段线程常驻，可以写个死循环。

---正则表达式

---详细的用法可以参照

---[这篇博客](http://blog.csdn.net/allwefantasy/article/details/3136570)

---或者

---[正则表达式教程](http://www.runoob.com/java/java-regular-expressions.html)

---. 这里只写出最简单明了的用法

---查找：

---import java

---.util

---.ArrayList

---;

---import java

---.util

---.List

---;

---import java

---.util

---.regex

---.Matcher

---;

---import java

---.util

---.regex

---.Pattern

---;

---public class test {
    public static void main(String[] args){
        List<String> allMatches = new ArrayList<String>()

---;

---String regEx =

---"<a>([\\s\\S]*?)</a>"

---;

---String s =

---"<a>123</a><a>456</a><a>789</a>"

---;

---Pattern pat = Pattern

---.compile

---(regEx)

---;

---Matcher mat = pat

---.matcher

---(s)

---;

---// 方法一，返回第一个匹配值
        if (mat

---.find

---()){
            System

---.out

---.println

---(mat

---.group

---())

---;

---}
        // 方法二，返回所有匹配值
        while (mat

---.find

---()){
            allMatches

---.add

---(mat

---.group

---())

---;

---}
        for (String i:allMatches){
            System

---.out

---.println

---(i)

---;

---}
    }
}

---Map用法

---具体资料可以参照

---[这里](http://www.zhixing123.cn/jsp/30113.html)

---使用Map的

---[demo](https://examples.javacodegeeks.com/java-basics/java-map-example/)

---Map的常用操作有：

---V

---get

---(

---Object

---key

---)       返回指定键所映射的值；如果此映射不包含该键的映射关系，则返回 null。
V put(K

---key

---, V value)   将指定的值与此映射中的指定键关联。
void putAll(Map<? extends K,? extends V> m)  从指定映射中将所有映射关系复制到此映射中
void clear()            从此映射中移除所有映射关系

---boolean

---containsKey(

---Object

---key

---)     如果此映射包含指定键的映射关系，则返回

---true

---。

---boolean

---containsValue(

---Object

---value) 如果此映射将一个或多个键映射到指定值，则返回

---true

---。

---Set

---<Map.Entry<K,V>> entrySet()      返回此映射中包含的映射关系的

---Set

---视图。

---boolean

---equals

---(

---Object

---o)            比较指定的对象与此映射是否相等。
int hashCode()                      返回此映射的哈希码值。

---boolean

---isEmpty()       如果此映射未包含键-值映射关系，则返回

---true

---。

---Set

---<K> keySet()         返回此映射中包含的键的

---Set

---视图。
V remove(

---Object

---key

---)    如果存在一个键的映射关系，则将其从此映射中移除（可选操作）。
int size()              返回此映射中的键-值映射关系数。
Collection<V> values()  返回此映射中包含的值的 Collection 视图。


