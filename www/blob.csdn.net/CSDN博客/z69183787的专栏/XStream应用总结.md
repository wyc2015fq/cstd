# XStream应用总结 - z69183787的专栏 - CSDN博客
2016年12月15日 17:37:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：302
XStream应用总结
XStream是一个Java对象和XML相互转换的工具，很好很强大。提供了所有的基础类型、数组、集合等类型直接转换的支持。因此XML常用于数据交换、对象序列化（这种序列化和Java对象的序列化技术有着本质的区别）。
XStream中的核心类就是XStream类，一般来说，熟悉这个类基本就够用了，如果你用的更多，估计是你设计有问题，否则不需要。
XStream对象相当Java对象和XML之间的转换器，转换过程是双向的。创建XSteam对象的方式很简单，只需要new XStream()即可。
Java到xml，用toXML()方法。
Xml到Java，用fromXML()方法。
在没有任何设置默认情况下，java到xml的映射，是java成员名对应xml的元素名，java类的全名对应xml根元素的名字。而实际中，往往是xml和java类都有了，要完成相互转换，必须进行别名映射。
别名配置包含三种情况：
1、类别名，用alias(String name, Class type)。
2、类成员别名，用aliasField(String alias, Class definedIn, String fieldName)
3、类成员作为属性别名，用 aliasAttribute(Class definedIn, String attributeName, String alias)，单独命名没有意义，还要通过useAttributeFor(Class definedIn, String fieldName) 应用到某个类上。
别名的配置是非常重要的，但是其中有些细节问题很重要，在例子中会专门做详细说明。
另外还有不太常用的方法：
addImplicitCollection(Class ownerType, String fieldName)，去掉集合类型生成xml的父节点。
registerConverter(Converter converter) ，注册一个转换器。
如果你的xml很大，或者为了安全性，以流的方式传输，那么XStream也提供丰富的API，
使用起来也非常简便。目前还用不到，暂不考虑。
如果这些基本的操作还不能满足你应用的需求，XStream提供丰富的扩展点。你可以实现自己的转换器。还可以利用XStream完成更负责的功能，比如输出其他非xml格式的数据，还可以输出html，还支持XML Dom类型数据，这些应用起来稍微复杂些。当然这些不是XStream应用的重点，也不用理会，真正需要的时候再查看API和源码研究研究。
XStream的优点很多，但是也有一些小bug，比如在定义别名中的下划线“_”转换为xml后会变成“__”这个符号，很变态。因此，尽量避免在别名中实用任何符号，却是需要下划线的时候，可以考虑实用连接符“-”，这个没有问题。
另外，我们的Java Bean中，常常有一些常量，在转换过程，XStream也会将这些常量转换过去，形成常量的xml节点，这显然不是想要的结果，对于常量字段，就不做转换了。
下面给出一个非常典型的而且实用的例子，作为对总结的补充：
package test; 
import java.util.List; 
/** 
* Created by IntelliJ IDEA.<br> 
* <b>User</b>: leizhimin<br> 
* <b>Date</b>: 2008-5-22 21:10:13<br> 
* <b>Note</b>: Please add comment here! 
*/
publicclass Person { 
private String name; 
private String age; 
private Profile profile; 
private List<Address> addlist; 
public Person(String name, String age, Profile profile, List<Address> addlist) { 
this.name = name; 
this.age = age; 
this.profile = profile; 
this.addlist = addlist; 
    } 
public String toString() { 
return"Person{" + 
"name='" + name + '\'' + 
", age='" + age + '\'' + 
", profile=" + profile + 
", addlist=" + addlist + 
                '}'; 
    } 
}
package test; 
import java.sql.Date; 
/** 
* Created by IntelliJ IDEA.<br> 
* <b>User</b>: leizhimin<br> 
* <b>Date</b>: 2008-5-22 21:10:32<br> 
* <b>Note</b>: Please add comment here! 
*/
publicclass Profile { 
private String job; 
private String tel; 
private String remark; 
public Profile(String job, String tel, String remark) { 
this.job = job; 
this.tel = tel; 
this.remark = remark; 
    } 
public String toString() { 
return"Profile{" + 
"job='" + job + '\'' + 
", tel='" + tel + '\'' + 
", remark='" + remark + '\'' + 
                '}'; 
    } 
}
package test; 
/** 
* Created by IntelliJ IDEA.<br> 
* <b>User</b>: leizhimin<br> 
* <b>Date</b>: 2008-5-22 21:10:22<br> 
* <b>Note</b>: Please add comment here! 
*/
publicclass Address { 
private String add; 
private String zipcode; 
public Address(String add, String zipcode) { 
this.add = add; 
this.zipcode = zipcode; 
    } 
public String toString() { 
return"Address{" + 
"add='" + add + '\'' + 
", zipcode='" + zipcode + '\'' + 
                '}'; 
    } 
}
package test; 
import com.thoughtworks.xstream.XStream; 
import java.util.List; 
import java.util.ArrayList; 
/** 
* Created by IntelliJ IDEA.<br> 
* <b>User</b>: leizhimin<br> 
* <b>Date</b>: 2008-5-22 21:10:47<br> 
* <b>Note</b>: XStream学习[[url]http://lavasoft.blog.51cto.com[/url]] 
*/
publicclass TestXStream { 
publicstaticvoid main(String args[]) { 
        test(); 
    } 
publicstaticvoid test() { 
        System.out.println("----------XStream学习:http://lavasoft.blog.51cto.com----------"); 
//目标对象 
        Address address1 = new Address("郑州市经三路", "450001"); 
        Address address2 = new Address("西安市雁塔路", "710002"); 
        List<Address> addList = new ArrayList<Address>(); 
        addList.add(address1); 
        addList.add(address2); 
        Profile profile = new Profile("软件工程师", "13512129933", "备注说明"); 
        Person person = new Person("熔岩", "27", profile, addList); 
//转换装配 
        XStream xStream = new XStream(); 
/**************    设置类别名   ****************/
        xStream.alias("PERSON", test.Person.class); 
        xStream.alias("PROFILE", test.Profile.class); 
        xStream.alias("ADDRESS", test.Address.class); 
        output(1, xStream, person); 
/*************  设置类成员的别名 ***************/
//设置Person类的name成员别名Name 
        xStream.aliasField("Name", Person.class, "name"); 
/*[注意] 设置Person类的profile成员别名PROFILE,这个别名和Profile类的别名一致, 
         * 这样可以保持XStream对象可以从profile成员生成的xml片段直接转换为Profile成员, 
         * 如果成员profile的别名和Profile的别名不一致,则profile成员生成的xml片段不可 
         * 直接转换为Profile对象,需要重新创建XStream对象,这岂不给自己找麻烦?       */
        xStream.aliasField("PROFILE", test.Person.class, "profile"); 
        xStream.aliasField("ADDLIST", test.Person.class, "addlist"); 
        xStream.aliasField("Add", test.Address.class, "add"); 
        xStream.aliasField("Job", test.Profile.class, "job"); 
        output(2, xStream, person); 
/*******  设置类成员为xml一个元素上的属性 *******/
        xStream.useAttributeFor(Address.class, "zipcode"); 
/*************  设置属性的别名 ***************/
        xStream.aliasAttribute(test.Address.class, "zipcode", "Zipcode"); 
        output(3, xStream, person); 
/*************  将xml转为java对象 ******×****/
        String person_xml = "<PERSON>\n" + 
"  <Name>熔岩</Name>\n" + 
"  <age>27</age>\n" + 
"  <PROFILE>\n" + 
"    <Job>软件工程师</Job>\n" + 
"    <tel>13512129933</tel>\n" + 
"    <remark>备注说明</remark>\n" + 
"  </PROFILE>\n" + 
"  <ADDLIST>\n" + 
"    <ADDRESS Zipcode=\"450001\">\n" + 
"      <Add>郑州市经三路</Add>\n" + 
"    </ADDRESS>\n" + 
"    <ADDRESS Zipcode=\"710002\">\n" + 
"      <Add>西安市雁塔路</Add>\n" + 
"    </ADDRESS>\n" + 
"  </ADDLIST>\n" + 
"</PERSON>"; 
        String profile_xml = "  <PROFILE>\n" + 
"    <Job>软件工程师</Job>\n" + 
"    <tel>13512129933</tel>\n" + 
"    <remark>备注说明</remark>\n" + 
"  </PROFILE>"; 
        String address_xml = "    <ADDRESS Zipcode=\"710002\">\n" + 
"      <Add>西安市雁塔路</Add>\n" + 
"    </ADDRESS>"; 
//同样实用上面的XStream对象xStream 
        System.out.println(xStream.fromXML(person_xml).toString()); 
        System.out.println(xStream.fromXML(profile_xml).toString()); 
        System.out.println(xStream.fromXML(address_xml).toString()); 
    } 
publicstaticvoid output(int i,
 XStream xStream, Object obj) { 
        String xml = xStream.toXML(obj); 
        System.out.println(">>>第[ " + i + "]次输出\n"); 
        System.out.println(xml + "\n"); 
    } 
}
----------XStream学习:[url]http://lavasoft.blog.51cto.com----------[/url] 
>>>第[ 1]次输出 
<PERSON> 
  <name>熔岩</name> 
  <age>27</age> 
  <profile> 
    <job>软件工程师</job> 
    <tel>13512129933</tel> 
    <remark>备注说明</remark> 
  </profile> 
  <addlist> 
    <ADDRESS> 
      <add>郑州市经三路</add> 
      <zipcode>450001</zipcode> 
    </ADDRESS> 
    <ADDRESS> 
      <add>西安市雁塔路</add> 
      <zipcode>710002</zipcode> 
    </ADDRESS> 
  </addlist> 
