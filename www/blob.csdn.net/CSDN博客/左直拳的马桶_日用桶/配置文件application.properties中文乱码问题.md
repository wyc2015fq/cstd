# 配置文件application.properties中文乱码问题 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月11日 18:15:01[左直拳](https://me.csdn.net/leftfist)阅读数：114
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
application.properties有个配置项，值含中文，就不行了。
网上绝大多数给出的方案，在idea里面，都是说调整设置，setting->editor->File Encodings之类，其实没有什么卵用，对于我这个个案来说。任凭我设来设去，读到的配置信息，仍然是乱码。我的配置文件，其实已经是UTF-8了。
后来还是改代码了事，读取配置文件的时候，指定编码。
原代码：
```java
InputStream is = getInputStream(path);
if (is != null) {
	Properties pro = new Properties();
	try {
		System.err.println("加载配置文件...");
		pro.load(is);
		System.err.println("加载配置文件完毕");
	} catch (IOException e) {
		System.err.println("加载配置文件失败");
	}
}
```
修正过的代码：
```java
InputStream is = getInputStream(path);
if (is != null) {
	Properties pro = new Properties();
	try {
		System.err.println("加载配置文件...");
		BufferedReader bf = new BufferedReader(new InputStreamReader(is, "UTF-8"));
		pro.load(bf);
		System.err.println("加载配置文件完毕");
	} catch (IOException e) {
		System.err.println("加载配置文件失败");
	}
}
```
我在配置文件里，设置的这个含中文字符的键值，是一个路径，一个外部应用程序的路径。它装在中文路径里。看起来，不够明智，纯英文比较不容易出麻烦一点。有些鬼东西，就算你配置文件可以读出中文，但它本身真的不支持中文，如nginx，放在中文路径里，是跑不起来的。
这种非英文的问题，从我开始认识计算机以来就存在，20年过去了，看上去并没有什么进展。
参考文章：
[读取Properties文件以及中文乱码问题](https://www.cnblogs.com/acm-bingzi/p/javaProperties.html)
