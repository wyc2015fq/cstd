# [python爬虫] 招聘信息定时系统 (一).BeautifulSoup爬取信息并存储MySQL - 杨秀璋的专栏 - CSDN博客





2017年04月22日 16:03:54[Eastmount](https://me.csdn.net/Eastmount)阅读数：5672
所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)








        这系列文章主要讲述，如何通过Python爬取招聘信息，且爬取的日期为当前天的，同时将爬取的内容保存到数据库中，然后制作定时系统每天执行爬取，最后是Python调用相关库发送短信到手机。
        最近研究了数据库的定时计划备份，联系爬虫简单做了这个实验，虽然方法是基于单机，比较落后，但可行，创新也比较好。整个系列主要分为五部分，共五篇文章：
1.Python爬取招聘信息，并且存储到MySQL数据库中；
        2.调用pyinstaller包将py文件打包成exe可执行文件；
        3.设置Windows系统的计划，制作定时任务，每天早上定时执行exe爬虫；
        4.结合PHP（因毕业设计指导学生的是PHP系统）简单实现前端招聘信息界面；
        5.补充知识：Python调用短信猫发送招聘短信到客户手机，研究ing。
文章比较基础好玩，希望对您有所帮助，如果文章中存在错误或不足之处。



## 一. 运行结果

        爬取地址为智联招聘网站：[http://sou.zhaopin.com/](http://sou.zhaopin.com/)



![](https://img-blog.csdn.net/20170422150627292)

        爬取结果存储至MySQL数据库如下图所示，注意只有4月22日的信息。


![](https://img-blog.csdn.net/20170422150729768)

        运行结果及保存TXT文件如下所示：


![](https://img-blog.csdn.net/20170422150819941)








## 二. BeautifulSoup爬虫详解

        完整代码如下所示：


```python
# -*- coding: utf-8 -*-
"""
Created on 2017-04-22 15:10

@author: Easstmount
"""

import urllib2 
import re 
from bs4 import BeautifulSoup
import codecs
import MySQLdb
import os


#存储数据库
#参数:职位名称 公司名称 职位月薪 工作地点 发布时间 职位链接
def DatabaseInfo(zwmc, gsmc, zwyx, gzdd, gxsj, zwlj):  
    try:  
        conn = MySQLdb.connect(host='localhost',user='root',
                             passwd='123456',port=3306, db='eastmount')  
        cur=conn.cursor() #数据库游标  
  
        #报错:UnicodeEncodeError: 'latin-1' codec can't encode character  
        conn.set_character_set('utf8')  
        cur.execute('SET NAMES utf8;')  
        cur.execute('SET CHARACTER SET utf8;')  
        cur.execute('SET character_set_connection=utf8;')

        #SQL语句 智联招聘(zlzp)
        sql = '''insert into eastmount_zlzp 
                    (zwmc,gsmc,zwyx,gzdd,gxsj,zwlj) 
                values(%s, %s, %s, %s, %s, %s)'''

        cur.execute(sql, (zwmc, gsmc, zwyx, gzdd, gxsj, zwlj))
        print '数据库插入成功'            
        
    #异常处理  
    except MySQLdb.Error,e:  
        print "Mysql Error %d: %s" % (e.args[0], e.args[1])  
    finally:  
        cur.close()    
        conn.commit()    
        conn.close()  
    

#爬虫函数
def crawl(url):
    page = urllib2.urlopen(url) 
    contents = page.read() 
    soup = BeautifulSoup(contents, "html.parser") 
    print u'贵阳JAVA招聘信息: 职位名称 \t 公司名称 \t 职位月薪 \t 工作地点 \t 发布日期 \n'
    infofile.write(u"贵阳JAVA招聘信息: 职位名称 \t 公司名称 \t 职位月薪 \t 工作地点 \t 发布日期  \r\n")
    print u'爬取信息如下:\n'

    i = 0
    for tag in soup.find_all(attrs={"class":"newlist"}):
        #print tag.get_text()
        i = i + 1
        
        #职位名称
        zwmc = tag.find(attrs={"class":"zwmc"}).get_text()
        zwmc = zwmc.replace('\n','')
        print zwmc
        #职位链接
        url_info = tag.find(attrs={"class":"zwmc"}).find_all("a")
        #print url_info
        #url_info.get(href) AttributeError: 'ResultSet' object has no attribute 'get' 
        for u in url_info:
            zwlj = u.get('href')
            print zwlj
        #公司名称
        gsmc = tag.find(attrs={"class":"gsmc"}).get_text()
        gsmc = gsmc.replace('\n','')
        print gsmc
        #find另一种定位方法 <td class="zwyx">8000-16000</td>
        zz = tag.find_all('td', {"class":"zwyx"})
        print zz
        #职位月薪
        zwyx = tag.find(attrs={"class":"zwyx"}).get_text()
        zwyx = zwyx.replace('\n','')
        print zwyx
        #工作地点
        gzdd = tag.find(attrs={"class":"gzdd"}).get_text()
        gzdd = gzdd.replace('\n','')
        print gzdd
        #发布时间
        gxsj = tag.find(attrs={"class":"gxsj"}).get_text()
        gxsj = gxsj.replace('\n','')
        print gxsj
        #获取当前日期并判断写入文件
        import datetime
        now_time = datetime.datetime.now().strftime('%m-%d') #%Y-%m-%d
        #print now_time
        if now_time==gxsj:
            print u'存入文件'
            infofile.write(u"[职位名称]" + zwmc + "\r\n")
            infofile.write(u"[公司名称]" + gsmc + "\r\n")
            infofile.write(u"[职位月薪]" + zwyx + "\r\n")
            infofile.write(u"[工作地点]" + gzdd + "\r\n")
            infofile.write(u"[发布时间]" + gxsj + "\r\n")
            infofile.write(u"[职位链接]" + zwlj + "\r\n\r\n")  
        else:
            print u'日期不一致，当前日期: ', now_time
        
        #####################################
        # 重点：写入MySQL数据库
        #####################################
        if now_time==gxsj:
            print u'存入数据库'
            DatabaseInfo(zwmc, gsmc, zwyx, gzdd, gxsj, zwlj)
        
        print '\n\n'

    else:
        print u'爬取职位总数', i
        
        
#主函数
if __name__ == '__main__':
    
    infofile = codecs.open("Result_ZP.txt", 'a', 'utf-8')     
    #翻页执行crawl(url)爬虫
    i = 1  
    while i<=2:  
        print u'页码', i
        url = 'http://sou.zhaopin.com/jobs/searchresult.ashx?in=160400&jl=%E8%B4%B5%E9%98%B3&kw=java&p=' + str(i) + '&isadv=0'  
        crawl(url)  
        infofile.write("###########################\r\n\r\n\r\n")  
        i = i + 1
        
    infofile.close()
```
        安装Beautifulsoup如下图所示，使用pip install bs4即可。


![](https://img-blog.csdn.net/20170422151353187)

        重点是分析智联招聘的DOM树结构。
**1.分析URL**
        URL为：http://sou.zhaopin.com/jobs/searchresult.ashx?in=160400&jl=%E8%B4%B5%E9%98%B3&kw=java&p=2&isadv=0
        其中，"in=160400" 表示 "行业类别" 选择"计算机软件"（可以多选）；"jl=贵阳" 表示工作地点选择贵阳市；"kw=java" 表示职位选择Java相关专业；"p=2" 表示页码，main函数通过循环分析爬取。

![](https://img-blog.csdn.net/20170422150627292)




**2.分析DOM树节点**
        然后浏览器右键审查元素，可以看到每行职位信息都是在HTML中都是一个<table></table>，其中class为newlist。
        核心代码：**for tag in soup.find_all(attrs={"class":"newlist"}):**
        定位该节点后再分别爬取内容，并赋值给变量，存储到MySQL数据库中。


![](https://img-blog.csdn.net/20170422153027915)


**3.具体内容分析**
        获取职位名称代码如下：
*zwmc = tag.find(attrs={"class":"zwmc"}).get_text()        print zwmc*
        另一段代码，会输出节点信息，如：
*zz = tag.find_all('td', {"class":"zwyx"})        print zz        #<td class="zwyx">8000-16000</td>*
        对应的HTML DOM树分析如下图所示。


![](https://img-blog.csdn.net/20170422154203780)

        4.判断为当前日期则保存到TXT和MySQL中，这是为了后面方便，每天爬取最新的信息并周期执行，然后发送短信给手机。我也是佩服自己的大脑，哈哈~

        参考前文，并推荐官网。
[Python爬虫之Selenium+BeautifulSoup+Phantomjs专栏](http://blog.csdn.net/column/details/eastmount-spider.html)
[[python知识] 爬虫知识之BeautifulSoup库安装及简单介绍](http://blog.csdn.net/eastmount/article/details/44593165)
[[python爬虫] BeautifulSoup和Selenium对比爬取豆瓣Top250电影信息](http://blog.csdn.net/eastmount/article/details/53932775)







## 三. 数据库操作

SQL语句创建表代码如下：


```
CREATE TABLE `eastmount_zlzp` (  
  `ID` int(11) NOT NULL AUTO_INCREMENT,  
  `zwmc` varchar(100) COLLATE utf8_bin DEFAULT NULL COMMENT '职位名称',  
  `gsmc` varchar(50) COLLATE utf8_bin DEFAULT NULL COMMENT '公司名称',  
  `zwyx` varchar(50) COLLATE utf8_bin DEFAULT NULL COMMENT '职位月薪',  
  `gzdd` varchar(50) COLLATE utf8_bin DEFAULT NULL COMMENT '工作地点',  
  `gxsj` varchar(50) COLLATE utf8_bin DEFAULT NULL COMMENT '发布时间',
  `zwlj` varchar(50) COLLATE utf8_bin DEFAULT NULL COMMENT '职位链接',
	`info` varchar(200) COLLATE utf8_bin DEFAULT NULL COMMENT '详情', 
  PRIMARY KEY (`ID`)  
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
```
        显示如下图所示：


![](https://img-blog.csdn.net/20170422155057114)




其中，[Python](http://lib.csdn.net/base/python)调用MySQL推荐下面这篇文字。
[[python] 专题九.Mysql数据库编程基础知识](http://blog.csdn.net/eastmount/article/details/52156383)
        核心代码如下所示：

```python
# coding:utf-8 
import MySQLdb
 
try:
    conn=MySQLdb.connect(host='localhost',user='root',passwd='123456',port=3306, db='test01')
    cur=conn.cursor()
    
    #插入数据
    sql = '''insert into student values(%s, %s, %s)'''
    cur.execute(sql, ('yxz','111111', '10'))

    #查看数据
    print u'\n插入数据:'
    cur.execute('select * from student')
    for data in cur.fetchall():
        print '%s %s %s' % data
    cur.close()
    conn.commit()
    conn.close()
except MySQLdb.Error,e:
     print "Mysql Error %d: %s" % (e.args[0], e.args[1])
```

        后面还将继续探寻、继续写文，写完这种单击版的定时发送功能，后面研究Python服务器的相关功能。最后希望文章对你有所帮助，如果文章中存在错误或不足之处，还请海涵~
        太忙了，但是年轻人忙才好，多经历多磨砺多感悟；想想自己都是下班在学习，配女神的时候学习，真的有个好贤内助。胡子来省考，晚上陪他们吃个饭。感觉人生真的很奇妙，昨天加完班走了很远给女神一个91礼物和一个拼图，感觉挺开心的。生活、教学、编程、爱情，最后献上一首最近写的诗，每句都是近期一个故事。
风雪交加雨婆娑，
        琴瑟和鸣泪斑驳。
        披星戴月辗转梦，
        娜璋白首爱连绵。
同时准备写本python书给我的女神，一直没定下来，唯一要求就是她的署名及支持。
      （By:Eastmount 2017-04-22 下午4点  [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）








