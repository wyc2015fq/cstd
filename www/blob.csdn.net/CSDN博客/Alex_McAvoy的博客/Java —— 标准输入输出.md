# Java —— 标准输入输出 - Alex_McAvoy的博客 - CSDN博客





2018年10月28日 20:49:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【标准输入输出流】

System 类是 final 类，不能被继承也不能创建实例对象，System 类中提供了标准的输入与输出流。

标准输入流：System.in

标准输出流：System.out

```java
InputStreamRead isr=new InputStreamRead(System.in);
BufferedReader br=new BufferedRead(isr);
try{
    String str=br.readLine();
    br.close();
} catch(IOException e){
    e.printStackTrace();
}
System.out.println(str);
```

# 【Scanner 类】

Scanner 类是 java.util 包中的类，常用该类实现用户的输入，是一种只要有控制台就能实现输入操作的类，其构造方法如下：
- Scanner(InputStream in)：该方法通过 InputStream 类创建实例对象
- Scanner(File file)：该方法通过 File 类创建实例对象

```java
Scanner sc=new Scanner(System.in);//创建实例对象
sc.next();//获取输入
sc.close();//关闭输入流
```



