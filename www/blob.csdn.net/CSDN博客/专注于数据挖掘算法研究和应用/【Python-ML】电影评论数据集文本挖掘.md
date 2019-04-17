# 【Python-ML】电影评论数据集文本挖掘 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月22日 16:22:39[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1212
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月22日
@author: Jason.F
@summary: 文本挖掘，对电影评论进行内容抽取、特征向量化并训练模型预测
电影评论数据：http://ai.stanford.edu/~amaas/data/sentiment/
'''
import pyprind
import pandas as pd
import os
import numpy as np
import re
from nltk.stem.porter import PorterStemmer
import nltk
from nltk.corpus import stopwords
from sklearn.grid_search import GridSearchCV
from sklearn.pipeline import Pipeline
from sklearn.linear_model import LogisticRegression
from sklearn.feature_extraction.text import TfidfVectorizer
import time

start = time.clock()

homedir = os.getcwd()#获取当前文件的路径
#第一步：导入数据并输出到moive_data.csv
'''
pbar=pyprind.ProgBar(50000)
labels={'pos':1,'neg':0}#正面和负面评论标签
df = pd.DataFrame()
for s in ('test','train'):
    for l in ('pos','neg'):
        path=homedir+'/aclImdb/%s/%s' %(s,l)
        for file in os.listdir(path):
            with open(os.path.join(path,file),'r') as infile:
                txt =infile.read()
            df =df.append([[txt,labels[l]]],ignore_index=True)
            pbar.update()
df.columns=['review','sentiment']
np.random.seed(0)
df=df.reindex(np.random.permutation(df.index))#重排数据集，打散正负样本数据
df.to_csv(homedir+'/movie_data.csv',index=False)
'''
#第二步：文本数据清洗和特征向量化
df=pd.read_csv(homedir+'/movie_data.csv')
def preprocessor(text):
    text=re.sub('<[^>]*>','',text)#移除HTML标记，#把<>里面的东西删掉包括内容
    emotions=re.findall('(?::|;|=)(?:-)?(?:\)|\(|D|P)',text)
    text=re.sub('[\W]+',' ',text.lower())+''.join(emotions).replace('-','')
    return text
#print (preprocessor(df.loc[0,'review'][-50:]))#数据集第一行review字段的最后50个字符
#print (preprocessor("</a>This :) is :( a test :-)!"))
df['review']=df['review'].apply(preprocessor)
def tokenizer(text):#提取词汇
    return text.split()
porter=PorterStemmer()
def tokenizer_porter(text):#文本分词并提取词干
    return [porter.stem(word) for word in text.split()]
nltk.download('stopwords')#停用词移除(stop-word removal)，停用词是文本中常见单不能有效判别信息的词汇
stop = stopwords.words('english')#获得英文停用词集
#print ([w for w in tokenizer_porter('a runner likes running and runs a lot') if w not in stop])
#第三步：模型训练
X_train=df.loc[:25000,'review'].values
y_train=df.loc[:25000,'sentiment'].values
X_test=df.loc[25000:,'review'].values
y_test=df.loc[25000:,'sentiment'].values
tfidf=TfidfVectorizer(strip_accents=None,lowercase=False,preprocessor=None)
param_grid = [{'vect__ngram_range':[(1,1)],'vect__stop_words':[stop,None],'vect__tokenizer':[tokenizer,tokenizer_porter],'clf__penalty':['l1','l2'],'clf__C':[1.0,10.1,100.0]},\
              {'vect__ngram_range':[(1,1)],'vect__stop_words':[stop,None],'vect__tokenizer':[tokenizer,tokenizer_porter],'vect__use_idf':[False],'vect__norm':[None],'clf__penalty':['l1','l2'],'clf__C':[1.0,10.1,100.0]} ]
lr_tfidf =Pipeline([('vect',tfidf),('clf',LogisticRegression(random_state=0))])
gs_lr_tfidf=GridSearchCV(lr_tfidf,param_grid,scoring='accuracy',cv=5,verbose=1,n_jobs=-1)
gs_lr_tfidf.fit(X_train,y_train)
print ('Best parameter set :%s' % gs_lr_tfidf.best_params_)
print ('CV Accuracy:%.3f'%gs_lr_tfidf.best_score_)
clf=gs_lr_tfidf.best_estimator_
print ('Test Accuracy:%.3f'%clf.score(X_test,y_test))

end = time.clock()    
print('finish all in %s' % str(end - start))
```

结果：

![](https://img-blog.csdn.net/20180122162224407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