</PERSON> 
>>>第[ 2]次输出 
<PERSON> 
  <Name>熔岩</Name> 
  <age>27</age> 
  <PROFILE> 
    <Job>软件工程师</Job> 
    <tel>13512129933</tel> 
    <remark>备注说明</remark> 
  </PROFILE> 
  <ADDLIST> 
    <ADDRESS> 
      <Add>郑州市经三路</Add> 
      <zipcode>450001</zipcode> 
    </ADDRESS> 
    <ADDRESS> 
      <Add>西安市雁塔路</Add> 
      <zipcode>710002</zipcode> 
    </ADDRESS> 
  </ADDLIST> 
</PERSON> 
>>>第[ 3]次输出 
<PERSON> 
  <Name>熔岩</Name> 
  <age>27</age> 
  <PROFILE> 
    <Job>软件工程师</Job> 
    <tel>13512129933</tel> 
    <remark>备注说明</remark> 
  </PROFILE> 
  <ADDLIST> 
    <ADDRESS Zipcode="450001"> 
      <Add>郑州市经三路</Add> 
    </ADDRESS> 
    <ADDRESS Zipcode="710002"> 
      <Add>西安市雁塔路</Add> 
    </ADDRESS> 
  </ADDLIST> 
</PERSON> 
Person{name='熔岩', age='27', profile=Profile{job='软件工程师', tel='13512129933', remark='备注说明'}, addlist=[Address{add='郑州市经三路', zipcode='450001'}, Address{add='西安市雁塔路', zipcode='710002'}]} 
Profile{job='软件工程师', tel='13512129933', remark='备注说明'} 
Address{add='西安市雁塔路', zipcode='710002'} 
Process finished with exit code 0
在实际中，类的属性很多，嵌套层次也很复杂，如果仅仅使用XStream原生API来硬编码设置别名等属性，显得太生硬也难以维护。完全可以考虑通过一个xml配置文件来定义所有用到的类的别名定义（包括其成员），然后，通过读取配置构建一个XStream的工厂，在用到时候直接去取，而不是让实用者组装。我目前的一个项目中，就是这么实现的，效果非常的好。
下面我给出针对上面提出的问题一个解决方案：
思想：考虑做一个过滤器，在xml转java之前，在Java转xml之后，应用这个过滤器。这个过滤器提供将xml中的“__”替换为“-”，并且将xml中的不需要的节点剔除。
在过滤之前，我实现了个转换器装配，这一步通过xml来配置，并在java中获取。
代码就省略了，这一步很灵活，关键看你的应用了。
为了能过滤xml，我们需要用Dom4j递归遍历xml文档。下面一些算法代码：
//递归算法：遍历配置文件，找出所有有效的xpath 
privatestaticvoid recursiveElement(Element element) { 
        List<Element> elements = element.elements(); 
        validXPathList.add(element.getPath()); 
if (elements.size() == 0) { 
//没有子元素 
        } else { 
//有子元素 
for (Iterator<Element> it = elements.iterator(); it.hasNext();) { 
//递归遍历 
                recursiveElement(it.next()); 
            } 
        } 
    } 
