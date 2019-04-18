# Java中在类定义时对HashSet的初始化方法 - bigfacesafdasgfewgf - CSDN博客





2015年03月17日 17:31:42[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：27543










    Java中很多时候都要用到HashSet的查找功能，那么在类的定义时，数据成员假如就是HashSet类型的，我们定义数据成员之后，不好直接调用add函数来实现初始化，这个时候怎么办？




    我们可以这样来做：






```java
public static final HashSet<String> salesWords = new HashSet<String>() {{
		add("销售额");
		add("销售");
		add("销售金额");
		add("销售值");
		add("销售总金额");
		add("销售总额");
		add("销售总值");
	}};
```


上面我们使用两个双括号来完成的，在括号中再add，这样就可以初始化一个HashSet了。






