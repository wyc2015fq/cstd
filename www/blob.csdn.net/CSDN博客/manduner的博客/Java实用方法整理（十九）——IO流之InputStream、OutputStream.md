
# Java实用方法整理（十九）——IO流之InputStream、OutputStream - manduner的博客 - CSDN博客


2018年10月30日 22:25:27[Manduner_TJU](https://me.csdn.net/manduner)阅读数：68


# 1，IO流概述
## （1）描述
IO流是java程序用来进行设备间的数据传输问题
## （2）IO流分类
**1）字节流：**当用windows记事本打开待传输文件时，是乱码形式，此时用字节流进行文件传输。当然，如果你不知道文件内容到底是啥，就完全可以用字节流传输文件，它是万能的
InputStream：读取数据（例如：把磁盘文件读取到控制台）
OutputStream：写出数据（例如：把数据写到磁盘）
**2）字符流：**当用windows记事本打开待传输文件时，可以读懂的情况下，可用字符流进行传输且最好用字符流进行数据传输
Reader：读取数据
Writer：写出数据
# 2，字节流
## （1）字节输出流：FileOutputStream案例+源码
**1）案例一：把数据写到磁盘**
```python
import java.io.FileOutputStream;
import java.io.IOException;
/**
* @author Manduner_TJU
* @version 创建时间：2018年10月30日下午8:56:34
*/
public class TestOutputStream {
	public static void main(String[] args){
		//1,创建输出流对象
		FileOutputStream fos=null;
		try {
			//1,创建输出流对象
			fos = new FileOutputStream("test.txt");
			//2,写数据，数据格式必须是字节的格式
			fos.write("我爱Java".getBytes());
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			//3,释放资源。原因：1）把流对象变成垃圾，以使垃圾回收器回收；2）通知系统释放和该操作相关的资源
			if(fos != null) {
				//为了保证close()一定会执行，需在此处写上close()
				try {
					fos.close();
				} catch (IOException e) {
					e.printStackTrace();
				}	
			}
		}
	}
}
```
**2）案例二：实现数据的换行，以及追加写入**
```python
import java.io.FileOutputStream;
import java.io.IOException;
/**
* @author Manduner_TJU
* @version 创建时间：2018年10月30日下午9:17:30
*/
public class TestOutputStream2 {
	public static void main(String[] args) throws IOException {
		//1,创建写出流对象，并且通过“true”实现数据的追加
		FileOutputStream fos = new FileOutputStream("test.txt",true);
		//2，写数据，并实现数据换行
		for(int i = 0; i < 10; i++) {
			fos.write(("我爱java"+i).getBytes());
			//实现换行
			//windows系统换行符:"\r\n"; linux系统换行符:"\n"; Mac系统换行符:"\r"
			fos.write("\r\n".getBytes());
		}
		//3,释放资源
		fos.close();
		
	}
}
```
## （2）字节输入流：FileInputStream案例
**1）案例一：把磁盘数据读出到控制台**
```python
import java.io.FileInputStream;
import java.io.IOException;
/**
* @author Manduner_TJU
* @version 创建时间：2018年10月30日下午9:54:28
*/
public class TestInputStream {
	public static void main(String[] args) throws IOException {
		//1,创建输出流对象(注：该案例中的test.txt文档中只能是英文才能读取成功)
		FileInputStream fis = new FileInputStream("test.txt");
		//2，读书数据到控制台，-1表示文件末尾
		int tmp = 0;
		while((tmp = fis.read())!= -1) {
			System.out.print((char)tmp);
		}
		//3,释放资源
		fis.close();
	}
}
```
**2）案例二：把源文件数据复制到目的文件（即，简单的文件copy功能），是一个字节一个字节的读数据，一个字节一个字节的写数据**
```python
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
/**
* @author Manduner_TJU
* @version 创建时间：2018年10月30日下午10:16:00
*/
public class TestInputStream2 {
	public static void main(String[] args) throws IOException {
		//1,创建读出流对象，写出流对象
		FileInputStream fis = new FileInputStream("test.txt");
		FileOutputStream fos = new FileOutputStream("e:\\testNew.txt");
		//2,从源文件读取数据
		int tmp = 0;
		while((tmp = fis.read())!= -1) {
			fos.write(tmp);
		}
		//3,释放资源（释放的顺序无关紧要）
		fis.close();
		fos.close();
	}
}
```
**3）案例三：文件复制功能。与案例二不同是，该案例是通过字节数组传输数据，效率高**
```python
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
* @author Manduner_TJU
* @version 创建时间：2018年11月1日下午10:08:02
*/
//上传下载功能，首先要搞清楚 数据源和目的地
public class TestInputStream4 {
	public static void main(String[] args) throws IOException {
		//1,封装数据源
		FileInputStream fis = new FileInputStream("e:\\test.mp4");
		FileOutputStream fos = new FileOutputStream("g:\\testNew.mp4");
		//2,文件复制，通过一个字节数组一个字节数组的读取
		byte[] bys = new byte[256];
		int len = 0;
		//==============复制工作开始时间================//
		long timeStart = System.currentTimeMillis();
		//System.out.println(timeStart);
		//==============复制工作开始时间================//
		while((len = fis.read(bys)) != -1) {
			fos.write(bys,0,len);
		}
		//3,释放资源
		fis.close();
		fos.close();
		//==============复制工作结束时间================//
		long timeEnd = System.currentTimeMillis();
		//System.out.println(timeEnd);
		long time = timeEnd - timeStart;
		float allTime = (float)time/1000;
		//==============复制工作结束时间================//
		System.out.println("文件上传/下载完毕,共用时"+String.format("%.2f", allTime)+"s");//0.77s
		
	}
}
```


