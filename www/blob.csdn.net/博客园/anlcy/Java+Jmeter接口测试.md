
# Java+Jmeter接口测试 - anlcy - 博客园






# [Java+Jmeter接口测试](https://www.cnblogs.com/camilla/p/7127780.html)
一、创建工程、引包
1、创建JAVA工程
2、引入Jmeter中lib\ext基础包：ApacheJMeter_java.jar、ApacheJMeter_core.jar
3、引入Jmeter日志包：jorphan.jar，logkit-2.0.jar，commons-logging-1.1.1.jar，avalon-framework-4.1.4.jar
4、引入httpclient-4.3.4.jar（是一套http协议实现包）
![](http://img.blog.csdn.net/20140711094516012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2FzaF9jaGVuMDA3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Jmeter用到的4个类：
[Arguments  ：](http://jmeter.apache.org/api/org/apache/jmeter/config/Arguments.html)getDefaultParameters用于获取界面的参数
[SampleResult：](http://jmeter.apache.org/api/org/apache/jmeter/samplers/SampleResult.html)**[runTest](http://jmeter.apache.org/api/org/apache/jmeter/protocol/java/sampler/JavaSamplerClient.html#runTest%28org.apache.jmeter.protocol.java.sampler.JavaSamplerContext%29)**JavaSamplerContext context) 类似于LR的Action
void：(context)初始化方法，类似于LR的init和Junit中的setUp()
void：teardownTest(JavaSamplerContext context) 类似于LR的end和Junit中的tearDown()

执行的先后顺序为：getDefaultParameters() --> setupTest(JavaSamplerContext context) -->
runTest(JavaSamplerContext context) --> teardownTest(JavaSamplerContext context)
①、addArgument("name", "value")    定义参数
②、sampleStart()    定义事务的开始，类似于LR的lr_start_transaction，和LR一样事务间不要放无关代码
③、sampleEnd()    定义事务的结束，类似于LR的lr_end_transaction
④、setSuccessful(true、false)    设置运行结果的成功或失败，Jmeter统计成功失败的次数，在聚合报告中能够体现。
二、用例
![](http://img.blog.csdn.net/20140711094535356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2FzaF9jaGVuMDA3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
1、TestApiGetSearchSuggestion.java，该代码实现对Api接口的请求，对返回的数据进行判断，并写文件。
2、Jmeter_GetSearchSuggestion.java，为本节介绍的重点
package com.test.jmeter;
import java.io.IOException;
import org.apache.http.client.ClientProtocolException;
import org.apache.jmeter.config.Arguments;
import org.apache.jmeter.protocol.java.sampler.AbstractJavaSamplerClient;
import org.apache.jmeter.protocol.java.sampler.JavaSamplerContext;
import org.apache.jmeter.samplers.SampleResult;
publicclass Jmeter_GetSearchSuggestionextends AbstractJavaSamplerClient {
privatestatic String label ="Jmeter_GetSearchSuggestion ";//定义label名称，显示在jmeter的结果窗口
private String url;
private String data;
publicvoid setupTest(){
//定义测试初始值，setupTest只在测试开始前使用
System.out.println("setupTest");
}
@Override
public SampleResult runTest(JavaSamplerContext arg0) {
url = arg0.getParameter("url");
data = arg0.getParameter("data");
SampleResult sr;
sr =new SampleResult();
sr.setSampleLabel(label);
TestApiGetSearchSuggestion t =new TestApiGetSearchSuggestion();
sr.sampleStart();
try {
//调用被压测接口的方法
t.PostJson(url, data);
sr.setSuccessful(true);
}catch (ClientProtocolException e) {
sr.setSuccessful(false);
e.printStackTrace();
}catch (IOException e) {
sr.setSuccessful(false);
e.printStackTrace();
}
sr.sampleEnd();// jmeter 结束统计响应时间标记
return sr;
}
publicvoid teardownTest(JavaSamplerContext arg0){
super.teardownTest(arg0);
}
public Arguments getDefaultParameters(){
//参数定义，显示在前台，也可以不定义
Arguments params =new Arguments();
params.addArgument("url","http://gapp.test.com/merchandise/GetSearchSuggestion");
params.addArgument("data","data={\"token\": \"aaaaaaaaaa\",\"body\": {\"keywords\": \"蓝月亮\"}}");
return params;
}
}


3、main函数测试成功后，打成jar包，放到%Jmeter_Home%\lib\ext目录中即可


三、运行用例
1、运行%Jmeter_Home%\bin\jmeter.bat
2、添加线程组，Java请求、查看结果树、聚合报告
![](http://img.blog.csdn.net/20140711094542329?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2FzaF9jaGVuMDA3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、执行后的结果为
![](http://img.blog.csdn.net/20140711094306812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2FzaF9jaGVuMDA3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Tips:
快捷键
Ctrl+R运行用例、Ctrl+E清除运行结果





