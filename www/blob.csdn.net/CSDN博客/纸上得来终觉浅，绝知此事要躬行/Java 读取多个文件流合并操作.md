# Java 读取多个文件流合并操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年02月04日 11:47:53[boonya](https://me.csdn.net/boonya)阅读数：6272








**       在Java IO编程中还有很多值得我们学习的地方，很多时候我们都不太注重基础知识的积累；工作越久越感觉没有新意，说明我们错误地高估了自己，如果给你一个没有网络的办公环境，你怎么正常开展自己的工作，这是一个值得思考的问题，我们对网络太过于依赖，以至于懒得去记忆。下面要说的是SequenceInputStream的使用。春节回家，在这里做一个标注以便节后继续研究。**

**给大家推荐一个Java IO编程的阅读地址**：[http://tutorials.jenkov.com/java-io/index.html](http://tutorials.jenkov.com/java-io/index.html)




```java
package com.boonya.io;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.SequenceInputStream;
import java.util.Vector;

public class IOSequenceInputStream {
	
	public static void main(String[] args) {
		IOSequenceInputStream io=new IOSequenceInputStream();
		//io.sample();
		io.advance();
	}

	public void sample() {
		InputStream input1;
		InputStream input2;
		SequenceInputStream sequenceInputStream = null;
		try {
			input1 = new FileInputStream("c:\\data\\file1.txt");
			input2 = new FileInputStream("c:\\data\\file2.txt");

			sequenceInputStream = new SequenceInputStream(input1, input2);

			int data = sequenceInputStream.read();
			while (data != -1) {
				System.out.println((char)data);
				data = sequenceInputStream.read();
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO: handle exception
		} finally {
			if (sequenceInputStream != null) {
				try {
					sequenceInputStream.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}

	}
	
	public void advance() {
		SequenceInputStream sequenceInputStream=null;
		try {
			InputStream input1 = new FileInputStream("c:\\data\\file1.txt");
			InputStream input2 = new FileInputStream("c:\\data\\file2.txt");

			Vector<InputStream> streams = new Vector<InputStream>();
			streams.add(input1);
			streams.add(input2);
			
			sequenceInputStream = new SequenceInputStream(streams.elements());
			
			int data= sequenceInputStream.read();
			
			while(data != -1){
			    System.out.println((char)data);
			    data = sequenceInputStream.read();
			}
			sequenceInputStream.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			if (sequenceInputStream != null) {
				try {
					sequenceInputStream.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
	}

}
```




[](http://tutorials.jenkov.com/java-io/index.html)






