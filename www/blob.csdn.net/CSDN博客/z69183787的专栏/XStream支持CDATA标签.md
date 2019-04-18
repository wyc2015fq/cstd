# XStream支持CDATA标签 - z69183787的专栏 - CSDN博客
2016年12月15日 17:49:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2337
需求：在生成XML文档中需要支持CDATA标签，而且不是所字段都需要增加，只需要某些特定的字段增加CDATA的标签。 
研究：在网上找了一些相关资料，基本采用的方法都是重载XppDriver，做一个MyXppDriver类对XppDriver进行扩展，MyXppDriver中的作用，就是将要生成的xml文件的text进行字符转义，最后使用XStream
 xstream = new XStream(new MyXppDriver)来创建XStream对象，最终支持CDATA标签。 
思考：首先本人认为这样写这个类比较麻烦，现在我需要修改部份重载让那些要输出的text中，如果有"<![CDATA["开始和"]]>"结束的字段就不用进行任何特殊符号的转义，而某字段没有CDATA标签的起讫符号时，就依然使用原有的PrettyPrintWriter 的writerText方法输出，即将text中的特殊符号进行转义，如:引号(")会被转义为"""，以此来确保输出的xml文件在读取时不会出错 
**[java]**[view
 plain](http://blog.csdn.net/hong0220/article/details/50773226#)[copy](http://blog.csdn.net/hong0220/article/details/50773226#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1593395/fork)
- package com.wechat.utils.xml;  
- 
- import java.io.Writer;  
- 
- import com.thoughtworks.xstream.XStream;  
- import com.thoughtworks.xstream.core.util.QuickWriter;  
- import com.thoughtworks.xstream.io.HierarchicalStreamWriter;  
- import com.thoughtworks.xstream.io.xml.PrettyPrintWriter;  
- import com.thoughtworks.xstream.io.xml.XppDriver;  
- 
- publicclass XStreamUtil {  
- privatestatic String PREFIX_CDATA = "<![CDATA[";  
- privatestatic String SUFFIX_CDATA = "]]>";  
- 
- /**
-      * 全部转化
-      */
- publicstatic XStream initXStream() {  
- returnnew XStream(new XppDriver() {  
- @Override
- public HierarchicalStreamWriter createWriter(Writer out) {  
- returnnew PrettyPrintWriter(out) {  
- protectedvoid writeText(QuickWriter writer, String text) {  
- // if (text.startsWith(PREFIX_CDATA) &&
- // text.endsWith(SUFFIX_CDATA)) {
-                         writer.write(PREFIX_CDATA + text + SUFFIX_CDATA);  
- // } else {
- // super.writeText(writer, text);
- // }
-                     }  
-                 };  
-             }  
-         });  
-     }  
- 
- /**
-      * 初始化XStream可支持某一字段可以加入CDATA标签,如果需要某一字段使用原文,就需要在String类型的text的头加上
-      * "<![CDATA["和结尾处加上"]]>"标签， 以供XStream输出时进行识别
-      * @param isAddCDATA 是否支持CDATA标签
-      */
- publicstatic XStream initXStream(boolean isAddCDATA) {  
-         XStream xstream = null;  
- if (isAddCDATA) {  
-             xstream = new XStream(new XppDriver() {  
- @Override
- public HierarchicalStreamWriter createWriter(Writer out) {  
- returnnew PrettyPrintWriter(out) {  
- protectedvoid writeText(QuickWriter writer, String text) {  
- if (text.startsWith(PREFIX_CDATA) && text.endsWith(SUFFIX_CDATA)) {  
-                                 writer.write(text);  
-                             } else {  
- super.writeText(writer, text);  
-                             }  
-                         }  
-                     };  
-                 }  
-             });  
-         } else {  
-             xstream = new XStream();  
-         }  
- return xstream;  
-     }  
- }  
- 
