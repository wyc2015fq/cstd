# [python爬虫] 招聘信息定时系统 (二).调用pyinstaller包生成exe文件 - 杨秀璋的专栏 - CSDN博客





2017年04月24日 00:07:01[Eastmount](https://me.csdn.net/Eastmount)阅读数：4303
所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)








        前面一篇文章主要讲述，如何通过Python爬取招聘信息，且爬取的日期为前一天的，同时将爬取的内容保存到数据库中；这篇文章主要讲述如何将python文件压缩成exe可执行文件，供后面的操作。
        这系列文章主要是最近研究了数据库的定时计划备份，联系爬虫简单做的一个实验，但方法可以，尤其在Windows xp年代。整个系列主要分为五部分，共五篇文章：
1.Python爬取招聘信息，并且存储到MySQL数据库中；
        2.调用pyinstaller包将py文件打包成exe可执行文件；
        3.设置Windows系统的计划，制作定时任务，每天早上定时执行exe爬虫；
        4.结合PHP（因毕业设计指导学生的是PHP系统）简单实现前端招聘信息界面；
        5.补充知识：Python调用短信猫发送招聘短信到客户手机。
文章比较基础，希望对您有所帮助，如果文章中存在错误或不足之处。




## 一. 安装pyinstaller

        Py文件通常需要运行在Python编程坏境中，而可执行exe文件更方便，尤其是我们第三篇文章制作定时任务 时，执行exe文件更加方便。
        而Python将py文件打包成exe可执行文件的方法通常包括两个：pyInstaller和py2exe。这篇文章主要是讲述如何安装pyinstaller包及其打包操作。
        你可以再Github中下载进行安装，参考下面"哈士奇说喵"的文章。
        下载地址：[https://github.com/pyinstaller/pyinstaller/](https://github.com/pyinstaller/pyinstaller/)

        这篇文章主要介绍pip进行安装的方法，步骤如下：
        1.打开cmd，然后去到Python的Scripts文件夹下，如下图所示。
        输入安装命令：**pip install pyinstaller**



![](https://img-blog.csdn.net/20170421134601122)


        2.安装过程如下图所示，调用pip指令安装会进行自动下载及匹配。


![](https://img-blog.csdn.net/20170421134722873)




        3.安装成功后，会在Python/Scripts文件夹下显示该包。



![](https://img-blog.csdn.net/20170421134946920)





参考文章：[关于python打包成exe的一点经验之谈 - ChenJian](http://www.cnblogs.com/chjbbs/archive/2014/01/25/3533187.html)
[将自己的python程序打包成exe - 哈士奇说喵](http://blog.csdn.net/mrlevo520/article/details/51840217)
[[Python] 项目打包：5步将py文件打包exe文件 - ccccshq](http://www.360doc.com/content/13/1216/11/11029609_337549928.shtml)





## 二. 简单实现exe打包 

         安装成功pyinstaller扩展包后，需要对文件进行打包操作 。
         假设存在一个test01.py文件，功能是打开百度网页，代码如下：


```python
from selenium import webdriver  
from selenium.webdriver.common.keys import Keys  
import time  
    
driver = webdriver.Firefox()  
driver.get("http://www.baidu.com/")
```
        我们将该test01.py文件放到Python安装路径的Scripts下，我的路径如下：
        C:\Software\Program Software\Python\Scripts
        注意Scripts放置Python的各种第三方扩展包，这里的pyinstaller.exe在此处。


![](https://img-blog.csdn.net/20170421135440906)

        现在需要打包exe程序，步骤：
        1.在cmd环境进入该Scripts文件夹路径下；
        2.输入指令打包：**pyinstaller - F test01.py**
        之后你的命令窗（黑框）会运行该代码，并进行打包，运行过程如下图所示。


![](https://img-blog.csdn.net/20170421140026362)



![](https://img-blog.csdn.net/20170421140049065)

        打包好会在Scripts路径下生成两个文件夹：**dist和build**，如下图所示。


![](https://img-blog.csdn.net/20170421140155297)




        然后，可执行exe文件在dist文件夹下，如下图所示。



![](https://img-blog.csdn.net/20170421140322159)

        双击该test01.exe，会自动弹出Firefox浏览器，然后访问百度。


![](https://img-blog.csdn.net/20170421140630772)




        更多参数如下图所示：



![](https://img-blog.csdn.net/20170421140729258)






## 三. 定时爬虫打包exe文件

        下面用同样的方法打包前一篇文章写的定时爬虫代码。步骤如下：
        1.将test_zl.py （智联）文件放置Scripts文件夹，与pyinstaller.exe一起。



![](https://img-blog.csdn.net/20170423235622611)

        2.打开cmd，去到Scripts文件夹下。
        执行命令： pyinstaller -F test_zl.py


![](https://img-blog.csdn.net/20170423235855849)

![](https://img-blog.csdn.net/20170423235930209)


        3.生成文件在当前目录下，包括build和dist文件夹。


![](https://img-blog.csdn.net/20170424000204760)

![](https://img-blog.csdn.net/20170424000228526)

![](https://img-blog.csdn.net/20170424000247182)




        4.执行test_zl.exe文件，运行如下图所示。



![](https://img-blog.csdn.net/20170424000325449)

        5.同时数据库如下，可以看到4月23日的招聘信息被爬取成功。


![](https://img-blog.csdn.net/20170424000416054)

![](https://img-blog.csdn.net/20170424000442445)






        后面还将继续探寻、继续写文，写完这种单击版的定时发送功能，后面研究Python服务器的相关功能。
风雪交加雨婆娑，
        琴瑟和鸣泪斑驳。
        披星戴月辗转梦，
        娜璋白首爱连绵。
最后希望文章对你有所帮助，如果文章中存在错误或不足之处，还请海涵~
      （By:Eastmount 2017-04-24 凌晨12点  [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）





