# 【NLP】基于CRF条件随机场的命名实体识别原理详解 - zkq_1986的博客 - CSDN博客





2018年07月30日 14:27:35[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：2096








## 1. 命名实体用来做什么？

在自然语言处理应用领域中，命名实体识别是信息检索、知识图谱、机器翻译、情感分析、问答系统等多项自然语言处理应用的基础任务，例如，我们需要利用命名实体识别技术自动识别用户的查询，然后将查询中的实体链接到知识图谱对应的结点上其识别的准确率将会直接影响到后续的一系列工作。

## 2. 命名实体识别有哪些难点？
- 命名实体在不同领域或不同场景下的识别具有较大的差异。目前已标注的语料通常局限于某些领域，难以适用于其他语料，例如：基于新闻语料进行训练，然后在社交语料进行测试，测试的结果往往难以达到理想的效果，因为社交语料中存在大量非规范的用语。
- 命名实体识别标注成本较大，目前命名实体识别标注语料较少，如何从较少的语料中学习到较好的模型，或者借助其他相似任务语料以及大量未标记的语料进行学习，这给命名实体识别提出了新的挑战。
- 中文命名实体识别中“字”的边界是确定的，但是“词”的边界是模糊的，因此通常会出现一些语义理解歧义的情况，例如：“让人大吃一惊”这句话存在两种分词方案，“让／人大／吃一惊”和“让人／大吃一惊”，这两种分词方案的语句意思完全不一样。中文命名实体识别通常要与中文分词、浅层语法分析等过程相结合，而分词、语法分析的准确率直接影响了命名实体识别的效果。
- 待识别的文本中存在着大量的未登录词，即新的实体词，随着时间的推移，我们难以维护这些新词。

## 3 现有研究

基于统计模型的命名实体识别方法归纳

![](https://img-blog.csdn.net/20180730142044124?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 4 CRF（ Conditional Random Fields，条件随机场）

### 4.1 条件随机场介绍

![](https://img-blog.csdn.net/20180730142238839?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730142339691?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

四种模型对比

在给定观察序列X时，某个特定标记序列Y的概率可以定义为

![](https://img-blog.csdn.net/20180730142448962?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730142458785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730142521249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 4.2 CRF的参数估计

![](https://img-blog.csdn.net/20180730142608856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730142918498?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730142950274?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730143019440?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730143037219?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730143052299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730143119448?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730143134699?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730143147898?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 4.3 预测

![](https://img-blog.csdn.net/20180730143238429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180730143606858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 5 实验

1998年人民日报测试语料
||#sentence|#PER|#LOC|#ORG|
|----|----|----|----|----|
|train|46364|17615|36517|20571|
|test|4365|1973|2877|1331|

![](https://img-blog.csdn.net/2018073014265897?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



