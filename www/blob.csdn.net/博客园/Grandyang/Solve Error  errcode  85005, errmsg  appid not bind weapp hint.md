# Solve Error: "errcode": 85005, "errmsg": "appid not bind weapp hint" - Grandyang - 博客园







# [Solve Error: "errcode": 85005, "errmsg": "appid not bind weapp hint"](https://www.cnblogs.com/grandyang/p/8285373.html)







在使用微信官方给的添加自定义菜单的示例代码：



```
{
     "button":[
     {    
          "type":"click",
          "name":"今日歌曲",
          "key":"V1001_TODAY_MUSIC"
      },
      {
           "name":"菜单",
           "sub_button":[
           {    
               "type":"view",
               "name":"搜索",
               "url":"http://www.soso.com/"
            },
            {
                 "type":"miniprogram",
                 "name":"wxa",
                 "url":"http://mp.weixin.qq.com",
                 "appid":"wx286b93c14bbf93aa",
                 "pagepath":"pages/lunar/index"
             },
            {
               "type":"click",
               "name":"赞一下我们",
               "key":"V1001_GOOD"
            }]
       }]
 }
```



可能会遇到如下的错误：

```
Connection: keep-alive
Date: Sun, 14 Jan 2018 17:50:18 GMT
Content-Type: application/json; encoding=utf-8
Content-Length: 72
{
    "errcode": 85005, 
    "errmsg": "appid not bind weapp hint: []"
}
```



原因出在小程序的链接上，官方只是给了个例子教你如何添加小程序链接到菜单栏，但是这个小程序并不是归属于你的，所以错误提示说appid没有绑定，这里的appid是小程序的，当然没有和你的绑定，所以只要将这段整个删掉就好了：



```
{
    "button": [
        {
            "type": "click", 
            "name": "今日歌曲", 
            "key": "V1001_TODAY_MUSIC"
        }, 
        {
            "name": "菜单", 
            "sub_button": [
                {
                    "type": "view", 
                    "name": "搜索", 
                    "url": "http://www.soso.com/"
                }, 
                {
                    "type": "click", 
                    "name": "赞一下我们", 
                    "key": "V1001_GOOD"
                }
            ]
        }
    ]
}
```














