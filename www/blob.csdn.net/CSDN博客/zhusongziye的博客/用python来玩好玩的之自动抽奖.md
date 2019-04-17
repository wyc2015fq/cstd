# 用python来玩好玩的之自动抽奖 - zhusongziye的博客 - CSDN博客





2018年08月13日 21:40:47[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：469








不知道你们有没有玩过无码科技的小程序抽奖助手，没有玩过的可以在微信小程序入门搜索抽奖助手，首页有很多奖品进行抽奖的，我前几天发现了之后就把那里的所有奖品都点了一次，就突发萌想，能不能用python来实现自动抽奖啊？这样就不用我每天都点进去看了，我只需要关心是否中奖就可以了。答案是肯定的，今天就为大家带来如何实现自动抽奖，解放双手。



**需要知识**
- 
会利用charles对手机进行抓包

- 
会用requests库




嗯，就需要这么多知识。如果不会抓包的话请自行百度，这里就不一一展开说了。



**1.分析页面**



我们先打开charles进行准备，然后打开小程序抽奖助手的首页，可以看到有每日福利和自助福利两个专栏，自助福利的下端还有个加载更多的按钮，这些都是我们需要进行模拟请求的。



![](https://img-blog.csdn.net/20180813213612669?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





![](https://img-blog.csdn.net/20180813213631895?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





![](https://img-blog.csdn.net/20180813213752376?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



现在就可以去看看charles里面抓包的数据，很容易就发现下面这几个请求都是一一对应上面的图片的数据的。

![](https://img-blog.csdn.net/20180813213810652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





![](https://img-blog.csdn.net/20180813213830452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

获得每日福利的奖品数据的请求url为**https://lucky.nocode.com/public_lottery?page=1&size=5**，而获得自助福利的奖品数据的请求url为**https://lucky.nocode.com/square**，点击加载更多的请求url为**https://lucky.nocode.com/square?cursor=355&start=447**，只是比上面的多了两个参数，三个url请求方式都是get，可以说是基本不会变的url。这里需要注意的是，我们在请求时，添加请求头需要带上**authorization**参数，这个翻译过来时授权的意思，其实就是用来验证身份的，可以说通过加上这个就是登陆上了微信。



接下来就是看看如何点击抽奖按钮之后是如何请求的了。



![](https://img-blog.csdn.net/20180813213843796?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



我们先点击下每日福利的抽奖，在charles可以看到这个请求



![](https://img-blog.csdn.net/2018081321385712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



请求url为https://lucky.nocode.com/lottery/**奖品id**/join，url和变化的，里面有该奖品的id，请求方式是post，from-data里面的数据根据经验可以知道这是个13位的时间戳。这个搞定了，再看看自助福利的有什么不用。



![](https://img-blog.csdn.net/2018081321392379?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



可以看到，请求的都是一样的，就不多说了。



**2.代码实现**



这个是获取每日福利的实现



```
def __get_public_draw(self):
       url = 'https://lucky.nocode.com/public_lottery?page=1&size=5'
       response = requests.get(url, headers=self.headers, verify=False)
       if response.status_code == 200:
           for prize in response.json()['data']:
               if not prize['joined'] and '不满足' not in prize.get('condition_error', '1'):
                   yield prize['id'], prize['prizes']['data'][0]['name']

       else:
           print('请求失败,状态码为%s' % response.status_code)
```

可以左右滑动

这里需要注意的是，进行请求的时候需要带上参数verify关键字参数，这个是设置不验证证书，如果不设置就会报错



![](https://img-blog.csdn.net/20180813214004611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


`OpenSSL.SSL.Error: [('SSL routines', 'ssl3_get_server_certificate', 'certificate verify failed')]`
证书验证失败

接着是自助福利的代码实现



```
def __get_welfare(self):
       urls = ['https://lucky.nocode.com/square', 'https://lucky.nocode.com/square?cursor=293&start=394']
       for url in urls:
           response = requests.get(url, headers=self.headers, verify=False)
           if response.status_code == 200:
               for prize in response.json()['data']:
                   if not prize['joined']:
                       yield prize['id'], prize['initiator']['nick_name']
```



这里有两个url，第二个url里面的参数可以设置成固定的，其他的就不多说了。



最后还有自动抽奖的实现



```
def __draw_prize(self, prize):
       print('正在对%s的奖品进行抽奖' % prize[1])
       url = 'https://lucky.nocode.com/lottery/%s/join' % prize[0]
       data = {
           'form_id': int(time.time()*1000)
       }
       response = requests.post(url, data=data, headers=self.headers, verify=False).json()
       if response.get('data', 0):
           print('抽奖成功')
       else:
           print('抽奖失败')
```



因为time.time()的结果是带只有十位整数的float数值，所以需要乘于1000再取整数即可。其他的都没什么问题了。



**最后**



这个代码大家可以每天运行一次，这样就可以把所有的奖品基本都参数抽奖了，中不中是另外一回事，反正我每次都是这样



![](https://img-blog.csdn.net/20180813214017238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



我也只能祝你们好运了。



