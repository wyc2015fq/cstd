# Java中级第一课：进制 - PeterBishop - CSDN博客





2019年01月19日 21:03:35[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：34
个人分类：[Java中级](https://blog.csdn.net/qq_40061421/article/category/8626230)









  欢迎各位正式进入Java中级。



  第一节课先来一节数学课，这节课学进制。



  什么是进制？

进制也就是进位计数制，是人为定义的带进位的计数方法（有不带进位的计数方法，比如原始的结绳计数法，唱票时常用的“正”字计数法，以及类似的tally mark计数）。 对于任何一种进制---X进制，就表示每一位置上的数运算时都是逢X进一位。 十进制是逢十进一，[十六进制](https://baike.baidu.com/item/%E5%8D%81%E5%85%AD%E8%BF%9B%E5%88%B6/4162457)是逢十六进一，[二进制](https://baike.baidu.com/item/%E4%BA%8C%E8%BF%9B%E5%88%B6/361457)就是逢二进一，以此类推，x进制就是逢x进位。



简单的说，进制就是数的一种表现形式和运算规则



  总所周知，计算机里的数据都是以二进制显示的，在计算机里，一个位(bit)储存一个0或1，一个字节(byte)由8位组成，一个整形变量占4个字节(c语言里是这样，java估计也是)。



  接下来说进制的转换: n进制与10进制的互相换算规则


- n进制转10进制

现有一个n进制数, a1a2...an,则这个n进制数转为10进制数的公式为

dec = an * n^0 + a(n-1)*n^1 + a(n-2) * n^2 + ... + a2 * n^(n-2) + a1*n^(n-1)



举例: 3进制数 210转为10进制为:

0*3^0 + 1 * 3^1 + 2*3^2 = 0 + 3 + 18 = 21


- 10进制转n进制

10进制转n进制用的辗转相除法

现有10进制数an，转为n进制数的方法为:



String n = “”;

while(an > 0)

{

n += an%n;

an /= n;

}

//倒序

n = n.reverse();

最后求得的n就是n进制的结果

查了下，可以用base表示进制



Demo:

```
import java.util.Scanner;

import java.util.Stack;



//进制教学

public class BaseTeach {



    public static void main(String[] args) {

        int an,baseN;

        Scanner scanner = new Scanner(System.in);

        an = scanner.nextInt();

        baseN = scanner.nextInt();

        convertDecimalToBaseN(an,baseN);

    }

    



    /**

     * 把10进制转换成n进制

     */

    public static void convertDecimalToBaseN(int an, int baseN)

    {

        String n = "";

        while (an > 0)

        {

            n += an % baseN;

            an /= baseN;

        }

        n = reverse2(n);

        System.out.println(n);

    }



    private static String reverse(String n) {

        char[] chars = n.toCharArray();

        //栈是一种后进先出的数据结构

        //没学过的话我就换种写法

        Stack<Character> stack = new Stack<>();

        for (char aChar : chars) {

            stack.push(aChar);

        }

        String str = "";

        while (!stack.isEmpty())

        {

            str += stack.pop();

        }

        return str;

    }



    private static String reverse2(String n) {

        char[] chars = n.toCharArray();



        String str = "";

        for (int i = chars.length - 1; i >= 0; i--)

        {

            str += chars[i];

        }

        return str;

    }

}
```





