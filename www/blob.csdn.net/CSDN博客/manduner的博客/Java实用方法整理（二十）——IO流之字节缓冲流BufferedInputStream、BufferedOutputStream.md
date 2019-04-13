
# Java实用方法整理（二十）——IO流之字节缓冲流BufferedInputStream、BufferedOutputStream - manduner的博客 - CSDN博客


2018年11月02日 19:37:35[Manduner_TJU](https://me.csdn.net/manduner)阅读数：55


# 1，描述
BufferedInputStream、BufferedOutputStream是Java为高效传输IO流而设计的一种缓冲区类，使用方法和InputStream、OutputStream类似。下面用一个简单的文件复制的案例（同[Java实用方法整理（十九）](https://blog.csdn.net/manduner/article/details/83548311)中的案例三）来说明它们的用法。
# 2，案例+源码
**需求：将源文件复制到目的地文件中（****大家可以直接测试下面的这段程序，效率真的很高0.0****）**
```python
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
/**
* @author Manduner_TJU
* @version 创建时间：2018年11月2日下午6:50:39
*/
public class TestBufferIO {
	public static void main(String[] args) throws IOException {
		//1,封装数据源
		BufferedInputStream bis = new BufferedInputStream(
				new FileInputStream("e:\\test.mp4"));
		BufferedOutputStream bos = new BufferedOutputStream(
				new FileOutputStream("g:\\testNew.mp4"));
		//==========开始时间============//
		long timeStart = System.currentTimeMillis();
		
		//2,复制文件
		byte[] bys = new byte[256];
		int len = 0;
		while((len = bis.read(bys))!=-1) {
			bos.write(bys,0,len);
		}
		//3,释放资源
		bis.close();
		bos.close();
		//========结束时间==============//
		long timeEnd = System.currentTimeMillis();
		float time = (float)(timeEnd-timeStart)/1000;
		
		System.out.println("文件复制完毕，共用时"+
				String.format("%.2f", time)+"s");//0.07s
	}
}
```


