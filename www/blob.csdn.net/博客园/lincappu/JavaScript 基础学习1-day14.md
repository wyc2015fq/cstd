# JavaScript 基础学习1-day14 - lincappu - 博客园







# [JavaScript 基础学习1-day14](https://www.cnblogs.com/lincappu/p/8157532.html)






# JavaScript 基础学习1

知识预览
JavaScript概述
二 JavaScript的基础
三 JavaScript的对象
BOM对象
DOM对象
实例练习
js扩展

## JavaScript概述

### JavaScript的历史

1992年Nombas开发出C-minus-minus(C--)的嵌入式脚本语言(最初绑定在CEnvi软件中).后将其改名ScriptEase.(客户端执行的语言)

Netscape(网景)接收Nombas的理念,(Brendan Eich)在其Netscape Navigator 2.0产品中开发出一套livescript的脚本语言.Sun和Netscape共同完成.后改名叫Javascript

微软随后模仿在其IE3.0的产品中搭载了一个JavaScript的克隆版叫Jscript.

为了统一三家,ECMA(欧洲计算机制造协会)定义了ECMA-262规范.国际标准化组织及国际电工委员会（ISO/IEC）也采纳 ECMAScript 作为标准（ISO/IEC-16262）。从此，Web 浏览器就开始努力（虽然有着不同的程度的成功和失败）将 ECMAScript 作为 JavaScript 实现的基础。EcmaScript是规范.

### ECMAScript