//递归算法：遍历xml，标识无效的元素节点 
privatestaticvoid recursiveFixElement(Element element) { 
        List<Element> elements = element.elements(); 
if (!validXPathList.contains(element.getPath())) { 
            element.addAttribute("delete", "true"); 
        } 
if (elements.size() == 0) { 
//没有子元素 
        } else { 
//有子元素 
for (Iterator<Element> it = elements.iterator(); it.hasNext();) { 
                Element e = it.next(); 
if (!validXPathList.contains(e.getPath())) { 
                    e.addAttribute("delete", "true"); 
                } 
//递归遍历 
                recursiveFixElement(e); 
            } 
        } 
    } 
/** 
     * 过滤器接口方法，转换不规范字符，剔除无效节点 
     * 
     * @param xmlStr 要过滤的xml 
     * @return 符合转换器要求的xml 
     */
publicstatic String filter(String xmlStr) { 
        Document document = null; 
try { 
            document = DocumentHelper.parseText(xmlStr.replaceAll("__", "_")); 
//递归的调用：标记要剔除的xml元素 
            recursiveFixElement(document.getRootElement());      
            List<Node> nodeList = document.selectNodes("//@delete"); 
for (Node node : nodeList) { 
                node.getParent().detach();  //剔除xml元素 
            } 
        } catch (DocumentException e) { 
            System.out.println(e.getMessage()); 
            e.printStackTrace(); 
        } 
return document.asXML(); 
    }
