# Java中各类型所占字节及值的范围 - z69183787的专栏 - CSDN博客
2012年11月14日 21:42:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1285
```java
public class Test {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		 	byte b=-128;  //-2 7  到 2 7-1  1个字节
	        short s=1024*32-1;  //-2 15 到 2 15 -1  2个字节
	        int  i=1024*1024*1024*2-1;    //-2 31 到 2 31-1  4个字节        
	        long l=1024*1024*1024*2;  
	          
	        float f=(float) 3.14;   //4个字节
	        double d=3.14159;  	//8个字节
	          
	        boolean bl=true;          
	          
	        System.out.println("byte b="+b);  
	        System.out.println("short s="+s);  
	        System.out.println("int i="+i);  
	        System.out.println("long l="+l);  
	        System.out.println("float f="+f);  
	        System.out.println("double d="+d);  
	        System.out.println("boolean bl="+bl);  
	        
	        System.out.println("---------------------------");
	        System.out.println(b+4+"");  
	}
}
```
byte:              1Byte
char,short:     2Byte
int,float:          4Byte
long,double:  8Byte
关于 boolean 网上看到如下解释，也有看到说是  1bit（不是byte，即8分之1个byte） 1位空间今天面试问到了这个问题,java中boolean类型到底占多少字节呢,到网上搜了下,最后采用了这个答案：
答：我的结论是：
1）boolean a=true;//这个a在JVM中占4个字节即：32位。
2)boolean[] b = new boolean[10];//数组时，每一个boolean在JVM中占一个字节。
理由：
1）JAVA规范中没有定义boolean类型的大小。
2）但是：在JVM规范第2版中讲得十分清楚。我上边的结论就是从它当中取出来的。
根据：（JVM规范第2版 3.3.4节）
Instead, expressions in the Java programming language that operate on boolean values are compiled to use values of the Java virtual machine int data type.  
Where Java programming language boolean values are mapped by compilers to values of Java virtual machine type int, the compilers must use the same encoding. 而：Java virtual machine type int, whose values are 32-bit signed two's-complement integers。
Arrays of type boolean are accessed and modified using the byte array instructions  
In Sun's JDK releases 1.0 and 1.1, and the Java 2 SDK, Standard Edition, v1.2, boolean arrays in the Java programming language are encoded as Java virtual machine byte arrays, using 8 bits per boolean element.另外关于单位的换算：
1Byte = 8 bit
1K＝1024Byte
1M＝1024K
1G=1024M
1T=1000G
