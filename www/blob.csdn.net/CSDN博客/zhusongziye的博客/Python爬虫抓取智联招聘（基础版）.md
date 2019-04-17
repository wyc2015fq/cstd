# Python爬虫抓取智联招聘（基础版） - zhusongziye的博客 - CSDN博客





2018年04月24日 21:15:49[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：104










> 
对于每个上班族来说，总要经历几次换工作，如何在网上挑到心仪的工作？如何提前为心仪工作的面试做准备？今天我们来抓取智联招聘的招聘信息，助你换工作成功！


运行平台： Windows  
Python版本： Python3.6  
IDE： Sublime Text   
其他工具：  Chrome浏览器

# 1、网页分析

## 1.1 分析请求地址

以北京海淀区的python工程师为例进行网页分析。打开智联招聘首页，选择北京地区，在搜索框输入"python工程师"，点击"搜工作"：

![](https://img-blog.csdn.net/20180424211018856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




接下来跳转到搜索结果页面，按"F12"打开开发者工具，然后在"热门地区"栏选择"海淀"，我们看一下地址栏：






![](https://img-blog.csdn.net/20180424211028228?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




由地址栏后半部分`searchresult.ashx?jl=北京&kw=python工程师&sm=0&isfilter=1&p=1&re=2005`可以看出，我们要自己构造地址了。接下来要对开发者工具进行分析，按照如图所示步骤找到我们需要的数据：Request Headers和Query String Parameters ：







![](https://img-blog.csdn.net/2018042421104350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




构造请求地址：



```
paras = {
   'jl': '北京',                # 搜索城市
   'kw': 'python工程师',        # 搜索关键词 
   'isadv': 0,                    # 是否打开更详细搜索选项
   'isfilter': 1,                # 是否对结果过滤
   'p': 1,                        # 页数
   're': 2005                    # region的缩写，地区，2005代表海淀
}

url = 'https://sou.zhaopin.com/jobs/searchresult.ashx?' + urlencode(paras)
```




请求头：





```
headers = {
   'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36',
   'Host': 'sou.zhaopin.com',
   'Referer': 'https://www.zhaopin.com/',
   'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
   'Accept-Encoding': 'gzip, deflate, br',
   'Accept-Language': 'zh-CN,zh;q=0.9'
}
```




## 1.2 分析有用数据

接下来我们要分析有用数据，从搜索结果中我们需要的数据有：职位名称、公司名称、公司详情页地址、职位月薪：




![](https://img-blog.csdn.net/20180424211126809?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




通过网页元素定位找到这几项在HTML文件中的位置，如下图所示：






![](https://img-blog.csdn.net/2018042421113856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




用正则表达式对这四项内容进行提取：

```
# 正则表达式进行解析
pattern = re.compile('<a style=.*? target="_blank">(.*?)</a>.*?'        # 匹配职位信息
   '<td class="gsmc"><a href="(.*?)" target="_blank">(.*?)</a>.*?'     # 匹配公司网址和公司名称
   '<td class="zwyx">(.*?)</td>', re.S)                                # 匹配月薪      

# 匹配所有符合条件的内容
items = re.findall(pattern, html)
```




注意：解析出来的部分职位名称带有标签，如下图所示：






![](https://img-blog.csdn.net/20180424211155190?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




那么在解析之后要对该数据进行处理剔除标签，用如下代码实现：



```
for item in items:
   job_name = item[0]
   job_name = job_name.replace('<b>', '')
   job_name = job_name.replace('</b>', '')
   yield {
       'job': job_name,
       'website': item[1],
       'company': item[2],
       'salary': item[3]
   }
```




# 2、写入文件

我们获取到的数据每个职位的信息项都相同，可以写到数据库中，但是本文选择了csv文件，以下为百度百科解释：

> 
逗号分隔值（Comma-Separated Values，CSV，有时也称为字符分隔值，因为分隔字符也可以不是逗号），其文件以纯文本形式存储表格数据（数字和文本）。纯文本意味着该文件是一个字符序列，不含必须像二进制数字那样被解读的数据。


由于python内置了csv文件操作的库函数，所以很方便：

```
import csv
def write_csv_headers(path, headers):
   '''
   写入表头
   '''
   with open(path, 'a', encoding='gb18030', newline='') as f:
       f_csv = csv.DictWriter(f, headers)
       f_csv.writeheader()

def write_csv_rows(path, headers, rows):
   '''
   写入行
   '''
   with open(path, 'a', encoding='gb18030', newline='') as f:
       f_csv = csv.DictWriter(f, headers)
       f_csv.writerows(rows)
```




# 3、进度显示

要想找到理想工作，一定要对更多的职位进行筛选，那么我们抓取的数据量一定很大，几十页、几百页甚至几千页，那么我们要掌握抓取进度心里才能更加踏实啊，所以要加入进度条显示功能。

本文选择tqdm 进行进度显示，来看一下酷炫结果（图片来源网络）：

![](https://img-blog.csdn.net/20180424211451704?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





执行以下命令进行安装：`pip install tqdm`。

简单示例：

```
from tqdm import tqdm
from time import sleep

for i in tqdm(range(1000)):
   sleep(0.01)
```




# 4、完整代码

以上是所有功能的分析，如下为完整代码：

```
#-*- coding: utf-8 -*-
import re
import csv
import requests
from tqdm import tqdm
from urllib.parse import urlencode
from requests.exceptions import RequestException

def get_one_page(city, keyword, region, page):
   '''
   获取网页html内容并返回
   '''
   paras = {
       'jl': city,         # 搜索城市
       'kw': keyword,      # 搜索关键词 
       'isadv': 0,         # 是否打开更详细搜索选项
       'isfilter': 1,      # 是否对结果过滤
       'p': page,          # 页数
       're': region        # region的缩写，地区，2005代表海淀
   }

   headers = {
       'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36',
       'Host': 'sou.zhaopin.com',
       'Referer': 'https://www.zhaopin.com/',
       'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
       'Accept-Encoding': 'gzip, deflate, br',
       'Accept-Language': 'zh-CN,zh;q=0.9'
   }

   url = 'https://sou.zhaopin.com/jobs/searchresult.ashx?' + urlencode(paras)
   try:
       # 获取网页内容，返回html数据
       response = requests.get(url, headers=headers)
       # 通过状态码判断是否获取成功
       if response.status_code == 200:
           return response.text
       return None
   except RequestException as e:
       return None

def parse_one_page(html):
   '''
   解析HTML代码，提取有用信息并返回
   '''
   # 正则表达式进行解析
   pattern = re.compile('<a style=.*? target="_blank">(.*?)</a>.*?'        # 匹配职位信息
       '<td class="gsmc"><a href="(.*?)" target="_blank">(.*?)</a>.*?'     # 匹配公司网址和公司名称
       '<td class="zwyx">(.*?)</td>', re.S)                                # 匹配月薪      

   # 匹配所有符合条件的内容
   items = re.findall(pattern, html)   

   for item in items:
       job_name = item[0]
       job_name = job_name.replace('<b>', '')
       job_name = job_name.replace('</b>', '')
       yield {
           'job': job_name,
           'website': item[1],
           'company': item[2],
           'salary': item[3]
       }

def write_csv_file(path, headers, rows):
   '''
   将表头和行写入csv文件
   '''
   # 加入encoding防止中文写入报错
   # newline参数防止每写入一行都多一个空行
   with open(path, 'a', encoding='gb18030', newline='') as f:
       f_csv = csv.DictWriter(f, headers)
       f_csv.writeheader()
       f_csv.writerows(rows)

def write_csv_headers(path, headers):
   '''
   写入表头
   '''
   with open(path, 'a', encoding='gb18030', newline='') as f:
       f_csv = csv.DictWriter(f, headers)
       f_csv.writeheader()

def write_csv_rows(path, headers, rows):
   '''
   写入行
   '''
   with open(path, 'a', encoding='gb18030', newline='') as f:
       f_csv = csv.DictWriter(f, headers)
       f_csv.writerows(rows)

def main(city, keyword, region, pages):
   '''
   主函数
   '''
   filename = 'zl_' + city + '_' + keyword + '.csv'
   headers = ['job', 'website', 'company', 'salary']
   write_csv_headers(filename, headers)
   for i in tqdm(range(pages)):
       '''
       获取该页中所有职位信息，写入csv文件
       '''
       jobs = []
       html = get_one_page(city, keyword, region, i)
       items = parse_one_page(html)
       for item in items:
           jobs.append(item)
       write_csv_rows(filename, headers, jobs)

if __name__ == '__main__':
   main('北京', 'python工程师', 2005, 10)
```




上面代码执行效果如图所示：



![](https://img-blog.csdn.net/20180424211206574?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





执行完成后会在py同级文件夹下会生成名为：`zl_北京_python工程师.csv`的文件，打开之后效果如下：




![](https://img-blog.csdn.net/20180424211229970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







