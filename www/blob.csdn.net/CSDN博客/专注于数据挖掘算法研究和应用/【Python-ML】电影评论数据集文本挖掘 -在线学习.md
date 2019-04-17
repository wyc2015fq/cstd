# 【Python-ML】电影评论数据集文本挖掘 -在线学习 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月22日 16:23:39[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：994
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月22日
@author: Jason.F
@summary: 文本挖掘，对电影评论进行内容抽取、特征向量化并训练模型预测，在线学习并持久化模型
电影评论数据：http://ai.stanford.edu/~amaas/data/sentiment/
'''
import pyprind
import pandas as pd
import os
import numpy as np
import re
import time
import pickle
from nltk.corpus import stopwords
from sklearn.feature_extraction.text import HashingVectorizer
from sklearn.linear_model import SGDClassifier

start = time.clock()

homedir = os.getcwd()#获取当前文件的路径
#导入数据并输出到moive_data.csv
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
#文本向量化，并训练模型和更新
df=pd.read_csv(homedir+'/movie_data.csv')
stop = stopwords.words('english')#获得英文停用词集
def tokenizer(text):
    text=re.sub('<[^>]*>','',text)#移除HTML标记，#把<>里面的东西删掉包括内容
    emotions=re.findall('(?::|;|=)(?:-)?(?:\)|\(|D|P)',text)
    text=re.sub('[\W]+',' ',text.lower())+' '.join(emotions).replace('-','')
    tokenized = [w for w in text.split() if w not in stop]
    return tokenized
def stream_docs(path):
    with open(path,'r') as csv:
        next(csv) #skip header
        for line in csv:
            text,label = line[:-3] ,int(line[-2])
            yield text,label
def get_minibatch(doc_stream,size):
    docs,y =[],[]
    try:
        for _ in range(size):
            text,label =next(doc_stream)
            docs.append(text)
            y.append(label)
    except StopIteration:
        return None,None
    return docs,y
vect=HashingVectorizer(decode_error='ignore',n_features=2**21,preprocessor=None,tokenizer=tokenizer)
clf = SGDClassifier (loss='log',random_state=1,n_iter=1)#随机梯度下降，每次用一个样本更新权重
doc_stream = stream_docs(path=homedir+'/movie_data.csv')
pbar = pyprind.ProgBar(45)
classes=np.array([0,1])
for _ in range(45):
    X_train,y_train = get_minibatch(doc_stream, size=1000)
    if not X_train:break
    X_train = vect.transform(X_train)
    clf.partial_fit(X_train, y_train, classes=classes)#部分训练
    pbar.update()
#测试
X_test,y_test=get_minibatch(doc_stream, size=5000)
X_test=vect.transform(X_test)
print ('Accuracy:%.3f' %clf.score(X_test,y_test))
clf=clf.partial_fit(X_test,y_test)#更新模型
#持久化模型
dest=os.path.join('pkl_objects')
if not os.path.exists(dest):os.makedirs(dest)
pickle.dump(stop,open(os.path.join(dest,'stopwords.pkl'),'wb'),protocol=2)#保存停用词
pickle.dump(clf,open(os.path.join(dest,'classifier.pkl'),'wb'),protocol=2)#保存模型
#导入模型预测
clf =pickle.load(open(os.path.join('pkl_objects','classifier.pkl'),'rb'))
label ={0:'negative',1:'positive'}
example=['I love this movie']
X=vect.transform(example)
print ('Prediction:%s \nProbability:%.2f%%'%(label[clf.predict(X)[0]],np.max(clf.predict_proba(X))*100))

end = time.clock()    
print('finish all in %s' % str(end - start))
```


结果：



```
Warning: No valid output stream.
Accuracy:0.867
Prediction:positive 
Probability:82.53%
finish all in 50.6331459967
```





