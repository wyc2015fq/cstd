# 爬虫中复杂点的ajax请求该如何破？ - zhusongziye的博客 - CSDN博客





2018年08月10日 22:28:05[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1563








本次需要用到的工具是charles工具进行抓包。这个工具大家自行百度下载，不会用的话也可以百度下，本篇文章就不对这个工具多说了。用这个工具是它有强大的搜索功能，可以一键搜索出我想要的网络请求。



这次网站是这个http://drugs.dxy.cn/



![](https://img-blog.csdn.net/20180810222113401?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



需求是获取所有药品的详细信息，第一眼看上去是觉得不难的，点了进去你才知道是有坑的。





![](https://img-blog.csdn.net/20180810222139494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



比如上面这个，需要获取详细信息就需要模拟点击那个三角形按钮，就需要通过发送ajax请求来进行获取了。



![](https://img-blog.csdn.net/20180810222204217?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



而且在你点开的时候还需要进行登陆才可以获取全部信息，所以这就多了一步，我们需要先模拟登陆成功后才可以进行aax请求来获取信息，要不然没用。



1.模拟登陆



登陆是在这个网址上进行登陆https://auth.dxy.cn/accounts/login?service=http%3A%2F%2Fdrugs.dxy.cn%2Flogin.do%3Fdone%3Dhttp%253A%252F%252Fdrugs.dxy.cn%252Fdrug%252F54565%252Fdetail.htm





![](https://img-blog.csdn.net/20180810222227739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



可以看到需要验证码，不过问题不大，可以解决。这个时候我们需要打开开发者工具，按下f12即可，然后点击持续日志，进行登陆，可以看到下面的图

![](https://img-blog.csdn.net/20180810222243277?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





拉下到fromdata可以看到下面数据

![](https://img-blog.csdn.net/20180810222312769?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



经过多次测试，username和password是登陆账号和密码，validatecode是验证码，nlt是一个加密参数，由js加载而来的，其他的都是不变的。由于nlt参数是由js加载的，这就需要用到charles工具了。

![](https://img-blog.csdn.net/20180810222340138?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



登陆成功之后很容易就可以在charles找到这个请求，让我们先看看nlt参数是怎样来的。



我们可以先复制这个nlt参数，然后在charles工具内按下ctrl+f就会出现这个页面



![](https://img-blog.csdn.net/20180810222356380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



把那两个勾选上，然后把nlt参数内容填上，点击find就会看到下面的内容，下面就是产生nlt参数的地方，点击进去，可以看到下面内容。





![](https://img-blog.csdn.net/20180810222409987?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



nlt参数是在html里面就提供的了，这就不需要去解析js了，这就相对容易点，再看看请求网址



![](https://img-blog.csdn.net/20180810222428978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



请求网址可以看到是和登陆网址一样的，说明nlt参数直接提供的了，我们只需要使用正则来将nlt参数提取出来即可，现在看看验证码是在哪个请求产生的就可以了。

![](https://img-blog.csdn.net/20180810222450832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180810222504963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



看到上面这些就知道验证码怎样来了，直接使用get请求对那个网址即可。

分析完了，接下来就是代码部分。



2.使用python来模拟登陆





```
def __get_nlt(self):
       """给ult参数"""
       url = "https://auth.dxy.cn/accounts/login?service=http%3A%2F%2Fdrugs.dxy.cn%2Flogin.do%3Fdone%3Dhttp%253A%252F%252Fdrugs.dxy.cn%252Fdrug%252F89790%252Fdetail.htm&qr=false&method=1"
       response = self.session.post(url)
       nlt = re.findall('nlt" value="([^"]+?)"', response.text)  # 匹配nlt参数
       return nlt

   def __login(self):
       """登陆网站"""
       # 登陆需要提交的表单
       print('进行登陆')
       data = {'username':  13060618237,  # 账号
               'password':  'qwe12345678',  # 密码
               'loginType': '1',
               'validateCode':  self.__get_chapter(),  # 验证码
               'keepOnlineType': '2',
               'trys':  '0',
               'nlt':  self.__get_nlt(),  # 获取nlt参数
               '_eventId':  'submit'}
       # 登陆网址
       url = 'https://auth.dxy.cn/accounts/login?service=http%3A%2F%2Fdrugs.dxy.cn%2Flogin.do%3Fdone%3Dhttp%253A%252F%252Fdrugs.dxy.cn%252Fdrug%252F89790%252Fdetail.htm&qr=false&method=1'
       response = self.session.post(url, headers=self.headers, data=data)  # 请求登陆
       if 'dxy_zbadwxq6' in response.text:  # 此处填写你的用户名字，用于验证是否登陆成功
           print('登陆成功')
       else:
           print('登陆失败，正在尝试重新登陆')
           self.__login()

   def __get_chapter(self):
       """获取验证码"""
       try:
           url = 'https://auth.dxy.cn/jcaptcha'
           response = self.session.get(url, headers=self.headers)
           # 保存验证码
           with open('code.jpg', 'wb')as f:
               f.write(response.content)
           im = Image.open('code.jpg')
           im.show()
           valide_code = input('输入验证码')
           return valide_code
       except Exception as e:
           """验证码失败，再次获取"""
           print(e)
           self.__get_chapter()
```



这是部分重要代码，里面都有注释，就不多说了，进入重要部分吧。



3.分析ajax请求



登陆成功之后。随便点进一个页面都可以然后点击三角形按钮都可以看到详细内容



![](https://img-blog.csdn.net/20180810222528408?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



我们继续使用charles工具来进行抓包，先对charles刚才抓的 包进行清理，然后点击页面三角形按钮来进学校获取信息

![](https://img-blog.csdn.net/20180810222549162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



可以看到上面的这个请求 ，数据都是Unicode编码的，我们需要转，其实可以直接复制然后到命令行窗口进行打印即可，就可以看到这个就是我们想要的详细数据

![](https://img-blog.csdn.net/2018081022260967?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



接着看下请求方式和其他需要的东西

![](https://img-blog.csdn.net/20180810222621858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180810222634137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



可以看到，是个post请求，成功状态码是200，有很多参数，经过多次测试后，发现下面箭头所指的五个参数都是会变化的，第一个为药品id，第二个可以通过抓包发现(和上面获取nlt参数都是一样的方法)是经过js来加载的，**注意，要想在charles里面加载js要先把浏览器的缓存先清除掉才行，要不然不会加载出来，抓包是抓不到的哈。**第三个变的也是药品的id，第四个就是通过药品页面加载出来的，而最后的batchId的起始id是2，之后每获取一个详细的内容该参数就加1。

![](https://img-blog.csdn.net/20180810222648303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180810222701178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



好了，需要的内容都分析出来了，最后就是剩下实现部分了。



4.用代码来进行ajax请求



这个是获取药品的页面内容的



```
def __get_content(self, item, href):
       """获取需要提取的信息"""
       param0 = int(re.findall('\d+', item)[0])
       href_id = re.findall('\d+', href)[0]
       html = self.__get_html(item)  # 获取药品的html信息
       name = re.findall('fl commend">(.*?)<', html, re.S)[0].replace('\n', '').replace('\t', '')
       batchId = 2  # 初始化id，提交表单需要的，起始数字是2
       id = "20A548B2C7B5F05093DFD2C71F112EEE"  # scriptSessionId中的加密需要用到的数据
       scriptSessionId = id + str(int(random() * 1000))  # 获取详情页需要的表单数据
       soup = BeautifulSoup(html, 'lxml')  # 使用bs4解析
       content_dd = soup.find_all('div', id='container')[0].find_all('dl')[0].find_all('dd', style=False)  # 获取整个页面的所有数据
       content_dt = soup.find_all('div', id='container')[0].find_all('dl')[0].find_all('dt')  # 获取数据的类型，就是比如适应症
       keys = re.findall('<span.*?>(.*?):<', str(content_dt), re.S)  # 清洗数据
       values = []  # 存储所有清洗后的数据
       for i in content_dd:  # 获取所有的数据，并进行清洗
           if '...   ' in i.get_text():  # 这个证明数据不完整，需要进行点击
               param = re.search('id="([0-9]+?)_([\d]+?)"', str(i))
               # 获取相关的表单数据
               param1 = param.group(2)
               # 获取详情内容
               data_content = detail = self.__get_detail(scriptSessionId, param0, param1, batchId)
               if 'img' in detail:  # 判断是否有图片链接
                   data_content = ''
                   for x in re.split('<img.*?/>', detail, re.S):
                       data_content += x
                   # 找图片链接
                   src = re.findall("<img src='([^']+?)'", detail, re.S)
                   for s in src:
                       data_content += s+'  '
               data_content = self.dr.sub('', data_content).replace('\n', ' ').replace('\t', ' ')
               values.append(data_content)
               batchId += 1
           else:
               if '商品名称' in i.get_text():
                   con = str(i)
               else:
                   con = self.dr.sub('', i.get_text().strip().replace('\n', ' ').replace('\t', ' '))
               values.append(con)
```



这个是获取页面中需要进行ajax的请求的方法



```
def __get_detail(self, scriptSessionId, param0, param1, batch_id):
       """获取那些需要点击才能看到的所有数据，就是模拟点击"""
       data = {'callCount': 1,
               'page': '/drug/%s/detail.htm' % param0,  # 这个参数是药品id
               'httpSessionId': '',
               'scriptSessionId': scriptSessionId,  # 获取对应的参数
               'c0-scriptName': 'DrugUtils',
               'c0-methodName': 'showDetail',
               'c0-id': '0',
               'c0-param0': 'number:%s' % param0,  # 这个参数是药品id
               'c0-param1': 'number:%s' % param1,  # 这个参数是需要获取想想页面的id
               'batchId': batch_id
               }
       # 请求头
       headers = {
                   'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36',
                   'content-type': 'text/plain'}
       # 发送请求
       r = self.session.post('http://drugs.dxy.cn/dwr/call/plaincall/DrugUtils.showDetail.dwr', headers=headers, data=data)
       # 把返回的html编码并找出相对应的数据
       detail = re.findall('"(.*?)"', r.text.encode('utf-8').decode("unicode-escape"), re.S)[0].strip()
       return detail
```



好了，就是这么多了！上面重要的不是代码，而是思想，只要你的思路跟上了，别的什么ajax请求都是这样子的，所以爬虫没什么难的，分析这些ajax请求主要还是怕遇到加密参数，需要解析那些混淆js，这些自然而然就是爬虫的主要部分了，做爬虫的主要还是想办法避开这些东西。



