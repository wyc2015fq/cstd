# JavaScript对象之字符串-String - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月24日 12:55:32[boonya](https://me.csdn.net/boonya)阅读数：566
所属专栏：[JavaScript学习提升](https://blog.csdn.net/column/details/jsmanlan.html)









String 对象用于处理文本（字符串）。字符串作为最基本的数据类型而广为使用。字符串数据用""或''表示。

# String语法



```java
new String(s);
String(s);
```


### 参数



参数 *s* 是要存储在 String 对象中或转换成原始字符串的值。

### 返回值

当 String() 和运算符 new 一起作为构造函数使用时，它返回一个新创建的 String 对象，存放的是字符串 *s* 或 *s* 的字符串表示。

当不用 new 运算符调用 String() 时，它只把 *s* 转换成原始的字符串，并返回转换后的值。
![](https://img-blog.csdn.net/20160824124741917)
![](https://img-blog.csdn.net/20160824124746909)





## String 对象描述

字符串是 JavaScript 的一种基本的数据类型。String 对象的 length 属性声明了该字符串中的字符数。

String 类定义了大量操作字符串的方法，例如从字符串中提取字符或子串，或者检索字符或子串。

需要注意的是，JavaScript 的字符串是不可变的（immutable），String 类定义的方法都不能改变字符串的内容。像 String.toUpperCase() 这样的方法，返回的是全新的字符串，而不是修改原始字符串。

在较早的 Netscape 代码基的 JavaScript 实现中（例如 Firefox 实现中），字符串的行为就像只读的字符数组。例如，从字符串 s 中提取第三个字符，可以用 s[2] 代替更加标准的 s.charAt(2)。此外，对字符串应用 for/in 循环时，它将枚举字符串中每个字符的数组下标（但要注意，ECMAScript 标准规定，不能枚举 length 属性）。因为字符串的数组行为不标准，所以应该避免使用它。

# String构造函数

语法：




```java
String(object);
```
示例：

```java
<script type="text/javascript">

var test1= new Boolean(1);
var test2= new Boolean(0);
var test3= new Boolean(true);
var test4= new Boolean(false);
var test5= new Date();
var test6= new String("999 888");
var test7=12345;

document.write(String(test1)+ "<br />");
document.write(String(test2)+ "<br />");
document.write(String(test3)+ "<br />");
document.write(String(test4)+ "<br />");
document.write(String(test5)+ "<br />");
document.write(String(test6)+ "<br />");
document.write(String(test7)+ "<br />");

</script>
```
输出：



```java
true
false
true
false
Wed Oct 28 00:17:40 UTC+0800 2009
999 888
12345
```




# String方法示例



```java
var str="HelloWorld!";
/* slice() */
document.write(str.slice(3)+"<br />");    // output -> 'loWorld!'
document.write(str.slice(5,10)+"<br />"); // output -> 'World'
document.write(str.slice(5,3)+"<br />");  // output -> 'null'
document.write(str.slice(3,-2)+"<br />");  // output -> 'loWorl'
/* substring() */
document.write(str.substring(3)+"<br />");    // output -> 'loWorld!'
document.write(str.substring(5,10)+"<br />"); // output -> 'World'
document.write(str.substring(5,3)+"<br />");  // output -> 'lo'
document.write(str.substring(3,-2)+"<br />"); /*-> document.write(str.substring(3,0)+"<br />"); -> document.write(str.substring(0,3)+"<br />");*/ // output -> 'Hel'
/* substr() */
document.write(str.substr(3)+"<br />");      // output -> 'loWorld!'
document.write(str.substr(5,3)+"<br />");    // output -> 'Wor'
document.write(str.substr(3,-2)+"<br />");   // output -> 'null'
document.write(str.substr(1,3)+"<br />");    // output -> 'ell'
```





