
# WebService-（三）springMVC集成CXF后调用已知的wsdl接口 - wangzhiguo9261的博客 - CSDN博客


2018年02月25日 15:51:49[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：88


# [webService学习之路（三）：springMVC集成CXF后调用已知的wsdl接口](http://www.cnblogs.com/xiaochangwei/p/5400303.html)
[webService学习之路一：讲解了通过传统方式怎么发布及调用webservice](http://www.cnblogs.com/xiaochangwei/p/4969448.html)
[webService学习之路二：讲解了SpringMVC和CXF的集成及快速发布webservice](http://www.cnblogs.com/xiaochangwei/p/5399507.html)
本篇文章将讲解SpringMVC+CXF环境下，怎么调用其他系统通过webService方式暴露出来的接口
① 为避免怀疑同一个项目中调用本项目的接口，这里我新打开一个eclipse通过最原始的方式发布了一个webservice并启动保证可以被访问
![](https://images2015.cnblogs.com/blog/658609/201604/658609-20160417082354988-734939147.png)
打开浏览器确认可以被访问
![](https://images2015.cnblogs.com/blog/658609/201604/658609-20160417082546035-1880281499.png)
②进入CXF/bin 利用wsdl2java工具将webService接口转换为可供调用的java类。调用方式wsdl2java   XXXXX?wsdl
![](https://images2015.cnblogs.com/blog/658609/201604/658609-20160417082827535-1529318362.png)
执行完成后，你会发现bin目录下多了一个文件夹，文件夹路径和接口包路径一致
![](https://images2015.cnblogs.com/blog/658609/201604/658609-20160417083032223-484262412.png)

③ 将生成的java文件拷贝到需要调用接口的项目中，请注意保持包的路径一致。
![](https://images2015.cnblogs.com/blog/658609/201604/658609-20160417083525051-753425306.png)
④ 接口的调用，前面的都是准备工作。 o(∩_∩)o
继webService学习之路二基础上，集成好CXF后并保证xml中命名空间都已经正确
在applicationContext.xml中添加如下配置：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<import resource="classpath:META-INF/cxf/cxf.xml"/>
    <!-- 注意：serviceClass为接口类并非实现类 -->
    <!-- <jaxws:server serviceClass="com.xiaochangwei.web.service.WebserviceTest"address="/webSerciceTest_service_jaxws"></jaxws:server> -->
    <!-- 注意：implementor为接口的具体实现类 -->
    <!-- <jaxws:endpoint implementor="com.xiaochangwei.web.service.impl.WebServiceTestImpl"address="/webSerciceTest_endpoint"></jaxws:endpoint> -->**<jaxws:client id=****"****webServiceBean****"****serviceClass=****"****webservice.server.Service****"****address****=****"****http://localhost:9999/service****"****/>**![复制代码](http://common.cnblogs.com/images/copycode.gif)
请注意address后面不需要添加?wsdl
⑤ 在Controller层写一个测试代码进行测试
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.xiaochangwei.web.controller;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import webservice.server.Service;
@Controller
@RequestMapping("webServiceTest")publicclassWebServiceTestController {
    @AutowiredprivateService**webServiceBean**;
    
    @RequestMapping("test")public@ResponseBody String test(){returnwebServiceBean.getWelcome("xiaochangwei");
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
后经自己折腾发现，controller这里的名字不需要和xml中的一致，因为采用的是ioc， by tye吧
⑥ 打开浏览器见证奇迹的时刻到了
![](https://images2015.cnblogs.com/blog/658609/201604/658609-20160417085506910-726879968.png)
并且查看接口发布端，控制台也打印出了 service called   。至此，调用webService接口结束。
怎么能这么顺利的就完成了，喜欢折腾的我怎么可能不折腾下. 经过折腾发现
① controller中注入的接口实例名称可以和xml中的不一样，因为采用的是ioc by type， 最开始我还以为和普通bean getbean(id)不同，必须一样呐
![复制代码](http://common.cnblogs.com/images/copycode.gif)
packagecom.xiaochangwei.web.controller;importorg.springframework.beans.factory.annotation.Autowired;importorg.springframework.stereotype.Controller;importorg.springframework.web.bind.annotation.RequestMapping;importorg.springframework.web.bind.annotation.ResponseBody;importwebservice.server.Service;
@Controller
@RequestMapping("webServiceTest")publicclassWebServiceTestController {
    @AutowiredprivateService service;
    
    @RequestMapping("test")public@ResponseBody String test(){returnservice.getWelcome("xiaochangwei ok");
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
这样也是可行的
② xml配置中，我以为可以不指定id，因为我们不用getbean(id)进行调用，经折腾发现，id是必须的 否则报 Id is required for element 'client' when used as a top-level tag
③ 我想去掉xml中的jaxws:clent 配置，而直接在生成的接口service.java 文件上增加@Component 或者 @Service来进行注入，经尝试失败了，不过我还会继续折腾的
每天折腾一点点
噢 错了，是每天进步一点点
至此 webservice学习之路全部完结，有不懂的地方和有错误的地方请及时告知我，共勉
未整理的源码下载：http://pan.baidu.com/s/1eSuIQxs （请勿用于商业项目，若需使用请完善，整理好的代码近期会更新上去）


