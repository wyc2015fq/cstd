
# 利用sas软件将txt文档转为excel文档 - lpty的博客 - CSDN博客

2017年02月04日 21:52:16[lpty](https://me.csdn.net/sinat_33741547)阅读数：1372


一、前言
在[python爬虫抓取拉勾网职业信息](http://blog.csdn.net/sinat_33741547/article/details/54847950)这篇博客中，已经抓取了拉勾网数据分析职位的信息并储存在本地，下面介绍一下如果将txt文档转化为csv文档。
二、实战
这里要注意的是，为了在sas中数据步编写方便，爬虫储存在本地的数据格式有所改动，后边会详细说明。
1、数据读取

```python
libname lagou 'F:\lagou';
filename intxt 'F:\lagou\深圳.txt';
filename outcsv 'F:\lagou\深圳.csv';
data lagou.sz;
	infile intxt firstobs=2;
	length companyName $60. companyType $20. companyStage $20. companyLabel $60. companySize $10. companyDistrict $10.
	       positionType $15. positionEducation $10. positionAdvantage $60. positionSalary $10. positionWorkYear $10.;
	input companyName companyType companyStage companyLabel companySize companyDistrict  
              positionType positionEducation positionAdvantage positionSalary positionWorkYear;
run;
```
在上面可以看到，读取文件的时候，分隔符使用的是默认的空格，这与之前爬虫抓取时存储的数据格式有所不同，需要自行修改。因为数据第一行是标签名，所以从第二个观测开始读取，length语句保证数据长度足够，结果如下：
![](https://img-blog.csdn.net/20170204220455997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、转化为csv文档

```python
option nocenter;
ods listing close;
ods results off;
ods csvall file=outcsv;
	proc print data=lagou.sz;
		title '2017年拉勾网深圳数据分析职位信息';
	run;
ods csvall close;
ods results on;
ods listing;
```
option nocenter将将输出的文件内容局左，ods listing close关闭到output窗口的默认输出，ods result off关闭默认到result的输出，结果如下：![](https://img-blog.csdn.net/20170204221221814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、到这里将txt文档转为csv文档就结束了

