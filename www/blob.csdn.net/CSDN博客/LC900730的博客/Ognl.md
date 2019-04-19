# Ognl - LC900730的博客 - CSDN博客
2017年08月05日 11:54:17[lc900730](https://me.csdn.net/LC900730)阅读数：130标签：[struts](https://so.csdn.net/so/search/s.do?q=struts&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## Ognl表达式语言
```
作用：页面取值
EL表达式语言，用于页面取值，jsp页面取值的标准(默认直接可以使用)
    (应用范围更广)
Ognl表达式语言：struts标签默认支持的表达式语言。必须配置struts标签用，不能离开struts标签直接用。
```
### OgnlContext对象
```
OgnlContext对象是ognl表达式语言的核心
```
优势： 
    1.支持对象方法调用 
    2.支持类静态方法调用和值访问，表达式的格式 
        @[类全名(包括包路径)] @[方法名|值名]，例如 
        @java.lang.String@format(‘foo %s’,’bar’) 
        或者@tutorial.MyConstrant@APP_NAME; 
    3.支持赋值操作和表达式串联 
    4.访问OGNL上下文 
    总结：OGNL有一个上下文(Context)概念，说白了上下文就是一个MAP结构，它实现了java.utils.Map的接口 
    5.操作集合对象
1.使用ognl表达式取值如果取非根元素的值，必须用# 
2.
```
OgnlContext context2=new OgnlContext();
        Object ognl2=Ognl.parseExpression("@Math@floor(10.9)");
        Object value2=Ognl.getValue(ognl2, context,context.getRoot());
        System.out.println(value2);
```
ValueStack对象 
    值栈对象
struts-Action运行过程会产生数据，最终要响应用户。因此struts把运行的数据放到ValueStack中，在页面就可以使用ValueStack
ValueStack实际上是一个接口，在Struts中利用OGNL时，实际上使用的是实现了该接口的OgnlValueStack类，这个类是Struts2利用OGNL的基础； 
ValueStack贯穿了整个Action的生命周期(每个Action类的对象实例都有一个ValueStack对象)，相当于一个数据中转站。在其中保存当前action对象和其他相关对象。Struts2框架会把ValueStack对象保存在名为struts.valueStack的request的请求属性中。 
用户每次访问struts的action都会创建一个Action对象，值栈对象、ActionContext对象，然后把Action对象放入值栈中；最后把值栈对象放入request中，传入jsp页面。 
开发者只需要通过ActionContext对象就可以访问struts的其他关键对象。(ActionContext是给开发使用)
### 拿值栈对象：
```java
public class OgnlDemo2 extends ActionSupport {
    public String execute() throws Exception{
        //获取值栈对象：方式1
        HttpServletRequest request=ServletActionContext.getRequest();
        ValueStack vs1=(ValueStack)request.getAttribute("struts.valueStack");
        //获取值栈对象：方式2
        ActionContext ac=ActionContext.getContext();
        ValueStack vs2=ac.getValueStack();
        System.out.println(vs1==vs2);     //true
        return SUCCESS;
    }
```
### 值栈对象的存储数据原理
根元素的值: 
非根元素的值：
