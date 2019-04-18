# Java 泛型数组 - z69183787的专栏 - CSDN博客
2014年11月24日 20:11:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：645
个人分类：[泛型](https://blog.csdn.net/z69183787/article/category/2736483)
Java 不支持泛型数组。也就是说，
**[java]**[view plain](http://blog.csdn.net/orzlzro/article/details/7017435#)[copy](http://blog.csdn.net/orzlzro/article/details/7017435#)
- List<String>[] ls = new ArrayList<String>[10];  
是不支持的，而
**[java]**[view plain](http://blog.csdn.net/orzlzro/article/details/7017435#)[copy](http://blog.csdn.net/orzlzro/article/details/7017435#)
- List<String>[] ls = new ArrayList[10]  
却可以。
是我一直不清楚为什么不能够声明泛型的数组，指定类型可以让编译的时候不会出现类型安全的提示。
直到今天我看到[Sun的一篇文档](http://download.oracle.com/javase/tutorial/extra/generics/fineprint.html)才清楚，里面提到了一种情况：
**[java]**[view plain](http://blog.csdn.net/orzlzro/article/details/7017435#)[copy](http://blog.csdn.net/orzlzro/article/details/7017435#)
- List<String>[] lsa = new List<String>[10]; // Not really allowed.
- Object o = lsa;  
- Object[] oa = (Object[]) o;  
- List<Integer> li = new ArrayList<Integer>();  
- li.add(new Integer(3));  
- oa[1] = li; // Unsound, but passes run time store check
- String s = lsa[1].get(0); // Run-time error: ClassCastException.
这种情况下，由于JVM泛型的擦除机制，在运行时JVM是不知道泛型信息的，所以可以给oa[1]赋上一个ArrayList<Integer>而不会出现ArrayStoreException，但是在取出数据的时候却要做一次类型转换，所以就会出现ClassCastException，如果可以进行泛型数组的声明，上面说的这种情况在编译期将不会出现任何的警告和错误，只有在运行时才会出错。而对泛型数组的声明进行限制，对于这样的情况，可以在编译期提示代码有类型安全问题，比没有任何提示要强很多。
基于以上的原因，Java不支持声明泛型数组，更确切地表达是：***数组的类型不可以是类型变量，除非是采用通配符的方式***，看下面这个例子：
**[java]**[view plain](http://blog.csdn.net/orzlzro/article/details/7017435#)[copy](http://blog.csdn.net/orzlzro/article/details/7017435#)
- List<?>[] lsa = new List<?>[10]; // OK, array of unbounded wildcard type.
- Object o = lsa;  
- Object[] oa = (Object[]) o;  
- List<Integer> li = new ArrayList<Integer>();  
- li.add(new Integer(3));  
- oa[1] = li; // Correct.
- String s = (String) lsa[1].get(0); // Run time error, but cast is explicit.
因为对于通配符的方式，最后取出数据是要做显式的类型转换的，所以并不会存在上一个例子的问题。
