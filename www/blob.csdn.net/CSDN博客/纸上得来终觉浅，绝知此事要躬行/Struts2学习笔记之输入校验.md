# Struts2学习笔记之输入校验 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年09月01日 21:53:51[boonya](https://me.csdn.net/boonya)阅读数：1663标签：[struts																[action																[string																[input																[mobile](https://so.csdn.net/so/search/s.do?q=mobile&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记

**第十一记：struts2输入校验**

注：action内针对所有方法或指定方法名的校验。如果有验证方法必须在struts.xml中设置input视图,当未通过验证返回名称为input的视图


**1、struts2的输入校验方式**

  方式一：采用手工编码实现

  方式二：基于XML配置实现

**2、采用手工编码实现 **

**  2-1、编写JSP表单输入:change.jsp**

  <from action="${pageContext.request.contextPath}/person/person_save.action"method="post">

   用户名：<input name="username" type="text"/>

  手机号：<input name="mobile"type="text"/>

  <input  type="submit"value="提交"/>

  </from>

** 2-2、编写Action的方法**

    该action继承自ActionSupport类，并且需要重写validate方法。

   public  class PersonAction extends ActionSupport{

           private  String username;

           private  String mobile;

           public String getUsername(){

                  return this.message;

           }

          public void setUsername(String username){

                 this.username=username;

          }

          public String getMobile(){

                  return this.mobile;

           }

          public void setMobile(String mobile){

                 this.mobile=mobile;

          }

         publicString  save( ){

                    ActionContext.getContext.put("message","保存成功！");

                    return "message";

          }

         public String  update( ){

                    ActionContext.getContext.put("message","更新成功！");

                     return "message";

          }

        @override//此方法会对action中的所有方法进行校验

         public  void   validate(){

                 if(this.username==null||"".equals(this.username.trim())){

                      this.addFieldError("username","用户名不能为空");

                }

                if(this.mobile==null||"".equals(this.username.trim())){

                       this.addFieldError("mobile","手机号不能为空");

               }else{

                      if(!Pattern.compile("^1[358]\\d${9}")).matcher(this.mobile).method()){

                                this.addFieldError("mobile","手机号格式不正确");

                      }

               }

         }

  }

**   2-3、导入struts标签库并设置错误结果显示标签**

     <%@  taglib  uri="struts-tags"  prefix="s" %>

   在表单外部或者内部添加错误显示标签：<s:fielderror />

**  2-4、修改struts.xml文件**

<package name="person" namespace="/person" extends="struts default">

    <action   name="person_*" class="com.struts.action.PersonAction">

             <result  name="input">/WEB-INF/page/change.jsp</result>

             <result  name="message">/WEB-INF/page/message.jsp</result>

    </action>

    </package>

**  2-5、控制指定方法的校验**


    规则：名称如validateXxx的方法表示对xxx方法的校验。


   如果将validate方法改为validateSave则只能对save方法进行校验。


**  2-6、校验过程梳理**


      2-6-1、类型转换：对action对象的属性赋值


      2-6-2、检测类型转换异常，如果发现则通过ActionContext和ConversionError对象添加异常信息到fieldErrors对象


      2-6-3、执行validateXxx()方法。


      2-6-4、执行validate()方法。


      2-6-5、读取fieldErrors对象，如果不为空则转到Input视图，否则执行action操作。


  注：这里发生的错误多半是类型转换。


**3、XML配置校验**


**3-1、验证文件的命名和验证规则**

实现action类同样需要继承自ActionSupport，需要验证的action的配置文件于struts.xml都在src目录下，并且该xml验证文件的名称是ActionName-validation.xml


"*-validation.xml"是固定写法；*是action的名称，如PersonAction的验证文件是PersonAction-validation.xml.


xwork目录下路径;com.opensymphony.xwork2.validator.validators有个default.xml文件提供了基本数据验证的默认验证。


default.xml的配置如下所示:


<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE validators PUBLIC

"-//OpenSymphony Group//XWork Validator Config 1.0//EN"

  "http://www.opensymphony.com/xwork/xwork-validator-config-1.0.dtd">

<!-- START SNIPPET: validators-default -->

<validators>

    <validator name="required" class="com.opensymphony.xwork2.validator.validators.RequiredFieldValidator"/>

    <validator name="requiredstring" class="com.opensymphony.xwork2.validator.validators.RequiredStringValidator"/>

    <validator name="int" class="com.opensymphony.xwork2.validator.validators.IntRangeFieldValidator"/>

    <validator name="long" class="com.opensymphony.xwork2.validator.validators.LongRangeFieldValidator"/>

    <validator name="short" class="com.opensymphony.xwork2.validator.validators.ShortRangeFieldValidator"/>

    <validator name="double" class="com.opensymphony.xwork2.validator.validators.DoubleRangeFieldValidator"/>

    <validator name="date" class="com.opensymphony.xwork2.validator.validators.DateRangeFieldValidator"/>

    <validator name="expression" class="com.opensymphony.xwork2.validator.validators.ExpressionValidator"/>

    <validator name="fieldexpression" class="com.opensymphony.xwork2.validator.validators.FieldExpressionValidator"/>

    <validator name="email" class="com.opensymphony.xwork2.validator.validators.EmailValidator"/>

    <validator name="url" class="com.opensymphony.xwork2.validator.validators.URLValidator"/>

    <validator name="visitor" class="com.opensymphony.xwork2.validator.validators.VisitorFieldValidator"/>

    <validator name="conversion" class="com.opensymphony.xwork2.validator.validators.ConversionErrorFieldValidator"/>

    <validator name="stringlength" class="com.opensymphony.xwork2.validator.validators.StringLengthFieldValidator"/>

    <validator name="regex" class="com.opensymphony.xwork2.validator.validators.RegexFieldValidator"/>

    <validator name="conditionalvisitor" class="com.opensymphony.xwork2.validator.validators.ConditionalVisitorFieldValidator"/>

