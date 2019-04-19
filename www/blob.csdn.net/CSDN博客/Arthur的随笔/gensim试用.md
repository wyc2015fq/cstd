# gensim试用 - Arthur的随笔 - CSDN博客
2013年08月28日 12:22:14[largetalk](https://me.csdn.net/largetalk)阅读数：23823
## gensim试用[](#gensim)
gensim: [http://radimrehurek.com/gensim/index.html](http://radimrehurek.com/gensim/index.html)
Gensim is a free Python framework designed to automatically extract semantic topics from documents, as efficiently (computer-wise) and painlessly (human-wise) as possible.
### gensim安装[](#id1)
sudo apt-get install python-numpy python-scipy
pip install gensim
### lsi计算文档相似度[](#lsi)
先准备数据，我爬了约2w篇豆瓣日记作为这次试验的数据，数据和代码可以在这里https://github.com/largetalk/yaseg 找到。
主要代码如下：
importjiebafromgensimimportcorpora,models,similaritiesimportosimportrandomfrompprintimportpprintimportstringimportreRESULT_DIR='douban_result'regex=re.compile(ur"[^\u4e00-\u9f5aa-zA-Z0-9]")classDoubanDoc(object):def__init__(self,root_dir='douban'):self.root_dir=root_dirdef__iter__(self):fornameinos.listdir(self.root_dir):ifos.path.isfile(os.path.join(self.root_dir,name)):data=open(os.path.join(self.root_dir,name),'rb').read()title=data[:data.find('\r\n')]yield(name,title,data)classDoubanCorpus(object):def__init__(self,root_dir,dictionary):self.root_dir=root_dirself.dictionary=dictionarydef__iter__(self):forname,title,datainDoubanDoc(self.root_dir):yieldself.dictionary.doc2bow(jieba.cut(data,cut_all=False))defrandom_doc():name=random.choice(os.listdir('douban'))data=open('douban/%s'%name,'rb').read()print'random choice ',namereturnname,datatexts=[]forname,title,datainDoubanDoc():defetl(s):#remove 标点和特殊字符s=regex.sub('',s)returnsseg=filter(lambdax:len(x)>,map(etl,jieba.cut(data,cut_all=False)))texts.append(seg)# remove words that appear only onceall_tokens=sum(texts,[])token_once=set(wordforwordinset(all_tokens)ifall_tokens.count(word)==)texts=[[wordforwordintextifwordnotintoken_once]fortextintexts]dictionary=corpora.Dictionary(texts)corpus=list(DoubanCorpus('douban',dictionary))tfidf=models.TfidfModel(corpus)corpus_tfidf=tfidf[corpus]lsi=models.LsiModel(corpus_tfidf,id2word=dictionary,num_topics=)i=fortinlsi.print_topics():print'[topic #%s]: '%i,ti+=index=similarities.MatrixSimilarity(lsi[corpus])_,doc=random_doc()vec_bow=dictionary.doc2bow(jieba.cut(doc,cut_all=False))vec_lsi=lsi[vec_bow]print'topic probability:'pprint(vec_lsi)sims=sorted(enumerate(index[vec_lsi]),key=lambdaitem:-item[])print'top 10 similary notes:'pprint(sims[:])
一共有这么些步：
- 计算词袋(bag of word), 即这里的dictionary
- 计算corpus
- 训练TF-IDF模型
- 计算tf-idf向量
- 训练LSI模型
- 对文档用LSI模型分类并建立索引
- 查寻
### 结果[](#id2)
[topic #0]:  0.277*"我" + 0.268*"你" + 0.196*"的" + 0.165*"他" + 0.146*"了" + 0.138*"她" + 0.124*"是" + 0.116*"自己" + 0.111*"在" + 0.107*"人"
[topic #1]:  0.504*"the" + 0.303*"to" + 0.268*"and" + 0.265*"of" + 0.235*"I" + 0.235*"a" + 0.219*"you" + 0.178*"in" + 0.175*"is" + 0.139*"that"
[topic #2]:  -0.732*"你" + -0.172*"我" + -0.119*"爱" + 0.107*"的" + 0.088*"中国" + 0.076*"和" + 0.075*"年" + 0.065*"与" + 0.061*"他们" + 0.061*"中"
[topic #3]:  -0.620*"她" + -0.288*"他" + 0.281*"你" + -0.160*"我" + -0.099*"说" + -0.098*"了" + -0.092*"啊" + 0.089*"与" + 0.080*"的" + 0.067*"中国"
[topic #4]:  0.524*"她" + -0.264*"我" + 0.246*"他" + 0.186*"你" + -0.160*"啊" + -0.138*"了" + 0.110*"女人" + 0.097*"爱" + 0.095*"男人" + 0.093*"与"
[topic #5]:  -0.741*"他" + 0.459*"她" + 0.155*"你" + 0.097*"月" + 0.076*"日" + 0.072*"啊" + 0.068*"1" + 0.067*"2" + 0.062*"年" + -0.062*"我"
[topic #6]:  -0.367*"他" + -0.331*"你" + 0.188*"自己" + 0.140*"她" + 0.130*"生活" + -0.128*"啊" + -0.128*"月" + -0.119*"日" + -0.117*"1" + 0.116*"我"
[topic #7]:  0.162*"自己" + -0.153*"着" + -0.138*"在" + 0.120*"做" + -0.116*"它" + 0.113*"别人" + -0.112*"我们" + -0.112*"里" + 0.109*"工作" + 0.104*"啊"
[topic #8]:  0.521*"I" + 0.445*"you" + -0.386*"the" + -0.253*"of" + 0.190*"me" + 0.160*"my" + 0.144*"t" + 0.128*"love" + -0.113*"and" + 0.092*"your"
[topic #9]:  0.302*"說" + 0.198*"我們" + 0.193*"對" + 0.187*"來" + 0.181*"一個" + 0.166*"會" + 0.164*"於" + 0.156*"後" + 0.145*"沒" + 0.136*"為"
[topic #10]:  -0.300*"月" + -0.287*"日" + -0.215*"年" + -0.176*"爱" + -0.141*"2012" + 0.140*"啊" + -0.132*"2011" + -0.129*"他" + 0.124*"你" + -0.119*"11"
[topic #11]:  -0.547*"我" + 0.202*"爱情" + 0.189*"男人" + 0.186*"女人" + 0.174*"吃" + -0.141*"中国" + 0.125*"爱" + 0.123*"啊" + -0.107*"企业" + 0.092*"不要"
[topic #12]:  -0.376*"爱" + -0.290*"啊" + -0.240*"爱情" + 0.194*"孩子" + 0.183*"妈妈" + -0.153*"或者" + -0.140*"我" + 0.131*"你" + -0.127*"女人" + -0.124*"男人"
[topic #13]:  0.264*"啊" + -0.245*"爱" + -0.231*"或者" + -0.188*"妈妈" + -0.178*"吃" + -0.177*"那里" + -0.176*"孩子" + -0.167*"我" + -0.119*"不念" + -0.118*"不增"
[topic #14]:  -0.349*"孩子" + -0.300*"妈妈" + -0.244*"我们" + -0.220*"啊" + -0.206*"你们" + 0.204*"喜欢" + -0.179*"他们" + -0.131*"父母" + -0.130*"爸爸" + 0.119*"他"
[topic #15]:  0.322*"我们" + -0.210*"孩子" + 0.161*"爱情" + -0.152*"日" + 0.148*"企业" + -0.145*"月" + 0.138*"客户" + 0.133*"元" + 0.126*"产品" + -0.123*"或者"
[topic #16]:  0.347*"我" + -0.249*"我们" + -0.212*"或者" + 0.188*"女人" + 0.165*"男人" + -0.165*"那里" + -0.116*"工作" + -0.111*"不见" + -0.110*"不念" + -0.109*"不增"
[topic #17]:  0.281*"妈妈" + -0.257*"女人" + -0.251*"男人" + 0.239*"豆瓣" + 0.239*"爱" + 0.231*"孩子" + 0.212*"喜欢" + 0.130*"啊" + 0.128*"电影" + -0.125*"月"
[topic #18]:  0.404*"啊" + -0.325*"男人" + -0.324*"女人" + -0.202*"喜欢" + -0.165*"豆瓣" + -0.136*"电影" + 0.116*"她" + -0.109*"孩子" + -0.104*"妈妈" + 0.100*"他"
[topic #19]:  -0.357*"我们" + 0.254*"啊" + -0.192*"你们" + 0.163*"女人" + 0.152*"企业" + 0.146*"男人" + -0.139*"喜欢" + -0.136*"吃" + 0.120*"自己" + -0.113*"他们"
[topic #20]:  -0.312*"豆瓣" + 0.259*"爱情" + 0.219*"妈妈" + -0.218*"你们" + 0.179*"中国" + -0.169*"男人" + -0.168*"女人" + 0.160*"爱" + -0.153*"您" + -0.138*"我们"
[topic #21]:  0.395*"爱情" + -0.341*"喜欢" + 0.231*"豆瓣" + -0.171*"啊" + -0.143*"中国" + -0.143*"元" + -0.135*"人" + -0.112*"你们" + 0.110*"阅读" + 0.106*"了"
[topic #22]:  -0.304*"你们" + 0.296*"爱情" + 0.288*"孩子" + -0.240*"吃" + -0.220*"2012" + -0.167*"爱" + -0.158*"豆瓣" + -0.135*"一年" + 0.113*"他们" + 0.092*"元"
[topic #23]:  0.305*"我们" + 0.261*"妈妈" + -0.237*"爱" + -0.189*"爱情" + 0.188*"女人" + -0.160*"他们" + -0.159*"工作" + 0.140*"男人" + -0.126*"孩子" + -0.123*"我"
[topic #24]:  0.275*"爱" + -0.269*"啊" + 0.240*"豆瓣" + 0.231*"中国" + -0.213*"爱情" + -0.182*"工作" + -0.159*"喜欢" + -0.155*"我" + -0.123*"生活" + -0.109*"2012"
[topic #25]:  0.355*"你们" + -0.210*"我们" + 0.205*"孩子" + -0.166*"妈妈" + 0.142*"2012" + -0.139*"我" + -0.134*"啊" + -0.128*"爱" + -0.110*"电影" + -0.109*"人生"
[topic #26]:  -0.304*"豆瓣" + 0.277*"孩子" + -0.270*"妈妈" + -0.168*"日" + -0.166*"他们" + 0.150*"2012" + -0.132*"您" + -0.130*"月" + -0.126*"元" + -0.113*"生活"
[topic #27]:  -0.361*"元" + -0.214*"您" + 0.188*"豆瓣" + 0.172*"啊" + 0.167*"喜欢" + 0.141*"他们" + 0.117*"月" + 0.115*"日" + -0.114*"原价" + 0.112*"你们"
[topic #28]:  -0.340*"2012" + 0.321*"你们" + -0.315*"您" + -0.226*"爱" + 0.195*"爱情" + -0.168*"我们" + 0.163*"中国" + 0.151*"妈妈" + -0.133*"孩子" + -0.115*"它"
[topic #29]:  0.276*"你们" + 0.245*"妈妈" + 0.219*"2012" + -0.186*"孩子" + -0.162*"豆瓣" + -0.156*"吃" + -0.154*"中国" + -0.143*"生活" + 0.131*"电影" + -0.113*"啊"
这是分类出来的30个topic, 看起来区分度不大，这估计和豆瓣本身特质相光。
