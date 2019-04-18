# 【LDA学习系列】LDA-Python库 - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年05月22日 10:22:57[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：596
LDA-python库：https://github.com/lda-project/lda
API参考：https://pythonhosted.org/lda/api.html
![](https://img-blog.csdn.net/2018052210214342)
构建好doc-word矩阵，就可以直接调用接口，参考如下：
```python
#LDA 
    def matrix_topic(self,R,K=2):
        model = lda.LDA(n_topics=K, n_iter=100, random_state=0)
        model.fit(R)
        return model.doc_topic_,model.topic_word_
```
具体代码可在github上深入。
