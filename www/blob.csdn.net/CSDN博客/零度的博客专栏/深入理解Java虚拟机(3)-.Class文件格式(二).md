# 深入理解Java虚拟机(3)-.Class文件格式(二) - 零度的博客专栏 - CSDN博客
2017年08月10日 11:53:23[零度anngle](https://me.csdn.net/zmx729618)阅读数：223
在上一篇文章 [深入理解Java Class文件格式（一）](http://blog.csdn.net/zhangjg_blog/article/details/21486985) 中， 介绍了class文件在整个java体系结构中的位置和作用，
 并对class文件的整体格式做了说明， 介绍了其中的魔数和版本号的相关内容， 并对常量池做了概述。 在本文章， 继续介绍class文件中的其他内容。
# class文件中的特殊字符串
首先说明一下， 所谓的特殊字符串出现在class文件中的常量池中， 所以在上一篇博客中， 只是对常量池介绍了一个大概。 本着循序渐进和减少跨度的原则， 首先把class文件中的特殊字符串做一个详细的介绍， 然后再回过头来继续讲解常量池。 
在上文中， 我们提到特殊字符串是常量池中符号引用的一部分， 至于符号引用的概念， 会在以后提到。 现在我们将重点放在特殊字符串上。 特殊字符串包括三种： 类的全限定名， 字段和方法的描述符， 特殊方法的方法名。 下面我们就分别介绍这三种特殊字符串。
## （1） 类的全限定名
在常量池中， 一个类型的名字并不是我们在源文件中看到的那样， 也不是我们在源文件中使用的包名加类名的形式。 源文件中的全限定名和class文件中的全限定名不是相同的概念。 源文件中的全新定名是包名加类名， 包名的各个部分之间，包名和类名之间， 使用点号分割。 如Object类， 在源文件中的全限定名是java.lang.Object 
 。 而class文件中的全限定名是将点号替换成“/” 。 例如， Object类在class文件中的全限定名是 java/lang/Object 。 如果读者之前没有接触过class文件格式， 是class文件格式的初学者， 在这里不必知道全限定名在class文件中是如何使用的， 只需要知道， 源文件中一个类的名字， 在class文件中是用全限定名表述的。 
## （2） 描述符
我们知道在一个类中可以有若干字段和方法， 这些字段和方法在源文件中如何表述， 我们再熟悉不过了。 既然现在我们要学习class文件格式， 那么我们就要问， 一个字段或一个方法在class文件中是如何表述的？ 在本文中， 我们会讨论方法和字段在class文件中的描述。 方法和字段的描述符并不会把方法和字段的所有信息全都描述出来，
 毕竟描述符只是一个简单的字符串。 
在讲解描述符之前， 要先说明一个问题， 那就是所有的类型在描述符中都有对应的字符或字符串来对应。 比如， 每种基本数据类型都有一个大写字母做对应， void也有一个大写字符做对应。 下表是void和基本数据类型在描述符中的对应。
|**基本数据类型和void类型**|**类型的对应字符**|
|----|----|
|byte|B|
|char|C|
|double|D|
|float|F|
|int|I|
|long|J|
|short|S|
|boolean|Z|
|void|V|
基本上都是以类型的首字符变成大写来对应的， 其中long和boolean是特例， long类型在描述符中的对应字符是J， boolean类型在描述符中的对应字符是Z 。 
基本类型和void在描述符中都有一个大写字符和他们对应， 那么引用类型（类和接口，枚举）在描述符中是如何对应的呢？ 引用类型的对应字符串（注意， 引用类型在描述符中使用一个字符串做对应） ， 这个字符串的格式是：
**[plain]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[print](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[?](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)
- “L” + 类型的全限定名 + “;”  
` “L” + 类型的全限定名 + “;”`
注意，这三个部分之间没有空格， 是紧密排列的。 如Object在描述符中的对应字符串是： Ljava/lang/Object;  ； ArrayList在描述符中的对应字符串是： Ljava/lang/ArrayList;  ； 自定义类型com.example.Person在描述符中的对应字符串是： Lcom/example/Person; 
 。
我们知道， 在Java语言中数组也是一种类型， 一个数组的元素类型和他的维度决定了他的类型。 比如， 在 int[] a 声明中， 变量a的类型是int[] ， 在 int[][] b 声明中， 变量b的类型是int[][] , 在 Object[] c 声明中， 变量c的类型是Object[] 。既然数组是类型，
 那么在描述符中， 也应该有数组类型的对应字符串。 在class文件的描述符中， 数组的类型中每个维度都用一个 [ 代表， 数组类型整个类型的对应字符串的格式如下：
**[plain]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[print](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[?](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)
- 若干个“[”  +  数组中元素类型的对应字符串   
`若干个“[”  +  数组中元素类型的对应字符串 `
下面举例来说名。 int[]类型的对应字符串是： [I  。 int[][]类型的对应字符串是： [[I 。 Object[]类型的对应字符串是： 
 [Ljava/lang/Object; 。 Object[][][]类型的对应字符串是： [[[Ljava/lang/Object; 。
介绍完每种类型在描述符中的对应字符串， 下面就开始讲解字段和方法的描述符。 
字段的描述符就是字段的类型所对应的字符或字符串。 如： int i 中， 字段i的描述符就是 I 。 Object o中， 字段o的描述符就是 Ljava/lang/Object;  。 double[][] d中， 字段d的描述符就是 [[D 。 
方法的描述符比较复杂， 包括所有参数的类型列表和方法返回值。 它的格式是这样的：
**[plain]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[print](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)[?](http://blog.csdn.net/zhangjg_blog/article/details/21487287#)
- (参数1类型 参数2类型 参数3类型 ...)返回值类型  
`   (参数1类型 参数2类型 参数3类型 ...)返回值类型`
其中， 不管是参数的类型还是返回值类型， 都是使用对应字符和对应字符串来表示的， 并且参数列表使用小括号括起来， 并且各个参数类型之间没有空格， 参数列表和返回值类型之间也没有空格。 
下面举例说明（此表格来源于《深入Java虚拟机》）。
|方法描述符|方法声明|
|----|----|
|()I|int getSize()|
|()Ljava/lang/String;|String toString()|
|([Ljava/lang/String;)V|void main(String[] args)|
|()V|void wait()|
|(JI)V|void wait(long timeout, int nanos)|
|(ZILjava/lang/String;II)Z|boolean regionMatches(boolean ignoreCase, int toOffset, String other, int ooffset, int len)|
|([BII)I|int read(byte[] b, int off, int len )|
|()[[Ljava/lang/Object;|Object[][] getObjectArray()|
## （3） 特殊方法的方法名
首先要明确一下， 这里的特殊方法是指的类的构造方法和类型初始化方法。 构造方法就不用多说了， 至于类型的初始化方法， 对应到源码中就是静态初始化块。 也就是说， 静态初始化块， 在class文件中是以一个方法表述的， 这个方法同样有方法描述符和方法名。 
类的构造方法的方法名使用字符串 <init> 表示， 而静态初始化方法的方法名使用字符串 <clinit> 表示。 除了这两种特殊的方法外， 其他普通方法的方法名， 和源文件中的方法名相同。
# 总结
到此为止， 关于特殊字符串就讲解完了。 最后做一下总结：
class文件中的特殊字符串包括类（包括接口， 枚举）的全限定名， 字段的描述符和方法的描述符。 其中类的全限定名比较简单易于理解， 字段和方法的描述符由于涉及到每种类型的映射， 可能稍显复杂。 要理解描述符， 主要是要熟记每种类型（包括8种基本数据类型，类类型， 数组类型和void）在描述符中所对应的描述字符或字符串。 
还有一点需要注意， 就是方法和字段的描述符中， 不包括字段名和方法名， 字段描述符中只包括字段类型， 方法描述符中只包括参数列表和返回值类型。 
本文只是介绍class中的特殊字符串， 关于在class文件中如何使用这些字符串， 将会在下一篇博客中讲解， 敬请关注。 
