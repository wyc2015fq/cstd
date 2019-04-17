# 基于Selenium的Web自动化测试实战&面试题以及答案整理 - zhusongziye的博客 - CSDN博客





2018年11月10日 21:51:19[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：642








**1.你如何用Selenium测试？**

Selenium+Maven+TestNG+Jekins



**2.你在测试中常遇到哪些问题？**

这个常见的比如定位问题，



**3.如何解决问题？**

先思考，然后百度，考虑网速、电脑配置等原因，这题主要看重解决问题的能力和思维。



**4.Selenium的工作原理？**

可以结合图片进行解释。脚本启动driver，然后启动浏览器作为远程服务器，执行脚本发送请求，服务器解析请求作出相应的操作，反馈给客户端（脚本）。

![](https://img-blog.csdnimg.cn/201811102150418.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**5.常见的定位方式？**

常见8大定位（id/name/tagName/classname/linkText/PatrilLinkText/Xpath/cssselector）+层级和表单。



**6.上传图片的几种方式？**

如sendkeys,利用AutoIT工具实现



**7.如何处理动态元素？**

用Xpath定位



***✎✎✎第二小节***



**8.自动化测试用例怎么来？**

一般来源于冒烟测试测试



**9.你是怎么开发测试框架的？**

Selenium+Maven+TestNG+Jekins



**10.做自动化测试的目的？**

a.用来回归  b.用来监控系统稳定性 c.提高工作效率



**11.PO模型是怎么做的？**

pageObject ：页面层，操作层，业务层集合成case



**12.如何判断case是否通过？**

断言，与预期结果比较



**13.你项目中有多少case，通过率怎么样？**

按实际回答，上千个，一般是70-80%左右，



**14.失败截图的原理是什么？**

利用监听，重写TestNG的失败方法进行截图。



**15.报告哪里来，主要统计什么？**

报告由测试用例执行情况，如TestNG生成，主要统计成功、失败率，截图和记录日志。



