# JavaWeb —— JavaBean 基础 - Alex_McAvoy的博客 - CSDN博客





2018年11月01日 23:43:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41








# 【概述】

在 JavaWeb 开发过程中，为避免代码混乱，便于维护，JSP 页面应避免出现 Java 代码，因此令 HTML 代码与 Java 代码相分离，将 Java 代码单独封装成为一个处理某种业务逻辑的类，然后在 JSP 页面中通过 JSP 动作标签来调用相关的类，从而降低 HTML 代码与 Java 代码的耦合度，简化 JSP 页面，提高 Java 程序代码的重用性与灵活性。

这种与 HTML 代码相分离，使用 Java 代码封装的类，就是一个 JavaBean 组件。

JavaBean 常与 JSP 动作 <jsp:useBean>、<jsp:getProperty>、<jsp:setPropety> 结合使用。

# 【JavaBean 格式】
- 用作 JavaBean 的类必须有一个公共、无参的构造器
- JavaBean 用于对属性赋值的方法称为属性修改器（setter 方法），用于读取属性值的方法称为属性访问器（getter 方法）
- 属性修改器以小写的 set 为前缀，后跟属性名，属性名的首字母为大写；属性访问器以小写的 get 为前缀，后跟属性名，属性名的首字母为大写

```java
public class Produce {
    //属性
    private String name;
    private double price;
    //无参构造器
    public Produce(){
    }
    //getter方法
    public String getName() {
        return name;
    }
    public double getPrice() {
        return price;
    }
    //setter方法
    public void setName(String name) {
        this.name = name;
    }
    public void setPrice(double price) {
        this.price = price;
    }
}
```

# 【具体应用】

## 1.获取属性信息

在 JavaBean 对象中，为防止外部直接对 JavaBean 属性调用，常将 JavaBean 中的属性设为 private，但需为其提供 public 的访问方法，也就是常说的 getXXX() 方法。

1）创建所需的类，在类中创建 private 及其相应的 public 的 getXXX() 方法

2）在 JSP 页面中通过 JSP 动作标签获取 JavaBean 中的属性信息

```
名称：<jsp:getProperty property="name" name="produce"/> <br>
价格：<jsp:getProperty property="price" name="produce"/> <br>
数量：<jsp:getProperty property="count" name="produce"/> <br>
```

## 2.对 JavaBean 属性赋值

编写 JavaBean 要遵循 JavaBean 规范，在 JavaBean 规范中的访问器 setXXX() 方法，用于对 JavaBean 中的属性赋值，若对 JavaBean 对象的属性提供了 setXXX() 方法，在 JSP 页面中使用 <jsp:setProperty> 对其进行赋值。

1）创建所需的类，在类中创建 private 及其相应的 public 的 setXXX() 方法

2）在 JSP 页面中通过 JSP 动作标签修改 JavaBean 中的属性信息

```
<jsp:useBean id="produce" class="javabean.Produce">
    <jsp:setProperty name="produce" property="*"/>
</jsp:useBean>
```

## 3.生命周期

在 JSP 页面中使用 JavaBean，其生命周期可以自行设置，存在于 4 种范围中：page、request、session、application，默认情况下，JavaBean 的生存周期作用于 page 范围内。
`<jsp:useBean id="produce" class="javabean.Produce" scope="session" />`




