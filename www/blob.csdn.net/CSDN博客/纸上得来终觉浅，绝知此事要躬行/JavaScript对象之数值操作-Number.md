# JavaScript对象之数值操作-Number - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月26日 14:35:22[boonya](https://me.csdn.net/boonya)阅读数：670标签：[javascript																[number](https://so.csdn.net/so/search/s.do?q=number&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[JavaScript/VueJS](https://blog.csdn.net/boonya/article/category/3273231)








Number数值运算在一般应用中都是相当常用的，如“四舍五入”，“格式化数值长度”，“保留几位小数”等操作都是很有必要的。



# Number对象描述





在 JavaScript 中，数字是一种基本的数据类型。JavaScript 还支持 Number 对象，该对象是原始数值的包装对象。在必要时，JavaScript 会自动地在原始数据和对象之间转换。在 JavaScript 1.1 中，可以用构造函数 Number() 明确地创建一个 Number 对象，尽管这样做并没有什么必要。

构造函数 Number() 可以不与运算符 new 一起使用，而直接作为转化函数来使用。以这种方式调用 Number() 时，它会把自己的参数转化成一个数字，然后返回转换后的原始数值（或 NaN）。

构造函数通常还用作 5 个有用的数字常量的占位符，这 5 个有用的数字常量分别是可表示的最大数、可表示的最小数、正无穷大、负无穷大和特殊的 NaN 值。注意，这些值是构造函数 Number() 自身的属性，而不是单独的某个 Number 对象的属性。
**比如这样使用属性 MAX_VALUE 是正确的：**


```java
var big = Number.MAX_VALUE
```



**但是这样是错误的：**



```java
var n= new Number(2);
var big = n.MAX_VALUE
```



作为比较，我们看一下 toString() 和 Number 对象的其他方法，它们是每个 Number 对象的方法，而不是 Number() 构造函数的方法。前面提到过，在必要时，JavaScript 会自动地把原始数值转化成 Number 对象，调用 Number 方法的既可以是 Number 对象，也可以是原始数字值。



```java
var n = 123;
var binary_value = n.toString(2);
```



# Number对象语法



```java
var myNum=new Number(value);
var myNum=Number(value);
```




### 参数



参数 *value* 是要创建的 Number 对象的数值，或是要转换成数字的值。

### 返回值

当 Number() 和运算符 new 一起作为构造函数使用时，它返回一个新创建的 Number 对象。如果不用 new 运算符，把 Number() 作为一个函数来调用，它将把自己的参数转换成一个原始的数值，并且返回这个值（如果转换失败，则返回 NaN）。
**注：NaN 意思是Not a Number 不是一个数值类型。**
**![](https://img-blog.csdn.net/20160826143441314)**

# Number操作示例

示例代码：




```
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN"
"http://www.w3.org/TR/html4/strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>Number TEST</title>
		<meta name="author" content="pengjunlin" />
		<!-- Date: 2016-08-26 -->
	</head>
	<script type="text/javascript">
		function Anumber() {
			var value = "128.8028302";
			
			var myNum = new Number(value);
			console.log("Number对象:"+myNum);

			var myNum = Number(value);
			console.log("Numebr构造:"+myNum);
			
			var myNum=128.8028302;
			console.log("toString把数字转换为字符串，使用指定的基数。"+myNum.toString());
			console.log("toFixed把数字转换为字符串，结果的小数点后有指定位数的数字。(2)"+myNum.toFixed(2));
			console.log("toPrecision把数字格式化为指定的长度。(6)"+myNum.toPrecision(6));
		}
	</script>
	<body>
        <input type="button"  value="点击获取数字" onblur="Anumber()"/>
	</body>
</html>
```


效果图：



![](https://img-blog.csdn.net/20160826142804868)





