# 基本类型包装类以及String类型的常量池 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年01月16日 15:04:47[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：85



public class try2 {

public static void main(String[] args) {

//只要new 就会创建对象，在堆中开辟空间，尽管常量池中以存在相同的字符串。
//句柄指向 堆 地址
String s1 = new String("abc");
String s2 = new String("abc");
System.out.println(s1==s2);     //false  

//这种声明方法，在常量池中已存在该字符串的情况下，不会再常量池中开辟新的空间
//句柄指向常量池地址
String s3 = "abc";
String s4 = "abc";
System.out.println(s3==s4);     //true

//“==”对于基本类型而言，判断其值是否相等。其他则判断的是地址
int s5 =130;
int s6 =130;
System.out.println(s5==s6);    //true
System.out.println(s1.equals(s2)); //true

//8大基本类型的包装类中，除了Float和Double 没有常量池，其余均有
//当值在-128到127范围内，不会在常量池中开辟新的空间
Integer s9 = 130;
Integer s10 = 130;
System.out.println(s9==s10); //false

Integer s11 = 110;
Integer s12 = 110;
System.out.println(s11==s12);//true

}

}
            

