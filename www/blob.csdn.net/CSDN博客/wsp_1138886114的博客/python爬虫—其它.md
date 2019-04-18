# python爬虫—其它 - wsp_1138886114的博客 - CSDN博客





2018年10月20日 15:25:26[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：143
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)












### 文章目录
- [爬取西刺IP代理网站](#IP_1)
- [豆瓣美女图片](#_72)
- [豆瓣电影导演评分等](#_119)
- [化学品供应信息](#_199)




##### 爬取西刺IP代理网站

```python
import re
from bs4 import BeautifulSoup
import chardet
import requests
import xlwt
import random
import re

def Get_Html(Page_num):
    USER_AGENTS = [
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1",
        "Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.36 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_0) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3"]  # 浏览器
    proxies = [{"HTTP": "117.63.78.64:6666"},
               {"HTTPS": "114.225.169.215:53128"},
               {"HTTPS": "222.185.22.108:6666"}]  # 代理IP
    url = "http://www.xicidaili.com/wn/"+str(Page_num)
    r = requests.get(url,
                     headers={"User-Agent": random.choice(USER_AGENTS)},
                     proxies=random.choice(proxies))
    code = chardet.detect(r.content)['encoding']
    return r.content.decode(code)

data = []
def Get_data(Page_num):
    for index in range(Page_num):
        html = Get_Html(index+1)
        print('正在获取第{}页数据'.format(index+1))
        soup = BeautifulSoup(html,"html.parser")
        parents = soup.find("table",attrs={'id':"ip_list"})
        TRS = parents.find_all("tr")
        TRS.pop(0)
        reg = re.compile('(\d+)天$')
        for td in TRS:
            tds = td.find_all('td')
            IP_adress = tds[1].string
            duankou_IP =tds[2].get_text()
            town_adress = tds[4].get_text()
            leixing_IP = tds[5].get_text()
            alive_time = re.findall(reg,tds[8].get_text())
            fabu_time = tds[9].get_text()
            data.append([IP_adress+":"+duankou_IP,town_adress,leixing_IP,alive_time,fabu_time])
    print(data)
    return data

def Save_Excel():
    wbk = xlwt.Workbook(encoding="utf-8")
    sheet1 = wbk.add_sheet("西刺网站IP地址")
    fields = ["IP地址：端口号","地区","HTTPs/HTTP","存活时间(天)","发布时间"]
    for i in range(len(fields)):
        sheet1.write(0,i,fields[i])
    for j in range(len(data)):
        for k in range(len(fields)):
            sheet1.write(j+1,k,data[j][k])
    wbk.save("西刺网站IP地址.xls")

def main(Page_num):
    Get_data(Page_num)
    Save_Excel()
main(5)
```

##### 豆瓣美女图片

```python
#encoding:utf-8
print('------------豆瓣美女--------------')
# 方法，使用urllib.urlretrieve() 方法直接将远程数据下载到本地
import requests
from bs4 import BeautifulSoup
import urllib
import os

address = str(1049732)

# 设置获取网页内容的函数
def getHtml(index,number):
    url = "https://movie.douban.com/celebrity/"+number+"/photos/?type=C&start=" + str(index)
    r = requests.get(url,{"User-Agent":"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11"})
    soup = BeautifulSoup(r.text,"html.parser")
    return soup

images = []
imgLen = len(getHtml(0,address).find('ul', attrs={'class': "poster-col3 clearfix"}).find_all('img'))
def getImages(pageNum,name,number):
    if os.path.exists(name):
        os.rmdir("photos")
    else:
        os.mkdir(name)
    os.chdir(name)
    global address,images,imgLen
    for k in range(pageNum):
        eachsoup = getHtml(k*imgLen,number)
        imageList = eachsoup.find('ul', attrs={'class': "poster-col3 clearfix"})
        # 3、获取所有image
        #     通过extend方法，还是一个list，如果用append会是多个list，下面的循环的就要额外处理了
        images.extend(imageList.find_all('img'))

    #3、用循环处理所有li内的具体内容
    for i in range(len(images)):
        try:
            suffix = images[i]['src'][-3:]
            image_name = str(i+1)+'.'+suffix
            urllib.urlretrieve(images[i]['src'],image_name)
        except Exception:
            print('存储有异常')
    return

getImages(1,'liyifei1',address)
```

##### 豆瓣电影导演评分等

```python
#encoding:utf-8
import requests
from bs4 import BeautifulSoup
import chardet
import re
import xlwt
import random
from urllib import request

#获取某页的内容
def getHtml(page_num):
    USER_AGENTS = ["Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1",
        "Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.36 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_0) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3"]  # 浏览器
    proxies = ["110.72.36.241:8123","121.31.100.224:8123",
               "220.171.89.66:808","61.135.217.7:80","121.31.192.227:8123"]      # 代理IP
    url = "https://movie.douban.com/top250?start=" + str(page_num*25) + "&filter="
    r = requests.get(url,
                     headers={"User-Agent":random.choice(USER_AGENTS)},
                     proxies={"http": random.choice(proxies)})
    code = chardet.detect(r.content)["encoding"]
    return r.content.decode(code)

dataList = []
def get_Data(num):
    for i in range(num):
        html = getHtml(i)
        soup = BeautifulSoup(html,"html.parser")
        print(soup)
        DIVparent = soup.find("div",attrs={"id":"content"}) #寻找父类
        lis = DIVparent.find_all("li")
        reg = re.compile('.*(\d{4}).*')
        for i in lis:
            data = []
            titles = i.find("span",attrs={"class":"title"}).string
            data.append(titles)             #电影名
            years_str = i.find('div',attrs={'class':'bd'}).find('p').get_text()
            years = re.findall(reg,years_str)[0]
            data.append(years)              #发行时间
            actor = i.find("p").get_text().strip().split('\t')
            data.append(actor[0])           # 导演
            count_num = i.find("span",attrs={"class":"rating_num",'property':"v:average"}).string
            data.append(count_num)          #电影评分
            person_num = i.find('div', attrs={'class': 'star'}).find_all('span')[3].get_text()
            number = re.findall(re.compile('\d*'), person_num)[0]
            data.append(number)             #评价人数
            if i.find('div', attrs={'class': 'bd'}).find('p', attrs={'class': 'quote'}):
                pinglun = i.find('div', attrs={'class': 'bd'}).find('p', attrs={'class': 'quote'}).find(
                    'span').get_text()
            else:
                pinglun = ''            #简评
            data.append(pinglun)
        dataList.append(data)
    return dataList

def save_Excel():
    book = xlwt.Workbook(encoding="utf-8")  # 创建工作簿
    sheet = book.add_sheet("daouban_top250")
    col = ('电影名', '发行时间','导演与主演', '电影评分', '评价人数','简评')
    for i in range(len(col)):
        sheet.write(0, i, col[i])
    for i in range(len(dataList)):  # 控制行
        for j in range(len(dataList[i])):  # 控制列
            sheet.write(i + 1, j, dataList[i][j])
    book.save('daouban_top250.xls')

def main(num):
    get_Data(num)
    save_Excel()
main(2)
```

##### 化学品供应信息

```python
import chardet
import random
import requests
from bs4 import BeautifulSoup
import time
import xlwt

def Get_Html(Page_num):
    url = "https://www.chemicalbook.com/SellingLeadList.aspx?start="+str(Page_num*100)
    USER_AGENTS = [
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1",
        "Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.36 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_0) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3"]  # 浏览器
    proxies = [{"HTTP": "117.63.78.64:6666"},
               {"HTTPS": "114.225.169.215:53128"},
               {"HTTPS": "222.185.22.108:6666"}]  # 代理IP
    r = requests.get(url,
                     headers={'User-Agent':random.choice(USER_AGENTS)},
                     proxies=random.choice(proxies))
    code = chardet.detect(r.content)['encoding']
    return r.content.decode(code)

data = []
def Get_Data(Page_num):
    for i in range(Page_num):
        html = Get_Html(i)
        print('正在获取第{}页...'.format(i+1))
        time.sleep(3)
        print('正在休息...')
        soup = BeautifulSoup(html,"html.parser")
        parents = soup.find('div',attrs={'class':'main_960px'})
        TRs = parents.find_all('tr',attrs={'style':'background-color:#EEEEEE;'})
        for td in TRs:
            tds = td.find_all('td')
            chemical_information = []
            for j in tds:
                if j.string != None:
                    chemical_info = j.get_text().strip()
                elif j.find('a') !=None:
                    chemical_info = j.find('a').get_text().strip()
                else:
                    chemical_info = ''
                chemical_information.append(chemical_info)
            data.append(chemical_information)
    print('==========================', data)
    return data
def Save_Excel():
    wbk = xlwt.Workbook(encoding="utf-8")
    sheet1 = wbk.add_sheet("化学品供应信息")
    fields = ["发布日期","产品名称","产品英文名称","CAS号","包装与价格信息",'公司名称']
    for i in range(len(fields)):
        sheet1.write(0,i,fields[i])
    for j in range(len(data)):
        for k in range(len(fields)):
            sheet1.write(j+1,k,data[j][k])
    wbk.save("化学品供应信息.xls")

def main(Page_num):
    Get_Data(Page_num)
    Save_Excel()
main(10)
```

获取的数据请查看:[https://download.csdn.net/download/wsp_1138886114/10733881](https://download.csdn.net/download/wsp_1138886114/10733881)



