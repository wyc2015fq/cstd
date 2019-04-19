# textRank,,类似于pageRank_用户5706034127_新浪博客
||分类：[自然语言处理](http://blog.sina.com.cn/s/articlelist_5706034127_3_1.html)|
### 类似于pageRank，给一个文档中的单词排序
### 基于 TextRank 算法的关键词抽取
- 
jieba.analyse.textrank(sentence, topK=20, withWeight=False,
allowPOS=('ns', 'n', 'vn', 'v')) 直接使用，接口相同，注意默认过滤词性。
- 
jieba.analyse.TextRank() 新建自定义 TextRank 实例
算法论文： [TextRank:
Bringing Order into Texts](http://web.eecs.umich.edu/~mihalcea/papers/mihalcea.emnlp04.pdf)
#### [](https://github.com/fxsjy/jieba/#%E5%9F%BA%E6%9C%AC%E6%80%9D%E6%83%B3)基本思想:
- 将待抽取关键词的文本进行分词
- 
以固定窗口大小(默认为5，通过span属性调整)，词之间的共现关系，构建图
- 
计算图中节点的PageRank，注意是无向带权图
