# Java 的方法签名与字段类型表示-[Ljava.lang.String; - z69183787的专栏 - CSDN博客
2017年01月06日 17:25:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9434
我们什么时候会接触到 Java 的方法签名呢？在进行 JNI 调用时，还有在看方法重载时。重载的方法是有不同的方法签名的，而是不区分返回值，而实际方法签名还揉入了返回值类型的，还有就是 javap -s 查看方法签名时，如 javap -s java.util.Date。
看来方法签名与我们实际工作的关系还真的不大。倒是有次遇着了，事出于 Struts2 应用中提交表单时报出了下面的错误：
00:43:59.716 [http-8080-4] WARN  com.opensymphony.xwork2.ognl.OgnlValueStack - Error setting expression 'version' with value '[Ljava.lang.String;@e18a9a'
ognl.MethodFailedException: Method "setVersion" failed for object [cc.unmi.model.Post@ed0cd7](mailto:cc.unmi.model.Post@ed0cd7)
 at ognl.OgnlRuntime.callAppropriateMethod(OgnlRuntime.java:1285) ~[ognl-3.0.jar:na]
 at ognl.OgnlRuntime.setMethodValue(OgnlRuntime.java:1474) ~[ognl-3.0.jar:na]
 at ognl.ObjectPropertyAccessor.setPossibleProperty(ObjectPropertyAccessor.java:85) ~[ognl-3.0.jar:na]
 at ognl.ObjectPropertyAccessor.setProperty(ObjectPropertyAccessor.java:162) ~[ognl-3.0.jar:na]
 at com.opensymphony.xwork2.ognl.accessor.ObjectAccessor.setProperty(ObjectAccessor.java:27) ~[xwork-core-2.2.1.jar:2.2.1]
 at ognl.OgnlRuntime.setProperty(OgnlRuntime.java:2245) ~[ognl-3.0.jar:na]
 at com.opensymphony.xwork2.ognl.accessor.CompoundRootAccessor.setProperty(CompoundRootAccessor.java:77) ~[xwork-core-2.2.1.jar:2.2.1]
 at ognl.OgnlRuntime.setProperty(OgnlRuntime.java:2245) ~[ognl-3.0.jar:na]
 at ognl.ASTProperty.setValueBody(ASTProperty.java:127) ~[ognl-3.0.jar:na]
 at ognl.SimpleNode.evaluateSetValueBody(SimpleNode.java:220) ~[ognl-3.0.jar:na]
 at ognl.SimpleNode.setValue(SimpleNode.java:301) ~[ognl-3.0.jar:na]
 at ognl.Ognl.setValue(Ognl.java:737) ~[ognl-3.0.jar:na]
 at com.opensymphony.xwork2.ognl.OgnlUtil.setValue(OgnlUtil.java:209) ~[xwork-core-2.2.1.jar:2.2.1]
 at com.opensymphony.xwork2.ognl.OgnlValueStack.trySetValue(OgnlValueStack.java:173) [xwork-core-2.2.1.jar:2.2.1]
...........................................................
 at org.apache.coyote.http11.Http11Protocol$Http11ConnectionHandler.process(Http11Protocol.java:583) [tomcat-coyote.jar:na]
 at org.apache.tomcat.util.net.JIoEndpoint$Worker.run(JIoEndpoint.java:454) [tomcat-coyote.jar:na]
 at java.lang.Thread.run(Thread.java:619) [na:1.6.0_20]
