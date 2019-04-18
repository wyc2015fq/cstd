# XPP3 即 MXP1--一个非常快的XML解析器 - z69183787的专栏 - CSDN博客
2015年06月27日 22:20:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1037
alfresco用到了大量的第三方的工具或者项目。本文列举并介绍这些项目和工具，以便在使用和分析alfresco的时候能够有一些帮助。
**1、XPP3 也叫 MXP1**
# XPP —— XML Pull Parser，XPP3是一个非常快的XML解析器，是第三版的XPP。
XPP2是XPP的第二版。
XPP3是XPP2的继承者，实现了 [http://www.xmlpull.org](http://www.xmlpull.org/) 定义的 XmlPull
 API 。
XPP3只能用于解析XML，不支持节点树的增加操作。
看到有人做的对比测试，XPP3 比 DOM4J 快6倍，也就是使用了1/6时间。
XPP3使用的是 Apache 1.1 variant License 协议。
[http://www.extreme.indiana.edu/xgws/xsoap/xpp/](http://www.extreme.indiana.edu/xgws/xsoap/xpp/)
[http://www.extreme.indiana.edu/xgws/xsoap/xpp/mxp1/index.html](http://www.extreme.indiana.edu/xgws/xsoap/xpp/mxp1/index.html)
[http://mvnrepository.com/artifact/xpp3/xpp3](http://mvnrepository.com/artifact/xpp3/xpp3)
[http://www.xmlpull.org/](http://www.xmlpull.org/)
使用举例：[http://www.xmlpull.org/v1/download/unpacked/doc/quick_intro.html](http://www.xmlpull.org/v1/download/unpacked/doc/quick_intro.html)
MyXmlPull.java
============
**[java]**[view
 plain](http://blog.csdn.net/teamlet/article/details/7700506#)[copy](http://blog.csdn.net/teamlet/article/details/7700506#)
- import java.io.FileReader;  
- import java.io.IOException;  
- import java.io.StringReader;  
- 
- import org.xmlpull.v1.XmlPullParser;  
- import org.xmlpull.v1.XmlPullParserException;  
- import org.xmlpull.v1.XmlPullParserFactory;  
- 
- /**
-  * An example of an application that uses XMLPULL V1 API.
-  *
-  * @author <a href="http://www.extreme.indiana.edu/~aslom/">Aleksander Slominski</a>
-  */
- publicclass MyXmlPullApp  
- {  
- 
- publicfinalstatic String SAMPLE_XML =  
- "<?xml version=\"1.0\"?>\n"+  
- "\n"+  
- "<poem xmlns=\"http://www.megginson.com/ns/exp/poetry\">\n"+  
- "<title>Roses are Red</title>\n"+  
- "<l>Roses are red,</l>\n"+  
- "<l>Violets are blue;</l>\n"+  
- "<l>Sugar is sweet,</l>\n"+  
- "<l>And I love you.</l>\n"+  
- "</poem>";  
- 
- publicstaticvoid main (String args[])  
- throws XmlPullParserException, IOException  
-     {  
-         XmlPullParserFactory factory = XmlPullParserFactory.newInstance();  
-         factory.setNamespaceAware(true);  
-         XmlPullParser xpp = factory.newPullParser();  
-         System.out.println("parser implementation class is "+xpp.getClass());  
- 
-         MyXmlPullApp app = new MyXmlPullApp();  
- 
- if(args.length == 0) {  
-             System.out.println("Parsing simple sample XML");//:\n"+ SAMPLE_XML);
-             xpp.setInput( new StringReader( SAMPLE_XML ) );  
-             app.processDocument(xpp);  
-         } else {  
- for (int i = 0; i < args.length; i++) {  
-                 System.out.println("Parsing file: "+args[i]);  
-                 xpp.setInput ( new FileReader ( args [i] ) );  
-                 app.processDocument(xpp);  
-             }  
-         }  
-     }  
- 
- 
- publicvoid processDocument(XmlPullParser xpp)  
- throws XmlPullParserException, IOException  
-     {  
- int eventType = xpp.getEventType();  
- do {  
- if(eventType == xpp.START_DOCUMENT) {  
-                 System.out.println("Start document");  
-             } elseif(eventType == xpp.END_DOCUMENT) {  
-                 System.out.println("End document");  
-             } elseif(eventType == xpp.START_TAG) {  
-                 processStartElement(xpp);  
-             } elseif(eventType == xpp.END_TAG) {  
-                 processEndElement(xpp);  
-             } elseif(eventType == xpp.TEXT) {  
-                 processText(xpp);  
-             }  
-             eventType = xpp.next();  
-         } while (eventType != xpp.END_DOCUMENT);  
-     }  
- 
- 
- publicvoid processStartElement (XmlPullParser xpp)  
-     {  
-         String name = xpp.getName();  
-         String uri = xpp.getNamespace();  
- if ("".equals (uri)) {  
-             System.out.println("Start element: " + name);  
-         } else {  
-             System.out.println("Start element: {" + uri + "}" + name);  
-         }  
-     }  
- 
- 
- publicvoid processEndElement (XmlPullParser xpp)  
-     {  
-         String name = xpp.getName();  
-         String uri = xpp.getNamespace();  
- if ("".equals (uri))  
-             System.out.println("End element: " + name);  
- else
-             System.out.println("End element:   {" + uri + "}" + name);  
-     }  
- 
- int holderForStartAndLength[] = newint[2];  
- 
- publicvoid processText (XmlPullParser xpp) throws XmlPullParserException  
-     {  
- char ch[] = xpp.getTextCharacters(holderForStartAndLength);  
- int start = holderForStartAndLength[0];  
- int length = holderForStartAndLength[1];  
-         System.out.print("Characters:    \"");  
- for (int i = start; i < start + length; i++) {  
- switch (ch[i]) {  
- case'\\':  
-                     System.out.print("\\\\");  
- break;  
- case'"':  
-                     System.out.print("\\\"");  
- break;  
- case'\n':  
-                     System.out.print("\\n");  
- break;  
- case'\r':  
-                     System.out.print("\\r");  
- break;  
- case'\t':  
-                     System.out.print("\\t");  
- break;  
- default:  
-                     System.out.print(ch[i]);  
- break;  
-             }  
-         }  
-         System.out.print("\"\n");  
-     }  
- }  
java源文件下载地址： [http://download.csdn.net/detail/teamlet/4399658](http://download.csdn.net/detail/teamlet/4399658)
