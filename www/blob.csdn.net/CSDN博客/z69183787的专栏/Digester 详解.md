# Digester 详解 - z69183787的专栏 - CSDN博客
2015年07月19日 12:50:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1737
Digester本来仅仅是Jakarta Struts中的一个工具，用于处理struts-config.xml配置文件。显然，将XML文件转换成相应的Java对象是一项很通用的功能，这个工具理应具有更广泛的用途，所以很快它就在Jakarta Commons项目(用于提供可重用的Java组件库)中有了一席之地。
如今Digester随着Struts的发展以及其的公用性而被提到commons中独自立项，是apache的一个组件 apache commons-digester.jar,通过它可以很方便的从xml文件生成java对象.你不用再象以前通过jdom或者Xerces去读取一个document对象.(jdom和Xerces仍然有它们的用武之地及强大之处,在其它应用里你也少不了它们) 。
Digester由"事件"驱动，通过调用预定义的规则操作对象栈，将XML文件转换为Java对象。工作原理如下:
Digester底层采用[SAX解析](http://baike.baidu.com/view/4180507.htm)XML文件，所以很自然的，对象转换由"事件"驱动，即在识别出特定XML元素时(实际被细分为begin、body、end、finish四个时点)，将执行特定的动作，比如创建特定的Java对象，或调用特定对象的方法等。此处的XML元素根据匹配模式(matching
 pattern)识别，而相关操作由规则(rule)定义。在转换过程中，Digester维持了一个对象栈，可以看作对象转换的工作台，用来存放转换中生成的、或是为转换临时创建的Java对象。对输入XML文件作了一趟完整的扫描后，对象栈的栈顶元素即为目标对象。由于Digester屏蔽了[SAX解析](http://baike.baidu.com/view/4180507.htm)的细节，使用者仅需关注转换操作本身，大大简化了转换操作。
对使用者而言，Digester的核心在于匹配模式与规则(matching pattern + rule)。
匹配规则示例如下:
<a> -- Matches pattern "a"
<b> -- Matches pattern "a/b"
<c/> -- Matches pattern "a/b/c"
<c/> -- Matches pattern "a/b/c"
</b>
<b> -- Matches pattern "a/b"
<c/> -- Matches pattern "a/b/c"
<c/> -- Matches pattern "a/b/c"
<c/> -- Matches pattern "a/b/c"
</b>
</a>
Digester提供了一些编程中经常用到的规则(rule)，以下五类九个rule较为常用:
A:对象创建
1.ObjectCreateRule 当begin()方法被调用时, 此rule创建相应Java对象, 并将其push到Digester的对象栈上。当end()方法被调用时, 栈顶对象将被pop, Digester内所有对该对象的引用都将失效。
2.FactoryCreateRule 创建Java对象的另一种选择。当待创建的Java对象没有无参[构造函数](http://baike.baidu.com/view/411124.htm)，或需要在创建时需要进行额外的设置时，需要用此rule。
B:属性设置
3.SetPropertiesRule 当begin()方法被调用时, Digester使用标准的Java反射API，将栈顶对象的属性设置为XML元素的同名属性值。
4.SetPropertyRule 当begin()方法被调用时, Digester调用栈顶对象某指定属性的设置方法，设置其值。
C:父子关系管理
5.SetNextRule 当end()方法被调用时, Digester将栈顶元素设置进次栈顶元素中(调用相应的设置方法)。
6.SetTopRule 当end()方法被调用时, Digester将次栈顶元素设置[进栈](http://baike.baidu.com/view/346788.htm)顶元素中(调用相应的设置方法)。
D:任意方法调用
7.CallMethodRule 当end()方法被调用时, Digester将调用栈顶元素指定名称的方法。除了方法名外，此rule还需要配置参数数目，参数类型。参数值一般通过CallParamRule得到。
8.CallParamRule 此rule内嵌于CallParamRule中，按顺序(相对于0)定义了CallParamRule中参数值的来源，可选的来源包括当前XML元素的属性或内容。
E:其它
9.NodeCreateRule 将XML文件树的一部分转换为DOM节点，并push到Digester的对象栈上。
使用Digester需要如下几个软件包：
Digester ,
BeanUtils,
Collections,
Commens-Logging，
还有一个遵循SAX(Simple API for XML)2.0或JAXP(Java API for XML Parsing) 1.1规范的XML解析器，如Xerces。
如果需要使用log4j做为日志输出，则亦请下载之。
使用者通过调用Digester类的相关方法，来创建匹配模式与规则的映射序列。
比如，调用addSetProperties(String pattern)，向Digester中加入SetPropertiesRule。
基本步骤如下:
1.创建Digester对象实例。
2.设置该Digester对象的配置属性(可选)。
3.将需要的初始对象push到该Digester对象的对象栈上(可选)。
4.需要注册所有的XML元素匹配模式与处理规则之间的映射关系。
5.用digester.parse()解析的[XML文档对象](http://baike.baidu.com/view/32559.htm)，得到目标对象。
**简单示例**
1.foo.xml 数据源文件
2.Foo.java 目标Java对象
3.Bar.java 目标Java对象
4.Entry.java 调用Digester的入口类
【foo.xml】
<?xml version="1.0" encoding="GBK"?>
<foo name="The Parent">
<bar id="123" title="The First Child"/>
<bar id="456" title="The Second Child"/>
</foo>
【Foo.java】
package org.easev.digester;
import java.util.HashMap;
import java.util.Iterator;
public class Foo {
private String name;
public void setName(String name) {
this .name = name;
}
public String getName() {
return name;
}
private HashMap bars = new HashMap();
public void addBar(Bar bar) {
bars.put(String.valueOf(bar.getId()), bar);
}
public Bar findBar(int id) {
return (Bar) bars.get(String.valueOf(id));
}
public Iterator getBars() {
return bars.keySet().iterator();
}
}
【Bar.java】
package org.easev.digester;
public class Bar {
private int id;
public int getId() {
return id;
}
public void setId(int id) {
this .id = id;
}
private String title;
public String getTitle() {
return title;
}
public void setTitle(String title) {
this.title = title;
}
}
【Entry.java】
package org.easev.digester;
import java.io.File;
import java.util.Iterator;
import org.apache.commons.digester.Digester;
public class Entry {
public static void main(String[] args) throws Exception {
//[相对路径](http://baike.baidu.com/view/25307.htm)定义与包名相关
File input = new File("org/easev/digester/foo.xml");
Digester digester = new Digester();
digester.setValidating(false);
//完整类名定义，包名改变时需做相应变化
digester.addObjectCreate("foo", "org.easev.digester.Foo");
digester.addSetProperties("foo");
digester.addObjectCreate("foo/bar", "org.easev.digester.Bar");
digester.addSetProperties("foo/bar");
digester.addSetNext("foo/bar", "addBar", "org.easev.digester.Bar");
Foo foo = (Foo) digester.parse(input);
//测试装载是否成功
Iterator iter = foo.getBars();
while (iter.hasNext()) {
System.out.println((String) iter.next());
}
}
}
匹配模式没有什么文章好做，那么下面要讨论的就必然是规则了。在基本的使用方式下，Digester虽然使用XML文件定义Java对象的状态，提高了系统的灵活性，但是匹配模式与规则的映射序列(装载逻辑)仍然通过硬编码来定义，这种方式不易修改与重用。所以Digester还提供了一种高级的使用方式，用一个XML文件定义Java对象的状态(数据源文件)，用另一个XML文件定义装载数据源文件的装载逻辑。
这样，对象的装载过程分成了两步:
1.装载逻辑的"装载"，其结果表现为定义了rule的Digester；
2.根据上一步得到的Digester，装载目标对象。
套用上面的一个例子，增加了rule.xml，并改写了Entry.java
【rule.xml】
<?xml version='1.0'?>
<!DOCTYPE digester-rules
PUBLIC "-//Jakarta Apache //DTD digester-rules XML V1.0//EN"
"file:///../digester/dtds/digester-rules.dtd">
<digester-rules>
<pattern value="foo">
<object-create-rule classname="org.easev.digester.Foo"/>
<set-properties-rule/>
<pattern value="bar">
<object-create-rule classname="org.easev.digester.Bar"/>
<set-properties-rule/>
<set-next-rule methodname="addBar"/>
</pattern>
</pattern>
</digester-rules>
【Entry.java】
package org.easev.digester;
import java.io.File;
import java.util.Iterator;
import org.apache.commons.digester.Digester;
public class Entry {
public static void main(String[] args) throws Exception {
//相对路径定义与包名相关
File data = new File("org/easev/digester/foo.xml");
File rule = new File("org/easev/digester/rule.xml");
Digester digester = DigesterLoader.createDigester(rule.toURL());
Foo foo = (Foo) digester.parse(data);
//测试装载是否成功
Iterator iter = foo.getBars();
while (iter.hasNext()) {
System.out.println((String) iter.next());
}
}
}
我们可以看到，使用Digester的代码变得相当简洁，而要付出的代价就是为装载逻辑写一个配置文件。
除了Digester之外，当然还有其它的方法来实现Java对象的绑定与装载:
1.java.util.Properties，简单的配置属性(比如数据库连接信息)，可以写在properties文件中，调用Properties对象的load(InputStream)方法将配置中的健值对加载到Properties对象中。这种方式一般仅适用于简单的配置信息的加载。
2.JAXB，Java Architecture for XML Binding，在Java Web Services Developer Pack V 1.1中提供了一个参考实现。使用这种方式时，除了提供数据源XML文件之外，还必须提供相应的Schema文件。加载前，首先用Binding Compiler将Schema转换得到目标Java类的接口与实现，然后再调用Unmarshaller或Marshaller将数据源XML文件信息加载到Java对象中，或将设置的Java对象数据导出为XML文件。
3.XPath
4.JaxMe
