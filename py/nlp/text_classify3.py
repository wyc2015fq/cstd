
import os

def mylistdir(path):
    files = os.listdir(path)
    dirList=[]
    fileList=[]
    for f in files:
        if(os.path.isdir(path + '/' + f)):
            if(f[0] == '.'):
                pass
            else:
                dirList.append(f)
        if(os.path.isfile(path + '/' + f)):
            fileList.append(f)

    return dirList,fileList


def load_train_texts(path, ext):
    dirList,fileList = mylistdir(path)
    #print(dirList)
    #print(fileList)
    texts = []
    labels = []
    for d in dirList:
        path1 = path+'/'+d+'/'
        _,files = mylistdir(path1)
        for fn in files:
            if fn.endswith(ext):
                f=open(path1+fn,'rt',encoding='utf8')
                t=f.read()
                f.close()
                if len(texts) >100000:
                    return labels,texts
                texts.append(t)
                labels.append(d)

    return labels,texts

def load_text_texts(path1, ext):
    texts = []
    _,files = mylistdir(path1)
    for fn in files:
        if fn.endswith(ext):
            f=open(path1+fn,'rt',encoding='utf8')
            t=f.read()
            f.close()
            if len(texts) >100000:
                return labels,texts
            texts.append(t)
    return texts

path='../../doc/'

import os
import re
from jieba import lcut
from gensim import corpora
import jieba.posseg as pseg
from sklearn.naive_bayes import MultinomialNB

# º”‘ÿÕ£”√¥ ±Ì
stopwords = [line.rstrip() for line in open('stopwords.txt', 'r', encoding='utf-8')]


def words2onehot(words, dt):
    length = len(dt)
    vector = [0] * length
    for word in words:
        if word in dt:
            vector[dt[word]] += 1
    return vector

used_flags = ['v', 'a', 'ad']
used_flags = ['n']
filter_pattern = re.compile('[^\u4E00-\u9FD5]+')

def train(labels, texts):
    ls_of_words=[]
    for text1 in texts:
        #chinese_only = filter_pattern.sub('', text1[0:1024])
        chinese_only = text1[0:1024]
        word_list = pseg.cut(chinese_only)

        meaninful_words = []
        for word, flag in word_list:
            if (word not in stopwords) and (flag in used_flags):
                meaninful_words.append(word)
        ls_of_words.append(meaninful_words)

    dictionary = corpora.Dictionary(ls_of_words)
    dt = dictionary.token2id
    #print(dt)
    ls_of_wid = []
    for words in ls_of_words:
        ls_of_wid.append(words2onehot(words, dt))
    classifier = MultinomialNB()
    classifier.fit(ls_of_wid, labels)
    return (classifier,dt)

import pickle

def saveobj(fn, d):
    f = open(fn, 'wb')
    pickle.dump(d, f)
    f.close()

def loadobj(fn):
    f = open(fn, 'rb')
    d = pickle.load(f)
    f.close()
    return d

datafn='data.dat'

if os.path.exists(datafn):
    cas,dt,labels =loadobj(datafn)
else:
    labels,texts=load_train_texts(path, '.md')
    cas,dt=train(labels,texts)
    saveobj(datafn, (cas, dt,labels))


def featrue1(text1, dt):
    #chinese_only = filter_pattern.sub('', text1[0:1024])
    chinese_only = text1[0:1024]
    word_list = pseg.cut(chinese_only)

    meaninful_words = []
    for word, flag in word_list:
        if (word not in stopwords) and (flag in used_flags):
            meaninful_words.append(word)
    t = words2onehot(meaninful_words, dt)
    return t

def predict1(cas, text1, dt):
    ls_of_wid = [featrue1(text1, dt)]
    t = cas.predict_proba(ls_of_wid)
    return t


texts=load_text_texts(path, '.md')
import numpy as np
for text1 in texts:
    t = predict1(cas, text1, dt)[0]
    t1 = np.argsort(t)
    k = t1[-1]
    print(k, t[k], labels[k])
    print(t)
print(dt)
