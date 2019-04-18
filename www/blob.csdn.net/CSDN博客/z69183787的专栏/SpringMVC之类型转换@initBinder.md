# SpringMVC之类型转换@initBinder - z69183787的专栏 - CSDN博客
2016年08月20日 14:25:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2489
## 1. 摘要
类型转换器常用于转换double、float、date等类型。
上文讲解了Converter类型转换器，这属于Spring 3新支持的类型转换器；
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054549242096.png)
本文主要介绍@InitBinder，可参考如下链接：
参考： [http://blog.csdn.net/axin66ok/article/details/17938095](http://blog.csdn.net/axin66ok/article/details/17938095)
功能说明：
当表单提交double、date等类型时，我们需要将其转换为java可识别的date，double等，如在浏览器中输入：
[http://localhost:8080/SpringMVCTest/test/conversionTest.action?person=zhangsan:666:ssss:3.1415](http://localhost:8080/SpringMVCTest/test/conversionTest.action?person=zhangsan:666:ssss:3.1415) 
 时，
需要将其转化到Person对象中，此时需要对double类型进行转换。
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054555808495.png)
## 2. 定义转换器类
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054562202665.png)
- - `package com.ll.model;`
- ``
- `import java.beans.PropertyEditorSupport;`
- ``
- `public class PersonEditor extends PropertyEditorSupport {`
- ``
- `	@Override`
- `	public void setAsText(String text) throws IllegalArgumentException {`
- `		Person p = new Person();`
- `		if (text != null) {`
- `			String[] items = text.split(":");`
- `			p.setUsername(items[0]+"by propertyeEditor");`
- `			p.setPasswd(items[1]);`
- `			p.setRealName(items[2]);`
- `			p.setPrice(Double.parseDouble(items[3]));`
- `		}`
- `		setValue(p);`
- `	}`
- `	`
- `	@Override`
- `	public String getAsText() {`
- `		return getValue().toString();`
- `	}   `
- `}`
## 3. 注册自定义编辑器
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054569552508.png)
## 4. 类型转换测试
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054577208406.png)
在浏览器中输入：
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054583142293.png)
类型转换器会自动将"person=zhangsan:666:ssss:3.1415"转换为Person对象；
![技术分享](http://images0.cnblogs.com/blog/731047/201506/251054589396934.png)
