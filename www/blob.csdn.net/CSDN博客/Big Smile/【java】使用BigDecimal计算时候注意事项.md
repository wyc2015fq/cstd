# 【java】使用BigDecimal计算时候注意事项 - Big Smile - CSDN博客
2018年03月31日 17:47:50[王啸tr1912](https://me.csdn.net/tr1912)阅读数：384
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        一般数据在用BigDecimal自带的运算方式时候是不会出现问题的，但是碰到了无限小数的时候，这个类直接运算方式就会出现一些取不到位或者溢出的错误。
**错误1：Non-terminating decimal expansion; no exact representable decimal result**
        这个错误翻译过来大概是：十进制小数点膨胀，没有确切的表示的十进制结果。其隐含的意义就是：无法结束的除法表达式；没有精确的除结果。这样的结果多出现在相除的数字最后结果是无限小数。例如：
```java
BigDecimal num1 = new BigDecimal("10");
BigDecimal num2 = new BigDecimal("3");
BigDecimal num3 = num1.divide(num2);
```
        进行这类运算的时候，尤其我们在业务需求中计算金额的时候会碰到许多，那么我们需要怎么处理呢？其实通过上一篇博客的分析，我们可以看出来，BigDecimal在处理数学运算的时候引入了scale这个量，就是小数度量的意思，然后我们在处理的时候可以选择保留的位数，然后设定规范，从而让程序自动舍去不要的数字。我们来看除法运算的一个重载：
BigDecimal.divide(BigDecimal divisor, int scale, RoundingMode roundingMode) ;
其中：scale为小数位数，roundingMode为小数模式。
所以我们只要指定了小数位数，然后再指定小数模式，就可以得到想要的结果了
保留小数的模式：
```
ROUND_CEILING
如果 BigDecimal 是正的，则做 ROUND_UP 操作；如果为负，则做 ROUND_DOWN 操作。
ROUND_DOWN
从不在舍弃(即截断)的小数之前增加数字。
ROUND_FLOOR
如果 BigDecimal 为正，则作 ROUND_UP ；如果为负，则作 ROUND_DOWN 。
ROUND_HALF_DOWN
若舍弃部分> .5，则作 ROUND_UP；否则，作 ROUND_DOWN 。
ROUND_HALF_EVEN
如果舍弃部分左边的数字为奇数，则作 ROUND_HALF_UP ；如果它为偶数，则作 ROUND_HALF_DOWN 。
ROUND_HALF_UP
若舍弃部分>=.5，则作 ROUND_UP ；否则，作 ROUND_DOWN 。
ROUND_UNNECESSARY
该“伪舍入模式”实际是指明所要求的操作必须是精确的，，因此不需要舍入操作。
ROUND_UP
总是在非 0 舍弃小数(即截断)之前增加数字。
```
我们代码写成这样就不会有问题了，但是未知的位数被舍弃了：
```java
BigDecimal num3 = num1.divide(num2，10，ROUND_HALF_DOWN);
```
其实这个保留小数位数还有另外一种写法：
```java
BigDecimal num3 = num1.divide(num2，10).setScale(2);
```
这样写也是可以保留两位小数的，但是在没有指定策略的情况下会碰到另外一个错误：
**错误2： java.lang.ArithmeticException: Rounding necessary**
这种就是没有指定舍弃规则的错误了，总之加上就好了。
```java
BigDecimal num3 = num1.divide(num2，10).setScale(2,BigDecimal.ROUND_HALF_UP);
```
或者
```java
BigDecimal num3 = num1.divide(num2，10).setScale(2,RoundingMode.HALF_UP);
```
**误差1：业务上舍弃导致的在保留精度上少1的错误**
        我们都知道，在进行舍弃或者进位的时候，我们其实是放弃了属于数字的一部分精度上的数字，很显然，我们如果要是做按照比例分钱或者是平账一类的系统的时候会出现一些短账的错误。
比如，三个人要均分一笔钱，假设是100元，那么他们每个人所分得的钱都是100的三分之一。我们在计算的时候都取用满5进一，不满就舍去的保留算法，假设结果就是两位小数，那么通过上述计算，每个人得到的钱都是33.33元，但是33.33*3=99.99，离100还差0.01，这就是在精度的位上差1的结果。
       但是现在还没有很好的处理结果，一般这种业务上，两方的除法操作，需要不同的舍取策略才能达到最后的平衡，比如一个用up一个用down，即一个任何情况都进位，一个任何情况都舍弃。但是这样也会产生不公平的因素，但是金额的总数可以达到一个平衡。
