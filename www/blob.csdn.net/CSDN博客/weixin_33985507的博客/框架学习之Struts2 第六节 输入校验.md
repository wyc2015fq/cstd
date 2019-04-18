# 框架学习之Struts2 第六节 输入校验 - weixin_33985507的博客 - CSDN博客
2011年07月27日 08:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
1. 对输入进行校验有两种方法
方法一：手工编写代码进行校验
方法二：利用XML配置方式校验
两种方式都有对action的所有方法和指定的方法进行校验
![19_1_thumb1](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270817546752.png)
2.手工编写代码：对action的所有方法进行输入校验
![19_2_thumb1](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270817554984.png)
对action的指定方法进行输入校验
![20_thumb1](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270817566554.png)
3.输入校验的流程
需要注意的是：如果请求发生错误，进入到input视图，可能有两个原因，一个是类型转换错误，另一个是校验失败了
![21_thumb1](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270817576488.png)
4.基于XML的配置方式：对action的所有方法进行输入校验
![22_thumb1](https://images.cnblogs.com/cnblogs_com/yinger/201107/20110727081758783.png)
基于XML的配置方式：对action的指定方法进行输入校验
![23_thumb1](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270817594621.png)
5.struts2提供的校验器列表和使用例子
![22_2_thumb2](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270818005078.png)
![22_3_thumb2](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270818006648.png)
![22_4_thumb2](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270818015502.png)
6.编写校验文件时没有提供帮助信息的解决办法
![22_5_thumb2](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270818013484.png)
7.基于 XML 校验的一些特点
要注意在有多个校验作用于同一个action方法时它们的校验顺序
![23_2_thumb1](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107270818023418.png)
测试：
(1)人工编写代码校验
```
login.jsp
<body>
<s:fielderror></s:fielderror>
<s:form method="post" action="login" namespace="/yinger">
         name:<br><input name="onePerson.name" /><br>
         id:<br><input name="onePerson.id" /><br>
         mobile:<br><input name="onePerson.mobile" /><br>
<input type="submit" name="submit" value="提交" /><br>
</s:form>
</body>
```
```
HelloWorld.java action类
两个action配置中的method，两个验证这两个方法的validate方法，还有一个是验证所有的方法的validate
public String login() {
        ActionContext.getContext().getSession().put("name", onePerson.getName());
        message = "User logins!";
return "message";
    }
public String validation() {
        ActionContext.getContext().getSession().put("name", onePerson.getName());
        message = "User logins!HelloWorld.validation method!";
return "message";
    }
public void validateLogin() {
if (onePerson.getName() == null || "".equals(onePerson.getName().trim())) {
this.addFieldError("name", "名字不能为空");//name  或者   onePerson.name  都可以
        }
if (onePerson.getId() == null || "".equals(onePerson.getId().trim())
|| !Pattern.compile("^\\d{5}$").matcher(onePerson.getId().trim()).matches()) {
this.addFieldError("onePerson.id", "Id不能为空并且Id一定是5个数字");
        }
if (onePerson.getMobile() == null || "".equals(onePerson.getMobile().trim())
|| !Pattern.compile("^1[358]\\d{9}$").matcher(onePerson.getMobile().trim()).matches()) {
this.addFieldError("onePerson.mobile", "手机号不能为空并且手机号码一定要符合格式要求");
        }
    }
public void validateValidation() {
if (onePerson.getName() == null || "".equals(onePerson.getName().trim())) {
this.addFieldError("name", "名字不能为空");//name  或者   onePerson.name  都可以
        }
if (onePerson.getId() == null || "".equals(onePerson.getId().trim())
|| !Pattern.compile("^\\d{5}$").matcher(onePerson.getId().trim()).matches()) {
this.addFieldError("onePerson.id", "Id不能为空并且Id一定是5个数字");
        }
if (onePerson.getMobile() == null || "".equals(onePerson.getMobile().trim())
|| !Pattern.compile("^1[358]\\d{9}$").matcher(onePerson.getMobile().trim()).matches()) {
this.addFieldError("onePerson.mobile", "手机号不能为空并且手机号码一定要符合格式要求");
        }
    }
    @Override
public void validate() {
if (onePerson.getName() == null || "".equals(onePerson.getName().trim())) {
this.addFieldError("name", "名字不能为空");//name  或者   onePerson.name  都可以
        }
if (onePerson.getId() == null || "".equals(onePerson.getId().trim())
|| !Pattern.compile("^\\d{5}$").matcher(onePerson.getId().trim()).matches()) {
this.addFieldError("onePerson.id", "Id不能为空并且Id一定是5个数字");
        }
if (onePerson.getMobile() == null || "".equals(onePerson.getMobile().trim())
|| !Pattern.compile("^1[358]\\d{9}$").matcher(onePerson.getMobile().trim()).matches()) {
this.addFieldError("onePerson.mobile", "手机号不能为空并且手机号码一定要符合格式要求");
        }
    }
```
```
两个action
<action name="login" class="com.yinger.HelloWorld" method="login">
<result name="message">/message.jsp</result>
<result name="input">/login.jsp</result>
</action>
<action name="validation" class="com.yinger.HelloWorld" method="validation">
<result name="message">/message.jsp</result>
<result name="input">/login.jsp</result>
</action>
```
测试结果：
1.如果login.jsp的form表单的action是login，并且只有validateLogin方法开启
正确的登陆
![38](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226476894.png)
提示：用户登录了
![39](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226478256.png)
登录时 Id和手机号码错误输入
![40](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226482683.png)
提示：Id。。。
![41](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226484286.png)
登录时不输入name
![42](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226486205.png)
提示：名字不能为空
![43](https://images.cnblogs.com/cnblogs_com/yinger/201107/2011073122264976.png)
2.测试此时的 validation，将action改为validation
测试：手机号码错误输入
![44](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226491679.png)
提示：用户登陆了，并没有提示手机号码错误
![45](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226491962.png)
3.开启validate方法，关闭其他的两个方法
action是validation：可以校验
![46](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226509977.png)![47](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226501580.png)
action是login：可以校验
![48](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226506007.png)![49](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312226504022.png)
(2) 配置xml文件校验
 新增一个文件：HelloWorld-validation.xml
```
内容：验证person的三个字段
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE validators PUBLIC "-//OpenSymphony Group//XWork Validator 1.0//EN" 
    "http://www.opensymphony.com/xwork/xwork-validator-1.0.dtd">
<validators>
<field name="onePerson.name">
<field-validator type="requiredstring">
<param name="trim">true</param>
<message>名字不能为空!</message>
</field-validator>
</field>
<field name="onePerson.id">
<field-validator type="stringlength">
<param name="maxLength">5</param>
<param name="minLength">5</param>
<param name="trim">true</param>
<message><![CDATA[Id必须是5个数字]]></message>
</field-validator>
</field>
<field name="onePerson.mobile">
<field-validator type="regex">
<param name="trim">true</param>
<param name="expression"><![CDATA[^1[358]\d{9}$]]></param>
<message>手机号格式不正确!</message>
</field-validator>
</field>
</validators>
```
测试结果：测试时注释掉前面的三个validate方法
可以进行校验，至于针对某个方法的就不做测试了，估计结果和上面是一样的
![50](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312256183352.png)![51](https://images.cnblogs.com/cnblogs_com/yinger/201107/201107312256187987.png)
