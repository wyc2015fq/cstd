# JavaWeb —— 数组转字符串的 JavaBean - Alex_McAvoy的博客 - CSDN博客





2018年11月01日 23:44:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：37








在程序开发中，将数组转为字符串是经常使用的，如表单中的复选框按钮，在提交之后就是一个数组对象，由于数组对象在处理中不方便，因此实际应用中通过将其转换成字符串后再进行处理。

以下实例为将数组转换成字符串的实际应用，实现对问卷调查表单中复选框的数值的处理

1.创建 Paper 类，进行封装

```java
package javabean;

public class Paper {
    private String[] languages;
    private String[] technics;
    private String[] parts;
	
    public Paper() {
    }

    public String[] getLanguages() {
        return languages;
    }
    public void setLanguages(String[] languages) {
        this.languages = languages;
    }
    public String[] getTechnics() {
        return technics;
    }
    public void setTechnics(String[] technics) {
        this.technics = technics;
    }
    public String[] getParts() {
        return parts;
    }
    public void setParts(String[] parts) {
        this.parts = parts;
    }
}
```

2.创建将数组转换为字符串的 JavaBean 对象，在该类中编写 arr2Str() 方法，将数组对象转换为指定格式字符串，通过 for 循环遍历数组，将数组元素转换成 " , " 分隔的字符串对象，由于 String 对象不可变，每次改变长度都需新建一新的 String 对象，而 StringBuffer 是可变的字符序列，因此在字符串经常修改的地方使用 StringBuffer

```java
package javabean;

public class Convert {
    public String arr2Str(String[] arr) {
        StringBuffer sb=new StringBuffer();//StringBuffer 可变的字符序列
        if(arr!=null && arr.length>0) {//判断arr是否为有效数组
            for(String s:arr) {//遍历数组
                sb.append(s);//将字符串追加到StringBuffer中
                sb.append(",");
            }
		    
            if(sb.length()>0) {//判断字符串长度是否有效
                sb=sb.deleteCharAt(sb.length()-1);//截取字符
            }
        }
		
        return sb.toString();
    }
}
```

3.创建主页面，在页面中存放所需表单，对于属性相同的 checkbox 值，会被转换为一个数组对象

```
<%@ page language="java" contentType="text/html; charset=UTF-8" 
    pageEncoding="UTF-8" %>
<form action="reg.jsp" method="post">
    <h1>调查问卷</h1> <hr>
    <ul>
        <li>经常使用哪种编程语言开发：</li>
        <li>
            <input type="checkbox" name="languages" value="Java"> Java
            <input type="checkbox" name="languages" value="C++"> C++
            <input type="checkbox" name="languages" value="Python"> Python
            <input type="checkbox" name="languages" value="PHP"> PHP
        </li>		
    </ul>
    <ul>
        <li>目前掌握的技术：</li>
        <li>
            <input type="checkbox" name="technics" value="JSP"> Java
            <input type="checkbox" name="technics" value="C++"> C++
            <input type="checkbox" name="technics" value="Python"> Python
            <input type="checkbox" name="technics" value="PHP"> PHP
        </li>		
    </ul>	
    <ul>
        <li>在学习中哪一部分感觉有困难：</li>
        <li>
            <input type="checkbox" name="parts" value="Java"> Java
            <input type="checkbox" name="parts" value="C++"> C++
            <input type="checkbox" name="parts" value="Python"> Python
            <input type="checkbox" name="parts" value="PHP"> PHP
        </li>		
    </ul>
    <input type="submit" value="提交">
</form>
```

4.创建名为 reg.jsp 的页面，用于对 index.jsp 页面表单提交请求进行处理，将用户所提交的结果输出到页面中

```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" %>
<jsp:useBean id="paper" class="javabean.Paper"/>
<jsp:useBean id="convert" class="javabean.Convert"/>
<jsp:setProperty property="*" name="paper"/>
<h1>调查结果</h1> <hr/>
<ul>
    <li> 经常使用的编程语言：<%= convert.arr2Str(paper.getLanguages()) %> </li>
    <li> 目前掌握的技术：<%= convert.arr2Str(paper.getTechnics()) %> </li>
    <li> 有困难的部分：<%= convert.arr2Str(paper.getParts()) %> </li>
</ul>
```

运行界面：

![](https://img-blog.csdnimg.cn/20181216223402760.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181216223416691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)



