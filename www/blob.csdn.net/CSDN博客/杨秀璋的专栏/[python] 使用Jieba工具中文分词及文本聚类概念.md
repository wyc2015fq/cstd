
# [python] 使用Jieba工具中文分词及文本聚类概念 - 杨秀璋的专栏 - CSDN博客

2015年12月11日 02:38:17[Eastmount](https://me.csdn.net/Eastmount)阅读数：44988所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)



前面讲述了很多关于Python爬取本体Ontology、消息盒InfoBox、虎扑图片等例子，同时讲述了VSM向量空间模型的应用。但是由于InfoBox没有前后文和语义概念，所以效果不是很好，这篇文章主要是爬取百度5A景区摘要信息，再利用Jieba分词工具进行中文分词，最后提出文本聚类算法的一些概念知识。
相关文章：
[[Python爬虫] Selenium获取百度百科旅游景点的InfoBox消息盒](http://blog.csdn.net/eastmount/article/details/48234733)
[[python爬虫] Selenium定向爬取海量精美图片及搜索引擎杂谈](http://blog.csdn.net/eastmount/article/details/48854535)
[Python简单实现基于VSM的余弦相似度计算](http://blog.csdn.net/eastmount/article/details/49898133)
[基于VSM的命名实体识别、歧义消解和指代消解](http://blog.csdn.net/eastmount/article/details/48566671)
[[python爬虫] Selenium定向爬取PubMed生物医学摘要信息](http://blog.csdn.net/eastmount/article/details/50191629)

## 一. Selenium爬取百度百科摘要
简单给出Selenium爬取百度百科5A级景区的代码：

```python
# coding=utf-8  
""" 
Created on 2015-12-10 @author: Eastmount  
"""  
  
import time          
import re          
import os  
import sys
import codecs
import shutil
from selenium import webdriver      
from selenium.webdriver.common.keys import Keys      
import selenium.webdriver.support.ui as ui      
from selenium.webdriver.common.action_chains import ActionChains  
  
#Open PhantomJS  
driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")  
#driver = webdriver.Firefox()  
wait = ui.WebDriverWait(driver,10)
#Get the Content of 5A tourist spots  
def getInfobox(entityName, fileName):  
    try:  
        #create paths and txt files
        print u'文件名称: ', fileName
        info = codecs.open(fileName, 'w', 'utf-8')  
        #locate input  notice: 1.visit url by unicode 2.write files
        #Error: Message: Element not found in the cache -
        #       Perhaps the page has changed since it was looked up
        #解决方法: 使用Selenium和Phantomjs
        print u'实体名称: ', entityName.rstrip('\n') 
        driver.get("http://baike.baidu.com/")  
        elem_inp = driver.find_element_by_xpath("//form[@id='searchForm']/input")  
        elem_inp.send_keys(entityName)  
        elem_inp.send_keys(Keys.RETURN)  
        info.write(entityName.rstrip('\n')+'\r\n')  #codecs不支持'\n'换行  
  
        #load content 摘要
        elem_value = driver.find_elements_by_xpath("//div[@class='lemma-summary']/div")
        for value in elem_value:
            print value.text
            info.writelines(value.text + '\r\n')
        #爬取文本信息
        #爬取所有段落<div class='para'>的内容 class='para-title'为标题 [省略]
        time.sleep(2)  
          
    except Exception,e:    #'utf8' codec can't decode byte  
        print "Error: ",e  
    finally:  
        print '\n'  
        info.close() 
  
#Main function  
def main():
    #By function get information
    path = "BaiduSpider\\"
    if os.path.isdir(path):
        shutil.rmtree(path, True)
    os.makedirs(path)
    source = open("Tourist_spots_5A_BD.txt", 'r')
    num = 1
    for entityName in source:  
        entityName = unicode(entityName, "utf-8")  
        if u'故宫' in entityName:   #else add a '?'  
            entityName = u'北京故宫'
        name = "%04d" % num
        fileName = path + str(name) + ".txt"
        getInfobox(entityName, fileName)
        num = num + 1
    print 'End Read Files!'  
    source.close()  
    driver.close()
    
if __name__ == '__main__':
    main()
```
内容如下图所示，共204个国家5A级景点的摘要信息。这里就不再叙述：
![](https://img-blog.csdn.net/20151210232925012)


## 二. Jieba中文分词
Python中分分词工具很多，包括盘古分词、Yaha分词、Jieba分词等。
中文分词库：[http://www.oschina.net/project/tag/264/segment](http://www.oschina.net/project/tag/264/segment)
其中它们的基本用法都相差不大，但是Yaha分词不能处理如“黄琉璃瓦顶”或“圜丘坛”等词，所以使用了结巴分词。
**1.安装及入门介绍**
参考地址：[http://www.oschina.net/p/jieba](http://www.oschina.net/p/jieba)
下载地址：[https://pypi.python.org/pypi/jieba/](https://pypi.python.org/pypi/jieba/)
Python 2.0我推荐使用"pip install jieba"或"easy_install jieba"全自动安装，再通过import jieba来引用（第一次import时需要构建Trie树，需要等待几秒时间）。
安装时如果出现错误"unknown encoding: cp65001"，输入"chcp 936"将编码方式由utf-8变为简体中文gbk。
![](https://img-blog.csdn.net/20151211001842210)
结巴中文分词涉及到的算法包括：
(1) 基于Trie树结构实现高效的词图扫描，生成句子中汉字所有可能成词情况所构成的有向无环图（DAG)；
(2) 采用了动态规划查找最大概率路径, 找出基于词频的最大切分组合；
(3) 对于未登录词，采用了基于汉字成词能力的HMM模型，使用了Viterbi算法。
结巴中文分词支持的三种分词模式包括：
(1) 精确模式：试图将句子最精确地切开，适合文本分析；
(2) 全模式：把句子中所有的可以成词的词语都扫描出来, 速度非常快，但是不能解决歧义问题；
(3) 搜索引擎模式：在精确模式的基础上，对长词再次切分，提高召回率，适合用于搜索引擎分词。
同时结巴分词支持繁体分词和自定义字典方法。
```python
#encoding=utf-8
import jieba
#全模式
text = "我来到北京清华大学"
seg_list = jieba.cut(text, cut_all=True)
print u"[全模式]: ", "/ ".join(seg_list) 
#精确模式
seg_list = jieba.cut(text, cut_all=False)
print u"[精确模式]: ", "/ ".join(seg_list)
#默认是精确模式
seg_list = jieba.cut(text)
print u"[默认模式]: ", "/ ".join(seg_list) 
#新词识别 “杭研”并没有在词典中,但是也被Viterbi算法识别出来了
seg_list = jieba.cut("他来到了网易杭研大厦") 
print u"[新词识别]: ", "/ ".join(seg_list)
#搜索引擎模式
seg_list = jieba.cut_for_search(text) 
print u"[搜索引擎模式]: ", "/ ".join(seg_list)
```
输出如下图所示：
![](https://img-blog.csdn.net/20151211004304803)
代码中函数简单介绍如下：
jieba.cut()：第一个参数为需要分词的字符串，第二个cut_all控制是否为全模式。
jieba.cut_for_search()：仅一个参数，为分词的字符串，该方法适合用于搜索引擎构造倒排索引的分词，粒度比较细。
其中待分词的字符串支持gbk\utf-8\unicode格式。返回的结果是一个可迭代的generator，可使用for循环来获取分词后的每个词语，更推荐使用转换为list列表。
**2.添加自定义词典**
由于"国家5A级景区"存在很多旅游相关的专有名词，举个例子：
[输入文本] 故宫的著名景点包括乾清宫、太和殿和黄琉璃瓦等
[精确模式] 故宫/的/著名景点/包括/乾/清宫/、/太和殿/和/黄/琉璃瓦/等
[全 模 式] 故宫/的/著名/著名景点/景点/包括/乾/清宫/太和/太和殿/和/黄/琉璃/琉璃瓦/等
显然，专有名词"乾清宫"、"太和殿"、"黄琉璃瓦"(假设为一个文物)可能因分词而分开，这也是很多分词工具的又一个缺陷。但是Jieba分词支持开发者使用自定定义的词典，以便包含jieba词库里没有的词语。虽然结巴有新词识别能力，但自行添加新词可以保证更高的正确率，尤其是专有名词。
基本用法：jieba.load_userdict(file_name) \#file_name为自定义词典的路径
词典格式和dict.txt一样，一个词占一行；每一行分三部分，一部分为词语，另一部分为词频，最后为词性（可省略，ns为地点名词），用空格隔开。
强烈推荐一篇词性标注文章，链接如下：
[http://www.hankcs.com/nlp/part-of-speech-tagging.html](http://www.hankcs.com/nlp/part-of-speech-tagging.html)
```python
#encoding=utf-8
import jieba
#导入自定义词典
jieba.load_userdict("dict.txt")
#全模式
text = "故宫的著名景点包括乾清宫、太和殿和黄琉璃瓦等"
seg_list = jieba.cut(text, cut_all=True)
print u"[全模式]: ", "/ ".join(seg_list) 
#精确模式
seg_list = jieba.cut(text, cut_all=False)
print u"[精确模式]: ", "/ ".join(seg_list)
#搜索引擎模式
seg_list = jieba.cut_for_search(text) 
print u"[搜索引擎模式]: ", "/ ".join(seg_list)
```
输出结果如下所示，其中专有名词连在一起，即"乾清宫"和"黄琉璃瓦"。
![](https://img-blog.csdn.net/20151211013017417)
**3.关键词提取**
在构建VSM向量空间模型过程或者把文本转换成数学形式计算中，你需要运用到关键词提取的技术，这里就再补充该内容，而其他的如词性标注、并行分词、获取词位置和搜索引擎就不再叙述了。
基本方法：jieba.analyse.extract_tags(sentence, topK)
需要先import jieba.analyse，其中sentence为待提取的文本，topK为返回几个TF/IDF权重最大的关键词，默认值为20。
```python
#encoding=utf-8
import jieba
import jieba.analyse
#导入自定义词典
jieba.load_userdict("dict.txt")
#精确模式
text = "故宫的著名景点包括乾清宫、太和殿和午门等。其中乾清宫非常精美，午门是紫禁城的正门，午门居中向阳。"
seg_list = jieba.cut(text, cut_all=False)
print u"分词结果:"
print "/".join(seg_list)
#获取关键词
tags = jieba.analyse.extract_tags(text, topK=3)
print u"关键词:"
print " ".join(tags)
```
输出结果如下，其中"午门"出现3次、"乾清宫"出现2次、"著名景点"出现1次，按照顺序输出提取的关键词。如果topK=5，则输出："午门 乾清宫 著名景点 太和殿 向阳"。
```python
>>> 
分词结果:
故宫/的/著名景点/包括/乾清宫/、/太和殿/和/午门/等/。/其中/乾清宫/非常/精美/，/午门/是/紫禁城/的/正门/，/午门/居中/向阳/。
关键词:
午门 乾清宫 著名景点
>>>
```
**4.对百度百科获取摘要分词**
从BaiduSpider文件中读取0001.txt~0204.txt文件，分别进行分词处理再保存。
```python
#encoding=utf-8
import sys
import re
import codecs
import os
import shutil
import jieba
import jieba.analyse
#导入自定义词典
jieba.load_userdict("dict_baidu.txt")
#Read file and cut
def read_file_cut():
    #create path
    path = "BaiduSpider\\"
    respath = "BaiduSpider_Result\\"
    if os.path.isdir(respath):
        shutil.rmtree(respath, True)
    os.makedirs(respath)
    num = 1
    while num<=204:
        name = "%04d" % num 
        fileName = path + str(name) + ".txt"
        resName = respath + str(name) + ".txt"
        source = open(fileName, 'r')
        if os.path.exists(resName):
            os.remove(resName)
        result = codecs.open(resName, 'w', 'utf-8')
        line = source.readline()
        line = line.rstrip('\n')
        
        while line!="":
            line = unicode(line, "utf-8")
            seglist = jieba.cut(line,cut_all=False)  #精确模式
            output = ' '.join(list(seglist))         #空格拼接
            print output
            result.write(output + '\r\n')
            line = source.readline()
        else:
            print 'End file: ' + str(num)
            source.close()
            result.close()
        num = num + 1
    else:
        print 'End All'
#Run function
if __name__ == '__main__':
    read_file_cut()
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20151211020623945)
**5.去除停用词**
在信息检索中，为节省存储空间和提高搜索效率，在处理自然语言数据（或文本）之前或之后会自动过滤掉某些字或词，这些字或词即被称为Stop Words（停用词）。这些停用词都是人工输入、非自动化生成的，生成后的停用词会形成一个停用词表。但是，并没有一个明确的停用词表能够适用于所有的工具。甚至有一些工具是明确地避免使用停用词来支持短语搜索的。[参考百度百科]
```python
#encoding=utf-8
import jieba
#去除停用词
stopwords = {}.fromkeys(['的', '包括', '等', '是'])
text = "故宫的著名景点包括乾清宫、太和殿和午门等。其中乾清宫非常精美，午门是紫禁城的正门。"
segs = jieba.cut(text, cut_all=False)
final = ''
for seg in segs:
    seg = seg.encode('utf-8')
    if seg not in stopwords:
            final += seg
print final
#输出:故宫著名景点乾清宫、太和殿和午门。其中乾清宫非常精美，午门紫禁城正门。
seg_list = jieba.cut(final, cut_all=False)
print "/ ".join(seg_list)
#输出:故宫/ 著名景点/ 乾清宫/ 、/ 太和殿/ 和/ 午门/ 。/ 其中/ 乾清宫/ 非常/ 精美/ ，/ 午门/ 紫禁城/ 正门/ 。
```

## 三. 基于VSM的文本聚类算法
这部分主要参考2008年上海交通大学姚清坛等《基于向量空间模型的文本聚类算法》的论文，因为我的实体对齐使用InfoBox存在很多问题，发现对齐中会用到文本内容及聚类算法，所以简单讲述下文章一些知识。
![](https://img-blog.csdn.net/20151211021909383)

文本聚类的主要依据聚类假设是：同类的文档相似度较大，而非同类文档相似度较小。同时使用无监督学习方法，聚类不需要训练过程以及不需要预先对文档手工标注类别，因此具有较高的灵活性和自动化处理能力。主要分为以下部分：
(1) 预处理常用方法
文本信息预处理（词性标注、语义标注），构建统计词典，对文本进行词条切分，完成文本信息的分词过程。
(2) 文本信息的特征表示
采用方法包括布尔逻辑型、概率型、混合型和向量空间模型。其中向量空间模型VSM（Vector Space Model）是将文档映射成向量的形式，(T1, T2, ..., Tn)表示文档词条，(W1, W2, ..., Wn)文档词条对应权重。建立文本特征主要用特征项或词条来表示目标文本信息，构造评价函数来表示词条权重，尽最大限度区别不同的文档。
(3) 文本信息特征缩减
VSM文档特征向量维数众多。因此，在文本进行聚类之前，应用文本信息特征集进行缩减，针对每个特征词的权重排序，选取最佳特征，包括TF-IDF。推荐向量稀疏表示方法，提升聚类的效果，其中(D1, D2, ..., Dn)表示权重不为0的特征词条。
(4) 文本聚类
文本内容表示成数学课分析形势后，接下来就是在此数学基础上进行文本聚类。包括基于概率方法和基于距离方法。其中基于概率是利用贝叶斯概率理论，概率分布方式；基于聚类是特征向量表示文档（文档看成一个点），通过计算点之间的距离，包括层次聚类法和平面划分法。
后面我可能也会写具体的Python聚类算法，VSM计算相似度我前面已经讲过。同时，他的实验数据是搜狐中心的10个大类，包括汽车、财经、IT、体育等，而我的数据都是旅游，如何进一步聚类划分，如山川、河流、博物馆等等，这是另一个难点。
最后还是那句话：不论如何，希望文章对你有所帮助，如果文章中有错误或不足之处，还请海涵~写文不易，且看且分析。加油！！！
（By:Eastmount 2015-12-11 深夜3点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


