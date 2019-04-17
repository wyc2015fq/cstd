# JavaWeb —— 自定义标签 - Alex_McAvoy的博客 - CSDN博客





2019年01月17日 21:11:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：136








# 【概述】

自定义标签是用户自定义的 JSP 标记。当一个含有自定义标签的 JSP 页面被 JSP 引擎编译成 Servlet 时，tag 标签被转换成了一个被称为**标签处理类**的对象。因此，当 JSP 页面被 JSP 引擎转换成 Servlet 后，实际上 tag 标签的操作被转换成了对**标签处理类**的操作。

标签库 API 定义在 javax.servlet.jsp.tagext 包中

![](https://img-blog.csdnimg.cn/20190219193439449.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

开发自定义标签，核心就是编写处理器类，一个标签对应一个标签处理器类，一个标签库就是许多标签处理器的集合。所有的标签处理器类都要实现 JspTag 接口，该接口中没有定义任何方法，主要是作为 Tag 和 SimpleTag 接口的父接口。

在 JSP2.0以前，所有的标签处理器都要实现 Tag 接口，这样的标签称为传统标签，而在 JSP2.0 规范中，定义了一种新类型的标签，其对应的处理器要实现 SimpleTag 接口，这样的标签称为简单标签。

# 【SimpleTag 接口】
- setJspContext() 方法：JSP 引擎自动调用，将代表 JSP 页面的 pageContext 对象传入
- setParent() 方法：将当前标签的父标签处理器对象传递给当前标签处理器对象
- getParent() 方法：用于获取当前标签的父标签处理器对象
- setJspBody() 方法：将封装了标签体的 JspFragment 对象传递给标签处理器对象
- doTag() 方法：标签体逻辑实际执行方法，用于完成所有的标签逻辑，可以抛出 javax.servlet.jsp.SkipPageException 异常，用于通知 Web 容器不再执行 JSP 页面中位于结束标记后面的内容
- 

![](https://img-blog.csdnimg.cn/20190219194826516.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

```java
public class Test implements SimpleTag {

    @Override
    public void doTag() throws JspException, IOException {
    }

    @Override
    public JspTag getParent() {
        return null;
    }

    @Override
    public void setJspBody(JspFragment arg0) {
    }

    @Override
    public void setJspContext(JspContext arg0) {
    }

    @Override
    public void setParent(JspTag arg0) {
    }

}
```

# 【SimpleTagSupport 类】

由于每次都要实现 SimpleTag 接口过于麻烦，因此可直接继承其简单的实现类 ，每次在自定义标签中，根据需要设置标签属性与 setXXX() 方法，并实现 doTag() 方法即可

```java
public class SimpleTagSupport implements SimpleTag {
    private JspTag parentTag;
    private JspContext jspContext;
    private JspFragment jspBody;
	
    public void doTag() throws JspException, IOException {

    }

    public void setParent(JspTag parent) {
        this.parentTag=parent;
    }
    public JspTag getParent() {
        return this.parentTag;
    }

    public void setJspContext(JspContext pc) {
        this.jspContext=pc;
    }
    public JspContext getJspContext() {
        return this.jspContext;
    }

    public void setJspBody(JspFragment jspBody) {
        this.jspBody=jspBody;
    }
    public JspFragment getJspBody() {
        return this.jspBody;
    }
}
```

# 【自定义标签的开发与应用步骤】
- 编写完成标签功能的 Java 类（标签处理器），要求实现 SimpleTag 接口或继承 SimpleTagSupport 类
- 编写标签库描述文件（tld 文件），在 tld 文件中对自定义中进行描述，需放置在 WEB-INF 文件夹下
- 在 JSP 页面中导入、使用自定义标签

TLD 文件的描述部分

```
<!-- 描述tld文件 -->
<description>描述</description>
<display-name>名称</display-name>
<tlib-version>版本</tlib-version>

<!-- 建议在JSP页面上使用的标签前缀 -->
<short-name>test</short-name>
<!-- 作为tld文件的id，用于唯一标识当前tld文件 -->
<uri>http://www.test.com/mytag/core</uri>

<!-- 描述自定义的空标签 -->
<tag>
    <!-- 标签名 -->
    <name>test</name>
    <!-- 标签所在类 -->
    <tag-class>com.tag.Test</tag-class>
    <!-- 指定标签体类型，可能取值有3种
        empty：没有标签体
        scriptless：标签体可含EL表达式与JSP动作元素，但不能含JSP脚本元素
        tagdependent：标签体交由标签本身去解析处理，其中所有内容会原封不动的交由标签处理器
     -->
    <body-content>empty<body-content>

    <!-- 描述标签的属性 -->
    <attribute>
        <name>属性名</name>
        <!-- 该属性是否为必需 -->
        <required>true</required>
        <!-- runtime expression value 运行时表达式的值
             当前属性是否可以接受运行时表达式的动态值
         -->
        <rtexprvalue>true</rtexprvalue>
    </attribute>

</tag>
```

# 【三种自定义标签】

## 1.带有属性的自定义标签
- 在 tld 中需要使用 <attribute> 来描述标签的属性
- 在标签处理器中定义相关属性与 setXXX() 方法

 例如：定制一个带有两个属性的标签<max>，用于计算并输出两个数的最大值

```java
public class MaxTag extends SimpleTagSupport {
    private String num1;
    private String num2;
    public void setNum1(String num1) {
        this.num1=num1;
    }
    public void setNum2(String num2) {
        this.num2=num2;
    }

    @Override
    public void doTag() throws JspException, IOException {
        PageContext pageContext=(PageContext)getJspContext();
        
        int a=Integer.parseInt(num1);
        int b=Integer.parseInt(num2);
        
        pageContext.getOut().print(a>b?a:b);
    }
}
```

## 2.带有标签体的自定义标签
- 在自定义的标签处理器中使用 JspFragment 对象封装标签体信息
- 若配置的标签含有标签体，则 JSP 引擎会自动调用 setJspBody() 方法将 JspFragment 对象传递给标签处理器
- 在 SimpleTagSupport 类中定义了 getJspBody() 方法，用于返回 JspFragment 对象
- JspFragment 对象中的 **invoke(JspWriter writer) **方法将标签体中的内容从 writer 中输出

	若为 null，等同于 invoke(getJspContext().getOut()) 中，即直接将标签体内容输出到页面上
- 在 tld 文件中，使用 <body-content> 节点来描述标签体类型，一般取值为 scriptless

例如：订制一个标签，其有属性 time 与标签体，要求将标签体变为大写后输出 time 次

```java
public class MaxTag extends SimpleTagSupport {
    private String time;
    public void setNum1(String time) {
        this.time=time;
    }

    @Override
    public void doTag() throws JspException, IOException {
        PageContext pageContext=(PageContext)getJspContext();
        JspFrament bodyContent=getJspBody();

        StringWriter sw=new StringWriter();
        String content=sw.toString().toUpperCase();//转为大写
        bodyContent.invoke(sw);

        int count=Integer.parseInt(time);//获取time属性
        for(int i=1;i<=count;i++)
            pageContext.getOut().print(content);
    }
}
```

## 3.带有父标签的自定义标签
- 父标签的类型是 JspTag 类型，其是一个空接口，用于统一 Tag 与 SimpleTag，实际使用时，进行类型强制转换即可
- 父标签无法获取子标签的应用，父标签仅将子标签作为标签体使用；子标签可以通过 getParent() 方法来获取父标签的引用，JSP 引擎会将父标签的引用通过 setParent() 方法赋给标签处理器
- 在 tld 配置文件中，无需为父标签有额外的配置，但子标签是以标签体的形式存在的，所以父标签的 <body-content> 需要设置为 scriptless

例如：父标签打印 name 到控制台，子标签将父标签的 name 属性打印到 JSP 页面上

```java
//父标签实现类
public class ParentTag extends SimpleTagSupport{
    private String name;
    public void setName(String name){
        this.name=name;
    }
    publict String getName(){
        return name;
    }
    @Override
    public void doTag() throws JspException, IOException{
        System.out.println("父标签："+name);
        getJspBody().invoke(null);
    }
}
//子标签实现类
public class SonTag extends SimpleTagSupport{
    @Override
    public void doTag() throws JspException, IOException{
        //得到父标签的引用
        JspTag parent=getParent();
        //获取父标签的name属性
        ParentTag parentTag=(ParentTag) parent;
        String name=parentTag.getName();
        //将name打印到JSP页面上
        getJspContext().getOut.print("子标签："+name);
    }
}
```





