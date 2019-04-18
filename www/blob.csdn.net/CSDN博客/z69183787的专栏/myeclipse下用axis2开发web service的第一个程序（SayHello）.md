# myeclipse下用axis2开发web service的第一个程序（SayHello） - z69183787的专栏 - CSDN博客
2014年06月03日 20:23:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：854
jar包
axis.jar
jaxrpc.jar
saaj.jar
commons-logging.jar
commons-discovery.jar
wsdl4j.jar
SayHello是很多人都写过的web service。实现的功能是通过web服务输出“人名，how are you？”
本文参考网上众多资料，有的也不知源作者是谁，就不一一列举了，在此感谢愿意share经验的同志们。
一、环境搭建问题
1、本文环境：myeclipse7.5+jdk1.5+Tomcat5.5+axis2-1.5.4+Axis2_Codegen_Wizard_1.3.0+Axis2_Service_Archiver_1.3.0
2、Axis工具选择1.3.0，而不用1.5的原因
    开始从apache的网站上直接下的最新的tools axis2-eclipse-codegen-plugin-1.5.4和axis2-eclipse-service-plugin-1.5.4。从安装到使用，浪费了大半天的时间，到底还是没有成功。这个版本可能太新了，网上也没有解决问题的方案，就放弃了，选用了1.3的版本。1.5.4的问题如下：
(1)安装时，将.jar插件文件解压在C:/Program Files/Genuitec/MyEclipse 7.5/dropins下，启动myeclipse时，提示一系列的插件依赖问题。大多数问题在将manifest文件中的版本信息删掉后就不提示了，只有javax.xml.stream和javax.xml.stream一直提示没有。下了个javax.xml.stream.jar放在common目录下，还是不行，就按照网上说的将manifest文件中的javax.xml.stream和javax.xml.stream直接删除了。后来在tools使用中出了问题，一直没有解决，不知道是什么原因。
(2)用codegen工具由wsdl文件生成代码时，*stub.java要Implements 服务名接口（本例中是SayHello），可是自动生成的不是接口，而是类，没办法实现，不知道怎么会有这样的bug。解决时直接在*stub中删去了那个implements。
(3)按照网上的步骤，当然也是下文中描述的步骤，在用Archive工具打包service时要用到services.xml文件。那文件明明没有问题，可是老是提示The service file is not valid.网上没有这样的问题解决方法，不知道是不是从来没有人遇到过。也是这个原因决定放弃1.5.4的版本。
总之，开源的东西难免有bug，新的版本很容易有问题。而1.3.0用的人多，虽然也有小bug，好在大家已经找到了解决的办法。
3、安装axis2
    这个比较简单，只要把/axis2-1.5.4-war下axis2.war拷贝到tomcat的webapps下即可。成功后启动tomcat，浏览器中键入http://localhost:8080/axis2/后如下图所示，表示安装成功。
