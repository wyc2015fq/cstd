
# testng.xml文件结构组成及节点属性说明 - anlcy - 博客园






# [testng.xml文件结构组成及节点属性说明](https://www.cnblogs.com/camilla/p/6840695.html)
<?xmlversion="1.0"encoding="UTF-8"?>
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd">
<suitename="suitename"junit="false"verbose="3"parallel="false"thread-count="5"configfailurepolicy="<span style="font-family:Arial;"><spanstyle="font-size: 14px; line-height: 26px;">skip</span></span>"annotations="javadoc"time-out="10000"skipfailedinvocationcounts="true"data-provider-thread-count="5"object-factory="classname"allow-return-values="true"><!-- name参数为必须 -->
<suite-files>
<suite-filepath="/path/to/suitefile1"></suite-file><!-- path参数为必须 -->
<suite-filepath="/path/to/suitefile2"></suite-file>
</suite-files>
<parametername="par1"value="value1"></parameter><!-- name, value参数为必须 -->
<parametername="par2"value="value2"></parameter>
<method-selectors>
<method-selector>
<selector-classname="classname"priority="1"></selector-class><!-- name参数为必须 -->
<scriptlanguage="java"></script><!-- language参数为必须 -->
</method-selector>
</method-selectors>
<testname="testename"junit="false"verbose="3"parallel="false"thread-count="5"annotations="javadoc"time-out="10000"enabled="true"skipfailedinvocationcounts="true"preserve-order="true"allow-return-values="true"><!-- name参数为必须 -->
<parametername="par1"value="value1"></parameter><!-- name, value参数为必须 -->
<parametername="par2"value="value2"></parameter>
<groups>
<definename="xxx"><!-- name参数为必须 -->
<includename=""description=""invocation-numbers=""/><!-- name参数为必须 -->
<includename=""description=""invocation-numbers=""/>
</define>
<run>
<includename=""/><!-- name参数为必须 -->
<excludename=""/><!-- name参数为必须 -->
</run>
<dependencies>
<groupname=""depends-on=""></group><!-- name,depends-on均为参数为必须 -->
<groupname=""depends-on=""></group>
</dependencies>
</groups>
<classes>
<classname="classname"><!-- name参数为必须 -->
<methods>
<parametername="par3"value="value3"></parameter>
<includename=""description=""invocation-numbers=""></include>
<excludename=""></exclude>
</methods>
<methods></methods>
</class>
</classes>
<packages>
<packagename=""/><!-- name参数为必须 -->
<packagename="">
<includename=""description=""invocation-numbers=""></include>
<excludename=""></exclude>
</package>
</packages>
<listeners>
<listenerclass-name="classname1"/><!-- name参数为必须 -->
<listenerclass-name="classname2"/>
</listeners>
</test>
<test></test>
</suite>
**testng.xml文件节点属性说明：**
**suite属性说明：**
@name: suite的名称，必须参数
@junit：是否以Junit模式运行，可选值(true | false)，默认"false"
@verbose：命令行信息打印等级，不会影响[测试](http://lib.csdn.net/base/softwaretest)报告输出内容；可选值(1|2|3|4|5)
@parallel：是否多线程并发运行测试；可选值(false | methods | tests | classes | instances)，默认 "false"
@thread-count：当为并发执行时的线程池数量，默认为"5"
@configfailurepolicy：一旦Before/After Class/Methods这些方法失败后，是继续执行测试还是跳过测试；可选值 (skip | continue)，默认"skip"
@annotations：获取注解的位置，如果为"javadoc", 则使用javadoc注解，否则使用jdk注解
@time-out：为具体执行单元设定一个超时时间，具体参照parallel的执行单元设置；单位为毫秒
@skipfailedinvocationcounts：是否跳过失败的调用，可选值(true | false)，默认"false"
@data-provider-thread-count：并发执行时data-provider的线程池数量，默认为"10"
@object-factory：一个实现IObjectFactory接口的类，用来实例测试对象
@allow-return-values：是否允许返回函数值，可选值(true | false)，默认"false"
@preserve-order：顺序执行开关，可选值(true | false) "true"
@group-by-instances：是否按实例分组，可选值(true | false) "false"
**test属性说明：**
@name：test的名字，必选参数；测试报告中会有体现
@junit：是否以Junit模式运行，可选值(true | false)，默认"false"
@verbose：命令行信息打印等级，不会影响测试报告输出内容；可选值(1|2|3|4|5)
@parallel：是否多线程并发运行测试；可选值(false | methods | tests | classes | instances)，默认 "false"
@thread-count：当为并发执行时的线程池数量，默认为"5"
@annotations：获取注解的位置，如果为"javadoc", 则使用javadoc注解，否则使用jdk5注解
@time-out：为具体执行单元设定一个超时时间，具体参照parallel的执行单元设置；单位为毫秒
@enabled：设置当前test是否生效，可选值(true | false)，默认"true"
@skipfailedinvocationcounts：是否跳过失败的调用，可选值(true | false)，默认"false"
@preserve-order：顺序执行开关，可选值(true | false) "true"
@group-by-instances：是否按实例分组，可选值(true | false) "false"
@allow-return-values：是否允许返回函数值，可选值(true | false)，默认"false"





