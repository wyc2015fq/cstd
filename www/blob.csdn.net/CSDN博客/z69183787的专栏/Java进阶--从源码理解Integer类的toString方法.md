# Java进阶--从源码理解Integer类的toString方法 - z69183787的专栏 - CSDN博客
2018年06月05日 18:06:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：97
个人分类：[算法](https://blog.csdn.net/z69183787/article/category/2176993)
[https://blog.csdn.net/u013309870/article/details/72158054](https://blog.csdn.net/u013309870/article/details/72158054?utm_source=itdadao&utm_medium=referral)
Java中的jdk是深入学习Java编程的宝库，其中各种方法的实现不仅可以深入了解 设计模式的应用，还可以看到jdk编写者如何优化代码（这种优化已经到达了一种极端的程度）对我们编写自己的代码是有十分重要的作用的。read the fucking source code 是成为一名优秀程序猿必须的经历。本博客通过分析Integer类的toString方法来看看jdk编写者如何编写他们的代码的。
## Integer类的toString的基本用法
```
public class IntegerDemo {
    public static void main(String[] args) {
        Integer OUT_MAX_VALUE=new Integer(Integer.MAX_VALUE+1);     
        Integer MAX_VALUE=new Integer(Integer.MAX_VALUE);
        Integer MIN_VALUE=new Integer(Integer.MIN_VALUE);
        Integer NOR_VALUE=new Integer(-128);
        Integer OUT_MIN_VALUE=new Integer(Integer.MIN_VALUE-1);
        System.out.println("max_val :"+MAX_VALUE.toString());
        System.out.println("max_val :"+MAX_VALUE);
        System.out.println("-------------------------");
        System.out.println("out_max :"+OUT_MAX_VALUE.toString());
        System.out.println("out_min :"+OUT_MIN_VALUE.toString());
        System.out.println("-------------------------");        
        System.out.println("min_val :"+MIN_VALUE.toString());
        System.out.println("min_val :"+MIN_VALUE);
        System.out.println("-------------------------");
        System.out.println("nor_val :"+NOR_VALUE.toString());
        System.out.println("nor_val :"+NOR_VALUE);
    }
}
//输出
/*
max_val :2147483647
max_val :2147483647
-------------------------
out_max :-2147483648
out_min :2147483647
-------------------------
min_val :-2147483648
min_val :-2147483648
-------------------------
nor_val :-128
nor_val :-128
*/
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
由上面可知直接输出Integer的值和调用Integer类的toString方法是一样的，其实直接打印一个对象的时候就是调用了该对象的toString方法。调用toString方法的时候其实输出的是Integer的value值，toString方法就是把int类型的value值转化为string类型输出。注意一下几点：
①Integer的value值从 Integer.MIN_VALUE 到 Integer.MAX_VALUE 如果超出了这个范围就会得到一些奇怪的结果。
②在Integer.MAX_VALUE基础上加1输出的结果是个负值。
③在Integer.MIN_VALUE基础上加1输出的结果是个正值。
## Integer类的toString的源码分析
下面就深入看看Integer类的toString方法是如何实现的。还是直接上源码。
```
public String toString() {
        return toString(value);
    }
//------------------------------
 public static String toString(int i) {
        if (i == Integer.MIN_VALUE)
            return "-2147483648";
         //如果是最小值直接返回其字符串因为Integer.MIN_VALUE=-2147483648 ，这样可以节省下面计算时间
         //①
        int size = (i < 0) ? stringSize(-i) + 1 : stringSize(i);
        //获取整数值的长度10进制
        char[] buf = new char[size];
        //②
        getChars(i, size, buf);
        //得到整数中的每一个字符
        //③
        return new String(buf, true);
        //返回字符串值
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
上面的代码做几点说明： 
①如果Integer的value值正好是 Integer.MIN_VALUE 直接返回 “-2147483648” 节省时间。
②得到integer值的十进制的长度，如果负数先求出绝对值的长度，然后再长度加1，因为负数的符号位占一位。
③得到integer的value值的每一个字符。
④得到的字符新建字符串返回。
下面看看 stringSize方法是怎样实现的：
```java
static int stringSize(int x) {
        for (int i=0; ; i++)
            if (x <= sizeTable[i])
                return i+1;
                //看看x到底是几位数
    }
//----------------------------------
final static int [] sizeTable = { 9, 99, 999, 9999, 99999, 999999, 9999999,
                                      99999999, 999999999, Integer.MAX_VALUE };
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
上面的 stringSize 非常的巧妙的来整数的长度，首先定义了一个数组这个数组中分别存放了一位十进制的最大值，二位十进制的最大值，依次到 Integer.MAX_VALUE为止，因为integer的最大值为 Integer.MAX_VALUE，也就是说integer的最大长度为10位。注意： 
①stringSize(int x) 中的参数x 必须是正整数。
下面用一个实例看看stringSize(int x)方法。
```
public class IntegerDemo {
    public static void main(String[] args) {
        int x=199;
        int max=Integer.MAX_VALUE;
        int out_max=Integer.MAX_VALUE+1;
        System.out.println(stringSize(x));
        System.out.println("-------------------------");
        System.out.println(stringSize(max));
        System.out.println("-------------------------");
        System.out.println(stringSize(out_max));        
    }
     final static int [] sizeTable = { 9, 99, 999, 9999, 99999, 999999, 9999999,
         99999999, 999999999, Integer.MAX_VALUE };
     static int stringSize(int x) {
         for (int i=0; ; i++)
             if (x <= sizeTable[i])
                 return i+1;
     }
}
//输出
/*
3
-------------------------
10
-------------------------
1
*/
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
上面的199小于999所以位数为3，而 out_max的值因为超过了 Integer.MAX_VALUE是个负数，得到的长度为1，因此求负数的长度时必须将其转化为正数来求，而且求出的长度还要加上一位来放‘ - ’。得到integer的value值的长度后使用 getChars 方法来得到value的每一位上的字符。下面看看这个getChars方法。
```
static void getChars(int i, int index, char[] buf) {
        int q, r;
        int charPos = index;
        char sign = 0;
        if (i < 0) {
            sign = '-';
            i = -i;
        }
        // 当i >= 65536的时候每一次获取两位的char值。
        while (i >= 65536) {
            q = i / 100;
        // really: r = i - (q * 100);
            r = i - ((q << 6) + (q << 5) + (q << 2));
            //使用移位操作快速计算出q*100，2^6+2^5+2^2=64+32+4=100.
            i = q;
            buf [--charPos] = DigitOnes[r];
            buf [--charPos] = DigitTens[r];
        }
        // 当 i <= 65536的时候每次只获取一位的char值
        // assert(i <= 65536, i);
        for (;;) {
            q = (i * 52429) >>> (16+3);
            //q/10,2^19=524288, (double)52429/(1<<19)=0.10000038146972656
            r = i - ((q << 3) + (q << 1));  // r = i-(q*10) ...
            buf [--charPos] = digits [r];
            i = q;
            if (i == 0) break;
        }
        if (sign != 0) {
            buf [--charPos] = sign;
            //如果是负数加上符号位
        }
    }
//--------------------------------------
final static char [] DigitOnes = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        } ;
//--------------------------------------
final static char [] DigitTens = {
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
        '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
        '2', '2', '2', '2', '2', '2', '2', '2', '2', '2',
        '3', '3', '3', '3', '3', '3', '3', '3', '3', '3',
        '4', '4', '4', '4', '4', '4', '4', '4', '4', '4',
        '5', '5', '5', '5', '5', '5', '5', '5', '5', '5',
        '6', '6', '6', '6', '6', '6', '6', '6', '6', '6',
        '7', '7', '7', '7', '7', '7', '7', '7', '7', '7',
        '8', '8', '8', '8', '8', '8', '8', '8', '8', '8',
        '9', '9', '9', '9', '9', '9', '9', '9', '9', '9',
        } ;
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
getChars方法也是十分的巧妙 
①在 i >= 65536 的时候每次能够求出2位的char 值，求2位的char值时利用了两个数组 DigitOnes 和 DigitTens 就是在这两个数组的帮助下才能一次求出2位的char值下面就简单的说一下怎样利用这两个数组来分别求出个位和十位上的值的，假如现在得到两位数是 65，个位上是 5 要得到个位上的5，这时候不管十位是多少个位上一定是5，所以数组DigitOnes的 05，15，25，35，45，55，65，75，85，95位置上都是 5，这样不管是25，还是35 都能得到个位上的5。在来看看如何得到十位上的数，还是65，十位是6，所以DigitTens 的60，61，62，63，64，……69 位置上都是6。
② q = (i * 52429) >>> (16+3);这段代码其实就是q=i/10 ,其中 (double)52429/(1<<19)=0.10000038146972656也就是在int型的时候计算一个数的十分之1的精度是够的，可以看出jdk开发者的这种优化意识是非常强的。
③(q << 6) + (q << 5) + (q << 2) 这段等价于q*100，因为 q*2^6+q*2^5+q*2^2=q*(64+32+4)=100q.这里为什么不直接使用q*100，而要使用位移操作，因为其实q*100底层还是要进行位移操作的，直接使用位移操作效率更高，这也给我们编程带来启示。能够优化的时候一定要优化。
