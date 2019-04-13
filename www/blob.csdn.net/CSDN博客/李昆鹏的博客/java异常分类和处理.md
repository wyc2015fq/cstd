
# java异常分类和处理 - 李昆鹏的博客 - CSDN博客


2018年03月08日 14:57:37[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：2281


-------------------------------------**java异常分类和处理**--------------------------------
**异常****异常都是以****Exception****结尾的。**
在我们写程序的时候难免会出现错误，java中的异常机制为了提高我们程序的健壮性和容错性而存在。
异常例子：
![](https://img-blog.csdn.net/20180308145451679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
异常体系
Throwable:  Throwable类是java语言中所有错误或异常的超类
## java.lang
## 类 Throwable
java.lang.Object
**java.lang.Throwable**
**所有已实现的接口：**
Serializable
**直接已知子类：**
Error,Exception
-----------------**错误**-------------------
Error是throwable的子类，他是程序出现严重的问题，这种问题程序解决不了。
如：因为内存溢出或没有可用的内存提供给垃圾回收器时，java虚拟机无法分配一个对象，这时抛      出该异常。
**错误都是以****Error****为结尾**
**如：**
![](https://img-blog.csdn.net/20180308145504564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**----------------------****异常的分类****------------------------**
**1.****编译期异常**
**编译期的异常我们必须处理，如果我们不处理就会编译失败。我们必须通过程序来处理编译期的异常**
**我们两种解决方案：抛出异常，把异常抛给调用当前方法的层内部处理，****catch****来处理。**
**除了RuntimeException****都是编译期的异常**
**java.lang**
**类 Exception**
java.lang.Object
java.lang.Throwable
**java.lang.Exception**
**所有已实现的接口：**
Serializable
**直接已知子类：**
AclNotFoundException,ActivationException,AlreadyBoundException,ApplicationException,AWTException,BackingStoreException,BadAttributeValueExpException,BadBinaryOpValueExpException,BadLocationException,BadStringOperationException,BrokenBarrierException,CertificateException,ClassNotFoundException,CloneNotSupportedException,DataFormatException,DatatypeConfigurationException,DestroyFailedException,ExecutionException,ExpandVetoException,FontFormatException,GeneralSecurityException,GSSException,IllegalAccessException,IllegalClassFormatException,InstantiationException,InterruptedException,IntrospectionException,InvalidApplicationException,InvalidMidiDataException,InvalidPreferencesFormatException,InvalidTargetObjectTypeException,InvocationTargetException,IOException,JAXBException,JMException,KeySelectorException,LastOwnerException,LineUnavailableException,MarshalException,MidiUnavailableException,MimeTypeParseException,MimeTypeParseException,NamingException,NoninvertibleTransformException,NoSuchFieldException,NoSuchMethodException,NotBoundException,NotOwnerException,ParseException,ParserConfigurationException,PrinterException,PrintException,PrivilegedActionException,PropertyVetoException,RefreshFailedException,RemarshalException,**RuntimeException**,SAXException,ScriptException,ServerNotActiveException,SOAPException,SQLException,TimeoutException,TooManyListenersException,TransformerException,TransformException,UnmodifiableClassException,UnsupportedAudioFileException,UnsupportedCallbackException,UnsupportedFlavorException,UnsupportedLookAndFeelException,URIReferenceException,URISyntaxException,UserException,XAException,XMLParseException,XMLSignatureException,XMLStreamException,XPathException
---

public class**Exception**
extendsThrowable
Exception 类及其子类是 Throwable 的一种形式，它指出了合理的应用程序想要捕获的条件。
**从以下版本开始：**
JDK1.0
**另请参见：**
Error,序列化表格
**--------------------------**
**import**java.net.ServerSocket;
**public****class**test3 {
**public****static****void**main(String[]args) {
/**
*有一些异常JVM能够预见到异常，它会让我们来处理
*/
ServerSocketss=**new**ServerSocket(8888);
}
}
**2.****运行期异常**
**编译的时候可以通过，但是在运行的时候产生的异常叫做运行期异常。**
**所有****RuntimeException****的子类都是运行期的异常**
**java.lang**
**类 RuntimeException**
java.lang.Object
java.lang.Throwable
java.lang.Exception
**java.lang.RuntimeException**
**所有已实现的接口：**
Serializable
**直接已知子类：**
AnnotationTypeMismatchException,ArithmeticException,ArrayStoreException,BufferOverflowException,BufferUnderflowException,CannotRedoException,CannotUndoException,ClassCastException,CMMException,ConcurrentModificationException,DOMException,EmptyStackException,EnumConstantNotPresentException,EventException,IllegalArgumentException,IllegalMonitorStateException,IllegalPathStateException,IllegalStateException,ImagingOpException,IncompleteAnnotationException,IndexOutOfBoundsException,JMRuntimeException,LSException,MalformedParameterizedTypeException,MirroredTypeException,MirroredTypesException,MissingResourceException,NegativeArraySizeException,NoSuchElementException,NoSuchMechanismException,NullPointerException,ProfileDataException,ProviderException,RasterFormatException,RejectedExecutionException,SecurityException,SystemException,TypeConstraintException,TypeNotPresentException,UndeclaredThrowableException,UnknownAnnotationValueException,UnknownElementException,UnknownTypeException,UnmodifiableSetException,UnsupportedOperationException,WebServiceException
---

public class**RuntimeException**
extendsException
RuntimeException 是那些可能在 Java 虚拟机正常运行期间抛出的异常的超类。
可能在执行方法期间抛出但未被捕获的 RuntimeException 的任何子类都无需在 throws 子句中进行声明。
**从以下版本开始：**
JDK1.0
**另请参见：**
序列化表格

例子
![](https://img-blog.csdn.net/20180308145549276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**---------------****运行期异常的处理****-------------------------------**
**第一种方法****：****自行解决。**
**Try{**
**//****可能发生异常的代码**
**} catch****（异常类型****变量名）****{**
**//****处理异常的代码**
**}**
**如果把发生异常就不会产生异常的对象也不会进入****catch**
**如果发生异常就会产生一个相应的异常这个异常的实例会传入到****catch****中，走****catch****中的代码。**
**如果异常得到处理（通过****catch****来处理了），****catch{}****后面的代码，调用端的后续代码也会得到执行。**

**异常中要掌握的方法：**
---------------------------------------------------------------------
**public****String****getMessage()**返回此 throwable 的详细消息字符串。
**返回：**
此Throwable实例（可以为null）的详细消息字符串。
---------------------------------------------------------------------
**public void printStackTrace()**
将此 throwable 及其追踪输出至标准错误流。此方法将此 Throwable 对象的堆栈跟踪输出至错误输出流，作为字段 System.err 的值。输出的第一行包含此对象的toString()方法的结果。剩余行表示以前由方法fillInStackTrace()记录的数据。此信息的格式取决于实现，但以下示例是最常见的：
---------------------------------------------------------------------
**public****String****toString()**
返回此 throwable 的简短描述。结果是以下字符串的串联：
·        此对象的类的name
·        ": "（冒号和一个空格）
·        调用此对象getLocalizedMessage()方法的结果
如果 getLocalizedMessage 返回 null，则只返回类名称。
**覆盖：**
类Object中的toString
**返回：**
该 throwable 的字符串表示形式。
---------------------------------------------------------------------
**例子：**
**public****class**test2 {
**public****static****void**main(String[]args) {
**int**a= 10;
**int**b= 0;
//a = a / b;
//如果发生运行时的异常，后面的代码不会执行
*divide*(a,b);
System.**out**.println("-----------");
System.**out**.println(a);
}
**public****static****int**divide(**int**a,**int**b) {
**int**c= 0;
**try**{
//如果下面这行代码发生异常，
//jvm就会创建出ArithmeticException的实列，
//传给catch中的变量。
c=a/b;
//在try语句块中如果发生异常，异常这行代码后面的不会执行
System.**out**.println("try中异常后面的代码");
}**catch**(ArithmeticExceptione) {
//打印异常信息
Stringmes=e.getMessage();
System.**out**.println(mes);
System.**out**.println("**********************************************");
//把异常打印出来
e.printStackTrace();
System.**out**.println("***********************************************");
//打印出来具体的异常类和异常信息
Stringst=e.toString();
System.**out**.println(st);
System.**out**.println("发生了异常，我要处理了！");
}
//如果发生运行时的异常，后面的代码不会执行
**return**c;
}
}
**第二种方法****：****向上抛出，如果不处理就会自动向上抛。**
**由于发生异常，把异常抛向上层。**
**由于执行者出现抛异常的情况，所以调用者必须处理异常所以如果不去处理那么后面的代码就不会执行**
**Try{}catch{}****的快捷键****shift+alt+Z****（需要选中异常）**
**列子：**
**public****class**test4 {
**public****static****void**main(String[]args) {
**int**a= 10;
**int**b= 0;
//Try{}catch{}的快捷键shift+alt+Z
**try**{
*divide*(a,b);
}**catch**(Exceptione) {
e.printStackTrace();
System.**out**.println("调用者在处理异常");
}
System.**out**.println("-----------");
System.**out**.println(a);
}
**public****static****int**divide(**int**a,**int**b) {
**int**c= 0;
//如果执行者不去处理就会自动的抛向调用者
c=a/b;
//由于上面的代码出现了异常，以下代码不会得到执行
System.**out**.println("-----------");
**return**c;
}
-----------------------**多种异常的处理**-------------------------------------
**在一段程序中可能产生多种异常**
**例子一：**
**public****class**test5 {
**public****static****void**main(String[]args) {
**int**a= 10;
**int**b= 0;
//Try{}catch{}的快捷键shift+alt+Z
*divide*(a,b);
}
**public****static****int**divide(**int**a,**int**b) {
**int**c= 0;
**try**{
**int**[]arr=**new****int**[10];
System.**out**.println(arr[10]);
c=a/b;
//数组越界异常
}**catch**(IndexOutOfBoundsExceptione) {
System.**out**.println("处理数组越界异常");
//数学异常
}**catch**(ArithmeticExceptione) {
System.**out**.println("处理数学异常");
}
System.**out**.println("-----------");
**return**c;
}
}
**例子二：**
**public****class**test6 {
**public****static****void**main(String[]args) {
**int**a= 10;
**int**b= 0;
//Try{}catch{}的快捷键shift+alt+Z
*divide*(a,b);
}
**public****static****int**divide(**int**a,**int**b) {
**int**c= 0;
**try**{
**int**[]arr=**new****int**[10];
System.**out**.println(arr[10]);
c=a/b;
}**catch**(Exceptione) {
**if**(e**instanceof**ArithmeticException) {
System.**out**.println("处理数学异常");
}**else**{
System.**out**.println("数组越界异常");
}
}
System.**out**.println("-----------");
**return**c;
}
}
**------------------****异常中的****finally-------------------------**
finally里面的代码无论何时一定会执行，主要用于释放资源
例子一：
**try**{
*divide*(a,b);
//finally里面的代码无论何时一定会执行，
//主要用于释放资源
}**finally**{
System.**out**.println("finally被执行");
}
**例子二：**
**public****class**test8 {
**public****static****void**main(String[]args) {
**int**a= 10;
**int**b= 0;
//Try{}catch{}的快捷键shift+alt+Z
**int**i=*divide*();
//finally里面的代码无论何时一定会执行，
//主要用于释放资源
System.**out**.println(i);
}

**public****static****int**divide() {
**int**i= 0;
**int**a= 9;
**int**b= 0;
**try**{
i= 2;
a=a/b;
**return**i;
}**catch**(Exceptione) {
i= 3;
//执行了return放回值就保存下来了
//return后的finally无法改变已经形成了的返回值
**return**i;
}**finally**{
System.**out**.println("finally被执行到");
i= 4;
//这个return会修改返回值
**return**i;
}
}
}
**------------------------------------------------------**
**Final****，****finally****，****finalize****的区别？**
**1.****Final****可以修饰类，方法，属性，如果在类上，类不能被继承，如果在方法上不能重写，如果在属性上值不能改。**
**2.****Finally****是和异常****try…catch****异常处理模块一同使用的语句块，它在最后一定被执行。**
**3.****Finalize****当堆中的对象没有任何引用时，这个方法会被调用，来做垃圾回收。**


