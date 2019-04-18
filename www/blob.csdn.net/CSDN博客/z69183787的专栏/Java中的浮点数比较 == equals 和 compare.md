# Java中的浮点数比较 == equals 和 compare - z69183787的专栏 - CSDN博客
2016年11月22日 15:20:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7045
前几天有位同学问我一个问题，为什么float和double不能直接用==比较？
例如：
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- System.out.println(0.1d == 0.1f);  
结果会是flase
当时我只是简单的回答，因为精度丢失，比较结果是不对的。
那么，到底为什么不对呢？ 此文略作整理记录。
## 类型升级(type promotion)
首先，来看看[Java](http://lib.csdn.net/base/javaee)中的几种原生的数值类型进行==或!=比较运算的时候会发生什么。
如果运算符两边的数值类型不同，则首先会进行类型升级(type promotion)，规则如下：
- 如果运算符任意一方的类型为double，则另一方会转换为double
- 否则，如果运算符任意一方的类型为float，则另一方会转换为float
- 否则，如果运算符任意一方的类型为long，则另一方会转换为long
- 否则，两边都会转换为int
详情见官方文档 [http://docs.oracle.com/javase/specs/jls/se7/html/jls-5.html#jls-5.6.2](http://docs.oracle.com/javase/specs/jls/se7/html/jls-5.html#jls-5.6.2)
然后，浮点数执行浮点数相等比较（int或者long执行整型相等比较）
那么，上面那个例子，float首先会被升级为double，然后执行浮点数相等比较。那为什么会返回flase呢？
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- System.out.println(0.1d == (**double**) 0.1f);  
结果为false
## 舍入误差(round-off error)
我们知道，根据IEEE 754，单精度的float是32位，双精度的double为64位，如下图：
![](https://img-blog.csdn.net/20150117165126171)
其中，第一部分(s)为符号位，第二部分(exponent)为指数位，第三部分(mantissa)为基数部分。 这是科学计数法的二进制表示。
那么，既然位数是固定的，要表示像 1/3=0.3333333333333...或者pi=3.1415926..... 这样的无限循环小数，就变得不可能了。
根据规范，则需要将不能标识的部分舍掉。
第二，还与10进制不同的是，二进制对于一些有限的小数，也不能精确的标示。比如像0.1这样的小数，用二进制也无法精确表示。所以，也需要舍掉。
关于0.1无法用二进制精确表示，可以参见文章： [http://en.wikipedia.org/wiki/Floating_point](http://en.wikipedia.org/wiki/Floating_point)
### 补充：科学计数法及浮点数的二进制表示
首先，再来回忆一下，科学计数法是什么样子的。一个数，可以有多重表示方法。
例如，254可以有但不仅仅有以下几种表示：
![](https://img-blog.csdn.net/20150123101432749)
上面这是10进制的表示方式，也就是基数为10的表示方式。 基数，就是上面例子中 25.4 * 10 这里的10，当然，指数是1.
但是如果基数是2，需要怎么转换呢？
看下面这个例子：
![](https://img-blog.csdn.net/20150123102103436)
所以，经过这个转换，就可以用IEEE 754表示一个浮点数了。
### 单精度转换为双精度会发生什么
首先，我们来看，单精度浮点数0.1表示成二进制会是什么样子的：
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- System.out.println(Integer.toBinaryString(Float.floatToIntBits(0.1f)));  
结果是：111101110011001100110011001101
然后，双精度的浮点数0.1的二进制会是什么样子呢：
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- System.out.println(Long.toBinaryString(Double.doubleToLongBits(0.1d)));  
结果是：11111110111001100110011001100110011001100110011001100110011010
然后，在比较float==double的时候，首先，会将float进行类型升级，得到的新的double 的值会是什么样子：
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- System.out.println(Long.toBinaryString(Double.doubleToLongBits(0.1f)));  
结果是：11111110111001100110011001100110100000000000000000000000000000
我们可以看到，经过转换后的double的值已经和直接赋值的double的值不相等了。所以这样用==比较返回的值是false
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- System.out.println(Integer.toBinaryString(Float.floatToIntBits(0.1f)));  
- System.out.println(Long.toBinaryString(Double.doubleToLongBits(0.1d)));  
- System.out.println(Long.toBinaryString(Double.doubleToLongBits(0.1f)));  
![](https://img-blog.csdn.net/20150117172408622)
## 用equals方法进行比较
既然，用==或者!=来比较非常坑爹，那可以用equals来进行比较吗？ **我的答案是一定不能。**
看看下面2个例子。
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- Double a = Double.valueOf("0.0");  
- Double b = Double.valueOf("-0.0");  
- System.out.println(a.equals(b));  
返回值是false
这是经常出现的场景，不过我简化了。试想，经过一系列运算过后，一个结果为0，一个结果为-0，结果不等。很难接受是吧？
如果上面那个列子只是坑，下面这个简直就是地雷了。
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- Double a = Math.sqrt(-1.0);  
- Double b = 0.0d / 0.0d;  
- Double c = a + 200.0d;  
- Double d = b + 1.0d;  
- System.out.println(a.equals(b));  
- System.out.println(b.equals(c));  
- System.out.println(c.equals(d));  
连续3个比较返回都是true，这个简直无法理解。
其实，在Java里面，a和b表示为NaN(Not a Number)，既然不是数字，就无法比较嘛。
**但是equals方法是比较2个对象是否等值，而不是对象的值是否相等，所以equals方法设计的初衷根本就不是用来做数值比较的。勿乱用。**
关于equals方法，我另外一篇记录会做更多解释。
## 用compareTo方法进行比较
虽然说它在设计上是用于数值比较的，但它表现跟equals方法一模一样——对于NaN和0.0与-0.0的比较上面。
另外，由于舍入误差的存在，也可能会导致浮点数经过一些运算后，结果会有略微不同。
**所以最好还是不要直接用Float.compareTo和Double.compareTo方法。**
## **结论**
在进行浮点数比较的时候，主要需要考虑3个因素
- NaN
- 无穷大/无穷小
- 舍入误差
NaN和无穷出现的可能场景如下
![](https://img-blog.csdn.net/20150123112601103)
所以，要比较浮点数是否相等，需要做的事情是：
- 排除NaN和无穷
- 在精度范围内进行比较
例如下面的列子：
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- **public****boolean** isEqual(**double** a, **double** b) {  
- **if** (Double.isNaN(a) || Double.isNaN(b) || Double.isInfinite(a) || Double.isInfinite(b)) {  
- **return****false**;  
-     }  
- **return** (a - b) < 0.001d;  
- }  
当然，如果在要求精确的场合，例如金融计算中，可以考虑BigDecimal这个类。
它什么都好，就是效率略低。需要自行在性能和精度之间取舍。
## 思考一下
为什么下面这种方式可能会出现精度问题
**[java]**[view plain](http://blog.csdn.net/wcxiaoych/article/details/42806313#)[copy](http://blog.csdn.net/wcxiaoych/article/details/42806313#)
- BigDecimal.valueOf(0.1d);  
- **new** BigDecimal(0.1d);  
## 引用列表
[http://www.radford.edu/aaray/ITEC_352/Lectures_files/ITEC352-Lecture08.pptx](http://www.radford.edu/aaray/ITEC_352/Lectures_files/ITEC352-Lecture08.pptx)
[http://docs.oracle.com/javase/specs/jls/se7/html/jls-5.html#jls-5.6.2](http://docs.oracle.com/javase/specs/jls/se7/html/jls-5.html#jls-5.6.2)
[http://docs.oracle.com/javase/specs/jls/se7/html/jls-15.html#jls-15.21.1](http://docs.oracle.com/javase/specs/jls/se7/html/jls-15.html#jls-15.21.1)
