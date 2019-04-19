# Java中@Override的作用 - 三少GG - CSDN博客
2011年11月18日 13:20:47[三少GG](https://me.csdn.net/scut1135)阅读数：875标签：[java																[string																[编译器																[编程																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[重构Java](https://blog.csdn.net/scut1135/article/category/621771)
Java中@Override的作用
2009-10-15 15:11
@override有注释文档的作用，可有可无有点像鸡肋
但它对于编程粗心的人可是个很人性化的功能
如果想重写父类的方法，比如toString()方法的话，在被重载的方法前面加上@Override ，这样编译的时候系统可以帮你检查方法的正确性
如下
@Override
public String toString(){...}这是正确的
如果将toString写成tostring
@Override
public String tostring(){...}编译器可以检测出这种写法是错误的,提醒你改正
而如果不加@Override
public String tostring(){...}这样编译器是不会报错的，它会认为是你在类中加的新方法
所以编程时一定得细心点，不是所有错误系统都能找到的
