# JavaScript 数据类型 - 定义域 - CSDN博客
置顶2016年01月29日 17:02:34[定义域](https://me.csdn.net/u010278882)阅读数：320标签：[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[笔记](https://blog.csdn.net/u010278882/article/category/6086647)
# JavaScript 数据类型和数据类型转换
## JavaScript 数据类型
![这里写图片描述](https://img-blog.csdn.net/20160129165753269)
实际上 JavaScript 中仅有一种数据类型，就是对象类型。但是为了便于说明，可以将语言规范中提到的基本值（Primitive Value）称为基本数据类型。这样，JavaScript 中的数据类型就分为两大类：基本数据类型和对象类型。
其中，基本数据类型分为5小类：字符串值、数值、布尔值、null和undefiend。
字符串是Unicode字符构成的字符串。与 Java 不同的是：
- JavaScript 中无字符型
- 字符串可用” 或 “” 包含
与 Java 相同的：转移字符。
数值的存储格式遵循 IEEE 754 标准。与Java 不同的是：
- 仅有一种数值类型，可用于表示整数和小数
与 Java 相同的：浮点数精度问题、多进制支持、科学计数等。
布尔值和null的值都是字面量：true、false、null（都是关键字）。undefined 不是字面量。与 Java 不同的是：
- 有表示变量没有初始化的值 undefined
与 Java 相同的：null 表示空引用。
对象类型是与 Java 最大的不同：
- JavaScript 是基于原型的系统，而 Java 是基于类的系统（原型特性来源于 Self 语言）
- JavaScript 中的函数是对象（函数可作为数据的的特性来源于 Scheme 语言）
## JavaScript 变量
与 Java 不同过的是，JavaScript 中的变量没有类型。因为 JavaScript 是动态语言，动态语言的变量是没有类型的，可以赋予任意值。
（每种运算都针对特定的运算数类型，所以要定义数据类型。）
## JavaScript 常量
常量不属于标注范围
## JavaScript 字面量
|number|.3、.3E|
|----|----|
|string|’ ‘a’|
|boolean|TRUE|
|null|null|
|undefined|undefined|
|function|function([params]) {[statements]}|
|object|{x:1, y:true, z:function(){}}|
|array|[‘a’,1,true]|
|| |
## 数据类型转换（重点）
JavaScript 语言特点就是代码精简，编写的代码要能够很好的利用语言提供的机制简化。（由于语言内容要经过网络传输，减少大小）
![这里写图片描述](https://img-blog.csdn.net/20160129165830097)
与 Java 不同，由于是弱类型语言（可能引发类型错误的语言），所以语言中存在大量的隐式类型转换。并且，在 JavaScript 中也不提倡强制类型转换。隐式类型转换常常发生在布尔表达式的计算（比如，if语句中）和赋值表达式中（比如，赋值表达式、函数调用）。
何时发生?
- `+`运算：字符串连接，其中一个运算数是字符串，则另一个要转为字符串
- 非 + 的算术运算：要转换为数值
- = 运算
- 比较运算
- 方法调用
转换规则?
除了 “5. 方法调用”时要将基本值隐式转换为对象类型，其他情况都是转换成基本值，然后通过基本值进行计算。
> 
数据转换方法
![这里写图片描述](https://img-blog.csdn.net/20160129170330708)
> 
比较运算规则
- 同类 
- 基本类型：比值
- 对象类型：比引用
- 异类 
- 等于 
- null==null null==undefined undefined==undefined
- null、undefined 和任意：fasle
- Boolean 和（String Number Object）：toNumber(boolean) 然后比较
- Object 和（String Number）：toPrimitive(object) 然后比较
- Number 和 String：toNumber(string) 然后比较
- 不等于 
- - null、undefined：toNumber(null)=>0 toNumber(undefined)=>NaN
> 
测试用例
```
(function jsGrammarComparsion() {
    var type = [2, -2, '1', 'a', true, false, null, undefined, new Object(1), new Object('a'), new Object({x:1, y:2})];
    for(var n in type) {
        for(var m in type) {
            console.log(typeof(type[n])  + ":" + type[n] + " "+ typeof(type[m]) + ":" + type[m]);
            console.log("= " + (type[n] == type[m]) + " > " + (type[n] > type[m]) + " < " + (type[n] < type[m]));
        }
        console.log("----");
    }
 })();
 // 输出结果
 /*
 number:2 number:2
= true > false < false
number:2 number:-2
= false > true < false
number:2 string:1
= false > true < false
number:2 string:a
= false > false < false
number:2 boolean:true
= false > true < false
number:2 boolean:false
= false > true < false
number:2 object:null
= false > true < false
number:2 undefined:undefined
= false > false < false
number:2 object:1
= false > true < false
number:2 object:a
= false > false < false
number:2 object:[object Object]
= false > false < false
----
number:-2 number:2
= false > false < true
number:-2 number:-2
= true > false < false
number:-2 string:1
= false > false < true
number:-2 string:a
= false > false < false
number:-2 boolean:true
= false > false < true
number:-2 boolean:false
= false > false < true
number:-2 object:null
= false > false < true
number:-2 undefined:undefined
= false > false < false
number:-2 object:1
= false > false < true
number:-2 object:a
= false > false < false
number:-2 object:[object Object]
= false > false < false
----
string:1 number:2
= false > false < true
string:1 number:-2
= false > true < false
string:1 string:1
= true > false < false
string:1 string:a
= false > false < true
string:1 boolean:true
= true > false < false
string:1 boolean:false
= false > true < false
string:1 object:null
= false > true < false
string:1 undefined:undefined
= false > false < false
string:1 object:1
= true > false < false
string:1 object:a
= false > false < true
string:1 object:[object Object]
= false > false < true
----
string:a number:2
= false > false < false
string:a number:-2
= false > false < false
string:a string:1
= false > true < false
string:a string:a
= true > false < false
string:a boolean:true
= false > false < false
string:a boolean:false
= false > false < false
string:a object:null
= false > false < false
string:a undefined:undefined
= false > false < false
string:a object:1
= false > false < false
string:a object:a
= true > false < false
string:a object:[object Object]
= false > true < false
----
boolean:true number:2
= false > false < true
boolean:true number:-2
= false > true < false
boolean:true string:1
= true > false < false
boolean:true string:a
= false > false < false
boolean:true boolean:true
= true > false < false
boolean:true boolean:false
= false > true < false
boolean:true object:null
= false > true < false
boolean:true undefined:undefined
= false > false < false
boolean:true object:1
= true > false < false
boolean:true object:a
= false > false < false
boolean:true object:[object Object]
= false > false < false
----
boolean:false number:2
= false > false < true
boolean:false number:-2
= false > true < false
boolean:false string:1
= false > false < true
boolean:false string:a
= false > false < false
boolean:false boolean:true
= false > false < true
boolean:false boolean:false
= true > false < false
boolean:false object:null
= false > false < false
boolean:false undefined:undefined
= false > false < false
boolean:false object:1
= false > false < true
boolean:false object:a
= false > false < false
boolean:false object:[object Object]
= false > false < false
----
object:null number:2
= false > false < true
object:null number:-2
= false > true < false
object:null string:1
= false > false < true
object:null string:a
= false > false < false
object:null boolean:true
= false > false < true
object:null boolean:false
= false > false < false
object:null object:null
= true > false < false
object:null undefined:undefined
= true > false < false
object:null object:1
= false > false < true
object:null object:a
= false > false < false
object:null object:[object Object]
= false > false < false
----
undefined:undefined number:2
= false > false < false
undefined:undefined number:-2
= false > false < false
undefined:undefined string:1
= false > false < false
undefined:undefined string:a
= false > false < false
undefined:undefined boolean:true
= false > false < false
undefined:undefined boolean:false
= false > false < false
undefined:undefined object:null
= true > false < false
undefined:undefined undefined:undefined
= true > false < false
undefined:undefined object:1
= false > false < false
undefined:undefined object:a
= false > false < false
undefined:undefined object:[object Object]
= false > false < false
----
object:1 number:2
= false > false < true
object:1 number:-2
= false > true < false
object:1 string:1
= true > false < false
object:1 string:a
= false > false < false
object:1 boolean:true
= true > false < false
object:1 boolean:false
= false > true < false
object:1 object:null
= false > true < false
object:1 undefined:undefined
= false > false < false
object:1 object:1
= true > false < false
object:1 object:a
= false > false < false
object:1 object:[object Object]
= false > false < false
----
object:a number:2
= false > false < false
object:a number:-2
= false > false < false
object:a string:1
= false > true < false
object:a string:a
= true > false < false
object:a boolean:true
= false > false < false
object:a boolean:false
= false > false < false
object:a object:null
= false > false < false
object:a undefined:undefined
= false > false < false
object:a object:1
= false > false < false
object:a object:a
= true > false < false
object:a object:[object Object]
= false > true < false
----
object:[object Object] number:2
= false > false < false
object:[object Object] number:-2
= false > false < false
object:[object Object] string:1
= false > true < false
object:[object Object] string:a
= false > false < true
object:[object Object] boolean:true
= false > false < false
object:[object Object] boolean:false
= false > false < false
object:[object Object] object:null
= false > false < false
object:[object Object] undefined:undefined
= false > false < false
object:[object Object] object:1
= false > false < false
object:[object Object] object:a
= false > false < true
object:[object Object] object:[object Object]
= true > false < false
----
*/
```
> 
等值比较
![这里写图片描述](https://img-blog.csdn.net/20160129165908614)
> 
`>`
![这里写图片描述](https://img-blog.csdn.net/20160129180430359)
> 
`<`
![这里写图片描述](https://img-blog.csdn.net/20160129165946912)
## 参考
- [ECMAScript Specification](http://es5.github.io/#x8.12.8)
- [How do the JavaScript relational comparison operators coerce types?](http://stackoverflow.com/questions/14687876/how-do-the-javascript-relational-comparison-operators-coerce-types)
- [Truth, Equality and JavaScript](https://javascriptweblog.wordpress.com/2011/02/07/truth-equality-and-javascript/)
