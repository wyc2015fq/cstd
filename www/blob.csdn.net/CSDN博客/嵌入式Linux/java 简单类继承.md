
# java 简单类继承 - 嵌入式Linux - CSDN博客

2013年08月31日 20:46:54[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：687标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java																](https://blog.csdn.net/weiqifa0/article/category/1606957)



```python
class Person
{
	String name;
	int age;
	public String talk()
	{
		return "我是："+this.name+",今年："+this.age+"岁";
	}
	public Person()/* 无参数的构造函数*/
	{
		System.out.println("1.public Person(){}");
	}
}
class Student extends Person/*继承父类结构体*/
{
	String school;
	public Student()/*无参数的构造函数*/
	{
		System.out.println("2.public Student(){}");
	}
	public Student(String name,int age,String school)/*有参数的构造函数*/
	{
		super.name=name;/*用super来调用父类的变量*/
		super.age=age;/*用super来调用父类的变量*/
		System.out.print(super.talk());
		this.school=school;
	}
}
public class TestPerson 
{
	public static void main(String[] args) 
	{
		Student s= new Student("韦启发",45,"广西师范大学");
		System.out.println("，学校；"+s.school);
//		s.name = "为启发";
//		s.age=45;
//		s.school="广西师范大学";
//		System.out.println("姓名："+s.name+"，年龄："+s.age+",学校："+s.school);
//		System.out.println("Hello World!");
	}
}
```
![](https://img-blog.csdn.net/20130831204626640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpcWlmYTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

