# Onvif框架代码生成 - 建建的博客 - CSDN博客
2017年02月09日 14:20:35[纪建](https://me.csdn.net/u013898698)阅读数：1104
## 1:Onvif简介
      安防产品的通用标准目前有两大主流阵营：Onvif以及Psia，从目前厂家的支持数量以及市场占有量来说，Onvif已经牢牢占据了领先地位，且在网络视频监控领域，优势地位更明显。下面摘录百科的描述来给Onvif做下简单介绍：
      2008年5月，由安讯士联合博世及索尼三方宣布将携手共同成立一个国际开放型网络视频产品标准网络接口开发论坛，取名为ONVIF（Open Network Video Interface Forum，开放型网络视频接口论坛），并以公开、开放的原则共同制定开放性行业标准。
      ONVIF标准将为网络视频设备之间的信息交换定义通用协议，包括装置搜寻、实时视频、音频、元数据和控制信息等。网络视频产品由此所能提供的多种可能性，使终端用户，集成商，顾问和生产厂商能够轻松地从中获益，并获得高性价比、更灵活的解决方案、市场扩张的机会以及更低的风险。
据了解，目前有部分企业和电信运营商已经制定了与之相关的业内规范，也有一些企业联盟提出了一些互联互通的企业标准。ONVIF则是面向全球，出发点是制订一个完全开放的标准。这项标准的主要目标在于推动不同品牌网络视频设备之间的整合，协助制造商，软件开发人员以及独立软件厂商确保产品的互联互通性，很适合于企业级市场。
从ONVIF的官方网站上可了解，从去年年底已经正式对外招收会员。同时为适应各种不同的参与级别，ONVIF还提供了不同等级的会员资格，可以选择高级、中级或用户会员级别。论坛接纳的会员已有近40家。其中，高级会员有12家，除了三家发起公司外，松下、三星、思科、西门子、TI等著名品牌位列其中，而国内的海康威视、浙江大华、波粒科技和佳信捷也是ONVIF论坛的高级会员。
推行标准的过程也是净化市场的重要手段，有利于整合安防市场资源，减少大量重复开发的劳动，和无谓的人力、物力和时间消耗，提高产品的可靠性。更大的互通和兼容性有利于产业的集体升级，最终给消费者带来更大的实惠。相信统一的开放型标准平台将极大促进安防监控业的健康发展。
##  2：开发框架搭建
     1：gsoap官网([http://gsoap2.sourceforge.net/](http://gsoap2.sourceforge.net/))下载最新版gsoap(本次版本为gsoap_2.8.17)并解压。
     2：新建一个文件夹（OnvifFramework）
           将解压的gsoap_2.8.17文件拷贝至OnvifFramework目录下
           将gsoap_2.8.17\gsoap-2.8\gsoap 路径下的typemap.dat以及gsoap_2.8.17\gsoap-2.8\gsoap\bin\win32 路径下的wsdl2h.exe  soapcpp2.exe也拷贝至该目录下。
     3： Onvif官网（**[http://www.onvif.org/Documents/Specifications.aspx](http://www.onvif.org/Documents/Specifications.aspx)**）页面里ONVIF WSDL and XML Schemas
 Specifications找到各wsdl（18个左右）并下载下来
           （右键另存为即可），放入GenerateSource文件夹中。
     4：生成onvif.h
           生成方式有在线生成和本地生成两种方式。由于网络的原因，在线生成很容易中断。故临时改用本地生成方式，在实际运行时发现，本地生成过程中也需要从网络上下载
           相关的依赖文件，并且对文件路径有如下要求： 
           OnvifFramework
                         ----level1
                              ----level2
                                   ----GenerateSource
                                         ----typemap.dat
                                         ----wsdl2h.exe
                                         ----soapcpp2.exe
                                         ----remotediscovery.wsdl  
                                         ----devicemgmt.wsdl
                                         ----analytics.wsdl
                                         ----analyticsdevice.wsdl 
                                         ----media.wsdl  
                                         ----deviceio.wsdl  
                                         ----display.wsdl  
                                         ----event.wsdl
                                         ----imaging.wsdl  
                                         ----recording.wsdl  
                                         ----replay.wsdl 
                                         ----search.wsdl  
                                         ----receiver.wsdl  
                                         ----ptz.wsdl
                                   ----ver10
                                          ----device
                                                 ----wsdl
                                                        ----devicemgmt.wsdl
                                          ----media
                                                ----wsdl
                                                      ----media.wsdl
                                         ----schema
                                               ----onvif.xsd
                       ----ver10
                             ----schema
                                    ----onvif.xsd
          管理员权限打开CMD，切换至GenerateSource目录下，将如下指令贴到CMD窗口中回车运行即可生成onvif.h文件：
          wsdl2h -o onvif.h  -c  -s -t  ./typemap.dat  remotediscovery.wsdl  devicemgmt.wsdl analytics.wsdl  analyticsdevice.wsdl  media.wsdl  deviceio.wsdl  display.wsdl  event.wsdl imaging.wsdl
  recording.wsdl  replay.wsdl search.wsdl  receiver.wsdl  ptz.wsdl
          结果信息如下图所示：
![](https://img-blog.csdn.net/20140529114600343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2Fsb29uX3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        注：生成的时候可能会出现如下错误：
                wsa5.h(290): **ERROR**: remote method nameclash: struct/class 'SOAP_ENV__Fault' already declared at line 274
                解决办法如下：
                打开gsoap_2.8.17\gsoap-2.8\gsoap\import 路径下的wsa5.h， 将277行的SOAP_ENV__Fault结构体注释掉（改成其他名字亦可）
      5：通过onvif.h生成源代码框架
            将如下指令贴到CMD窗口中回车运行即可：
            soapcpp2 -2  -c  onvif.h  -x -I  ..\..\..\gsoap_2.8.17\gsoap-2.8\gsoap\import -I ..\..\..\gsoap_2.8.17\gsoap-2.8\gsoap
            看到下图中的Compilation successful即可：
![](https://img-blog.csdn.net/20140529114449953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2Fsb29uX3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
       6：生成文件代码文件如下：
![](https://img-blog.csdn.net/20140529114640453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2Fsb29uX3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
                 最后做如下修改
                 1.后缀为nsmap的命名空间文件内容都一样，保留一个即可，后缀改为.h文件（wsdd.nsmap更名为wsdd.h），需要的时候包含此文件即可
                 2.删除soapClientLib.c和soapServerLib.c文件。
                 3.将gsoap-2.8.17/gsoap-2.8/gsoap下的stdsoap2.h和stdsoap2.c文件和gsoap-2.8.17/gsoap-2.8/gsoap/custom下的duration.c文件都拷贝到当前目录下
                最终的文件如下：
![](https://img-blog.csdn.net/20140529133708578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2Fsb29uX3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
           至此Onvif的开发环境框架已经完成，客户端的开发的话，使用soapClient.c文件；服务端开发的话，使用soapServer.c即可。
## [转载地址](http://blog.csdn.net/saloon_yuan/article/details/24901597)
