# Java中基本的语法规范 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 15:28:48[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：743








- Java中类名都是以大写字母开头的。例如Welcome类在定义的时候就是：public class Welcome{...}。
- 为了运行某一个类，那么这个类里面一定要有main方法（注：在Java中“方法”就是C语言中“函数”的含义）。
- Java中的输出。需要输出信息的时候，需要使用：System.out.println("Welcome to Java!");  除此之外，println括号中也不一定要是字符串的形式，只有在输出字符串的时候才需要用双引号框住，也可以显示一个计算公式的结果，例如：System.out.println(1+1); 那么就显示结果2；
- Java中的输入。输入需要用到System.in来表示标准输入设备。我们可以创建一个Scanner类的对象input，并将System.in的值赋值给对象input。完整的代码为：Scanner input= new Scanner(System.in);  因为Scanner类在java.util包中，所以在程序开头需要import java.util.Scanner; 在程序主体中，把输入的值赋值给变量的时候，就使用：
 double radius = input.nextDouble(); 便可以赋值了，注意nextDouble是Scanner对象的一个方法，也可以根据输入数值的不同数据类型，调用不同的方法，例如： nextInt(), nextFloat()；，每次输入之后点击回车键即可以读入。当输入字符串的时候，我们可以使用input.next()方法读取以空白字符结束的字符串（空白字符包括  ' ', '\t', '\f', '\r', '\n'）。而input.nextLine();可以输入一行文本，该方法读取的是按下回车键为结束标志的字符串，空白字符不管用了。
- Java中是区分大小写的; 对于变量，首字母一定要小写，如果该变量名有多个单词，那么从第二个单词开始，首字母均大写；例如：radius, interestDate; 方法的命名方法和变量一样，例如：showInputDialog. 类名中的每个单词都需要大写，例如：ComputeArea, Math等。 
- 假如用到Java中一些预定义的类，那么需要在程序的一开始导入这些预定义类放置的包，例如：在GUI显示对话框时，我们需要用到JOptionPage类中的showMessageDialog方法，而JOptionPage类又被放置在javax.swing包中，则我们在程序的一开始就需要: import javax.swing.JOptionPage;  然后在程序主体中我们就只需要JOptionPage.showMessageDialog(null,
 "Welcome to Java!"); 不需要写出javax.swing.JOptionPage全称；
- +号在Java中有两个用途：1）作为算术运算符，当左右两边都是数字时；2）字符串的连接，当两边都是字符串时，+号把两个字符串连接，当一遍是字符串，一遍是数值时，就把数值自动转化成字符，然后再和另外一个字符串连接。
- println在显示字符串之后，光标会移动到下一行；print则不会移动到下一行；
- 赋值语句中，等号左边变量的数据类型必须要与右边值的数据类型兼容，例如：int x=1.0; 就是非法的，在不实用类型转换的情况下，不能把double赋值给int变量。这个时候需要数据类型转换；
- 对于pi这样的恒定值，我们可以声明一个常量，即：final double PI = 3.1415926;
- Java中涉及浮点数（float和double）的计算都是近似的，因为这些数没有准确的精度来存储。不过，整数可以（int, short, long, byte）；
- Java中的类型转换。一个数值赋值给更大范围类型的变量的时候，java可以自动转换。反之，则需要使用类型转换。类型转换的格式为：double d=4.5; int i=(int) d; 也就是转换的类型在数值的前面；
- 输出数值需要保留几位小数的时候，可以使用: (int) (result*100)/100.0的形式，这样就可以显示小数点后2位了；
- 如果程序中使用到Math类中的一些表示数学运算的方法时候，我们不需要事先在程序的开头import，这是因为Math类在java.lang包中，而java.lang包中所有的类都被隐式导入的，不需要用户额外编写代码；ps: java.lang包中包含了许多经常用到的类，例如: Math, Package, Boolean, Double, Short, String, Void等；
- 变量定义前面有final，如final double a=1.0; 那么a就是一个常量；




