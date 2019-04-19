# SpringMVC配置返回pdf/json/xml等不同的视图 - 零度的博客专栏 - CSDN博客
2016年05月31日 17:12:22[零度anngle](https://me.csdn.net/zmx729618)阅读数：2264
个人分类：[SpringMVC](https://blog.csdn.net/zmx729618/article/category/6248665)
       spring mvc通过使用org.springframework.web.servlet.view.ContentNegotiatingViewResolver，就可以根据不同的场景返回不同的view了，比如返回json,xml,pdf等不同视图。代码实现如下： 
先是一个简单的pojo 
Java代码  
- package com.hmkcode.vo;  
- 
- import javax.xml.bind.annotation.XmlRootElement;  
- 
- @XmlRootElement(name = "person")  
- publicclass Person {  
- 
- privateint id;  
- private String name;  
- publicint getId() {  
- return id;  
-     }  
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- public String toString(){  
- return"ID: "+id+" - Name: "+name;  
-     }  
- 
- }  
   注意使用@XmlRootElement标签之。 
然后是controller: 
Java代码  
- @Controller
- publicclass MyController {  
- 
- @RequestMapping("/view")  
- public Person getPerson(Model model){  
-         Person person = new Person();  
-         person.setId(1);  
-         person.setName("test");  
-         model.addAttribute("person",person);  
- return person;  
-     }  
  然后分别假设要导出为PDF的view，引入著名的itext了； 
Java代码  
- publicclass PdfView extends AbstractPdfView {  
- 
- protectedvoid buildPdfDocument(  
-         Map model,  
-         Document doc,  
-         PdfWriter writer,  
-         HttpServletRequest req,  
-         HttpServletResponse resp)  
- throws Exception {  
- 
- 
-         Person person = (Person) model.get("person");  
-         doc.add( new Paragraph(person.toString()));  
- 
-     }  
   这里演示简单往itext中写入一个输出而已； 
然后是web.xml: 
Java代码  
- <servlet>  
-         <servlet-name>springmvc</servlet-name>  
-         <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
-         <load-on-startup>1</load-on-startup>  
-     </servlet>  
- 
-     <servlet-mapping>  
-         <servlet-name>springmvc</servlet-name>  
-         <url-pattern>/rest/*</url-pattern>  
-     </servlet-mapping>  
  最核心的配置来了： 
Java代码  
-    <bean class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">  
- <property name="order" value="1" />  
- <property name="mediaTypes">  
-   <map>  
-      <entry key="json" value="application/json" />  
-      <entry key="xml" value="application/xml" />  
-      <entry key="pdf" value="application/pdf" />  
- 
-   </map>  
- </property>  
- 
- <property name="defaultViews">  
-   <list>  
-     <!-- JSON View -->  
-     <bean  
- class="org.springframework.web.servlet.view.json.MappingJacksonJsonView">  
-     </bean>  
-     <!--  XML view -->  
-       <bean class="org.springframework.web.servlet.view.xml.MarshallingView">  
-       <constructor-arg>  
-           <bean class="org.springframework.oxm.jaxb.Jaxb2Marshaller">  
-              <property name="classesToBeBound">  
-               <list>  
-                  <value>com.test.Person</value>  
-               </list>  
-              </property>  
-           </bean>  
-       </constructor-arg>  
-     </bean>  
-     <!--  PDF view -->  
-     <bean class="com.test.PdfView">  
- 
-       </bean>  
-    </list>  
- </property>  
- <property name="ignoreAcceptHeader" value="true" />  
- 
- t;/bean>  
   可以看到，其中使用的是： org.springframework.web.servlet.view.ContentNegotiatingViewResolver，对其pdf,xml.json进行分别的view设置，JSON用的是著名的jackyson去解析了； 最后，通过rest风格的地址，比如[http://localhost/view/view.json](http://localhost/view/view.json),返回json； 通过[http://localhost/view/view.pdf](http://localhost/view/view.pdf),则导出到pdf,http://localhost/view/view.xml， 返回前端就xml了，十分方便。
