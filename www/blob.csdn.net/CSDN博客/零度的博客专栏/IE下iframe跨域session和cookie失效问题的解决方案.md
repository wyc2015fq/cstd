# IE下iframe跨域session和cookie失效问题的解决方案 - 零度的博客专栏 - CSDN博客
2016年05月20日 16:19:58[零度anngle](https://me.csdn.net/zmx729618)阅读数：388
                
问题来源：
#### 何为跨域跨域session/cookie？
也就是第三方session/cookie。第一方session/cookie指的是访客当前访问的网站给访客的浏览器设置的seesion/cookie, 会被存储在访客的计算机上。第三方session/cookie指的是当前访问的网站中会加载（嵌入）另外第三方的网站代码，例如促销广告，那么第三方网站也会在访客的计算机上添加session/cookie，这种就是第三方session/cookie。
#### IE限制第三方session/cookie
随着IE版本的不断更新，版本之间变化很大，其兼容性问题困扰着许多开发者。本问题也不例外，IE7以后，微软逐渐改进了IE安全性，其中默认设置下第三方session/cookie是不允许使用的，这就造成了使用iframe嵌入式访问另外的第三方网站时，不能为其保存会话状态，无法进行登录或跨越取值，从而影响第三方网站功能的使用。
### 解决方法
#### 手动调整客户端IE浏览器的安全级别
在Internet选项-隐私卡中，调低安全级别到接受所有cookie，或者在[高级]中设置接受第三方cookie。
点评：此方法需要使用者更改客户端浏览器设置，极不便利，且会给使用者电脑带来安全隐患，故不推荐。
#### 代码中使用P3P协议自动更改IE浏览器安全级别
P3P（Platform for Privacy Preferences）是一种可以提供这种个人隐私保护策略。具体做法是在被iframe嵌入的第三方网站代码中加入如下代码： 
**[java]**[view
 plain](http://blog.csdn.net/wauit/article/details/9875157#)[copy](http://blog.csdn.net/wauit/article/details/9875157#)
- response.setHeader("P3P","CP=\"IDC DSP COR ADM DEVi TAIi PSA PSD IVAi IVDi CONi HIS OUR IND CNT\"");  
要注意的有：
1、上段代码是jsp的，如果是asp或php等，需要改成相应的语法，参数和取值不变。如ruby为：
**[ruby]**[view
 plain](http://blog.csdn.net/wauit/article/details/9875157#)[copy](http://blog.csdn.net/wauit/article/details/9875157#)
- response.headers["P3P"] = "CP=\"IDC DSP COR ADM DEVi TAIi PSA PSD IVAi IVDi CONi HIS OUR IND CNT\""
2、此方法仅支持动态Web应用，也就是需要使用动态语言设置response的header，且如果是MVC架构的话，最好是在总控制器或过滤器中添加上段代码，这样改动最小。
点评：该方法不需要手动修改客户端IE设置，但需要修改第三方网站的代码，对于第三方网站不在控制范围内情况下无能为力。
#### 在第三方网站服务器中设置P3P协议
第三方网站所使用的应用服务器程序，如果支持设置HTTP头，那么可以通过设置服务器而不必修改第三方网站代码。
例如IIS下，可以打开IIS窗口——〉选择一个网站——〉属性——〉http头，增加一个http头
然后输入头名：P3P
输入头内容：CP=CAO PSA OUR
点评：与上一个方法类似，此方法也要求第三方在可控可管理。
