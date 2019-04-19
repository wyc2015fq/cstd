# 【java】使用Lambda表达式 - Big Smile - CSDN博客
2018年04月08日 22:25:42[王啸tr1912](https://me.csdn.net/tr1912)阅读数：262标签：[java																[lambda](https://so.csdn.net/so/search/s.do?q=lambda&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java学习](https://blog.csdn.net/tr1912/article/category/6453602)
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        在需求中，有这么一段，是从获得的设备列表（json类型）中获得特定条件的设备，例如未损坏，未使用中的设备等等，由于是一个非构造型的集合，所以就联想到了当时用C#的时候在下面用lambda表达式查询集合中需要的值的方法了，正好在java8的时候引入了这个特性，索性可以尝试一下，但是因为用的json对象，想要获得对应字段的值非常不好转换，所以作罢，然后用刀了一个吧json转换为pojo的类之后，这样做就省了不少的事情。
# 一、使用lambda进行条件查询
        用我们自带的pojo做了一个最简单的例子：
```java
public static void main(String[] arg){
	List<McSceneOrder> settingList=new ArrayList<>();  //数据源
	List<McSceneOrder> settingList2=new ArrayList<>();  结果列
	for(int i=0;i<10;i++){           //循环生成 序列at：+数字的结果
		McSceneOrder mcSceneOrder=new McSceneOrder();
		mcSceneOrder.setSceneOrderId(i);
		mcSceneOrder.setSceneOrderNum("序列at：" + i);
		settingList.add(mcSceneOrder);
	}
	//筛选id不等于10的数据	
	settingList2=settingList.stream().filter(s->s.getSceneOrderId()!=10).collect(Collectors.toList());
	//输出结果	
	settingList2.forEach(mso -> System.out.println(mso.getSceneOrderNum()));
}
```
      结果：
![](https://img-blog.csdn.net/2018040822050457)
        这个简单的例子就用了stream里面的filter方法，然后再filter中写入lambda表达式就可以了。最后用collect转换成了list类型返回给了带接收的list，最后用foreach方法通过lambda遍历出了我们想要输出的结果。其中s和mso都可以是任意的字符。
        这个例子里面用了两个写lambda的方法，一个是stream的filter，另外一个是foreach。
# 二、更多用法
        1、使用map方法可以使用一套方法处理数据：
```java
List<String> list1 = Lists.newArrayList();  
        List<String> list2 = Lists.newArrayList();  
        list1.add("1");  
        list1.add("2");  
        list1.add("3");  
  
        list2 = list1.stream().map(string -> {  
            return "stream().map()处理之后：" + string;  
        }).collect(Collectors.toList());  
          
        list2.stream().forEach(string -> {  
            System.out.println(string);  
        });  
运行结果：  
stream().map()处理之后：1  
stream().map()处理之后：2  
stream().map()处理之后：3
```
    2、加入reduce对于数据处理：
```java
// 为每个订单加上12%的税
// 老方法：
List costBeforeTax = Arrays.asList(100, 200, 300, 400, 500);
double total = 0;
for (Integer cost : costBeforeTax) {
    double price = cost + .12*cost;
    total = total + price;
}
System.out.println("Total : " + total);
 
// 新方法：
List costBeforeTax = Arrays.asList(100, 200, 300, 400, 500);
double bill = costBeforeTax.stream().map((cost) -> cost + .12*cost).reduce((sum, cost) -> sum + cost).get();
System.out.println("Total : " + bill);
```
   3、匿名类的使用：
```java
() -> System.out.println("Hello Lambda Expressions");
```
    上面一句话直接回输出你想要输出的语句。
```java
(int even, int odd) -> even + odd
```
或者加上参数，对于参数进行操作等等。
更多的使用方法，参见：[**http://www.importnew.com/16436.html**](http://www.importnew.com/16436.html)
