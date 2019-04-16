# Java中使用InputStream读入Jar/zip内容 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年10月30日 21:55:07[gloomyfish](https://me.csdn.net/jia20003)阅读数：8629








因为项目需要，要把一个zip/jar文件读入到后台服务器上，前台用户选择好zip/jar文件

以后，upload文件之后，从JarFile对象中循环读取JarEntry对象，然后根据JarFile的

API使用jarFile.getInputStream(JarEntry jarEntry)来读取每个JarEntry的内容。

可是最近他们想把文件读入变成基于inputstream的直接读取，从http请求中获取

inputstream之后可以直接读入。这样问题就来，怎么从流中知道每个JarEntry对象

一番研究之后，有了下面的代码：



```java
package com.gloomyfish.image.study;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;
import java.util.jar.Manifest;

public class JarFileReader {
	private JarInputStream jarInput;
	private HashMap<String, ByteArrayOutputStream> entriesStreamMap;
	public JarFileReader(InputStream in) throws IOException {
		jarInput = new JarInputStream(in);
		entriesStreamMap = new HashMap<String, ByteArrayOutputStream>();
	}
	
	public void readEntries() throws IOException {
		JarEntry entry = jarInput.getNextJarEntry();
		String manifestEntry = null;
		while(entry != null) {
			System.out.println("Entry Name = " + entry.getName());
			if("jp/co/sony/csl/nielsen/phoenix/objectremoval/Mark.class".equals(entry.getName())) {
				manifestEntry = entry.getName();
				copyInputStream(jarInput, entry.getName());		
			}
			entry = jarInput.getNextJarEntry();
		}
		System.out.println("Now!! + get jar entry inputstream again...");
		InputStream inputStream = getCopy(manifestEntry);
		System.out.println(inputStream.read());
	}
	
	public void copyInputStream(InputStream in, String entryName) throws IOException {
		if(!entriesStreamMap.containsKey(entryName)) {
			ByteArrayOutputStream _copy = new ByteArrayOutputStream();
			int read = 0;
			int chunk = 0;
			byte[] data = new byte[256];
			while(-1 != (chunk = in.read(data)))
			{
				read += data.length;
				_copy.write(data, 0, chunk);
			}
			entriesStreamMap.put(entryName, _copy);
		}
	}
	
	public InputStream getCopy(String entryName) {
		ByteArrayOutputStream _copy = entriesStreamMap.get(entryName);
		return (InputStream)new ByteArrayInputStream(_copy.toByteArray());
	}
	
	public static void main(String[] args) {
		File jarFile = new File("D:\\agriculture\\clickremoval.jar");
		try {
			InputStream in = new BufferedInputStream(new FileInputStream(jarFile));
			JarFileReader reader = new JarFileReader(in);
			reader.readEntries();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
}
```



