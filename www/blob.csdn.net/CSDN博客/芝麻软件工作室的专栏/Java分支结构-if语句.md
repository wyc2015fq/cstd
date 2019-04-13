
# Java分支结构-if语句 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月01日 06:39:48[seven-soft](https://me.csdn.net/softn)阅读数：254


在Java程序设计中，分支结构由条件语句（if语句）来完成。if语句既可以单独使用，也可以与else一起使用。以下是使用方法：if语句单独使用
if(条件表达式){
目的;    //这里是一段代码
}
与else一起使用
if(条件表达式){
目的一;   //这里是一段代码
}
else{
目的二;    这里是一段代码
}
注意：条件表达式是一个关系表达式，其结果是布尔型数据。请看如下代码：
public class control1{
public static void main(String[] args){
int salary=10000;
if ((salary>500)&&salary){
System.out.println("要想请吃饭！");
System.out.println("想请唱歌！");
System.out.println("想请喝酒！");
}
}
}
运行结果：
想请吃饭！
想请唱歌！
想请喝酒！
分析：条件语句(salary>500)&&salary中，salary>500的运算结果为true，salary的值为10000，进行"与"运算时转换为true，true&&true的结果为true，条件匹配。
使用好条件语句需要注意以下几点：应该绘制流程图，使编程时思路更加清晰。
编程时，在最简单形式的条件语句中，可以不使用大括号，因为它不会产生混淆，但建议无论是哪种形式的条件语句，都应该使用大括号。

