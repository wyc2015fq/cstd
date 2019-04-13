
# python爬虫爬取拉勾网职业信息 - lpty的博客 - CSDN博客

2017年02月03日 16:37:25[lpty](https://me.csdn.net/sinat_33741547)阅读数：13130


一、前言
最近想做一份关于拉勾网数据分析类职业的报告，便顺手写了个简单的爬虫，记录分享如下。
二、思路整理
1、首先我们打开拉勾网，并搜索“”数据分析“”，显示出来的职位便是我们的目标
2、接下来我们需要确定，怎样将信息提取出来
(1)查看页面源代码，这时候发现，页面源码里面找不到职位相关信息，这证明拉勾网关于职位的信息是异步加载的，这也是一种很常用的技术
(2)异步加载的信息，我们需要借助chrome浏览器的小工具进行分析，按F12即可打开，界面如下：
![](https://img-blog.csdn.net/20170204012750284?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(3)点击Nerwork进入网络分析界面，这时候是一片空白，刷新一下界面就可以看到一系列的网络请求了
(4)前面我们说到，拉勾网关于职位的信息是异步加载的，那么必定在这一系列的网络请求中，有某个请求发送到了服务器的接口处，响应职位信息。
(5)正常我们可以忽略css，png等类型的请求，关注点放在xhr这种类型请求上，如下：
![](https://img-blog.csdn.net/20170204012910197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(6)上图发现了两个xhr请求，从字面意思看很有可能是我们需要的信息，右键点击在另一个界面打开
![](https://img-blog.csdn.net/20170204013052554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(7)我们对比一下，上图显示的信息便是我们所要的职位信息，可以用json工具检验一下，更加直观
(8)之后再查看请求发送参数列表，到这里我们可以肯定city参数便是城市，pn参数便是页数，kd参数便是职位关键字
![](https://img-blog.csdn.net/20170204013309604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(9)思路分析到此结束了，接下来是代码了
三、代码
爬虫我按自己的习惯分成了四个部分，便于后期维护
1、基本https请求--https.py
这部分对requests包进行了一些封装，部分代码如下，完成post请求

```python
def post(self,url, para, headers=None, cookies=None, proxy=None, timeOut=5, timeOutRetry=5):
        '''
        post获取响应
        url: 目标链接
        para: 参数
        headers: headers
        cookies: cookies
        proxy: 代理
        timeOut: 请求超时时间
        timeOutRetry: 超时重试次数
        return: 响应
        '''
        if not url or not para:
            logging.error('PostError url or para not exit')
            return None
        logging.error('Post %s' % url)
        try:
            if not headers:
                headers = {'User-Agent': 'Mozilla/5.0 (Windows; U; Windows NT 5.1) Gecko/20070309 Firefox/2.0.0.3'}
            response = requests.post(url, data=para, headers=headers, cookies=cookies, proxies=proxy, timeout=timeOut)
            if response.status_code == 200 or response.status_code == 302:
                htmlCode = response.text
            else:
                htmlCode = None
            logging.error('Post %s %s' % (str(response.status_code), url))
        except Exception as e:
            logging.error('PostExcept %s' % str(e))
            if timeOutRetry > 0:
                htmlCode = self.post(url=url, para=para, timeOutRetry=(timeOutRetry-1))
            else:
                logging.error('PostTimeOut %s' % url)
                htmlCode = None
        return htmlCode
```
2、代码主逻辑部分--manage.py
这部分是程序逻辑，如下：
(1)获取职位信息

```python
url = 'https://www.lagou.com/jobs/positionAjax.json'
para = {'first': 'true','pn': '1', 'kd': kd, 'city': city}
def getInfo(url, para):
    """
    获取信息
    """
    generalHttp = Http()
    htmlCode = generalHttp.post(url, para=para, headers=hd, cookies=ck)
    generalParse = Parse(htmlCode)
    pageCount = generalParse.parsePage()
    info = []
    for i in range(1, pageCount+1):
        print('第%s页' % i)
        para['pn'] = str(i)
        htmlCode = generalHttp.post(url, para=para, headers=hd, cookies=ck)
        generalParse = Parse(htmlCode)
        info = info + getInfoDetail(generalParse)
        time.sleep(2)
    return info
```
(2)对信息进行储存

```python
def processInfo(info, para):
    """
    信息存储
    """
    logging.error('Process start')
    try:
        title = 'companyName,companyType,companyStage,companyLabel,companySize,companyDistrict,' \
                'positionType,positionEducation,positionAdvantage,positionSalary,positionWorkYear\n'
        file = open('%s.txt' % para['city'], 'a')
        file.write(title)
        for p in info:
            line = str(p['companyName']) + ',' + str(p['companyType']) + ',' + str(p['companyStage']) + ',' + \
                   str(p['companyLabel']) + ',' + str(p['companySize']) + ',' + str(p['companyDistrict']) + ',' + \
                   str(p['positionType']) + ',' + str(p['positionEducation']) + ',' + str(p['positionAdvantage']) + ',' +\
                   str(p['positionSalary']) + ',' + str(p['positionWorkYear']) + '\n'
            file.write(line)
        file.close()
        return True
    except:
        logging.error('Process except')
        return None
```
3、具体信息解析部分--parse.py
这部分针对服务器返回得职位信息特点，进行解析，如下：

```python
def parsePage(self):
        '''
        解析并计算页面数量
        :return: 页面数量
        '''
        totalCount = self.json['content']['positionResult']['totalCount']      #职位总数量
        resultSize = self.json['content']['positionResult']['resultSize']      #每一页显示的数量
        pageCount = int(totalCount) // int(resultSize) + 1          #页面数量
        return pageCount

    def parseInfo(self):
        '''
        解析信息
        '''
        info = []
        for position in self.json['content']['positionResult']['result']:
            i = {}
            i['companyName'] = position['companyFullName']
            i['companyDistrict'] = position['district']
            i['companyLabel'] = position['companyLabelList']
            i['companySize'] = position['companySize']
            i['companyStage'] = position['financeStage']
            i['companyType'] = position['industryField']
            i['positionType'] = position['firstType']
            i['positionEducation'] = position['education']
            i['positionAdvantage'] = position['positionAdvantage']
            i['positionSalary'] = position['salary']
            i['positionWorkYear'] = position['workYear']
            info.append(i)
        return info
```
4、配置部分--setting.py
这部分加入cookies的原因是为了应对拉勾网的反爬，长期使用需要进行改进，进行动态cookies获取

```python
#HEADER
headers = {'content-type': 'application/x-www-form-urlencoded; charset=UTF-8',
           'Accept-Encoding': 'gzip, deflate',
           'Host': 'www.lagou.com',
           'Origin': 'http://www.lagou.com',
           'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36',
           'X-Requested-With': 'XMLHttpRequest',
           'Referer': 'http://www.lagou.com',
           'Proxy-Connection': 'keep-alive',
           'X-Anit-Forge-Code': '0',
           'X-Anit-Forge-Token': None}
#COOKIES
cookies = {'JSESSIONID': '99021FFD6F8EC6B6CD209754427DEA93',
           '_gat': '1',
           'user_trace_token': '20170203041008-9835aec2-e983-11e6-8a36-525400f775ce',
           'PRE_UTM': '',
           'PRE_HOST': '',
           'PRE_SITE': '',
           'PRE_LAND': 'https%3A%2F%2Fwww.lagou.com%2Fzhaopin%2F',
           'LGUID': '20170203041008-9835b1c9-e983-11e6-8a36-525400f775ce',
           'SEARCH_ID': 'bfed7faa3a0244cc8dc1bb361f0e8e35',
           'Hm_lvt_4233e74dff0ae5bd0a3d81c6ccf756e6': '1486066203',
           'Hm_lpvt_4233e74dff0ae5bd0a3d81c6ccf756e6': '1486066567',
           '_ga': 'GA1.2.2003702965.1486066203',
           'LGSID': '20170203041008-9835b03a-e983-11e6-8a36-525400f775ce',
           'LGRID': '20170203041612-714b1ea3-e984-11e6-8a36-525400f775ce'}
```
5、代码基本如上
四、测试
我这里只对广州及深圳的数据分析岗位进行爬虫，如下：
![](https://img-blog.csdn.net/20170204013547122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来在项目目录下，便可以看到两个城市的职位信息：
![](https://img-blog.csdn.net/20170204013705875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
关于拉勾网的职位信息抓取到这里就完成了，具体代码可以在我的github上找到：[https://github.com/lpty/crawl_tutorial](https://github.com/lpty/crawl_tutorial)
注意：本项目代码仅作学术交流使用





