# 基于Spring可扩展Schema提供自定义配置支持 - z69183787的专栏 - CSDN博客
2016年05月04日 06:24:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：565
个人分类：[Spring-Schema自定义																[Spring-注入&注解&扫描](https://blog.csdn.net/z69183787/article/category/7066579)](https://blog.csdn.net/z69183787/article/category/7065889)

在很多情况下，我们需要为系统提供可配置化支持，简单的做法可以直接基于[spring](http://lib.csdn.net/base/javaee)的标准Bean来配置，但配置较为复杂或者需要更多丰富控制的时候，会显得非常笨拙。一般的做法会用原生态的方式去解析定义好的xml文件，然后转化为配置对象，这种方式当然可以解决所有问题，但实现起来比较繁琐，特别是是在配置非常复杂的时候，解析工作是一个不得不考虑的负担。Spring提供了可扩展Schema的支持，这是一个不错的折中方案，完成一个自定义配置一般需要以下步骤：
- 设计配置属性和JavaBean
- 编写XSD文件
- 编写NamespaceHandler和BeanDefinitionParser完成解析工作
- 编写spring.handlers和spring.schemas串联起所有部件
- 在Bean文件中应用
下面结合一个小例子来实战以上过程
**1）设计配置属性和JavaBean**
首先当然得设计好配置项，并通过JavaBean来建模，本例中需要配置People实体，配置属性name和age（id是默认需要的）
**[[Java](http://lib.csdn.net/base/javaee)]**[view
 plain](http://blog.csdn.net/cutesource/article/details/5864562#)[copy](http://blog.csdn.net/cutesource/article/details/5864562#)
- publicclass People {  
- private String id;  
- private String name;  
- private Integer age;  
- }  
**2）编写XSD文件**
为上一步设计好的配置项编写XSD文件，XSD是schema的定义文件，配置的输入和解析输出都是以XSD为契约，本例中XSD如下：
**[xhtml]**[view
 plain](http://blog.csdn.net/cutesource/article/details/5864562#)[copy](http://blog.csdn.net/cutesource/article/details/5864562#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <xsd:schema
- xmlns="http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/cutesource/schema/people"
- xmlns:xsd="http://www.w3.org/2001/XMLSchema"
- xmlns:beans="http://www.springframework.org/schema/beans"
- targetNamespace="http://blog.csdn.net/cutesource/schema/people"
- elementFormDefault="qualified"
- attributeFormDefault="unqualified">
- <xsd:importnamespace="http://www.springframework.org/schema/beans"/>
- <xsd:elementname="people">
- <xsd:complexType>
- <xsd:complexContent>
- <xsd:extensionbase="beans:identifiedType">
- <xsd:attributename="name"type="xsd:string"/>
- <xsd:attributename="age"type="xsd:int"/>
- </xsd:extension>
- </xsd:complexContent>
- </xsd:complexType>
- </xsd:element>
- </xsd:schema>
关于xsd:schema的各个属性具体含义就不作过多解释，可以参见http://www.w3school.com.cn/schema/schema_schema.asp
<xsd:element name="people">对应着配置项节点的名称，因此在应用中会用people作为节点名来引用这个配置
<xsd:attribute name="name" type="xsd:string" />和<xsd:attribute name="age" type="xsd:int" />对应着配置项people的两个属性名，因此在应用中可以配置name和age两个属性，分别是string和int类型
完成后需把xsd存放在classpath下，一般都放在META-INF目录下（本例就放在这个目录下）
**3）编写NamespaceHandler和BeanDefinitionParser完成解析工作**
下面需要完成解析工作，会用到NamespaceHandler和BeanDefinitionParser这两个概念。具体说来NamespaceHandler会根据schema和节点名找到某个BeanDefinitionParser，然后由BeanDefinitionParser完成具体的解析工作。因此需要分别完成NamespaceHandler和BeanDefinitionParser的实现类，Spring提供了默认实现类NamespaceHandlerSupport和AbstractSingleBeanDefinitionParser，简单的方式就是去继承这两个类。本例就是采取这种方式：
**[java]**[view
 plain](http://blog.csdn.net/cutesource/article/details/5864562#)[copy](http://blog.csdn.net/cutesource/article/details/5864562#)
- import org.springframework.beans.factory.xml.NamespaceHandlerSupport;  
- publicclass MyNamespaceHandler extends NamespaceHandlerSupport {  
- publicvoid init() {  
-         registerBeanDefinitionParser("people", new PeopleBeanDefinitionParser());  
-     }  
- }  
其中registerBeanDefinitionParser("people", new PeopleBeanDefinitionParser());就是用来把节点名和解析类联系起来，在配置中引用people配置项时，就会用PeopleBeanDefinitionParser来解析配置。PeopleBeanDefinitionParser就是本例中的解析类：
**[java]**[view
 plain](http://blog.csdn.net/cutesource/article/details/5864562#)[copy](http://blog.csdn.net/cutesource/article/details/5864562#)
- import org.springframework.beans.factory.support.BeanDefinitionBuilder;  
- import org.springframework.beans.factory.xml.AbstractSingleBeanDefinitionParser;  
- import org.springframework.util.StringUtils;  
- import org.w3c.dom.Element;  
- publicclass PeopleBeanDefinitionParser extends AbstractSingleBeanDefinitionParser {  
- protected Class getBeanClass(Element element) {  
- return People.class;  
-     }  
- protectedvoid doParse(Element element, BeanDefinitionBuilder bean) {  
-         String name = element.getAttribute("name");  
-         String age = element.getAttribute("age");  
-         String id = element.getAttribute("id");  
- if (StringUtils.hasText(id)) {  
-             bean.addPropertyValue("id", id);  
-         }  
- if (StringUtils.hasText(name)) {  
-             bean.addPropertyValue("name", name);  
-         }  
- if (StringUtils.hasText(age)) {  
-             bean.addPropertyValue("age", Integer.valueOf(age));  
-         }  
-     }  
- }  
其中element.getAttribute就是用配置中取得属性值，bean.addPropertyValue就是把属性值放到bean中。
**4）编写spring.handlers和spring.schemas串联起所有部件**
上面几个步骤走下来会发现开发好的handler与xsd还没法让应用感知到，就这样放上去是没法把前面做的工作纳入体系中的，spring提供了spring.handlers和spring.schemas这两个配置文件来完成这项工作，这两个文件需要我们自己编写并放入META-INF文件夹中，这两个文件的地址必须是META-INF/spring.handlers和META-INF/spring.schemas，spring会默认去载入它们，本例中spring.handlers如下所示：
http/://blog.csdn.net/cutesource/schema/people=study.schemaExt.MyNamespaceHandler
以上表示当使用到名为"http://blog.csdn.net/cutesource/schema/people"的schema引用时，会通过study.schemaExt.MyNamespaceHandler来完成解析
spring.schemas如下所示：
http/://blog.csdn.net/cutesource/schema/people.xsd=META-INF/people.xsd
以上就是载入xsd文件
**5）在Bean文件中应用**
到此为止一个简单的自定义配置以完成，可以在具体应用中使用了。使用方法很简单，和配置一个普通的spring bean类似，只不过需要基于我们自定义schema，本例中引用方式如下所示：
**[xhtml]**[view
 plain](http://blog.csdn.net/cutesource/article/details/5864562#)[copy](http://blog.csdn.net/cutesource/article/details/5864562#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:cutesource="http://blog.csdn.net/cutesource/schema/people"
- xsi:schemaLocation="  
- http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
- http://blog.csdn.net/cutesource/schema/people http://blog.csdn.net/cutesource/schema/people.xsd">
- <cutesource:peopleid="cutesource"name="袁志俊"age="27"/>
- </beans>
其中xmlns:cutesource="http://blog.csdn.net/cutesource/schema/people"是用来指定自定义schema，xsi:schemaLocation用来指定xsd文件。<cutesource:people id="cutesource" name="zhijun.yuanzj" age="27"/>是一个具体的自定义配置使用实例。
最后就可以在具体程序中使用基本的bean载入方式来载入我们的自定义配置对象了，如：
**[java]**[view
 plain](http://blog.csdn.net/cutesource/article/details/5864562#)[copy](http://blog.csdn.net/cutesource/article/details/5864562#)
- ApplicationContext ctx = new ClassPathXmlApplicationContext("application.xml");  
- People p = (People)ctx.getBean("cutesource");  
- System.out.println(p.getId());  
- System.out.println(p.getName());  
- System.out.println(p.getAge());  
会输出：
cutesource
袁志俊
27
以上就是一个基于Spring可扩展Schema提供自定义配置支持实战过程，一些复杂应用和技巧还有待挖掘
