# IE与COOKIE - 左直拳的马桶_日用桶 - CSDN博客
2010年04月19日 18:21:00[左直拳](https://me.csdn.net/leftfist)阅读数：1183标签：[ie](https://so.csdn.net/so/search/s.do?q=ie&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
IE下，同名不同域的COOKIE有机会变成紊乱。例如：HttpCookie cookie = Request.Cookies[_CookieSSO];，得到的COOKIE可能会吓你一跳，本来是多值的一个COOKIE，里面只有一个值，并且这个值可能是N年前设置的，你都不知道它从哪来的。
将COOKIE清空或删除相关的COOKIE就一点问题没有了。
但FF就没见过有这种问题。
IE就是烂，烂到我一见到它那种界面就烦，觉得很低档。
