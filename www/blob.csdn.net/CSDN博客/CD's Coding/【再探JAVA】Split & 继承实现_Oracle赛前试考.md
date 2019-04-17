# 【再探JAVA】Split & 继承实现_Oracle赛前试考 - CD's Coding - CSDN博客





2015年04月11日 11:25:35[糖果天王](https://me.csdn.net/okcd00)阅读数：607标签：[oracle																[java																[继承																[interface																[赛前试考](https://so.csdn.net/so/search/s.do?q=赛前试考&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)





今天Oracle来了一次赛前试考呢~

吾辈的JAVA算是漏洞百出的菜鸟了，也凭着坚毅的意志（和高人@ZoeCur 的指点）完成了试考呢~

选择题看似是问分隔之后有多少份，但实际上是问text.split的作用——

先给个完整源码吧：



```java
package Test01;

public class Test 
{
	public static void main(String[] args)
	{
		String text = "Welcome to Java contest";
		String[] words = text.split("\s");
		System.out.println(words.length);	
	}

}
```
编译时，在“\s"的部分会报错：



Invalid escape sequence (valid ones are  \b  \t  \n  \f  \r  \"  \'  \\ )

关于Split的使用，MSDN上给出了这样的源码（Language：C#）： 【看到了不错的点子，可以用这个去过CodeHunt那道AEIOU的题了】



```
using System;

public class SplitTest {
    public static void Main() {

        string words = "This is a list of words, with: a bit of punctuation" +
                       "\tand a tab character.";

        string [] split = words.Split(new Char [] {' ', ',', '.', ':', '\t' });

        foreach (string s in split) {

            if (s.Trim() != "")
                Console.WriteLine(s);
        }
    }
}
// The example displays the following output to the console:
//       This
//       is
//       a
//       list
//       of
//       words
//       with
//       a
//       bit
//       of
//       punctuation
//       and
//       a
//       tab
//       character
```



返回的数组元素中不包含分隔符字符。

如果此实例不包含 separator 中的任何字符，则返回的数组由包含此实例的单个元素组成。

如果 separator 参数为 null 或不包含任何字符，则采用空白字符作为分隔符。空白字符由
 Unicode 标准定义，如果将它们传递给[Char.IsWhiteSpace](https://msdn.microsoft.com/zh-cn/library/system.char.iswhitespace.aspx) 方法，将返回 true。

separator 的每一个元素都定义一个单独的分隔符字符。如果两个分隔符相邻，或者在此实例的开头或末尾找到分隔符，则相对应的数组元素包含 [Empty](https://msdn.microsoft.com/zh-cn/library/system.string.empty.aspx)。下表提供了示例：



|字符串值|Separator|返回的数组|
|----|----|----|
|"42, 12, 19"|new Char[] {',', ' '} (C#)Char() = {","c, " "c}) (Visual Basic)|{"42", "", "12", "", "19"}|
|"42..12..19"|new Char[] {'.'} (C#)Char() = {"."c} (Visual Basic)|{"42", "", "12", "", "19"}|
|"Banana"|new Char[] {'.'} (C#)Char() = {"."c} (Visual Basic)|{"Banana"}|
|"Darb\nSmarba" (C#)"Darb" & vbLf & "Smarba" (Visual Basic)|new Char[] {} (C#)Char() = {} (Visual Basic)|{"Darb", "Smarba"}|
|"Darb\nSmarba" (C#)"Darb" & vbLf & "Smarba" (Visual Basic)|null (C#)Nothing (Visual Basic)|{"Darb", "Smarba"}|


### 然后就是那些呆萌呆萌的编程题咯~

为啥我敢在这写……就不怕错了误导大家嘛？

哈哈~~ 每道题我都从【等待评判中】一直等到了【完全正确】才写的下一题哦~

![](https://img-blog.csdn.net/20150411111040648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20150411111130193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

嘛，话不多说，这次大概能学到东西的就是继承与实现了（前两道抄答案的题目如果都有人写错我就没话说了哟~）

先给了一个抽象类，里头一个抽象函数，让你写一个公开矩形类继承它，需要构造函数和长宽的Get\Set函数



```java
package Test02;

public abstract class Shape {
	
	abstract public int getArea();
}
```
这是个抽象类，是个抽象类，因为很重要所以说两次。



抽象类里的东西继承类里你要是不给它写出来，你告诉我你Eclipse老提醒你要加abstract在你的public class前头，

想想看，让你写的就是实现它要不然Shape类里抽象它干啥？

关于Get\Set，Eclipse里有一键添加的快捷键哦~ 不要告诉别人我没提醒你哦~

构造函数，花括号里的东西应该会写吧……

为了方便理解还是贴个代码好了：



```java
package Test02;

public class Rectangle extends Shape{
	int width;
	int height;
	
	Rectangle(int w,int h){width=w; height=h;}
	
	public int getWidth() {
		return width;
	}
	public void setWidth(int width) {
		this.width = width;
	}
	public int getHeight() {
		return height;
	}
	public void setHeight(int height) {
		this.height = height;
	}
	
	public int getArea()
	{
		return width * height;
	}
	
	public static void main(String[] args)
	{
		
	}
}
```


然后是要一个接口的实现类……



当时就懵掉了，啥，接口是啥，实现类又是啥，能吃么？

后来问了大神，啊，关键字implements，然后构造函数是要new一个IList，那就声明一个IList，然后为它new一个写在构造函数里就好啦~

接口如下：



```java
package Test03;

public interface IList {
	void add(Object elem);
	Object get(int index);
	int size();
	void clear();
}
```


实现类如下：





```java
package Test03;

public class MyList implements IList{
	
	public IList list;
	MyList(){list = new MyList();}
	
	public void add(Object elem)
	{
		list.add(elem);
	}
	
	public Object get(int index)
	{
		Object obj = list.get(index);
		return obj;
	}
	
	public int size()
	{
		return list.size();
	}
	
	public void clear()
	{
		list.clear();
	}
	
	public static void main(String[] args)
	{
		
	}
}
```](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=继承&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)




