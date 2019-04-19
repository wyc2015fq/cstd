# Java之由String.getBytes()说编码 - 零度的博客专栏 - CSDN博客
2016年06月15日 17:36:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：361
#### 1、JAVA之String.getBytes()
       在Java中，String的getBytes()方法是得到一个操作系统默认的编码格式的字节数组。这个表示在不同的操作系统下，返回的东西不一样！ String.getBytes(String decode)方法，则会根据指定的decode编码返回某字符串在该编码下的byte数组表示，如：
```
<span style="font-family:Arial;font-size:12px;">byte[] b_gbk = "中".getBytes("GBK"); 
byte[] b_utf8 = "中".getBytes("UTF-8"); 
byte[] b_iso88591 = "中".getBytes("ISO8859-1"); </span>
```
将分别返回“中”这个汉字在GBK、UTF-8和ISO8859-1编码下的byte数组表示，此时b_gbk的长度为2，b_utf8的长度为3，b_iso88591的长度为1。 
而与getBytes相对的，可以通过new String(byte[], decode)的方式来还原这个“中”字时，这个new String(byte[], decode)实际是使用decode指定的编码来将byte[]解析成字符串。
```
<span style="font-family:Arial;font-size:12px;">String s_gbk = new String(b_gbk,"GBK"); 
String s_utf8 = new String(b_utf8,"UTF-8"); 
String s_iso88591 = new String(b_iso88591,"ISO8859-1"); </span>
```
        通过打印s_gbk、s_utf8和s_iso88591，会发现，s_gbk和s_utf8都是“中”，而只有s_iso88591是一个不认识的字符，为什么使用ISO8859-1编码再组合之后，无法还原“中”字呢，其实原因很简单，因为ISO8859-1编码的编码表中，根本就没有包含汉字字符，当然也就无法通过"中".getBytes("ISO8859-1");来得到正确的“中”字在ISO8859-1中的编码值了，所以再通过new
 String()来还原就无从谈起了。 
因此，通过String.getBytes(String decode)方法来得到byte[]时，一定要确定decode的编码表中确实存在String表示的码值，这样得到的byte[]数组才能正确被还原。 
       有时候，为了让中文字符适应某些特殊要求（如http header头要求其内容必须为iso8859-1编码），可能会通过将中文字符按照字节方式来编码的情况，如 ： String s_iso88591 = new String("中".getBytes("UTF-8"),"ISO8859-1")， 这样得到的s_iso8859-1字符串实际是三个在 ISO8859-1中的字符，在将这些字符传递到目的地后， 目的地程序再通过相反的方式String
 s_utf8 = new String(s_iso88591.getBytes("ISO8859-1"),"UTF-8")来得到正确的中文汉字“中”。这样就既保证了遵守协议规定、也支持中文。 
#### 2、JAVA编码转换的详细过程
       我们常见的JAVA程序包括以下类别：
       *直接在console上运行的类(包括可视化界面的类) 
       *JSP代码类（注：JSP是Servlets类的变型） 
       *Servelets类 
       *EJB类 
       *其它不可以直接运行的支持类
       这些类文件中，都有可能含有中文字符串，并且我们常用前三类JAVA程序和用户直接交互，用于输出和输入字符，如：我们在JSP和Servlet中得到客户端送来的字符，这些字符也包括中文字符。无论这些JAVA类的作用如何，这些JAVA程序的生命周期都是这样的： 
*编程人员在一定的操作系统上选择一个合适的编辑软件来实现源程序代码并以.java扩展名保存在操作系统中，例如我们在中文win2k中用记事本编辑一个java源程序； 
       *编程人员用JDK中的javac.exe来编译这些源代码，形成.class类(JSP文件是由容器调用JDK来编译的)； 
       *直接运行这些类或将这些类布署到WEB容器中去运行，并输出结果。 
      那么，在这些过程中，JDK和JVM是如何将这些文件如何编码和解码并运行的呢？ 
      这里，我们以中文win2k操作系统为例说明JAVA类是如何来编码和被解码的。
     第一步，我们在中文win2k中用编辑软件如记事本编写一个Java源程序文件(包括以上五类JAVA程序)，程序文件在保存时默认采用了操作系统默认支持GBK编码格式(操作系统默认支持的格式为file.encoding格式)形成了一个.java文件，也即，java程序在被编译前，我们的JAVA源程序文件是采用操作系统默认支持的file.encoding编码格式保存的，java源程序中含有中文信息字符和英文程序代码；要查看系统的file.encoding参数，可以用以下代码：
