# ECMAScript基础 - weixin_33985507的博客 - CSDN博客
2016年05月07日 16:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
## ECMAScript语法
- 区分大小写
- 变量是弱类型的，与 Java 和 C 不同，定义变量时只用**var运算符**，可以将它初始化为任意值。
- 与Java不同的还有变量可以存放不同类型的值。这是弱类型变量的优势。例如，可以把变量初始化为字符串类型的值，之后把它设置为数字值，如下所示：
```
var test = "hi";
alert(test);
test = 55;          // 可由字符串直接转换成数字
alert(test);
```
- 每行结尾的分号可有可无
- 有两种类型的注释：1) 单行注释以双斜杠开头（//） 2)多行注释以单斜杠和星号开头（/*），以星号和单斜杠结尾（*/）
## ECMAScript变量
### 声明变量
ECMAScript 中的变量是用 var 运算符（variable 的缩写）加变量名定义的。例如：
```
var test = "hi";
```
### 命名变量
变量名需要遵守两条简单的规则：
- 第一个字符必须是字母、下划线（_）或美元符号（$）
- 余下的字符可以是下划线、美元符号或任何字母或数字字符
### 命名规则
- Camel 标记法
首字母是小写的，接下来的字母都以大写字符开头。例如：
```
var myTestValue = 0, mySecondValue = "hi";
```
- Pascal 标记法
首字母是大写的，接下来的字母都以大写字符开头。例如：
```
var MyTestValue = 0, MySecondValue = "hi";
```
- 匈牙利类型标记法
在以 Pascal 标记法命名的变量前附加一个小写字母（或小写字母序列），说明该变量的类型。例如，i 表示整数，s 表示字符串，如下所示:
```
var iMyTestValue = 0, sMySecondValue = "hi";
```
推荐使用匈牙利类型标记法。
### 变量声明不是必须的
ECMAScript 另一个有趣的方面（也是与大多数程序设计语言的主要区别），是在使用变量之前不必声明。例如：
```
var sTest = "hello ";
sTest2    = sTest + "world";  // 变量声明不是必须的
alert(sTest2);
```
ECMAScript 的解释程序遇到未声明过的标识符时，**用该变量名创建一个全局变量，并将其初始化为指定的值。**
这是该语言的便利之处，不过如果不能紧密跟踪变量，这样做也很危险。最好的习惯是像使用其他程序设计语言一样，总是声明所有变量。
## 值
在 ECMAScript 中，变量可以存在两种类型的值，即原始值和引用值。
### 原始值(primitive type)
- 存储在栈（stack）中的简单数据段，也就是说，它们的值直接存储在变量访问的位置。
- 原始类型有5种：Undefined、Null、Boolean、Number和String型。由于这些原始类型占据的空间是固定的，所以可将他们存储在较小的内存区域(栈)中。这样存储便于迅速查寻变量的值。
- **在许多语言中，字符串都被看作引用类型，而非原始类型，因为字符串的长度是可变的。ECMAScript 打破了这一传统。**
<br />
### 引用值
- 存储在堆（heap）中的对象，也就是说，存储在变量处的值是一个指针（point），指向存储对象的内存处。
- 
由于引用值的大小会改变，所以不能把它放在栈中，否则会降低变量查寻的速度。相反，放在变量的栈空间中的值是该对象存储在堆中的地址。地址的大小是固定的，所以把它存储在栈中对变量性能无任何负面影响。如下图所示：
![1402261-2863cf8cfe602799.gif](https://upload-images.jianshu.io/upload_images/1402261-2863cf8cfe602799.gif)
原始值与引用值
<br />
## 原始类型
### typeof 运算符
对变量或值调用 typeof 运算符将返回下列值（**注意是小写**）之一：
- undefined - 如果变量是 Undefined 类型的
- boolean - 如果变量是 Boolean 类型的
- number - 如果变量是 Number 类型的
- string - 如果变量是 String 类型的
- object - 如果变量是一种**引用类型或Null类型**的
注释：您也许会问，为什么 typeof 运算符对于 null 值会返回 "Object"。这实际上是 JavaScript 最初实现中的一个错误，然后被 ECMAScript 沿用了。现在，null 被认为是对象的占位符，从而解释了这一矛盾，但从技术上来说，它仍然是原始值。
<br />
### Undefined类型
Undefined 类型只有一个值，即 undefined(**注意小写**)。当声明的变量未初始化时，该变量的默认值是 undefined。
```
var oTemp;
alert(typeof oTemp);        // 输出 "undefined"
alert(oTemp == undefined);  // 输出 "true"
alert(typeof oTemp2);       // 输出 "undefined"
```
如果对 oTemp2 使用除 typeof 之外的其他运算符的话，会引起错误，因为其他运算符只能用于已声明的变量上。
例如，下面的代码将引发错误：
```
var oTemp;
alert(oTemp2 == undefined);
```
当函数无明确返回值时，返回的也是值 "undefined"，如下所示：
```
function testFunc() {
}
alert(testFunc() == undefined);  // 输出 "true"
```
<br />
### Null类型
另一种只有一个值的类型是 Null，它只有一个专用值 null(**注意小写**)，值 undefined 实际上是从值 null 派生来的，因此 ECMAScript 把它们定义为相等的。
```
alert(null == undefined);  //输出 "true"
```
尽管这两个值相等，但它们的含义不同。undefined是声明了变量但未对其初始化时赋予该变量的值，null则用于表示尚未存在的对象。如果函数或方法要返回的是对象，那么找不到该对象时，返回的通常是 null。
<br />
### Boolean类型
Boolean 类型是 ECMAScript 中最常用的类型之一。它有两个值 true 和 false。
即使 false 不等于 0，0 也可以在必要时被转换成 false，这样在 Boolean 语句中使用两者都是安全的。
```
var bTest = false;
alert(typeof bTest);  // 输出"boolean"
```
<br />
### Number类型
ECMA-262 中定义的最特殊的类型是 Number 类型。这种类型既可以表示 32 位的整数，还可以表示 64 位的浮点数。
对于浮点字面量的有趣之处在于，用它进行计算前，真正存储的是字符串。
ECMAScript 默认把具有 6 个或 6 个以上前导 0 的浮点数转换成科学计数法。
Number类型定义了几个特殊值：
- Number.MAX_VALUE
- Number.MIN_VALUE
- NaN（Not a Number）
当计算生成的数大于 Number.MAX_VALUE 时，它将被赋予值 Number.POSITIVE_INFINITY，意味着不再有数字值。同样，生成的数值小于 Number.MIN_VALUE 的计算也会被赋予值 Number.NEGATIVE_INFINITY，也意味着不再有数字值。如果计算返回的是无穷大值，那么生成的结果不能再用于其他计算。
与无穷大一样，NaN 也不能用于算术计算。NaN 的另一个奇特之处在于，它与自身不相等，这意味着下面的代码将返回 false：
```
alert(NaN == NaN);     // 输出 "false"
```
出于这个原因，不推荐使用 NaN 值本身。函数 isNaN() 会做得相当好：
```
alert(isNaN("blue"));  // 输出 "true"
alert(isNaN("666"));   // 输出 "false"
```
<br />
### String类型
String 类型的独特之处在于，它是唯一没有固定大小的原始类型。可以用字符串存储 0 或更多的 Unicode 字符，有 16 位整数表示。
字符串字面量是由双引号（"）或单引号（'）声明的。而Java则是用双引号声明字符串，用单引号声明字符。但是**由于ECMAScript没有字符类型，所以可使用这两种表示法中的任何一种。**
```
var sColor1 = "red";    // 使用双引号定义字符串
var sColor2 = 'red';    // 使用单引号定义字符串
alert(typeof sColor1);  // 输出"string"
alert(typeof sColor2);  // 输出"string"
```
<br />
## 类型转换
### 转换成字符串
ECMAScript 的 Boolean、Number和String的**原始值**(下面会介绍引用值)的有趣之处在于它们是**伪对象，这意味着它们实际上具有属性和方法。**
这3种主要的原始类型都有toString()方法，可以把它们的值转换成字符串。
提示：您也许会问，“字符串还有toString()方法吗，这不是多余吗？”是的，的确如此，不过ECMAScript定义**所有对象都有toString()方法，无论它是伪对象，还是真对象。**因为 String 类型属于伪对象，所以它一定有toString()方法。
Boolean 类型的 toString() 方法只是输出 "true" 或 "false"，结果由变量的值决定：
```
var bFound = false;
alert(bFound.toString());   //输出 "false"
```
Number 类型的 toString() 方法比较特殊，它有两种模式，即默认模式和基模式。
在默认模式中，无论最初采用什么表示法声明数字，Number 类型的 toString() 方法返回的都是数字的十进制表示。
```
var iNum1 = 10;
var iNum2 = 10.0;
alert(iNum1.toString());    //输出 "10"
alert(iNum2.toString());    //输出 "10"
```
采用 Number 类型的 toString() 方法的基模式，可以用不同的基输出数字，例如二进制的基是 2，八进制的基是 8，十六进制的基是 16。
```
var iNum = 10;
alert(iNum.toString(2));    //输出 "1010"
alert(iNum.toString(8));    //输出 "12"
alert(iNum.toString(16));   //输出 "A"
```
<br />
### 转换成数字
ECMAScript 提供了两种把非数字的原始值转换成数字的方法，即 parseInt() 和 parseFloat()。
#### parseInt()方法
**从位置 0 开始查看每个字符，直到找到第一个非法的字符为止，然后把该字符之前的字符串转换成整数。** 8进制的前导符"0"和16进制的"0x"被认为是合法的字符。
- 一般转换规则:
```
var iNum1 = parseInt("123");        // 10进制，返回 123
var iNum1 = parseInt("010");        // 8进制，返回 8
var iNum1 = parseInt("0xA");        // 16进制，返回 10
var iNum1 = parseInt("56.9");       // 浮点数，返回 56
var iNum1 = parseInt("12345red");   // 数字与字符组合，返回 12345
var iNum1 = parseInt("red");        // 字符串，返回 NaN
```
parseInt() 方法还有基模式，可以把二进制、八进制、十六进制或其他任何进制的字符串转换成整数。
```
var iNum1 = parseInt("10", 2);  // 返回 2
var iNum2 = parseInt("10", 8);  // 返回 8
var iNum3 = parseInt("10", 10); // 返回 10
```
#### parseFloat()方法
parseFloat() 方法与 parseInt() 方法的处理方式相似，从位置 0 开始查看每个字符，直到找到第一个非有效的字符为止，然后把该字符之前的字符串转换成整数。
不过，对于这个方法来说，第一个出现的小数点是有效字符。如果有两个小数点，第二个小数点将被看作无效的。parseFloat() 会把这个小数点之前的字符转换成数字。这意味着字符串 "11.22.33" 将被解析成 11.22。
使用 parseFloat() 方法的另一不同之处在于，字符串必须以十进制形式表示浮点数，而不是用八进制或十六进制。该方法会忽略前导 0，所以八进制数 0102 将被解析为 102。对于十六进制数 0xA，该方法将返回 0，因为在浮点数中，x 不是有效字符。
```
var fNum3 = parseFloat("11.2");     // 返回 11.2
var fNum1 = parseFloat("12345red"); // 返回 12345
var fNum5 = parseFloat("0102");     // 返回 102
var fNum2 = parseFloat("0xA");      // 返回 0
var fNum4 = parseFloat("11.22.33"); // 返回 11.22
var fNum1 = parseFloat("red");      // 返回 NaN
```
<br />
### 强类型转换（type casting）
ECMAScript 中可用的 3 种强制类型转换如下：
- Boolean(value) - 把给定的值转换成 Boolean 型；
- Number(value) - 把给定的值转换成数字（可以是整数或浮点数）；
- String(value) - 把给定的值转换成字符串；
#### Boolean()函数
当要转换的值是至少有一个字符的字符串、非 0 数字或对象时，Boolean() 函数将返回 true。如果该值是空字符串、数字 0、undefined 或 null，它将返回 false。
```
var b1 = Boolean("");           // false - 空字符串
var b2 = Boolean("hello");      // true -  非空字符串
var b1 = Boolean(0);            // false - 零
var b1 = Boolean(50);           // true -  非零数字
var b1 = Boolean(null);         // false - null
var b1 = Boolean(undefined);    // false - undefined
var b1 = Boolean(new object()); // true -  对象
```
<br />
#### Number()函数
Number() 函数的强制类型转换与 parseInt() 和 parseFloat() 方法的处理方式相似，只是**它转换的是整个值，而不是部分值。**
parseInt() 和 parseFloat() 方法只转换第一个无效字符之前的字符串，因此 "1.2.3" 将分别被转换为 "1" 和 "1.2"。
用 Number() 进行强制类型转换，"1.2.3" 将返回 NaN，因为整个字符串值不能转换成数字。如果字符串值能被完整地转换，Number() 将判断是调用 parseInt() 方法还是 parseFloat() 方法。
```
var b1 = Number("12");          // 12
var b2 = Number("1.2");         // 1.2
var b2 = Number("1.2.3");       // NaN
var b1 = Number(false);         // 0
var b1 = Number(true);          // 1
var b1 = Number(null);          // 0，注意与undefined的区别
var b1 = Number(undefined);     // NaN
var b1 = Boolean(new object()); // NaN
```
<br />
#### String()函数
最后一种强制类型转换方法 String() 是最简单的，因为它可把任何值转换成字符串。
要执行这种强制类型转换，只需要调用作为参数传递进来的值的 toString() 方法。
强制转换成字符串和调用 toString() 方法的唯一不同之处在于，对 null 和 undefined 值强制类型转换可以生成字符串而不引发错误：
```
var s1 = String(null);      // String()方法可正常转换，返回"null"
var s2 = String(undefined); // String()方法可正常转换，返回"undefined"
var oNull = null;
var s3 = oNull.toString();  // toString()方法会引发错误
```
<br />
## 引用类型
引用类型通常叫做类（class）。对应的**对象即是引用值。**
对象是由 new 运算符加上要实例化的对象的名字创建的。
```
var o = new Object(); // 创建一个对象(引用值)
```
<br />
### Object对象
与Java中的java.lang.Object相似，ECMAScript中的所有对象都由这个对象继承而来，Object对象中的所有属性和方法都会出现在其他对象中。
Object 对象具有下列属性：
- constructor: 对创建对象的函数的引用（指针）。对于 Object 对象，该指针指向原始的 Object() 函数。
- Prototype: 对该对象的对象原型的引用。对于所有的对象，它默认返回 Object 对象的一个实例。
Object 对象还具有几个方法：
- hasOwnProperty(property): 判断对象是否有某个特定的属性。必须用字符串指定该属性。（例如，o.hasOwnProperty("name")）
- IsPrototypeOf(object): 判断该对象是否为另一个对象的原型。
- PropertyIsEnumerable: 判断给定的属性是否可以用 for...in 语句进行枚举。
- ToString(): 返回对象的原始字符串表示。对于 Object 对象，ECMA-262 没有定义这个值，所以不同的 ECMAScript 实现具有不同的值.
- ValueOf(): 返回最适合该对象的原始值。对于许多对象，该方法返回的值都与 ToString() 的返回值相同。
<br />
### Boolean对象
Boolean对象是**Boolean原始类型的引用类型。**
要创建Boolean对象，只需要传递Boolean值作为参数：
```
var oFalseObject = new Boolean(false);
var bResult = oFalseObject && true;     // 输出 true !!!
```
在 Boolean 表达式中，所有对象都会被自动转换为 true，所以 oFalseObject 的值是 true。然后 true 再与 true 进行 AND 操作，结果为 true。
注意：虽然你应该了解 Boolean 对象的可用性，不过**最好还是使用Boolean原始值**，避免发生这一节提到的问题。
<br />
### Number对象
Number 对象是**Number原始类型的引用类型。**
要得到数字对象的 Number 原始值，只需要使用 valueOf() 方法：
```
var oNumberObject = new Number(68);
var iNumber = oNumberObject.valueOf();  // 返回Number的原始值
```
除了从 Object 对象继承的标准方法外，Number 对象还有几个处理数值的专用方法。
#### toFixed()方法
方法返回的是具有指定位数小数的数字的**字符串**表示。
```
var oNumberObject = new Number(68);
alert(oNumberObject.toFixed(2));        // 输出 "68.00"
```
<br />
#### toExponential()方法
方法返回的是用科学计数法表示的数字的**字符串**形式。
```
var oNumberObject = new Number(68);
alert(oNumberObject.toExponential(1));  // 输出 "6.8e+1"
```
<br />
#### toPrecision()方法
方法根据最有意义的形式来返回数字的预定形式或指数形式。它有一个参数，即用于表示数的数字总数（不包括指数）。
```
var oNumberObject = new Number(68);
alert(oNumberObject.toPrecision(1));  // 用1位数字表示数字68，输出 "7e+1"
```
```
var oNumberObject = new Number(68);
alert(oNumberObject.toPrecision(2));  // 用2位数字表示数字68，输出 "68"
```
```
var oNumberObject = new Number(68);
alert(oNumberObject.toPrecision(3));  // 用3位数字表示数字68，输出 "68.0"
```
toFixed()、toExponential() 和 toPrecision() 方法都会进行舍入操作，以便用正确的小数位数正确地表示一个数。
提示：与Boolean对象相似，Number对象也很重要，不过应该少用这种对象，以避免潜在的问题。**只要可能，都使用数字的原始表示法**。
<br />
### String对象
String 对象是**String原始类型的引用类型。**
#### length属性(是属性不是方法！)
它是字符串中的**字符**个数
```
var oStringObject = new String("hello world");
alert(oStringObject.length);                    // 输出 "11"
```
注意，即使字符串包含双字节的字符，每个字符也只算一个字符。
<br />
#### slice() & substring()
与 concat() 方法一样，slice() 和 substring() 方法都 **不改变String对象自身的值。只返回原始的String值**。
这两种方法返回的都是要处理的字符串的子串，都接受一个或两个参数。第一个参数是要获取的子串的起始位置，第二个参数（如果使用的话）是要获取子串终止前的位置（也就是说，获取终止位置处的字符不包括在返回的值内）。如果省略第二个参数，终止位就默认为字符串的长度。
```
var oStringObject = new String("hello world");
alert(oStringObject.slice("3"));                // 输出 "lo world"
alert(oStringObject.substring("3"));            // 输出 "lo world"
alert(oStringObject.slice("3", "7"));           // 输出 "lo w"
alert(oStringObject.substring("3", "7"));       // 输出 "lo w"
```
为什么有两个功能完全相同的方法呢？事实上，这两个方法并不完全相同，不过只在参数为负数时，它们处理参数的方式才稍有不同。
```
var oStringObject = new String("hello world");
alert(oStringObject.slice("-3"));               // 输出 "rld"
alert(oStringObject.substring("-3"));           // 输出 "hello world"
alert(oStringObject.slice("3, -4"));            // 输出 "lo w"
alert(oStringObject.substring("3, -4"));        // 输出 "hel"
```
slice方法接收负值时，将其理解为倒着数的字符索引;而substring方法将负值直接解析成0(即第1个字符的索引)。
使用参数 3 和 -4 时，slice() 将被转换成 slice(3, 7)，返回 "lo w"。而 substring() 方法则将两个参数解释为 substring(3, 0)，实际上即 substring(0, 3)，因为 substring() 总把较小的数字作为起始位，较大的数字作为终止位。因此，substring("3, -4") 返回的是 "hel"。
<br />
## instanceof运算符
在使用 typeof 运算符时采用引用类型存储值会出现一个问题，无论引用的是什么类型的对象，它都返回 "object"。
instanceof 运算符与 typeof 运算符相似，用于识别正在处理的对象的类型。与 typeof 方法不同的是，instanceof 方法要求开发者明确地确认对象为某特定类型。例如：
```
var oStringObject = new String("hello world");
alert(oStringObject instanceof String);         // 输出 "true"
```
<br />
更多请参考：[W3School](https://link.jianshu.com?t=http://www.w3school.com.cn)
