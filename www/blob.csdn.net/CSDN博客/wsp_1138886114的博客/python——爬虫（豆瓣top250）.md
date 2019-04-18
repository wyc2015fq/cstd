# python——爬虫（豆瓣top250） - wsp_1138886114的博客 - CSDN博客





2018年06月28日 21:42:39[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：396
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









### 一、抓取豆瓣top250网页数据

```
import urllib.request as urlrequest
from bs4 import BeautifulSoup

top250_url = "https://movie.douban.com/top250?start={}&filter="

with open('./top250_f1.csv','w',encoding='utf8') as outputfile:
    outputfile.write("num#title#director#role#init_year#area\
                     #genre#rating_num#comment_num#comment#url\n")
    for i in range(10):
        start = i*25
        url_visit = top250_url.format(start)
        crawl_content = urlrequest.urlopen(url_visit).read()
        http_content = crawl_content.decode('utf8')
        soup = BeautifulSoup(http_content,'html.parser')
        all_item_divs = soup.find_all(class_='item')

        for each_item_div in all_item_divs:
            pic_div = each_item_div.find(class_='pic')
            num = pic_div.find('em').get_text()   #排名
            href = pic_div.find('a')['href']      #电影链接
            title = pic_div.find('img')['alt']    #电影名称
            bd_div = each_item_div.find(class_='bd')
            infos = bd_div.find('p').get_text().strip().split('\n')
            infos_1 = infos[0].split('\xa0\xa0\xa0')
            director = infos_1[0][4:].rstrip('...').rstrip('/').split('/')[0]   #导演
            role = str(infos_1[1:])[6:].split('/')[0]                           #主演
            infos_2 = infos[1].lstrip().split('\xa0/\xa0')
            year = infos_2[0]    #上映时间
            area = infos_2[1]    #国家/地区
            genre = infos_2[2:]  #电影类型
            star_div = each_item_div.find(class_='star')
            rating_num = star_div.find(class_='rating_num').get_text()      #评分
            comment_num = star_div.find_all('span')[3].get_text()[:-3]      #评价数量
            quote = each_item_div.find(class_='quote')
            inq =''
            try:
                inq = quote.find(class_='inq').get_text()  #一句话评价
            except:
                inq = ''
            outputfile.write('{}#{}#{}#{}#{}#{}#{}#{}#{}#{}#{}\n'.\
                             format(num,title,director,role,year,area,\
                                    genre,rating_num,comment_num,inq,href))

print('ok!!')
```

### 二、 抓取上述代码中豆瓣top250缺失的详细数据

```
import urllib
import urllib.request as urlrequest
import json
import time
import random
import pandas as pd

df = pd.read_csv("./top250_f1.csv",sep = "#", encoding = 'utf8',
                 names=['num','title','director','role','init_year','area',\
                        'genre','rating_num','comment_num','comment','url'])             
urlsplit = df.url.str.split('/').apply(pd.Series)
num=0
id_list = list(urlsplit[4])[1:]

#这里写几个可用的IP地址和端口号，只抓250个页面，有两三个IP就够了
#(建议使用此代理：http://www.xicidaili.com/或http://www.data5u.com/)

IP_list = ['166.111.80.162:3128','39.134.153.26:8080' ]
IP=random.choice(IP_list)
with open('top250_f5.csv', 'w',encoding='utf8') as outputfile:
    outputfile.write("num#rank#alt_title#title#pubdate#language\#writer#\
                      director#cast#movie_duration#year#movie_type#tags#image\n")     
    proxy = urlrequest.ProxyHandler({'https': '' + IP}) 
    opener = urlrequest.build_opener(proxy)
    opener.addheaders = [('User-Agent',
    'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_4)AppleWebKit/603.1.30 \
    (KHTML, like Gecko) Version/10.1 Safari/603.1.30')]           
    urlrequest.install_opener(opener) 
    for id in id_list:
        url_visit = 'https://api.douban.com/v2/movie/{}'.format(id)
        crawl_content = urlrequest.urlopen(url_visit).read() 
        json_content = json.loads(crawl_content.decode('utf-8'))
        rank = json_content['rating']['average']
        alt_title = json_content['alt_title']
        image = json_content['image']
        title = json_content['title']
        pubdate = json_content['attrs']['pubdate']
        language = json_content['attrs']['language']
        try:
            writer = json_content['attrs']['writer']
        except:
            writer = 'None'
        director = json_content['attrs']['director']
        try:
            cast = json_content['attrs']['cast']
        except:
            cast='None'
        movie_duration = json_content['attrs']['movie_duration']
        year = json_content['attrs']['year']
        movie_type = json_content['attrs']['movie_type']
        tags = json_content['tags']
        num = num +1
        data ="{}#{}#{}#{}#{}#{}#{}#{}#{}#{}#{}#{}#{}#{}\n".\
                format(num,rank,alt_title,title,pubdate,language,writer,\
                       director,cast,movie_duration,year,movie_type,tags,image)
        print(data)
        outputfile.write(data)
        time.sleep(10)

print("ok!!!")
```



