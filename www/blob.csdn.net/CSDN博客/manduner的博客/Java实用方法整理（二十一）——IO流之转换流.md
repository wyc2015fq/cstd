
# Java实用方法整理（二十一）——IO流之转换流 - manduner的博客 - CSDN博客


2018年11月07日 17:43:35[Manduner_TJU](https://me.csdn.net/manduner)阅读数：166


# 1，描述
字符流的读取和写出有以下两种方式
FileReader、FileWriter
InputStreamReader、OutputStreamWriter
**两种方式的区别是：**FileReader、FileWriter使用默认的字符编码和字节缓冲区进行字符流传输，是 InputStreamReader、      OutputStreamWriter的简化用法
InputStreamReader、OutputStreamWriter既可以使用默认的字符编码和字节缓冲区大小进行字符流的传输，也可以指定字符编码和字节缓冲区进行字符流的传输。

# 2，案例+源码
（1）需求：将e:\test.json复制到g:\\testNew.json
（2）源码中展示了4个案例，分别为：
InputStreamReader、OutputStreamWriter通过字符数组传递数据流
InputStreamReader、OutputStreamWriter通过字符传递数据流
FileReader,FileWriter通过字符数组传递数据流
FileReader,FileWriter通过字符传递数据流
```python
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
/**
* @author Manduner_TJU
* @version 创建时间：2018年11月2日下午10:13:01
*/
public class TestIO {
	public static void main(String[] args) throws IOException{
		//copyChar("e:\\test.json","g:\\testNew.json");
		//copyChar11("e:\\test.json","g:\\testNew.json");
		//copyChar2("e:\\test.json","g:\\testNew.json");
		copyChar22("e:\\test.json","g:\\testNew.json");
		System.out.println("文件复制完毕");
	}
	
	//InputStreamReader、OutputStreamWriter通过字符数组传递数据流
	public static void copyChar(String source, String des) throws IOException{
		//1,封装数据源
		InputStreamReader isr = new InputStreamReader(
				new FileInputStream(source),"gbk");
		OutputStreamWriter osw = new OutputStreamWriter(
				new FileOutputStream(des),"gbk");
		//2,复制文件
		char[] ch = new char[1024];
		int len = 0;
		while((len = isr.read(ch)) !=-1) {
			osw.write(ch,0,len);
		}
		//3,释放资源
		isr.close();
		osw.close();
	}
	
	//InputStreamReader、OutputStreamWriter通过字符传递数据流
	public static void copyChar11(String source, String des) throws IOException{
		//1,封装数据源
		InputStreamReader isr = new InputStreamReader(new FileInputStream(source));
		OutputStreamWriter osw = new OutputStreamWriter(new FileOutputStream(des));
		
		//3,复制文件
		int ch = 0;
		while((ch=isr.read())!=-1) {
			osw.write(ch);
		}
		
		//2,释放资源
		isr.close();
		osw.close();
		
	}
	//FileReader,FileWriter通过字符数组传递数据流
	public static void copyChar2(String source, String des) throws IOException{
		//1,封装数据源
		FileReader fr = new FileReader(source);
		FileWriter fw = new FileWriter(des);
		
		//2,复制文件
		char[] ch = new char[1024];
		int len = 0;
		while((len = fr.read(ch)) != -1) {
			fw.write(ch,0,len);
		}
		
		//3,释放资源
		fr.close();
		fw.close();
	}
	//FileReader,FileWriter通过字符传递数据流
	public static void copyChar22(String source, String des) throws IOException{
		//1,封装资源
		FileReader fr = new FileReader(source);
		FileWriter fw = new FileWriter(des);
		
		//2,复制文件
		int ch = 0;
		while((ch=fr.read())!=-1) {
			fw.write(ch);
		}
		
		//3,释放资源
		fr.close();
		fw.close();
	}
}
```


