# Solve Error: "errcode": 48001, "errmsg": "api unauthorized hint" - Grandyang - 博客园







# [Solve Error: "errcode": 48001, "errmsg": "api unauthorized hint"](https://www.cnblogs.com/grandyang/p/10049126.html)







当你想给微信公众号（不是测试账号）自定义菜单创建接口，遇到如下错误：



```
200    OK
Connection: keep-alive
Date: Sat, 01 Dec 2018 05:02:08 GMT
Content-Type: application/json; encoding=utf-8
Content-Length: 69
{
    "errcode": 48001, 
    "errmsg": "api unauthorized hint: [fCbXNA0528vr65!]"
}
```



很可能的原因是你并没有自定义菜单接口的权限，去【开发】->【接口权限】 界面去查看“自定义菜单”的权限，如果显示 “未获得”，那么恭喜你，用不了。而显示的获得条件是【1】订阅号必须通过[微信认证](https://mp.weixin.qq.com/acct/wxverifyorder?action=index&token=468262775&lang=zh_CN) 【2】服务号自动获得。但是坑爹的是，个人申请的订阅号（2014年8月26日之后申请的）无法微信认证，而个人又无法申请服务号。如果使用【功能】中的【自定义菜单】，只能回复固定的文本或者图文，而且没法加入自己的回复逻辑，而用自己的服务器，就没法自定义菜单，我也是醉了····