Caused by: java.lang.NoSuchMethodException: cc.unmi.model.Post.setVersion([Ljava.lang.String;)
 at ognl.OgnlRuntime.callAppropriateMethod(OgnlRuntime.java:1226) ~[ognl-3.0.jar:na]
 ... 111 common frames omitted
缺少 setVersion() 方法，看看 Post.java 中确实有啊。注意啦，看那参数是  [Ljava.lang.String;，要是不明白 [L 的用意的话，就很难明白是说不存在 setVersion(String[] args) 方法的。Lxxx; 用来表示对象类型，[ 用来表示数组的。
具体原因是请求中有两个 version 参数，可能是表单中两个，也可能是 url 中一个，表单中也一个；就是用 request.getParameterValue("version") 返回一个数组。其实对于 HttpServletRequest 来说，无论提交的是一个 version 还是多个 version 值，用 request.getParameter("version") 总是取到第一个提交的 version 值，所以这在 struts1 似乎不会出类似的错误。一个或是多个 version 影响的总是
 getParameterValue("version") 是否为 null。
解决办法是只留下一个 version 请求参数，这个问题警示了我们要去 JVM 中的数据类型表示法有一定的嗅觉。
下出列出一下 Java 的类型与虚拟机中的表示法：
字节码助记符的前缀
|类型|代码|示例|描述|
|----|----|----|----|
|byte|b|baload|从数组装载 byte 类型|
|short|s|sastore|将 short 类型存入数组中|
|int|i|iload_1|从局部变量 1 中装载 int 类型|
|long|l|lcmp|比较 long 类型值|
|char|c|i2c|把 int 类型数据转换为 char 类型|
|float|f|fload|从局部变量中装载 float 类型|
|double|d|dconst_1|将 double 类型常量 1.0 压入栈|
|reference|a|aaload|从数组装载引用类型|
本文原始链
Java 虚拟机中的保存类型和计算类型
|保存类型|堆或者方法区中的最小比特数|计算类型|Java 栈帧中的字长|
|----|----|----|----|
|byte|8|int|1|
|short|16|int|1|
|int|32|int|1|
|long|64|long|2|
|char|16|int|1|
|float|32|float|1|
|double|64|double|2|
|reference|32|reference|1|
基本类型终结符(这个与方法签名就比较亲近了)
|终结符|类型|
|----|----|
|B|byte|
|C|char|
|D|double|
|F|float|
|I|int|
|J|long|
|S|short|
|Z|boolean|
boolean 的终结符不为 B 是让 byte 占用了，long 的终结符不是 L 也是因为让对象终结符占用了。返回值为 void 类型的终结符是 V，对象类型终结符为 L 和 ;，数组类型终结符 [
字段描述符示例
|描述符|字段声明|
|----|----|
|I|int i|
|[[J|long[][] windingRoad|
|[Ljava/lang/Object;|Object[] stuff|
|Ljava/util.Hashtable;|java.util.Hashtable ht|
|[[[Z|boolean[][][] isReady|
方法描述符示例
|描述符|方法声明|
|----|----|
|()I|int getSize()|
|()Ljava/lang/String;|String toString()|
|([Ljava/lang/String;)V|void main(String[] args)|
|()V|void wait()|
|(JI)V|void wait(long timeout, int nanos)|
|(Z[Ljava/lang/String;II)Z|boolean regionMatches(boolean ignoreCase, int toOffset, String other, in oooffset, int len|
|([BII)I|int read(byte[] b, int off, int len)|
执行一下 javap -s java.util.Date 来看看 java.util.Date 的所有方法签名，下面列出一个片断：
C:\Documents
 and Settings\Administrator>javap -s java.util.Date
Compiled from "Date.java"
public class java.util.Date extends java.lang.Object implements java............
public java.util.Date();
  Signature: ()V
public java.util.Date(long);
  Signature: (J)V
public java.util.Date(int, int, int);
  Signature: (III)V
public java.util.Date(int, int, int, int, int);
  Signature: (IIIII)V
public java.util.Date(int, int, int, int, int, int);
  Signature: (IIIIII)V
public java.util.Date(java.lang.String);
  Signature: (Ljava/lang/String;)V
public java.lang.Object clone();
  Signature: ()Ljava/lang/Object;
public static long UTC(int, int, int, int, int, int);
  Signature: (IIIIII)J
public static long parse(java.lang.String);
  Signature: (Ljava/lang/String;)J
public int getYear();
.....................................
表格中的数据来自
 《深入 Java 虚拟机》
本文链接 [http://unmi.cc/java-signatures-data-types/](http://unmi.cc/java-signatures-data-types/),
 来自 [隔叶黄莺 Unmi Blog](http://unmi.cc/)
