# C++实现WebService - youfangyuan - CSDN博客
2012年12月16日 21:56:21[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：8271
                
一、引言 
    Web Service其实就是一个应用程序，它向外界暴露出一个能够通过Web进行调用的API，开发人员可以用编程的方法通过Web调用来实现某个功能的应用程序。从深层次上看，Web Service是一种新的Web应用程序分支，它们是自包含、自描述、模块化的应用，可以在网络(通常为Web)中被描述、发布、查找以及通过Web来调用。 Web服务给人们带来许多令人吃惊的好处，它具有在完全不同平台之间和互操作性，在无处不在的网络上调用另一种Web服务的能力。由于web服务使用http协议通信(通常使用80端口)，因此天然地可以穿越防火墙。另外，我们可以使用各种语言来开发和调用Web服务，有利于企业业务的集成和数据重用。
    如上所述，我们可以使用各种语言开发web服务，但主要以Java和.NET为主，用C/C++开发web服务的工具不多，主要有Axis C++和gSoap。相比于Java版的Axis，Axis C++在安装配置和开发部署新的Web服务都比较复杂，开发时要关注每一个细节，因为Axis C++[1]本身并没有详细的日志信息，当出现错误的时候很难跟踪哪个环节出了问题。另外，Axis C++对WSDL信息的内容要求非常严格，而且往往找不到问题所在。虽然存在一些问题，但是Axis C++在性能上还是很有优势，也因此是许多Web服务开发者的首选。[2]
    至于Axis C++的安装配置，请参考其官方文档[1]。另外，还可以使用gSoap[3]来开发C/C++的Web服务。 gSoap是一个开源项目，可以从SourceForge下载。
二、基本过程
   使用Axis C++开发Web服务的基本过程可以简单描述如下：
   1、使用Java语言描述WEB服务
   2、使用Java2WSDL工具生成WSDL文件
   3、使用WSDL2Ws工具生成服务端代码框架
   4、编码实现service，并部署服务
   5、使用WSDL2Ws工具生成客户端代码框架
   6、编码实现客户端
三、实例演示
   这里以一个简单的加法Add服务来说明用Axis C++构建webService的过程。
1、使用Java语言描述WEB服务
   Axis C++中我们必须先定义好WSDL后方可生成用来实现该服务的代码框架，这一点与Axis Java不同。生成WSDL方法很多，可以使用Java2WSDL工具，当然也可以手工编写，这里我们使用Java2WSDL工具生成。下面我们对Add服务用Java语言描述：
  public interface Add{
     public int add(int x, int y);
  }
  我们只要方法的声明即可，不需要实现方法，因此我们使用接口来定义。将上面的代码保存为Add.java。
2、使用Java2WSDL工具生成WSDL文件
   javac Add.java   (生成.class文件)
   java -Djava.ext.dirs=$AXISCPP_HOME/lib/axisjava  org.apache.axis.wsdl.Java2WSDL -o Add.wsdl -l"http://localhost/axis/Add" -n"http://localhost/axis/Add" Add  
   经过上面两步，我们就可以得到Add.wsdl文件。需要说明的是，生成的WSDL文件有时会有错（intf都成了impl）,这可能是Java2WSDL工具的Bug。如果出现这种情况，可以和Axis C++自带的例子WSDL文件对比。（注：我花了两天时间才找出这个问题）
3、使用WSDL2Ws工具生成服务端代码框架
   有上面的WSDL文件，我们可以用WSDL2Ws工具生成服务端构架。
   java -Djava.ext.dirs=$AXISCPP_HOME/lib/axisjava -cp $AXISCPP_HOME/lib/axis/wsdl2ws.jar org.apache.axis.wsdl.wsdl2ws.WSDL2Ws Add.wsdl -lc++ -sserver
   -s server 表示生成server代码框架
   -l c++    表示生成C++代码
4、编码实现service，并部署服务
   上面生成的文件中，我们只需要实现Add.cpp中的add方法即可。
   ... ...
   xsd__int Add::add(xsd__int Value0, xsd__int Value1)
   {
     return Value0+Value1;
   }
   ... ...
   对代码编译并链接生成.so动态库：
   g++ -shared -I$AXISCPP_HOME/include -o libadd.so *.cpp
   对服务进行部署，步骤如下：
   (1).将生成的libadd.so文件复制至$AXISCPP_DEPLOY/lib目录
   (2).复制Add.wsdl文件至$AXISCPP_DEPLOY/wsdls目录
   (3).编辑AXISCPP_DEPLOY/etc/server.wsdd文件并在相应位置添加如下内容：
       <service name="Add" provider="CPP:RPC" description="Add Service ">
             <parameter name="className" value="/apache/axiscpp/lib/libadd.so"/>
      <parameter name="allowedMethods" value="add "/>
       </service>
   (4).重新启动HTTP服务
   (5).检查http://localhost/axis是不否有新增的Add服务
   为了测试我们发布的Web服务是否工作正常，我们下面利用Axis C++来生成调用该服务的客户端代码并进行测试。
5、使用WSDL2Ws工具生成客户端代码框架
   java -Djava.ext.dirs=$AXISCPP_HOME/lib/axisjava -cp $AXISCPP_HOME/lib/axis/wsdl2ws.jar org.apache.axis.wsdl.wsdl2ws.WSDL2Ws Add.wsdl -lc++ -sclient
   -s client 表示生成client代码框架
   -l c++    表示生成C++代码
6、编码实现客户端   
   上述命令生成了Add.hpp, Add.cpp,我们再编写客户端入口程序 AddClient.cpp：
   #include "Add.hpp"
   #include <stdio.h>
   int main(int argc, char** argv)
   {
     Add ws("http://localhost/axis/Add");
     int sum;
     if(argc<2)
     {
       printf("Usage:%s num1 num2\n", argv[0]);
       return -1;
     }
     sum = ws.add(atoi(argv[1]),atoi(argv[2]));
     printf("%d\n",sum);
     return 0;
  }
  编译客户端，生成可执行文件：
  g++ -I$AXISCPP_HOME/include -L$AXISCPP_DEPLOY/lib -laxis_client -ldl -o Add *.cpp
  之后，可以用Add命令测试服务工作是否正常。
四、参考文献
1、官方网站：http://ws.apache.org/axis/
2、Linux下使用Axis C++构建Web服务：http://www.javayou.com/showlog.jspe?log_id=580
3、用C实现WebService：http://www.hnlinux.net/bbs/simple/index.php?t2303.html
            