![](http://hi.csdn.net/attachment/201104/19/0_13032268760rhp.gif)
4、安装codegen和archive工具
    1.3.0版本跟1.5.4不同之处之一在于前者是文件夹下一堆内容，而后者是jar包。
    安装时，同样将文件夹拷贝到dropins目录下。启动后在new-other时显示下图，表示安装成功。但是在用wsdl文件生成代码时会出现弹出框报错如下：
An error occurred while completing process -java.lang.reflect.InvocationTargetException 。
![](http://hi.csdn.net/attachment/201104/19/0_1303227283O8s6.gif)
解决办法：
（1）如果是已经安装了插件，碰到上面提到的错误后，先关闭MyElipse7.5 ；（否则
1和2步骤可以省略）；
MyEclipse7.5默认安装的目录是“C:/Programe Files/Genuitec/MyEclipse7.5/” ;
删除MyEclipse7.5安装目录下dropins/目录的axis2插件 ， 然后重新启动MyEclipse （为了防止缓存）；
（2）启动完成后，会提示已经删除了axis2 插件; 然后再次关闭MyEclipse；
（3）找到jar包geronimo-stax-api_1.0_spec-1.0.1.jar和backport-util-concurrent-3.1.jar（前者在axis2的lib下，后者在1.5.4中没有，我在1.4.1的版本的lib下找到的），将其拷贝到codegen插件的lib目录下，然后修改对应的plugin.xml文件，在runtime中添加如下代码：
[-](http://blog.csdn.net/moonsheep_liu/article/details/6334854#)<libraryname="**lib/geronimo-stax-api_1.0_spec-1.0.1.jar**">
<exportname="*****"/>
</library>
[-](http://blog.csdn.net/moonsheep_liu/article/details/6334854#)<libraryname="**lib/backport-util-concurrent-3.1.jar**">
<exportname="*****"/>
</library>
（4）将 $workspace 位置 /.metadata/.plugins 下目录 Axis2_Codegen_Wizard 删除；
（ 5） 在命令行下切换至 $ECLIPSE_HOME 目录，然后执行： eclipse –clear（这一步我没做）。
这样就完成了。不知道是不是因为我开始没有做第4,5步的原因，第一次生成的java代码，类没有自动编译。又关闭myeclipse，做了第4步，重启myeclipse，重新生成代码，类才自动编译。
二、建立web service（图省事，很多直接从网上拷贝）
2.1创建webApps项目
启动 MyEclipse，新建立一个 WebApps（ File->New->Project->Web Project） ,给 Project Name 为 SayHello ，其他保持不改变。
2.2创建WSDL文件
A.选择File->New-Other菜单，进入后找到 MyEclipse->Web Services并选择 WSDL.如下图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_select-wsdl.PNG)
B.点击下一步按钮，显示如下图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_set-wsdl-dir.PNG)
C.选择“ src”目录作为“ Enter or select the parent folder”的值，“ File name”值给定为 SayHello.wsdl。
D.下一步
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_wsdl-new-content.PNG)
》所有界面内容取默认值，Finish！
E.显示 WSDL设计器的界面如下：
F.修改图中标红色的部分：
修改图中的NewOperation为SayHello;
修改input中parameters为SayHelloRequest;
修改output中parameters为SayHelloResponse;
修改完后如下图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_wsdl-changed-content.PNG)
红色框内为变化的内容，WSDL文件修改完毕。
2.3从 WSDL生成 Java代码
A.选择菜单“ File->New->Other”，从对话框中选取“ Axis2 Wizards”下面的“ Axis2 Code
 Generator”.点击“ Next”
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_axis-code-wizards.PNG)
B.进入下一个页面，保持“ Generate java source code from WSDL file”被选中，点击“ Next”进入下一步。
C.点击“ Browse”来选取存放在 src目录下的 SayHello.wsdl文件，“ Next” 进入下一步。
D.首先生成客户端代码和测试代码，设置如下：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_generate-client-code-custom.PNG)
E.下一步后，选择SayHellosrc目录，如下图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_set-gencode-dir.PNG)
Finish !
F.重复从A到C的步骤，然后生成服务端代码，设置如下：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_generate-server-code-custom.PNG)
后面同E。
G.结束后，刷新项目，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/teamlet/278972/o_code-pre-error.PNG)
这是由于没有将Axis2的 jar包放入 class path 中以及源代码的 package不为 org.example.www.sayhello引起的。
H.修改包名为org.example.www.sayhello并从tomcat中的 axis2 web应用 WEB-INF/lib目录中加载所有 jar包到项目的classpath中。
I.在SayHelloTest.java 中引入junit包。
2.4 编写业务代码
打开 SayHelloSKeleton.java文件其中的方法如下：(偶的跟这个不太一样，懒得再做一次了，反正差不多)
public  org.example.www.sayhello.SayHelloResponse SayHello( org.example.www.sayhello. .SayHello
 param0)
{
//  Todo fill this with the necessary business logic 
throw new  java.lang.UnsupportedOperationException();
}
修改为：
public org.example.www.sayhello.SayHelloResponse SayHello
                  (
                  org.example.www.sayhello.SayHello request
                  )
            {
                //TODO : fill this with the necessary business logic
                     try
                     {
                     SayHelloResponse response = new SayHelloResponse();
                     response.setOut("Hi,"+request.getIn()+".How are you?");
                     return response;
                     }catch(UnsupportedOperationException e)
                     {
                     throw e;
                     }
        }
2.5打包
A.选择菜单“ File->New->Other”，从对话框中选取“ Axis2 Wizards”下面的“ Axis2 Services Archive”.点击“ Next”进入下一个页面。
B.选择编译好的 class文件所在目录（本项目应该是 WebRoot下面 WEB-INF/classes），“ Next”进入下一步。
C.选择 WSDL文件所在目录，此处为 src目录下的 SayHello.wsdl文件。“ Next”,再“ Next”。选择 services.xm文件。
D.点击“ Next”，将记入最后一个页面，此处有两个选项，一个是输出目录一个是处处文件名称。这里的输出文件名称给的是SayHello。点击“ Finish”完成。
2.6部署
A．将打包好的文件 SayHello.jar拷贝到 Tomcat中已经部署的 Axis2应用的 WEB-INF/services目录下面。
B.重新启动 Tomcat。
C.打开浏览器，输入[http://127.0.0.1:8080/axis2](http://127.0.0.1:8080/axis2) ,并点击其中的“ services”链接可以看到 SayHello已经被部署。点击SayHello提供的连接可以看到与之对应的 WSDL。
2.7 调用测试
A.修改 SayHelloTest.java文件中的方法testSayHello 的内容为：
 public  void testSayHello() throws java.lang.Exception{
          String url="http://localhost:8080/axis2/services/SayHello";
          org.example.www.sayhello.SayHelloStub stub =
                    new org.example.www.sayhello.SayHelloStub(url);//the default implementation should point to the right endpoint
           org.example.www.sayhello.SayHelloStub.SayHello request=
                                                        (org.example.www.sayhello.SayHelloStub.SayHello)getTestObject(org.example.www.sayhello.SayHelloStub.SayHello.class);
                    // TODO : Fill in the sayHello2 here
           request.setIn("Sally");
           System.out.println(stub.SayHello(request).getOut());   
                        assertNotNull(stub.SayHello(
                        request));
         }
从菜单中选择“ Run－ >Run as->JUnit Test ，在控制台输出Hi,Sally.How are you? 完成测试！