</validators>

<!-- END SNIPPET: validators-default -->

-校验规则：

required:必填校验器

requiredstring:必填字符串校验器

int:整数校验器

double:双精度浮点数校验器

date:日期校验器

expression:表达式校验器

fieldexpression:字段表达式校验器

email:电子邮件校验器

url:网址校验器

visitor:Visitor校验器

conversion:转换校验器

stringlength:字符串长度校验器

  regex:正则表达式校验器


**3-2、struts2例子提供的配置校验的编写示例**

路径：/struts_2.2.1.1/apps/blank/src/main/resources/example/login-validation.xml

<!DOCTYPE validators PUBLIC

"-//OpenSymphony Group//XWork Validator 1.0.2//EN"

         "http://www.opensymphony.com/xwork/xwork-validator-1.0.2.dtd"> 

<validators> 

      <field name="username"> 

            <field-validator type="requiredstring"> 

                <message key="requiredstring"/> 

         </field-validator> 

       </field> 

       <field name="password"> 

              <field-validator type="requiredstring"> 

                   <message key="requiredstring"/> 

           </field-validator> 

        </field> 

</validators>
















**3-3、编写自己的验证文件**

<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE validators PUBLIC "-//OpenSymphony Group//XWork Validator 1.0.2//EN"

"http://www.opensymphony.com/xwork/xwork-validator-1.0.2.dtd">

 <validators>

  <!-- 验证用户名是否合法 -->

     <field name="username">

         <field-validator type="requiredstring">

         <param name="trim">true</param>

         <message>请填写用户名</message>

      </field-validator>

     <field-validator type="stringlength"> 

          <param name="minLength">4</param>

         <param name="maxLength">32</param> 

         <message>用户名长度应在4到32个字符间</message> 

   </field-validator>

</field>

<!-- 验证密码是否合法 -->

   <field name="password">

      <field-validator type="requiredstring">

          <message>请填写密码</message>

    </field-validator>

     <field-validator type="stringlength"> 

        <param name="minLength">6</param>

        <param name="maxLength">32</param> 

         <message>为了您账号的安全，请设置6个字母以上的密码（最长可设置32个字母）</message> 

    </field-validator>

</field>

<!-- 验证手机号是否合法 -->

<validators>

   <field name="user">

     <field-validator type="regex">

        <param name="expression_r"><![CDATA[^1[358]\d{9}$]]></param>

        <message>不是合法的手机号</message>

    </field-validator>

   </field>

</validators>


</validators>

**3-4、常用的验证配置3-4-1、必填检验**

<validators>

     <field name="username">

       <field-validator type="required">

             <message>指定检验失败的提示信息</message>

       </field-validator>

    </field>

</validators>

3-4-2、必填字符串检验

<validators>

   <field name="username">

         <field-validator type="requiredstring">

                <param name="trim">true</param>

              <message>指定检验失败的提示信息</message>

        </field-validator>

    </field>

</validators>
3-4-3、整数检验器/浮点检验

<validators>

    <field name="age">

       <field-validator type="int">

           <param name="min">1</param>

          <param name="max">150</param>

          <message>年纪必须在1到150之间</message>

       </field-validator>

    </field>

</validators>
3-4-4、日期检验

<validators>

 <field name="birth">

     <field-validator type="date">

         <param name="min">1900-01-01</param>

         <param name="max">2050-02-21</param>

        <message key="birth.range"/>

    </field-validator>

 </field>

</validators>
**3-4-5、字段表达式检验器(要求指定字段满足一个逻辑表达式**

<validators>

    <field name="re_pass">

        <field-validator type="fieldexpression">

        <!--指定逻辑表达式 -->

             <param name="expression"> (pass eq re_pass)</param>

             <message>密码必须和确认密码相等</message>

       </field-validator>

     </field>

</validators>
3-4-6、邮件地址校验

<validators>

  <field name="email">

       <field-validator type="email">

          <message>你的电子邮件地址必须是一个有效的电邮地址</message>

        </field-validator>

   </field>

</validators>
3-4-7、网址**检验**

<validators>

    <field name="url">

        <field-validator type="url">

             <message>你的主页地址必须是一个有效的网址</message>

       </field-validator>

    </field>

</validators>
**3-4-8、字符串长度检验**

<validators>

   <field name="user">











       <field-validator type="stringlength">

          <param name="minlength">4</param>

         <param name="maxlength">20</param>

         <message>你的用户名长度必须在4到20之间</message>

      </field-validator>

   </field>

</validators>
**3-4-9、正则表达式检验**

<validators>

   <field name="user">

     <field-validator type="regex">

        <param name="expression_r"><![CDATA[(\w{4,25})]]></param>

        <message>您输入的用户名只能是字母和数组，且长度必须在4到25之间</message>

    </field-validator>

   </field>

</validators>

**3-5、配置指定方法的校验**

文件命名规则：ActionClassName-ActonName-validation.xml

如在PersionAction中访问update的方法验证,则可以写成:

PersonAction-person_update-validation.xml

struts.xml的配置如下：

<action name="person_*" class="com.struts.action.PersonAction">

<result name="input">/WEB-INF/page/change.jsp</result><!-- 验证失败返回该页面 -->

<result name="message">/WEB-INF/page/message.jsp</result><!-- 成功转到页面 -->

...</action>


注意：所有方法的校验和指定方法的校验同时存在的时候，方法验证会合并然后执行，对于同样的方法

执行后面那个，前面的被覆盖。](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




