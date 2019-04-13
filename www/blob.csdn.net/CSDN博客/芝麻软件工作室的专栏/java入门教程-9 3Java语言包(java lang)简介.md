
# java入门教程-9.3Java语言包(java.lang)简介 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:44:27[seven-soft](https://me.csdn.net/softn)阅读数：255个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



Java语言包（java.lang）定义了Java中的大多数基本类，**由Java语言自动调用，不需要显示声明**。该包中包含了Object类，Object类是整个类层次结构的根结点，同时还定义了基本数据类型的类，如：String、Boolean、Byter、Short等。这些类支持数字类型的转换和字符串的操作等，下面将进行简单介绍。
## Math类
Math类提供了常用的数学运算方法以及Math.PI和Math.E两个数学常量。该类是final的，不能被继承，类中的方法和属性全部是静态，不允许在类的外部创建Math类的对象。因此，只能使用Math类的方法而不能对其作任何更改。表8-1列出了Math类的主要方法。
表8-1 Math类的主要方法|方法|功能|
|int abs(int i)|求整数的绝对值（另有针对long、float、double的方法）|
|double ceil(double d)|不小于d的最小整数（返回值为double型）|
|double floor(double d)|不大于d的最大整数（返回值为double型）|
|int max(int i1,int i2)|求两个整数中最大数（另有针对long、float、double的方法）|
|int min(int i1,int i2)|求两个整数中最小数（另有针对long、float、double的方法）|
|double random()|产生0~1之间的随机数|
|int round(float f)|求最靠近f的整数|
|long round(double d)|求最靠近d的长整数|
|double sqrt(double a)|求平方根|
|double sin(double d)|求d的sin值（另有求其他三角函数的方法如cos，tan，atan）|
|double log(double x)|求自然对数|
|double exp(double x)|求e的x次幂（e|x|）|
|double pow(double a, double b)|求a的b次幂|
【例8-2】产生10个10~100之间的随机整数。
//********** ep8_2.java **********
class ep8_2{
public static void main(String args[]){
int a;
System.out.print("随机数为：");
for(int i=1;i<=10;i++){
a=(int)((100-10+1)*Math.random()+10);
System.out.print(" "+a);
}
System.out.println();
}
}
运行结果： 随机数为：12 26 21 68 56 98 22 69 68 31
由于产生的是随机数，例8-2每次运行的结果都不会相同。若要产生[a,b]之间的随机数其通式为：
（b-a+1）*Math.random()+a
## 字符串类
字符串是字符的序列。在 Java 中，字符串无论是常量还是变量都是用类的对象来实现的。java.lang
 提供了两种字符串类：String 类和 StringBuffer 类。
1.String 类
按照 Java 语言的规定，String 类是 immutable 的 Unicode 字符序列，其作用是实现一种不能改变的静态字符串。例如，把两个字符串连接起来的结果是生成一个新的字符串，而不会使原来的字符串改变。实际上，所有改变字符串的结果都是生成新的字符串，而不是改变原来字符串。
字符串与数组的实现很相似，也是通过 index 编号来指出字符在字符串中的位置的，编号从0 开始，第 2 个字符的编号为 1，以此类推。如果要访问的编号不在合法的范围内，系统会产生 StringIndexOutOfBoundsExecption
 异常。如果 index 的值不是整数，则会产生编译错误。
String 类提供了如表 8-2 所示的几种字符串创建方法。
表 8-2 String 创建字符串的方法|方法|功能|
|String s=”Hello!”|用字符串常量自动创建 String 实例。|
|String s=new String(String s)|通过 String 对象或字符串常量传递给构造方法。|
|public String(char value[])|将整个字符数组赋给 String 构造方法。|
|public String(char value[], int offset, int count)|将字符数组的一部分赋给 String 构造方法，offset 为起始下标，count为子数组长度。|
2.StringBuffer 类
String 类不能改变字符串对象中的内容，只能通过建立一个新串来实现字符串的变化。如果字符串需要动态改变，就需要用 StringBuffer 类。StringBuffer 类主要用来实现字符串内容的添加、修改、删除，也就是说该类对象实体的内存空间可以自动改变大小，以便于存放一个可变的字符序列。
StringBuffer 类提供的三种构造方法|构造方法|说明|
|StringBuffer()|使用该无参数的构造方法创建的 StringBuffer 对象，初始容量为 16 个字符，当对象存放的字符序列大于 16 个字符时，对象的容量自动增加。该对象可以通过 length()方法获取实体中存放的字符序列的长度，通过 capacity()方法获取当前对象的实际容量。|
|StringBuffer(int length)|使用该构造方法创建的 StringBuffer 对象，其初始容量为参数 length 指定的字符个数，当对象存放的字符序列的长度大于 length 时，对象的容量自动增加，以便存放所增加的字符。|
|StringBuffer(Strin str)|使用该构造方法创建的 StringBuffer 对象，其初始容量为参数字符串 str 的长度再加上 16 个字符。|
几种 StringBuffer 类常用的方法|方法|说明|
|append()|使用 append() 方法可以将其他 Java 类型数据转化为字符串后再追加到 StringBuffer 的对象中。|
|insert(int index, String str)|insert() 方法将一个字符串插入对象的字符序列中的某个位置。|
|setCharAt(int n, char ch)|将当前 StringBuffer 对象中的字符序列 n 处的字符用参数 ch 指定的字符替换，n 的值必须是非负的，并且小于当前对象中字符串序列的长度。|
|reverse()|使用 reverse()方法可以将对象中的字符序列翻转。|
|delete(int n, int m)|从当前 StringBuffer 对象中的字符序列删除一个子字符序列。这里的 n 指定了需要删除的第一个字符的下标，m 指定了需要删除的最后一个字符的下一个字符的下标，因此删除的子字符串从 n~m-1。|
|replace(int n, int m, String str)|用 str 替换对象中的字符序列，被替换的子字符序列由下标 n 和 m 指定。
|