尽管 ECMAScript 是一个重要的标准，但它并不是 JavaScript 唯一的部分，当然，也不是唯一被标准化的部分。实际上，一个完整的 JavaScript 实现是由以下 3 个不同部分组成的：
- 核心（ECMAScript）
- 文档对象模型（DOM） Document object model (整合js，css，html)
- 浏览器对象模型（BOM） Broswer object model（整合js和浏览器）
- Javascript 在开发中绝大多数情况是基于对象的.也是面向对象的.
![](http://i.imgur.com/JhOmZGd.png)

简单地说，ECMAScript 描述了以下内容：
- 语法
- 类型
- 语句
- 关键字
- 保留字
- 运算符
- 对象 (封装 继承 多态) 基于对象的语言.使用对象.

## 二 JavaScript的基础

### 2.1 JS的引入方式

### 1 直接编写

```
<script>
    alert('hello yuan')
</script>
```

### 2 导入文件
`<script src="hello.js"></script>`
## 2.2 JS的变量、常量和标识符

### 2.2.1 JS的变量

```
x=5
y=6
z=x+y
```

在代数中，我们使用字母（比如 x）来保存值（比如 5）。通过上面的表达式 z=x+y，我们能够计算出 z 的值为 11。在 JavaScript 中，这些字母被称为变量。

那么如何在JS中定义使用变量呢？

1、声明变量时不用声明变量类型. 全都使用var关键字;
`var a;<br>a=3;`
2、一行可以声明多个变量.并且可以是不同类型
`var name="yuan", age=20, job="lecturer";`
3、声明变量时 可以不用var. 如果不用var 那么它是全局变量

4、变量命名,首字符只能是字母,下划线,$美元符 三选一，余下的字符可以是下划线、美元符号或任何字母或数字字符且区分大小写，x与X是两个变量　

```php
Camel 标记法
首字母是小写的，接下来的字母都以大写字符开头。例如：
var myTestValue = 0, mySecondValue = "hi";
Pascal 标记法
首字母是大写的，接下来的字母都以大写字符开头。例如：
Var MyTestValue = 0, MySecondValue = "hi";
匈牙利类型标记法
在以 Pascal 标记法命名的变量前附加一个小写字母（或小写字母序列），
说明该变量的类型。例如，i 表示整数，s 表示字符串，如下所示“
Var iMyTestValue = 0, sMySecondValue = "hi";
```

### 2.2.2 常量和标识符

常量 ：直接在程序中出现的数据值

标识符：

由不以数字开头的字母、数字、下划线(_)、美元符号($)组成
常用于表示函数、变量等的名称
例如：_abc,$abc,abc,abc123是标识符，而1abc不是
JavaScript语言中代表特定含义的词称为保留字，不允许程序再定义为标识符

![](http://i.imgur.com/rWo0uZO.png)


## 2.3 JS的数据类型

![](http://i.imgur.com/0TyZ7D5.png)

```
number     -----  数值
    boolean    -----  布尔值
    string     -----  字符串
    undefined  -----  undefined
    null       -----   null
```

### 2.3.1 数字类型(number)

![](http://i.imgur.com/neiWtJc.png)
- 不区分整型数值和浮点型数值;
- 所有数字都采用64位浮点格式存储，相当于Java和C语言中的double格式
- 能表示的最大值是±1.7976931348623157 x 10308
- 能表示的最小值是±5 x 10 -324 　

整数：

```
在JavaScript中10进制的整数由数字的序列组成
       精确表达的范围是 -9007199254740992 (-253) 到 9007199254740992 (253)
       超出范围的整数，精确度将受影响
```

浮点数：

```
使用小数点记录数据
       例如：3.4，5.6
       使用指数记录数据
       例如：4.3e23 = 4.3 x 1023
```

16进制和8进制数的表达:

```
16进制数据前面加上0x，八进制前面加0;16进制数是由0-9,A-F等16个字符组成;8进制数由0-7等8个数字组成

       16进制和8进制与2进制的换算:
```

2进制: 1111 0011 1101 0100 <-----> 16进制:0xF3D4 <-----> 10进制:62420
2进制: 1 111 001 111 010 100 <-----> 8进制:0171724

### 2.3.2 字符串类型(string)

是由Unicode字符、数字、标点符号组成的序列；字符串常量首尾由单引号或双引号括起；JavaScript中没有字符类型；常用特殊字符在字符串中的表达；
字符串中部分特殊字符必须加上右划线；常用的转义字符 \n:换行 ':单引号 ":双引号 \:右划线

### 2.3.3 布尔类型(boolean)

Boolean类型仅有两个值：true和false，也代表1和0，实际运算中true=1,false=0
布尔值也可以看作on/off、yes/no、1/0对应true/false
Boolean值主要用于JavaScript的控制语句，例如：

```cpp
if (x==1){
      y=y+1;
}else{
      y=y-1;
      }
```

### 2.3.4 Null & Undefined类型

### Undefined类型

Undefined 类型只有一个值，即 undefined。当声明的变量未初始化时，该变量的默认值是 undefined。

当函数无明确返回值时，返回的也是值 "undefined";

### Null类型

另一种只有一个值的类型是 Null，它只有一个专用值 null，即它的字面量。值 undefined 实际上是从值 null 派生来的，因此 ECMAScript 把它们定义为相等的。

尽管这两个值相等，但它们的含义不同。undefined 是声明了变量但未对其初始化时赋予该变量的值，null 则用于表示尚未存在的对象（在讨论 typeof 运算符时，简单地介绍过这一点）。如果函数或方法要返回的是对象，那么找不到该对象时，返回的通常是 null。

## 2.4 运算符

算术运算符：
+ - * / % ++ --

比较运算符：
> >= < <= != == === !==

逻辑运算符：
&& || ！

赋值运算符：
= += -= *= /=

字符串运算符：
+ 连接，两边操作数有一个或两个是字符串就做连接运算

### 2.4.1 算术运算符

#### 注意1: 自加自减

假如x=2，那么x++表达式执行后的值为3，x--表达式执行后的值为1；i++相当于i=i+1，i--相当于i=i-1；
递增和递减运算符可以放在变量前也可以放在变量后：--i

```matlab
var i=10;
console.log(i++);
console.log(i);
console.log(++i);
console.log(i);
console.log(i--);
console.log(--i);
```

#### 注意2: 单元运算符
- 
除了可以表示减号还可以表示负号 例如：x=-y

- 
除了可以表示加法运算还可以用于字符串的连接 例如："abc"+"def"="abcdef"
js不同于python，是一门弱类型语言


#### 注意3: NaN

```java
var d="yuan";
d=+d;
alert(d);//NaN:属于Number类型的一个特殊值,当遇到将字符串转成数字无效时,就会得到一个NaN数据
alert(typeof(d));//Number

//NaN特点:

var n=NaN;

alert(n>3);
alert(n<3);
alert(n==3);
alert(n==NaN);

alert(n!=NaN);//NaN参与的所有的运算都是false,除了!=
```

### 2.4.2 比较运算符

> 
> 
= < <= != == === !==
用于控制语句时：



```
if (2>1){       //  3  0  false null undefined ［］
        console.log("条件成立!")
    }
```

等号和非等号的同类运算符是全等号和非全等号。这两个运算符所做的与等号和非等号相同，只是它们在检查相等性前，不执行类型转换。

```cpp
console.log(2==2);
console.log(2=="2");
console.log(2==="2");
console.log(2!=="2");
```

### 注意1：

```
var bResult = "Blue" < "alpha";
alert(bResult); //输出 true
```

在上面的例子中，字符串 "Blue" 小于 "alpha"，因为字母 B 的字符代码是 66，字母 a 的字符代码是 97。

比较数字和字符串

另一种棘手的状况发生在比较两个字符串形式的数字时，比如：

```
var bResult = "25" < "3";
alert(bResult); //输出 "true"
```

上面这段代码比较的是字符串 "25" 和 "3"。两个运算数都是字符串，所以比较的是它们的字符代码（"2" 的字符代码是 50，"3" 的字符代码是 51）。

不过，如果把某个运算数该为数字，那么结果就有趣了：

```
var bResult = "25" < 3;
alert(bResult); //输出 "false"
```

这里，字符串 "25" 将被转换成数字 25，然后与数字 3 进行比较，结果不出所料。

总结：

比较运算符两侧如果一个是数字类型,一个是其他类型,会将其类型转换成数字类型.
比较运算符两侧如果都是字符串类型,比较的是最高位的asc码,如果最高位相等,继续取第二位比较.

### 注意2：

等性运算符：执行类型转换的规则如下：

如果一个运算数是 Boolean 值，在检查相等性之前，把它转换成数字值。false 转换成 0，true 为 1。
如果一个运算数是字符串，另一个是数字，在检查相等性之前，要尝试把字符串转换成数字。
如果一个运算数是对象，另一个是字符串，在检查相等性之前，要尝试把对象转换成字符串。
如果一个运算数是对象，另一个是数字，在检查相等性之前，要尝试把对象转换成数字。
在比较时，该运算符还遵守下列规则：

值 null 和 undefined 相等。
在检查相等性时，不能把 null 和 undefined 转换成其他值。
如果某个运算数是 NaN，等号将返回 false，非等号将返回 true。
如果两个运算数都是对象，那么比较的是它们的引用值。如果两个运算数指向同一对象，那么等号返回 true，否则两个运算数不等。

### 2.4.3 逻辑运算符

```cpp
if (2>1 && [1,2]){
    console.log("条件与")
}

// 思考返回内容?
console.log(1 && 3);
console.log(0 && 3);
console.log(0 || 3);
console.log(2 || 3);
```

## 2.5 流程控制
- 顺序结构(从上向下顺序执行)
- 分支结构
- 
循环结构

### 2.5.1 顺序结构

### 2.5.2 分支结构

if-else结构：

if (表达式){
语句１;
......
} else{
语句２;
.....
}
功能说明：如果表达式的值为true则执行语句1,否则执行语句2


### 示例：

```java
var x= (new Date()).getDay();
        //获取今天的星期值，0为星期天
        var y;

        if ( (x==6) || (x==0) ) {
            y="周末";
        }else{
            y="工作日";
            }
        
        console.log(y);

        /*等价于

        y="工作日";
        if ( (x==6) || (x==0) ) {
        y="周末";
        }
        console.log(y);  */
```

### if-elif－else结构：

```java
if (表达式1) {
    语句1;
}else if (表达式2){
    语句2;
}else if (表达式3){
    语句3;
} else{
    语句4;
}

var score=window.prompt("您的分数:");

if (score>90){
    ret="优秀";
}else if (score>80){
    ret="良";
}else if (score>60){
    ret="及格";
}else {
    ret = "不及格";

}
alert(ret);
```

### switch-case结构

```cpp
switch基本格式
switch (表达式) {
    case 值1:语句1;break;
    case 值2:语句2;break;
    case 值3:语句3;break;
    default:语句4;
}
```

![](http://i.imgur.com/ZTLD1hE.png)

```
switch(x){
case 1:y="星期一";    break;
case 2:y="星期二";    break;
case 3:y="星期三";    break;
case 4:y="星期四";    break;
case 5:y="星期五";    break;
case 6:y="星期六";    break;
case 7:y="星期日";    break;
default: y="未定义";
}
switch比else if结构更加简洁清晰，使程序可读性更强,效率更高。
```

### 2.5.2 循环结构

![](http://i.imgur.com/bANZDq7.png)
循环语句流程图

### for循环：

语法规则：

```cpp
for(初始表达式;条件表达式;自增或自减)
{
        执行语句
        ……
}
```

功能说明：实现条件循环，当条件成立时，执行语句1，否则跳出循环体

for循环的另一种形式：

for( 变量 in 数组或对象)

{
执行语句
……
}

### while循环：

语法规则：

```
while (条件){
    语句1；
    ...
}
```

功能说明：运行功能和for类似，当条件成立循环执行语句花括号{}内的语句，否则跳出循环；同样支持continue与break语句。

### 示例：

```java
var i=1;
while (i<=7) {
    document.write("<H"+i+">hello</H "+i+"> ");
    document.write("<br>");
    i++;
}
//循环输出H1到H7的字体大小
```

### 2.5.3 异常处理

```
try {
    //这段代码从上往下运行，其中任何一个语句抛出异常该代码块就结束运行
}
catch (e) {
    // 如果try代码块中抛出了异常，catch代码块中的代码就会被执行。
    //e是一个局部变量，用来指向Error对象或者其他抛出的对象
}
finally {
     //无论try中代码是否有异常抛出（甚至是try代码块中有return语句），finally代码块中始终会被执行。
}
```

注：主动抛出异常 throw Error('xxxx')

## 三 JavaScript的对象

简介：

在JavaScript中除了null和undefined以外其他的数据类型都被定义成了对象，也可以用创建对象的方法定义变量，String、Math、Array、Date、RegExp都是JavaScript中重要的内置对象，在JavaScript程序大多数功能都是基于对象实现的。

```
<script language="javascript">
var aa=Number.MAX_VALUE; 
//利用数字对象获取可表示最大数
var bb=new String("hello JavaScript"); 
//创建字符串对象
var cc=new Date();
//创建日期对象
var dd=new Array("星期一","星期二","星期三","星期四"); 
//数组对象
</script>
```

### 3.1 String对象

字符串对象创建

字符串创建(两种方式)

```java
① 变量 = “字符串”
   ② 字串对象名称 = new String (字符串)

var str1="hello world";
var str1= new String("hello word");
```

字符串对象的属性和函数
x.length －－－－获取字符串的长度

```
x.toLowerCase()        －－－－转为小写
 x.toUpperCase()        －－－－转为大写
 x.trim()               －－－－去除字符串两边空格
```

### 字符串查询方法

```
x.charAt(index)         －－－－str1.charAt(index);－－－－获取指定位置字符，其中index为要获取的字符索引

x.indexOf(findstr,index)－－－－查询字符串位置
x.lastIndexOf(findstr)  

x.match(regexp)         －－－－match返回匹配字符串的数组，如果没有匹配则返回null
x.search(regexp)        －－－－search返回匹配字符串的首字符位置索引

                        示例：
                        var str1="welcome to the world of JS!";
                        var str2=str1.match("world");
                        var str3=str1.search("world");
                        alert(str2[0]);  // 结果为"world"
                        alert(str3);     // 结果为15
```

### 子字符串处理方法

```
x.substr(start, length) －－－－start表示开始位置，length表示截取长度
x.substring(start, end) －－－－end是结束位置

x.slice(start, end)     －－－－切片操作字符串
                    示例：
                        var str1="abcdefgh";
                        var str2=str1.slice(2,4);
                        var str3=str1.slice(4);
                        var str4=str1.slice(2,-1);
                        var str5=str1.slice(-3,-1);

                        alert(str2); //结果为"cd"
                        
                        alert(str3); //结果为"efgh"
                        
                        alert(str4); //结果为"cdefg"
                        
                        alert(str5); //结果为"fg"

x.replace(findstr,tostr) －－－－    字符串替换

x.split();                 －－－－分割字符串
                             var str1="一,二,三,四,五,六,日"; 
                            var strArray=str1.split(",");
                            alert(strArray[1]);//结果为"二"
                            
x.concat(addstr)         －－－－    拼接字符串
```

### 3.2 Array对象

### 3.2.1 数组创建

创建数组的三种方式：

### 创建方式1:
`var arrname = [元素0,元素1,….];          // var arr=[1,2,3];`
### 创建方式2:

```java
var arrname = new Array(元素0,元素1,….); // var test=new Array(100,"a",true);
```

### 创建方式3:

```java
var arrname = new Array(长度); 
        //  初始化数组对象:
            var cnweek=new Array(7);
                cnweek[0]="星期日";
                cnweek[1]="星期一";
                ...
                cnweek[6]="星期六";
```

创建二维数组：

```
var cnweek=new Array(7);
for (var i=0;i<=6;i++){
    cnweek[i]=new Array(2);
}
cnweek[0][0]="星期日";
cnweek[0][1]="Sunday";
cnweek[1][0]="星期一";
cnweek[1][1]="Monday";
...
cnweek[6][0]="星期六";
cnweek[6][1]="Saturday";
```

### 3.2.2 数组对象的属性和方法

#### join方法：

```
x.join(bystr)       －－－－将数组元素拼接成字符串

var arr1=[1, 2, 3, 4, 5, 6, 7];
var str1=arr1.join("-");
alert(str1);  //结果为"1-2-3-4-5-6-7"
```

#### concat方法：

```
x.concat(value,...)    －－－－ 
var a = [1,2,3];
var b=a.concat(4,5) ;
alert(a.toString());  //返回结果为1,2,3            
alert(b.toString());  //返回结果为1,2,3,4,5
```

#### 数组排序-reverse sort：

```java
//x.reverse()
//x.sort()

var arr1=[32, 12, 111, 444];
//var arr1=["a","d","f","c"];

arr1.reverse(); //颠倒数组元素
alert(arr1.toString());
//结果为444,111,12,32

arr1.sort();    //排序数组元素
alert(arr1.toString());
//结果为111,12,32,444

//------------------------------
arr=[1,5,2,100];

//arr.sort();
//alert(arr);
//如果就想按着数字比较呢?

function intSort(a,b){
    if (a>b){
        return 1;//-1
    }
    else if(a<b){
        return -1;//1
    }
    else {
        return 0
    }
}

arr.sort(intSort);

alert(arr);

function IntSort(a,b){
    return a-b;
}
```

#### 数组切片操作：

```
//x.slice(start, end)
//
//使用注解
//
//x代表数组对象
//start表示开始位置索引
//end是结束位置下一数组元素索引编号
//第一个数组元素索引为0
//start、end可为负数，-1代表最后一个数组元素
//end省略则相当于从start位置截取以后所有数组元素

var arr1=['a','b','c','d','e','f','g','h'];
var arr2=arr1.slice(2,4);
var arr3=arr1.slice(4);
var arr4=arr1.slice(2,-1);

alert(arr2.toString());
//结果为"c,d" 
alert(arr3.toString());
//结果为"e,f,g,h"
alert(arr4.toString());
//结果为"c,d,e,f,g"
```

#### 删除子数组：

```
//x. splice(start, deleteCount, value, ...)

//使用注解

//x代表数组对象
//splice的主要用途是对数组指定位置进行删除和插入
//start表示开始位置索引
//deleteCount删除数组元素的个数
//value表示在删除位置插入的数组元素
//value参数可以省略       
        
var a = [1,2,3,4,5,6,7,8];
a.splice(1,2);

alert(a.toString());//a变为 [1,4,5,6,7,8]

a.splice(1,1);

alert(a.toString());//a变为[1,5,6,7,8]

a.splice(1,0,2,3);

alert(a.toString());//a变为[1,2,3,5,6,7,8]
```

#### 数组的push和pop：

```
//push pop这两个方法模拟的是一个栈操作

//x.push(value, ...)  压栈
//x.pop()             弹栈      
//使用注解
//
//x代表数组对象
//value可以为字符串、数字、数组等任何值
//push是将value值添加到数组x的结尾
//pop是将数组x的最后一个元素删除


var arr1=[1,2,3];
arr1.push(4,5);
alert(arr1);
//结果为"1,2,3,4,5"
arr1.push([6,7]);
alert(arr1)
//结果为"1,2,3,4,5,6,7"
arr1.pop();
alert(arr1);
//结果为"1,2,3,4,5"
```

#### 数组的shift和unshift：

```
//x.unshift(value,...)
//x.shift()
//使用注解

//x代表数组对象
//value可以为字符串、数字、数组等任何值
//unshift是将value值插入到数组x的开始
//shift是将数组x的第一个元素删除

var arr1=[1,2,3];
arr1.unshift(4,5);
alert(arr1);  //结果为"4,5,1,2,3"

arr1. unshift([6,7]);
alert(arr1);  //结果为"6,7,4,5,1,2,3"

arr1.shift();
alert(arr1);  //结果为"4,5,1,2,3"
```

### 总结js的数组特性：

```java
//  js中数组的特性
     //java中数组的特性,  规定是什么类型的数组,就只能装什么类型.只有一种类型.
     //js中的数组特性1: js中的数组可以装任意类型,没有任何限制.
     //js中的数组特性2: js中的数组,长度是随着下标变化的.用到多长就有多长.
     var arr5 = ['abc',123,1.14,true,null,undefined,new String('1213'),new Function('a','b','alert(a+b)')];
    /*  alert(arr5.length);//8
     arr5[10] = "hahaha";
     alert(arr5.length); //11
     alert(arr5[9]);// undefined */
```

## 3.3 Date对象

### 3.3.1 创建Date对象

//方法1：不指定参数

```
var nowd1=new Date();
alert(nowd1.toLocaleString( ));
```

//方法2：参数为日期字符串

```
var nowd2=new Date("2004/3/20 11:12");
alert(nowd2.toLocaleString( ));
var nowd3=new Date("04/03/20 11:12");
alert(nowd3.toLocaleString( ));
```

//方法3：参数为毫秒数

```
var nowd3=new Date(5000);
alert(nowd3.toLocaleString( ));
alert(nowd3.toUTCString());
```

//方法4：参数为年月日小时分钟秒毫秒

```java
var nowd4=new Date(2004,2,20,11,12,0,300);
alert(nowd4.toLocaleString( ));//毫秒并不直接显示
```

### Date对象的方法—获取日期和时间

```
获取日期和时间
getDate()                 获取日
getDay ()                 获取星期
getMonth ()               获取月（0-11）
getFullYear ()            获取完整年份
getYear ()                获取年
getHours ()               获取小时
getMinutes ()             获取分钟
getSeconds ()             获取秒
getMilliseconds ()        获取毫秒
getTime ()                返回累计毫秒数(从1970/1/1午夜)
```

### 实例练习：

```java
function getCurrentDate(){
    //1. 创建Date对象
    var date = new Date(); //没有填入任何参数那么就是当前时间
    //2. 获得当前年份
    var year = date.getFullYear();
    //3. 获得当前月份 js中月份是从0到11.
    var month = date.getMonth()+1;
    //4. 获得当前日
    var day = date.getDate();
    //5. 获得当前小时
    var hour = date.getHours();
    //6. 获得当前分钟
    var min = date.getMinutes();
    //7. 获得当前秒
    var sec = date.getSeconds();
    //8. 获得当前星期
    var week = date.getDay(); //没有getWeek
    // 2014年06月18日 15:40:30 星期三
    return year+"年"+changeNum(month)+"月"+day+"日 "+hour+":"+min+":"+sec+" "+parseWeek(week);
}

alert(getCurrentDate());
```

//解决 自动补齐成两位数字的方法

```java
function changeNum(num){
    if(num < 10){
        return "0"+num;
    }else{
        return num;
    }

}
```

//将数字 0~6 转换成 星期日到星期六

```java
function parseWeek(week){
    var arr = ["星期日","星期一","星期二","星期三","星期四","星期五","星期六"];
    //             0      1      2      3 .............
    return arr[week];
}
```

Date对象的方法—设置日期和时间

```java
//设置日期和时间
//setDate(day_of_month)       设置日
//setMonth (month)                 设置月
//setFullYear (year)               设置年
//setHours (hour)         设置小时
//setMinutes (minute)     设置分钟
//setSeconds (second)     设置秒
//setMillliseconds (ms)       设置毫秒(0-999)
//setTime (allms)     设置累计毫秒(从1970/1/1午夜)
    
var x=new Date();
x.setFullYear (1997);    //设置年1997
x.setMonth(7);        //设置月7
x.setDate(1);        //设置日1
x.setHours(5);        //设置小时5
x.setMinutes(12);    //设置分钟12
x.setSeconds(54);    //设置秒54
x.setMilliseconds(230);        //设置毫秒230
document.write(x.toLocaleString( )+"<br>");
//返回1997年8月1日5点12分54秒

x.setTime(870409430000); //设置累计毫秒数
document.write(x.toLocaleString( )+"<br>");
//返回1997年8月1日12点23分50秒
```

Date对象的方法—日期和时间的转换

```java
日期和时间的转换:

getTimezoneOffset():8个时区×15度×4分/度=480;
返回本地时间与GMT的时间差，以分钟为单位
toUTCString()
返回国际标准时间字符串
toLocalString()
返回本地格式时间字符串
Date.parse(x)
返回累计毫秒数(从1970/1/1午夜到本地时间)
Date.UTC(x)
返回累计毫秒数(从1970/1/1午夜到国际时间)
```

## 3.4 Math对象

//该对象中的属性方法 和数学有关.

```
abs(x)    返回数的绝对值。
exp(x)    返回 e 的指数。
floor(x)对数进行下舍入。
log(x)    返回数的自然对数（底为e）。
max(x,y)    返回 x 和 y 中的最高值。
min(x,y)    返回 x 和 y 中的最低值。
pow(x,y)    返回 x 的 y 次幂。
random()    返回 0 ~ 1 之间的随机数。
round(x)    把数四舍五入为最接近的整数。
sin(x)    返回数的正弦。
sqrt(x)    返回数的平方根。
tan(x)    返回角的正切。
```

//方法练习:

```cpp
//alert(Math.random()); // 获得随机数 0~1 不包括1.
    //alert(Math.round(1.5)); // 四舍五入
    //练习：获取1-100的随机整数，包括1和100
         //var num=Math.random();
         //num=num*10;
         //num=Math.round(num);
         //alert(num)
    //============max  min=========================
    /* alert(Math.max(1,2));// 2
    alert(Math.min(1,2));// 1 */
    //-------------pow--------------------------------
    alert(Math.pow(2,4));// pow 计算参数1 的参数2 次方.
```

### 3.5 Function 对象(重点)

### 3.5.1 函数的定义

```java
function 函数名 (参数){ <br>    函数体;
    return 返回值;
}
```

功能说明：

可以使用变量、常量或表达式作为函数调用的参数
函数由关键字function定义
函数名的定义规则与标识符一致，大小写是敏感的
返回值必须使用return
Function 类可以表示开发者定义的任何函数。

用 Function 类直接创建函数的语法如下：

```php
var 函数名 = new Function("参数1","参数n","function_body");
```

虽然由于字符串的关系，第二种形式写起来有些困难，但有助于理解函数只不过是一种引用类型，它们的行为与用 Function 类明确创建的函数行为是相同的。

### 示例：

```php
function func1(name){
    alert('hello'+name);
    return 8
}

    ret=func1("yuan");
    alert(ret);


var func2=new Function("name","alert(\"hello\"+name);")
func2("egon")
```

注意：js的函数加载执行与python不同，它是整体加载完才会执行，所以执行函数放在函数声明上面或下面都可以：

```
<script>
    //f(); --->OK

    function f(){
        console.log("hello")

    }

    f() //----->OK
</script>
```

### 3.5.2 Function 对象的属性

如前所述，函数属于引用类型，所以它们也有属性和方法。
比如，ECMAScript 定义的属性 length 声明了函数期望的参数个数。
`alert(func1.length)`
### 3.5.3 Function 的调用

```
function func1(a,b){

    alert(a+b);
}

    func1(1,2);  //3
    func1(1,2,3);//3
    func1(1);    //NaN
    func1();     //NaN

    //只要函数名写对即可,参数怎么填都不报错.

-------------------面试题-----------
 function a(a,b){
    alert(a+b);
}

   var a=1;
   var b=2;
   a(a,b)
```

### 3.5.4 函数的内置对象arguments

```java
function add(a,b){

        console.log(a+b);//3
        console.log(arguments.length);//2
        console.log(arguments);//[1,2]

    }
    add(1,2)

    ------------------arguments的用处1 ------------------
    function nxAdd(){
        var result=0;
        for (var num in arguments){
            result+=arguments[num]
        }
        alert(result)

    }

    nxAdd(1,2,3,4,5)

//     ------------------arguments的用处2 ------------------

    function f(a,b,c){
        if (arguments.length!=3){
            throw new Error("function f called with "+arguments.length+" arguments,but it just need 3 arguments")
        }
        else {
            alert("success!")
        }
    }

    f(1,2,3,4,5)
```

### 3.5.4 匿名函数

```java
// 匿名函数
    var func = function(arg){
        return "tony";
    }

// 匿名函数的应用
    (function(){
        alert("tony");
    } )()

    (function(arg){
        console.log(arg);
    })('123')
```

# BOM对象

### window对象

所有浏览器都支持 window 对象。
概念上讲.一个html文档对应一个window对象.
功能上讲: 控制浏览器窗口的.
使用上讲: window对象不需要创建对象,直接使用即可.

Window 对象方法

```
alert()            显示带有一段消息和一个确认按钮的警告框。
confirm()          显示带有一段消息以及确认按钮和取消按钮的对话框。
prompt()           显示可提示用户输入的对话框。

open()             打开一个新的浏览器窗口或查找一个已命名的窗口。
close()            关闭浏览器窗口。

setInterval()      按照指定的周期（以毫秒计）来调用函数或计算表达式。
clearInterval()    取消由 setInterval() 设置的 timeout。
setTimeout()       在指定的毫秒数后调用函数或计算表达式。
clearTimeout()     取消由 setTimeout() 方法设置的 timeout。
scrollTo()         把内容滚动到指定的坐标。
```

### 方法使用

1、alert confirm prompt以及open函数

```java
//----------alert confirm prompt----------------------------
//alert('aaa');


/* var result = confirm("您确定要删除吗?");
alert(result); */

//prompt 参数1 : 提示信息.   参数2:输入框的默认值. 返回值是用户输入的内容.

// var result = prompt("请输入一个数字!","haha");
// alert(result);


方法讲解:    
    //open方法 打开和一个新的窗口 并 进入指定网址.参数1 : 网址.
    //调用方式1
        //open("http://www.baidu.com");
    //参数1 什么都不填 就是打开一个新窗口.  参数2.填入新窗口的名字(一般可以不填). 参数3: 新打开窗口的参数.
        open('','','width=200,resizable=no,height=100'); // 新打开一个宽为200 高为100的窗口
    //close方法  将当前文档窗口关闭.
        //close();
```

### 示例：

```java
var num = Math.round(Math.random()*100);
    function acceptInput(){
    //2.让用户输入(prompt)    并接受 用户输入结果
    var userNum = prompt("请输入一个0~100之间的数字!","0");
    //3.将用户输入的值与 随机数进行比较
            if(isNaN(+userNum)){
                //用户输入的无效(重复2,3步骤)
                alert("请输入有效数字!");
                acceptInput();
            }
            else if(userNum > num){
            //大了==> 提示用户大了,让用户重新输入(重复2,3步骤)
                alert("您输入的大了!");
                acceptInput();
            }else if(userNum < num){
            //小了==> 提示用户小了,让用户重新输入(重复2,3步骤)
                alert("您输入的小了!");
                acceptInput();
            }else{
            //答对了==>提示用户答对了 , 询问用户是否继续游戏(confirm).
                var result = confirm("恭喜您!答对了,是否继续游戏?");
                if(result){
                    //是 ==> 重复123步骤.
                    num = Math.round(Math.random()*100);
                    acceptInput();
                }else{
                    //否==> 关闭窗口(close方法).
                    close();
                }
            }
    }
```

### 2、setInterval，clearInterval

```
setInterval() 方法会不停地调用函数，直到 clearInterval() 被调用或窗口被关闭。由 setInterval() 返回的 ID 值可用作 clearInterval() 方法的参数。

语法：<br>     setInterval(code,millisec)
```

其中，code为要调用的函数或要执行的代码串。millisec周期性执行或调用 code 之间的时间间隔，以毫秒计。

### 示例：

```
<input id="ID1" type="text" onclick="begin()">
<button onclick="end()">停止</button>

<script>
    function showTime(){
           var nowd2=new Date().toLocaleString();
           var temp=document.getElementById("ID1");
           temp.value=nowd2;

    }
    var ID;
    function begin(){
        if (ID==undefined){
             showTime();
             ID=setInterval(showTime,1000);
        }
    }
    function end(){
        clearInterval(ID);
        ID=undefined;


    }

</script>
```

# DOM对象

什么是HTML DOM

```
HTML  Document Object Model（文档对象模型）
HTML DOM 定义了访问和操作HTML文档的标准方法
HTML DOM 把 HTML 文档呈现为带有元素、属性和文本的树结构（节点树)
```

DOM树

![](http://i.imgur.com/2qKC5CI.png)

画dom树是为了展示文档中各个对象之间的关系，用于对象的导航。

### DOM节点

节点类型

HTML 文档中的每个成分都是一个节点。

DOM 是这样规定的：
整个文档是一个文档节点
每个 HTML 标签是一个元素节点
包含在 HTML 元素中的文本是文本节点
每一个 HTML 属性是一个属性节点

其中，document与element节点是重点。

### 节点关系

节点树中的节点彼此拥有层级关系。
父(parent),子(child)和同胞(sibling)等术语用于描述这些关系。父节点拥有子节点。同级的子节点被称为同胞（兄弟或姐妹）。

```
在节点树中，顶端节点被称为根（root）
每个节点都有父节点、除了根（它没有父节点）
一个节点可拥有任意数量的子
同胞是拥有相同父节点的节点
```

下面的图片展示了节点树的一部分，以及节点之间的关系：
![](http://i.imgur.com/xKTQbIH.png)

访问 HTML 元素（节点）,访问 HTML 元素等同于访问节点,我们能够以不同的方式来访问 HTML 元素。

### 节点查找

直接查找节点

```
document.getElementById(“idname”)
document.getElementsByTagName(“tagname”)
document.getElementsByName(“name”)
document.getElementsByClassName(“name”)

<div id="div1">

    <div class="div2">i am div2</div>
    <div name="yuan">i am div2</div>
    <div id="div3">i am div2</div>
    <p>hello p</p>
</div>

<script>

   var div1=document.getElementById("div1");

////支持;
//   var ele= div1.getElementsByTagName("p");
//   alert(ele.length);
////支持
//   var ele2=div1.getElementsByClassName("div2");
//   alert(ele2.length);
////不支持
//   var ele3=div1.getElementById("div3");
//   alert(ele3.length);
////不支持
//   var ele4=div1.getElementsByName("yuan");
//   alert(ele4.length)

</script>
```

注意：设计到寻找元素，注意




















# [JavaScript 基础学习1-day14](https://www.cnblogs.com/lincappu/p/8157532.html)






# JavaScript 基础学习1

知识预览
JavaScript概述
二 JavaScript的基础
三 JavaScript的对象
BOM对象
DOM对象
实例练习
js扩展

## JavaScript概述

### JavaScript的历史

1992年Nombas开发出C-minus-minus(C--)的嵌入式脚本语言(最初绑定在CEnvi软件中).后将其改名ScriptEase.(客户端执行的语言)

Netscape(网景)接收Nombas的理念,(Brendan Eich)在其Netscape Navigator 2.0产品中开发出一套livescript的脚本语言.Sun和Netscape共同完成.后改名叫Javascript

微软随后模仿在其IE3.0的产品中搭载了一个JavaScript的克隆版叫Jscript.

为了统一三家,ECMA(欧洲计算机制造协会)定义了ECMA-262规范.国际标准化组织及国际电工委员会（ISO/IEC）也采纳 ECMAScript 作为标准（ISO/IEC-16262）。从此，Web 浏览器就开始努力（虽然有着不同的程度的成功和失败）将 ECMAScript 作为 JavaScript 实现的基础。EcmaScript是规范.

### ECMAScript

尽管 ECMAScript 是一个重要的标准，但它并不是 JavaScript 唯一的部分，当然，也不是唯一被标准化的部分。实际上，一个完整的 JavaScript 实现是由以下 3 个不同部分组成的：
- 核心（ECMAScript）
- 文档对象模型（DOM） Document object model (整合js，css，html)
- 浏览器对象模型（BOM） Broswer object model（整合js和浏览器）
- Javascript 在开发中绝大多数情况是基于对象的.也是面向对象的.
![](http://i.imgur.com/JhOmZGd.png)

简单地说，ECMAScript 描述了以下内容：
- 语法
- 类型
- 语句
- 关键字
- 保留字
- 运算符
- 对象 (封装 继承 多态) 基于对象的语言.使用对象.

## 二 JavaScript的基础

### 2.1 JS的引入方式

### 1 直接编写

```
<script>
    alert('hello yuan')
</script>
```

### 2 导入文件
`<script src="hello.js"></script>`
## 2.2 JS的变量、常量和标识符

### 2.2.1 JS的变量

```
x=5
y=6
z=x+y
```

在代数中，我们使用字母（比如 x）来保存值（比如 5）。通过上面的表达式 z=x+y，我们能够计算出 z 的值为 11。在 JavaScript 中，这些字母被称为变量。

那么如何在JS中定义使用变量呢？

1、声明变量时不用声明变量类型. 全都使用var关键字;
`var a;<br>a=3;`
2、一行可以声明多个变量.并且可以是不同类型
`var name="yuan", age=20, job="lecturer";`
3、声明变量时 可以不用var. 如果不用var 那么它是全局变量

4、变量命名,首字符只能是字母,下划线,$美元符 三选一，余下的字符可以是下划线、美元符号或任何字母或数字字符且区分大小写，x与X是两个变量　

```php
Camel 标记法
首字母是小写的，接下来的字母都以大写字符开头。例如：
var myTestValue = 0, mySecondValue = "hi";
Pascal 标记法
首字母是大写的，接下来的字母都以大写字符开头。例如：
Var MyTestValue = 0, MySecondValue = "hi";
匈牙利类型标记法
在以 Pascal 标记法命名的变量前附加一个小写字母（或小写字母序列），
说明该变量的类型。例如，i 表示整数，s 表示字符串，如下所示“
Var iMyTestValue = 0, sMySecondValue = "hi";
```

### 2.2.2 常量和标识符

常量 ：直接在程序中出现的数据值

标识符：

由不以数字开头的字母、数字、下划线(_)、美元符号($)组成
常用于表示函数、变量等的名称
例如：_abc,$abc,abc,abc123是标识符，而1abc不是
JavaScript语言中代表特定含义的词称为保留字，不允许程序再定义为标识符

![](http://i.imgur.com/rWo0uZO.png)


## 2.3 JS的数据类型

![](http://i.imgur.com/0TyZ7D5.png)

```
number     -----  数值
    boolean    -----  布尔值
    string     -----  字符串
    undefined  -----  undefined
    null       -----   null
```

### 2.3.1 数字类型(number)

![](http://i.imgur.com/neiWtJc.png)
- 不区分整型数值和浮点型数值;
- 所有数字都采用64位浮点格式存储，相当于Java和C语言中的double格式
- 能表示的最大值是±1.7976931348623157 x 10308
- 能表示的最小值是±5 x 10 -324 　

整数：

```
在JavaScript中10进制的整数由数字的序列组成
       精确表达的范围是 -9007199254740992 (-253) 到 9007199254740992 (253)
       超出范围的整数，精确度将受影响
```

浮点数：

```
使用小数点记录数据
       例如：3.4，5.6
       使用指数记录数据
       例如：4.3e23 = 4.3 x 1023
```

16进制和8进制数的表达:

```
16进制数据前面加上0x，八进制前面加0;16进制数是由0-9,A-F等16个字符组成;8进制数由0-7等8个数字组成

       16进制和8进制与2进制的换算:
```

2进制: 1111 0011 1101 0100 <-----> 16进制:0xF3D4 <-----> 10进制:62420
2进制: 1 111 001 111 010 100 <-----> 8进制:0171724

### 2.3.2 字符串类型(string)

是由Unicode字符、数字、标点符号组成的序列；字符串常量首尾由单引号或双引号括起；JavaScript中没有字符类型；常用特殊字符在字符串中的表达；
字符串中部分特殊字符必须加上右划线；常用的转义字符 \n:换行 ':单引号 ":双引号 \:右划线

### 2.3.3 布尔类型(boolean)

Boolean类型仅有两个值：true和false，也代表1和0，实际运算中true=1,false=0
布尔值也可以看作on/off、yes/no、1/0对应true/false
Boolean值主要用于JavaScript的控制语句，例如：

```cpp
if (x==1){
      y=y+1;
}else{
      y=y-1;
      }
```

### 2.3.4 Null & Undefined类型

### Undefined类型

Undefined 类型只有一个值，即 undefined。当声明的变量未初始化时，该变量的默认值是 undefined。

当函数无明确返回值时，返回的也是值 "undefined";

### Null类型

另一种只有一个值的类型是 Null，它只有一个专用值 null，即它的字面量。值 undefined 实际上是从值 null 派生来的，因此 ECMAScript 把它们定义为相等的。

尽管这两个值相等，但它们的含义不同。undefined 是声明了变量但未对其初始化时赋予该变量的值，null 则用于表示尚未存在的对象（在讨论 typeof 运算符时，简单地介绍过这一点）。如果函数或方法要返回的是对象，那么找不到该对象时，返回的通常是 null。

## 2.4 运算符

算术运算符：
+ - * / % ++ --

比较运算符：
> >= < <= != == === !==

逻辑运算符：
&& || ！

赋值运算符：
= += -= *= /=

字符串运算符：
+ 连接，两边操作数有一个或两个是字符串就做连接运算

### 2.4.1 算术运算符

#### 注意1: 自加自减

假如x=2，那么x++表达式执行后的值为3，x--表达式执行后的值为1；i++相当于i=i+1，i--相当于i=i-1；
递增和递减运算符可以放在变量前也可以放在变量后：--i

```matlab
var i=10;
console.log(i++);
console.log(i);
console.log(++i);
console.log(i);
console.log(i--);
console.log(--i);
```

#### 注意2: 单元运算符
- 
除了可以表示减号还可以表示负号 例如：x=-y

- 
除了可以表示加法运算还可以用于字符串的连接 例如："abc"+"def"="abcdef"
js不同于python，是一门弱类型语言


#### 注意3: NaN

```java
var d="yuan";
d=+d;
alert(d);//NaN:属于Number类型的一个特殊值,当遇到将字符串转成数字无效时,就会得到一个NaN数据
alert(typeof(d));//Number

//NaN特点:

var n=NaN;

alert(n>3);
alert(n<3);
alert(n==3);
alert(n==NaN);

alert(n!=NaN);//NaN参与的所有的运算都是false,除了!=
```

### 2.4.2 比较运算符

> 
> 
= < <= != == === !==
用于控制语句时：



```
if (2>1){       //  3  0  false null undefined ［］
        console.log("条件成立!")
    }
```

等号和非等号的同类运算符是全等号和非全等号。这两个运算符所做的与等号和非等号相同，只是它们在检查相等性前，不执行类型转换。

```cpp
console.log(2==2);
console.log(2=="2");
console.log(2==="2");
console.log(2!=="2");
```

### 注意1：

```
var bResult = "Blue" < "alpha";
alert(bResult); //输出 true
```

在上面的例子中，字符串 "Blue" 小于 "alpha"，因为字母 B 的字符代码是 66，字母 a 的字符代码是 97。

比较数字和字符串

另一种棘手的状况发生在比较两个字符串形式的数字时，比如：

```
var bResult = "25" < "3";
alert(bResult); //输出 "true"
```

上面这段代码比较的是字符串 "25" 和 "3"。两个运算数都是字符串，所以比较的是它们的字符代码（"2" 的字符代码是 50，"3" 的字符代码是 51）。

不过，如果把某个运算数该为数字，那么结果就有趣了：

```
var bResult = "25" < 3;
alert(bResult); //输出 "false"
```

这里，字符串 "25" 将被转换成数字 25，然后与数字 3 进行比较，结果不出所料。

总结：

比较运算符两侧如果一个是数字类型,一个是其他类型,会将其类型转换成数字类型.
比较运算符两侧如果都是字符串类型,比较的是最高位的asc码,如果最高位相等,继续取第二位比较.

### 注意2：

等性运算符：执行类型转换的规则如下：

如果一个运算数是 Boolean 值，在检查相等性之前，把它转换成数字值。false 转换成 0，true 为 1。
如果一个运算数是字符串，另一个是数字，在检查相等性之前，要尝试把字符串转换成数字。
如果一个运算数是对象，另一个是字符串，在检查相等性之前，要尝试把对象转换成字符串。
如果一个运算数是对象，另一个是数字，在检查相等性之前，要尝试把对象转换成数字。
在比较时，该运算符还遵守下列规则：

值 null 和 undefined 相等。
在检查相等性时，不能把 null 和 undefined 转换成其他值。
如果某个运算数是 NaN，等号将返回 false，非等号将返回 true。
如果两个运算数都是对象，那么比较的是它们的引用值。如果两个运算数指向同一对象，那么等号返回 true，否则两个运算数不等。

### 2.4.3 逻辑运算符

```cpp
if (2>1 && [1,2]){
    console.log("条件与")
}

// 思考返回内容?
console.log(1 && 3);
console.log(0 && 3);
console.log(0 || 3);
console.log(2 || 3);
```

## 2.5 流程控制
- 顺序结构(从上向下顺序执行)
- 分支结构
- 
循环结构

### 2.5.1 顺序结构

### 2.5.2 分支结构

if-else结构：

if (表达式){
语句１;
......
} else{
语句２;
.....
}
功能说明：如果表达式的值为true则执行语句1,否则执行语句2


### 示例：

```java
var x= (new Date()).getDay();
        //获取今天的星期值，0为星期天
        var y;

        if ( (x==6) || (x==0) ) {
            y="周末";
        }else{
            y="工作日";
            }
        
        console.log(y);

        /*等价于

        y="工作日";
        if ( (x==6) || (x==0) ) {
        y="周末";
        }
        console.log(y);  */
```

### if-elif－else结构：

```java
if (表达式1) {
    语句1;
}else if (表达式2){
    语句2;
}else if (表达式3){
    语句3;
} else{
    语句4;
}

var score=window.prompt("您的分数:");

if (score>90){
    ret="优秀";
}else if (score>80){
    ret="良";
}else if (score>60){
    ret="及格";
}else {
    ret = "不及格";

}
alert(ret);
```

### switch-case结构

```cpp
switch基本格式
switch (表达式) {
    case 值1:语句1;break;
    case 值2:语句2;break;
    case 值3:语句3;break;
    default:语句4;
}
```

![](http://i.imgur.com/ZTLD1hE.png)

```
switch(x){
case 1:y="星期一";    break;
case 2:y="星期二";    break;
case 3:y="星期三";    break;
case 4:y="星期四";    break;
case 5:y="星期五";    break;
case 6:y="星期六";    break;
case 7:y="星期日";    break;
default: y="未定义";
}
switch比else if结构更加简洁清晰，使程序可读性更强,效率更高。
```

### 2.5.2 循环结构

![](http://i.imgur.com/bANZDq7.png)
循环语句流程图

### for循环：

语法规则：

```cpp
for(初始表达式;条件表达式;自增或自减)
{
        执行语句
        ……
}
```

功能说明：实现条件循环，当条件成立时，执行语句1，否则跳出循环体

for循环的另一种形式：

for( 变量 in 数组或对象)

{
执行语句
……
}

### while循环：

语法规则：

```
while (条件){
    语句1；
    ...
}
```

功能说明：运行功能和for类似，当条件成立循环执行语句花括号{}内的语句，否则跳出循环；同样支持continue与break语句。

### 示例：

```java
var i=1;
while (i<=7) {
    document.write("<H"+i+">hello</H "+i+"> ");
    document.write("<br>");
    i++;
}
//循环输出H1到H7的字体大小
```

### 2.5.3 异常处理

```
try {
    //这段代码从上往下运行，其中任何一个语句抛出异常该代码块就结束运行
}
catch (e) {
    // 如果try代码块中抛出了异常，catch代码块中的代码就会被执行。
    //e是一个局部变量，用来指向Error对象或者其他抛出的对象
}
finally {
     //无论try中代码是否有异常抛出（甚至是try代码块中有return语句），finally代码块中始终会被执行。
}
```

注：主动抛出异常 throw Error('xxxx')

## 三 JavaScript的对象

简介：

在JavaScript中除了null和undefined以外其他的数据类型都被定义成了对象，也可以用创建对象的方法定义变量，String、Math、Array、Date、RegExp都是JavaScript中重要的内置对象，在JavaScript程序大多数功能都是基于对象实现的。

```
<script language="javascript">
var aa=Number.MAX_VALUE; 
//利用数字对象获取可表示最大数
var bb=new String("hello JavaScript"); 
//创建字符串对象
var cc=new Date();
//创建日期对象
var dd=new Array("星期一","星期二","星期三","星期四"); 
//数组对象
</script>
```

### 3.1 String对象

字符串对象创建

字符串创建(两种方式)

```java
① 变量 = “字符串”
   ② 字串对象名称 = new String (字符串)

var str1="hello world";
var str1= new String("hello word");
```

字符串对象的属性和函数
x.length －－－－获取字符串的长度

```
x.toLowerCase()        －－－－转为小写
 x.toUpperCase()        －－－－转为大写
 x.trim()               －－－－去除字符串两边空格
```

### 字符串查询方法

```
x.charAt(index)         －－－－str1.charAt(index);－－－－获取指定位置字符，其中index为要获取的字符索引

x.indexOf(findstr,index)－－－－查询字符串位置
x.lastIndexOf(findstr)  

x.match(regexp)         －－－－match返回匹配字符串的数组，如果没有匹配则返回null
x.search(regexp)        －－－－search返回匹配字符串的首字符位置索引

                        示例：
                        var str1="welcome to the world of JS!";
                        var str2=str1.match("world");
                        var str3=str1.search("world");
                        alert(str2[0]);  // 结果为"world"
                        alert(str3);     // 结果为15
```

### 子字符串处理方法

```
x.substr(start, length) －－－－start表示开始位置，length表示截取长度
x.substring(start, end) －－－－end是结束位置

x.slice(start, end)     －－－－切片操作字符串
                    示例：
                        var str1="abcdefgh";
                        var str2=str1.slice(2,4);
                        var str3=str1.slice(4);
                        var str4=str1.slice(2,-1);
                        var str5=str1.slice(-3,-1);

                        alert(str2); //结果为"cd"
                        
                        alert(str3); //结果为"efgh"
                        
                        alert(str4); //结果为"cdefg"
                        
                        alert(str5); //结果为"fg"

x.replace(findstr,tostr) －－－－    字符串替换

x.split();                 －－－－分割字符串
                             var str1="一,二,三,四,五,六,日"; 
                            var strArray=str1.split(",");
                            alert(strArray[1]);//结果为"二"
                            
x.concat(addstr)         －－－－    拼接字符串
```

### 3.2 Array对象

### 3.2.1 数组创建

创建数组的三种方式：

### 创建方式1:
`var arrname = [元素0,元素1,….];          // var arr=[1,2,3];`
### 创建方式2:

```java
var arrname = new Array(元素0,元素1,….); // var test=new Array(100,"a",true);
```

### 创建方式3:

```java
var arrname = new Array(长度); 
        //  初始化数组对象:
            var cnweek=new Array(7);
                cnweek[0]="星期日";
                cnweek[1]="星期一";
                ...
                cnweek[6]="星期六";
```

创建二维数组：

```
var cnweek=new Array(7);
for (var i=0;i<=6;i++){
    cnweek[i]=new Array(2);
}
cnweek[0][0]="星期日";
cnweek[0][1]="Sunday";
cnweek[1][0]="星期一";
cnweek[1][1]="Monday";
...
cnweek[6][0]="星期六";
cnweek[6][1]="Saturday";
```

### 3.2.2 数组对象的属性和方法

#### join方法：

```
x.join(bystr)       －－－－将数组元素拼接成字符串

var arr1=[1, 2, 3, 4, 5, 6, 7];
var str1=arr1.join("-");
alert(str1);  //结果为"1-2-3-4-5-6-7"
```

#### concat方法：

```
x.concat(value,...)    －－－－ 
var a = [1,2,3];
var b=a.concat(4,5) ;
alert(a.toString());  //返回结果为1,2,3            
alert(b.toString());  //返回结果为1,2,3,4,5
```

#### 数组排序-reverse sort：

```java
//x.reverse()
//x.sort()

var arr1=[32, 12, 111, 444];
//var arr1=["a","d","f","c"];

arr1.reverse(); //颠倒数组元素
alert(arr1.toString());
//结果为444,111,12,32

arr1.sort();    //排序数组元素
alert(arr1.toString());
//结果为111,12,32,444

//------------------------------
arr=[1,5,2,100];

//arr.sort();
//alert(arr);
//如果就想按着数字比较呢?

function intSort(a,b){
    if (a>b){
        return 1;//-1
    }
    else if(a<b){
        return -1;//1
    }
    else {
        return 0
    }
}

arr.sort(intSort);

alert(arr);

function IntSort(a,b){
    return a-b;
}
```

#### 数组切片操作：

```
//x.slice(start, end)
//
//使用注解
//
//x代表数组对象
//start表示开始位置索引
//end是结束位置下一数组元素索引编号
//第一个数组元素索引为0
//start、end可为负数，-1代表最后一个数组元素
//end省略则相当于从start位置截取以后所有数组元素

var arr1=['a','b','c','d','e','f','g','h'];
var arr2=arr1.slice(2,4);
var arr3=arr1.slice(4);
var arr4=arr1.slice(2,-1);

alert(arr2.toString());
//结果为"c,d" 
alert(arr3.toString());
//结果为"e,f,g,h"
alert(arr4.toString());
//结果为"c,d,e,f,g"
```

#### 删除子数组：

```
//x. splice(start, deleteCount, value, ...)

//使用注解

//x代表数组对象
//splice的主要用途是对数组指定位置进行删除和插入
//start表示开始位置索引
//deleteCount删除数组元素的个数
//value表示在删除位置插入的数组元素
//value参数可以省略       
        
var a = [1,2,3,4,5,6,7,8];
a.splice(1,2);

alert(a.toString());//a变为 [1,4,5,6,7,8]

a.splice(1,1);

alert(a.toString());//a变为[1,5,6,7,8]

a.splice(1,0,2,3);

alert(a.toString());//a变为[1,2,3,5,6,7,8]
```

#### 数组的push和pop：

```
//push pop这两个方法模拟的是一个栈操作

//x.push(value, ...)  压栈
//x.pop()             弹栈      
//使用注解
//
//x代表数组对象
//value可以为字符串、数字、数组等任何值
//push是将value值添加到数组x的结尾
//pop是将数组x的最后一个元素删除


var arr1=[1,2,3];
arr1.push(4,5);
alert(arr1);
//结果为"1,2,3,4,5"
arr1.push([6,7]);
alert(arr1)
//结果为"1,2,3,4,5,6,7"
arr1.pop();
alert(arr1);
//结果为"1,2,3,4,5"
```

#### 数组的shift和unshift：

```
//x.unshift(value,...)
//x.shift()
//使用注解

//x代表数组对象
//value可以为字符串、数字、数组等任何值
//unshift是将value值插入到数组x的开始
//shift是将数组x的第一个元素删除

var arr1=[1,2,3];
arr1.unshift(4,5);
alert(arr1);  //结果为"4,5,1,2,3"

arr1. unshift([6,7]);
alert(arr1);  //结果为"6,7,4,5,1,2,3"

arr1.shift();
alert(arr1);  //结果为"4,5,1,2,3"
```

### 总结js的数组特性：

```java
//  js中数组的特性
     //java中数组的特性,  规定是什么类型的数组,就只能装什么类型.只有一种类型.
     //js中的数组特性1: js中的数组可以装任意类型,没有任何限制.
     //js中的数组特性2: js中的数组,长度是随着下标变化的.用到多长就有多长.
     var arr5 = ['abc',123,1.14,true,null,undefined,new String('1213'),new Function('a','b','alert(a+b)')];
    /*  alert(arr5.length);//8
     arr5[10] = "hahaha";
     alert(arr5.length); //11
     alert(arr5[9]);// undefined */
```

## 3.3 Date对象

### 3.3.1 创建Date对象

//方法1：不指定参数

```
var nowd1=new Date();
alert(nowd1.toLocaleString( ));
```

//方法2：参数为日期字符串

```
var nowd2=new Date("2004/3/20 11:12");
alert(nowd2.toLocaleString( ));
var nowd3=new Date("04/03/20 11:12");
alert(nowd3.toLocaleString( ));
```

//方法3：参数为毫秒数

```
var nowd3=new Date(5000);
alert(nowd3.toLocaleString( ));
alert(nowd3.toUTCString());
```

//方法4：参数为年月日小时分钟秒毫秒

```java
var nowd4=new Date(2004,2,20,11,12,0,300);
alert(nowd4.toLocaleString( ));//毫秒并不直接显示
```

### Date对象的方法—获取日期和时间

```
获取日期和时间
getDate()                 获取日
getDay ()                 获取星期
getMonth ()               获取月（0-11）
getFullYear ()            获取完整年份
getYear ()                获取年
getHours ()               获取小时
getMinutes ()             获取分钟
getSeconds ()             获取秒
getMilliseconds ()        获取毫秒
getTime ()                返回累计毫秒数(从1970/1/1午夜)
```

### 实例练习：

```java
function getCurrentDate(){
    //1. 创建Date对象
    var date = new Date(); //没有填入任何参数那么就是当前时间
    //2. 获得当前年份
    var year = date.getFullYear();
    //3. 获得当前月份 js中月份是从0到11.
    var month = date.getMonth()+1;
    //4. 获得当前日
    var day = date.getDate();
    //5. 获得当前小时
    var hour = date.getHours();
    //6. 获得当前分钟
    var min = date.getMinutes();
    //7. 获得当前秒
    var sec = date.getSeconds();
    //8. 获得当前星期
    var week = date.getDay(); //没有getWeek
    // 2014年06月18日 15:40:30 星期三
    return year+"年"+changeNum(month)+"月"+day+"日 "+hour+":"+min+":"+sec+" "+parseWeek(week);
}

alert(getCurrentDate());
```

//解决 自动补齐成两位数字的方法

```java
function changeNum(num){
    if(num < 10){
        return "0"+num;
    }else{
        return num;
    }

}
```

//将数字 0~6 转换成 星期日到星期六

```java
function parseWeek(week){
    var arr = ["星期日","星期一","星期二","星期三","星期四","星期五","星期六"];
    //             0      1      2      3 .............
    return arr[week];
}
```

Date对象的方法—设置日期和时间

```java
//设置日期和时间
//setDate(day_of_month)       设置日
//setMonth (month)                 设置月
//setFullYear (year)               设置年
//setHours (hour)         设置小时
//setMinutes (minute)     设置分钟
//setSeconds (second)     设置秒
//setMillliseconds (ms)       设置毫秒(0-999)
//setTime (allms)     设置累计毫秒(从1970/1/1午夜)
    
var x=new Date();
x.setFullYear (1997);    //设置年1997
x.setMonth(7);        //设置月7
x.setDate(1);        //设置日1
x.setHours(5);        //设置小时5
x.setMinutes(12);    //设置分钟12
x.setSeconds(54);    //设置秒54
x.setMilliseconds(230);        //设置毫秒230
document.write(x.toLocaleString( )+"<br>");
//返回1997年8月1日5点12分54秒

x.setTime(870409430000); //设置累计毫秒数
document.write(x.toLocaleString( )+"<br>");
//返回1997年8月1日12点23分50秒
```

Date对象的方法—日期和时间的转换

```java
日期和时间的转换:

getTimezoneOffset():8个时区×15度×4分/度=480;
返回本地时间与GMT的时间差，以分钟为单位
toUTCString()
返回国际标准时间字符串
toLocalString()
返回本地格式时间字符串
Date.parse(x)
返回累计毫秒数(从1970/1/1午夜到本地时间)
Date.UTC(x)
返回累计毫秒数(从1970/1/1午夜到国际时间)
```

## 3.4 Math对象

//该对象中的属性方法 和数学有关.

```
abs(x)    返回数的绝对值。
exp(x)    返回 e 的指数。
floor(x)对数进行下舍入。
log(x)    返回数的自然对数（底为e）。
max(x,y)    返回 x 和 y 中的最高值。
min(x,y)    返回 x 和 y 中的最低值。
pow(x,y)    返回 x 的 y 次幂。
random()    返回 0 ~ 1 之间的随机数。
round(x)    把数四舍五入为最接近的整数。
sin(x)    返回数的正弦。
sqrt(x)    返回数的平方根。
tan(x)    返回角的正切。
```

//方法练习:

```cpp
//alert(Math.random()); // 获得随机数 0~1 不包括1.
    //alert(Math.round(1.5)); // 四舍五入
    //练习：获取1-100的随机整数，包括1和100
         //var num=Math.random();
         //num=num*10;
         //num=Math.round(num);
         //alert(num)
    //============max  min=========================
    /* alert(Math.max(1,2));// 2
    alert(Math.min(1,2));// 1 */
    //-------------pow--------------------------------
    alert(Math.pow(2,4));// pow 计算参数1 的参数2 次方.
```

### 3.5 Function 对象(重点)

### 3.5.1 函数的定义

```java
function 函数名 (参数){ <br>    函数体;
    return 返回值;
}
```

功能说明：

可以使用变量、常量或表达式作为函数调用的参数
函数由关键字function定义
函数名的定义规则与标识符一致，大小写是敏感的
返回值必须使用return
Function 类可以表示开发者定义的任何函数。

用 Function 类直接创建函数的语法如下：

```php
var 函数名 = new Function("参数1","参数n","function_body");
```

虽然由于字符串的关系，第二种形式写起来有些困难，但有助于理解函数只不过是一种引用类型，它们的行为与用 Function 类明确创建的函数行为是相同的。

### 示例：

```php
function func1(name){
    alert('hello'+name);
    return 8
}

    ret=func1("yuan");
    alert(ret);


var func2=new Function("name","alert(\"hello\"+name);")
func2("egon")
```

注意：js的函数加载执行与python不同，它是整体加载完才会执行，所以执行函数放在函数声明上面或下面都可以：

```
<script>
    //f(); --->OK

    function f(){
        console.log("hello")

    }

    f() //----->OK
</script>
```

### 3.5.2 Function 对象的属性

如前所述，函数属于引用类型，所以它们也有属性和方法。
比如，ECMAScript 定义的属性 length 声明了函数期望的参数个数。
`alert(func1.length)`
### 3.5.3 Function 的调用

```
function func1(a,b){

    alert(a+b);
}

    func1(1,2);  //3
    func1(1,2,3);//3
    func1(1);    //NaN
    func1();     //NaN

    //只要函数名写对即可,参数怎么填都不报错.

-------------------面试题-----------
 function a(a,b){
    alert(a+b);
}

   var a=1;
   var b=2;
   a(a,b)
```

### 3.5.4 函数的内置对象arguments

```java
function add(a,b){

        console.log(a+b);//3
        console.log(arguments.length);//2
        console.log(arguments);//[1,2]

    }
    add(1,2)

    ------------------arguments的用处1 ------------------
    function nxAdd(){
        var result=0;
        for (var num in arguments){
            result+=arguments[num]
        }
        alert(result)

    }

    nxAdd(1,2,3,4,5)

//     ------------------arguments的用处2 ------------------

    function f(a,b,c){
        if (arguments.length!=3){
            throw new Error("function f called with "+arguments.length+" arguments,but it just need 3 arguments")
        }
        else {
            alert("success!")
        }
    }

    f(1,2,3,4,5)
```

### 3.5.4 匿名函数

```java
// 匿名函数
    var func = function(arg){
        return "tony";
    }

// 匿名函数的应用
    (function(){
        alert("tony");
    } )()

    (function(arg){
        console.log(arg);
    })('123')
```

# BOM对象

### window对象

所有浏览器都支持 window 对象。
概念上讲.一个html文档对应一个window对象.
功能上讲: 控制浏览器窗口的.
使用上讲: window对象不需要创建对象,直接使用即可.

Window 对象方法

```
alert()            显示带有一段消息和一个确认按钮的警告框。
confirm()          显示带有一段消息以及确认按钮和取消按钮的对话框。
prompt()           显示可提示用户输入的对话框。

open()             打开一个新的浏览器窗口或查找一个已命名的窗口。
close()            关闭浏览器窗口。

setInterval()      按照指定的周期（以毫秒计）来调用函数或计算表达式。
clearInterval()    取消由 setInterval() 设置的 timeout。
setTimeout()       在指定的毫秒数后调用函数或计算表达式。
clearTimeout()     取消由 setTimeout() 方法设置的 timeout。
scrollTo()         把内容滚动到指定的坐标。
```

### 方法使用

1、alert confirm prompt以及open函数

```java
//----------alert confirm prompt----------------------------
//alert('aaa');


/* var result = confirm("您确定要删除吗?");
alert(result); */

//prompt 参数1 : 提示信息.   参数2:输入框的默认值. 返回值是用户输入的内容.

// var result = prompt("请输入一个数字!","haha");
// alert(result);


方法讲解:    
    //open方法 打开和一个新的窗口 并 进入指定网址.参数1 : 网址.
    //调用方式1
        //open("http://www.baidu.com");
    //参数1 什么都不填 就是打开一个新窗口.  参数2.填入新窗口的名字(一般可以不填). 参数3: 新打开窗口的参数.
        open('','','width=200,resizable=no,height=100'); // 新打开一个宽为200 高为100的窗口
    //close方法  将当前文档窗口关闭.
        //close();
```

### 示例：

```java
var num = Math.round(Math.random()*100);
    function acceptInput(){
    //2.让用户输入(prompt)    并接受 用户输入结果
    var userNum = prompt("请输入一个0~100之间的数字!","0");
    //3.将用户输入的值与 随机数进行比较
            if(isNaN(+userNum)){
                //用户输入的无效(重复2,3步骤)
                alert("请输入有效数字!");
                acceptInput();
            }
            else if(userNum > num){
            //大了==> 提示用户大了,让用户重新输入(重复2,3步骤)
                alert("您输入的大了!");
                acceptInput();
            }else if(userNum < num){
            //小了==> 提示用户小了,让用户重新输入(重复2,3步骤)
                alert("您输入的小了!");
                acceptInput();
            }else{
            //答对了==>提示用户答对了 , 询问用户是否继续游戏(confirm).
                var result = confirm("恭喜您!答对了,是否继续游戏?");
                if(result){
                    //是 ==> 重复123步骤.
                    num = Math.round(Math.random()*100);
                    acceptInput();
                }else{
                    //否==> 关闭窗口(close方法).
                    close();
                }
            }
    }
```

### 2、setInterval，clearInterval

```
setInterval() 方法会不停地调用函数，直到 clearInterval() 被调用或窗口被关闭。由 setInterval() 返回的 ID 值可用作 clearInterval() 方法的参数。

语法：<br>     setInterval(code,millisec)
```

其中，code为要调用的函数或要执行的代码串。millisec周期性执行或调用 code 之间的时间间隔，以毫秒计。

### 示例：

```
<input id="ID1" type="text" onclick="begin()">
<button onclick="end()">停止</button>

<script>
    function showTime(){
           var nowd2=new Date().toLocaleString();
           var temp=document.getElementById("ID1");
           temp.value=nowd2;

    }
    var ID;
    function begin(){
        if (ID==undefined){
             showTime();
             ID=setInterval(showTime,1000);
        }
    }
    function end(){
        clearInterval(ID);
        ID=undefined;


    }

</script>
```

# DOM对象

什么是HTML DOM

```
HTML  Document Object Model（文档对象模型）
HTML DOM 定义了访问和操作HTML文档的标准方法
HTML DOM 把 HTML 文档呈现为带有元素、属性和文本的树结构（节点树)
```

DOM树

![](http://i.imgur.com/2qKC5CI.png)

画dom树是为了展示文档中各个对象之间的关系，用于对象的导航。

### DOM节点

节点类型

HTML 文档中的每个成分都是一个节点。

DOM 是这样规定的：
整个文档是一个文档节点
每个 HTML 标签是一个元素节点
包含在 HTML 元素中的文本是文本节点
每一个 HTML 属性是一个属性节点

其中，document与element节点是重点。

### 节点关系

节点树中的节点彼此拥有层级关系。
父(parent),子(child)和同胞(sibling)等术语用于描述这些关系。父节点拥有子节点。同级的子节点被称为同胞（兄弟或姐妹）。

```
在节点树中，顶端节点被称为根（root）
每个节点都有父节点、除了根（它没有父节点）
一个节点可拥有任意数量的子
同胞是拥有相同父节点的节点
```

下面的图片展示了节点树的一部分，以及节点之间的关系：
![](http://i.imgur.com/xKTQbIH.png)

访问 HTML 元素（节点）,访问 HTML 元素等同于访问节点,我们能够以不同的方式来访问 HTML 元素。

### 节点查找

直接查找节点

```
document.getElementById(“idname”)
document.getElementsByTagName(“tagname”)
document.getElementsByName(“name”)
document.getElementsByClassName(“name”)

<div id="div1">

    <div class="div2">i am div2</div>
    <div name="yuan">i am div2</div>
    <div id="div3">i am div2</div>
    <p>hello p</p>
</div>

<script>

   var div1=document.getElementById("div1");

////支持;
//   var ele= div1.getElementsByTagName("p");
//   alert(ele.length);
////支持
//   var ele2=div1.getElementsByClassName("div2");
//   alert(ele2.length);
////不支持
//   var ele3=div1.getElementById("div3");
//   alert(ele3.length);
////不支持
//   var ele4=div1.getElementsByName("yuan");
//   alert(ele4.length)

</script>
```

注意：设计到寻找元素，注意



















