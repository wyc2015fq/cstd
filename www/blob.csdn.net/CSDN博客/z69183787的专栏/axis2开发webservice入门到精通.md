# axis2开发webservice入门到精通 - z69183787的专栏 - CSDN博客
2014年08月05日 11:41:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1398
1，准备工作：
首先我们要下载：axis2-1.4.1-war（发布webservice），axis2-1.4.1-bin.zip(webservice调用使用的各种包)。
下载好了，把axis2-1.4.1-war目录下面的axis2.war发布到tomcat中。发布好，访问：http://localhost:8080/axis2/  界面如下：
![axis2开发webservice入门到精通](http://s11.sinaimg.cn/mw690/6d187d27gddfb07ec0e4a&690)
然后编写我们的java类，注意java类不要包，如下面：
![axis2开发webservice入门到精通](http://s16.sinaimg.cn/mw690/6d187d27gddfb1620c1ff&690)
![axis2开发webservice入门到精通](http://s7.sinaimg.cn/mw690/6d187d27gddfb17d3cac6&690)
然后把这个类的class文件拷贝D:\apache-tomcat-6.0.36\webapps\axis2\WEB-INF\pojo  如果WEB-INF这个目录下面没有pojo这个目录，那你得必须建立一个，因为在axis2的配置文件中配置的（文件路径是：conf/axis2.xml）
![axis2开发webservice入门到精通](http://s3.sinaimg.cn/mw690/6d187d27g7c991d979d62&690)
拷贝后，启动你的tomcat,访问http://localhost:8080/axis2/services/listServices  看是否发布成功，成功界面如下面：
![axis2开发webservice入门到精通](http://s13.sinaimg.cn/mw690/6d187d27gddfb338d44fc&690)
这样 你就可以访问这两个方法了，
http://localhost:8080/axis2/services/SimpleService/getGreeting?name=bill 
http://localhost:8080/axis2/services/SimpleService/getPrice
成功界面如下：
![axis2开发webservice入门到精通](http://s15.sinaimg.cn/mw690/6d187d27gddfb4255e99e&690)
![axis2开发webservice入门到精通](http://s9.sinaimg.cn/mw690/6d187d27gddfb4329c148&690)
好，你的webservie就已经发布好，接下来 我们就来调用发布的webservice，两种方法：
第一种：代码式
package com.tl.test;
import org.apache.axis2.addressing.EndpointReference;
import org.apache.axis2.client.Options;
import org.apache.axis2.rpc.client.RPCServiceClient;
import javax.xml.namespace.QName;
public class RPCClient {
public static void main(String[] args) throws Exception {
//  使用RPC方式调用WebService 
RPCServiceClient serviceClient = new RPCServiceClient();
Options options = serviceClient.getOptions();
//  指定调用WebService的URL
EndpointReference er = new EndpointReference("http://localhost:8080/axis2/services/SimpleService");
options.setTo(er);
//  指定getGreeting方法的参数值
Object[] opAddArgs = new Object[]{"张三"};
//指定getGreeting方法返回值的数据类型的class对象
Class[] classs = new Class[]{String.class};
//指定要调用的getGreeting方法及wsdl文件的命名空间，第一个参数表示WSDL文件的命名空间
//通过访问http://localhost:8080/axis2/services/SimpleService?wsdl 就可以看见 元素的targetNamespace属性值
界面如下：
![axis2开发webservice入门到精通](http://s1.sinaimg.cn/mw690/6d187d27gddfb5e759920&690)
QName qname = new QName("http://ws.apache.org/axis2","getGreeting");
//调用getGreeting方法并输出该方法的返回值
//这里有三个参数的意思：1，是QName对象，表示要调用的方法名；2，webservice的参数值，参数类型是Object[]；3，返回值class对象，参数类型是Class[],
System.out.println(serviceClient.invokeBlocking(qname, opAddArgs,classs)[0]);
//下面是调用getPrice方法的代码，这些代码与调用getGreeting方法的代码类似
classs = new Class[]{int.class};
qname = new QName("http://ws.apache.org/axis2","getPrice");
//如果没有参数，参数不能为null，应该为new Object[]{},没有返回值，就应该调用invokeRobust
System.out.println(serviceClient.invokeBlocking(qname, new Object[]{},classs)[0]);
}
}
我们运行的结果如下：
![axis2开发webservice入门到精通](http://s5.sinaimg.cn/mw690/6d187d27gddfb62aed9f4&690)
第二种：wsdl2java
要配置环境变量：AXIS2_HOME
这个时候 我们下axis2-1.4.1-bin.zip 这个文件就派上用途了，打开该文件中bin里面就可以找到wsdl2java.bat 
然后我们通过命令行工具 来实现这个 首先我们到该文件下面的bin目录，如：
![axis2开发webservice入门到精通](http://s9.sinaimg.cn/mw690/6d187d27g0abd1f9e2a18&690)
然后开始工作：运行该命令：wsdl2java -uri http://localhost:8080/axis2/services/SimpleService?wsdl -p com.tl.test -s -o stub
解释：-uri 后面运行的是 你的wsdl， -p 是文件生成的包名 -o stub生成的文件目录，是bin目录下面的
运行结果如下：
![axis2开发webservice入门到精通](http://s9.sinaimg.cn/mw690/6d187d27gddfb7e219828&690)
这样 你就可以在你的bin里面下面找到你要的文件了
如下面：
![axis2开发webservice入门到精通](http://s8.sinaimg.cn/mw690/6d187d27gddfb7f75d707&690)
然后 你就可以把这个类 拷贝到你的工程下面使用了，主要包名，我这里是com.tl.test，所以你相应的在你的工程下面建一个com.tl.test,
如下面：
![axis2开发webservice入门到精通](http://s14.sinaimg.cn/mw690/6d187d27gddfb86b24bfd&690)
最后，我们就可以建立一个类来测试，代码如下面：
package com.tl.test;
//这样的方法大大的减少了调用webservice的步骤，并且代码更加简介。但要注意的是，wsdl2java.bat命令生成stub类将
//webservice方法的参数都封装在相应的类中。
public class StubClient {
public static void main(String[] args) throws Exception {
SimpleServiceStub sss = new SimpleServiceStub();
//封装getGreeting方法的参数
SimpleServiceStub.GetGreeting gg = new SimpleServiceStub.GetGreeting();
gg.setName("王武");
System.out.println(sss.getGreeting(gg).get_return());
System.out.println(sss.getPrice().get_return());
}
}
运行效果如下：
![axis2开发webservice入门到精通](http://s2.sinaimg.cn/mw690/6d187d27gddfb9252a121&690)
到此，我们就结束，祝你测试通过。
