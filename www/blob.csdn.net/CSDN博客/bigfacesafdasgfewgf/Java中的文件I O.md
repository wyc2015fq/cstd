# Java中的文件I/O - bigfacesafdasgfewgf - CSDN博客





2015年03月12日 09:55:52[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：942标签：[文件IO																[java																[Scanner																[PrintWriter																[异常](https://so.csdn.net/so/search/s.do?q=异常&t=blog)
个人分类：[Java](https://blog.csdn.net/puqutogether/article/category/2595127)







    Java中的文件输入输出格式和C++中不太一样，这篇我们整理一下Java中是如何实现文件I/O的。




 首先，确定文件对象。



```java
java.io.File file = new java.io.File("score.txt");
```

这里最好判断文件是否存在，防止后面读取文件内容的时候出错。



    输入：

1）使用java.io.PrintWriter类型，即java.io.PrintWriter inputFile = new java.io.PrintWriter(file); 

2）往文件中写入String类型，可以使用inputFile.println(str). 一行行的写入。

3）全部写完之后，需要关闭文件才能够生效，即inputFile.close(); 

4）PrintWriter方法，如果文件不存在会创建一个新文件；如果文件已经存在，会舍弃之前的内容，重新读写。

5）出现文件操作的方法，在声明的时候需要加入throws Exception。




    输出：

1）使用Scanner类型，Scanner outputFile= new Scanner(file); 注意，Scanner需要添加import java.util.Scanner; 

2）读取文件内容的时候，需要循环判断outputFile.hasNext()，一直到文件尾端；

3）真正读取文件，我们还需要方法next()，它会读取用分隔符隔开的令牌，默认的分隔符是空格，我们还可以使用useDelimiter(String regex)方法来设置新的分隔符；

4）全部读取完之后，需要关闭文件, outputFile.close()。

5）出现文件操作的方法，在声明的时候需要加入throws Exception。






```java
package testing;
/***
 * 
 * @author Hadoop
 *
 */
import java.util.Scanner;  // 为了后面的Scanner
public class TestingFile {

	/**
	 * MAIN CLASS
	 * @param args
	 */
	

	
	public static void main(String[] args) throws Exception{
		// TODO Auto-generated method stub
		

		java.io.File file = new java.io.File("score.txt"); 
		if(!file.exists()) {
			System.out.println("File not found"); 
//			System.exit(0);
		}
		java.io.PrintWriter output = new java.io.PrintWriter(file);  //  need to throws Exception
		String str = "LiLei 90"; 
		output.println(str); 
		str = "HanMei 100"; 
		output.println(str); 
		output.close();
		
		Scanner input = new Scanner(file); 
		while(input.hasNext()) {
			String name = input.next();  // 以空格为分隔符
			String score = input.next(); 
			
			System.out.println("The score of " + name + " is " + score); 
		}
		input.close();
	}

}
```](https://so.csdn.net/so/search/s.do?q=PrintWriter&t=blog)](https://so.csdn.net/so/search/s.do?q=Scanner&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=文件IO&t=blog)




