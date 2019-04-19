# 从JavaWeb危险字符过滤浅谈ESAPI使用 - =朝晖= - 博客园
# [从JavaWeb危险字符过滤浅谈ESAPI使用](https://www.cnblogs.com/dhcn/p/7105459.html)
      事先声明：只是浅谈，我也之用了这个组件的一点点。
      又到某重要XX时期(但愿此文给面临此需求的同仁有所帮助)，某Web应用第一次面临安全加固要求，AppScan的安全[测试](http://lib.csdn.net/base/softwaretest)报告还是很清爽的，内容全面，提示建议到位，而且是中午哦，当然有的中文明显狗屁不通。
      之前此应用的后端[架构](http://lib.csdn.net/base/architecture)相对比较稳固，所以出的重要问题主要出在靠近前端方向的问题，一些类似输出过滤这样的动作做的不到位，需要引入比较成熟的代码专门干这些活，翻了一下吴瀚清的同学《白帽子谈Web安全》推荐了OWASP的ESAPI，这个东西全面叫Enterprise Security API，官网地址：[https://www.owasp.org/index.php/Category:OWASP_Enterprise_Security_API](https://www.owasp.org/index.php/Category:OWASP_Enterprise_Security_API) 到其主页上看看，好像提供了不少语言的分支版本，我就下了个JavaEE的版本，这个玩意功能很强大，直接来官方文档的特性列表吧：
- The features in this release of ESAPI for Java EE include:  
-     ESAPI Core components  
-         ESAPI locator and interface classes.  
-         ESAPI security control reference implementations for the following security controls:  
-         Authentication  
-         Identity  
-         Access Control  
-         Input Validation  
-         Output Escaping  
-         Encryption  
-         Random Numbers  
-         Exception Handling  
-         Logging  
-         Intrusion Detection  
-         Security Configuration  
-     ESAPI Web Application Firewall (WAF) component  
-     Fixes for specific issues. For more information, see “Enhancements and Resolved Issues”.  
      这个组件使用不是直接引入jar就ok，初始化时要读两个配置文件ESAPI.properties和validation.properties,这两个配置文件不一定能在安装指南所示的目录中找到，不过你解压搜索一下dist目录，应该是可以找到的，把这两个文件扔进src目录中就Ok了。
      再有什么可说的，来代码，我就用了一下DefaultEncoder中的一些EncodeForXXX功能，基本上都是getInstance()单例方式一句搞定,也没啥好说的，大家看看文档谁都会。那写这Blog干什么？主要想说现在中文网上搜到的关于JavaWeb输出过滤大都是一些人(其实就一个版本)自己写的代码，不是说他的代码不好，引入相对成熟经过一定使用检验的组件级代码总是相对更好一点，再怎么说，这些东西都是安全相关的东东，不可小视。