```
public class ShowSystemDefaultEncoding { 
public static void main(String[] args) { 
String encoding = System.getProperty("file.encoding"); 
System.out.println(encoding); 
}}
```
       第二步，我们用JDK的javac.exe文件编译我们的Java源程序，由于JDK是国际版的，在编译的时候，如果我们没有用-encoding参数指定我们的JAVA源程序的编码格式，则javac.exe首先获得我们操作系统默认采用的编码格式，也即在编译java程序时，若我们不指定源程序文件的编码格式，JDK首先获得操作系统的file.encoding参数(它保存的就是操作系统默认的编码格式，如WIN2k，它的值为GBK)，然后JDK就把我们的java源程序从file.encoding编码格式转化为JAVA内部默认的UNICODE格式放入内存中。然后，javac把转换后的unicode格式的文件进行编译成.class类文件，此时.class文件是UNICODE编码的，它暂放在内存中，紧接着，JDK将此以UNICODE编码的编译后的class文件保存到我们的操作系统中形成我们见到的.class文件。对我们来说，我们最终获得的.class文件是内容以UNICODE编码格式保存的类文件，它内部包含我们源程序中的中文字符串，只不过此时它己经由file.encoding格式转化为UNICODE格式了。这一步中，对于JSP源程序文件是不同的，对于JSP，这个过程是这样的：即WEB容器调用JSP编译器，JSP编译器先查看JSP文件中是否设置有文件编码格式，如果JSP文件中没有设置JSP文件的编码格式，则JSP编译器调用JDK先把JSP文件用JVM默认的字符编码格式(也即WEB容器所在的操作系统的默认的file.encoding)转化为临时的Servlet类，然后再把它编译成UNICODE格式的class类，并保存在临时文件夹中。如：在中文win2k上，WEB容器就把JSP文件从GBK编码格式转化为UNICODE格式，然后编译成临时保存的Servlet类，以响应用户的请求。 
      第三步，运行第二步编译出来的类，分为三种情况： 
      A、 直接在console上运行的类 
      B、 EJB类和不可以直接运行的支持类(如JavaBean类) 
      C、 JSP代码和Servlet类 
      D、 JAVA程序和数据库之间 
      下面我们分这四种情况来看。 
** A、直接在console上运行的类**
       这种情况，运行该类首先需要JVM支持，即操作系统中必须安装有JRE。运行过程是这样的：首先java启动JVM，此时JVM读出操作系统中保存的class文件并把内容读入内存中，此时内存中为UNICODE格式的class类，然后JVM运行它，如果此时此类需要接收用户输入，则类会默认用file.encoding编码格式对用户输入的串进行编码并转化为unicode保存入内存（用户可以设置输入流的编码格式）。程序运行后，产生的字符串（UNICODE编码的）再回交给JVM，最后JRE把此字符串再转化为file.encoding格式(用户可以设置输出流的编码格式)传递给操作系统显示接口并输出到界面上。以上每一步的转化都需要正确的编码格式转化，才能最终不出现乱码现象。 
**B、EJB类和不可以直接运行的支持类(如JavaBean类) **
由于EJB类和不可以直接运行的支持类，它们一般不与用户直接交互输入和输出，它们常常与其它的类进行交互输入和输出，所以它们在第二步被编译后，就形成了内容是UNICODE编码的类保存在操作系统中了，以后只要它与其它的类之间的交互在参数传递过程中没有丢失，则它就会正确的运行。 
**C、JSP代码和Servlet类**
       经过第二步后，JSP文件也被转化为Servlets类文件，只不过它不像标准的Servlets一校存在于classes目录中，它存在于WEB容器的临时目录中，故这一步中我们也把它做为Servlets来看。 
对于Servlets，客户端请求它时，WEB容器调用它的JVM来运行Servlet，首先，JVM把Servlet的class类从系统中读出并装入内存中，内存中是以UNICODE编码的Servlet类的代码，然后JVM在内存中运行该Servlet类，如果Servlet在运行的过程中，需要接受从客户端传来的字符如：表单输入的值和URL中传入的值，此时如果程序中没有设定接受参数时采用的编码格式，则WEB容器会默认采用ISO-8859-1编码格式来接受传入的值并在JVM中转化为UNICODE格式的保存在WEB容器的内存中。Servlet运行后生成输出，输出的字符串是UNICODE格式的，紧接着，容器将Servlet运行产生的UNICODE格式的串（如html语法，用户输出的串等）直接发送到客户端浏览器上并输出给用户，如果此时指定了发送时输出的编码格式，则按指定的编码格式输出到浏览器上，如果没有指定，则默认按ISO-8859-1编码发送到客户的浏览器上。 
**  D、Java程序和数据库之间**
        对于几乎所有数据库的JDBC驱动程序，默认的在JAVA程序和数据库之间传递数据都是以ISO-8859-1为默认编码格式的，所以，我们的程序在向数据库内存储包含中文的数据时，JDBC首先是把程序内部的UNICODE编码格式的数据转化为ISO-8859-1的格式，然后传递到数据库中，在数据库保存数据时，它默认即以ISO-8859-1保存，所以，这是为什么我们常常在数据库中读出的中文数据是乱码。
