# python模拟艺龙网登录requests包 - _天枢 - 博客园
## [python模拟艺龙网登录requests包](https://www.cnblogs.com/yhleng/p/6767364.html)
2017-04-26 10:35 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=6767364)
对比urllib、urllib2与requests不难发现，前者功能更强大，但是实现一个功能要写很多的代码，后者，requests代码简洁，用起来更快速
下面一个模拟登录的代码：看看吧一共也没有几行就实现了。
```
#-*- coding:utf-8 -*-
from requests import Request,Session
import json
s =Session()
codeurl = 'https://secure.elong.com/passport/getValidateCode'
loginurl='https://secure.elong.com/passport/ajax/elongLogin'
code = s.get(codeurl)//这句不加会提示验证码错误
#print code.content
postData = {'userName':'13718651668',
            'passwd':'yhlexx8701xx',
            'validateCode':'1234',
            'rememberMe':'false'}
reponse = s.post(loginurl,data=postData)
print reponse.text
#如果需要取结果中的值做登录后的后续操作，可以使用json方式，很方便
esid = json.loads(reponse.text)["esid"]
print esid
c ='http://my.elong.net/Connection_en.html?SessionTag='+esid+'nextUrl=http%3A%2F%2Fmy.elong.com%2FCashCoupon_cn.html&expireTime=0'
r = s.get(c)
print r.content
```
![](https://images2015.cnblogs.com/blog/1149221/201704/1149221-20170426103116819-479277761.png)
![](https://images2015.cnblogs.com/blog/1149221/201704/1149221-20170426103929319-1625874255.png)
登录是成功的，但是从返回的结果中可以看出，本次登录不需要验证码输入。
有一些网站，只要不输入密码错误，是不是需要输入验证码的。
猜测这个是在session中做的记录，这里呢每次执行重新创建session，即使密码错误也不会提示输入验证码。竞然把验证码避过了。
我也不是很明白，有懂的同学，给回复下。学习一下，谢谢！
