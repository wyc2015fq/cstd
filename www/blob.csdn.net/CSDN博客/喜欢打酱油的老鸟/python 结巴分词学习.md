
# python 结巴分词学习 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月17日 08:24:35[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：64


[https://www.toutiao.com/a6643201326710784520/](https://www.toutiao.com/a6643201326710784520/)
2019-01-06 10:14:00
# 结巴分词（自然语言处理之中文分词器）
jieba分词算法使用了基于前缀词典实现高效的词图扫描，生成句子中汉字所有可能生成词情况所构成的有向无环图(DAG), 再采用了动态规划查找最大概率路径，找出基于词频的最大切分组合，对于未登录词，采用了基于汉字成词能力的HMM模型，使用了Viterbi算法。
![python 结巴分词学习](http://p3.pstatp.com/large/pgc-image/6828e1a1701341f09b69079bc886d8ec)
jieba分词支持三种分词模式：
1. 精确模式, 试图将句子最精确地切开，适合文本分析：
2. 全模式，把句子中所有的可以成词的词语都扫描出来，速度非常快，但是不能解决歧义；
3. 搜索引擎模式，在精确模式的基础上，对长词再词切分，提高召回率，适合用于搜索引擎分词。
**一 结巴分词的安装**
```python
pip3 install jieba
```
**二 结巴分词的主要功能**
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
1. jieba.cut：该方法接受三个输入参数：
　　参数1：需要分词的字符串; 
　　参数2：cut_all参数用来控制是否采用全模式，默认为精确模式；
 cut_all=True 全模式
 cut_all=false 精确（默认）模式
　　参数3：HMM参数用来控制是否适用HMM模型
```
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
2. jieba.cut_for_search：该方法接受两个参数：
　　参数1：需要分词的字符串；
　　参数2：是否使用HMM模型，
该方法适用于搜索引擎构建倒排索引的分词，粒度比较细。
```
```python
3. jieba.cut 以及jieba.cut_for_search
返回的结构都是可以得到的generator(生成器)
```
```python
4. jieb.lcut 以及 jieba.lcut_for_search 
直接返回list
```
```python
5.jieba.Tokenizer(dictionary=DEFUALT_DICT)
新建自定义分词器，
可用于同时使用不同字典，
jieba.dt为默认分词器，所有全局分词相关函数都是该分词器的映射。
```
**三 结巴分词的三种模式**
```python
import jieba
text='赵丽颖主演的正午阳光剧,知否知否应是绿肥红瘦'
```
**1 全模式 cut_all=True**
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
seq_list=jieba.cut(text,cut_all=True)
print(seq_list) #<generator object Tokenizer.cut at 0x0000026EB6F0CD58>
print(list(seq_list))
'''
['赵', '丽', '颖', '主演', '的', '正午', '阳光', '剧', '', '', '知', '否', '知', '否', '应', '是', '绿肥', '绿肥红瘦']
'''
```
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)
**2 精确模式 （默认模式） cut_all =False**
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
# 02精确模式
seq_list=jieba.cut(text,cut_all=False)
print(list(seq_list))
'''
['赵丽颖', '主演', '的', '正午', '阳光', '剧', ',', '知否', '知否', '应', '是', '绿肥红瘦']
'''
```
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)
**3 搜索引擎模式 cut_for_search**
```python
seq_list=jieba.cut_for_search(text,)
print(list(seq_list))
'''
['赵丽颖', '主演', '的', '正午', '阳光', '剧', ',', '知否', '知否', '应', '是', '绿肥', '绿肥红瘦']
'''
```
**四 自定义分词器(jieba.Tokenizer)**
**1　创建词典内容的格式**
```python
一个词语占一行(分三部分)
格式: 词语 词频 词性
如:张三 5
 李四 10 eng
```
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/0852a357873149dcbe8af48f032c3033)
**2 自定义词典的导入(load_userdict)**
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
text='赵丽颖主演的正午阳光剧,知否知否应是绿肥红瘦'
# 自定义词典
jieba.load_userdict('自定义词典.txt')
sep_list=jieba.lcut(text)
print('userdict>>>',sep_list)
```
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
userdict>>> ['赵丽颖', '主演', '的', '正午', '阳光剧', ',', '知否', '知否', '应是', '绿肥红瘦']
```
**五 利用jieba 进行关键词的抽取**
**1 基于TF－IDF算法的关键词抽取**
详解自然语言处理之TF-IDF模型和python实现
**2 python 实现关键提取**
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
jieba.analyse.extract_tags(text,topK=20,withWeight=False,allowPOS=())
'''
 text:为待提取的文本;
 topK:返回几个TF/IDF权重最大的关键字,默认值为20;
 withWeight:是否一并返回关键词权重值,默认False;
'''
jieba.analyse.TFIDF(idf_path=None) #新建tf-idf实例,idf_path为IDF实例
```
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)
**五 使用结巴的词云实例**
**1 数据准备**
文档:
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/cb8500a1ded84e969b9c9b1a11eeff04)
死了都要爱.txt
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/cb8500a1ded84e969b9c9b1a11eeff04)
dream ispossible.txt
图片:(红心.jpg)
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/be93fc1609504e838650a3efca2c78c6)
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
# 数据获取
with open('死了都要爱.txt','r',encoding='utf8')as f:
 text=f.read()
# with open('dream is possible.txt','r',encoding='utf8')as f:
# text=f.read()
#图片获取
mask=np.array(Image.open('红心.jpg'))
```
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)
**2 数据清洗**
屏蔽不需要的数据和分词
```python
# 数据清洗
# 屏蔽死了都要爱
STOPWORDS.add('死了都要爱')
sep_list=jieba.lcut(text,cut_all=False)
sep_list=" ".join(sep_list) #转为字符串
```
自定义画布
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)

```python
wc=WordCloud(
 font_path=font,#使用的字体库
 margin=2,
 mask=mask,#背景图片
 background_color='white', #背景颜色
 max_font_size=25,
 max_words=200,
 stopwords=STOPWORDS, #屏蔽的内容
)
```
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/1b2000ba7e0a4aa0b36a5dd64a86ff39)
生成词语,保存图片
```python
wc.generate(text) #制作词云
wc.to_file('新增图片.jpg') #保存到当地文件
```
**3 数据展示**
```python
plt.imshow(wc,interpolation='bilinear')
plt.axis('off')
plt.show()
```
**完整代码和效果展示**
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/cb8500a1ded84e969b9c9b1a11eeff04)
完整代码
图片一(未分词):
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/88708cd639a84dfea6cc0737db81396d)
图片二(分词效果)
![python 结巴分词学习](http://p1.pstatp.com/large/pgc-image/bf16339bccfb41e49ba66e76e6431ad9)
![python 结巴分词学习](http://p3.pstatp.com/large/pgc-image/8ed921b9a1e0455da26014f2ec3474e4)

