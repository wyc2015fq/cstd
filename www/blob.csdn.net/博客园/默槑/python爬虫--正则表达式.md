# python爬虫--正则表达式 - 默槑 - 博客园







# [python爬虫--正则表达式](https://www.cnblogs.com/modaidai/p/6995753.html)





一、

re.search

re.search(r'(fish)\1','fish') #这是匹配不上的，因为后面还有一个'\1'，表示括号里面的东西

re.search(r'[\]','fish')　　　#会报错，反斜线不能直接放进中括号

e=re.search(r'\w','fish:fish')　　print(e.group())#会得到fish

 　　　　　　　　　　　　　　　　print(e.start())#会得到0

　　　　　　　　　　　　　　　　print (e.span())#会得到(0,4)












