# scala简要：文件访问 - 我相信...... - CSDN博客





2015年01月02日 09:56:56[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2614







scala.io.source对象的getlines方法可以读取文件的所有行，可以把source对象当成迭代器读取文件中的每个字符，java.util.Scanner来处理同事包含文本和数字的文件。

从URL中读取时，需要事先知道编码格式，scala中没有提供读取二进制文件的方法，需要使用Java类库,同样没有内建的对写入文件的支持，可使用java.io.PrintWriter,访问目录也要用java的方法，例如java.nio.file.Files类中的walkFileTree。


scala集合类都是可序列化的。




scala.sys.process包提供了用于与shell程序交互的工具，包含了一个从字符串到processbuilder对象的隐式转换，！操作符就是执行这个processbuidler的对象。scala.util.matching.Regex 利用正则表达式对字符串进行分析。



