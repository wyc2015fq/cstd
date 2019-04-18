# windows下基于selenium保存网页为图片 - 专注于数据挖掘算法研究和应用 - CSDN博客





2019年04月11日 18:16:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：12








环境：windows+python3+chorme.exe

1.安装：pip3 install selenium;

2.安装：下载chormedriver，解压到指定目录

3.代码：

```python
# -*- coding: utf-8 -*-
'''
Created on 2019年4月11日

@author: cvter
'''
from selenium import webdriver

if __name__ == "__main__":
    driver = webdriver.Chrome(r"D:\\Program Files\\chromedriver.exe")
    driver.get('http://www.baidu.com')
    driver.save_screenshot('baidu.png')
    driver.quit()
```

![](https://img-blog.csdnimg.cn/20190411181626179.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)



