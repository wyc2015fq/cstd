# java字符串判断（错题） - westbrook1998的博客 - CSDN博客





2018年02月24日 23:24:26[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：57
个人分类：[java](https://blog.csdn.net/westbrook1998/article/category/7459913)









牛客网 

public class StringDemo{ 

  private static final String MESSAGE=”taobao”; 

  public static void main(String [] args) { 

    String a =”tao”+”bao”; 

    String b=”tao”; 

    String c=”bao”; 

    System.out.println(a==MESSAGE); 

    System.out.println((b+c)==MESSAGE); 

  } 

} 

判断两个输出是true还是false
注意的几个点： 

1.字符串常量在内存中只有一份，尽管可以有多个引用，所以a（”tao”+”bao”）和MESSAGE（”taobao”）是同一个字符串常量，所以为true 

2.hotspot中 编译时”tao”+”bao”将直接变成”taobao”，b+c则不会优化，new一个String对象来处理，而==判断的是是否是同一个引用，因此false




