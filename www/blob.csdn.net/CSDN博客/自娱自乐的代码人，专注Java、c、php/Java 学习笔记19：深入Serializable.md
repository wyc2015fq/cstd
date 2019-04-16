# Java 学习笔记19：深入Serializable - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月10日 14:49:17[initphp](https://me.csdn.net/initphp)阅读数：1442
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- Java的Serializable


Serialization（序列化）是一种将对象以一连串的字节描述的过程；反序列化deserialization是一种将这些字节重建成一个对象的过程。Java序列化API提供一种处理对象序列化的标准机制。在这里你能学到如何序列化一个对象，什么时候需要序列化以及Java序列化的算法，我们用一个实例来示范序列化以后的字节是如何描述一个对象的信息的。
说白了，Java都是通过对象来描述实体，而对象是不能再网络上传递的，如果将对象生成一种可以解析的"一连串的字节描述"，那么对象的数据就可以在网络上进行传递了。



对象序列化后，会生成一个.out结尾的文件，这个文件用16进制打开就像下面的一样：


```java
AC ED 00 05 73 72 00 0A 53 65 72 69 61 6C 54 65

73 74 A0 0C 34 00 FE B1 DD F9 02 00 02 42 00 05

63 6F 75 6E 74 42 00 07 76 65 72 73 69 6F 6E 78

70 00 64
```


- 一个Java的Serializable实例：

- Java的对象需要序列化，数据结构对象就需要实现Serializable接口，例如以下代码：


```java
/**
 * guest_book 数据结构
 * @author zhuli
 */
public class GuestbookDo implements Serializable{ //implements Serializable是关键
	
	private String title;
	
	private String content;
	
	private String username;
	
	private int id;
	
	
	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public int getDtime() {
		return dtime;
	}

	public void setDtime(int dtime) {
		this.dtime = dtime;
	}

	private int dtime;
}
```


- 我们通过数据流的方式，将GuestbookDo对象进行序列化，写进数据流。并且在web目录下会生成temps.out，就是一个序列化的对象的数据流文件。通过url中的http://xxxx/test/test/访问，就能将对象写入数据流了。


```java
@RequestMapping(value="/test")
	@ResponseBody
	public String test() throws IOException {
		  //对象实例化
	      GuestbookDo gb = new GuestbookDo();
	      gb.setContent("asdasd");
	      gb.setId(100);
	      //数据流
		  FileOutputStream fos = new FileOutputStream("temps.out");  
	      ObjectOutputStream oos = new ObjectOutputStream(fos);  
	      oos.writeObject(gb);  
	      oos.flush();  
	      oos.close();  
		return "test";
	}
```

![](https://img-my.csdn.net/uploads/201212/10/1355123153_5521.jpg)


- 我们可以通过读取temps.out的方法就能读取到这个数据流中的对象结构。


```java
@RequestMapping(value="/testa")
	@ResponseBody
	public String testa() throws Exception {
	      InputStream is=new FileInputStream("temps.out");


	       ObjectInputStream ois=new ObjectInputStream(is);

	     GuestbookDo gb = (GuestbookDo) ois.readObject();
	 
	       System.out.println("id="+gb.getId());  
	       System.out.println("content="+gb.getContent());  
	       ois.close();
		return "test1";
	}
```


结果：

![](https://img-my.csdn.net/uploads/201212/10/1355123230_9020.jpg)



- 序列化算法一般会按步骤：

- 将对象实例相关的类元数据输出。
- 递归地输出类的超类描述直到不再有超类。
- 类元数据完了以后，开始从最顶层的超类开始输出对象实例的实际数据值。
- 从上至下递归输出实例的数据







