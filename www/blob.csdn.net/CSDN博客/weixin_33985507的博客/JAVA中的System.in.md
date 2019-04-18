# JAVA中的System.in - weixin_33985507的博客 - CSDN博客
2014年03月11日 14:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
　　System.in读取标准输入设备数据（从标准输入获取数据，一般是键盘），其数据类型为InputStream。方法：
int read()   // 返回输入数值的ASCII码，，该值为0到 255范围内的int字节值。若返回值为-1，说明没有读取到任何字节读取工作结束。
int read(byte[] b)  // 读入多个字节到缓冲区b中，返回值是读入的字节数
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package InPackage;
/**
 * System.in.read()返回值为输入数值的ASCII码，该值为0到 255范围内的int字节值
 * 如果因为已经到达流末尾而没有可用的字节，则返回值 -1。
 */
public class Intest1 {
    public static void main(String args[]) throws java.io.IOException
      {
       int a=0;
       System.out.println("请输入a：");
       a=System.in.read();
       System.out.println("a="+a);
       System.out.println("(char)a="+(char)a);
      }
    /**
     * 假设我们输入a为1
     * 输出结果为：
     * 请输入a：
     * 1
     * a=49
     * (char)a=1
     */
    
}
View Code
```
有一个有意思的问题是：当我们输入一个字符，System.in.read()会读取几个字符呢？
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package InPackage;
import java.util.Arrays;
/**
 * 当我们输入一个字符，System.in.read()会读取几个字符
 * 我们从运行结果可以看出是三个
 * 假设我们输入一个字符，那么它会接着读取该字符后面的/r和/n
 */
public class Intest2 {
     public static void main(String[] args) throws Exception {  
         int[] x = new int[6];  
         Arrays.fill(x, 5);  //Arrays.fill(int[] a,int b)方法用于给数组中的每个元素赋值
         for (int i = 0; i < x.length; i++) {  
             System.in.read();  
             System.out.println(x[i]);  
         }  
     }
     /**
      * 假设我们输入值分别为1,2
      * 输出结果：
      * 1
      * 5
      * 5
      * 5
      * 2
      * 5
      * 5
      * 5
      */
}
View Code
```
　System.in.read()每次只是读取一个字符，但它多读取的是哪几个字符呢？
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package InPackage;
import java.io.IOException;
/**
 * System.in.read()每次只是读取一个字符
 * 按下回车键代表了两个字符\r\n,\r的ASCII码值是10，\n是13。另外，1对应的ASCII是49
 */
public class Intest3 {
    public static void main(String args[]) throws IOException {
        for (int j = 0; j < 5; j++) {
            System.out.println("请输入:");
            char c = 0;
            c = (char) System.in.read();
            if (c == '1') {
                System.out.println("OK!");
            } else {
                System.out.println((int) c);
            }
        }
    }
}
View Code
```
对于上面的程序，我们首先输入的是w1，结果如下图所示：
![](https://images0.cnblogs.com/i/459322/201403/111404248798604.png)
　　可以看出程序还没有执行完，阻塞于最后一个“请输入：”，此时我们再次输入1，程序执行完成，结果如下图所示：
![](https://images0.cnblogs.com/i/459322/201403/111406256337314.png)
　如何让System..in.read()读入一行数据呢？
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package InPackage;
import java.io.IOException;
public class Intest4 {
    public static void main(String args[]) {
        int b;
        try {
            System.out.println("请输入：");
            while ((b = System.in.read()) != -1) {
                System.out.print((char) b);
            }
        } catch (IOException e) {
            System.out.println(e.toString());
        }
    }
    /**
     * 输出结果：
     * 请输入：
     * test
     * test
     */
}
View Code
```
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package InPackage;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.InputStreamReader;
/**
 * 通常情况下，你会用readLine( )一行一行地读取输入，
 * 因此要把System.in包装成BufferedReader。但在这之前还得先用InputSteamReader把System.in转换成Reader。
 * BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
 * in.readLine()返回值为String类型
 *
 */
public class Intest5 {
    public static void main(String args[]) throws java.io.IOException {
        System.out.println("请输入整数：");
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        //或者这么写也可以：DataInputStream reader = new DataInputStream(System.in);
        int a = Integer.parseInt(reader.readLine()); // 这样得到的是String类型的，需要转换为需要的类型
        System.out.println("a=" + a);
        int sum = 0;
        for (int i = 0; i <= a; i++)
            sum += i;
        System.out.println(sum);
    }
    /**
     * 假设我们输入a为100
     * 输出结果为：
     * 100
     * a=100
     * 5050
     */
}
View Code
```
　　public int read(byte[] b) throws IOException又是怎么使用的呢？
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package InPackage;
/**
 * public int read(byte[] b) throws IOException 
 * 从输入流中读取一定数量的字节，并将其存储在缓冲区数组 b中。
 * 返回值为：以整数形式返回实际读取的字节数。 
 * 如果 b的长度为0，则不读取任何字节并返回 0； 否则，尝试读取至少一个字节。 
 * 如果因为流位于文件末尾而没有可用的字节，则返回值 -1；否则，至少读取一个字节并将其存储在b中。
 * 
 */
public class Intest6 {
    public static void main(String args[]) throws Exception {
        byte[] barray = new byte[5];
        System.out.println("请输入：");
        System.in.read(barray);
        for (int i = 0; i < barray.length; i++) {
            System.out.println((char) barray[i]);
        }
    }
}
View Code
```
参考资料：
http://uule.iteye.com/blog/1128243
http://blog.sina.com.cn/s/blog_78edf5db0100xr7a.html
