# Java 读取 Properties 文件 获取参数值 - z69183787的专栏 - CSDN博客
2014年04月02日 06:51:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3957
```java
package com.wonders.schedule.util;
import java.io.InputStream;
import java.util.Properties;
public class PropertyUtil {
	private static Properties p = null;
	public synchronized static void initP(String propertyName) throws Exception {
		if (p == null) {
			p = new Properties();
			InputStream inputstream = Properties.class.getClassLoader()
					.getResourceAsStream(propertyName);//<span style="color: rgb(0, 0, 255); font-family: Arial, sans-serif, Helvetica, Tahoma; font-size: 14px; line-height: 25px;">abc.properties</span>
			if (inputstream == null) {
				throw new Exception("inputstream " + propertyName
						+ " open null");
			}
			p.load(inputstream);
			inputstream.close();
			inputstream = null;
		}
	}
	public static String getValueByKey(String propertyName, String key) {
		String result = "";
		try {
			initP(propertyName);
			result = p.getProperty(key);
			return result;
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}
	}
	public static void main(String[] s) {
		// System.out.println(PWSProperties.getValueByKey("pws.properties","ws_split_chars"));
	}
}
```
