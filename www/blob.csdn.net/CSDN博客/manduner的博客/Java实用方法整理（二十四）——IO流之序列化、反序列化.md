
# Java实用方法整理（二十四）——IO流之序列化、反序列化 - manduner的博客 - CSDN博客


2018年12月27日 21:46:01[Manduner_TJU](https://me.csdn.net/manduner)阅读数：40标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[序列化																](https://so.csdn.net/so/search/s.do?q=序列化&t=blog)[反序列化																](https://so.csdn.net/so/search/s.do?q=反序列化&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=序列化&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)
[
																								](https://so.csdn.net/so/search/s.do?q=java&t=blog)


# 1，序列化、反序列化作用
序列化：通过ObjectOutputSteam将对象以流的方式写到文件，或者用于网络传输。
反序列化：序列化的逆过程，通过ObjectInputStream还原对象
# 2，注意事项
所有需要序列化的对象，需要实现Serializable接口
一定要给待序列化的对象分配一个固定的序列化ID，否则，修改序列化对象时，会导致反序列化时报错
以上两点注意事项如下图标注所示：
![](https://img-blog.csdnimg.cn/20181227213703363.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
# 3，吐槽
如果大家熟悉python的话，就会觉得java序列化与反序列化的操作真是好麻烦，而python只要一个pickle包，和两个函数就行了：pickle.dump()，序列化对象；pickle.load(),  反序列化。 所以说，数据处理上，还是python更加高级和人性化一些。
# 4，案例+源码
**需求：序列化自定义的Teacher对象**
Teacher对象
```python
package people;
import java.io.Serializable;
/**
* @author Manduner_TJU
* @version 创建时间：2018年12月26日下午4:29:24
*/
public class Teacher implements Serializable{
	//为该对象设置固定的序列化ID，防止该对象有改动时导致反序列化出错。
	private static final long serialVersionUID = 7876932911036212283L;
	
	private String name;
	private int age;
	public Teacher() {
		super();
		// TODO Auto-generated constructor stub
	}
	public Teacher(String name, int age) {
		super();
		this.name = name;
		this.age = age;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	@Override
	public String toString() {
		return "Teacher [name=" + name + ", age=" + age + "]";
	}
	
}
```
序列化、反序列化操作
```python
package testCollection;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;
import people.Teacher;
/**
* @author Manduner_TJU
* @version 创建时间：2018年12月26日下午3:36:51
*/
public class ObjectStreamDemo {
	public static void main(String[] args) throws IOException, ClassNotFoundException{
		//1,对象序列化
		write();
		//2,对象反序列化
		read();
	}
	private static void write() throws IOException{
		//1,创建序列化对象
		ObjectOutputStream oos = new ObjectOutputStream(
				new FileOutputStream("data\\oos.txt"));
		
		//2, 创建待序列化的对象
		Teacher t = new Teacher("Manduner",22);
		
		//3, 将对象序列化并写入文本文件
		oos.writeObject(t);
		
		//4, 释放资源
		oos.close();
	}
	
	
	private static void read() throws IOException, ClassNotFoundException {
		//1,创建反序列化对象
		ObjectInputStream ois = new ObjectInputStream(
				new FileInputStream("data\\oos.txt"));
		
		//2,将对象反序列化(反序列时，最好用Object接收，因为所有对象的爹都是Object)
		Object obj = ois.readObject();
		
		//3,查看反序列化对象
		System.out.println(obj);
	}	
	
}
```


