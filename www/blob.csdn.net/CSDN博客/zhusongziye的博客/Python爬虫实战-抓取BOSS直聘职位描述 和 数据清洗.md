# Python爬虫实战 - 抓取BOSS直聘职位描述 和 数据清洗 - zhusongziye的博客 - CSDN博客





2018年11月25日 10:45:13[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：232








## 一、抓取详细的职位描述信息

### 详情页分析

![](https://img-blog.csdnimg.cn/20181125104341932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

在详情页中，比较重要的就是`职位描述`和`工作地址`这两个

由于在页面代码中`岗位职责`和`任职要求`是在一个 div 中的，所以在抓的时候就不太好分，后续需要把这个连体婴儿，分开分析。

### 爬虫用到的库

**使用的库有:**

```
requests
BeautifulSoup4
pymongo
```

Python 代码

```
"""
@author: jtahstu
@contact: root@jtahstu.com
@site: http://www.jtahstu.com
"""
# -*- coding: utf-8 -*-
import requests
from bs4 import BeautifulSoup
import time
from pymongo import MongoClient
headers = {
    'x-devtools-emulate-network-conditions-client-id': "5f2fc4da-c727-43c0-aad4-37fce8e3ff39",
    'upgrade-insecure-requests': "1",
    'user-agent': "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
    'accept': "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8",
    'dnt': "1",
    'accept-encoding': "gzip, deflate",
    'accept-language': "zh-CN,zh;q=0.8,en;q=0.6",
    'cookie': "__c=1501326829; lastCity=101020100; __g=-; __l=r=https%3A%2F%2Fwww.google.com.hk%2F&l=%2F; __a=38940428.1501326829..1501326829.20.1.20.20; Hm_lvt_194df3105ad7148dcf2b98a91b5e727a=1501326839; Hm_lpvt_194df3105ad7148dcf2b98a91b5e727a=1502948718; __c=1501326829; lastCity=101020100; __g=-; Hm_lvt_194df3105ad7148dcf2b98a91b5e727a=1501326839; Hm_lpvt_194df3105ad7148dcf2b98a91b5e727a=1502954829; __l=r=https%3A%2F%2Fwww.google.com.hk%2F&l=%2F; __a=38940428.1501326829..1501326829.21.1.21.21",
    'cache-control': "no-cache",
    'postman-token': "76554687-c4df-0c17-7cc0-5bf3845c9831"
}
conn = MongoClient('127.0.0.1', 27017)
db = conn.iApp  # 连接mydb数据库，没有则自动创建
def init():
    items = db.jobs_php.find().sort('pid')
    for item in items:
        if 'detail' in item.keys(): # 在爬虫挂掉再此爬取时，跳过已爬取的行
            continue
        detail_url = "https://www.zhipin.com/job_detail/%s.html?ka=search_list_1" % item['pid']
        print(detail_url)
        html = requests.get(detail_url, headers=headers)
        if html.status_code != 200: # 爬的太快网站返回403，这时等待解封吧
            print('status_code is %d' % html.status_code)
            break
        soup = BeautifulSoup(html.text, "html.parser")
        job = soup.select(".job-sec .text")
        if len(job) < 1:
            continue
        item['detail'] = job[0].text.strip()  # 职位描述
        location = soup.select(".job-sec .job-location")
        item['location'] = location[0].text.strip()  # 工作地点
        item['updated_at'] = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())  # 实时爬取时间
        res = save(item) # 保存数据
        print(res)
        time.sleep(40) # 停停停
# 保存数据到 MongoDB 中
def save(item):
    return db.jobs_php.update_one({"_id": item['_id']}, {"$set": item})
if __name__ == "__main__":
    init()
```

代码 easy，初学者都能看懂。



## 二、数据清洗

**2.1 校正发布日期**

```
"time" : "发布于03月31日",
"time" : "发布于昨天",
"time" : "发布于11:31",
```

这里拿到的都是这种格式的，所以简单处理下

```
import datetime
from pymongo import MongoClient
db = MongoClient('127.0.0.1', 27017).iApp
def update(data):
    return db.jobs_php.update_one({"_id": data['_id']}, {"$set": data})
# 把时间校正过来
def clear_time():
    items = db.jobs_php.find({})
    for item in items:
        if not item['time'].find('布于'):
            continue
        item['time'] = item['time'].replace("发布于", "2017-")
        item['time'] = item['time'].replace("月", "-")
        item['time'] = item['time'].replace("日", "")
        if item['time'].find("昨天") > 0:
            item['time'] = str(datetime.date.today() - datetime.timedelta(days=1))
        elif item['time'].find(":") > 0:
            item['time'] = str(datetime.date.today())
        update(item)
    print('ok')
```

**2.2 校正薪水以数字保存**

```
'''
"salary" : "5K-12K",
#处理成下面的格式
"salary" : {
    "low" : 5000,
    "high" : 12000,
    "avg" : 8500.0
},
'''
# 薪水处理成数字，符合 xk-yk 的数据处理，不符合的跳过
def clear_salary():
    items = db.jobs_lagou_php.find({})
    for item in items:
        if type(item['salary']) == type({}):
            continue
        salary_list = item['salary'].lower().replace("k", "000").split("-")
        if len(salary_list) != 2:
            print(salary_list)
            continue
        try:
            salary_list = [int(x) for x in salary_list]
        except:
            print(salary_list)
            continue
        item['salary'] = {
            'low': salary_list[0],
            'high': salary_list[1],
            'avg': (salary_list[0] + salary_list[1]) / 2
        }
        update(item)
    print('ok')
```

这里在处理 Boss 直聘的数据时，比较简单正常，但是后续抓到拉勾网的数据，拉勾网的数据有些不太规范。比如有‘20k以上’这种描述
**2.3 根据 工作经验年限 划分招聘等级**

```
# 校正拉勾网工作年限描述，以 Boss直聘描述为准
def update_lagou_workyear():
    items = db.jobs_lagou_php.find({})
    for item in items:
        if item['workYear'] == '应届毕业生':
            item['workYear'] = '应届生'
        elif item['workYear'] == '1年以下':
            item['workYear'] = '1年以内'
        elif item['workYear'] == '不限':
            item['workYear'] = '经验不限'
        update_lagou(item)
    print('ok')
# 设置招聘的水平，分两次执行
def set_level():
    items = db.jobs_zhipin_php.find({})
    # items = db.jobs_lagou_php.find({})
    for item in items:
        if item['workYear'] == '应届生':
            item['level'] = 1
        elif item['workYear'] == '1年以内':
            item['level'] = 2
        elif item['workYear'] == '1-3年':
            item['level'] = 3
        elif item['workYear'] == '3-5年':
            item['level'] = 4
        elif item['workYear'] == '5-10年':
            item['level'] = 5
        elif item['workYear'] == '10年以上':
            item['level'] = 6
        elif item['workYear'] == '经验不限':
            item['level'] = 10
        update(item)
    print('ok')
```

这里有点坑的就是，一般要求经验不限的岗位，需求基本都写在任职要求里了，所以为了统计的准确性，这个等级的数据，后面会被舍弃掉。





转载声明

作者：jtahstu

源自：http://www.jtahstu.com/blog/scrapy_zhipin_detail.html



