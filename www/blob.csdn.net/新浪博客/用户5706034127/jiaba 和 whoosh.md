# jiaba 和 whoosh_用户5706034127_新浪博客
||分类：[自然语言处理](http://blog.sina.com.cn/s/articlelist_5706034127_3_1.html)|
print(t.text)
注意到分词器用的是jiaba。 ***from jieba.analyse import ChineseAnalyzeranalyzer = ChineseAnalyzer()*** schema =
Schema(title=TEXT(stored=True), path=ID(stored=True),
content=TEXT(stored=True, ***analyzer=analyzer***))
