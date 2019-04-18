# python—爬虫练习题（scrapy） - wsp_1138886114的博客 - CSDN博客





2018年06月24日 18:21:03[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：470
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









### 一、爬取 51job

> 
##### 1.1 新建scrapy项目:

```
在D:\learning_code_scrapy （自定义）文件夹目录中执行cmd打开终端
终端执行：scrapy startproject Test_Monday_job51
使用pycharm打开文件：Test_Monday_job51（使用新窗口打开）
```

##### 1.2 编写scrapy项目:

```python
◆ 编辑 items.py 文件
import scrapy
from scrapy import Item,Field

class TestMondayItem(scrapy.Item):
    jobName = Field()                  # 工作名称
    ComName = Field()                  # 公司名称
    adress  = Field()                  # 工作地点
    money =   Field()                  # 薪资
    releaseTime = Field()              # 发布时间
    pass


◆ 在spiders 文件夹下 创建 Get_Data_job51.py 文件并编辑
from scrapy.selector import Selector
from scrapy.spiders import CrawlSpider
from Test_Monday_job51.items import TestMondayItem

class Get_Data_job51(CrawlSpider):
    pageNum = 1                      #初始化页面
    name = "Get_Data_job51"          #与文件名同名
    start_urls = ['https://search.51job.com/list/020000,000000,0000,00,9,99,%25E4%25\
    BA%25BA%25E5%25B7%25A5%25E6%2599%25BA%25E8%2583%25BD,2,1.html?lang=c&stype=\
    1&postchannel=0000&workyear=99&cotype=99°reefrom=99&jobterm=99&\companysize=\
    99&lonlat=0%2C0&radius=-1&ord_field=0&confirmdate=9&fromType=4&dibiaoid\
    =0&address=&line=&specialarea=00&from=&welfare=']

    def parse(self, response):
        Get_Data_job51.pageNum += 1   #获取下一页
        selector = Selector(response)
        item = TestMondayItem()
        Infos = selector.xpath('//div[@id="resultList"][1]//div[@class="el"]')
        print(len(Infos))

        for each in Infos:
            jobName = each.xpath('p/span/a/@title').extract()
            ComName = each.xpath('span[@class="t2"]/a/@title').extract()
            adress = each.xpath('span[2]/text()').extract()
            money = each.xpath('span[3]/text()').extract()
            releaseTime = each.xpath('span[4]/text()').extract()
            print(jobName,"\n",ComName,"\n",adress,"\n",money,"\n",releaseTime)

            item['jobName'] = jobName
            item['ComName'] = ComName
            item['adress'] = adress
            item['money'] = money
            item['releaseTime'] = releaseTime

            yield item                      #提交 item

        nextlink = selector.xpath('//div[@id="resultList"][1]//li[@class="bk"][2]/a/@href').extract()[0]
        if Get_Data_job51.pageNum<5 and nextlink:
            yield Request(nextlink,callback=self.parse)


◆ 在Test_Monday_job51文件夹下 创建 main.py 文件（与items文件同级）并编辑
from scrapy import cmdline
cmdline.execute("scrapy crawl Get_Data_job51".split())


◆ 编辑 pipline.py 文件(先将默认类注释)
class json_TestMondayPipeline(object):      #保存为json文件格式

    def __init__(self):

        #打开或新建文件
        self.file = open('json_51job.json','w',encoding='utf-8')
    def process_item(self,item,spider):     #写入item 数据
        line = json.dumps(dict(item),ensure_ascii=False)+"\n"

        #处理行数据
        self.file.write(line)
        return item
    def close_spider(self,spider):
        self.file.close()

----------------------------------------------------------------------------------------------- 
class Excel_TestMondayPipeline(object):            #保存为 Excel 文件格式
    index = 0
    def __init__(self):

        self.wk = xlwt.Workbook(encoding='utf-8')   #打开或新建文件
        self.sheet = self.wk.add_sheet('51job')
        field = ['职位名','公司名','工作地址','薪资','发布时间',]
        for i ,v in enumerate(field):
            self.sheet.write(0,i,v)

    def process_item(self,item,spider):            #写入item 数据
        Excel_TestMondayPipeline.index += 1
        for j, v in enumerate(item.keys()):
            self.sheet.write(Excel_TestMondayPipeline.index, j, item[v])
        return item

    def close_spider(self,spider):
        self.wk.save('51job.xls')                 # 保存文件 


◆ 设置 settings.py ( 找到 ITEM_PIPELINES 并编辑)
ITEM_PIPELINES = {
   # 'Test_Monday_job51.pipelines.json_TestMondayPipeline': 300,     #保存文 json 文件
   'Test_Monday_job51.pipelines.Excel_TestMondayPipeline': 300,    #保存为 excel 文件
}

#释放 DOWNLOAD_DELAY

DOWNLOAD_DELAY = 3            #延时 3 秒
```


### 二、[爬取梦幻西游门派音乐](http://xyq.163.com/download/down_music.html)

> 
###### Scrapy结合 CSS+xpath

![这里写图片描述](https://img-blog.csdn.net/20180610151941386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  win+r 打开cmd 执行：`scrapy startproject Music`
```python
◆ 在 spiders 目录下创建Get_Music.py文件
from scrapy.spiders import CrawlSpider
from scrapy.selector import Selector
from Music.items import MusicItem
import re

class Music_menghua(CrawlSpider):
    name = "Get_Music"
    start_urls = ["http://xyq.163.com/download/down_music.html"]
    def parse(self, response):
        item = MusicItem()
        selector = Selector(response)
        Music_List = selector.css('#dLeft .g-clr section')[0].css('tbody tr')
        for tr in Music_List:
            music_name = tr.xpath('td[1]/text()')[0].extract()
            time_range = tr.xpath('td[2]/text()')[0].extract()
            music_link = tr.xpath('td[3]/a/@href').extract()
            print('3333333333333333333333333333', time_range)
            # 给item赋值
            item['music_name'] = music_name     # 歌名
            item['time_range'] = time_range     # 歌曲大小
            item['music_link'] = music_link     # 链接
            yield item

◆ 编辑 items.py文件
import scrapy
from scrapy import Field,Item

class MusicItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    music_name = Field()
    time_range = Field()
    music_link = Field()
    pass

◆ 编辑 settings.py 文件
BOT_NAME = 'Music'

SPIDER_MODULES = ['Music.spiders']
NEWSPIDER_MODULE = 'Music.spiders'

USER_AGENT  =['Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_3) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.54 Safari/536.5']
FEED_URI = u'file:///D:/learning_code_scrapy/Music.csv'      #文件保存路径
FEED_FORMAT = 'CSV'

◆ 在spiders新建main.py(主函数)，并编辑。--与items.py为同级文件
from scrapy import cmdline
cmdline.execute("scrapy crawl Get_Music".split())
```





