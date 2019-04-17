# 基于Jmeter和Jenkins接口自动化测试框架搭建过程 - zhusongziye的博客 - CSDN博客





2017年09月17日 18:12:30[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：287








###   1、 下载地址


   Jmeter： http://jmeter.apache.org/download_jmeter.cgi


   Ant：http://ant.apache.org/bindownload.cgi


   Jenkins：https://jenkins.io/index.html




###     2. Jmeter+Ant安装配置




    Jmeter解压后运行Jmeter.bat即可使用

    Ant下载后解压到目录，按照下列配置系统变量即可：
- 



     1) ANT_HOME：D:\apache-ant-1.9.4-bin\apache-ant-1.9.4（此为ANT安装路径）

     2) PATH（后面添加以下内容）：;%ANT_HOME%\bin

     3)测试Ant：在CMD中输入ant –version，如果返回ant版本，则表示安装成功



- 
将JMeter目录下extras子目录里ant-JMeter-1.1.1.jar复制到Ant的lib目录下，这样Ant运行时才能找到，"org.programmerplanet.ant.taskdefs.jmeter.JMeterTask"这个类，从而成功触发JMeter脚本。

- 



- 
jmeter默认保存的是.csv格式的文件，所以我们先要设置一下bin/jmeter.properties文件内容，保存


> 
jmeter.save.saveservice.output_format=xml

- 
配置ant构建文件build.xml（见附录）

- 


- 



### 3. Jenkins安装与配置

Jenkins安装比较简单，点击msi文件按照要求操作即可，过程若出现问题，可重启下jenkins服务后，再继续安装；

接下来主要介绍Jenkins构建ant项目的配置：

浏览器输入地址http://localhost:8080/显示界面，则表示安装成功




### 4. shell运行环境搭建

安装cygwin后，配置环境变量

l 按照下列配置系统变量即可：

1） JAVA_HOME：D:\ Program Files\Java\jdk1.8.0_11（此为JDK安装路径）

2） ANT_HOME：D:\apache-ant-1.9.4-bin\apache-ant-1.9.4（此为ANT安装路径）

3） PATH（后面添加以下内容）：

;%JAVA_HOME%\bin;%JAVA_HOME%\jre\bin;%ANT_HOME%\bin;E:\Program
 Files\cygwin\bin

4） 测试Ant：在CMD中输入ant –version，如果返回ant版本，则表示安装成功

5） 测试Java：在CMD中输入 java –version，如果返回java版本，则表示安装成功

6） 测试shell运行环境：在CMD中输入sh，如果为提示错误，则表示安装成功




### 5. Testlink与Jenkins对接

1、先安装过程所需要的插件：Testlink Plugin、TAP Plugin

直接在系统管理—>管理插件—>可选插件，中搜索后，点击安装即可

注意：TAP插件必须用1.25版本的，新版本发布后tap文件链接失效




2、 系统管理—>系统设置，Testlink配置如下：

Name：TestLink 1.9.10

URL：http://192.168.4.56/testlink/lib/api/xmlrpc/v1/xmlrpc.PHP

Developer Key：登录TestLink后，选择个人账号—生成新的秘钥，API Key拷贝到这里即可

### 6. TestLink配置

Testlink需要增加自定义字段，如AutoTest，具体的用例如下：

