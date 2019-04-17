# JavaScript对象之数学函数运算-Math - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月23日 17:35:24[boonya](https://me.csdn.net/boonya)阅读数：3691
所属专栏：[JavaScript学习提升](https://blog.csdn.net/column/details/jsmanlan.html)










JavaScript封装了自己的函数库，下面来看看最常用的数学运算函数，以及封装使用的math.js工具包。

# Math基本属性和方法
![](https://img-blog.csdn.net/20160823174823827)



![](https://img-blog.csdn.net/20160823174836593)

# MathJs

![](https://img-blog.csdn.net/20160824100554238)


官方网址：[http://mathjs.org/](http://mathjs.org/)



# An extensive math library for JavaScript and Node.js



Math.js is an extensive math library for JavaScript and Node.js.It features a flexible expression parser and offers an integrated solutionto work with numbers, big numbers, complex numbers, units, and matrices.Powerful and easy to use.

# Features 
- Supports numbers, big numbers, fractions, complex numbers, units, strings, arrays, and matrices.
- Is compatible with JavaScript’s built-in Math library.
- Contains a flexible expression parser.
- Supports chained operations.
- Comes with a large set of built-in functions and constants.
- Has no dependencies. Runs on any JavaScript engine.
- Is easily extensible.
- Open source.

JS工具包引用示例：



```java
<!DOCTYPE HTML>
<html>
<head>
  <script src="math.js" type="text/javascript"></script>
</head>
<body>
  <script type="text/javascript">
    // use math.js
    math.sqrt(-4); // 2i
  </script>
</body>
</html>
```


更多函数示例：





```java
// functions and constants
math.round(math.e, 3);            // 2.718
math.atan2(3, -3) / math.pi;      // 0.75
math.log(10000, 10);              // 4
math.sqrt(-4);                    // 2i
math.pow([[-1, 2], [3, 1]], 2);
     // [[7, 0], [0, 7]]

// expressions
math.eval('1.2 * (2 + 4.5)');     // 7.8
math.eval('5.08 cm to inch');     // 2 inch
math.eval('sin(45 deg) ^ 2');     // 0.5
math.eval('9 / 3 + 2i');          // 3 + 2i
math.eval('det([-1, 2; 3, 1])');  // -7

// chaining
math.chain(3)
    .add(4)
    .multiply(2)
    .done(); // 14
```







