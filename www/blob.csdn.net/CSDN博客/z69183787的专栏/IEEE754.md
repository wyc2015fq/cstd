# IEEE754 - z69183787的专栏 - CSDN博客
2018年08月13日 10:24:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：183
[https://blog.csdn.net/charles_neil/article/details/59125861](https://blog.csdn.net/charles_neil/article/details/59125861)
# IEEE 754标准解读
## 简介
[IEEE 754 标准](https://en.wikipedia.org/wiki/IEEE_754-1985) 是IEEE二进位浮点数算术标准（IEEE Standard for Floating-Point Arithmetic）的标准编号,它规定了浮点数在计算机当中的存储方式以及算术标准等。
## 存储方式
计算机中数据均是按二进制的方式存储的，浮点数的存储也是如此。但是由于浮点数的特殊性，无法采用整数的补码存储方式，浮点数需要有特定的存储方式。总的来说，一个浮点数可以分为3部分存储：
- sign(符号)
- exponent(指数)
- fraction(尾数)
举例：0.00101(2)=1.01×2−30.00101(2)=1.01×2−3 
(2)表示二进制,上式中小数0.00101可以存储为3部分 
①sign: + 
②exponent: (-3) 
③fraction: 01 
为什么不存储小数点前面的数呢？ 
key:浮点数之所以叫浮点数，是因为小数点的位置是不确定的。 
如上面的0.00101(2)0.00101(2) 可以写成1.01×2−31.01×2−3 ，也可以写成10.1×2−410.1×2−4 ，而如果这台机器这样存，那台机器那样存，这可怎么计算呢？ 
因此，存储是统一化成科学记数法，即小数点之前一定是1。 
化成科学记数法之后最高位均为1，于是便将1省略，只存储小数点之后的尾数，这种机制使同样的内存空间可以存储更大范围的数。
单精度和双精度所占字节数不一样，他们②③部分的宽度自然不一样(如图分别为float和double) 
![float](https://img-blog.csdn.net/20170301201912129?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhcmxlc19uZWls/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![double](https://img-blog.csdn.net/20170301202144212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhcmxlc19uZWls/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**sign**
符号位在计算机中的存储是非常容易的，因为符号只有正负两种情况，因此需一个bit便可存储，0代表正，1代表负
**exponent**
指数是一个整数，一般来说整数会采用补码存储，但是补码存储不能方便地进行浮点数的比较以及运算(原因参见[wikipedia](https://en.wikipedia.org/wiki/IEEE_754-1985) )
于是exponent采用*biased exponent* = exponent + bias 
实际计算机中存储的是经过偏移的指数(*biased exponent*)，即在原来指数的基础上加上一个数(*float*是加上127，*double*是1023)，这个经过偏移的指数一定非负。 
举例:单精度浮点数，指数为-3，偏移过后为124，于是便存储为01111100
**fraction**
直接将fraction转化为二进制后存入计算机
## 规约与非规约(normalized & denormalized numbers)
我们先来计算按如此方式存储所能表示的最小的正数和最大的负数 
±1.0 × 2^minexp 
即fraction部分以及exponent部分均为最小，1是隐藏存储的，但是可不可以更小呢？ 
为了满足对精度的需求，使能表示的数更加地接近0，IEEE754标准引入了规约(normalized)与非规约(denormalized)的概念。
**normalized number**
标准规定规约型的浮点数，最高位隐藏存储为1，偏移后的指数b_exp范围为`0<b_exp<2^e - 1`. e指的是exponent部分的宽度。例如*float*指数宽度为8，能表示的最小指数为0-127 = -127。fraction部分直接存储
**denormalized number**
规定非规约的浮点数，最高位隐藏存储0，偏移后的指数b_exp为0，fraction部分直接存储(但是不能为0，原因往下看)。 
你也许会想既然规约和非规约都是隐藏存储最高位，那么计算机怎么知道隐藏的是1还是0呢？这个很简单，注意规约和非规约的b_exp范围是有区别的，计算机会根据这个进行判断。
## 特殊值
标准规定了一些特殊值(零，无穷，非数字)的存储方式 
如果 **指数** 是0 并且 **小数部分** 是0, 这个数±0 (和符号位相关,这就是上文说的**原因)**
如果 **指数** = 2^e - 1并且 **小数部分** 是0, 这个数是 ±无穷大 (同样和符号位相关) 
如果 **指数** = 2^e - 1并且 **小数部分** 非0, 这个数表示为不是一个数(NaN).
总结一下，列个表
|form|biased exponent|fraction|
|----|----|----|
|zero|0|0|
|denormalized|0|not 0|
|normalized|`0<b_exp<2^e - 1`||
|±infinity (±∞)|2^e - 1|0|
|NaN|2^e - 1|not 0|
参考资料：百度百科、Wikipedia
## 不精确存储
浮点数存在不精确存储的现象 
这种现象发生于一下几种情况之下：
- 有效位数过长
- 无法表示某些数
第一种很容易理解，以double类型为例，存储位数只有52(如果加上省略的那位是53位)，如果存储的数有效位数大于53，则无法准确存储 
第二种情况，举个例子吧，0.3就不能化成二进制
大一freshman，如发现错误之处望指出，感谢！
