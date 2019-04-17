# 用HMM实现中文分词 - qq_38593211的博客 - CSDN博客





2018年08月13日 23:47:58[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：465








## 1.jieba分词

    jieba分词的三种分词模式：
- 精确模式，试图将句子最精确地切开，适合文本分析；
- 全模式，把句子中所有的可以成词的词语都扫描出来, 速度非常快，但是不能解决歧义；
- 搜索引擎模式，在精确模式的基础上，对长词再次切分，提高召回率，适合用于搜索引擎分词。

    jieba分词的三种算法：
- 基于Trie树结构实现高效的词图扫描，生成句子中汉字所有可能成词情况所构成的有向无环图（DAG)，根据给定的词典进行查词典操作, 生成几种可能的句子切分。jieba的作者在DAG中记录的是句子中某个词的开始位置，从0到n-1(n为句子的长度), 每个开始位置作为字典的键, value是个list, 其中保存了可能的词语的结束位置，(通过查字典得到词, 开始位置+词语的长度得到结束位置)，注：所以可以联想到，jieba支持全模式分词，能把句子中所有的可以成词的词语都扫描出来
- 采用了动态规划查找最大概率路径, 找出基于词频的最大切分组合，查找待分词句子中已经切分好的词语, 对该词语查找该词语出现的频率(次数/总数), 根据动态规划查找最大概率路径的方法, 对句子从右往左反向计算最大概率
- 对于未登录词，采用了基于汉字成词能力的HMM模型，使用了Viterbi算法
- 
基于Viterbi算法做词性标注；

- 
基于tf-idf和textrank模型抽取关键词；


## 2.HMM实现

> 
第一部分是训练，有详细注释,所需要的训练语料：[人民日报中文语料库](https://download.csdn.net/download/qq_38593211/10601754)


```python
#encoding:utf-8
import sys
import math

A_dic = {}#状态转移概率矩阵
B_dic = {}#观测概率矩阵
Count_dic = {}#用于记录所有B、M、E、S的数量
Pi_dic = {}#初始概率矩阵，即记录了每个字是BMSE的概率
word_set = set()#所有词语的集合
state_list = ['B','M','E','S']
line_num = -1
INPUT_DATA = "trainCorpus.txt_utf8"
INPUT_DATA = "trainCorpus.txt_utf8"
PROB_START = "prob_start.py"   #初始状态概率
PROB_EMIT = "prob_emit.py"     #发射概率
PROB_TRANS = "prob_trans.py"   #转移概率

#初始化字典，初始化的矩阵A全部为0
def init():
    for state in state_list:
        A_dic[state]={}
        for state1 in state_list:
            A_dic[state][state1]=0.0
    for state in state_list:
        Pi_dic[state]=0.0
        B_dic[state]={}
        Count_dic[state]=0

#输出观测状态
def getList(input_str):
    outpout_str = []
    if len(input_str) == 1:
        outpout_str.append('S')
    elif len(input_str) == 2:
        outpout_str = ['B','E']
    else:
        M_num = len(input_str) -2
        M_list = ['M'] * M_num
        outpout_str.append('B')
        outpout_str.extend(M_list)  #把M_list中的'M'分别添加进去
        outpout_str.append('E')
    return outpout_str

#输出模型的三个矩阵
def Output():
    start_fp=open(PROB_START,'w')
    emit_fp=open(PROB_EMIT,'w')
    trans_fp=open(PROB_TRANS,'w')
    for key in Pi_dic:           #状态的初始概率
        Pi_dic[key]=float(Pi_dic[key])/ line_num
    print (Pi_dic,file=start_fp)
    for key in A_dic:            #状态转移概率
        for key1 in A_dic[key]:
            A_dic[key][key1]=A_dic[key][key1]/Count_dic[key]
    print (A_dic,file=trans_fp)
    for key in B_dic:            #观测概率，
        for word in B_dic[key]:
            B_dic[key][word] =B_dic[key][word]/Count_dic[key]
    print (B_dic,file=emit_fp)
    start_fp.close()
    emit_fp.close()
    trans_fp.close()

def main():
    ifp=open(INPUT_DATA,'rb')
    init()
    global word_set 
    global line_num
    for line in ifp.readlines():
        line_num+=1
        line=line.strip()
        if not line:continue
        line = line.decode("utf-8","ignore")
        word_list = []
        for i in range(len(line)):
            if line[i] == " ":continue
            word_list.append(line[i])
        word_set = word_set | set(word_list)   #训练预料库中所有字的集合
        lineArr=line.split(" ")#每一行的字符串
        line_state=[]#用于存放每个字符的BMSE属性
        for item in lineArr:
            line_state.extend(getList(item))
        for i in range(len(line_state)):#遍历这个字符串的所有BMSE
            if i==0:
                Pi_dic[line_state[0]]+=1
                Count_dic[line_state[0]]+= 1   #记录每一个状态的出现次数
            else:
                A_dic[line_state[i-1]][line_state[i]] += 1    #用于计算转移概率
                Count_dic[line_state[i]] += 1
                if word_list[i] not in B_dic[line_state[i]]:
                    B_dic[line_state[i]][word_list[i]] = 0.0
                else:
                    B_dic[line_state[i]][word_list[i]] += 1   #用于计算发射概率
    Output()
    ifp.close()

if __name__=="__main__":
    main()
```

> 
上一部分代码是用训练数据集求得了HMM的三要素，即初始概率矩阵（语料中第一个字是BMSE的概率），转移矩阵（B到E之类这种的转移概率），以及发射矩阵每个字是BMSE的概率，接下来就用这三个矩阵来进行分词测试。


```python
# -*- coding: utf-8 -*-
def load_model(f_name):
    ifp=open(f_name,'rb').read()
    ifp=ifp.decode('GB2312',"ignore")
    return eval(ifp)

import chardet
file=open("prob_emit.py",'rb').read()
f_charInfo=chardet.detect(file)
print(f_charInfo)
prob_start = load_model("prob_start.py")
prob_trans = load_model("prob_trans.py")
prob_emit = load_model("prob_emit.py")

def viterbi(obs, states, start_p, trans_p, emit_p): 
    V = [{}]
    path = {}
    for y in states:   #初始值
        V[0][y] = start_p[y] * emit_p[y].get(obs[0],0)  
        path[y] = [y]
    for t in range(1,len(obs)):
        V.append({})
        newpath = {}
        for y in states:      #从y0 -> y状态的递归
            (prob, state) = max([(V[t-1][y0] * trans_p[y0].get(y,0) * emit_p[y].get(obs[t],0) ,y0) for y0 in states if V[t-1][y0]>0])
            V[t][y] =prob
            newpath[y] = path[state] + [y]
        path = newpath  #记录状态序列
    (prob, state) = max([(V[len(obs) - 1][y], y) for y in states])  #在最后一个位置，以y状态为末尾的状态序列的最大概率
    return (prob, path[state])  #返回概率和状态序列
            

def cut(sentence):
    prob, pos_list =  viterbi(sentence,('B','M','E','S'), prob_start, prob_trans, prob_emit)
    return (prob,pos_list)
```





