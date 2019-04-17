# Java实现URL下载图片到本地 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年02月23日 15:43:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4790








功能：输入图片URL地址和图片名字，输出下载图片到指定目录。参考代码如下：



```java

```
import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;

public static void downloadImage(String Imageurl,String filename) throws IOException{
		System.getProperties().setProperty("http.proxyHost", "IP");//设置代理
		System.getProperties().setProperty("http.proxyPort", "Port");
		URL url = new URL(Imageurl);
		//打开网络输入流
		DataInputStream dis = new DataInputStream(url.openStream());
		String newImageName="D://tmp//"+filename+".jpg";
		//建立一个新的文件
		FileOutputStream fos = new FileOutputStream(new File(newImageName));
		byte[] buffer = new byte[1024];
		int length;
		//开始填充数据
		while((length = dis.read(buffer))>0){
			fos.write(buffer,0,length);
		}
		dis.close();
		fos.close();	
	}




