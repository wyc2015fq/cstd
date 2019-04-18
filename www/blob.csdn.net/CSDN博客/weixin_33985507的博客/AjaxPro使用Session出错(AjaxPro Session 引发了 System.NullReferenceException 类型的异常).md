# AjaxPro使用Session出错(AjaxPro "Session"引发了"System.NullReferenceException"类型的异常) - weixin_33985507的博客 - CSDN博客
2011年08月17日 07:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
关于在ASP.NET如何使用AjaxPro，这里就不多说了，先看代码:
    [AjaxPro.AjaxMethod(AjaxPro.HttpSessionStateRequirement.ReadWrite)]
publicstring btnNextZX(string data)
    {
        StringBuilder zx_Ret =new StringBuilder();
        Hashtable hash = (Hashtable)Session["jgd_data"];
![](https://www.cnblogs.com/Images/dot.gif)![](https://www.cnblogs.com/Images/dot.gif)
注意：这里排除Session中没有["jgd_data"]这个数据而引起的出错，当然即使不存在也报的并不是如下的错误：
![](https://images.cnblogs.com/cnblogs_com/over140/10%E6%9C%88/20071019.JPG)
很奇怪！我明明加了如下代码啊！！
[AjaxPro.AjaxMethod(AjaxPro.HttpSessionStateRequirement.ReadWrite)]
### 解决办法：
将如下代码行：
Hashtable hash = (Hashtable)Session["jgd_data"];
替换成如下代码：
Hashtable hash = (Hashtable）HttpContext.Current.Session["jgd_data"];
执行OK!!
