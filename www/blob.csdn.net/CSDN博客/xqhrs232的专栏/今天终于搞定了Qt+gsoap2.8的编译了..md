# 今天终于搞定了Qt+gsoap2.8的编译了. - xqhrs232的专栏 - CSDN博客
2018年11月02日 13:48:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：68
原文地址::[https://blog.csdn.net/dronly/article/details/70772005](https://blog.csdn.net/dronly/article/details/70772005)
相关文章
1、Qt：使用gSoap做一个简单的CS系统----[https://blog.csdn.net/wsj18808050/article/details/47706715](https://blog.csdn.net/wsj18808050/article/details/47706715)
2、Qt+gsoap调用WebService----[https://blog.csdn.net/tingsking18/article/details/5456831](https://blog.csdn.net/tingsking18/article/details/5456831)
3、How to use gSOAP with Qt for Web Service Client----[https://wiki.qt.io/How_to_use_gSOAP_with_Qt_for_Web_Service_Client](https://wiki.qt.io/How_to_use_gSOAP_with_Qt_for_Web_Service_Client)
4、qt 使用gsoap发布webservice服务与调用webservice服务----[https://blog.csdn.net/kaituojimo/article/details/78983160](https://blog.csdn.net/kaituojimo/article/details/78983160)
第一步，下载gsoap2.8 
                  地址：https://sourceforge.net/projects/gsoap2/files/ 
                  学习资料：https://www.genivia.com/dev.html
                  某大学资料： http://www.cs.fsu.edu/~engelen/soap.html
第二部：查看WSDL
                  Qt貌似用哪个版本没太大关系，gsoap导出的代码可以连STL都不带，跨平台性做的很彻底了。
                  假设WSDL的url如下：
  http://192.168.4.13:65153/edfx/WebService/webservice.asmx?op=ReadDispatchingPlanDataFromSAP
                  那么，从url已经可以看出，
                          A, 我们webservice的地址（EndPoint）是 http://192.168.4.13:65153/edfx/WebService/webservice.asmx
                          B. 我们的操作接口是ReadDispatchingPlanDataFromSAP
                          或者看到如下描述
                          POST /edfx/WebService/webservice.asmx HTTP/1.1
                          Host: 192.168.4.13
                          Content-Type: text/xml; charset=utf-8
                          Content-Length: length
                          SOAPAction: "http://tempuri.org/ReadDispatchingPlanDataFromSAP"
                   但我们没代码，因此先找到WSDL，一般如下：
                          http://192.168.4.13:65153/edfx/WebService/webservice.asmx?WSDL         
          这是全部接口都包含在内的页面，描述也包括了，你的接口由哪个出口返回。
第三步：导出代码
               一直cd到gsoap的gsoap\bin\win32目录下，会看到有两个程序 wsdl2h.exe 和 soapcpp2.exe
               wsdl2h.exe 根据WSDL内容，导出一个头文件，就是把webservice的借口C++函数化。
               soapcpp2.exe 根据函数化后的头文件，导出需要用到的类和类代码。
               两个程序都可以加 -h 查看详细的配置例如：wsdl2h -h 和 soapcpp2 -h 
               使用之前建议大家先看看，否则后面我的配置是看不懂的，而且配置大小写意义完全不同。
               为了可以在win，linux甚至嵌入式设备上运行，我使用了如下配置
               wsdl2h -s -o test.h http://192.168.4.13:65153/edfx/WebService/webservice.asmx?WSDL
               这样test.h文件就出来了，然后根据这个文件，继续以下指令
               soapcpp2 -CLx -IF:\gsoap-2.8\gsoap\import test.h
               这里注意，我的soap源码放在F:\gsoap-2.8上了。
第四步：要注意的地方：
               至此，其实gsoap要导出的东西都导出了。
               把导出的文件都加入到工程内，要注意的是，会出现一个xxx.nsmap的文件。
               这个文件需要当成是头文件那样include进去。
               根据WSDL，一般的接口假设叫sendMessage，那么就会有如下：
               参数表用到的 传入参数用的类  _ns1__sendMessage  和   传出参数用的类 _ns1__sendMessageResponse
               用到这些参数的函数 soap_call___ns1__sendMessage 这么一套东西。
               里面除了soap操作类外，要填写的endpoint和action都会自动填写好。
配合Qt5.8在win和ubuntu16.04上都测试通过了。
demo紧供参考，因为demo内的网址是内网，代码也是通过内网的WSDL导出，对一般使用没有太大意义。
demo的下载地址：http://pan.baidu.com/s/1boTyDn5
--------------------- 
作者：dronly 
来源：CSDN 
原文：https://blog.csdn.net/dronly/article/details/70772005 
版权声明：本文为博主原创文章，转载请附上博文链接！
