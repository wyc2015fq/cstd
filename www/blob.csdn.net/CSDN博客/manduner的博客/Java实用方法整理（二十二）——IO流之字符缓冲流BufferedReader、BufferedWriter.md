
# Java实用方法整理（二十二）——IO流之字符缓冲流BufferedReader、BufferedWriter - manduner的博客 - CSDN博客


2018年11月07日 20:13:49[Manduner_TJU](https://me.csdn.net/manduner)阅读数：50


# 1，描述
BufferedReader、BufferedWriter是java为了高效传输IO字符流而设计的一种缓冲类，用法和FileWriter，FileReader类似。BufferedReader、BufferedWriter只是对FileWriter，FileReader进行了装饰。
# 2，特殊功能
（1）BufferedReader的readLine()方法，每次读取文件的一行数据
（2）BufferedReader的newLine()方法，换行功能
# 3，案例+源码
将源文件复制到目的地文件（代码中的**方法三**最常用）
```python
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
/**
* @author Manduner_TJU
* @version 创建时间：2018年11月7日下午8:02:07
*/
public class TestBuffered {
	public static void main(String[] args) throws IOException{
//		copyFile1("e:\\test.json","g:\\testNew.json");
//		copyFile2("e:\\test.json","g:\\testNew.json");
		copyFile3("e:\\test.json","g:\\testNew.json");
		System.out.println("文件复制完毕");
	}
	
	//方法一：每次一个字符一个字符的读取与写出
	public static void copyFile1(String source, String des) throws IOException{
		//1,封装数据源
		BufferedReader br = new BufferedReader(new FileReader(source));
		BufferedWriter bw = new BufferedWriter(new FileWriter(des));
		
		//2,复制文件
		int ch = 0;
		while((ch = br.read()) != -1) {
			bw.write(ch);
		}
		
		//3,释放资源
		br.close();
		bw.close();
	}
	
	//方法二：每次一个字节一个字节的读取与写出
	public static void copyFile2(String source, String des) throws IOException{
		//1,封装数据源
		BufferedReader br = new BufferedReader(new FileReader(source));
		BufferedWriter bw = new BufferedWriter(new FileWriter(des));
		
		//2,复制文件
		char[] ch = new char[1024];
		int len = 0;
		while((len = br.read(ch)) != -1) {
			bw.write(ch,0,len);
		}
		
		//3,释放资源
		br.close();
		bw.close();
	}
	
	//方法三：每次一行一行数据的读取与写出
	private static void copyFile3(String source, String des) throws IOException {
		//1,封装数据源
		BufferedReader br = new BufferedReader(new FileReader(source));
		BufferedWriter bw = new BufferedWriter(new FileWriter(des));
		
		//2, 复制文件
		String str = null;
		while((str = br.readLine())!= null) {
			bw.write(str);
			bw.newLine();
			bw.flush();
		}
		
		//3,释放资源
		br.close();
		bw.close();
	}
	
}
```


