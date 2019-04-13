
# Java静态导入(import static)需谨慎 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月31日 06:40:08[seven-soft](https://me.csdn.net/softn)阅读数：998


从Java 5 开始引入了静态导入语法（import static），其目是为了减少字符输入量，提高代码的可阅读性，以便更好地理解程序。我们先来看一个不使用静态导入的例子，也就是一般导入：
public class MathUtils{
// 计算圆面积
public static double calCircleArea(double r){
return Math.PI * r * r;
}
// 计算球面积
public static double calBallArea(double r){
return 4* Math.PI * r * r;
}
}
这是很简单的数学工具类，我们在这两个计算面积的方法中都引入了java.lang.Math 类（该类是默认导入的）中的PI（圆周率）常量，而Math 这个类写在这里有点多余，特别是如果MathUtils 中的方法比较多时，如果每次都要敲入Math
 这个类，繁琐且多余，静态导入可解决此类问题，使用静态导入后的程序如下：
import static java.lang.Math.PI;
public class MathUtils{
// 计算圆面积
public static double calCircleArea(double r){
return PI * r * r;
}
// 计算球面积
public static double calBallArea(double r){
return 4 * PI * r * r;
}
}
静态导入的作用是把Math 类中的PI 常量引入到本类中，这会使程序更简单，更容易阅读，只要看到PI 就知道这是圆周率，不用每次都要把类名写全了。但是，滥用静态导入会使程序更难阅读，更难维护。静态导入后，代码中就不用再写类名了，但是我们知道类是“一类事物的描述”，缺少了类名的修饰，静态属性和静态方法的表象意义可以被无限放大，这会让阅读者很难弄清楚其属性或方法代表何意，甚至是哪一个类的属性（方法）都要思考一番（当然，IDE
 友好提示功能是另说），特别是在一个类中有多个静态导入语句时，若还使用了*（星号）通配符，把一个类的所有静态元素都导入进来了，那简直就是恶梦。我们来看一段例子：
import static java.lang.Double.*;
import static java.lang.Math.*;
import static java.lang.Integer.*;
import static java.text.NumberFormat.*;
public class Client {
// 输入半径和精度要求，计算面积
public static void main(String[] args) {
double s = PI * parseDouble(args[0]);
NumberFormat nf = getInstance();
nf.setMaximumFractionDigits(parseInt(args[1]));
formatMessage(nf.format(s));
}
// 格式化消息输出
public static void formatMessage(String s){
System.out.println(" 圆面积是："+s);
}
}
就这么一段程序，看着就让人火大：常量PI，这知道，是圆周率；parseDouble 方法可能是Double 类的一个转换方法，这看名称也能猜测到。那紧接着的getInstance 方法是哪个类的？是Client 本地类？不对呀，没有这个方法，哦，原来是NumberFormate
 类的方法，这和formateMessage 本地方法没有任何区别了—这代码也太难阅读了，非机器不可阅读。所以，对于静态导入，一定要遵循两个规则：不使用*（星号）通配符，除非是导入静态常量类（只包含常量的类或接口）。
方法名是具有明确、清晰表象意义的工具类。
何为具有明确、清晰表象意义的工具类？我们来看看JUnit 4 中使用的静态导入的例子，代码如下：
import static org.junit.Assert.*;
public class DaoTest {
@Test
public void testInsert(){
// 断言
assertEquals("foo", "foo");
assertFalse(Boolean.FALSE);
}
}
我们从程序中很容易判断出assertEquals 方法是用来断言两个值是否相等的，assertFalse方法则是断言表达式为假，如此确实减少了代码量，而且代码的可读性也提高了，这也是静态导入用到正确地方所带来的好处。

