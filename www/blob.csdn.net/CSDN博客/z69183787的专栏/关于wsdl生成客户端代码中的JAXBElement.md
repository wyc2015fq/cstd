# 关于wsdl生成客户端代码中的JAXBElement - z69183787的专栏 - CSDN博客
2016年12月07日 14:51:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9523
1、使用自动生成的java文件中的 ObjectFactory构造入参
关于cxf生成客户端代码中的JAXBElement<String>
    在使用cxf或者x-fire进行webservice开发时，从wsdl文件生成客户端代码时，如果有属性的类型是String，可能会出现JAXBElement<String>类型。
** 为什么webservice客户端不直接使用String，而使用JAXBElement<String>类型，自然是有原因的，是因为服务器端的wsdl文件有nillable="true"这个属性。**
   此时有两种方法对这个JAXBElement<String>类型赋值。
   1.
```java
User user = new User();  
JAXBElement<String> name = new JAXBElement<String>(new QName("http://memberService", "username"), String.class, "smallnest");
user.setUsername(name);
```
使用这种方法时，有很多人发现客户端的数据确实存入了，可是到了服务器端，通过.getUsername().getValue()调用时，却是null，原因就是Qname值不对！注意，在生成的客户端代码里有private static final QName SERVICE_NAME = new QName("xxx","xx");
并不是这个QName，真正的Qname要去ObjectFactory.java文件去找。只要QName写对，这种调用方法是可以的。
   2.当然，还有一种更简单的方法，直接使用ObjectFactory这个类来调用：
```java
User user = new User();
ObjectFactory objFac=new ObjectFactory();
JAXBElement<String> name = objFac.createUserUsername("smallnest");
user.setUsername(name);
```
这样就简便多了
2、生成代码时
使用的wsdl2java是cxf中的工具，cxf可以到apache下面下载
wsdl2java用法： 
wsdl2java -p com -d srcl  aa.wsdl 
-p  指定其wsdl的命名空间，也就是要生成代码的包名: 
-d  指定要产生代码所在目录 
-client 生成客户端[测试](http://lib.csdn.net/base/softwaretest)web service的代码 
-server 生成服务器启动web  service的代码 
-impl 生成web service的实现代码 
-b  binding-file-name
如果直接使用wsdl2java.bat -client -d E:/tmp -frontend jaxws21 wsdl文件
生成的客户端代码中有JAXBElement<String>
使用wsdl2java.bat -b "1.txt" -client -d E:/tmp -frontend jaxws21 wsdl文件,就可以生成正常的pojo类
1.txt的文件内容为
**[html]**[view
 plain](http://blog.csdn.net/genganpeng/article/details/8288574#)[copy](http://blog.csdn.net/genganpeng/article/details/8288574#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/116574/fork)
- <jaxb:bindingsversion="2.1"
- xmlns:jaxb="http://java.sun.com/xml/ns/jaxb"
- xmlns:xjc="http://java.sun.com/xml/ns/jaxb/xjc"
- xmlns:xs="http://www.w3.org/2001/XMLSchema">
- <jaxb:globalBindingsgenerateElementProperty="false"/>
- </jaxb:bindings>
打包为jar
进入到tmp目录中
jar -cvf ***.jar .
