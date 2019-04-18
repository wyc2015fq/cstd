# srtuts2 包继承时 默认拦截器无法拦截Unable to find interceptor class referenced by ref-name - z69183787的专栏 - CSDN博客
2015年10月28日 17:18:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2503
**原来父子 package的名字是 xxx-default ，结果一直无法拦截，最后去掉 - 就好了。。记录一下 **
父package
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE struts PUBLIC
        "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"
        "http://struts.apache.org/dtds/struts-2.0.dtd">
<struts>
    <!--
        use in develop environment
    -->
    <constant name="struts.devMode" value="false"/>
    <constant name="struts.enable.DynamicMethodInvocation" value="false"/>
    <constant name="struts.ognl.allowStaticMethodAccess" value="true"/>
    <constant name="struts.objectFactory"
              value="org.apache.struts2.spring.StrutsSpringObjectFactory"/>
    <!-- zhoushun add spring -->
    <constant name="struts.objectFactory.spring.autoWire.alwaysRespect" value="true"/>
    <!-- I18N -->
    <constant name="struts.custom.i18n.resources" value="config/i18n/globalMessages"></constant>
    <constant name="struts.i18n.encoding" value="UTF-8"/>
    <constant name="struts.locale" value="zh_CN"/>
    <constant name="struts.action.excludePattern" value="/static/img/.*"/>
    <constant name="struts.freemarker.templatesCache" value="true"/>
    <constant name="struts.ui.templateSuffix" value="java"/>
    <constant name="struts.ui.theme" value="simple"/>
    <!-- add by yejun.mei -->
    <constant name="struts.multipart.parse" value="jakarta"/>
    <constant name="struts.multipart.saveDir" value="/data/appdatas/upload_temp"/>
    <constant name="struts.multipart.maxSize" value="10240000"/>
    <!-- end by yejun.mei -->
    <constant name="struts.action.excludePattern" value="/.*swf,/.*png,/static/.*,/monitorservlet,/servlet/.*"/>
    <package name="customDefault" extends="json-default">
        <interceptors>
            <interceptor-stack name="basicStackV2">
                <interceptor-ref name="exception"/>
                <interceptor-ref name="params">
                    <param name="excludeParams">dojo\..*,^struts\..*,.*\\u0023.*</param>
                </interceptor-ref>
                <interceptor-ref name="servletConfig"/>
                <interceptor-ref name="prepare"/>
                <interceptor-ref name="chain"/>
                <interceptor-ref name="staticParams"/>
                <interceptor-ref name="actionMappingParams"/>
                <interceptor-ref name="params">
                    <param name="excludeParams">dojo\..*,^struts\..*,.*\\u0023.*</param>
                </interceptor-ref>
                <interceptor-ref name="conversionError"/>
            </interceptor-stack>
        </interceptors>
        <default-interceptor-ref name="basicStackV2"/>
        <!-- 处理空指针问题 -->
        <global-results>
            <!-- null pointer-->
            <result name="nullObject">/activities/common/nullObject.ftl</result>
        </global-results>
        <!--<global-exception-mappings>-->
        <!--<!–<exception-mapping exception="java.sql.SQLException" result="sql"/>–>-->
        <!--<exception-mapping exception="java.lang.NullPointerException" result="nullObject"/>-->
        <!--</global-exception-mappings>-->
        <!--<global-results>-->
        <!--<result name="sql">/internal_Error.jsp</result>-->
        <!--<result name="invalidinput">/invalid_Input.jsp</result>-->
        <!--<result name="naming">/internal_Error.jsp</result>-->
        <!--</global-results>-->
        <!--<global-exception-mappings>-->
        <!--<exception-mapping result="sql" exception="java.sql.SQLException"></exception-mapping>-->
        <!--<exception-mapping result="invalidinput"-->
        <!--exception="cn.codeplus.exception.InvalidInputException"></exception-mapping>-->
        <!--<exception-mapping result="naming"-->
        <!--exception="javax.naming.NamingException"></exception-mapping>-->
        <!--</global-exception-mappings>-->
        <!--<action name="*_*" class="cn.codeplus.action.{2}Action" method="{1}">-->
        <!--<result name="success">/{1}_{2}_success.jsp</result>-->
        <!--<result name="error">/{1}_{2}_error.jsp</result>-->
        <!--<!–<exception-mapping result="sql" exception="java.sql.SQLException"></exception-mapping>–>-->
        <!--</action>-->
    </package>
    <include file="config/struts/struts-*.xml"/>
</struts>
```
子package
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE struts PUBLIC
        "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"
        "http://struts.apache.org/dtds/struts-2.0.dtd">
<struts>
    <!-- merchant interceptor-->
    <package name="merchantDefault" extends="customDefault">
        <!-- 定义一个拦截器 -->
        <interceptors>
            <interceptor name="authority"
                         class="com.xxxx.xx.merchant.interceptor.LoginInterceptor">
            </interceptor>
            <!-- 拦截器栈 -->
            <interceptor-stack name="merchantStack">
                <interceptor-ref name="authority"/>
                <interceptor-ref name="basicStackV2"/>
                <interceptor-ref name="defaultStack"/>
            </interceptor-stack>
        </interceptors>
        <default-interceptor-ref name="merchantStack"/>
        <global-results>
            <!-- no authority -->
            <result name="noAuthority">/activities/common/noAuthority.ftl</result>
        </global-results>
    </package>
</struts>
```
