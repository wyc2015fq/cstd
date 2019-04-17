# zhwiki：中文维基百科数据获取与处理，抽取文本，opencc繁简转化，词向量训练 - ChihkAnchor的博客 - CSDN博客





2019年03月27日 08:49:11[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：28标签：[wiki																[语料处理](https://so.csdn.net/so/search/s.do?q=语料处理&t=blog)](https://so.csdn.net/so/search/s.do?q=wiki&t=blog)
个人分类：[问答系统](https://blog.csdn.net/weixin_40871455/article/category/8355978)








## 数据获取

中文wiki语料下载地址：[https://dumps.wikimedia.org/zhwiki/](https://dumps.wikimedia.org/zhwiki/)，可以选择最近的，也可以选择日期

![](https://img-blog.csdnimg.cn/20190326155004976.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)
- [zhwiki-20190320-pages-articles-multistream.xml.bz2](https://dumps.wikimedia.org/zhwiki/20190320/zhwiki-20190320-pages-articles-multistream.xml.bz2) 1.7 GB 是主文件；
- [zhwiki-20190320-pages-articles-multistream-index.txt.bz2](https://dumps.wikimedia.org/zhwiki/20190320/zhwiki-20190320-pages-articles-multistream-index.txt.bz2) 25.3 MB 是每个词条的编号信息

因为某些原因可能有时下载很慢，这里提供一个[网盘资源](https://pan.baidu.com/s/1mExG-phI4tJOKiuA9O-Zyw)， 提取码: s9wm 

## 抽取文本

**下载完安装包之后，不需要解压，****使用 gensim 抽取正文文本，如果没有gensim，pip install gensim 即可**

**    # 命令行下运行如下命令，wikipro.py代码如下：    # python wikipro.py zhwiki-20190320-pages-articles-multistream.xml.bz2 wiki.zh.txt**

**    # wikipro.py代码如下：**

```python
import logging
import sys
from gensim.corpora import WikiCorpus
logging.basicConfig(format='%(asctime)s: %(levelname)s: %(message)s', level=logging.INFO)
'''
    extract data from wiki dumps(*articles.xml.bz2) by gensim.
    @2019-3-26
'''
def help():
    print("Usage: python wikipro.py zhwiki-20190320-pages-articles-multistream.xml.bz2 wiki.zh.txt")

if __name__ == '__main__':
    if len(sys.argv) < 3:
        help()
        sys.exit(1)
    logging.info("running %s" % ' '.join(sys.argv))
    inp, outp = sys.argv[1:3]
    i = 0

    output = open(outp, 'w',encoding='utf8')
    wiki = WikiCorpus(inp, lemmatize=False, dictionary={})
    for text in wiki.get_texts():
        output.write(" ".join(text) + "\n")
        i = i + 1
        if (i % 10000 == 0):
            logging.info("Save "+str(i) + " articles")
    output.close()
    logging.info("Finished saved "+str(i) + "articles")

    # 命令行下运行
    # python wikipro.py zhwiki-20190320-pages-articles-multistream.xml.bz2 wiki.zh.txt
```

![](https://img-blog.csdnimg.cn/20190326155838608.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

```bash
finished iterating over Wikipedia corpus of 335210 documents with 76590871 positions (total 3266328 articles, 90672594 positions before pruning articles shorter than 50 words)
2019-03-26 15:59:09,924: INFO: Finished saved 335210articles
```

## 从xml转成TXT之后又很多繁体字，我们需要进行繁简转化 

![](https://img-blog.csdnimg.cn/20190326171428753.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

使用opencc进行繁简转化，[https://bintray.com/package/files/byvoid/opencc/OpenCC](https://bintray.com/package/files/byvoid/opencc/OpenCC)，根据版本不同选择不同的文件，我这里选择win64，下载然后解压，

![](https://img-blog.csdnimg.cn/20190326171601929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

cmd打开命令行，进入到这个文件夹

![](https://img-blog.csdnimg.cn/20190326172107339.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/201903261718518.png)

```bash
opencc -i G:\raw_data\wiki.zh.txt -o G:\raw_data\wiki.zh.simple.txt -c t2s.json
```

转为简体之后示例： 

![](https://img-blog.csdnimg.cn/20190326172322880.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## 使用jieba进行分词：

```python
# 文件名 jieba_cut.py
import jieba
import jieba.analyse
import jieba.posseg as pseg
import codecs,sys
def cut_words(sentence):
    #print sentence
    return " ".join(jieba.cut(sentence)).encode('utf-8')
f=codecs.open('wiki.zh.simple.txt','r',encoding="utf8")
target = codecs.open("wiki.zh.simple.seg.txt", 'w',encoding="utf8")
print ('open files')
line_num=1
line = f.readline()
while line:
	if(line_num % 1000 == 0):
		print('---- processing', line_num, 'article----------------')
	line_seg = " ".join(jieba.cut(line))
	target.writelines(line_seg)
	line_num = line_num + 1
	line = f.readline()
f.close()
target.close()
exit()
```

命令行下运行：python  jieba_cut.py

![](https://img-blog.csdnimg.cn/20190326174458555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## 训练词向量

```python
# 文件名 train_word2vec_model.py
import logging
import os
import sys
import multiprocessing

from gensim.models import Word2Vec
from gensim.models.word2vec import LineSentence

if __name__ == '__main__':
    program = os.path.basename(sys.argv[0])
    logger = logging.getLogger(program)

    logging.basicConfig(format='%(asctime)s: %(levelname)s: %(message)s')
    logging.root.setLevel(level=logging.INFO)
    logger.info("running %s" % ' '.join(sys.argv))

    # check and process input arguments
    if len(sys.argv) < 4:
        print(globals()['__doc__'] % locals())
        sys.exit(1)
    inp, outp1, outp2 = sys.argv[1:4]

    model = Word2Vec(LineSentence(inp), size=400, window=5, min_count=5,
                     workers=multiprocessing.cpu_count(),iter=100)

    # trim unneeded model memory = use(much) less RAM
    # model.init_sims(replace=True)
    model.save(outp1)
    model.wv.save_word2vec_format(outp2, binary=False)
```

命令行执行如下代码 

```bash
python train_word2vec_model.py  wiki.zh.simple.seg.txt  wiki.zh.text.model   wiki.zh.text.vector
```

![](https://img-blog.csdnimg.cn/20190327084536852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190327084358413.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190327084647451.png)

测试：

```python
from gensim.models import Word2Vec
 
zh_wiki_word2vec_model = Word2Vec.load('wiki.zh.text.model')
 
testwords = ['孩子', '数学', '学术', '白痴', '篮球']
for i in range(5):
    res = zh_wiki_word2vec_model.most_similar(testwords[i])
    print(testwords[i])
    print(res)
```

![](https://img-blog.csdnimg.cn/20190327090500781.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)