![](https://img-blog.csdn.net/20170917181342151?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



注意两点：

1）测试方式：自动的


2）AutoTest：内容为对应该条用例的.tap文件的文件名

### 7. 构建项目

#### 7.1. 准备测试脚本到SVN

![](https://img-blog.csdn.net/20170917181418175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



上图是SVN中对应的文件结构，下面会介绍jenkins构建项目的详细配置。

#### 7.2. 新建一个自由风格的软件项目

![](https://img-blog.csdn.net/20170917181451019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



旧的构建保存了你在一次构建中的所有临时文件，如果构建没有保留的必要，就勾选丢弃旧的构建，同时设置保持构建天使和保持构建的最大个数两个参数。注意：三个设置必须同时设置，否则无效




#### 7.3. 源码管理


选择subversion

![](https://img-blog.csdn.net/20170917181527584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



Repository URL：SVN中脚本目录


Credentials：SVN账号用户名和密码，可点击后面的Add添加


Local module directory：SVN下载到本地的目录，配置为【.】默认项目根目录下【注：这个与Ant的build.xml文件中的配置相关联，默认的文件是根目录，若这里有修改，需要修改对应的build.xml文件】




#### 7.4. 触发器配置

![](https://img-blog.csdn.net/20170917181553068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这里是让脚本每天晚上自动构建执行，若不需要重复执行，则不要勾选该项接口




#### 7.5. 构建Invoke TestLink

![](https://img-blog.csdn.net/20170917181619091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：

1、 TestLink Configuration中TestProjectName和TestPlan Name需要与testlink中的【测试项目】和测【测试计划】一致

2、 TestLink Configuration中BuildName：

1） 可以与Testlink设置一致（这样每次测试结果会在同一个测试版本中记录多次结果）

2） 也可以设置由Jenkins配置返回，如：上图中的$BUILD_TAG,则表示将构建这次项目的标记作为版本返回到TestLink上，这样每一次构建，Testlink上都会新增一个版本

3、 Targets：这里是指build.xml文件中的<target name=“xxx”>,可设置为空，则默认全部执行

4、 这里的Build File是指Ant的build.xml所在目录，若前面SVN目录有修改，这里也需要一并修改

5、 After iterating all test cases Build Steps：这里运行jmx.sh文件，将Jmeter运行结果XML格式，转换为TAP格式，以便TestLink转换为测试结果

![](https://img-blog.csdn.net/20170917181702793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这个配置是将TAP结果返回到Testlink，

注意：

Include Pattern：这里的路径与build.xml文件中输出的路径一致。




#### 7.6. TAP结果发布

![](https://img-blog.csdn.net/20170917181727319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


Test results:结果路径配置，与build.xml中配置输出路径一致




#### 7.7. 构建后邮件发送


增加构建后操作：Editable Email Notification

![](https://img-blog.csdn.net/20170917181758789?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





#### 7.8. 系统配置：邮件的全局参数配置

![](https://img-blog.csdn.net/20170917181825126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


未截图的按默认配置


备注：

Default Subject：用例执行结果:$PROJECT_NAME - Build # $BUILD_NUMBER - $BUILD_STATUS!

Default Content：



${FILE,path="sum/sum.html"}<hr/> (本邮件是程序自动下发的，请勿回复！)<br/> 项目名称：$PROJECT_NAME<br/> 构建编号：$BUILD_NUMBER<br/> svn版本号：${SVN_REVISION}<br/> 构建状态：$BUILD_STATUS<br/> 触发原因：${CAUSE}<br/> 构建日志地址：<a href="${BUILD_URL}console">${BUILD_URL}console</a><br/><hr/> 构建地址：<a href="$BUILD_URL">$BUILD_URL</a><br/><hr/> 变更集:${JELLY_SCRIPT,template="html"}<br/><hr/>




邮件通知配置，根据邮箱实际情况进行配置即可，如下：

![](https://img-blog.csdn.net/20170917182542952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







### 8.附录



JMX2TAP.SH内容：

```
1 #!/bin/bash
2 LANG=zh_CN.UTF-8 
3 export LANG 
4 # Parse each result file from the tests, created by JMeter when run by ant. 
5 # Every file created by JMeter will be parsed consecutively by the following for loop. 
6 for XML_resultfile in $(ls -1 results/*.xml); do 
7     echo "XML_resultfile: ${XML_resultfile}" 
8     BASE_filename=$(basename ${XML_resultfile} .xml)   
9     TestSamples=( $(xpath  ${XML_resultfile} '//@lb' 2>/dev/null) ) 
10     num_of_TestSamples=${#TestSamples[@]}
11      echo "num_of_TestSamples: ${num_of_TestSamples}"
12   # Reference to TAP13 file standard:
13   # link:http://www.4pan.net
14   # result_selector=0 
15   if [ ${num_of_TestSamples} -gt 0 ]; then
16       for ((line=0; ${line} < ${num_of_TestSamples}; line++)); do    
17         echo "------------------CASE ${line}--------------------------"    
18         echo "TestCases fullname: ${TestSamples[${line}]}"
19         isFlag=$( echo ${TestSamples[${line}]} | grep '.*-.*-.*(.\{1,\}):.*')
20         if [ "${isFlag}" != "" ]; then
21             filename=$( echo ${TestSamples[${line}]} | cut -d '"' -f 2 | cut -d '(' -f 1 )
22             TAP_filename=${filename}.tap
23             isExistTap=""
24             isExistTap=$(ls -R | grep "${TAP_filename}")
25             if [ "${isExistTap}" == "" ]; then
26                 TestCase=$( echo ${TestSamples[${line}]} | cut -d '"' -f 2 | cut -d ':' -f 1 )
27                 num_of_testcases=1
28                 TestCase_count1=( $(xpath ${XML_resultfile} "//httpSample[contains(@lb,'${filename}')]/@rc" 2>/dev/null) )
29                 TestCase_count2=( $(xpath ${XML_resultfile} "//sample[contains(@lb,'${filename}')]/@rc" 2>/dev/null) )
30                 ((num_of_testcases=${#TestCase_count1[@]}+${#TestCase_count2[@]}))
31                 echo "TestCase:${TestCase}"
32                 echo "filename:${TAP_filename}"
33                 echo "TestCase_count:${num_of_testcases}"        
34                 label="${BASE_filename} - ${TestCase}"        
35                 echo "TAP version 13" > results/${TAP_filename}
36                 echo ">>>${BASE_filename}.xml Covert to : ${TAP_filename}"    
37                 echo "1..${num_of_testcases}" >> results/${TAP_filename}
38                 # Create an array with testcases labels and results
39                 result_if1=( $(xpath ${XML_resultfile} "//httpSample[contains(@lb,'${filename}')]/assertionResult[failure='true']/failure" 2>/dev/null | tr -d '"' | tr ' ' '_' | sed 's/^_//') )
40                 result_if2=( $(xpath ${XML_resultfile} "//sample[contains(@lb,'${filename}')]/assertionResult[failure='true']/failure" 2>/dev/null | tr -d '"' | tr ' ' '_' | sed 's/^_//') )
41                 ((num_of_resultif_count=${#result_if1[@]}+${#result_if2[@]}))
42                 echo ">TestResult:${num_of_resultif_count}"
43                 #echo "number_result_if:${#result_if[@]}"
44                 if [ ${num_of_resultif_count} -gt 0 ]; then
45                     msg_texts=( $(xpath ${XML_resultfile} "//httpSample[contains(@lb,'${filename}')]/assertionResult[failure='true']/failureMessage" 2>/dev/null | tr -d '"' | tr ' ' '_' | sed 's/^_//') )
46                     result="not ok ${num_of_testcases}  ${label} - error[${num_of_resultif_count}]"
47                     echo "${result}">> results/${TAP_filename}
48                     echo "failureMessage:${msg_texts}">> results/${TAP_filename}                    
49                     #echo "${result}"
50                     #echo "failureMessage:${msg_texts}"
51                     #(( line=${line}+${num_of_testcases}-1 ))            
52                             #echo "msg_text:${msg_texts}">> results/${TAP_filename}
53                             #num_of_msg_texts=${#msg_texts[@]}
54                             #echo "num_of_msg_texts:${num_of_msg_texts}"
55                             #echo "num_of_msg_texts:${msg_texts}"
56                             #for ((msgnum=0; ${msgnum} <${num_of_msg_texts}; msgnum++)); do                
57                             #msg_text="message[${num}]:'$(echo ${msg_texts[${msgnum}]}| cut -d '>' -f 2 | cut -d '<' -f 1)'"
58                             #echo "${msg_texts}"
59                             #done                
60                     else                
61                         if [ ${num_of_testcases} -gt 0 ]; then
62                             result="ok ${num_of_testcases}  ${label} - error[${#result_if[@]}]"
63                             echo "${result}">> results/${TAP_filename}
64                             #echo "${result}"
65                             #(( line=${line}+${num_of_testcases}-1 ))
66                         else
67                             result="not ok 1 - ${num_of_testcases}  ${label} - error[XML Covert to TAP ERROR:Xpath query find 0 result]"
68                             echo "${result}">> results/${TAP_filename}
69                             #echo "${result}"
70                             #(( line=${line}+${num_of_testcases} ))
71                         fi                                    
72                 fi
73             else
74                 echo ">tap already converted"
75             fi
76         else
77             echo "sample name  is not standard"
78         fi
79     done
80     else
81         echo "over" >> results/${TAP_filename}
82     fi
83 done
```
View Code

build.xml内容



```
1 <?xml version="1.0" encoding="UTF-8"?> 
 2 <project name="ant-jmeter-test" default="run" basedir="."> 
 3     <tstamp> <format property="time" pattern="yyyyMMddhhmm" />    </tstamp> 
 4     <tstamp> <format property="report.datestamp" pattern="yyyy/MM/dd HH:mm" /></tstamp> 
 5     <property environment="env"/> 
 6     <property name="report.title" value="接口测试"/>    
 7     <property name="jmeter.home" value="${env.JMETER_HOME}" /><!-- 需要改成自己本地的 Jmeter 目录-->      
 8     <property name="jmeter.result.dir" value="${env.WORKSPACE}/results" /><!-- jmeter测试结果放置目录，这里是放在构建项目下的result目录--> 
 9     <property name="jmeter.sum.dir" value="${env.WORKSPACE}/sum" /><!-- jmeter测试概要放置目录(主要用于邮件发送时，作正文内容)，这里是放在构建项目下的result目录-->
 10    <property name="testplan.dir" value="${env.WORKSPACE}" /><!-- 运行的jmx脚本目录-->
 11    
 12    
 13     <property name="ReportName" value="TestReport" /><!-- 生成的报告的前缀 -->
 14     <property name="jmeter.result.jtlName" value="${jmeter.result.dir}/${ReportName}.xml" /><!-- 生成的jtl报告名称 -->
 15     <property name="jmeter.result.htmlName" value="${jmeter.result.dir}/${ReportName}.html" /><!-- 生成的html报告名称缀 -->
 16     <property name="jmeter.sum.htmlName" value="${jmeter.sum.dir}/sum.html" />     <!-- 生成的sum报告的名称 -->    
 17 
 18     <target name="run">
 19         <echo message="start..."/>
 20         <antcall target="clean" />
 21         <antcall target="test" />
 22         <antcall target="report" />
 23     </target>
 24     <target name="clean">
 25         <delete dir="${env.WORKSPACE}/results/" />
 26         <mkdir dir="${env.WORKSPACE}/results" />
 27         <delete dir="${env.WORKSPACE}/sum/" />
 28         <mkdir dir="${env.WORKSPACE}/sum" />        
 29     </target>
 30     <target name="test">
 31         <taskdef name="jmeter" classname="org.programmerplanet.ant.taskdefs.jmeter.JMeterTask" />
 32         <jmeter jmeterhome="${jmeter.home}" resultlog="${jmeter.result.jtlName}">
 33             <!-- 声明要运行的脚本。"*.jmx"指包含此目录下的所有jmeter脚本 -->
 34             <!-- 配置构建参数TESTLINK_TESTCASE_TESTFILEPATH在Jenkins上    -->
 35             <!-- ant -DGUI=false -DTEST_ENVIRONMENT=2 -DTESTLINK_TESTCASE_TESTFILEPATH=test.jmx -->
 36             <testplans dir="${testplan.dir}" includes="${env.TESTLINK_TESTCASE_TESTFILEPATH}.jmx" />
 37             <property name="jmeter.save.saveservice.output_format" value="xml"/>
 38             <property name="jmeter.save.saveservice.assertion_results" value="all"/>
 39             <property name="jmeter.save.saveservice.bytes" value="true"/>             
 40         </jmeter>
 41     </target>
 42      <path id="xslt.classpath">
 43         <fileset dir="${jmeter.home}/lib" includes="xalan*.jar"/>
 44         <fileset dir="${jmeter.home}/lib" includes="serializer*.jar"/>
 45      </path>
 46 
 47       <target name="report">
 48        
 49         <xslt  classpathref="xslt.classpath"
 50                 force="true"
 51                 in="${jmeter.result.jtlName}" 
 52                 out="${jmeter.result.htmlName}" 
 53                 style="${jmeter.home}/extras/jmeter.results.shanhe.me.xsl"> <!-- jtl测试报告转化为html报告 -->
 54         <param name="dateReport" expression="${report.datestamp}"/>
 55         </xslt>
 56         <xslt  classpathref="xslt.classpath"
 57                 force="true"
 58                 in="${jmeter.result.jtlName}" 
 59                 out="${jmeter.sum.htmlName}" 
 60                 style="${jmeter.home}/extras/jmeter-results-detail-report_21-lj.xsl"> <!-- sum.html报告，该xsl文件在上个文件的body中保留summary部分的显示，用于jenkins构建后邮件发送的正文展示 -->
 61         <param name="dateReport" expression="${report.datestamp}"/>
 62         </xslt>
 63         <!-- 因为上面生成报告的时候，不会将相关的图片也一起拷贝至目标目录，所以，需要手动拷贝 -->
 64         <copy todir="${jmeter.result.dir}">
 65             <fileset dir="${jmeter.home}/extras">
 66                 <include name="collapse.png"/>
 67                 <include name="expand.png"/>
 68             </fileset>
 69         </copy>
 70     </target>
 71 </project>
 72
```






