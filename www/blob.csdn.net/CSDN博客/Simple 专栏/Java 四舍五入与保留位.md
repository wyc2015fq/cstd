# Java 四舍五入与保留位 - Simple 专栏 - CSDN博客
2012年02月03日 14:02:15[Simple_Zz](https://me.csdn.net/love284969214)阅读数：1703标签：[java																[Java面试																[编程																[Java基础](https://so.csdn.net/so/search/s.do?q=Java基础&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=Java面试&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
###  四舍五入
```
// 四舍五入经典案例
public void testRound() {
    System.out.println("11.5的四舍五入值：" + Math.round(12.5));
    System.out.println("-11.5的四舍五入值：" + Math.round(-12.5));
}
// 运行结果
// 11.5的四舍五入值：12
// -11.5的四舍五入值：-11
```
- 四舍：0.000、0.001、0.002、0.003、0.004。这些舍的都是银行赚的钱。
- 五入：0.005、0.006、0.007、0.008、0.009。这些入的都是银行亏的钱。
- 所以对于银行来说它的盈利应该是0.000 + 0.001 + 0.002 + 0.003 + 0.004 - 0.005 - 0.004 - 0.003 - 0.002 - 0.001 = -0.005。
- 从结果中可以看出每10笔的利息银行可能会损失0.005元，千万别小看这个数字，这对于银行来说就是一笔非常大的损失。
- 面对这个问题就产生了如下的银行家涉入法了。
- 该算法是由美国银行家提出了，主要用于修正采用上面四舍五入规则而产生的误差。如下：
- 舍去位的数值小于5时，直接舍去。
- 舍去位的数值大于5时，进位后舍去。
- 当舍去位的数值等于5时，若5后面还有其他非0数值，则进位后舍去。
- 若5后面是0时，则根据5前一位数的奇偶性来判断，奇数进位，偶数舍去。
对于上面的规则我们举例说明
- 1.556   = 1.56 -----六入
- 1.554   = 1.55 -----四舍
- 1.545   = 1.54 -----五后无数，若前位为偶数应舍去。
- 1.555   = 1.56 -----五后无数，若前位为奇数应进位。
- 1.5551 = 1.56 -----五后有数，进位。
下面实例是使用银行家舍入法：在上面简单地介绍了银行家舍入法，目前java支持7中舍入法
```
public void testBigDecimal {
    BigDecimal d = new BigDecimal(100000);      //存款
    BigDecimal r = new BigDecimal(0.001875*3); //利息
    BigDecimal i = d.multiply(r).setScale(2,RoundingMode.HALF_EVEN); //使用银行家算法
    System.out.println("季利息是："+i);
}
// 程序输出
// 季利息是：562.50
```
在上面简单地介绍了银行家舍入法，目前java支持7中舍入法：
- ROUND_UP：远离零方向舍入。向绝对值最大的方向舍入，只要舍弃位非0即进位。
- ROUND_DOWN：趋向零方向舍入。向绝对值最小的方向输入，所有的位都要舍弃，不存在进位情况。
- ROUND_CEILING：向正无穷方向舍入。向正最大方向靠拢。若是正数，舍入行为类似于ROUND_UP，若为负数，舍入行为类似于ROUND_DOWN。Math.round()方法就是使用的此模式。
- ROUND_FLOOR：向负无穷方向舍入。向负无穷方向靠拢。若是正数，舍入行为类似于ROUND_DOWN；若为负数，舍入行为类似于ROUND_UP。
- HALF_UP：最近数字舍入(5进)。这是我们最经典的四舍五入。
- HALF_DOWN：最近数字舍入(5舍)。在这里5是要舍弃的。
- HAIL_EVEN：银行家舍入法。
### 保留位
### **方法一：四舍五入**
```
double f = 111231.5585;
BigDecimal b = new BigDecimal(f);
double f1 = b.setScale(2, RoundingMode.HALF_UP).doubleValue();
```
在这里使用BigDecimal ，并且采用setScale方法来设置精确度，同时使用RoundingMode.HALF_UP表示使用最近数字舍入法则来近似计算。在这里我们可以看出BigDecimal和四舍五入是绝妙的搭配。
### **方式二：**
```
// java.text.DecimalFormat df = new java.text.DecimalFormat("#.00");
// df.format(要格式化的数字);
// #.00 表示两位小数 #.000三位小数 以此类推…
java.text.DecimalFormat df = new java.text.DecimalFormat("#.00").format(3.1415926);
```
### **方式三：**
```
double d = 3.1415926;
String result = String .format(”%.2f”);
// %.2f %. 表示 小数点前任意位数 2 表示两位小数 格式后的结果为f 表示浮点型。
```
